#if !defined(AFX_SERVERTHREAD_H__8428DE58_1BDC_47EA_B738_AD646F4FA7C2__INCLUDED_)
#define AFX_SERVERTHREAD_H__8428DE58_1BDC_47EA_B738_AD646F4FA7C2__INCLUDED_

#include "FtpServer.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CServerThread thread

class CServerThread : public CWinThread
{
	DECLARE_DYNCREATE(CServerThread)
protected:
	CServerThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	CFtpServer m_Server;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CServerThread();

	// Generated message map functions
	//{{AFX_MSG(CServerThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERTHREAD_H__8428DE58_1BDC_47EA_B738_AD646F4FA7C2__INCLUDED_)
