#if !defined(AFX_LOGVIEWER_H__CED69B87_541F_4371_A4C0_B0F349507685__INCLUDED_)
#define AFX_LOGVIEWER_H__CED69B87_541F_4371_A4C0_B0F349507685__INCLUDED_

#include "DlgLayout.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogViewer dialog

class CLogViewer : public CDialog
{
// Construction
public:
	CLogViewer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogViewer)
	enum { IDD = IDD_LOG_VIEWER };
	CListCtrl	m_UploadList;
	CListCtrl	m_DownloadList;
	CTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddFile(CString line, BOOL bDownload);
	CStringList m_Log;
	static DLGLAYOUT m_DlgLayoutData[];
	CDlgLayout m_DlgLayout;

	// Generated message map functions
	//{{AFX_MSG(CLogViewer)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGVIEWER_H__CED69B87_541F_4371_A4C0_B0F349507685__INCLUDED_)
