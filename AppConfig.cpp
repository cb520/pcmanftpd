// AppConfig.cpp: implementation of the CAppConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AppConfig.h"
#include "TrayIcon.h"
#include "FtpUser.h"

#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CString AppPath;

CString AppPath;
CAppConfig AppConfig;

CAppConfig::CAppConfig()
{
	m_LogFile = INVALID_HANDLE_VALUE;
	m_TransferLogFile = INVALID_HANDLE_VALUE;
}

CAppConfig::~CAppConfig()
{
	if(m_LogFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_LogFile);

	DestroyIcon(m_NormalIcon);
	DestroyIcon(m_UserIcon);
	DestroyIcon(m_TransferIcon);
}

void CAppConfig::Load()
{
	m_CurIcon = m_NormalIcon = AfxGetApp()->LoadIcon(IDI_ICON);
	m_OfflineIcon = AfxGetApp()->LoadIcon(IDI_OFFLINEICON);
	m_UserIcon = AfxGetApp()->LoadIcon(IDI_USERICON);
	m_TransferIcon = AfxGetApp()->LoadIcon(IDI_TRANSFERICON);

	CString CfgPath=AppPath+"Server.ini";
	const char* pCfgPath=CfgPath;

	m_StartupWithGUI=(BOOL)GetPrivateProfileInt("Server","StartupWithGUI",0,pCfgPath);
	m_TrayIcon=(BOOL)GetPrivateProfileInt("Server","TrayIcon",1,pCfgPath);

	m_Port=(USHORT)GetPrivateProfileInt("Server","Port",21,pCfgPath);
	m_EnablePasv=(BOOL)GetPrivateProfileInt("Server","EnablePasv",TRUE,pCfgPath);
	m_PasvPortMax=(USHORT)GetPrivateProfileInt("Server","PasvPortMax",2248,pCfgPath);
	m_PasvPortMin=(USHORT)GetPrivateProfileInt("Server","PasvPortMin",2048,pCfgPath);

	m_LogToFile=(BOOL)GetPrivateProfileInt("Server","LogToFile",1,pCfgPath);
	m_LogTransferToFile=(BOOL)GetPrivateProfileInt("Server","LogTransferToFile",1,pCfgPath);
	m_LogToScreen=(BOOL)GetPrivateProfileInt("Server","LogToScreen",1,pCfgPath);
	m_ScreenMaxLine=(BOOL)GetPrivateProfileInt("Server","ScreenMaxLine",500,pCfgPath);

	m_MaxCon=(UINT)GetPrivateProfileInt("Server","MaxCon",0,pCfgPath);
	m_MaxConPerIp=(UINT)GetPrivateProfileInt("Server","MaxConPerIp",0,pCfgPath);
	m_MaxTryPerIp=(UINT)GetPrivateProfileInt("Server","MaxTryPerIp",0,pCfgPath);

	m_MaxUSpeed=(UINT)GetPrivateProfileInt("Server","MaxUSpeed",0,pCfgPath);
	m_MaxUSpeed	= m_MaxUSpeed ? m_MaxUSpeed*1024 : DWORD(-1);
	m_MaxDSpeed=(UINT)GetPrivateProfileInt("Server","MaxDSpeed",0,pCfgPath);
	m_MaxDSpeed	= m_MaxDSpeed ? m_MaxDSpeed*1024 : DWORD(-1);

	m_BufferSize=(UINT)GetPrivateProfileInt("Server","BufferSize",4096,pCfgPath);
	m_WaitForLogin=(UINT)GetPrivateProfileInt("Server","WaitForLogin",180,pCfgPath);

//	CFtpData::m_DataPort=m_PasvPortMin;

	LoadWelcomeMsg();

	InitServerLog();
	InitTransferLog();

	LoadIPFilter();

	CFtpUser::LoadSettings();
}

void CAppConfig::SetNormalIcon()
{
	if( m_CurIcon != m_NormalIcon )
	{
		if( m_TrayIcon )
		{
			CTrayIcon ti(AfxGetMainWnd()->m_hWnd, 10);
			ti.Modify(m_NormalIcon,NULL);
		}
		m_CurIcon = m_NormalIcon;
	}
}

void CAppConfig::SetOfflineIcon()
{
	if( m_CurIcon != m_OfflineIcon )
	{
		if( m_TrayIcon )
		{
			CTrayIcon ti(AfxGetMainWnd()->m_hWnd, 10);
			ti.Modify(m_OfflineIcon,NULL);
		}
		m_CurIcon = m_OfflineIcon;
	}
}

void CAppConfig::SetUserIcon()
{
	if(m_CurIcon != m_UserIcon)
	{
		if(m_TrayIcon)
		{
			CTrayIcon ti(AfxGetMainWnd()->m_hWnd, 10);
			ti.Modify(m_UserIcon,NULL);
		}
		m_CurIcon = m_UserIcon;
	}
}

void CAppConfig::SetTransferIcon()
{
	if( m_CurIcon != m_TransferIcon )
	{
		if( m_TrayIcon )
		{
			CTrayIcon ti(AfxGetMainWnd()->m_hWnd, 10);
			ti.Modify(m_TransferIcon,NULL);
		}
		m_CurIcon = m_TransferIcon;
	}
}

void CAppConfig::LoadIPFilter()
{
	m_AllowedIP.RemoveAll();
	m_BannedIP.RemoveAll();
	HANDLE hFilter = CreateFile((AppPath+"IPFilter.ini"), GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_ALWAYS, 0, 0);
	if( hFilter != INVALID_HANDLE_VALUE )
	{
		DWORD len = GetFileSize(hFilter,NULL);
		char* buf = new char[len+1];
		ReadFile(hFilter, buf, len, &len, NULL);
		CloseHandle(hFilter);		buf[len]=0;
		const char crlf[] = "\r\n";
		char* pline = strtok(buf,crlf);
		while( pline )
		{
//			CArray<in_addr, in_addr>* pfilter = NULL;
			CDWordArray* pfilter = NULL;
			if(*pline == '+')
				pfilter = &m_AllowedIP;
			else if(*pline == '-' )
				pfilter = &m_BannedIP;

			if( pfilter )
			{
				pline++;
//				in_addr addr;	addr.S_un.S_addr = inet_addr(pline);
				unsigned long addr = inet_addr(pline);
//				if( addr.S_un.S_addr != INADDR_NONE )
				if( addr != INADDR_NONE )
					pfilter->Add(addr);
			}
			pline = strtok(NULL,crlf);
		}
		delete [] buf;
	}
}

void CAppConfig::SaveIPFilter()
{
//	in_addr* paddr;
//	in_addr* plastaddr;
	unsigned long* paddr;
	unsigned long* plastaddr;
	char line[64];
	HANDLE hFile = CreateFile((AppPath+"IPFilter.ini"), GENERIC_WRITE, FILE_SHARE_READ,
		0, CREATE_ALWAYS, 0, 0);
	if( AppConfig.m_BannedIP.GetSize())
	{
		*line = '-';
		for( paddr = m_BannedIP.GetData(), plastaddr = paddr + m_BannedIP.GetSize();
			paddr < plastaddr; paddr++)
		{
			in_addr _addr;
			_addr.S_un.S_addr = *paddr;
//			strcpy(line+1, inet_ntoa( *paddr ));
			strcpy(line+1, inet_ntoa( _addr ));
			strcat(line, "\r\n");
			DWORD len;
			WriteFile( hFile, line, strlen(line), &len,NULL);
		}
	}
	if( AppConfig.m_AllowedIP.GetSize())
	{
		*line = '+';
		for( paddr = m_AllowedIP.GetData(),	plastaddr = paddr + m_AllowedIP.GetSize();
			paddr < plastaddr; paddr++)
		{
			in_addr _addr;
			_addr.S_un.S_addr = *paddr;
//			strcpy(line+1, inet_ntoa( *paddr ));
			strcpy(line+1, inet_ntoa( _addr ));
			strcat(line, "\r\n");
			DWORD len;
			WriteFile( hFile, line, strlen(line), &len,NULL);
		}
	}
	CloseHandle( hFile );
}

void CAppConfig::Unload()
{
	CFtpUser::Unload();
}

void CAppConfig::Save()
{
	CString CfgPath=AppPath+"Server.ini";
	const char* pCfgPath=CfgPath;
	char buf[12];
	WritePrivateProfileString("Server","StartupWithGUI",itoa(m_StartupWithGUI,buf,10),pCfgPath);
	WritePrivateProfileString("Server","TrayIcon",itoa(m_TrayIcon,buf,10),pCfgPath);
	WritePrivateProfileString("Server","Port",itoa(m_Port,buf,10),pCfgPath);
	WritePrivateProfileString("Server","EnablePasv",itoa(m_EnablePasv,buf,10),pCfgPath);
	WritePrivateProfileString("Server","PasvPortMax",itoa(m_PasvPortMax,buf,10),pCfgPath);
	WritePrivateProfileString("Server","PasvPortMin",itoa(m_PasvPortMin,buf,10),pCfgPath);
	WritePrivateProfileString("Server","LogToFile",itoa(m_LogToFile,buf,10),pCfgPath);
	WritePrivateProfileString("Server","LogTransferToFile",itoa(m_LogToFile,buf,10),pCfgPath);
	WritePrivateProfileString("Server","LogToScreen",itoa(m_LogToScreen,buf,10),pCfgPath);
	WritePrivateProfileString("Server","ScreenMaxLine",itoa(m_ScreenMaxLine,buf,10),pCfgPath);
	WritePrivateProfileString("Server","MaxCon",itoa(m_MaxCon,buf,10),pCfgPath);
	WritePrivateProfileString("Server","MaxConPerIp",itoa(m_MaxConPerIp,buf,10),pCfgPath);
	WritePrivateProfileString("Server","MaxTryPerIp",itoa(m_MaxTryPerIp,buf,10),pCfgPath);
	WritePrivateProfileString("Server","BufferSize",itoa(m_BufferSize,buf,10),pCfgPath);
	WritePrivateProfileString("Server","WaitForLogin",itoa(m_WaitForLogin,buf,10),pCfgPath);
	WritePrivateProfileString("Server","MaxUSpeed",itoa(m_MaxUSpeed,buf,10),pCfgPath);
	WritePrivateProfileString("Server","MaxDSpeed",itoa(m_MaxDSpeed,buf,10),pCfgPath);

	CFile file;
	if( file.Open(AppPath+"WelcomeMsg.ini",CFile::modeWrite|CFile::modeCreate) )
	{
		file.Write(LPCTSTR(m_WelcomeMsg), m_WelcomeMsg.GetLength());
		file.Close();
	}
}

void CAppConfig::LoadWelcomeMsg()
{
	char buf[1024];

	m_WelcomeMsg="";
	HANDLE hMsg = CreateFile((AppPath+"WelcomeMsg.ini"), GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_ALWAYS, 0, 0);
	if( hMsg != INVALID_HANDLE_VALUE )
	{
		char* pstr=buf;		DWORD len;
		ReadFile(hMsg, buf, sizeof(buf)-1, &len, NULL);
		CloseHandle(hMsg);		buf[len]=0;
		const char crlf[]="\r\n";
		pstr = strtok(buf,crlf);
		do
		{
			char* pnext = strtok(NULL,crlf);
			m_WelcomeMsg += pnext ? "220-" : "220 ";
			if( pstr )
				m_WelcomeMsg += pstr;
			m_WelcomeMsg += crlf;
			pstr = pnext;
		}while( pstr );
	}
}

void CAppConfig::InitServerLog()
{
	if(m_LogFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_LogFile);

	m_LogFile=m_LogToFile ?
		CreateFile(GetServerLogPath(), GENERIC_WRITE, FILE_SHARE_READ,
			0, OPEN_ALWAYS, 0, 0) : INVALID_HANDLE_VALUE;

	if(m_LogFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(m_LogFile,0,NULL,FILE_END);
		SetEndOfFile(m_LogFile);
	}
}

void CAppConfig::InitTransferLog()
{
	if(m_TransferLogFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_TransferLogFile);

	m_TransferLogFile=m_LogTransferToFile ?
		CreateFile(GetTransferLogPath(), GENERIC_WRITE, FILE_SHARE_READ,
			0, OPEN_ALWAYS, 0, 0) : INVALID_HANDLE_VALUE;
	if(m_TransferLogFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(m_TransferLogFile,0,NULL,FILE_END);
		SetEndOfFile(m_TransferLogFile);
	}
}
