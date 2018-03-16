// FtpUser.h: interface for the CFtpUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPUSER_H__7E31A180_2FF8_4210_9734_3C439FEB52B2__INCLUDED_)
#define AFX_FTPUSER_H__7E31A180_2FF8_4210_9734_3C439FEB52B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FtpFilePerm.h"	// Added by ClassView
#include "IniFile.h"	// Added by ClassView
#include "FilePermMap.h"

extern CString AppPath;

class CFtpUser  
{
friend class CFtpCmdCon;
public:
	bool VerifyPasswd( const char* passwd );
	enum	{ SHADOWER_L = 0xf30a439c, SHADOWER_R = 0x204be7a1 };

	void SetFilePerm(CFilePermMap &src){ m_FilePerm = src; }
	CFilePermMap& GetFilePerm(){ return m_FilePerm; }

	UINT GetTimeOut();
	UINT GetMaxConPerIP();
	UINT GetMaxCon();
	void ModifyPerm( CFtpFilePerm* perm, CString virt_path, CString real_path="", DWORD perm_val=-1 );
	void DelFilePerm( CFtpFilePerm* perm );
	static void Unload();
	static CFtpUser* FindUser(const char* name);
	static CFtpUser* FindGroup(const char* name);
	CFtpFilePerm* QueryPerm( const char* pVirtPath );
	static const char m_Anonymous[];
	void LoadUser( const char *Name, const char *path, char* val );
	void LoadFilePerm();
	void SaveFilePerm();
	static void LoadSettings();

	BOOL IsPathExist(CFtpFilePerm* Perm, CString VirtPath, bool show_hidden );
	CString GetRealPath(const CFtpFilePerm *Perm, const char* pVirtPath);
	CString GetRealPath(const char* pVirtPath);
	CFtpUser();
	CFtpUser (CFtpUser &user);
	virtual ~CFtpUser();

	CString m_GroupName;
	CString m_UserName;
	unsigned long m_PasswdShadowL;
	unsigned long m_PasswdShadowR;

	BOOL m_IsUser;
	CFtpUser* m_pParent;

	UINT m_MaxUSpeed;
	UINT m_MaxDSpeed;
	UINT m_MaxCon;
	UINT m_MaxConPerIp;
	UINT m_TimeOut;
	BOOL m_Banned;
	BOOL m_Enabled;

//DEL 	UINT AddRef(){ return ++m_RefCount; }
//DEL 	UINT GetRefCount(){ return m_RefCount; }
//DEL 	UINT Release();

	UINT GetUserCount(){ return m_UserCount; }
	void SetUserCount(UINT user_count){ m_UserCount = user_count; }
	static CMapStringToPtr& GetAllUsers(){ return m_AllUsers; }
	static CMapStringToPtr& GetAllGroups(){ return m_AllGroups; }

protected:
	BOOL m_PermChanged;
//	bool m_bModified;
	CFilePermMap m_FilePerm;
	UINT m_RefCount;
	UINT m_UserCount;
	static CMapStringToPtr m_AllUsers;
	static CMapStringToPtr m_AllGroups;
//	CString GetRealPath(const CFtpFilePerm *Access, const char* VirtPath);
};

#endif // !defined(AFX_FTPUSER_H__7E31A180_2FF8_4210_9734_3C439FEB52B2__INCLUDED_)
