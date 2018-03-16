// UserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "UserDialog.h"
#include "IniFile.h"
#include "resource.h"
#include "FtpUser.h"
#include "FtpCmdCon.h"
#include "FtpDataCon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserDialog dialog


CUserDialog::CUserDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDialog::IDD, pParent)
{
	m_pUser=NULL;
	m_pCurPage=NULL;
	m_CurGroup=-1;
	m_GeneralPage.m_pDlg=this;
//	m_ImgList.Create(IDB_USERIMG,16,0,RGB(255,0,255));
	m_ImgList.Create(24,24,ILC_COLOR8,2,0);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_USERIMG);
	m_ImgList.Add( &bmp, RGB(255,0,255));
	bmp.Detach();
	//{{AFX_DATA_INIT(CUserDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserDialog)
	DDX_Control(pDX, IDC_USERLIST, m_UserList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserDialog, CDialog)
	//{{AFX_MSG_MAP(CUserDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_USERLIST, OnSelchangedUserlist)
	ON_NOTIFY(TVN_SELCHANGING, IDC_USERLIST, OnSelchangingUserlist)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_ADD_USER, OnAddUser)
	ON_BN_CLICKED(IDC_ADD_GROUP, OnAddGroup)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserDialog message handlers

BOOL CUserDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Sheet.AddPage(&m_GeneralPage);
//	m_Sheet.AddPage(&m_AccessPage);
	m_Sheet.Create(this,WS_CHILD|WS_VISIBLE|DS_CONTROL,0);

	CRect rc,rc2;
	m_Sheet.GetWindowRect(rc);

	int tabw=rc.Width()+12;	int tabh=rc.Height();

	CRect tree_rc;
	m_UserList.GetWindowRect(tree_rc);
	ScreenToClient(tree_rc);

	rc.right=tree_rc.Width()+tabw;
	rc.left=0;	rc.top=0;		rc.bottom=tabh+40;
	CalcWindowRect(rc);

	DLGLAYOUT DlgLayoutData[]={
		{IDC_USERLIST,		DLA_TOP|DLA_LEFT|DLA_BOTTOM},
		{IDC_ADD_USER,		DLA_LEFT|DLA_BOTTOM},
		{IDC_ADD_GROUP,		DLA_LEFT|DLA_BOTTOM},
		{IDC_DELETE,		DLA_LEFT|DLA_BOTTOM},
		{IDOK,				DLA_RIGHT|DLA_BOTTOM},
		{IDCANCEL,			DLA_RIGHT|DLA_BOTTOM},
	};

	CDlgLayout DlgLayout;
	DlgLayout.Load(m_hWnd, DlgLayoutData, sizeof(DlgLayoutData)/sizeof(DLGLAYOUT));
	MoveWindow(rc);
	GetClientRect(rc);
	DlgLayout.OnSize(0,rc.Width(),rc.Height());

	m_Sheet.SetWindowPos(&CWnd::wndNoTopMost,tree_rc.right,tree_rc.top,0,0,SWP_NOSIZE|SWP_NOZORDER);
	CenterWindow();
	LoadUsers();
	return TRUE;
}

void CUserDialog::LoadUsers()
{
	CString str;

	str.LoadString(IDS_GROUP_NONE);
	m_GeneralPage.m_GroupCombo.AddString(str);	//Add "None" to Group Combo Box

	str.LoadString(IDS_GROUPS);
	m_groot=m_UserList.InsertItem(str,1,1);
	str.LoadString(IDS_USERS);
	m_uroot=m_UserList.InsertItem(str,0,0);

	char* fn[]={"Groups.ini","Users.ini"};
	char* buf=new char[2048];
	char* val=buf+1024;
	CString path;
	for(int i=0;i<2;i++)
	{
		path=AppPath+fn[i];
		GetPrivateProfileSectionNames(buf,1024,path);
		for( char* pbuf=buf; *pbuf; pbuf+=(strlen(pbuf)+1) )
		{
			CFtpUserEx* puser=new CFtpUserEx;
			if(puser)
			{
				puser->m_IsUser= (i==1) ;	// Is it a group or a user?;

				(puser->m_IsUser?m_Users:m_Groups).Add(puser);
				puser->LoadUser(pbuf, path,val);
				if( puser->m_IsUser && !puser->m_GroupName.IsEmpty() )
					puser->m_pParent = FindUser(puser->m_GroupName, false);

				m_UserList.InsertItem(TVIF_IMAGE|TVIF_PARAM|TVIF_TEXT|TVIF_SELECTEDIMAGE ,
					puser->m_UserName,!i,!i,0,0,LPARAM(puser),
					puser->m_IsUser?m_uroot:m_groot, TVI_LAST );

				if( !puser->m_IsUser )	// Add to Group Combo Box
					m_GeneralPage.m_GroupCombo.AddString(puser->m_UserName);
			}
		}
		m_UserList.SortChildren(i == 1 ? m_uroot:m_groot);
	}
	delete []buf;
	m_UserList.Expand(m_groot,TVE_EXPAND);
	m_UserList.Expand(m_uroot,TVE_EXPAND);

	HTREEITEM hitem=m_UserList.GetChildItem(m_groot);
	m_UserList.SetImageList( &m_ImgList, TVSIL_NORMAL);
	m_UserList.SelectItem(hitem?hitem:m_groot);
}

void CUserDialog::SaveUsers()
{
	CPtrArray* m_User_Groups[2] = { &m_Groups, &m_Users };
	CMapStringToPtr* map[] = { &CFtpUser::GetAllGroups(), &CFtpUser::GetAllUsers() };

	CIniFile file[2];
	char* FileName[] = {"Groups.ini","Users.ini"};
	char* DirName[] = {"Groups\\","Users\\"};
	for(int i=0;i < 2; i++)
	{
		if(!file[i].Open(AppPath + FileName[i],CFile::modeWrite|CFile::modeCreate))
			continue;
		int idx;
		for( idx=0; idx < (*m_User_Groups[i]).GetSize(); idx++ )
		{
			CFtpUserEx* puser=(CFtpUserEx*)(*m_User_Groups[i])[idx];
			if(puser)
				puser->SaveUser(file[i]);
		}
		file[i].Close();

		// Delete access settings of deleted users and groups.
		CFileFind finder;	BOOL bfound = finder.FindFile( AppPath + DirName[i] + "*.ini");
		while(bfound)
		{
			bfound = finder.FindNextFile();
			CString Name = finder.GetFileTitle();
			for( idx=(*m_User_Groups[i]).GetSize()-1; idx >=0 ; idx-- )
			{
				if( 0 == Name.CompareNoCase(((CFtpUserEx*)(*m_User_Groups[i])[idx])->m_UserName) )
					break;
			}
			if( idx <0 )
				DeleteFile( finder.GetFilePath() );
		}
		finder.Close();

		// Reload user settings for the server.
		POSITION pos = map[i]->GetStartPosition();
		while( pos )
		{
			CString key;
			CFtpUser* value;
			map[i]->GetNextAssoc( pos, key, (void*&)value );
			delete value;
		}
		map[i]->RemoveAll();

/*		for( idx = 0; idx < m_User_Groups[i]->GetSize(); ++idx )
		{
			CFtpUserEx* ptmp = (CFtpUserEx*)m_User_Groups[i]->GetAt(idx);
//			CFtpUser* pnew = new CFtpUser(*ptmp);
//			pnew->GetFilePerm() = ptmp->GetFilePerm();
//			map[i]->SetAt(ptmp->m_UserName, pnew );
			CFtpUser::LoadUser( ptmp->m_UserName, , );
		}
*/		
	}

	CFtpUser::LoadSettings();

	POSITION pos = CFtpCmdCon::GetFtpCmdConMap().GetStartPosition();
	while( pos )
	{
		SOCKET key;
		CFtpCmdCon* value;
		CFtpCmdCon::GetFtpCmdConMap().GetNextAssoc( pos, key, (CFtpSock*&)value );
		CFtpUser* puser;
		if( map[1]->Lookup( value->GetUserName(), (void*&)puser ) && puser->m_Enabled )
		{
			value->SetUser( puser );
			puser->SetUserCount( puser->GetUserCount() + 1 );
		}
		else
			value->KickUser();
	}

	pos = CFtpDataCon::GetFtpDataConMap().GetStartPosition();
	while( pos )
	{
		SOCKET key;
		CFtpDataCon* value;
		CFtpDataCon::GetFtpDataConMap().GetNextAssoc( pos, key, (CFtpSock*&)value );
		CFtpUser* puser;
		if( map[1]->Lookup( value->GetUserName(), (void*&)puser ) && puser->m_Enabled )
			value->SetUser( puser );
		else
			delete value;
	}

}

void CUserDialog::UpdateSelUser()
{
	HTREEITEM hitem=m_UserList.GetSelectedItem();
	m_pUser= hitem ? (CFtpUserEx*)m_UserList.GetItemData(hitem):NULL;
	if(m_pUser)
	{
		m_Sheet.ShowWindow(SW_SHOW);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_DELETE ),TRUE);
	}
	else
	{
		m_Sheet.ShowWindow(SW_HIDE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_DELETE ),FALSE);
	}
	if(IsWindow(m_GeneralPage.m_hWnd))
	{
		m_GeneralPage.UpdateData(FALSE);
		if(m_pUser)
		{
			int nshow=m_pUser->m_IsUser?SW_SHOW:SW_HIDE;
			::ShowWindow(::GetDlgItem(m_GeneralPage.m_hWnd,IDC_GROUP),nshow);
			::ShowWindow(::GetDlgItem(m_GeneralPage.m_hWnd,IDC_GROUPSTATIC),nshow);
			if( !m_pUser->m_IsUser )
			{
				m_CurGroup = m_GeneralPage.m_GroupCombo.FindStringExact(
					0,m_pUser->m_UserName);
			}
		}
	}
}

void CUserDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(::WindowFromPoint(point)==m_UserList.m_hWnd)
	{
		CMenu mnu;
		mnu.LoadMenu(IDR_POPUP);
		CPoint pt;	GetCursorPos(&pt);
		int x=pt.x,	y=pt.y;
		m_UserList.ScreenToClient(&pt);
		HTREEITEM hit=m_UserList.HitTest(pt);
		CMenu* pSub = mnu.GetSubMenu(0);
		if(hit)
		{
			m_UserList.SelectItem(hit);
			UpdateSelUser();
			if(!m_UserList.GetItemData(hit))
			{
//				pSub->EnableMenuItem(IDC_COPY,MF_BYCOMMAND|MF_GRAYED);
				pSub->EnableMenuItem(IDC_DELETE,MF_BYCOMMAND|MF_GRAYED);
			}
		}
		pSub->TrackPopupMenu(TPM_LEFTALIGN,x,y,this);
	}
	CDialog::OnContextMenu(pWnd,point);
}

void CUserDialog::OnSelchangedUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateSelUser();
	*pResult = 0;
}

void CUserDialog::OnSelchangingUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
	if(IsWindow(m_GeneralPage.m_hWnd))
		*pResult = !m_GeneralPage.UpdateData(TRUE);
}

void CUserDialog::OnOK() 
{
	if(IsWindow(m_GeneralPage.m_hWnd) && !m_GeneralPage.EndOK() )
		return;
	SaveUsers();
	CDialog::OnOK();
}

void CUserDialog::OnAddUser() 
{
	AddUser(m_uroot);
}

void CUserDialog::OnAddGroup() 
{
	AddUser(m_groot);
}

void CUserDialog::OnDelete() 
{
	if(AfxMessageBox(IDS_QUERYDEL,MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)
		return;

	HTREEITEM hItem=m_UserList.GetSelectedItem();
	HTREEITEM hNext=m_UserList.GetNextSiblingItem(hItem);
	if(!hNext)
		hNext=m_UserList.GetPrevSiblingItem(hItem);
	bool IsGroup = m_UserList.GetParentItem(hItem)==m_groot;
	if(IsGroup)
	{
		m_GeneralPage.m_GroupCombo.DeleteString( m_CurGroup );
	}

	CPtrArray& m_User_Groups = IsGroup ? m_Groups : m_Users;
	for(int i=0;i<m_User_Groups.GetSize();i++)
	{
		if( m_User_Groups[i] == m_pUser )
		{
			if( !m_pUser->m_IsUser )
			{
				CString GroupName_None;	GroupName_None.LoadString(IDS_GROUP_NONE);
				for(int j=0; j<m_Users.GetSize(); j++ )
				{
					if( 0 == m_pUser->m_UserName.CompareNoCase(
						((CFtpUserEx*)m_Users[j])->m_UserName) )
					{
						((CFtpUserEx*)m_Users[j])->m_UserName = GroupName_None;
					}
				}
			}
			delete m_pUser;
			m_pUser = NULL;
			m_User_Groups.RemoveAt(i);
			break;
		}
	}
	m_UserList.DeleteItem(hItem);
	if(hNext)
		m_UserList.SelectItem(hNext);
}


void CUserDialog::AddUser(HTREEITEM hParent)
{
	LRESULT r;
	OnSelchangingUserlist(NULL,&r);
	if(r)
		return;

/*	HTREEITEM hprev = m_UserList.GetChildItem(hParent);
	while( hrev && m_UserList.GetItemText(hprev).Compare() )
	{
	}
	if( !hprev )
		hprev = TVI_LAST;
*/
	CFtpUserEx* pUser=new CFtpUserEx;
	// No permission setting will cause access violation when QueryPerm get called.
	// There must be at least one permission object existing in the map.
	pUser->GetFilePerm()["/"] = new CFtpFilePerm("/", "");

	pUser->m_UserName="new";
	pUser->m_IsUser = (hParent==m_uroot);
	(pUser->m_IsUser ? m_Users : m_Groups).Add(pUser);
	int img=pUser->m_IsUser?0:1;
	HTREEITEM hItem=m_UserList.InsertItem(TVIF_IMAGE|TVIF_PARAM|TVIF_TEXT|TVIF_SELECTEDIMAGE ,
		pUser->m_UserName,img,img,0,0,LPARAM(pUser),
		hParent, TVI_LAST );
	if(!pUser->m_IsUser)
	{
		m_CurGroup = m_GeneralPage.m_GroupCombo.AddString("new");
	}
	m_UserList.SelectItem(hItem);
	CEdit* pEdit = (CEdit*)m_GeneralPage.GetDlgItem(IDC_NAME);
	pEdit->SetFocus();
	pEdit->SetSel(0,-1);
	UpdateSelUser();

	m_UserList.SortChildren(hParent);
}

void CUserDialog::PostNcDestroy() 
{
	CPtrArray* m_User_Groups[2] = { &m_Users, &m_Groups};
	for(int i=0;i < 2; i++)
	{
		for( int idx=0; idx < (*m_User_Groups[i]).GetSize(); idx++ )
		{
			CFtpUserEx* puser=(CFtpUserEx*)(*m_User_Groups[i])[idx];
			if(puser)
				delete puser;
		}
	}
	CDialog::PostNcDestroy();
}

CFtpUserEx* CUserDialog::FindUser(CString name, bool is_user)
{
	CPtrArray* array = is_user ? &m_Users : &m_Groups;
	for( int i = 0; i < array->GetSize(); ++i )
	{
		CFtpUserEx* user = (CFtpUserEx*)array->ElementAt(i);
		if( user->m_UserName == name )
			return user;
	}
	return NULL;
}
