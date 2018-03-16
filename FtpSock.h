// FtpSock.h: interface for the CFtpSock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPSOCK_H__032A1F77_CBD2_4B27_BB4B_DAF711EDF987__INCLUDED_)
#define AFX_FTPSOCK_H__032A1F77_CBD2_4B27_BB4B_DAF711EDF987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "Sock.h"

class CFtpSock;
class CFtpServer;

typedef CMap<SOCKET, SOCKET, CFtpSock*, CFtpSock*>	CFtpSockMap;

#define	WM_FTP_SERVER_SOCK		WM_USER+1
#define	WM_FTP_CMD_SOCK			WM_USER+2
#define	WM_FTP_DATA_SOCK		WM_USER+3
#define	WM_FTP_DATA_SEND		WM_USER+4
#define	WM_FTP_SOCK_DESTROY		WM_USER+5

class CMainFrm;

class CFtpSock : public CSock  
{
public:
	void Destroy();
	virtual void OnTimer();
	static void Cleanup();
	static bool Init();
	CFtpSock();
	virtual ~CFtpSock();

protected:
	CMainFrm* GetGUI() { return (CMainFrm*)AfxGetMainWnd(); }
	static CFtpServer* m_pServer;
	static CFtpSockMap m_FtpCmdConMap;
	static CFtpSockMap m_FtpDataConMap;
	static LRESULT SinkWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};

#endif // !defined(AFX_FTPSOCK_H__032A1F77_CBD2_4B27_BB4B_DAF711EDF987__INCLUDED_)
