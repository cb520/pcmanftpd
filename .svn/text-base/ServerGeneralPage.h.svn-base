#if !defined(AFX_SERVERGENERALPAGE_H__D0053FF9_CE64_4852_A8A1_8FBF293F0E4A__INCLUDED_)
#define AFX_SERVERGENERALPAGE_H__D0053FF9_CE64_4852_A8A1_8FBF293F0E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerGeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerGeneralPage dialog

class CServerGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CServerGeneralPage)

// Construction
public:
	CServerGeneralPage();
	~CServerGeneralPage();

// Dialog Data
	//{{AFX_DATA(CServerGeneralPage)
	enum { IDD = IDD_SERVER_GENERAL };
	UINT	m_MaxConPerIp;
	UINT	m_PasvPortMin;
	UINT	m_PasvPortMax;
	UINT	m_Port;
	UINT	m_MaxCon;
	BOOL	m_StartupWithGUI;
	BOOL	m_TrayIcon;
	UINT	m_MaxTryPerIp;
	UINT	m_MaxDSpeed;
	UINT	m_MaxUSpeed;
	BOOL	m_EnablePasv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CServerGeneralPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CServerGeneralPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERGENERALPAGE_H__D0053FF9_CE64_4852_A8A1_8FBF293F0E4A__INCLUDED_)
