// FtpDataCon.cpp: implementation of the CFtpDataCon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCManFTPD2.h"
#include "AppConfig.h"
#include "FtpCmdCon.h"
#include "FtpDataCon.h"
#include "FtpUser.h"

#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
USHORT CFtpDataCon::m_DataPort=0;
//DWORD CFtpDataCon::m_TotalMaxUSpeed = 0;
//DWORD CFtpDataCon::m_TotalMaxDSpeed = 0;
DWORD CFtpDataCon::m_TotalDBytesPerSec = 0;
DWORD CFtpDataCon::m_TotalUBytesPerSec = 0;

CFtpDataCon::CFtpDataCon(CFtpCmdCon* pCmdCon, bool IsPasv)
 : m_pCmdCon(pCmdCon), m_IsPasv(IsPasv), m_pUser(pCmdCon->GetUser())
{
	m_BytesPerSec = 0;
	m_BlockSize = AppConfig.m_BufferSize;
	m_Buffer = m_Block = NULL;
//	m_RealMaxSpeed = 0;
	m_TotalBytes =0;
	m_hFile = INVALID_HANDLE_VALUE;
	m_CanRecv = m_CanSend = false;
	m_TransferredBytes = 0;
	m_UserName = pCmdCon->GetUser()->m_UserName;
	m_IdleTime = 0;
	m_Type = FTPDATA_NONE;
	m_IsConnected = false;
}

CFtpDataCon::~CFtpDataCon()
{
	if( m_Buffer )
		delete []m_Buffer;
	Close();

	if( m_pCmdCon && m_pCmdCon->GetDataCon() == this )
		m_pCmdCon->SetDataCon(NULL);

}

void CFtpDataCon::OnConnect(int code)
{
//	Select( WM_FTP_DATA_SOCK, FD_WRITE|FD_READ|FD_CLOSE);
	if(m_pCmdCon)
		m_pCmdCon->ReplyDataConOpen();

	m_IsConnected = true;
}

void CFtpDataCon::OnClose(int code)
{
	// We don't need this since recv will return 0 to indicate connection closed.
	// delete this;
}

void CFtpDataCon::OnRecv(int code)
{
	m_CanRecv = true;

	if( m_Type != RECV_FILE )
		return;

	if( m_BytesPerSec >= m_pUser->m_MaxUSpeed || m_TotalUBytesPerSec >= AppConfig.m_MaxUSpeed )
		return;

	if( m_pCmdCon )
		m_pCmdCon->SetIdleTime(0);

	AppConfig.SetTransferIcon();

	DWORD actual_recv = Recv( m_Block, AppConfig.m_BufferSize );
	if( actual_recv == SOCKET_ERROR )
	{
		int e = WSAGetLastError();
		// The blocked socket will automatically emit FD_READ again once the socket 
		// can send data again. So don't generate FD_READ here.
		if( e == WSAEWOULDBLOCK )
			m_CanRecv = false;	// Don't send data until the socket is available next time.
		else	// Retry
		{
			Sleep(0);
			::PostMessage( m_SinkWnd, WM_FTP_DATA_SOCK, WPARAM(m_Sock), FD_READ );
		}
		return;
	}
	m_BytesPerSec += actual_recv;
	m_TotalUBytesPerSec += actual_recv;

	if( 0 == actual_recv )	// End of receiving
		EndDataTransfer();
	else
	{
		// Re-fill the 4k block buffer;
		DWORD bytes_written;
		if( ! WriteFile( m_hFile, m_Buffer, actual_recv, &bytes_written, NULL ) )
//			EndDataTransfer();// Error! Maybe the server runs out of disk space.
			Select( WM_FTP_DATA_SOCK, FD_READ );

//		::PostMessage( m_SinkWnd, WM_FTP_DATA_SOCK, WPARAM(m_Sock), FD_READ );
	}

	m_IdleTime = 0;
}

void CFtpDataCon::OnSend(int code)
{
	m_CanSend = true;

	if( m_BytesPerSec >= m_pUser->m_MaxDSpeed  || m_TotalDBytesPerSec >= AppConfig.m_MaxDSpeed )
		return;

	if( m_pCmdCon )
		m_pCmdCon->SetIdleTime(0);

	AppConfig.SetTransferIcon();

	DWORD bytes_this_time = AppConfig.m_BufferSize > m_BlockSize ? m_BlockSize : AppConfig.m_BufferSize;
	if( (m_BytesPerSec + bytes_this_time) > m_pUser->m_MaxDSpeed )
		bytes_this_time = (m_pUser->m_MaxDSpeed - m_BytesPerSec);

	DWORD actual_send = Send( m_Block, bytes_this_time );
	if( actual_send == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		// the blocked socket will automatically emit FD_WRITE again once the socket 
		// can send data again. So don't generate FD_WRITE here.
		if( err == WSAEWOULDBLOCK )
			m_CanSend =  false;	// Don't send data until the socket is available next time.
		else
		{
			delete this;
		}
		return;
	}
	m_BlockSize -= actual_send;
	m_BytesPerSec += actual_send;
	m_TotalDBytesPerSec += actual_send;

	m_Block += actual_send;
	if( 0 == m_BlockSize )
	{
		if( m_Type == SEND_FILE )	// File transfer
		{
			m_Block = m_Buffer;
			// Re-fill the 4k block buffer;
			if( ReadFile( m_hFile, m_Buffer, AppConfig.m_BufferSize, &m_BlockSize, NULL ) && m_BlockSize > 0 )
//				::PostMessage( m_SinkWnd, WM_FTP_DATA_SEND, 0, LPARAM(this) );
				::PostMessage( m_SinkWnd, WM_FTP_DATA_SOCK, WPARAM(m_Sock), FD_WRITE );
			else
				EndDataTransfer();
		}
		else	// File list
		{
			EndDataTransfer();
		}
	}
	else
	{
		::PostMessage( m_SinkWnd, WM_FTP_DATA_SOCK, WPARAM(m_Sock), FD_WRITE );
	}

	m_IdleTime = 0;
}

void CFtpDataCon::OnAccept(int code)
{
	sockaddr_in addr;
	int addrlen = sizeof(addr);
	memset( &addr, 0, sizeof(addr) );

	RemoveFromMap();	// remove old listeniing socket.
	SOCKET oldSock = m_Sock;

	Accept( this, (sockaddr*)&addr, &addrlen );
	// m_Sock is changed into new accepted socket.
	Select( WM_FTP_DATA_SOCK, FD_CLOSE);
	AddToMap();

	closesocket( oldSock );

	if(m_pCmdCon && m_Type != FTPDATA_NONE )
		m_pCmdCon->ReplyDataConOpen();

	m_IsConnected = true;
}

unsigned short CFtpDataCon::FindFreeDataPort()
{
	m_DataPort++;
	if( m_DataPort == AppConfig.m_Port || m_DataPort > AppConfig.m_PasvPortMax )
		m_DataPort=AppConfig.m_PasvPortMin;
	while( m_DataPort == AppConfig.m_Port /*|| m_DataPort == AppConfig.m_ControlPort*/ )
		m_DataPort++;
	return m_DataPort;
}

void CFtpDataCon::Init()
{
	m_DataPort = AppConfig.m_PasvPortMin ? (AppConfig.m_PasvPortMin-1) : AppConfig.m_PasvPortMin;
}

void CFtpDataCon::Close()
{
	if( !IsValid() )
		return;

	if( 1 == m_FtpDataConMap.GetCount() )// This is the last one data connection.
	{
		if( CFtpCmdCon::GetTotalUserCount() )
			AppConfig.SetUserIcon();
		else
			AppConfig.SetNormalIcon();
	}

	if( m_hFile != INVALID_HANDLE_VALUE )	// File transfer
	{
		CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}

	if( GetGUI() )
		GetGUI()->RemoveTransfer( this );

	RemoveFromMap();
	CFtpSock::Close();
}

void CFtpDataCon::OnTimer()
{
	m_IdleTime++;
	ULONG timeout = m_pUser->GetTimeOut();
	if( timeout && m_IdleTime > timeout )
	{
		delete this;
		return;
	}
	m_TransferredBytes += m_BytesPerSec;

	if( GetGUI() )
		GetGUI()->UpdateTransfer( this, m_BytesPerSec );

	m_BytesPerSec = 0;

	if( m_Type == RECV_FILE )
	{
//		if(m_CanRecv)
			OnRecv(0);
	}
/*	else if( m_Type & FTPDATA_SEND )
		if(m_CanSend)
			OnSend(0);
*/
	if( m_Type == RECV_FILE )
		Select( WM_FTP_DATA_SOCK, FD_READ );
	else
		Select( WM_FTP_DATA_SOCK, FD_WRITE|FD_CLOSE );
}

/*
void CFtpDataCon::ReCalcSpeedLimits()
{
	SOCKET key;
	CFtpDataCon* value;
	POSITION pos = m_FtpDataConMap.GetStartPosition();
	while(pos)
	{
		m_FtpDataConMap.GetNextAssoc( pos, key, (CFtpSock*&)value );
		if( value->m_Type == RECV_FILE )	// This conn is used to recv data.
		{
value->m_RealMaxSpeed = value->m_pUser->m_MaxUSpeed;
return;
			if( value->m_pUser->m_MaxUSpeed > 0 )
			{
				// Because m_TotalMaxDSpeed is the sum of all m_MaxDSpeeds,
				// m_TotalMaxDSpeed must > 0 when value->m_pUser->m_MaxDSpeed > 0.
				// So, I don't have to check whether m_TotalMaxDSpeed > 0 or not.
				if( m_TotalMaxUSpeed > AppConfig.m_MaxUSpeed && AppConfig.m_MaxUSpeed > 0 ) // If there is limit of total speed from server
				{
					float ratio = (float)value->m_pUser->m_MaxUSpeed / m_TotalMaxUSpeed;
					value->m_RealMaxSpeed = ratio * AppConfig.m_MaxUSpeed ;
				}
				else
					value->m_RealMaxSpeed = value->m_pUser->m_MaxUSpeed;
			}
			else
				value->m_RealMaxSpeed = -1;	// Maximize the speed of this conn. since it hass no speed limit.
		}
		else
		{
value->m_RealMaxSpeed = value->m_pUser->m_MaxDSpeed;
return;
			if( value->m_pUser->m_MaxDSpeed > 0 )
			{
				// Because m_TotalMaxDSpeed is the sum of all m_MaxDSpeeds,
				// m_TotalMaxDSpeed must > 0 when value->m_pUser->m_MaxDSpeed > 0.
				// So, I don't have to check whether m_TotalMaxDSpeed > 0 or not.
				if( m_TotalMaxDSpeed > AppConfig.m_MaxDSpeed && AppConfig.m_MaxDSpeed > 0 ) // If there is limit of total speed from server
				{
					float ratio = (float)value->m_pUser->m_MaxDSpeed / m_TotalMaxDSpeed;
					value->m_RealMaxSpeed = ratio * AppConfig.m_MaxDSpeed;
				}
				else
					value->m_RealMaxSpeed = value->m_pUser->m_MaxDSpeed;
			}
			else
				value->m_RealMaxSpeed = -1;	// Maximize the speed of this conn. since it hass no speed limit.
		}
	}
}
*/

void CFtpDataCon::EndDataTransfer()
{
/*	if( m_Type != RECV_FILE )	// This is a connection used to send data.
		m_TotalMaxDSpeed -= m_pUser->m_MaxDSpeed;
	else	// This is a connection used to receive data.
		m_TotalMaxUSpeed -= m_pUser->m_MaxUSpeed;
*/
	Close();

//	ReCalcSpeedLimits();

	if(m_pCmdCon)
		m_pCmdCon->EndDataTransfer();

}

void CFtpDataCon::InitDirListTransfer( CString dir_list)
{
	m_Type = SEND_LIST;
	m_FileList = dir_list;
	m_Block = (BYTE*)LPCTSTR(m_FileList);
	m_TotalBytes = m_BlockSize = m_FileList.GetLength();
	Select(WM_FTP_DATA_SOCK, FD_ACCEPT|FD_CONNECT|FD_WRITE|FD_CLOSE);
	if( IsPassive() )
	{
		if( IsConnected() && m_pCmdCon )
			m_pCmdCon->ReplyDataConOpen();
	}
	else
		Connect( GetAddress(), GetPort() );
}

bool CFtpDataCon::InitFileTransfer( enum FtpDataConType type, CString file_path, __int64 offset)
{
	m_Type = type;
	m_FilePath = file_path;
	m_Buffer = m_Block = new BYTE[ AppConfig.m_BufferSize ];
	m_BlockSize = AppConfig.m_BufferSize;
	m_TransferredBytes = 0;

	// Open the file for trnasfer
	if( m_Type == SEND_FILE )
	{
		m_hFile = CreateFile(m_FilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		Select(WM_FTP_DATA_SOCK, FD_WRITE|FD_ACCEPT|FD_CONNECT|FD_CLOSE);
		DWORD file_size_high;
		DWORD file_size_low = GetFileSize(m_hFile, &file_size_high);
		m_TotalBytes = __int64(file_size_high) << 32;
		m_TotalBytes |= file_size_low;
	}
	else
	{
		m_hFile = CreateFile(m_FilePath, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, 0);
		Select(WM_FTP_DATA_SOCK, FD_READ|FD_ACCEPT|FD_CONNECT|FD_CLOSE);
	}
	if(m_hFile == INVALID_HANDLE_VALUE )
		return false;

	if( offset )
	{
		DWORD file_offset_high;
		DWORD file_offset_low;
		if( m_Type == RECV_FILE )
		{
			file_offset_low= GetFileSize(m_hFile, &file_offset_high);
			offset = file_offset_high << 32;
			offset |= file_offset_low;
		}
		else
		{
			file_offset_high = DWORD(offset >> 32);
			file_offset_low = DWORD(offset & 0xffffffff);

			m_TotalBytes -= offset;
		}

		SetFilePointer(m_hFile, file_offset_low, (PLONG)&file_offset_high, FILE_BEGIN);
//		if( NO_ERROR != GetLastError() )
//			return false;

		if( m_Type == RECV_FILE )
			SetEndOfFile( m_hFile );
	}

	if( m_Type == SEND_FILE )
	{
		ReadFile( m_hFile, m_Buffer, m_BlockSize, &m_BlockSize, NULL );
	}
	GetGUI()->AddTransfer( this, m_pUser->m_UserName, m_Type == SEND_FILE, file_path );

	if( IsPassive() )
	{
		if( IsConnected() && m_pCmdCon )
			m_pCmdCon->ReplyDataConOpen();
	}
	else
		Connect( GetAddress(), GetPort() );

	return true;
}

void CFtpDataCon::OnAllConTimer()
{
	m_TotalDBytesPerSec = 0;
	m_TotalUBytesPerSec = 0;
}
