// MainFrm.cpp : implementation of the CMainFrm class
//

#include "stdafx.h"

#include "MainFrm.h"

#include "resource.h"

#include "UserDialog.h"
#include "LogViewer.h"

//	#include "ServerThread.h"

#include "FtpCmdCon.h"
#include "FtpDataCon.h"

#include "AppConfig.h"

#include "TrayIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrm

//static const UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

IMPLEMENT_DYNAMIC(CMainFrm, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrm, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrm)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_COPYDATA()
	ON_COMMAND(ID_USERLIST_KICK, OnUserlistKick)
	ON_UPDATE_COMMAND_UI(ID_USERLIST_KICK, OnUpdateUserlistKick)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_WM_CLOSE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
	ON_COMMAND(ID_VIEW_EDITSERVER, OnViewEditServer)
	ON_COMMAND(ID_VIEW_EDITUSER, OnViewEditUser)
	ON_COMMAND(ID_USERLIST_BAN, OnUserListBan)
	ON_COMMAND(ID_VIEW_LOG, OnViewLog)
	ON_COMMAND(ID_LOG_OPENFILE, OnLogOpenFile)
	ON_COMMAND(ID_TRANSFERLIST_KICK, OnTransferlistKick)
	ON_UPDATE_COMMAND_UI(ID_TRANSFERLIST_KICK, OnUpdateTransferlistKick)
	ON_COMMAND(ID_OPEN_MAINWINDOW, OnOpenMainWindow)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_DESTROY()
	ON_COMMAND(ID_TRANSFERLIST_KICK_USER, OnTransferlistKickUser)
	ON_UPDATE_COMMAND_UI(ID_TRANSFERLIST_KICK_USER, OnUpdateTransferlistKickUser)
	ON_NOTIFY(NM_RCLICK, IDC_USERLIST, OnRClickUserList)
	ON_NOTIFY(NM_RCLICK, IDC_TRANSFERLIST, OnRClickTransferList)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_USERLIST_BAN, OnUpdateUserlistKick)
	ON_COMMAND(ID_TRANSFERLIST_BAN, OnTransferlistBan)
	ON_UPDATE_COMMAND_UI(ID_TRANSFERLIST_BAN, OnUpdateTransferlistBan)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_NOTIFY_ICON, OnNotifyIcon )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrm construction/destruction

const char CMainFrm::m_WndClassName[] = "PCManFTPD";

CMainFrm::CMainFrm()
{
#if defined(USE_SERVER_THREAD)
	m_pServerThread = NULL;
#endif
}

CMainFrm::~CMainFrm()
{

}

static UINT indicators[] =
{
	ID_SEPARATOR           // status line indicator
};

int CMainFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
/*	if (!m_wndSplitter.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
*/
	SetIcon(::LoadIcon( AfxGetInstanceHandle(), LPCTSTR(IDR_MAIN)), TRUE);
	SetIcon(::LoadIcon( AfxGetInstanceHandle(), LPCTSTR(IDR_MAIN)), FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		/*| CBRS_GRIPPER */| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	CBitmap bmp;
	bmp.LoadBitmap(IDR_MAINFRAME);
	m_wndToolBar.SetBitmap((HBITMAP)bmp.m_hObject);
	m_wndToolBar.SetSizes( CSize(32+8,32+8),CSize(32,32));
	bmp.Detach();

	CRect rc(0,0,0,0);
	if( !m_UserList.CreateEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,NULL,
		WS_VISIBLE|WS_CHILD|LVS_REPORT|WS_HSCROLL|WS_VSCROLL,
		rc,this,IDC_USERLIST) )
		return -1;
	ListView_SetExtendedListViewStyle(m_UserList.m_hWnd,LVS_EX_FULLROWSELECT);


	if( !m_TransferList.CreateEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,NULL,
		WS_VISIBLE|WS_CHILD|LVS_REPORT|WS_HSCROLL|WS_VSCROLL,
		rc,this,IDC_TRANSFERLIST) )
		return -1;
	ListView_SetExtendedListViewStyle(m_TransferList.m_hWnd,LVS_EX_FULLROWSELECT);


	CString str;
	str.LoadString(IDS_USER);
	m_UserList.InsertColumn(0,str,LVCFMT_LEFT,100);
	str.LoadString(IDS_IP_ADDRESS);
	m_UserList.InsertColumn(1,str,LVCFMT_LEFT,100);
	str.LoadString(IDS_ID);
	m_UserList.InsertColumn(2,str,LVCFMT_LEFT,80);

	str.LoadString(IDS_USER);
	m_TransferList.InsertColumn(0,str,LVCFMT_LEFT,100);
	str.LoadString(IDS_IP_ADDRESS);
	m_TransferList.InsertColumn(1,str,LVCFMT_LEFT,80);
	str.LoadString(IDS_TRANSFER);
	m_TransferList.InsertColumn(2,str,LVCFMT_LEFT,80);
	str.LoadString(IDS_TRANSFER_SPEED);
	m_TransferList.InsertColumn(3,str,LVCFMT_LEFT,90);
	str.LoadString(IDS_PROGRESS);
	m_TransferList.InsertColumn(4,str,LVCFMT_LEFT,80);
	str.LoadString(IDS_FILE_NAME);
	m_TransferList.InsertColumn(5,str,LVCFMT_LEFT,300);

	if( !m_LogEdit.CreateEx(WS_EX_CLIENTEDGE,RICHEDIT_CLASS,NULL,
		WS_VISIBLE|WS_CHILD|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL|
		WS_HSCROLL|WS_VSCROLL|ES_READONLY|WS_BORDER|ES_LEFT|ES_SUNKEN|ES_DISABLENOSCROLL 
		,rc,this,IDC_LOGEDIT) )
		return -1;

	if( !m_wndBottomSplitter.Create(this, &m_UserList, &m_LogEdit, 220) )
		return -1;

	if( !m_wndMainSplitter.Create(this, &m_TransferList, &m_wndBottomSplitter, 140, FALSE) )
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)) )
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);

	CFont fnt;
	fnt.Attach(GetStockObject(DEFAULT_GUI_FONT));
	m_LogEdit.SetFont(&fnt);
	fnt.Detach();

	if( AppConfig.m_TrayIcon )
	{
		CTrayIcon ti( m_hWnd, 10 );
		ti.Add( AppConfig.m_NormalIcon, "PCMan's FTP Server" );
	}

	return 0;
}

BOOL CMainFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style = cs.style|WS_CLIPCHILDREN;

	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.style = CS_VREDRAW|CS_HREDRAW;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hInstance = cs.hInstance = AfxGetInstanceHandle();
	wc.hIcon = NULL;
	wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	cs.lpszClass = wc.lpszClassName = m_WndClassName;
	RegisterClass(&wc);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrm diagnostics

#ifdef _DEBUG
void CMainFrm::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrm::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrm message handlers
void CMainFrm::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
//	m_wndView.SetFocus();
}

BOOL CMainFrm::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
//	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrm::OnSize(UINT nType, int cx, int cy) 
{
	if( nType == SIZE_MINIMIZED )
		return;
	CFrameWnd::OnSize(nType, cx, cy);
	CSize sz=m_wndToolBar.CalcFixedLayout(TRUE,TRUE);
	m_wndMainSplitter.SetWindowPos(&CWnd::wndTop,0,sz.cy,cx,cy-sz.cy-20,SWP_NOZORDER);
	m_wndStatusBar.SetWindowPos(&CWnd::wndTop,0,cy-20,cx,20,SWP_NOZORDER);
}

void CMainFrm::OnRClickUserList(NMHDR *phdr, LRESULT *r)
{
	CPoint pt;	GetCursorPos(&pt);
	GetMenu()->GetSubMenu(1)->GetSubMenu(3)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	*r=0;
}

void CMainFrm::OnRClickTransferList(NMHDR *phdr, LRESULT *r)
{
	CPoint pt;	GetCursorPos(&pt);
	GetMenu()->GetSubMenu(1)->GetSubMenu(4)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	*r=0;
}

BOOL CMainFrm::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* cs) 
{
/*	switch( cs->dwData )
	{
	case FTPD_ADD_USER:
		{
			int idx=m_UserList.GetItemCount();
			if(idx>0)	idx--;
			m_UserList.InsertItem(idx,NULL);
			char* pstr=(char*)cs->lpData;
			for(int i=0;i<3;i++)
			{
				m_UserList.SetItemText(idx,i,pstr);
				pstr+=strlen(pstr)+1;
			}
			break;
		}
	case FTPD_LOG_RECV:
		WriteLog( RGB(0,0,128) ,(char*)cs->lpData);
		break;
	case FTPD_LOG_SEND:
		WriteLog( RGB(0,128,0) ,(char*)cs->lpData);
		break;
	}
*/
	return TRUE;
}

void CMainFrm::PostNcDestroy() 
{
	CFrameWnd::PostNcDestroy();
}

void CMainFrm::WriteLog(COLORREF Color, const char *Text)
{
	m_LogEdit.SetOptions( ECOOP_OR, ECO_AUTOVSCROLL );

	int lc = m_LogEdit.GetLineCount()-1;
	if(lc<0)	lc=0;
	int li=m_LogEdit.LineIndex(lc);
	lc=m_LogEdit.LineLength(lc)-1;
	if(lc<0)	lc=0;
	li+=lc;
	m_LogEdit.SetSel(li,li);
	CHARFORMAT cf;	cf.cbSize=sizeof(cf);	cf.dwEffects=0;
	cf.bCharSet=/*CHINESEBIG5_CHARSET*/ DEFAULT_CHARSET;
	cf.dwMask=CFM_COLOR|CFM_CHARSET;	cf.crTextColor=Color;
	m_LogEdit.SetSelectionCharFormat(cf);
	m_LogEdit.ReplaceSel(Text);
	lc=m_LogEdit.GetLineCount();
	lc-= AppConfig.m_ScreenMaxLine;
	if( lc > 0 )
	{
		m_LogEdit.SetSel(0,m_LogEdit.LineIndex(lc-1));
		m_LogEdit.ReplaceSel(NULL);
	}

	if( IsWindowVisible() )
		m_LogEdit.ScrollToEnd();
}

void CMainFrm::OnUserlistKick() 
{
	if( !m_UserList.GetFirstSelectedItemPosition() )
		return;

	POSITION pos=m_UserList.GetFirstSelectedItemPosition();
	while(pos)
	{
		CFtpCmdCon* pCmdCon = (CFtpCmdCon*)m_UserList.GetItemData( int(pos)-1 );
		pCmdCon->KickUser();
//s		delete pCmdCon;	// done in KickUser
		m_UserList.GetNextSelectedItem(pos);
	}
}

void CMainFrm::OnUpdateUserlistKick(CCmdUI* pCmdUI) 
{
	if( pCmdUI->m_pMenu )
		pCmdUI->Enable( !!m_UserList.GetFirstSelectedItemPosition() );
}

void CMainFrm::OnFileOpen() 
{
#if defined(USE_SERVER_THREAD)
	if( m_pServerThread )
		return;
	m_pServerThread = AfxBeginThread( RUNTIME_CLASS(CServerThread), THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED  );
	m_pServerThread->m_pMainWnd = this;
	m_pServerThread->ResumeThread();
#else
	if( m_Server.IsValid() )
		return;
	m_Server.Start();
#endif

	DisplayServerOnline(true);
}

void CMainFrm::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	if( pCmdUI->m_pMenu )
		pCmdUI->Enable( !m_Server.IsValid() );
}

void CMainFrm::OnFileClose() 
{
#if defined(USE_SERVER_THREAD)
	if( !m_pServerThread )
		return;

	m_pServerThread->PostThreadMessage( WM_QUIT, 0, 0 );
	m_pServerThread = NULL;
#else
	if( !m_Server.IsValid() )
		return;
	m_Server.Stop();
#endif

	m_UserList.DeleteAllItems();
	DisplayServerOnline( false );
}

void CMainFrm::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	if( pCmdUI->m_pMenu )
		pCmdUI->Enable( m_Server.IsValid() );
}

void CMainFrm::OnClose() 
{
	ShowWindow( SW_HIDE );
}

void CMainFrm::OnEditCopy() 
{
	m_LogEdit.Copy();
}

void CMainFrm::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	long s,e;
	m_LogEdit.GetSel(s,e);
	pCmdUI->Enable( s!=e );//m_LogEdit.GetSelectionType()&SEL_EMPTY );
}

void CMainFrm::OnEditClearAll() 
{
	m_LogEdit.SetWindowText(NULL);
}

void CMainFrm::OnUpdateEditClearAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_LogEdit.GetTextLength()>0 );	
}

//DEL void CMainFrm::OnEditFind() 
//DEL {
//DEL 	if(!m_pFindDlg)
//DEL 	{
//DEL 		m_pFindDlg = new CFindReplaceDialog;
//DEL 		m_pFindDlg->Create(TRUE,NULL);
//DEL 		m_pFindDlg->ShowWindow(SW_SHOW);
//DEL 	}
//DEL }

//DEL LRESULT CMainFrm::OnEditFindNext(WPARAM wp, LPARAM lp)
//DEL {
//DEL 	if(m_pFindDlg->IsTerminating() )
//DEL 	{
//DEL 		m_pFindDlg=NULL;
//DEL 		return 0;
//DEL 	}
//DEL 
//DEL 	FINDTEXTEX FindText;
//DEL 	CHARRANGE chr;
//DEL 	m_LogEdit.GetSel(chr);
//DEL 	if( m_pFindDlg->SearchDown() )
//DEL 	{
//DEL 		FindText.chrg.cpMin=chr.cpMax;
//DEL 		FindText.chrg.cpMax=-1;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		FindText.chrg.cpMin=-1;
//DEL 		FindText.chrg.cpMax=chr.cpMin;	
//DEL 	}
//DEL 
//DEL 	CString str=m_pFindDlg->GetFindString();
//DEL 	FindText.lpstrText=str;
//DEL 	if( m_LogEdit.FindText(
//DEL 		(m_pFindDlg->MatchCase()?FR_MATCHCASE:0)|
//DEL 		(m_pFindDlg->MatchWholeWord()?FR_WHOLEWORD:0),
//DEL 		&FindText) != -1)
//DEL 		m_LogEdit.SetSel(FindText.chrgText);
//DEL 	m_LogEdit.SetFocus();
//DEL 	return 0;
//DEL }

void CMainFrm::OnViewEditServer() 
{
	CPropertySheet dlg(IDS_EDIT_SERVER_TITLE);
	CServerGeneralPage General;
	CServerMsgPage Msg;
	CServerLogPage Log;
	CServerIPFilterPage IPFilter;
	dlg.AddPage(&General);
	dlg.AddPage(&Msg);
	dlg.AddPage(&IPFilter);
	dlg.AddPage(&Log);

	General.m_StartupWithGUI = AppConfig.m_StartupWithGUI;
	General.m_TrayIcon = AppConfig.m_TrayIcon;
	
	USHORT oldPort = General.m_Port = AppConfig.m_Port;
	USHORT old_pasvmin = AppConfig.m_PasvPortMin;

	General.m_MaxConPerIp = AppConfig.m_MaxConPerIp;
	General.m_MaxCon = AppConfig.m_MaxCon;
	General.m_EnablePasv = AppConfig.m_EnablePasv;
	General.m_PasvPortMin = AppConfig.m_PasvPortMin;
	General.m_PasvPortMax = AppConfig.m_PasvPortMax;
	General.m_MaxTryPerIp = AppConfig.m_MaxTryPerIp;

	General.m_MaxUSpeed = AppConfig.m_MaxUSpeed == DWORD(-1) ? 0 : AppConfig.m_MaxUSpeed/1024;
	General.m_MaxDSpeed = AppConfig.m_MaxDSpeed == DWORD(-1) ? 0 : AppConfig.m_MaxDSpeed/1024;

	Log.m_LogToFile = AppConfig.m_LogToFile;
	Log.m_LogTransferToFile = AppConfig.m_LogTransferToFile;
	Log.m_LogToScreen = AppConfig.m_LogToScreen;
	Log.m_ScreenMaxLine = AppConfig.m_ScreenMaxLine;

	CFile file;
	if( file.Open(AppPath+"WelcomeMsg.ini",CFile::modeRead) )
	{
		char* pstr = Msg.m_MsgStr.GetBuffer(1024);
		int len = file.Read(pstr,1024);
		file.Close();
		pstr[len] = 0;
		Msg.m_MsgStr.ReleaseBuffer();
	}

	if(dlg.DoModal() != IDOK )
		return;

	AppConfig.m_StartupWithGUI = General.m_StartupWithGUI;

	if( AppConfig.m_TrayIcon && !General.m_TrayIcon )
	{
		CTrayIcon ti( m_hWnd, 10 );
		ti.Remove();
	}
	else if( General.m_TrayIcon )
	{
		CTrayIcon ti( m_hWnd, 10 );	
		ti.Add( AppConfig.m_CurIcon, "PCMan's FTP Server" );
	}

	AppConfig.m_TrayIcon = General.m_TrayIcon;
	AppConfig.m_Port = General.m_Port;
	AppConfig.m_MaxConPerIp = General.m_MaxConPerIp;
	AppConfig.m_MaxCon = General.m_MaxCon;
	AppConfig.m_EnablePasv = General.m_EnablePasv;
	AppConfig.m_PasvPortMin = General.m_PasvPortMin;
	AppConfig.m_PasvPortMax = General.m_PasvPortMax;
	AppConfig.m_MaxTryPerIp = General.m_MaxTryPerIp;

	AppConfig.m_MaxUSpeed = General.m_MaxUSpeed;
	AppConfig.m_MaxDSpeed = General.m_MaxDSpeed;

	AppConfig.m_LogToFile = Log.m_LogToFile;
	AppConfig.m_LogTransferToFile = Log.m_LogTransferToFile;
	AppConfig.m_LogToScreen = Log.m_LogToScreen;
	AppConfig.m_ScreenMaxLine = Log.m_ScreenMaxLine;

	AppConfig.m_WelcomeMsg = Msg.m_MsgStr;
	AppConfig.Save();

	AppConfig.m_MaxUSpeed	= AppConfig.m_MaxUSpeed ? AppConfig.m_MaxUSpeed*1024 : DWORD(-1);
	AppConfig.m_MaxDSpeed	= AppConfig.m_MaxDSpeed ? AppConfig.m_MaxDSpeed*1024 : DWORD(-1);

	AppConfig.LoadWelcomeMsg();

	AppConfig.InitServerLog();
	AppConfig.InitTransferLog();

	if( old_pasvmin != AppConfig.m_PasvPortMin )
		CFtpDataCon::Init();

	if( oldPort != AppConfig.m_Port )
	{
		m_Server.Stop();
		m_Server.Start();
	}
//	::SendMessage(m_hFTPDWnd, WM_FTPD_CMD, FTPD_RELOAD_SERVER, 0);
}

//DEL void CMainFrm::LoadAppConfig()
//DEL {
//DEL 	CString CfgPath=AppPath+"Server.ini";
//DEL 	const char* pCfgPath=CfgPath;
//DEL 	m_StartupWithGUI=(BOOL)GetPrivateProfileInt("Server","StartupWithGUI",0,pCfgPath);
//DEL 	m_Port=(USHORT)GetPrivateProfileInt("Server","Port",21,pCfgPath);
//DEL 	m_PasvPortMax=(USHORT)GetPrivateProfileInt("Server","PasvPortMax",2248,pCfgPath);
//DEL 	m_PasvPortMin=(USHORT)GetPrivateProfileInt("Server","PasvPortMin",2048,pCfgPath);
//DEL 	m_LogToFile=(BOOL)GetPrivateProfileInt("Server","LogToFile",1,pCfgPath);
//DEL 	m_LogToScreen=(BOOL)GetPrivateProfileInt("Server","LogToScreen",1,pCfgPath);
//DEL 	m_MaxCon=(UINT)GetPrivateProfileInt("Server","MaxCon",0,pCfgPath);
//DEL 	m_MaxConPerIp=(UINT)GetPrivateProfileInt("Server","MaxConPerIp",0,pCfgPath);
//DEL 	m_MaxTryPerIp=(UINT)GetPrivateProfileInt("Server","MaxTryPerIp",0,pCfgPath);
//DEL 	m_TrayIcon=(BOOL)GetPrivateProfileInt("Server","TrayIcon",1,pCfgPath);
//DEL 
//DEL 	CFile file;
//DEL 	if( file.Open(AppPath+"WelcomeMsg.ini",CFile::modeRead) )
//DEL 	{
//DEL 		char* pstr=m_MsgStr.GetBuffer(1024);
//DEL 		int len=file.Read(pstr,1024);
//DEL 		file.Close();
//DEL 		pstr[len]=0;
//DEL 		m_MsgStr.ReleaseBuffer();
//DEL 	}
//DEL 
//DEL }

void CMainFrm::OnViewEditUser() 
{
//	CString str;
//	str.LoadString(IDR_USERCONFIG);
//	CUserPropertySheet dlg(str);
	CUserDialog dlg;
	if( dlg.DoModal() == IDOK)
		dlg.m_hWnd;//::SendMessage(m_hFTPDWnd, WM_FTPD_CMD, FTPD_RELOAD_USERS, 0);
/*	CModalFrm* pFrm = new CModalFrm;
	pFrm->LoadFrame(IDR_USERCONFIG,WS_OVERLAPPEDWINDOW,this);
	BeginModalState();
	pFrm->ShowWindow(SW_SHOW);
*/
}

void CMainFrm::DisplayServerOnline(bool online)
{
	CTime time = CTime::GetCurrentTime();
	const char* ip =  CSock::GetLocalIp();
	CString line = time.Format("%Y/%m/%d [%H:%M]");
	line += " Server ";
	if( online )
	{
		line += "Online -  ";
		line += ip;
	}
	else
		line += "Offline";

	line += "\r\n\r\n";

	WriteLog(0, line);

	DWORD wlen;
	if( AppConfig.m_LogFile != INVALID_HANDLE_VALUE )
		WriteFile( AppConfig.m_LogFile, LPCTSTR(line), line.GetLength(), &wlen, NULL);

	CString title("PCMan's FTP Server ");
	title += (online ? "[Online] - " : "[Offline] - ");
	title += ip;

	SetWindowText( title );

	if( online )
		AppConfig.SetNormalIcon();
	else
		AppConfig.SetOfflineIcon();
}

void CMainFrm::OnUserListBan() 
{
	POSITION pos=m_UserList.GetFirstSelectedItemPosition();
	while(pos)
	{
		CFtpCmdCon* pCmdCon = (CFtpCmdCon*)m_UserList.GetItemData( int(pos)-1 );
		pCmdCon->BanUser();
		m_UserList.GetNextSelectedItem(pos);
	}
}

void CMainFrm::OnViewLog() 
{
	CLogViewer viewer;
	viewer.DoModal();
}

void CMainFrm::OnLogOpenFile() 
{
	ShellExecute(m_hWnd,"open",AppConfig.GetServerLogPath(),NULL,AppPath,SW_SHOWMAXIMIZED);
}

void CMainFrm::AddUser( void* id, CString name, CString address )
{
	int idx = m_UserList.GetItemCount();
	if( idx > 0 )
		idx--;
	m_UserList.InsertItem( idx, NULL );
	m_UserList.SetItemData( idx, (DWORD)id );
	m_UserList.SetItemText( idx, 0, name );
	m_UserList.SetItemText( idx, 1, address );
	char tmp[10];
	_itoa( int(((CFtpCmdCon*)id)->GetSock()), tmp, 10 );
	m_UserList.SetItemText( idx, 2, tmp );
}

void CMainFrm::AddTransfer(void* id, CString user, bool download, CString file)
{
	int idx = m_TransferList.GetItemCount();
	if( idx > 0 )
		idx--;
	m_TransferList.InsertItem(idx, NULL);
	m_TransferList.SetItemData( idx, (DWORD)id );
	m_TransferList.SetItemText( idx, 0, user );
	CString str;
	m_TransferList.SetItemText(idx, 1, ((CFtpDataCon*)id)->GetAddress() );
	str.LoadString(download ? IDS_DOWNLOAD : IDS_UPLOAD );
	m_TransferList.SetItemText(idx, 2, str );
	m_TransferList.SetItemText(idx, 3, "0 KB/s");
	m_TransferList.SetItemText(idx, 4, "");
	m_TransferList.SetItemText(idx, 5, file);
}

void CMainFrm::RemoveUser(void* id)
{
	LVFINDINFO inf;
	inf.lParam = (LPARAM)id;
	inf.flags = LVFI_PARAM;
	int idx = m_UserList.FindItem( &inf );
	if( idx >= 0 )
		m_UserList.DeleteItem( idx );
}

void CMainFrm::RemoveTransfer(void* id)
{
	LVFINDINFO inf;
	inf.lParam = (LPARAM)id;
	inf.flags = LVFI_PARAM;
	int idx = m_TransferList.FindItem( &inf );
	if( idx >= 0 )
		m_TransferList.DeleteItem( idx );
}

static void FileSizeToStr( char* buf, __int64 size, char* tail="" )
{
//	size = 109951162777600L;

	__int64 size1 = size;
	__int64 size2;
	char* units[] = { "TB", "GB", "MB", "KB" };
	char* unit = units[2];

	if( size >= 1073741824 )	// 1 GB
	{
		if( size >= 1099511627776L )	// 1 TB
		{
			size2 = size1 % 1099511627776L;
			size1 /= 1099511627776L;
			size2 /= 109951162777L;
			unit = units[0];
		}
		else
		{
			size2 = size1 % 1073741824;
			size1 /= 1073741824;
			size2 /= 10737418;
			unit = units[1];
		}
	}
	else if( size >= 1048576 )	// 1 MB
	{
		size2 = size1 % 1048576;
		size1 /= 1048576;
		size2 /= 104858;
	}
	else	// KB
	{
		size2 = size1 % 1024;
		size1 /= 1024;
		size2 /= 102;
		unit = units[3];
	}
	sprintf( buf, "%I64u.%I64u %s%s", size1, size2, unit, tail );
}


void CMainFrm::UpdateTransfer( void* id, DWORD speed/*, int ratio*/ )
{
	LVFINDINFO inf;
	inf.lParam = (LPARAM)id;
	inf.flags = LVFI_PARAM;
	CFtpDataCon* con = (CFtpDataCon*)id;
	int idx = m_TransferList.FindItem( &inf );
//	if( idx >= 0 )
//	{
		char text[20];
		FileSizeToStr( text, speed, "/s" );
		m_TransferList.SetItemText(idx, 3, text);
		if( con->GetType() == SEND_FILE )
		{
			double ratio = double(con->GetTransferredBytes()) / con->GetTotalSize();
			sprintf( text, "%d %%", (int)(ratio*100) );
			m_TransferList.SetItemText(idx, 4, text);
		}
		else
		{
			FileSizeToStr( text, con->GetTransferredBytes() );
			m_TransferList.SetItemText(idx, 4, text);
		}		
//	}
}


void CMainFrm::OnTransferlistKick() 
{
	if( !m_TransferList.GetFirstSelectedItemPosition() )
		return;

	POSITION pos=m_TransferList.GetFirstSelectedItemPosition();
	while(pos)
	{
		CFtpDataCon* pDataCon = (CFtpDataCon*)m_TransferList.GetItemData( int(pos)-1 );
//		if( pDataCon->GetCmdCon() && pDataCon->GetCmdCon()->GetDataCon() == pDataCon )
//			pDataCon->GetCmdCon()->SetDataCon(NULL);
		delete pDataCon;
		m_TransferList.GetNextSelectedItem(pos);
	}	
}

void CMainFrm::OnUpdateTransferlistKick(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !!m_TransferList.GetFirstSelectedItemPosition() );	
}

LRESULT CMainFrm::OnNotifyIcon(WPARAM wp, LPARAM lp)
{
	if( lp == WM_LBUTTONUP )
	{
		if( IsWindowVisible() )
			ShowWindow( SW_HIDE );
		else
		{
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			m_LogEdit.ScrollToEnd();
		}
	}
	else if( lp == WM_RBUTTONUP )
	{
		CMenu pop;
		pop.LoadMenu( IDR_POPUP );
		SetForegroundWindow();
		POINT pt;	GetCursorPos(&pt);
		pop.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
		PostMessage(WM_NULL, 0, 0);
	}
	return 0;
}

void CMainFrm::OnOpenMainWindow() 
{
	ShowWindow(SW_SHOW);	
}

void CMainFrm::OnAppExit() 
{
	if( AfxMessageBox(IDS_QUERY_QUIT,MB_YESNO|MB_ICONQUESTION)!=IDYES )
		return;

	OnFileClose();
	DestroyWindow();
}


void CMainFrm::OnDestroy() 
{
	CTrayIcon ti(m_hWnd, 10);
	ti.Remove();
	CFrameWnd::OnDestroy();
}

void CMainFrm::OnTransferlistKickUser() 
{
	if( !m_TransferList.GetFirstSelectedItemPosition() )
		return;

	POSITION pos=m_TransferList.GetFirstSelectedItemPosition();
	while(pos)
	{
		CFtpDataCon* pDataCon = (CFtpDataCon*)m_TransferList.GetItemData( int(pos)-1 );
		if( pDataCon->GetCmdCon() )
			delete pDataCon->GetCmdCon();
		m_TransferList.GetNextSelectedItem(pos);
	}
}

void CMainFrm::OnUpdateTransferlistKickUser(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !!m_TransferList.GetFirstSelectedItemPosition() );	
}

void CMainFrm::OnTransferlistBan() 
{
	POSITION pos=m_TransferList.GetFirstSelectedItemPosition();
	while(pos)
	{
		CFtpDataCon* pDataCon = (CFtpDataCon*)m_TransferList.GetItemData( int(pos)-1 );
		if( pDataCon->GetCmdCon() )
			pDataCon->GetCmdCon()->BanUser();
		m_TransferList.GetNextSelectedItem(pos);
	}
}

void CMainFrm::OnUpdateTransferlistBan(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !!m_TransferList.GetFirstSelectedItemPosition() );	
}

