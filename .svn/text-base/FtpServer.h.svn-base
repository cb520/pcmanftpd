// FtpServer.h: interface for the CFtpServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPSERVER_H__66C03886_EEBA_415D_865A_F47C3EB5E7BE__INCLUDED_)
#define AFX_FTPSERVER_H__66C03886_EEBA_415D_865A_F47C3EB5E7BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FtpSock.h"
#include "AppConfig.h"

class CMainFrm;

class CFtpServer : public CFtpSock  
{
public:
	bool FilterIP( sockaddr_in& saddr );
	void OnClose( int code );
	void OnAccept(int code);
	void Stop();
	void Start();
	CFtpServer();
	virtual ~CFtpServer();

};

#endif // !defined(AFX_FTPSERVER_H__66C03886_EEBA_415D_865A_F47C3EB5E7BE__INCLUDED_)
