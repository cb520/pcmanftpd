// LogViewer.cpp : implementation file
//

#include "stdafx.h"
#include "AppConfig.h"
#include "LogViewer.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogViewer dialog


CLogViewer::CLogViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CLogViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogViewer)
	//}}AFX_DATA_INIT
}


void CLogViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogViewer)
	DDX_Control(pDX, IDC_UPLOADLIST, m_UploadList);
	DDX_Control(pDX, IDC_DOWNLOADLIST, m_DownloadList);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogViewer, CDialog)
	//{{AFX_MSG_MAP(CLogViewer)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogViewer message handlers

DLGLAYOUT CLogViewer::m_DlgLayoutData[]={
//	{IDC_TAB, DLA_TOP|DLA_LEFT|DLA_RIGHT},
	{IDC_TREE, DLA_ALL},
	{IDC_DOWNLOADLIST, DLA_ALL},
	{IDC_UPLOADLIST, DLA_ALL}
};

BOOL CLogViewer::OnInitDialog() 
{
	SetIcon(::LoadIcon( AfxGetInstanceHandle(), LPCTSTR(IDR_MAIN)), TRUE);
	SetIcon(::LoadIcon( AfxGetInstanceHandle(), LPCTSTR(IDR_MAIN)), FALSE);

	CDialog::OnInitDialog();
	m_DlgLayout.Load(m_hWnd, m_DlgLayoutData, sizeof(m_DlgLayoutData)/sizeof(DLGLAYOUT));
	CString str;
//	str.LoadString(IDS_LOG_FULL_TITLE);
//	m_Tab.InsertItem( 0, str);
	str.LoadString(IDS_LOG_DOWNLOAD_TITLE);
	m_Tab.InsertItem( 0, str);
	str.LoadString(IDS_LOG_UPLOAD_TITLE);
	m_Tab.InsertItem( 1, str);

	SHFILEINFO shinf;
	HIMAGELIST himg = (HIMAGELIST)SHGetFileInfo(AppPath,0,&shinf,sizeof(shinf),SHGFI_SYSICONINDEX|SHGFI_SMALLICON );
	CImageList* pImg = CImageList::FromHandle(himg);
	m_DownloadList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_UploadList.SetExtendedStyle( LVS_EX_FULLROWSELECT);
	m_DownloadList.SetImageList( pImg, LVSIL_SMALL );
	m_UploadList.SetImageList( pImg, LVSIL_SMALL );

	str.LoadString(IDS_FILE_NAME);
	m_DownloadList.InsertColumn(0, str, LVCFMT_LEFT, 430);
	m_UploadList.InsertColumn(0, str, LVCFMT_LEFT, 430);
	str.LoadString(IDS_TIME);
	m_DownloadList.InsertColumn(1, str, LVCFMT_LEFT, 120);
	m_UploadList.InsertColumn(1, str, LVCFMT_LEFT, 120);
	str.LoadString(IDS_USER);
	m_DownloadList.InsertColumn(1, str, LVCFMT_LEFT, 120);
	m_UploadList.InsertColumn(1, str, LVCFMT_LEFT, 120);

	m_Tab.SetCurSel(0);

	CFile file;
	if( file.Open(AppConfig.GetTransferLogPath(),CFile::modeRead|CFile::shareDenyNone ) )
	{
		CArchive ar(&file, CArchive::load);
		CString line;

		while(ar.ReadString(line) )
		{
			if( 0 == strncmp( line, "Download", 8) )
				AddFile( line, true);
			else if( 0 == strncmp( line, "Upload  ", 8) )
				AddFile( line, false);
			else
				continue;
		}
		ar.Close();
		file.Close();
	}
//	ParseLog();
//	m_Log.RemoveAll();
	return TRUE;
}

void CLogViewer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	m_DlgLayout.OnSize(nType, cx, cy);
}

void CLogViewer::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int sel = m_Tab.GetCurSel();
	CWnd* Views[] = {/*&m_Tree,*/ &m_DownloadList, &m_UploadList};
	Views[sel]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CLogViewer::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int sel = m_Tab.GetCurSel();
	CWnd* Views[] = {/*&m_Tree,*/ &m_DownloadList, &m_UploadList};
	Views[sel]->ShowWindow(SW_HIDE);
	*pResult = 0;
}

//DEL void CLogViewer::ParseLog()
//DEL {
//DEL 	for( POSITION pos = m_Log.GetHeadPosition(); pos; m_Log.GetNext(pos) )
//DEL 	{
//DEL 		CString &line = m_Log.GetAt(pos);
//DEL 		int idx = line.Find('>');
//DEL 		if( idx == -1 )
//DEL 			continue;
//DEL 		LPCTSTR pCmd = LPCTSTR(line)+idx+2;
//DEL 		if( 0 == strncmp( pCmd, "User c", 6) )	// user connecting...
//DEL 			AddUser( pos );
//DEL 		else if( 0 == strncmp( pCmd, "RETR", 4) )
//DEL 			AddFile( line , TRUE);
//DEL 		else if( 0 == strncmp( pCmd, "STOR", 4) )
//DEL 			AddFile( line , FALSE);
//DEL 	}
//DEL }

//DEL void CLogViewer::AddUser(POSITION StartPos)
//DEL {
//DEL 	CString& line = m_Log.GetAt(StartPos);
//DEL 	char* pline = (char*)LPCTSTR(line);
//DEL 	char* pTime=NULL;
//DEL 	char* pch=NULL;
//DEL 	if( pch = strchr(pline, ']') )
//DEL 	{
//DEL 		pch++;
//DEL 		*pch=0;
//DEL 		pTime = pline;
//DEL 	}
//DEL 	pch++;
//DEL 	char* pid = strchr(pch, '(')+1;
//DEL 	char* pUserName = pid+7;
//DEL 	char* pCmd = strchr(pUserName, '>');
//DEL 	HTREEITEM hUser = m_Tree.InsertItem( pUserName, 0, 0);
//DEL 	for( POSITION pos = StartPos; pos; m_Log.GetNext(pos) )
//DEL 	{
//DEL 		CString& line = m_Log.GetAt(pos);
//DEL 		char* _pid = strchr(line, '(');
//DEL 		if( !_pid || strncmp(_pid+1, pid, 5) )
//DEL 			continue;
//DEL 		pCmd = strchr(_pid, '>')+2;
//DEL 		if( !isdigit(*pCmd) )
//DEL 			m_Tree.InsertItem( line, 0, 0, hUser);
//DEL 		if( 0 == strncmp(pCmd, "User D",6) )
//DEL 			break;
//DEL 	}
//DEL }

void CLogViewer::AddFile(CString line, BOOL bDownload)
{
	CListCtrl* pList = bDownload ? &m_DownloadList : &m_UploadList;
	char date[40];
	char time[20];
	char user[1024];
	char file[1024];
	if( 4 != sscanf( LPCTSTR(line)+9, "%39s %19s %1023s %1023[^\r\n]s", date, time, user, file ) )
		return;
	strcat( date, " " );
	strcat( date, time );
	SHFILEINFO shinf;
	SHGetFileInfo(file,0,&shinf,sizeof(shinf),SHGFI_SYSICONINDEX|SHGFI_SMALLICON );
	int idx = pList->InsertItem(0, file, shinf.iIcon);
	pList->SetItemText(idx, 1, user);
	pList->SetItemText(idx, 2, date);
}

