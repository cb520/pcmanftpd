#if !defined(AFX_SPLITTER_H__10977B65_2876_40B5_A187_C88392872507__INCLUDED_)
#define AFX_SPLITTER_H__10977B65_2876_40B5_A187_C88392872507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Splitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitter window

class CSplitter : public CWnd
{
// Construction
public:
	CSplitter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitter)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReSize(int cx,int cy);
	BOOL Create(CWnd* pParent, CWnd* pFirst, CWnd* pSecond, int Pos, BOOL Vert=TRUE);
	virtual ~CSplitter();

	// Generated message map functions
protected:
	BOOL IsPtInBar(CPoint point);
	BOOL m_bDrag;
	HWND m_hFirstWnd;
	HWND m_hSecondWnd;
	int m_Pos;
	HCURSOR m_Cur;
	BOOL m_bVert;
	//{{AFX_MSG(CSplitter)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTER_H__10977B65_2876_40B5_A187_C88392872507__INCLUDED_)
