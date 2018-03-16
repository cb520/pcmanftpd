// ServerIPFilterPage.cpp : implementation file
//

#include "stdafx.h"
#include "ServerIPFilterPage.h"
#include "AppConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerIPFilterPage property page

IMPLEMENT_DYNCREATE(CServerIPFilterPage, CPropertyPage)

CServerIPFilterPage::CServerIPFilterPage() : CPropertyPage(CServerIPFilterPage::IDD)
{
	//{{AFX_DATA_INIT(CServerIPFilterPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CServerIPFilterPage::~CServerIPFilterPage()
{
}

void CServerIPFilterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerIPFilterPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerIPFilterPage, CPropertyPage)
	//{{AFX_MSG_MAP(CServerIPFilterPage)
	ON_BN_CLICKED(IDC_ADD_ALLOW, OnAddAllow)
	ON_BN_CLICKED(IDC_DEL_ALLOW, OnDelAllow)
	ON_BN_CLICKED(IDC_ADD_BAN, OnAddBan)
	ON_BN_CLICKED(IDC_DEL_BAN, OnDelBan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerIPFilterPage message handlers

void CServerIPFilterPage::OnAddAllow() 
{
	AddIP(TRUE);	
}

void CServerIPFilterPage::OnDelAllow() 
{
	DeleteIP(TRUE);
}

void CServerIPFilterPage::OnAddBan() 
{
	AddIP(FALSE);
}

void CServerIPFilterPage::OnDelBan() 
{
	DeleteIP(FALSE);
}

void CServerIPFilterPage::AddIP(BOOL bAllow)
{
	CListBox* pList = (CListBox*)GetDlgItem(bAllow?IDC_ALLOW_LIST:IDC_BAN_LIST);
	CEdit* pEdit = (CEdit*)GetDlgItem(bAllow?IDC_ALLOW_EDIT:IDC_BAN_EDIT);
	CString ip;
	pEdit->GetWindowText(ip);
	if( !ip.IsEmpty() && LB_ERR == pList->FindStringExact(0,ip) )
	{
		int sel = pList->AddString(ip);
		pList->SetCurSel(sel);
		pEdit->SetWindowText(NULL);
	}
}

void CServerIPFilterPage::DeleteIP(BOOL bAllow)
{
	CListBox* pList = (CListBox*)GetDlgItem(bAllow?IDC_ALLOW_LIST:IDC_BAN_LIST);
	int sel = pList->GetCurSel();
	if(sel != LB_ERR )// && IDYES==AfxMessageBox(IDS_QUERYDEL, MB_YESNO|MB_ICONQUESTION,0) )
		pList->DeleteString( sel );
}

BOOL CServerIPFilterPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CFile file;
	if(file.Open(AppPath + "IPFilter.ini",CFile::modeRead))
	{
		CArchive ar(&file,CArchive::load);
		CString line;
		CListBox* pAllowList = (CListBox*)GetDlgItem(IDC_ALLOW_LIST);
		CListBox* pBanList = (CListBox*)GetDlgItem(IDC_BAN_LIST);
		while(ar.ReadString(line))
		{
			line.Replace("255","*");
			if( line[0] == '+' )
				pAllowList->AddString( LPCTSTR(line)+1 );
			else if( line[0] == '-' )
				pBanList->AddString( LPCTSTR(line)+1 );
		}
		ar.Close();
		file.Close();
	}
	return TRUE;
}

void CServerIPFilterPage::OnOK()
{
//	CFile file;
//	if(file.Open(AppPath + "IPFilter.ini",CFile::modeWrite|CFile::modeCreate))
//	{
	CString line;
	CListBox* pLists[] = { (CListBox*)GetDlgItem(IDC_ALLOW_LIST), 
		(CListBox*)GetDlgItem(IDC_BAN_LIST) };

//	CArray<in_addr, in_addr>* pFilter[] = {&AppConfig.m_AllowedIP, &AppConfig.m_BannedIP};
	CDWordArray* pFilter[] = {&AppConfig.m_AllowedIP, &AppConfig.m_BannedIP};
	for( int i = 0; i < 2; ++i )
	{
		int c = pLists[i]->GetCount();
		pFilter[i]->RemoveAll();
		for( int idx = 0; idx < c; idx++ )
		{
			pLists[i]->GetText(idx,line);
			line.Replace("*","255");
			pFilter[i]->Add(inet_addr(line));
/*			line = ( pList == pLists ? '+':'-') + line;
			line += "\x0d\x0a";
			file.Write(LPCTSTR(line), line.GetLength());
*/

		}
	}
//		file.Close();
//	}
	AppConfig.SaveIPFilter();
}
