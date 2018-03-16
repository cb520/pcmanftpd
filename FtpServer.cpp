// FtpServer.cpp: implementation of the CFtpServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCManFTPD2.h"
#include "FtpServer.h"
#include "FtpCmdCon.h"
#include "AppConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpServer::CFtpServer()
{
}

CFtpServer::~CFtpServer()
{
	if( IsValid() )
		Stop();
}

void CFtpServer::Start()
{
	Create();
	Select( WM_FTP_SERVER_SOCK, FD_ACCEPT|FD_CLOSE);

	CFtpSock::m_pServer = this;
	m_Port = AppConfig.m_Port;
	Bind( NULL, m_Port );
	Listen(5);
}

void CFtpServer::Stop()
{
	Close();
	CFtpSock::m_pServer = NULL;

	SOCKET key;
	CFtpSock* value;
	POSITION pos = m_FtpDataConMap.GetStartPosition();
	while(pos)	// Run the timer procedures of every objects.
	{
		m_FtpDataConMap.GetNextAssoc( pos, key, value );
		delete value;
	}
	m_FtpDataConMap.RemoveAll();
	pos = m_FtpCmdConMap.GetStartPosition();
	while(pos)
	{
		m_FtpCmdConMap.GetNextAssoc( pos, key, value );
		delete value;
	}
	m_FtpCmdConMap.RemoveAll();
}

void CFtpServer::OnAccept(int code)
{
	sockaddr_in addr;
	int addrlen = sizeof(addr);
	memset( &addr, 0, sizeof(addr) );
	CFtpCmdCon* newsock = new CFtpCmdCon();
	if( newsock->Accept( this, (sockaddr*)&addr, &addrlen ) && FilterIP(addr) )
	{
		newsock->Select( WM_FTP_CMD_SOCK, FD_READ|FD_CONNECT|FD_CLOSE );
		newsock->AddToMap();
		newsock->SetAddress( inet_ntoa( addr.sin_addr ) );
		newsock->SetPort( ntohs( addr.sin_port ) );
		newsock->OnConnect(0);
	}
	else
		delete newsock;

	Listen(5);
}

void CFtpServer::OnClose(int code)
{

}


bool CFtpServer::FilterIP(sockaddr_in &saddr)
{
	BYTE* ip = &saddr.sin_addr.S_un.S_un_b.s_b1;
//	in_addr* paddr;
//	in_addr* plastaddr;
	unsigned long* paddr;
	unsigned long* plastaddr;
	int allow_precision = 0;
	int ban_precision = 0;
	if( AppConfig.m_BannedIP.GetSize())
	{
		for( paddr = AppConfig.m_BannedIP.GetData(),
			plastaddr = paddr + AppConfig.m_BannedIP.GetSize();
			paddr < plastaddr; paddr++)
		{
			if( (saddr.sin_addr.S_un.S_addr & *paddr /*paddr->S_un.S_addr*/)
				== saddr.sin_addr.S_un.S_addr )
			{
				BYTE* fip = (BYTE*)paddr;	//&paddr->S_un.S_un_b.s_b1;
				int i = 0;
				while( ip[i] == fip[i] )
					i++ ;
				i++;
				if( i > ban_precision )
					ban_precision = i;
			}
		}
	}
	if( AppConfig.m_AllowedIP.GetSize())
	{
		for( paddr = AppConfig.m_AllowedIP.GetData(),
			plastaddr = paddr + AppConfig.m_AllowedIP.GetSize();
			paddr < plastaddr; paddr++)
		{
			if( (saddr.sin_addr.S_un.S_addr & *paddr /*paddr->S_un.S_addr*/)
				== saddr.sin_addr.S_un.S_addr )
			{
				BYTE* fip = (BYTE*)paddr;	//&paddr->S_un.S_un_b.s_b1;
				int i = 0;
				while( ip[i] == fip[i] )
					i++ ;
				i++;
				if( i > allow_precision )
					allow_precision = i;
			}
		}
	}

	bool default_action = (!AppConfig.m_AllowedIP.GetSize() || AppConfig.m_BannedIP.GetSize());
	return allow_precision > ban_precision ? true : (allow_precision < ban_precision ? false : default_action);
}
