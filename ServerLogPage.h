#if !defined(AFX_SERVERLOGPAGE_H__ED81290C_9A49_435E_A564_E695FC6521A5__INCLUDED_)
#define AFX_SERVERLOGPAGE_H__ED81290C_9A49_435E_A564_E695FC6521A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerLogPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerLogPage dialog

class CServerLogPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CServerLogPage)

// Construction
public:
	CServerLogPage();
	~CServerLogPage();

// Dialog Data
	//{{AFX_DATA(CServerLogPage)
	enum { IDD = IDD_SERVER_LOG };
	BOOL	m_LogToFile;
	BOOL	m_LogTransferToFile;
	BOOL	m_LogToScreen;
	UINT	m_ScreenMaxLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CServerLogPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CServerLogPage)
	afx_msg void OnViewLog();
	afx_msg void OnDelLog();
	afx_msg void OnDelTransferLog();
	afx_msg void OnViewTransferLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERLOGPAGE_H__ED81290C_9A49_435E_A564_E695FC6521A5__INCLUDED_)
