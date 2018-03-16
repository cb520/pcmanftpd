#if !defined(AFX_USERDIALOG_H__7DE751E2_CFFB_459B_AA33_E411D253855F__INCLUDED_)
#define AFX_USERDIALOG_H__7DE751E2_CFFB_459B_AA33_E411D253855F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserDialog.h : header file
//

#include "UserGeneralPage.h"
#include "FtpUserEx.h"
#include "DlgLayout.h"	// Added by ClassView
#include "PropertySheetCtrl.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CUserDialog dialog

class CUserDialog : public CDialog
{
// Construction
public:
	CFtpUserEx* FindUser( CString name, bool is_user );
	int m_CurGroup;
	CImageList m_ImgList;
	CFtpUserEx* m_pUser;
	HTREEITEM m_groot;
	HTREEITEM m_uroot;
	void SaveUsers();
	void LoadUsers();
	void UpdateSelUser();
	CUserDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserDialog)
	enum { IDD = IDD_USER };
	CTabCtrl	m_Tab;
	CTreeCtrl	m_UserList;
	//}}AFX_DATA
	CPtrArray m_Users;
	CPtrArray m_Groups;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPropertySheet m_Sheet;
	CDialog* m_pCurPage;
	void AddUser(HTREEITEM hParent);
	CUserGeneralPage m_GeneralPage;

	// Generated message map functions
	//{{AFX_MSG(CUserDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void OnOK();
	afx_msg void OnAddUser();
	afx_msg void OnAddGroup();
	afx_msg void OnDelete();

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERDIALOG_H__7DE751E2_CFFB_459B_AA33_E411D253855F__INCLUDED_)
