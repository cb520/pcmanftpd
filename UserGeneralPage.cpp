// UserGeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "UserGeneralPage.h"
#include "UserDialog.h"
#include "UserPermDlg.h"
#include "FilePermMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserGeneralPage property page

CUserGeneralPage::CUserGeneralPage()
	: CPropertyPage(CUserGeneralPage::IDD)
{
	m_pDlg=NULL;
	//{{AFX_DATA_INIT(CUserGeneralPage)
	//}}AFX_DATA_INIT
}

CUserGeneralPage::~CUserGeneralPage()
{
}

void CUserGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserGeneralPage)
	DDX_Control(pDX, IDC_GROUP, m_GroupCombo);
	//}}AFX_DATA_MAP

	if(!m_pDlg)
		return;

	CFtpUserEx* pUser=m_pDlg->m_pUser;
	if(pUser)
	{
		DDX_Text(pDX, IDC_NAME, pUser->m_UserName);
		DDX_Text(pDX, IDC_PASSWD, pUser->m_Password);
		DDX_Text(pDX, IDC_TIMEOUT, pUser->m_TimeOut);
		DDX_Check(pDX, IDC_ENABLED, pUser->m_Enabled);

		DDX_Text(pDX, IDC_MAXDSPEED, pUser->m_MaxDSpeed);
		DDV_MinMaxUInt(pDX, pUser->m_MaxDSpeed, 0, 65535);
		DDX_Text(pDX, IDC_MAXUSPEED, pUser->m_MaxUSpeed);
		DDV_MinMaxUInt(pDX, pUser->m_MaxUSpeed, 0, 65535);
		DDX_Text(pDX, IDC_MAXCON, pUser->m_MaxCon);
		DDV_MinMaxUInt(pDX, pUser->m_MaxCon, 0, 32768);
		DDX_Text(pDX, IDC_MAXCONPERIP, pUser->m_MaxConPerIp);
		DDV_MinMaxUInt(pDX, pUser->m_MaxConPerIp, 0, 32768);

		DDX_Check(pDX, IDC_MAXCON_CHECK, pUser->m_bMaxCon);
		DDX_Check(pDX, IDC_MAXCONPERIP_CHECK, pUser->m_bMaxConPerIp);
		DDX_Check(pDX, IDC_MAXDSPEED_CHECK, pUser->m_bMaxDSpeed);
		DDX_Check(pDX, IDC_MAXUSPEED_CHECK, pUser->m_bMaxUSpeed);
		DDX_Check(pDX, IDC_PASSWD_CHECK, pUser->m_bPassword);
		DDX_Check(pDX, IDC_TIMEOUT_CHECK, pUser->m_bTimeOut);

		DDX_Text(pDX, IDC_NOTE, pUser->m_Note);

		if( pDX->m_bSaveAndValidate && pUser->m_IsUser )
		{
				
		}
		else
		{
			if( pUser->m_pParent )
				m_GroupCombo.SelectString(0, pUser->m_GroupName);
			else
			{
				CString str;	str.LoadString(IDS_GROUP_NONE);
				m_GroupCombo.SelectString(0, str);
			}
		}
//---------------------------------------------------
	}
}


BEGIN_MESSAGE_MAP(CUserGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUserGeneralPage)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_CBN_SELENDOK(IDC_GROUP, OnSelEndOKGroup)
	ON_BN_CLICKED(IDC_FILE_PERM, OnFilePerm)
	ON_EN_CHANGE(IDC_PASSWD, OnChangePasswd)
	//}}AFX_MSG_MAP
/*	ON_BN_CLICKED(IDC_PASSWD_CHECK, OnPasswdCheck)
	ON_BN_CLICKED(IDC_MAXUSPEED_CHECK, OnMaxUSpeedCheck)
	ON_BN_CLICKED(IDC_MAXDSPEED_CHECK, OnMaxDSpeedCheck)
	ON_BN_CLICKED(IDC_MAXCON_CHECK, OnMaxConCheck)
	ON_BN_CLICKED(IDC_TIMEOUT_CHECK, OnTimeOutCheck)
	ON_EN_CHANGE(IDC_PASSWD, OnChangePasswd)
	ON_EN_CHANGE(IDC_MAXUSPEED, OnChangeMaxUSpeed)
	ON_EN_CHANGE(IDC_MAXDSPEED, OnChangeMaxDSpeed)
	ON_EN_CHANGE(IDC_MAXCON, OnChangeMaxCon)
	ON_EN_CHANGE(IDC_TIMEOUT, OnChangeTimeOut)
*/
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserGeneralPage message handlers

void CUserGeneralPage::OnChangeName() 
{
	if( m_pDlg && m_pDlg->m_pUser )
	{
		CFtpUser* pUser = m_pDlg->m_pUser;
		GetDlgItemText(IDC_NAME,pUser->m_UserName);
		HTREEITEM hitem = m_pDlg->m_UserList.GetSelectedItem();
		m_pDlg->m_UserList.SetItemText(hitem,pUser->m_UserName);
		m_pDlg->m_UserList.SortChildren(m_pDlg->m_UserList.GetParentItem(hitem) );

		if( !pUser->m_IsUser )
		{
			m_GroupCombo.DeleteString( m_pDlg->m_CurGroup );
			m_GroupCombo.InsertString( m_pDlg->m_CurGroup, pUser->m_UserName );
			for(int i=0; i< m_pDlg->m_Users.GetSize(); i++)
			{
				CFtpUser* pGroupUser = (CFtpUser*)m_pDlg->m_Users[i];
				if( pGroupUser->m_pParent == pUser )
					pGroupUser->m_GroupName = pUser->m_UserName;
			}
		}
	}
}

BOOL CUserGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	((CSpinButtonCtrl*)GetDlgItem(IDC_MAXUSPEEDSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_MAXDSPEEDSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_TIMEOUTSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_MAXCONSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_MAXCONPERIPSPIN))->SetRange32(0,32768);
	return TRUE;
}

BOOL CUserGeneralPage::EndOK()
{
	if (!UpdateData(TRUE))
		return FALSE;
	EndDialog(IDOK);
	return TRUE;
}

/*
void CUserGeneralPage::OnPasswdCheck() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_PASSWD)->EnableWindow(m_pDlg->m_pUser->m_bPassword);
}

void CUserGeneralPage::OnMaxUSpeedCheck() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_MAXUSPEED)->EnableWindow(m_pDlg->m_pUser->m_bMaxUSpeed);
	GetDlgItem(IDC_MAXUSPEEDSPIN)->EnableWindow(m_pDlg->m_pUser->m_bMaxUSpeed);	
}

void CUserGeneralPage::OnMaxDSpeedCheck() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_MAXDSPEED)->EnableWindow(m_pDlg->m_pUser->m_bMaxDSpeed);
	GetDlgItem(IDC_MAXDSPEEDSPIN)->EnableWindow(m_pDlg->m_pUser->m_bMaxDSpeed);
}

void CUserGeneralPage::OnMaxConCheck() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_MAXCON)->EnableWindow(m_pDlg->m_pUser->m_bMaxCon);
	GetDlgItem(IDC_MAXCONSPIN)->EnableWindow(m_pDlg->m_pUser->m_bMaxCon);
}

void CUserGeneralPage::OnTimeOutCheck() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_TIMEOUT)->EnableWindow(m_pDlg->m_pUser->m_bMaxCon);
	GetDlgItem(IDC_TIMEOUTSPIN)->EnableWindow(m_pDlg->m_pUser->m_bMaxCon);	
}
*/

void CUserGeneralPage::OnSelEndOKGroup() 
{
	int sel=m_GroupCombo.GetCurSel();
	if( sel == CB_ERR )
		return;

	CString str;	m_GroupCombo.GetLBText(sel, str);
	m_pDlg->m_pUser->m_pParent = m_pDlg->FindUser( str, false );
	if(m_pDlg->m_pUser->m_pParent)
		m_pDlg->m_pUser->m_GroupName = str;
	else
		m_pDlg->m_pUser->m_GroupName.Empty();

	UpdateData();
}

/*
void CUserGeneralPage::OnChangePasswd() 
{
	
}

void CUserGeneralPage::OnChangeMaxUSpeed() 
{
}

void CUserGeneralPage::OnChangeMaxDSpeed() 
{
}

void CUserGeneralPage::OnChangeMaxCon() 
{
	
}

void CUserGeneralPage::OnChangeTimeOut() 
{
}
*/

void CUserGeneralPage::OnFilePerm() 
{
	CFilePermMap OldPerm;

	OldPerm = m_pDlg->m_pUser->GetFilePerm();

	CUserPermDlg dlg(this, m_pDlg->m_pUser);
	if( dlg.DoModal() != IDOK )
		m_pDlg->m_pUser->SetFilePerm( OldPerm );
}


void CUserGeneralPage::OnChangePasswd() 
{
	m_pDlg->m_pUser->m_PasswdChanged = true;
}
