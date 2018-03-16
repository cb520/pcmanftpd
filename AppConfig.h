// AppConfig.h: interface for the CAppConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPCONFIG_H__5E10BFAF_7320_4290_A62C_383C95CE2BD6__INCLUDED_)
#define AFX_APPCONFIG_H__5E10BFAF_7320_4290_A62C_383C95CE2BD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <winsock2.h>
#include <afxtempl.h>

extern CString AppPath;

class CAppConfig  
{
public:
	BOOL m_EnablePasv;
	void InitServerLog();
	CString GetServerLogPath(){return AppPath+"Logs\\ServerLog.txt";}
	void InitTransferLog();
	CString GetTransferLogPath(){return AppPath+"Logs\\TransferLog.txt";}
	UINT m_WaitForLogin;
	void LoadWelcomeMsg();
	void Save();
	BOOL m_StartupWithGUI;
	void Unload();
	void LoadIPFilter();
	void SaveIPFilter();
	void SetTransferIcon();
	void SetUserIcon();
	void SetNormalIcon();
	void SetOfflineIcon();
	HINSTANCE m_Instance;
	HANDLE m_LogFile;
	HANDLE m_TransferLogFile;
	void Load();
	CAppConfig();
	virtual ~CAppConfig();

	CString m_WelcomeMsg;
	USHORT m_Port;
//	USHORT m_ControlPort;
	USHORT m_PasvPortMin;
	USHORT m_PasvPortMax;
	UINT m_MaxConPerIp;
	UINT m_MaxTryPerIp;
	UINT m_MaxCon;
	DWORD m_MaxUSpeed;
	DWORD m_MaxDSpeed;
	DWORD m_BufferSize;
	BOOL m_LogToFile;
	BOOL m_LogTransferToFile;
	BOOL m_LogToScreen;
	UINT m_ScreenMaxLine;
	BOOL m_TrayIcon;
	HICON m_NormalIcon;
	HICON m_OfflineIcon;
	HICON m_UserIcon;
	HICON m_TransferIcon;
	HICON m_CurIcon;

//	CArray<in_addr, in_addr> m_AllowedIP;
//	CArray<in_addr, in_addr> m_BannedIP;
	CDWordArray m_AllowedIP;
	CDWordArray m_BannedIP;
};

extern CAppConfig AppConfig;

#endif // !defined(AFX_APPCONFIG_H__5E10BFAF_7320_4290_A62C_383C95CE2BD6__INCLUDED_)
