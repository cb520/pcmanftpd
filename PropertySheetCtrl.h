#if !defined(AFX_PROPERTYSHEETCTRL_H__0D7DD7E5_161B_4D09_BDEF_FD217D72CCE2__INCLUDED_)
#define AFX_PROPERTYSHEETCTRL_H__0D7DD7E5_161B_4D09_BDEF_FD217D72CCE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertySheetCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetCtrl

class CPropertySheetCtrl : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetCtrl)

// Construction
public:
	CPropertySheetCtrl(){}
	CPropertySheetCtrl(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertySheetCtrl(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertySheetCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertySheetCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertySheetCtrl)
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYSHEETCTRL_H__0D7DD7E5_161B_4D09_BDEF_FD217D72CCE2__INCLUDED_)
