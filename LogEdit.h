#if !defined(AFX_LOGEDIT_H__56B64B32_2646_40B6_9517_EE28AEFCFCD1__INCLUDED_)
#define AFX_LOGEDIT_H__56B64B32_2646_40B6_9517_EE28AEFCFCD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogEdit window

class CLogEdit : public CRichEditCtrl
{
// Construction
public:
	CLogEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ScrollToEnd();
	virtual ~CLogEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogEdit)
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGEDIT_H__56B64B32_2646_40B6_9517_EE28AEFCFCD1__INCLUDED_)
