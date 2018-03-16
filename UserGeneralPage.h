#if !defined(AFX_USERGENERALPAGE_H__DB765752_55B9_4CDB_8C11_EEDC17AB671F__INCLUDED_)
#define AFX_USERGENERALPAGE_H__DB765752_55B9_4CDB_8C11_EEDC17AB671F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserGeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserGeneralPage dialog

class CUserDialog;

class CUserGeneralPage : public CPropertyPage
{
friend class CUserDialog;
// Construction
public:
	BOOL EndOK();
	CUserGeneralPage();
	~CUserGeneralPage();

// Dialog Data
	//{{AFX_DATA(CUserGeneralPage)
	enum { IDD = IDD_USER_GENERAL };
	CComboBox	m_GroupCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUserGeneralPage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CUserDialog* m_pDlg;
	// Generated message map functions
	//{{AFX_MSG(CUserGeneralPage)
	afx_msg void OnChangeName();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelEndOKGroup();
	afx_msg void OnFilePerm();
	afx_msg void OnChangePasswd();
	//}}AFX_MSG

/*	afx_msg void OnPasswdCheck();
	afx_msg void OnMaxUSpeedCheck();
	afx_msg void OnMaxDSpeedCheck();
	afx_msg void OnMaxConCheck();
	afx_msg void OnTimeOutCheck();
	afx_msg void OnChangePasswd();
	afx_msg void OnChangeMaxUSpeed();
	afx_msg void OnChangeMaxDSpeed();
	afx_msg void OnChangeMaxCon();
	afx_msg void OnChangeTimeOut();
*/
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERGENERALPAGE_H__DB765752_55B9_4CDB_8C11_EEDC17AB671F__INCLUDED_)
