#include "stdafx.h"

#include "BrowseDialog.h"

int CBrowseDialog::DoModal()
{
	LPITEMIDLIST pidl = ::SHBrowseForFolder(&bi);
	if( pidl )
	{
		::SHGetPathFromIDList(pidl,bi.pszDisplayName);
		IMalloc* pmlc;
		SHGetMalloc(&pmlc);
		pmlc->Free(pidl);
		return IDOK;
	}
	return IDCANCEL;
}

CBrowseDialog::CBrowseDialog( CWnd* pParent , LPTSTR lpstrTitle = NULL )
{
	bi.hwndOwner = pParent->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = new char[MAX_PATH];
	bi.pszDisplayName[0] = 0;
	bi.lpszTitle = lpstrTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
}

CBrowseDialog::~CBrowseDialog()
{
	if(bi.pszDisplayName)
		delete []bi.pszDisplayName;
}
