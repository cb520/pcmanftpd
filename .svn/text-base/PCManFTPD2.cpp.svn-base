// PCManFTPD2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AppConfig.h"
#include "PCManFTPD2.h"
#include "FtpSock.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApp

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApp construction

CApp::CApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CApp object

CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CApp initialization

BOOL CApp::InitInstance()
{
	HWND hwnd = FindWindow( CMainFrm::m_WndClassName, NULL);
	if( hwnd )
	{
		ShowWindow( hwnd, SW_SHOW );
		return FALSE;
	}

	m_ResHandle = (HINSTANCE)LoadLibrary( "Lang.dll" );
	AfxSetResourceHandle(m_ResHandle);

	// Standard initialization
	GetModuleFileName(NULL, AppPath.GetBuffer(_MAX_PATH), MAX_PATH);
	AppPath.ReleaseBuffer();
	AppPath = AppPath.Left( AppPath.ReverseFind('\\')+1 );

	AppConfig.Load();

#if !defined(USE_SERVER_THREAD)
	if( !CFtpSock::Init() )
		return false;
#endif

	CMainFrm* pFrame = new CMainFrm;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	AfxInitRichEdit();

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	if( AppConfig.m_StartupWithGUI )
 	{
		pFrame->UpdateWindow();
		pFrame->ShowWindow(SW_SHOW);
	}

	pFrame->OnFileOpen();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnWebSite();
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP( CAboutDlg, CDialog )
	ON_BN_CLICKED(IDC_WEBSITE, OnWebSite)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}


// App command to run the dialog
void CApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CApp message handlers


int CApp::ExitInstance() 
{
#if !defined(USE_SERVER_THREAD)
	CFtpSock::Cleanup();
#endif

	AppConfig.Unload();

	FreeLibrary( (HMODULE)m_ResHandle );

	return CWinApp::ExitInstance();
}

void CAboutDlg::OnWebSite()
{
	ShellExecute( m_hWnd, "open", "http://pcman.ptt.cc/", NULL, NULL, SW_SHOWMAXIMIZED );
}
