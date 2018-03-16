// FtpDataCon.h: interface for the CFtpDataCon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPDATACON_H__5BD6435B_92C3_4D2F_B66E_A026DA388259__INCLUDED_)
#define AFX_FTPDATACON_H__5BD6435B_92C3_4D2F_B66E_A026DA388259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FtpSock.h"
class CFtpCmdCon;

enum FtpDataConType{FTPDATA_SEND = 1, SEND_FILE=FTPDATA_SEND, SEND_LIST=(2|FTPDATA_SEND), RECV_FILE=4, FTPDATA_NONE=0};

class CFtpDataCon : public CFtpSock  
{
friend class CFtpSock;
friend class CFtpCmdCon;
public:
	__int64 GetTotalSize() {	return m_TotalBytes;	}
	__int64 GetTransferredBytes() {	return m_TransferredBytes;	}
	CFtpCmdCon* GetCmdCon(){ return m_pCmdCon; }
	CString GetFilePath(){ return m_FilePath; }
	static void OnAllConTimer();
	void InitDirListTransfer( CString dir_list );
	bool InitFileTransfer( enum FtpDataConType type, CString file_path, __int64 offset = 0 );
	void EndDataTransfer();
//	static void ReCalcSpeedLimits();
	virtual void OnTimer();
	void Close();
	static void Init();
	static USHORT FindFreeDataPort();
	static CFtpSockMap& GetFtpDataConMap()	{ return m_FtpDataConMap; }
	void AddToMap(){ m_FtpDataConMap[m_Sock] = this; }
	void RemoveFromMap(){ m_FtpDataConMap.RemoveKey(m_Sock); }
	bool IsPassive() { return m_IsPasv; }
	bool IsConnected() { return m_IsConnected; }

	CFtpDataCon(CFtpCmdCon* pCmdCon, bool IsPasv);
	void SetType(FtpDataConType Type) { m_Type = Type; }
	FtpDataConType GetType() { return m_Type; }
	virtual ~CFtpDataCon();

	CFtpUser* GetUser(){ return m_pUser; }
	void SetUser(CFtpUser* puser) { m_pUser = puser; }
	CString GetUserName() { return m_UserName; }
protected:
	ULONG m_IdleTime;
	CString m_UserName;
	HANDLE m_hFile;
	void OnAccept(int code);
	void OnSend(int code);
	void OnRecv(int code);
	void OnClose(int code);
	void OnConnect(int code);

	FtpDataConType m_Type;
	bool m_IsPasv;
	bool m_IsConnected;
	bool m_CanSend;
	bool m_CanRecv;
	DWORD m_BytesPerSec;
	DWORD m_BlockSize;
	BYTE* m_Block;
	BYTE* m_Buffer;
	CFtpUser* m_pUser;
	__int64 m_TotalBytes;
	__int64 m_TransferredBytes;
	CString m_FilePath;
	CString m_FileList;
	CFtpCmdCon* m_pCmdCon;
	static USHORT m_DataPort;
//	static DWORD m_TotalMaxUSpeed;
//	static DWORD m_TotalMaxDSpeed;
	static DWORD m_TotalDBytesPerSec;
	static DWORD m_TotalUBytesPerSec;
};

#endif // !defined(AFX_FTPDATACON_H__5BD6435B_92C3_4D2F_B66E_A026DA388259__INCLUDED_)
