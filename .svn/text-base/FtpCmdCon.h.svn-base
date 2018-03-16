// FtpCmdCon.h: interface for the CFtpCmdCon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPCMDCON_H__943DB0C9_0B7F_4895_B6F9_7EB26A595913__INCLUDED_)
#define AFX_FTPCMDCON_H__943DB0C9_0B7F_4895_B6F9_7EB26A595913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FtpSock.h"

class CFtpDataCon;
class CFtpUser;

class CFtpCmdCon : public CFtpSock  
{
protected:
	int SendStr(const char* str, bool log = true )
	{
		if( log )
			WriteToLog(str);
		return CSock::SendStr(str);
	}
	void WriteToLog(const char *str);
	void WriteToTransferLog( bool is_download, const char* file );
	void OnLIST(const char* param);
	void OnLIST( const char* param, bool full );
	void OnPORT(const char* param);
	void OnPASV(const char* param);
	void OnPWD (const char* param);
	void OnCWD (const char* param);
	void OnCDUP (const char* param);
	void OnRETR(const char* param);
	void OnSTOR(const char* param);
	void OnREST(const char* param);
	void OnAPPE(const char* param);
	void OnDELE(const char* param);
	void OnRMD (const char* param);
	void OnMKD (const char* param);
	void OnRNFR(const char* param);
	void OnRNTO(const char* param);
	void OnABOR(const char* param);
	void OnTYPE(const char* param);
	void OnNLST(const char* param);
	void OnSIZE(const char* param);

	void OnNOOP(const char* param);
	void OnMODE(const char* param);
	void OnSTRU(const char* param);
	void OnSTAT(const char* param);
	void OnSYST(const char* param);

	void OnUSER(const char* param);
	void OnPASS(const char* param);
	void OnQUIT(const char* param);

	bool CanLogin();
public:
	void ReplyOK()
	{	SendStr("200 Command okay.\r\n");	}
	void ReplyFileActionOK()
	{	SendStr("250 Requested file action okay, completed.\r\n");	}
	void ReplyFileUnavailable()
	{	SendStr("550 File not found or access denied.\r\n");	}
	void ReplyNotImplemented()
	{	SendStr("502 Command not implemented.\r\n");	}
	void ReplyDataConOpen()
	{	SendStr("150 File status okay; Open data connection.\r\n");	}
	void ReplyDataConClose()
	{	SendStr("226 Data Sent okay.\r\n");	}
	void ReplyNoDataCon()
	{	SendStr("425 Can't open data connection.\r\n");	}
	void ReplyUnknownCommand()
	{	SendStr("500 Syntax error, command unrecognized.\r\n");	}

	void SetDataCon( CFtpDataCon* DataCon) { m_pDataCon = DataCon; }
	CFtpDataCon* GetDataCon() { return m_pDataCon; }

	static GetTotalUserCount()	{ return m_UserCount; }
	static CFtpSockMap& GetFtpCmdConMap()	{ return m_FtpCmdConMap; }

	ULONG GetIdleTime() { return m_IdleTime; }
	void SetIdleTime(ULONG idle_time) { m_IdleTime = idle_time; }

	CString GetUserName() { return m_UserName; }
	void SetUser( CFtpUser* puser) { m_pUser = puser; }
public:
	void BanUser( bool byName = false );
	void KickUser();
	CFtpUser* GetUser() const { return m_pUser; }
	void OnTimer();
	virtual void Close();
	void EndDataTransfer();
	bool GetFileList(CString virt_path, CString& list, bool full);
	void PreLogin();
	void AddToMap(){ m_FtpCmdConMap[m_Sock] = this; }
	void RemoveFromMap(){ m_FtpCmdConMap.RemoveKey(m_Sock); }

	bool ParseCmdLine(const char* pCmdLine);
	void StoreFile(const char *path, bool Append);

	void OnConnect(int code);
	void OnClose(int code);
	void OnSend(int code);
	void OnRecv( int code );
	CFtpCmdCon();
	virtual ~CFtpCmdCon();
	static void Init();

	typedef void (CFtpCmdCon::*FtpCmdFunc)(const char*);
	static struct CFtpCmdMapEnt
	{
		char cmd[5];
		FtpCmdFunc func;
	}m_CmdMap[];
protected:
	CString m_RenameFromPath;
	CString GetFullPath( const char* FileName );
	__int64 m_Rest;
	CString m_UserName;
	CFtpUser* m_pUser;
	ULONG m_IdleTime;
	CFtpDataCon* m_pDataCon;
	char m_Type;
	CString m_WorkingDir;
	char m_Mode;
	bool m_LoggedIn;
	static DWORD m_UserCount;
};

#endif // !defined(AFX_FTPCMDCON_H__943DB0C9_0B7F_4895_B6F9_7EB26A595913__INCLUDED_)
