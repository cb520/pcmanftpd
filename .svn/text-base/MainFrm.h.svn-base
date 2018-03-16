// MainFrm.h : interface of the CMainFrm class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D0C4F22E_E48F_4E82_95DC_DF7740B18154__INCLUDED_)
#define AFX_MAINFRM_H__D0C4F22E_E48F_4E82_95DC_DF7740B18154__INCLUDED_

#include "ServerGeneralPage.h"	// Added by ClassView
#include "LogEdit.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Splitter.h"
#include "DlgLayout.h"	// Added by ClassView
#include "ServerLogPage.h"
#include "ServerMsgPage.h"
#include "ServerGeneralPage.h"
#include "ServerIPFilterPage.h"
#include "FtpServer.h"

class CMainFrm : public CFrameWnd
{
friend class CApp;

public:
	CMainFrm();
protected: 
	DECLARE_DYNAMIC(CMainFrm)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrm)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT OnNotifyIcon( WPARAM wp, LPARAM lp );
	void UpdateTransfer( void* id, DWORD speed/*, int ratio*/);
	void RemoveTransfer( void* id );
	void RemoveUser( void* id );
	void AddTransfer( void* id, CString user, bool download, CString file );
	void AddUser(void* id, CString user, CString address );
	static const char m_WndClassName[];
	void DisplayServerOnline(bool online);
	void WriteLog( COLORREF Color, const  char* Text );
	CLogEdit m_LogEdit;
	CListCtrl m_UserList;
	CListCtrl m_TransferList;
	virtual ~CMainFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;
	CSplitter    m_wndMainSplitter;
	CSplitter    m_wndBottomSplitter;

	BOOL	m_LogToFile;
	BOOL	m_LogToScreen;

// Generated message map functions
protected:
	CStatusBar m_wndStatusBar;

#if defined(USE_SERVER_THREAD)
	CWinThread* m_pServerThread;
#else
	CFtpServer m_Server;
#endif
//	CFindReplaceDialog* m_pFindDlg;
	afx_msg void OnRClickUserList(NMHDR* phdr, LRESULT* r );
	afx_msg void OnRClickTransferList(NMHDR* phdr, LRESULT* r );
	//{{AFX_MSG(CMainFrm)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* cs);
	afx_msg void OnUserlistKick();
	afx_msg void OnUpdateUserlistKick(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditClearAll();
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnViewEditServer();
	afx_msg void OnViewEditUser();
	afx_msg void OnUserListBan();
	afx_msg void OnViewLog();
	afx_msg void OnLogOpenFile();
	afx_msg void OnTransferlistKick();
	afx_msg void OnUpdateTransferlistKick(CCmdUI* pCmdUI);
	afx_msg void OnOpenMainWindow();
	afx_msg void OnAppExit();
	afx_msg void OnDestroy();
	afx_msg void OnTransferlistKickUser();
	afx_msg void OnUpdateTransferlistKickUser(CCmdUI* pCmdUI);
	afx_msg void OnTransferlistBan();
	afx_msg void OnUpdateTransferlistBan(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D0C4F22E_E48F_4E82_95DC_DF7740B18154__INCLUDED_)
