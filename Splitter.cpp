// Splitter.cpp : implementation file
//

#include "stdafx.h"
#include "Splitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitter

CSplitter::CSplitter()
{
	m_Cur=NULL;
	m_bDrag=0;
	m_hFirstWnd=NULL;
	m_hSecondWnd=NULL;
	m_Pos=NULL;			
}

CSplitter::~CSplitter()
{
}


BEGIN_MESSAGE_MAP(CSplitter, CWnd)
	//{{AFX_MSG_MAP(CSplitter)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplitter message handlers

BOOL CSplitter::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( !CWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle=WS_EX_CONTROLPARENT;	cs.style=WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_TABSTOP;
	cs.lpszClass = AfxRegisterWndClass(0 /*, 0, HBRUSH(COLOR_BTNFACE+1)*/);
	return TRUE;
}

BOOL CSplitter::Create(CWnd* pParent, CWnd *pFirst, CWnd *pSecond, int Pos, BOOL Vert)
{
	if(!CWnd::Create(NULL,NULL,WS_VISIBLE,CRect(0,0,0,0),pParent,0))
		return FALSE;

	m_hFirstWnd=pFirst->m_hWnd;
	m_hSecondWnd=pSecond->m_hWnd;
	::SetParent(m_hFirstWnd, m_hWnd);
//	if( GetClassLong( m_hWnd, GCW_ATOM ) != GetClassLong( m_hFirstWnd, GCW_ATOM ) )
//		::SetWindowLong( m_hFirstWnd, GWL_EXSTYLE, ::GetWindowLong(m_hFirstWnd, GWL_EXSTYLE)|WS_EX_CLIENTEDGE);
	::SetParent(m_hSecondWnd, m_hWnd);
//	if( GetClassLong( m_hWnd, GCW_ATOM ) != GetClassLong( m_hSecondWnd, GCW_ATOM ) )
//		::SetWindowLong( m_hSecondWnd, GWL_EXSTYLE, ::GetWindowLong(m_hSecondWnd, GWL_EXSTYLE)|WS_EX_CLIENTEDGE);
	m_Pos=Pos;
	m_Cur=AfxGetApp()->LoadStandardCursor((m_bVert=Vert)?IDC_SIZEWE:IDC_SIZENS );
	return TRUE;
}

void CSplitter::PostNcDestroy() 
{
	if(m_Cur)	DestroyCursor(m_Cur);	
	CWnd::PostNcDestroy();
}

void CSplitter::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	ReSize(cx,cy);
}

void CSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
	if(!m_bDrag)
		return;

	CRect rc;	GetClientRect(rc);
	if(m_bVert)
	{
		if(point.x < 0 || point.x > rc.right-4 )
			return;
		m_Pos= point.x;
	}
	else
	{
		if(point.y < 0 || point.y > rc.bottom )
			return;
		m_Pos=point.y;
	}
	ReSize(rc.right, rc.bottom);
}

void CSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!IsPtInBar(point))
		return;

	m_bDrag=TRUE;
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void CSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bDrag=FALSE;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void CSplitter::ReSize(int cx, int cy)
{
	int l,t,w,h;
	if(m_bVert)
	{
		w=m_Pos;	h=cy;
		l=m_Pos+2;	t=0;
	}
	else
	{
		h=m_Pos;	w=cx;
		l=0;		t=m_Pos+2;
	}
	if(m_hFirstWnd)
		::MoveWindow(m_hFirstWnd,0,0,w,h,TRUE);

	if(m_bVert)
		w=cx-w-2;
	else
		h=cy-h-2;

	if(m_hSecondWnd)
		::MoveWindow(m_hSecondWnd,l,t,w,h,TRUE);
}

BOOL CSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
//	if( m_Drag )
	CPoint pt;
	GetCursorPos(&pt);	ScreenToClient(&pt);
	if(IsPtInBar(pt))
	{
		SetCursor(m_Cur);
	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	}
	return TRUE;
//	return CWnd::OnSetCursor(pWnd,nHitTest,message);
}

BOOL CSplitter::IsPtInBar(CPoint point)
{
	if(m_bVert)
	{
		if(point.x < m_Pos || point.x >= m_Pos+2 )
			return FALSE;
	}
	else
	{
		if(point.y < m_Pos || point.y >= m_Pos+2 )
			return FALSE;	
	}
	return TRUE;
}

BOOL CSplitter::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;	GetClientRect(rc);
	if(m_bVert)
	{
		rc.left=m_Pos;
		rc.right=rc.left+4;
	}
	else
	{
		rc.top=m_Pos;
		rc.bottom=rc.top+4;
	}
	::FillRect(pDC->m_hDC, rc, HBRUSH(COLOR_BTNFACE+1) );
	return TRUE;
}
