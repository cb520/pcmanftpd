// FtpFilePerm.cpp: implementation of the CFtpFilePerm class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "FtpFilePerm.h"
#include <mbstring.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpFilePerm::CFtpFilePerm(const char* VirtPath, const char* RealPath, 
	DWORD Perm) : m_VirtPath(VirtPath),m_RealPath(RealPath),m_Perm(Perm)
{
}

CFtpFilePerm::~CFtpFilePerm()
{
}

CString CFtpFilePerm::GetParentPath(const char *path)
{
    typedef const BYTE* LPCBYTE;
	BYTE slash= (*path == '/' ? '/' : '\\' );
	LPCBYTE first_slash=(BYTE*)_mbschr((BYTE*)(path),slash);
	LPCBYTE pstr=first_slash;
	LPCBYTE pslash=first_slash;
	while(pstr=_mbschr(pstr,slash))
	{
		pslash=pstr;
		pstr++;
	}
	if(pslash == first_slash)
		pslash++;
	CString spath;
	long len=(pslash-LPCBYTE(path));
	memcpy(spath.GetBuffer(len),path,len);
	spath.ReleaseBuffer();
	return spath;
}

