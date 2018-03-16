#if !defined(AFX_PermDLG_H__680AA6D5_19B4_4BB9_AD93_BB0A2A11127D__INCLUDED_)
#define AFX_PermDLG_H__680AA6D5_19B4_4BB9_AD93_BB0A2A11127D__INCLUDED_

#include "Splitter.h"	// Added by ClassView
#include "DlgLayout.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PermDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserPermDlg dialog

class CUserDialog;
class CFtpFilePerm;
class CFtpUser;

class CUserPermDlg : public CDialog
{
// Construction

public:
//	int m_iFolderLnkImg;
	typedef	void (CUserPermDlg::*ENUMDIR_PROC)(CString,CString,CString);

	CUserPermDlg(CWnd* pParent, CFtpUser* pUser);   // standard constructor
	~CUserPermDlg();

	CFtpUser* m_pUser;
	void EnableEdit(BOOL bEnable);
	static CString GetFileName( CString Path, char sep);
	CImageList m_Img;

	int m_iFolderImg;
	void AddItem(CString RealPath, CString DispName );
	CString GetTreeItemPath(HTREEITEM item );
	CString GetListItemPath(int idx);

// Dialog Data
	//{{AFX_DATA(CUserPermDlg)
	enum { IDD = IDD_USER_PERM };
	CListCtrl	m_FileList;
	CTreeCtrl	m_DirTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserPermDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateDirContent( HTREEITEM hItem );
	void UpdateBtns();
	HTREEITEM FindTreeItem(HTREEITEM hParent, LPCTSTR Text);
	void OnSelChanged();
	CString m_VirtName;
	int m_ListSel;
	HTREEITEM m_TreeSel;
	CFtpFilePerm* m_pCurPerm;
	bool m_IsCurPermInherited;
	CString m_CurPath;
	void OnEnumFileList( CString Name, CString VirtPath, CString RealPath );
	HTREEITEM m_EnumParent;
	void OnExpandEnumSubDir(CString Name, CString VirtPath, CString RealPath);
	int EnumDirFiles( CString virt_path, ENUMDIR_PROC EnumProc, BOOL bDirOnly );
	void UpdateChecksDisplay();
	void UpdateChecksValue();

	static DLGLAYOUT m_DlgLayoutData[];
	CDlgLayout m_DlgLayout;
	CSplitter m_Splitter;

	// Generated message map functions
	//{{AFX_MSG(CUserPermDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddDir();
	afx_msg void OnAddVDir();
	afx_msg void OnAddFile();
	afx_msg void OnRemove();
	afx_msg void OnSelchangedDirTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnItemExpandingDirTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangedFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInheritChange();
	afx_msg void OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRename();
	afx_msg void OnModifyPath();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnKeydownFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDirTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PermDLG_H__680AA6D5_19B4_4BB9_AD93_BB0A2A11127D__INCLUDED_)
