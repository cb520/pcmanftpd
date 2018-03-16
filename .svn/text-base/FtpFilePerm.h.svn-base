// FtpFilePerm.h: interface for the CFtpFilePerm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPFILEACCESS_H__C1BEADAF_31B5_4D66_83D9_102B28B73525__INCLUDED_)
#define AFX_FTPFILEACCESS_H__C1BEADAF_31B5_4D66_83D9_102B28B73525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CFtpFilePerm  
{
public:
	enum {cRead=1,cWrite=2,cDelete=4,cAppend=8,cMkDir=16,cRmDir=32,cList=64,cSubDir=128,cIsUser=256};
	static CString GetParentPath(const char* path);
	CString m_RealPath;
	CString m_VirtPath;
	CFtpFilePerm(){ m_Perm = 0; }
	CFtpFilePerm(const char* VirtPath,const char* RealPath,DWORD m_Perm=cRead|cList|cSubDir);
	virtual ~CFtpFilePerm();

	inline BOOL CanRead()
	{	return m_CanRead;	}
	inline BOOL CanWrite()
	{	return m_CanWrite;	}
	inline BOOL CanDelete()
	{	return m_CanDelete;	}
	inline BOOL CanAppend()
	{	return m_CanAppend;	}
	inline BOOL CanMkDir()
	{	return m_CanMkDir;	}
	inline BOOL CanRmDir()
	{	return m_CanRmDir;	}
	inline BOOL CanList()
	{	return m_CanList;	}
	inline BOOL CanSubDir()
	{	return m_CanSubDir;	}

	union{
		DWORD m_Perm;
		struct{
			DWORD m_CanRead:1;
			DWORD m_CanWrite:1;
			DWORD m_CanDelete:1;
			DWORD m_CanAppend:1;
			DWORD m_CanMkDir:1;
			DWORD m_CanRmDir:1;
			DWORD m_CanList:1;
			DWORD m_CanSubDir:1;
			DWORD m_IsUser:1;
		};
	};

};

inline bool operator == (const CFtpFilePerm &Access1, const CFtpFilePerm &Access2)
{
	return (Access1.m_Perm==Access2.m_Perm &&
		Access1.m_VirtPath==Access2.m_VirtPath &&
		Access1.m_RealPath.CompareNoCase(Access2.m_RealPath)==0 );
}

#endif // !defined(AFX_FTPFILEACCESS_H__C1BEADAF_31B5_4D66_83D9_102B28B73525__INCLUDED_)
