// Sock.h: interface for the CSock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCK_H__8E29C759_DA57_4D9A_9FF7_5502E67F421E__INCLUDED_)
#define AFX_SOCK_H__8E29C759_DA57_4D9A_9FF7_5502E67F421E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>

class CSock  
{
friend class CSock;
public:
	static const char* GetLocalIp();
	SOCKET GetSock() { return m_Sock; };
	bool Accept(CSock* pListenSock, sockaddr* addr, int *addrlen){ m_Sock = accept(pListenSock->m_Sock, addr, addrlen); return IsValid(); }
	bool Create() { m_Sock = socket(AF_INET, 0, 0);  return IsValid(); }
	int Listen(int backlog) { return listen(m_Sock, backlog); }
	int Recv( const void* buf, int len)  { return recv(m_Sock, (char*)buf, len, 0); }
	int Send( const void* buf, int len)  { return send(m_Sock, (char*)buf, len, 0); }
	int SendStr(const char* str) {	return Send(str,strlen(str));	}
	void SetAddress( CString ads ){ m_Address = ads; }
	void SetPort( unsigned int  port ){ m_Port = port; }
	CString GetAddress(){ return m_Address; }
	unsigned int GetPort() { return m_Port; }

	static void Cleanup();
	static bool Init();
	static void SetSinkWnd(HWND wnd) { m_SinkWnd = wnd; }

	int Select( UINT msg, long evt );
	int Connect(const char *address, unsigned short port);
	virtual void Close();
	int Bind(const char *address, short port);
	bool IsValid(){ return (m_Sock != INVALID_SOCKET); }
	CSock();
	virtual ~CSock();

protected:
	CString m_Address;
	unsigned int m_Port;
	static HWND m_SinkWnd;
	SOCKET m_Sock;
};

#endif // !defined(AFX_SOCK_H__8E29C759_DA57_4D9A_9FF7_5502E67F421E__INCLUDED_)
