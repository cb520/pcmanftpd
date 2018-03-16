// ModalFrm.cpp : implementation file
//

#include "stdafx.h"
#include "ModalFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModalFrm

IMPLEMENT_DYNCREATE(CModalFrm, CFrameWnd)

CModalFrm::CModalFrm()
{
}

CModalFrm::~CModalFrm()
{
}


BEGIN_MESSAGE_MAP(CModalFrm, CFrameWnd)
	//{{AFX_MSG_MAP(CModalFrm)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModalFrm message handlers

BOOL CModalFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
//	cs.style = WS_POPUP|WS_CAPTION|WS_SYSMENU;
	return TRUE;
}


void CModalFrm::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	ShowWindow(SW_HIDE);
	((CFrameWnd*)AfxGetApp()->m_pMainWnd)->EndModalState();
//	AfxGetApp()->m_pMainWnd->SetActiveWindow();
}
