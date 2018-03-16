// ServerLogPage.cpp : implementation file
//

#include "stdafx.h"
#include "ServerLogPage.h"
#include "LogViewer.h"
#include "AppConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerLogPage property page

IMPLEMENT_DYNCREATE(CServerLogPage, CPropertyPage)

CServerLogPage::CServerLogPage() : CPropertyPage(CServerLogPage::IDD)
{
	//{{AFX_DATA_INIT(CServerLogPage)
	m_LogToFile = FALSE;
	m_LogToScreen = FALSE;
	m_ScreenMaxLine = 0;
	//}}AFX_DATA_INIT
}

CServerLogPage::~CServerLogPage()
{
}

void CServerLogPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerLogPage)
	DDX_Check(pDX, IDC_LOGTOFILE, m_LogToFile);
	DDX_Check(pDX, IDC_LOGTRANSFERTOFILE, m_LogTransferToFile);
	DDX_Check(pDX, IDC_LOGTOSCREEN, m_LogToScreen);
	DDX_Text(pDX, IDC_MAXLINE, m_ScreenMaxLine);
	DDV_MinMaxUInt(pDX, m_ScreenMaxLine, 0, 32767);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerLogPage, CPropertyPage)
	//{{AFX_MSG_MAP(CServerLogPage)
	ON_BN_CLICKED(IDC_VIEWLOG, OnViewLog)
	ON_BN_CLICKED(IDC_DELLOG, OnDelLog)
	ON_BN_CLICKED(IDC_DELTRANSFERLOG, OnDelTransferLog)
	ON_BN_CLICKED(IDC_VIEW_TRANSFERLOG, OnViewTransferLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerLogPage message handlers

void CServerLogPage::OnViewLog() 
{
	ShellExecute(GetParent()->GetSafeHwnd(),"open",AppConfig.GetServerLogPath(),NULL,AppPath,SW_SHOWMAXIMIZED);
}

void CServerLogPage::OnDelLog() 
{
//	((CMainFrm*)AfxGetApp()->m_pMainWnd);
}

void CServerLogPage::OnDelTransferLog() 
{
	// TODO: Add your control notification handler code here
	
}

void CServerLogPage::OnViewTransferLog() 
{
	CLogViewer viewer;
	viewer.DoModal();
}
