// ServerMsgPage.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMsgPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerMsgPage property page

IMPLEMENT_DYNCREATE(CServerMsgPage, CPropertyPage)

CServerMsgPage::CServerMsgPage() : CPropertyPage(CServerMsgPage::IDD)
{
	//{{AFX_DATA_INIT(CServerMsgPage)
	m_MsgStr = _T("");
	//}}AFX_DATA_INIT
}

CServerMsgPage::~CServerMsgPage()
{
}

void CServerMsgPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerMsgPage)
	DDX_Text(pDX, IDC_EDIT, m_MsgStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerMsgPage, CPropertyPage)
	//{{AFX_MSG_MAP(CServerMsgPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerMsgPage message handlers
