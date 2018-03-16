// LogEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LogEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogEdit

CLogEdit::CLogEdit()
{
}

CLogEdit::~CLogEdit()
{
}


BEGIN_MESSAGE_MAP(CLogEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CLogEdit)
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogEdit message handlers

void CLogEdit::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPoint pt;	GetCursorPos(&pt);
	CMenu* pMainMenu=AfxGetApp()->m_pMainWnd->GetMenu();
	pMainMenu->GetSubMenu(1)->GetSubMenu(5)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,AfxGetApp()->m_pMainWnd);
	CRichEditCtrl::OnRButtonDown(nFlags, point);
}

void CLogEdit::ScrollToEnd()
{
	int line = GetLineCount() - 1;
	int	pos = LineIndex( line ) + LineLength(line);
	SetSel( pos, pos );
	SendMessage( EM_SCROLLCARET, 0, 0 );
}
