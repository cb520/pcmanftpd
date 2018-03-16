; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CServerGeneralPage
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "pcmanftpd2.h"
LastPage=0

ClassCount=16
Class1=CInputDlg
Class2=CLogEdit
Class3=CLogView
Class4=CLogViewer
Class5=CMainFrm
Class6=CModalFrm
Class7=CApp
Class8=CServerGeneralPage
Class9=CServerIPFilterPage
Class10=CServerLogPage
Class11=CServerMsgPage
Class12=CSplitter
Class13=CUserDialog
Class14=CUserGeneralPage
Class15=CUserPermDlg

ResourceCount=14
Resource1=IDD_SERVER_IPFILTER (Neutral)
Resource2=IDD_SERVER_GENERAL (Neutral)
Resource3=IDD_SERVER_MSG (Neutral)
Resource4=IDD_USER (Neutral)
Resource5=IDD_USER_PERM (Neutral)
Resource6=IDD_ABOUTBOX (English (U.S.))
Resource7=IDD_ABOUTBOX (Neutral)
Resource8=IDD_INPUT (Neutral)
Resource9=IDD_LOG_VIEWER (Neutral)
Resource10=IDD_SERVER_LOG (Neutral)
Resource11=IDR_MAINFRAME (Neutral)
Resource12=IDD_USER_GENERAL (Neutral)
Resource13=IDR_POPUP (Neutral)
Resource14=IDR_MAINFRAME (Chinese (P.R.C.))
Class16=CServerThread

[CLS:CInputDlg]
Type=0
BaseClass=CDialog
HeaderFile=InputDlg.h
ImplementationFile=InputDlg.cpp
Filter=D
VirtualFilter=dWC

[CLS:CLogEdit]
Type=0
BaseClass=CRichEditCtrl
HeaderFile=LogEdit.h
ImplementationFile=LogEdit.cpp

[CLS:CLogView]
Type=0
BaseClass=CWnd
HeaderFile=LogView.h
ImplementationFile=LogView.cpp

[CLS:CLogViewer]
Type=0
BaseClass=CDialog
HeaderFile=LogViewer.h
ImplementationFile=LogViewer.cpp

[CLS:CMainFrm]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC

[CLS:CModalFrm]
Type=0
BaseClass=CFrameWnd
HeaderFile=ModalFrm.h
ImplementationFile=ModalFrm.cpp

[CLS:CApp]
Type=0
BaseClass=CWinApp
HeaderFile=PCManFTPD2.h
ImplementationFile=PCManFTPD2.cpp
LastObject=IDC_ADDDIR

[CLS:CServerGeneralPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=ServerGeneralPage.h
ImplementationFile=ServerGeneralPage.cpp
Filter=D
VirtualFilter=idWC
LastObject=CServerGeneralPage

[CLS:CServerIPFilterPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=ServerIPFilterPage.h
ImplementationFile=ServerIPFilterPage.cpp

[CLS:CServerLogPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=ServerLogPage.h
ImplementationFile=ServerLogPage.cpp
Filter=D
VirtualFilter=idWC

[CLS:CServerMsgPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=ServerMsgPage.h
ImplementationFile=ServerMsgPage.cpp

[CLS:CSplitter]
Type=0
BaseClass=CWnd
HeaderFile=Splitter.h
ImplementationFile=Splitter.cpp

[CLS:CUserDialog]
Type=0
BaseClass=CDialog
HeaderFile=UserDialog.h
ImplementationFile=UserDialog.cpp

[CLS:CUserGeneralPage]
Type=0
BaseClass=CPropertyPage
HeaderFile=UserGeneralPage.h
ImplementationFile=UserGeneralPage.cpp

[CLS:CUserPermDlg]
Type=0
BaseClass=CDialog
HeaderFile=UserPermDlg.h
ImplementationFile=UserPermDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CUserPermDlg

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308480
Control6=IDC_WEBSITE,button,1342242816

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_USERLIST_KICK
Command4=ID_VIEW_EDITSERVER
Command5=ID_VIEW_EDITUSER
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[DLG:IDD_ABOUTBOX (Neutral)]
Type=1
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308480
Control6=IDC_WEBSITE,button,1342242816

[DLG:IDD_INPUT (Neutral)]
Type=1
Class=?
ControlCount=4
Control1=IDC_TEXT,edit,1350631552
Control2=IDOK,button,1342373889
Control3=IDCANCEL,button,1342373888
Control4=65535,static,1342308480

[DLG:IDD_LOG_VIEWER (Neutral)]
Type=1
Class=?
ControlCount=3
Control1=IDC_TAB,SysTabControl32,1342177600
Control2=IDC_UPLOADLIST,SysListView32,1082196033
Control3=IDC_DOWNLOADLIST,SysListView32,1350631489

[DLG:IDD_SERVER_GENERAL (Neutral)]
Type=1
Class=CServerGeneralPage
ControlCount=30
Control1=IDC_STARTUP_WITH_GUI,button,1342242819
Control2=IDC_STATIC,static,1342308352
Control3=IDC_PORT,edit,1350639744
Control4=IDC_PORTSPIN,msctls_updown32,1342177334
Control5=IDC_TRAYICON,button,1342242819
Control6=IDC_STATIC,static,1342308352
Control7=IDC_PASVPORTMIN,edit,1350639744
Control8=IDC_PASVPORTMINSPIN,msctls_updown32,1342177334
Control9=IDC_PASVPORTMAX,edit,1350639744
Control10=IDC_PASVPORTMAXSPIN,msctls_updown32,1342177334
Control11=IDC_STATIC,static,1342308352
Control12=IDC_MAXCONPERIP,edit,1350639744
Control13=IDC_MAXCONPERIPSPIN,msctls_updown32,1342177334
Control14=IDC_STATIC,static,1342308352
Control15=IDC_MAXCON,edit,1350639744
Control16=IDC_MAXCONSPIN,msctls_updown32,1342177334
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_MAXTRYPERIP,edit,1082204288
Control21=IDC_MAXTRYPERIPSPIN,msctls_updown32,1073741878
Control22=IDC_STATIC,static,1073872896
Control23=IDC_STATIC,static,1073872896
Control24=IDC_STATIC,button,1342177287
Control25=IDC_TOTALMAXUSPEED,edit,1350639744
Control26=IDC_MAXUSPEEDSPIN2,msctls_updown32,1342177334
Control27=IDC_TOTALMAXDSPEED,edit,1350639744
Control28=IDC_MAXDSPEEDSPIN2,msctls_updown32,1342177334
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352

[DLG:IDD_SERVER_IPFILTER (Neutral)]
Type=1
Class=?
ControlCount=10
Control1=IDC_ALLOW_LIST,listbox,1352728835
Control2=IDC_ALLOW_EDIT,edit,1350631552
Control3=IDC_ADD_ALLOW,button,1342242816
Control4=IDC_DEL_ALLOW,button,1342242816
Control5=IDC_BAN_LIST,listbox,1352728835
Control6=IDC_BAN_EDIT,edit,1350631552
Control7=IDC_ADD_BAN,button,1342242816
Control8=IDC_DEL_BAN,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_SERVER_LOG (Neutral)]
Type=1
Class=?
ControlCount=10
Control1=IDC_LOGTOSCREEN,button,1342242819
Control2=IDC_STATIC,static,1342308352
Control3=IDC_MAXLINE,edit,1350639744
Control4=IDC_MAXLINESPIN,msctls_updown32,1342177334
Control5=IDC_LOGTOFILE,button,1342242819
Control6=IDC_VIEWLOG,button,1342242816
Control7=IDC_DELLOG,button,1073807360
Control8=IDC_LOGTRANSFERTOFILE,button,1342242819
Control9=IDC_VIEW_TRANSFERLOG,button,1342242816
Control10=IDC_DELTRANSFERLOG,button,1073807360

[DLG:IDD_SERVER_MSG (Neutral)]
Type=1
Class=?
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT,edit,1353781444

[DLG:IDD_USER (Neutral)]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_USERLIST,SysTreeView32,1350635568
Control4=IDC_ADD_USER,button,1342242816
Control5=IDC_ADD_GROUP,button,1342242816
Control6=IDC_DELETE,button,1342242816

[DLG:IDD_USER_PERM (Neutral)]
Type=1
Class=CUserPermDlg
ControlCount=25
Control1=IDC_DIRTREE,SysTreeView32,1350631479
Control2=IDC_FILELIST,SysListView32,1350631773
Control3=IDC_ADDDIR,button,1342242816
Control4=IDC_ADDVDIR,button,1342242816
Control5=IDC_ADDFILE,button,1342242816
Control6=IDC_RENAME,button,1342242816
Control7=IDC_REMOVE,button,1342242816
Control8=IDC_PERM_GROUP,button,1342177287
Control9=IDC_INHERIT,button,1342242819
Control10=IDC_READ,button,1342242819
Control11=IDC_WRITE,button,1342242819
Control12=IDC_DEL,button,1342242819
Control13=IDC_APPEND,button,1342242819
Control14=IDC_LIST,button,1342242819
Control15=IDC_MKDIR,button,1342242819
Control16=IDC_RMDIR,button,1342242819
Control17=IDC_SUBDIR,button,1342242819
Control18=IDC_DISPPATH,edit,1350633600
Control19=IDC_REALPATH,edit,1350633600
Control20=IDC_MODIFY_PATH,button,1342242816
Control21=IDOK,button,1342242816
Control22=IDCANCEL,button,1342242816
Control23=IDC_REALPATHSTATIC,static,1342308352
Control24=IDC_SPLITTER,static,1350696960
Control25=IDC_DISPPATHSTATIC,static,1342308352

[DLG:IDD_USER_GENERAL (Neutral)]
Type=1
Class=?
ControlCount=29
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_NAME,edit,1350631552
Control4=IDC_GROUPSTATIC,static,1342308352
Control5=IDC_GROUP,combobox,1344340227
Control6=IDC_PASSWD,edit,1350631552
Control7=IDC_ENABLED,button,1342242819
Control8=IDC_STATIC,button,1342177287
Control9=IDC_MAXUSPEED,edit,1350639744
Control10=IDC_MAXUSPEEDSPIN,msctls_updown32,1342177334
Control11=IDC_MAXDSPEED,edit,1350639744
Control12=IDC_MAXDSPEEDSPIN,msctls_updown32,1342177334
Control13=IDC_STATIC,button,1342177287
Control14=IDC_MAXCON,edit,1350639744
Control15=IDC_MAXCONSPIN,msctls_updown32,1342177334
Control16=IDC_STATIC,static,1342308352
Control17=IDC_TIMEOUT,edit,1350639744
Control18=IDC_TIMEOUTSPIN,msctls_updown32,1342177334
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1073872896
Control21=IDC_MAXCONPERIP,edit,1082204288
Control22=IDC_MAXCONPERIPSPIN,msctls_updown32,1073741878
Control23=IDC_STATIC,static,1073872896
Control24=IDC_PASSWD_CHECK,button,1342242819
Control25=IDC_MAXUSPEED_CHECK,button,1342242819
Control26=IDC_MAXDSPEED_CHECK,button,1342242819
Control27=IDC_MAXCON_CHECK,button,1342242819
Control28=IDC_TIMEOUT_CHECK,button,1342242819
Control29=IDC_FILE_PERM,button,1342242816

[MNU:IDR_MAINFRAME (Neutral)]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_APP_EXIT
Command4=ID_VIEW_EDITSERVER
Command5=ID_VIEW_EDITUSER
Command6=ID_USERLIST_KICK
Command7=ID_USERLIST_BAN
Command8=ID_TRANSFERLIST_KICK
Command9=ID_TRANSFERLIST_KICK_USER
Command10=ID_TRANSFERLIST_BAN
Command11=ID_EDIT_COPY
Command12=ID_EDIT_CLEAR_ALL
Command13=ID_LOG_OPENFILE
Command14=ID_VIEW_LOG
Command15=ID_APP_ABOUT
CommandCount=15

[MNU:IDR_POPUP (Neutral)]
Type=1
Class=?
Command1=IDC_ADD_USER
Command2=IDC_ADD_GROUP
Command3=IDC_DELETE
Command4=ID_BROWSE_FOLDER
Command5=ID_BROWSE_FILE
Command6=ID_SET_VIRTUAL
Command7=ID_APP_ABOUT
Command8=ID_APP_EXIT
Command9=ID_FILE_CLOSE
Command10=ID_FILE_OPEN
Command11=ID_OPEN_MAINWINDOW
CommandCount=11

[TB:IDR_MAINFRAME (Neutral)]
Type=1
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_USERLIST_KICK
Command4=ID_VIEW_EDITSERVER
Command5=ID_VIEW_EDITUSER
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME (Neutral)]
Type=1
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[CLS:CServerThread]
Type=0
HeaderFile=ServerThread.h
ImplementationFile=ServerThread.cpp
BaseClass=CWinThread
Filter=N
VirtualFilter=TC
LastObject=CServerThread

