// LogView.cpp : implementation of the CLogView class
//

#include "stdafx.h"
#include "PCManFTPD2.h"
#include "LogView.h"
#include "FtpServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogView

CLogView::CLogView()
{
	m_pServer=NULL;				
}

CLogView::~CLogView()
{
}


BEGIN_MESSAGE_MAP(CLogView,CWnd )
	//{{AFX_MSG_MAP(CLogView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLogView message handlers

BOOL CLogView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CLogView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	// Do not call CWnd::OnPaint() for painting messages
}


int CLogView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pServer = new CFtpServer();
	m_pServer->Start();
	return 0;
}

void CLogView::OnDestroy() 
{
	CWnd ::OnDestroy();
	
	if( m_pServer )
		delete m_pServer;
}
