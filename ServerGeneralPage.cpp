// ServerGeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "ServerGeneralPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerGeneralPage property page

IMPLEMENT_DYNCREATE(CServerGeneralPage, CPropertyPage)

CServerGeneralPage::CServerGeneralPage() : CPropertyPage(CServerGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CServerGeneralPage)
	m_MaxConPerIp = 0;
	m_PasvPortMin = 0;
	m_PasvPortMax = 0;
	m_Port = 0;
	m_MaxCon = 0;
	m_StartupWithGUI = FALSE;
	m_TrayIcon = FALSE;
	m_MaxTryPerIp = 0;
	m_MaxDSpeed = 0;
	m_MaxUSpeed = 0;
	m_EnablePasv = FALSE;
	//}}AFX_DATA_INIT
}

CServerGeneralPage::~CServerGeneralPage()
{
}

void CServerGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerGeneralPage)
	DDX_Text(pDX, IDC_MAXCONPERIP, m_MaxConPerIp);
	DDV_MinMaxUInt(pDX, m_MaxConPerIp, 0, 32767);
	DDX_Text(pDX, IDC_PASVPORTMIN, m_PasvPortMin);
	DDV_MinMaxUInt(pDX, m_PasvPortMin, 0, 32767);
	DDX_Text(pDX, IDC_PASVPORTMAX, m_PasvPortMax);
	DDV_MinMaxUInt(pDX, m_PasvPortMax, 0, 32767);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDV_MinMaxUInt(pDX, m_Port, 0, 32767);
	DDX_Text(pDX, IDC_MAXCON, m_MaxCon);
	DDV_MinMaxUInt(pDX, m_MaxCon, 0, 32767);
	DDX_Check(pDX, IDC_STARTUP_WITH_GUI, m_StartupWithGUI);
	DDX_Check(pDX, IDC_TRAYICON, m_TrayIcon);
	DDX_Text(pDX, IDC_MAXTRYPERIP, m_MaxTryPerIp);
	DDX_Text(pDX, IDC_TOTALMAXDSPEED, m_MaxDSpeed);
	DDX_Text(pDX, IDC_TOTALMAXUSPEED, m_MaxUSpeed);
	DDX_Check(pDX, IDC_ENABLE_PASV, m_EnablePasv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CServerGeneralPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerGeneralPage message handlers

BOOL CServerGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	((CSpinButtonCtrl*)GetDlgItem(IDC_PORTSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_PASVPORTMINSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_PASVPORTMAXSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_MAXCONPERIPSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_MAXCONSPIN))->SetRange32(0,32768);
	((CSpinButtonCtrl*)GetDlgItem(IDC_MAXTRYPERIPSPIN))->SetRange32(0,32768);
	return TRUE;
}

