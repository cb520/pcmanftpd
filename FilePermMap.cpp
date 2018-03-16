// FilePermMap.cpp: implementation of the CFilePermMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pcmanftpd2.h"
#include "FilePermMap.h"
#include "FtpFilePerm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilePermMap::CFilePermMap()
{

}

CFilePermMap::~CFilePermMap()
{

}

void CFilePermMap::RemoveAll()
{
	CString key;
	CFtpFilePerm* value = NULL;
	for( POSITION pos = GetStartPosition(); pos; )
	{
		GetNextAssoc( pos, key, (void*&)value );
		delete value;
	}
	CMapStringToPtr::RemoveAll();
}

CFilePermMap::operator =(CFilePermMap &src)
{
	RemoveAll();
	CString key;
	CFtpFilePerm* value;
	POSITION pos = src.GetStartPosition();
	while( pos )
	{
		src.GetNextAssoc( pos, key, (void*&)value );
		operator[](key) = new CFtpFilePerm(*value);
	}

}

void CFilePermMap::Remove(CFtpFilePerm* perm)
{
	RemoveKey(perm->m_VirtPath);
}
