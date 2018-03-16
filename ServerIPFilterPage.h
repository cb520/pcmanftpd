#if !defined(AFX_SERVERIPFILTERPAGE_H__CB6B6BB3_D605_4FA0_8A54_33793E06B64D__INCLUDED_)
#define AFX_SERVERIPFILTERPAGE_H__CB6B6BB3_D605_4FA0_8A54_33793E06B64D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerIPFilterPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerIPFilterPage dialog

class CServerIPFilterPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CServerIPFilterPage)

// Construction
public:
	void OnOK();
	CServerIPFilterPage();
	~CServerIPFilterPage();

// Dialog Data
	//{{AFX_DATA(CServerIPFilterPage)
	enum { IDD = IDD_SERVER_IPFILTER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CServerIPFilterPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DeleteIP(BOOL bAllow);
	void AddIP(BOOL bAllow);
	// Generated message map functions
	//{{AFX_MSG(CServerIPFilterPage)
	afx_msg void OnAddAllow();
	afx_msg void OnDelAllow();
	afx_msg void OnAddBan();
	afx_msg void OnDelBan();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERIPFILTERPAGE_H__CB6B6BB3_D605_4FA0_8A54_33793E06B64D__INCLUDED_)
