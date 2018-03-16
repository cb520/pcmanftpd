// FtpSock.cpp: implementation of the CFtpSock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCManFTPD2.h"
#include "FtpSock.h"
#include "FtpServer.h"
#include "FtpCmdCon.h"
#include "FtpDataCon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static char SinkWndClassName[]  = "FtpSink";
CFtpServer* CFtpSock::m_pServer = NULL;
CFtpSockMap CFtpSock::m_FtpCmdConMap;
CFtpSockMap CFtpSock::m_FtpDataConMap;

CFtpSock::CFtpSock()
{

}

CFtpSock::~CFtpSock()
{

}

LRESULT CFtpSock::SinkWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
//	case WM_FTP_DATA_SEND:
//		reinterpret_cast<CFtpDataCon*>(lp)->OnSend(0);
//		break;
	case WM_FTP_DATA_SOCK:
		{
			CFtpDataCon* pDataCon = NULL;
			if( m_FtpDataConMap.Lookup( SOCKET(wp), (CFtpSock*&)pDataCon) )
			{
				int code = HIWORD(lp);
				switch( LOWORD(lp) )
				{
				case FD_READ:
					pDataCon->OnRecv(code);
					break;
				case FD_WRITE:
					pDataCon->OnSend(code);
					break;
				case FD_CLOSE:
					pDataCon->OnClose(code);
					break;
				case FD_CONNECT:
					pDataCon->OnConnect(code);
					break;
				case FD_ACCEPT:
					pDataCon->OnAccept(code);
					break;
				};
			}
		}
		break;
	case WM_FTP_CMD_SOCK:
		{
			CFtpCmdCon* pCmdCon = NULL;
			if( m_FtpCmdConMap.Lookup( SOCKET(wp), (CFtpSock*&)pCmdCon) )
			{
				int code = HIWORD(lp);
				switch( LOWORD(lp) )
				{
				case FD_READ:
					pCmdCon->OnRecv(code);
					break;
				case FD_CONNECT:
					pCmdCon->OnConnect(code);
					break;
				case FD_CLOSE:
					pCmdCon->OnClose(code);
				};
			}
		}
		break;
	case WM_FTP_SERVER_SOCK:
		switch( LOWORD(lp) )
		{
		case FD_ACCEPT:
			m_pServer->OnAccept(HIWORD(lp));
			break;
		case FD_CLOSE:
			m_pServer->OnClose(HIWORD(lp));
		};
		break;
	case WM_TIMER:
		{
			CFtpDataCon::OnAllConTimer();	// Run the timer procedure of the whole class.
			SOCKET key;
			CFtpSock* value;
			POSITION pos = m_FtpCmdConMap.GetStartPosition();
			while(pos)
			{
				m_FtpCmdConMap.GetNextAssoc( pos, key, value );
				value->OnTimer();
			}
			pos = m_FtpDataConMap.GetStartPosition();
			while(pos)
			{
				m_FtpDataConMap.GetNextAssoc( pos, key, value );
				value->OnTimer();
			}
			break;
		}
	case WM_FTP_SOCK_DESTROY:
		delete (CFtpSock*)lp;
		break;
	case WM_DESTROY:
		KillTimer( hwnd, 10 );
	};
	return DefWindowProc(hwnd,msg,wp,lp);
}

bool CFtpSock::Init()
{
	if( !CSock::Init() )
		return false;

	WNDCLASS wc;
	memset(&wc,0,sizeof(wc));
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpszClassName = SinkWndClassName;
	wc.lpfnWndProc = (WNDPROC)SinkWndProc;
	if( !RegisterClass(&wc) )
		return false;
	HWND wnd = CreateWindow( SinkWndClassName,NULL,0,0,0,0,0,HWND_DESKTOP,NULL,wc.hInstance,NULL);
	if(! wnd )
		return false;
	SetSinkWnd(wnd);
	SetTimer(wnd, 10, 1000, NULL );

	CFtpCmdCon::Init();
	CFtpDataCon::Init();

	return true;
}

void CFtpSock::Cleanup()
{
	SOCKET key;
	CFtpSock* value;
	POSITION pos = m_FtpCmdConMap.GetStartPosition();
	while(pos)
	{
		m_FtpCmdConMap.GetNextAssoc( pos, key, value );
		delete value;
	}
	m_FtpCmdConMap.RemoveAll();

	pos = m_FtpDataConMap.GetStartPosition();
	while(pos)
	{
		m_FtpDataConMap.GetNextAssoc( pos, key, value );
		delete value;
	}
	m_FtpDataConMap.RemoveAll();
}

void CFtpSock::OnTimer()
{

}


void CFtpSock::Destroy()
{
	delete this;
//	Close();
//	::PostMessage( m_SinkWnd, WM_FTP_SOCK_DESTROY, 0, LPARAM(this));
}
