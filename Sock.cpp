// Sock.cpp: implementation of the CSock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCManFTPD2.h"
#include "Sock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HWND CSock::m_SinkWnd = NULL;

CSock::CSock()
{
	m_Sock = INVALID_SOCKET;
	m_Port = 0;		 
}

CSock::~CSock()
{
}

int CSock::Bind(const char *address, short port)
{
	sockaddr_in sock;
	memset(&sock,0,sizeof(sock));
	sock.sin_family=AF_INET;
	sock.sin_addr.S_un.S_addr=address ? inet_addr(address):INADDR_ANY;
	sock.sin_port=htons(port);
	return bind(m_Sock,(sockaddr*)&sock,sizeof(sock));
}

void CSock::Close()
{
	if( IsValid() )
	{
		shutdown(m_Sock, SD_BOTH);
		closesocket(m_Sock);
//		PostMessage(m_MsgWnd,WM_SOCKET_REMOVE,WPARAM(m_Sock),LPARAM(this));
		m_Sock = INVALID_SOCKET;
	}
}


int CSock::Connect(const char *address, unsigned short port)
{
	sockaddr_in sock;
	memset(&sock,0,sizeof(sock));
	sock.sin_family=AF_INET;
	sock.sin_addr.S_un.S_addr=inet_addr(address);
	sock.sin_port=htons(port);
	return connect(m_Sock,(sockaddr*)&sock,sizeof(sock));
}

int CSock::Select(UINT msg, long evt)
{
	return WSAAsyncSelect( m_Sock, m_SinkWnd, msg, evt );
}


bool CSock::Init()
{
	WSADATA wsad;
	memset( &wsad, 0, sizeof(wsad));
	return !WSAStartup( MAKEWORD(1, 1), &wsad );
}

void CSock::Cleanup()
{
	WSACleanup();
}

const char* CSock::GetLocalIp()
{
	char hostname[128];
	gethostname(hostname, 128);
	hostent* host = gethostbyname(hostname);

	if( !host )
		return NULL;

	char** paddr = host->h_addr_list;
	
	if( !*paddr )
		return NULL;

	while( *(paddr+1) )
		paddr++;

	return inet_ntoa( *(in_addr*)*paddr );
}
