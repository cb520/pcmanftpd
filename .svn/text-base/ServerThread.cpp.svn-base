// ServerThread.cpp : implementation file
//

#include "stdafx.h"
#include "pcmanftpd2.h"
#include "ServerThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerThread

IMPLEMENT_DYNCREATE(CServerThread, CWinThread)

CServerThread::CServerThread()
{
	m_bAutoDelete = TRUE;
}

CServerThread::~CServerThread()
{
}

BOOL CServerThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	CFtpSock::Init();

	if( !m_Server.IsValid() )
		m_Server.Start();

	return TRUE;
}

int CServerThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if( m_Server.IsValid() )
		m_Server.Stop();

	CFtpSock::Cleanup();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CServerThread, CWinThread)
	//{{AFX_MSG_MAP(CServerThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerThread message handlers

int CServerThread::Run() 
{
	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		::DispatchMessage( &msg );
	}
	ExitInstance();
	return 0;
}
