// FilePermMap.h: interface for the CFilePermMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPERMMAP_H__491D5116_B081_45F4_8928_80335C396F03__INCLUDED_)
#define AFX_FILEPERMMAP_H__491D5116_B081_45F4_8928_80335C396F03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>
#include <afxcoll.h>

class CFtpFilePerm;

class CFilePermMap : public CMapStringToPtr  
{
public:
	void Remove(CFtpFilePerm* perm);
	operator =(CFilePermMap &src);
	void RemoveAll();
	CFilePermMap();
	virtual ~CFilePermMap();

};

#endif // !defined(AFX_FILEPERMMAP_H__491D5116_B081_45F4_8928_80335C396F03__INCLUDED_)
