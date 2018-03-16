#include<afxwin.h>
#include<shlobj.h>

#include "stdafx.h"

#ifndef	_BROWSE_DIALOG_H_INCLUDED
#define	_BROWSE_DIALOG_H_INCLUDED

class CBrowseDialog
{
protected :

	BROWSEINFO bi;
//	char *m_strTitle;
//	LPITEMIDLIST pidl;
//	HWND m_hWnd;

public :

	CBrowseDialog( CWnd* pParent , LPTSTR lpstrTitle);
	~CBrowseDialog();
	int DoModal();
	inline LPCTSTR GetPath()
	{	return bi.pszDisplayName;	}

};

#endif