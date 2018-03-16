// FtpUserEx.h: interface for the CFtpUserEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPUSEREX_H__48B1E672_3926_406D_A8D9_0AEEC7B51256__INCLUDED_)
#define AFX_FTPUSEREX_H__48B1E672_3926_406D_A8D9_0AEEC7B51256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FtpUser.h"

class CFtpUserEx : public CFtpUser  
{
public:
	CString m_Note;
	void LoadUser( const char *Name, const char *path, char* val );
	void SaveUser(CIniFile& file);
	CFtpUserEx();
	virtual ~CFtpUserEx();

	CString m_Password;

	int m_bMaxCon;
	int m_bMaxConPerIp;
	int m_bMaxDSpeed;
	int m_bMaxUSpeed;
	int m_bPassword;
	int m_bTimeOut;
	bool m_PasswdChanged;
};

#endif // !defined(AFX_FTPUSEREX_H__48B1E672_3926_406D_A8D9_0AEEC7B51256__INCLUDED_)
