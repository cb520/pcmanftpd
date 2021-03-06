// FtpCmdCon.cpp: implementation of the CFtpCmdCon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "appconfig.h"
#include "PCManFTPD2.h"
#include "FtpUser.h"
#include "FtpCmdCon.h"
#include "FtpDataCon.h"
#include "mbstring.h"

#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpCmdCon::CFtpCmdMapEnt CFtpCmdCon::m_CmdMap[]={
//File operation
	{"LIST",&CFtpCmdCon::OnLIST},
	{"PORT",&CFtpCmdCon::OnPORT},
	{"PASV",&CFtpCmdCon::OnPASV},
	{"PWD ",&CFtpCmdCon::OnPWD},
	{"CWD ",&CFtpCmdCon::OnCWD},
	{"CDUP",&CFtpCmdCon::OnCDUP},
	{"RETR",&CFtpCmdCon::OnRETR},
	{"STOR",&CFtpCmdCon::OnSTOR},
	{"APPE",&CFtpCmdCon::OnAPPE},
	{"REST",&CFtpCmdCon::OnREST},
	{"DELE",&CFtpCmdCon::OnDELE},
	{"RMD ",&CFtpCmdCon::OnRMD},
	{"MKD ",&CFtpCmdCon::OnMKD},
	{"RNFR",&CFtpCmdCon::OnRNFR},
	{"RNTO",&CFtpCmdCon::OnRNTO},
	{"ABOR",&CFtpCmdCon::OnABOR},
	{"TYPE",&CFtpCmdCon::OnTYPE},
	{"NLST",&CFtpCmdCon::OnNLST},
	{"SIZE",&CFtpCmdCon::OnSIZE},	// Non-standard but frequently used
//Misc
	{"NOOP",&CFtpCmdCon::OnNOOP},
	{"MODE",&CFtpCmdCon::OnMODE},
	{"STRU",&CFtpCmdCon::OnSTRU},
	{"STAT",&CFtpCmdCon::OnSTAT},
	{"QUIT",&CFtpCmdCon::OnQUIT},
	{"SYST",&CFtpCmdCon::OnSYST},
//Account
	{"USER",&CFtpCmdCon::OnUSER},
	{"PASS",&CFtpCmdCon::OnPASS},
	{NULL,NULL}
};

DWORD CFtpCmdCon::m_UserCount = 0;

CFtpCmdCon::CFtpCmdCon()
{
	m_pDataCon = NULL;
	m_pUser = NULL;
	m_WorkingDir = '/';
	m_Mode = '\0';
	m_Type = 'I';
	m_IdleTime=0;
	m_LoggedIn = false;
	m_Rest = 0;
}

CFtpCmdCon::~CFtpCmdCon()
{
	Close();
	if(m_pDataCon)
	{
		m_pDataCon->m_pCmdCon = NULL;
		delete m_pDataCon;
	}

//	if( m_pUser )
//		m_pUser->Release();
}

void CFtpCmdCon::OnSend(int code)
{

}

void CFtpCmdCon::OnClose(int code)
{
	WriteToLog("User Disconnected.\r\n");
	Close();
	delete this;
}

void CFtpCmdCon::OnConnect(int code)
{
	CString log("User connecting from ");
	log += m_Address;
	log += "\r\n";
	WriteToLog(log);
	PreLogin();
}

void CFtpCmdCon::PreLogin()
{
	SendStr(AppConfig.m_WelcomeMsg, false);
}

static inline CString get_masked_passwd(const char* passwd)
{
	CString ret = "PASS ";
	while(*passwd)
	{
		ret += '*';
		++passwd;
	}
	return ret;
}

void CFtpCmdCon::OnRecv(int code)
{
	m_IdleTime = 0;

	char buffer[4096+1];
	m_IdleTime = 0;
	int rlen = Recv(buffer,  sizeof(buffer)-1);
	buffer[rlen] = '\0';
	
	for(char* pCmdLine = strtok( buffer, "\r\n" ); pCmdLine; pCmdLine = strtok(NULL,"\r\n") )
	{
		for( int i=0; i < 4; i++ )
			pCmdLine[i] = toupper( pCmdLine[i] );

		if( *(DWORD*)pCmdLine == *(DWORD*)"PASS" )
		{
			CString pass = "PASS ";
			const char* passwd = (pCmdLine + 5);
			while(*passwd)
			{
				pass += '*';
				++passwd;
			}
			WriteToLog( pass );
		}
		else
				pCmdLine[500] = 0;
				WriteToLog(pCmdLine);

		if( pCmdLine[3] == 0 )
			pCmdLine[3] = ' ';

		ParseCmdLine(pCmdLine);
	}
}


bool CFtpCmdCon::ParseCmdLine(const char *pCmdLine)
{
	for( CFtpCmdMapEnt* ent = m_CmdMap; *ent->cmd; ent++ )
	{
		if( 0 == memcmp( ent->cmd, pCmdLine, 4) )
		{
			const char* param = pCmdLine + 4;
			while( *param==' ' && *param )
				param++;

			//	Not logged in: Access denied!
			if( !(m_LoggedIn) && memcmp(pCmdLine, "USER", 4) && memcmp(pCmdLine, "PASS", 4) )
			{
				SendStr( "530 Not logged in.\r\n" );
				Close();
				return false;
			}
			//Logged in: Process Request
			(this->*ent->func)(param);
			return true;
		}
	}
	ReplyUnknownCommand();
	return false;
}

void CFtpCmdCon::OnLIST(const char* param)
{
	OnLIST( param, true );
}

void CFtpCmdCon::OnPORT(const char* param)
{
	if( m_pDataCon )
		delete m_pDataCon;

	const char* ip = param;
	BYTE port[]={0,0};

	for( int vc = 0; *param; param++ )
	{
		if(*param==',' || !*param)
		{
			if(vc == 3)
			{
				*(char*)param=0;
				//port
				param++;
				port[1]=(BYTE)atoi(param);
				while( *param && *param != ',' ) param++;
				port[0] = *param ? (BYTE)atoi(param+1) : 0;
				break;
			}
			*(char*)param='.';
			vc++;
		}
	}
	
	m_pDataCon = new CFtpDataCon(this, false);
	m_pDataCon->Create();
	m_pDataCon->Select(WM_FTP_DATA_SOCK, FD_CONNECT|FD_CLOSE);
	m_pDataCon->AddToMap();

	m_pDataCon->SetAddress(ip);
	m_pDataCon->SetPort( *(USHORT*)port );
	ReplyOK();
}

void CFtpCmdCon::OnPASV(const char* param)
{
	if( !AppConfig.m_EnablePasv )
	{
		ReplyUnknownCommand();
		return;
	}

	if( m_pDataCon )
		delete m_pDataCon;

	m_pDataCon = new CFtpDataCon(this, true);
	//m_pDataCon = m_pDataCon; 
	m_pDataCon->Create();
	m_pDataCon->Select( WM_FTP_DATA_SOCK, FD_ACCEPT|FD_CLOSE);
	m_pDataCon->AddToMap();

	m_pDataCon->SetPort( CFtpDataCon::FindFreeDataPort() );
	m_pDataCon->Bind(NULL, m_pDataCon->GetPort() );

	m_pDataCon->Listen(1);

	char ip[32];	char* pip=ip;

	sockaddr_in addr;	memset(&addr,0,sizeof(addr));
	int addrlen = sizeof(addr);
	getsockname( m_Sock, (sockaddr*)&addr, &addrlen);
	strcpy(ip, inet_ntoa(addr.sin_addr));
	while(*pip)
	{
		if(*pip == '.')
			*pip = ',';
		pip++;
	}

	BYTE port[2];
	*((USHORT*)port) = m_pDataCon->GetPort();
	char ret[100];
	sprintf(ret,"227 Entering Passive Mode (%s,%d,%d)\r\n",ip,port[1], port[0]);
	SendStr(ret);

}

void CFtpCmdCon::OnPWD (const char* param)
{
	char ret[384];
	sprintf(ret, "257 \"%s\" is current dir.\r\n", LPCTSTR(m_WorkingDir));
	SendStr(ret);
}

void CFtpCmdCon::OnCWD (const char* param)	// Remember to translate ./ and ../
{
//	if( 0 == strnicmp( param, "-L ", 3 ) )
//		param += 3;
	CString newDir = GetFullPath( param );
	CFtpFilePerm* perm = m_pUser->QueryPerm(newDir);
	if( m_pUser->IsPathExist( perm, newDir, false ) )
	{
		m_WorkingDir = newDir;
		ReplyOK();
	}
	else
		ReplyFileUnavailable();
}

void CFtpCmdCon::OnCDUP (const char* param)
{
	if( m_WorkingDir.GetLength() > 1 )
	{
		const char* psep = (const char*)_mbsrchr( LPBYTE(LPCTSTR(m_WorkingDir)), '/' );
		size_t l = (psep - LPCTSTR(m_WorkingDir));
		if( l > 0 )
			m_WorkingDir = m_WorkingDir.Left(l);
		else
			m_WorkingDir = '/';
	}
	ReplyOK();
}

void CFtpCmdCon::OnRETR(const char* param)
{
	if( !m_pDataCon )
	{
		ReplyNoDataCon();
		m_Rest = 0;
		return;
	}

	CString virt_path = GetFullPath( param );
	CFtpFilePerm* perm = m_pUser->QueryPerm( virt_path );
	CString real_path = m_pUser->GetRealPath( perm, virt_path );

	if( !perm->CanRead() ||  !m_pDataCon->InitFileTransfer( SEND_FILE, real_path, m_Rest ) )
	{
		ReplyFileUnavailable();
		m_Rest = 0;
		return;
	}

	m_Rest = 0;	// Disable RESTART

	if( AppConfig.m_LogTransferToFile )
		WriteToTransferLog( true, m_pDataCon->GetFilePath() );

//	ReplyDataConOpen();	// Moved into CFtpDataCon::InitFileTransfer
}

void CFtpCmdCon::OnSTOR(const char* param)
{
	StoreFile( param, false );
}

void CFtpCmdCon::OnREST(const char* param)
{
	m_Rest = _atoi64(param);
	SendStr("350 REST supported.\r\n");
}

void CFtpCmdCon::OnDELE(const char* param)
{
	CString delete_path = GetFullPath(param);

	CFtpFilePerm* Perm = m_pUser->QueryPerm( delete_path );
	if( Perm->CanDelete() )
	{
		if( DeleteFile( m_pUser->GetRealPath( delete_path ) ) )
		{
			if( Perm->m_VirtPath == delete_path )
				m_pUser->DelFilePerm(Perm);
			ReplyFileActionOK();
			return;
		}
	}
	ReplyFileUnavailable();
}

void CFtpCmdCon::OnRMD (const char* param)
{
	CString rm_dir_path = GetFullPath(param);

	CFtpFilePerm* Perm = m_pUser->QueryPerm( rm_dir_path );
	if( Perm->CanRmDir() )
	{
		if( RemoveDirectory( m_pUser->GetRealPath( rm_dir_path )) )
		{
			if( Perm->m_VirtPath == rm_dir_path )
				m_pUser->DelFilePerm( Perm );
			ReplyFileActionOK();
			return;
		}
	}
	ReplyFileUnavailable();
}

void CFtpCmdCon::OnMKD (const char* param)
{
	CString new_dir_path = GetFullPath(param);
	CString parent_dir_path = CFtpFilePerm::GetParentPath(new_dir_path);

	CFtpFilePerm* Perm = m_pUser->QueryPerm( parent_dir_path );
	if( Perm->CanMkDir() )
	{
		if( CreateDirectory( m_pUser->GetRealPath( new_dir_path ), NULL) )
		{
			char ret[512];
			sprintf(ret,"257 \"%s\" directory created.\r\n", new_dir_path);
			SendStr(ret);
			return;
		}
	}
	ReplyFileUnavailable();
}

void CFtpCmdCon::OnRNFR(const char* param)
{
	m_RenameFromPath = GetFullPath(param);
	CFtpFilePerm* perm = m_pUser->QueryPerm( m_RenameFromPath );
	if( perm->CanWrite() )
		ReplyOK();
	else
	{
		m_RenameFromPath.Empty();
		ReplyFileUnavailable();
	}
}

void CFtpCmdCon::OnRNTO(const char* param)
{
	if( !m_RenameFromPath.IsEmpty() )
	{
		CString real_from_path = m_pUser->GetRealPath(m_RenameFromPath);
		CFtpFilePerm* oldperm = m_pUser->QueryPerm( m_RenameFromPath );

		CString to_path = GetFullPath( param );
		CFtpFilePerm* newperm = m_pUser->QueryPerm( to_path );

		if( oldperm->CanWrite() && newperm->CanWrite() )
		{
			if( oldperm->m_VirtPath == m_RenameFromPath )
			{
				m_pUser->ModifyPerm( oldperm, to_path );
				ReplyFileActionOK();
				m_RenameFromPath.Empty();
				return;
			}
			else
			{
				CString real_to_path = m_pUser->GetRealPath( to_path );
				if( MoveFile( real_from_path, real_to_path) )
				{
					ReplyFileActionOK();
					m_RenameFromPath.Empty();
					return;
				}
			}
		}
	}
	m_RenameFromPath.Empty();
	ReplyFileUnavailable();

}

void CFtpCmdCon::OnABOR(const char* param)
{
	if(m_pDataCon)
	{
		m_pDataCon->Close();
		m_pDataCon->Destroy();
		m_pDataCon = NULL;
	}
	SendStr("426 Connection closed; transfer aborted.\r\n");
	ReplyDataConClose();
}

void CFtpCmdCon::OnTYPE(const char* param)
{
	m_Type = toupper(*param);
	char ret[]="200 Type set to A\r\n";
	ret[strlen(ret)-3]=m_Type;
	SendStr(ret);
}

void CFtpCmdCon::OnNLST(const char* param)
{
	OnLIST( param, false );
}

void CFtpCmdCon::OnNOOP(const char* param)
{
	ReplyOK();
}

void CFtpCmdCon::OnMODE(const char* param)
{
	m_Mode = toupper(*param);
	ReplyOK();
}

void CFtpCmdCon::OnSTRU(const char* param)
{
	ReplyNotImplemented();
}

void CFtpCmdCon::OnSTAT(const char* param)
{
	ReplyNotImplemented();
}

void CFtpCmdCon::OnSYST(const char* param)
{
	SendStr("215 UNIX system type.\r\n");
}

void CFtpCmdCon::OnUSER(const char* param)
{
	m_LoggedIn = false;
	m_pUser = CFtpUser::FindUser( stricmp(param, CFtpUser::m_Anonymous) ?
		param : CFtpUser::m_Anonymous);
	if( m_pUser )
	{
		m_UserName = m_pUser->m_UserName;
//		m_pUser->AddRef();
	}
	SendStr("331 User name okay, need password.\r\n");
}

void CFtpCmdCon::OnPASS(const char* param)
{
	if(m_pUser && (m_pUser->m_UserName == CFtpUser::m_Anonymous || m_pUser->VerifyPasswd(param)) )
	{
		if( !CanLogin() )
		{
			SendStr("530 Not logged in.\n\n");
			delete this;
			return;
		}
		SendStr("230 User logged in\r\n");
		m_LoggedIn = true;
		m_UserCount++ ;
		m_pUser->m_UserCount++;
		AppConfig.SetUserIcon();
		GetGUI()->AddUser( this, m_UserName, m_Address );
	}
	else
	{
		SendStr("530 Not logged in.\n\n");
		delete this;
	}
}

void CFtpCmdCon::OnQUIT(const char* param)
{
	delete this;
}

void CFtpCmdCon::WriteToLog(const char *str)
{
	if( !AppConfig.m_LogToFile && !AppConfig.m_LogToScreen )
		return;

	SYSTEMTIME st;
	GetLocalTime(&st);
	char LogStr[ 2048 ];
	size_t len = sprintf( LogStr,"%d/%d/%d [%02d:%02d] (%05d) %s> %s\r\n",
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,(int)m_Sock, 
		m_pUser ? (LPCTSTR)m_pUser->m_UserName: (LPCTSTR)m_Address, str );

	DWORD wc;
	if(AppConfig.m_LogFile != INVALID_HANDLE_VALUE)
		WriteFile(AppConfig.m_LogFile,LogStr,len,&wc,NULL);

	if( AppConfig.m_LogToScreen && GetGUI() )
	{
		if( isdigit(*str) )
			GetGUI()->WriteLog( RGB(0,128,0), LogStr );
		else
			GetGUI()->WriteLog( RGB(0,0,128), LogStr );
	}
//		pGUI->LogToScreen(m_pLogStr, len);
}

void CFtpCmdCon::OnAPPE(const char *param)
{
	StoreFile( param, true );
}

void CFtpCmdCon::StoreFile(const char *path, bool Append)
{
	if( !m_pDataCon )
	{
		ReplyNoDataCon();
		return;
	}

	CString virt_path = GetFullPath( path );
	CFtpFilePerm* perm = m_pUser->QueryPerm( virt_path );
	CString real_path = m_pUser->GetRealPath( perm, virt_path );

	if( (Append && !perm->CanAppend()) || !perm->CanWrite()
		|| !m_pDataCon->InitFileTransfer( RECV_FILE, real_path, Append ) )
	{
		ReplyFileUnavailable();
		return;
	}

	if( AppConfig.m_LogTransferToFile )
		WriteToTransferLog( false, m_pDataCon->GetFilePath() );

//	ReplyDataConOpen();	// Moved into CFtpDataCon::InitFileTransfer
}

void CFtpCmdCon::OnTimer()
{
	if( ! GetDataCon() )
	{
		m_IdleTime++;
		ULONG timeout = m_pUser ? m_pUser->GetTimeOut() : AppConfig.m_WaitForLogin;
		if( timeout && m_IdleTime > timeout )
		{
			SendStr("421 Connection timed out - closing.\r\n");
			delete this;
		}
	}
}

void CFtpCmdCon::Init()
{
}

static CString GetFileListLine( const char* name, WIN32_FIND_DATA& find, SYSTEMTIME& curtime)
{
	CString line;
	const char* month[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char type = (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?'d':'-';
	char date_time[16];
	SYSTEMTIME filetime;
	FILETIME local_file_time;
	FileTimeToLocalFileTime(&find.ftLastWriteTime, &local_file_time);
	FileTimeToSystemTime(&local_file_time,&filetime);
	if(filetime.wYear == curtime.wYear ||
		((curtime.wYear-filetime.wYear)==1 && (curtime.wMonth-filetime.wMonth)<0) )
		sprintf(date_time,"%s %02d %02d:%02d",month[filetime.wMonth-1],
			filetime.wDay,filetime.wHour,filetime.wMinute);
	else
		sprintf(date_time,"%s %02d  %4d",month[filetime.wMonth-1],
			filetime.wDay,filetime.wYear);

	__int64 file_size = __int64(find.nFileSizeHigh) << 32;
	file_size |= find.nFileSizeLow;

	line.Format("%crwxr--r--   0 ftp      ftp %I64u %s %s\r\n",
		type, file_size, date_time, name );
	return line;
}

bool CFtpCmdCon::GetFileList(CString virt_path, CString& list, bool full)
{
	CFtpFilePerm* perm = m_pUser->QueryPerm( virt_path );
	if( !perm->CanList() )
		return false;

	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	WIN32_FIND_DATA find;
	HANDLE finder = INVALID_HANDLE_VALUE;

	CFtpUser* pUser[]  = {m_pUser, m_pUser->m_pParent};
	for( int i=0; i < 2; ++i)
	{
		if( !pUser[i] )
			continue;

		CString virt_path_it;
		POSITION pos = pUser[i]->GetFilePerm().GetStartPosition();
		while( pos )
		{
			CFtpFilePerm* perm_it;
			pUser[i]->GetFilePerm().GetNextAssoc( pos, virt_path_it, (void*&)perm_it );

			if( virt_path.GetLength() != virt_path_it.GetLength() 
				&& 0 == strncmp( virt_path, virt_path_it, virt_path.GetLength() )
				&& ( virt_path.GetLength() ==1 || virt_path_it[virt_path.GetLength()] == '/' ) )
			{
				// Check if user settings overrides group settings.
				CFtpFilePerm* tmp_perm;
				if( i == 1 && pUser[0]->GetFilePerm().Lookup(virt_path_it, (void*&)tmp_perm) )
					continue;

				// List every dir;
				if( perm_it->CanList() )
				{
					if( !perm_it->m_RealPath.IsEmpty() )	//如果檔案存在
						finder = FindFirstFile( perm_it->m_RealPath, &find );
					else
						finder = INVALID_HANDLE_VALUE;

					if( finder == INVALID_HANDLE_VALUE )	// 如果是虛擬目錄
					{
						SystemTimeToFileTime(&curtime, &find.ftLastWriteTime );
						find.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
						find.nFileSizeLow = 0;
						find.nFileSizeHigh = 0;
					}

					int i_name = virt_path.GetLength();
					if( i_name > 1 )	i_name++;
					const char* psep = (const char*)_mbschr(LPBYTE(LPCTSTR(virt_path_it) + i_name), '/');
					if( psep )
						continue;
					CString name = virt_path_it.Mid( i_name, virt_path_it.GetLength()-i_name);

					if( full )
						list += GetFileListLine( name, find, curtime );
					else
					{
						list += name;
						list += "\r\n";
					}
					FindClose(finder);
				}
			}
		}
	}

	CString real_path = m_pUser->GetRealPath( perm, virt_path );
	if( real_path.IsEmpty() )
		return true;

	// List all the file in the real dir.
	CString tmp = real_path + (mbslastchr(real_path) == '\\' ? "*" : "\\*");
	memset( &find, 0, sizeof(find) );
	finder = FindFirstFile( real_path + (mbslastchr(real_path) == '\\' ? "*" : "\\*"), &find );
	if( virt_path != '/' )
		virt_path += '/';
	if( finder == INVALID_HANDLE_VALUE )
	{
		FindClose(finder);
		return false;
	}
	do
	{
		if( (find.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) || '.' == *find.cFileName )
			continue;
		CString vfpath = virt_path + find.cFileName;
		perm = m_pUser->QueryPerm( vfpath );
		if( !perm->CanList() || perm->m_VirtPath == vfpath )
			continue;

		if( full )
			list += GetFileListLine( find.cFileName, find, curtime );
		else
		{
			list += find.cFileName;
			list += "\r\n";
		}
	}while( FindNextFile(finder, &find) );
	FindClose(finder);

	return true;
}

void CFtpCmdCon::EndDataTransfer()
{
	if( m_pDataCon )
	{
//		m_pDataCon->Close();
		delete m_pDataCon;
		m_pDataCon = NULL;
	}
	ReplyDataConClose();
}

void CFtpCmdCon::Close()
{
	if( !IsValid() )
		return;

	RemoveFromMap();

	if( m_LoggedIn )
	{
		m_UserCount-- ;
		m_pUser->m_UserCount--;

		if( 0 == m_UserCount && m_FtpDataConMap.IsEmpty() )
			AppConfig.SetNormalIcon();

		if( GetGUI() )
			GetGUI()->RemoveUser( this );
	}

	CFtpSock::Close();
}

void CFtpCmdCon::OnLIST(const char *param, bool full)
{
	if( !m_pDataCon || !m_pDataCon->IsValid() )
	{
		ReplyNoDataCon();
		return;
	}

	while( ' '== *param )
		param++;

	if( param[0] == '-' )
	{
		const char *tmp = param + 1;
		while( *tmp != ' ' && *tmp )
		{
			if( !isalpha(*tmp) )
			{
				tmp = NULL;
				break;
			}
			tmp ++;
		}
		if( tmp )
		{
			param = tmp;
			while( ' '== *param )
				param++;
		}
	}

	CString DirPath = GetFullPath( param );	//m_WorkingDir;

	CString file_list;
	if( !GetFileList( DirPath, file_list, full ) )
	{
		ReplyFileUnavailable();
		m_pDataCon->Close();
		m_pDataCon->Destroy();
		m_pDataCon = NULL;
		return;
	}

	m_pDataCon->InitDirListTransfer(file_list);
}

CString CFtpCmdCon::GetFullPath(const char *FileName)
{
	// For security reasons I have to deal with all ./ and .// in the path.
	CString full_path;
	char* pfull_path;
	size_t len = strlen( FileName );
	if( FileName[0] != '/' )
	{
		pfull_path = full_path.GetBuffer( m_WorkingDir.GetLength() + len + 2 );
		strcpy( pfull_path, m_WorkingDir );
		if( m_WorkingDir.GetLength() > 1 )
		{
			pfull_path [ m_WorkingDir.GetLength() ] = '/';
			strcpy( pfull_path + m_WorkingDir.GetLength() + 1, FileName );
		}
		else
			strcpy( pfull_path + m_WorkingDir.GetLength(), FileName );
	}
	else
	{
		pfull_path = full_path.GetBuffer( len + 1 );
		strcpy( pfull_path, FileName );
	}

	if( len > 0 )
	{
		char* ppath = pfull_path;
		while( *ppath )
		{
			if( ppath[0] == '/' )
			{
				if( '.' == ppath[1] )
				{
					if( '.' == ppath[2] )
					{
						if( '/' == ppath[3] || '\0' == ppath[3] )
						{
							ppath[0] = '\0';
	//						char* prev = (char*)_mbsrchr( (BYTE*)pfull_path, '/' );
							char* prev = strrchr( pfull_path, '/' );
							ppath[0] = '/';
							if( !prev )
								prev = pfull_path;
							strcpy( prev, ppath + 3 );
							ppath = prev;
							continue;
						}
					}
					else if( '/' == ppath[2] || '\0' == ppath[2] )
					{
						strcpy( ppath, ppath + 2 );
						continue;
					}
				}
			}
			ppath++;
//			ppath = (char*)_mbsinc( (BYTE*)ppath );
		}
	}
	if( !*pfull_path )
		strcpy( pfull_path, "/" );

	full_path.ReleaseBuffer();

	if( full_path.GetLength() > 1 && full_path[full_path.GetLength()-1] == '/' )
		full_path = full_path.Left(full_path.GetLength()-1);

	return full_path;
}

void CFtpCmdCon::KickUser()
{
	SendStr("530 Kicked by the server.\r\n");
	delete this;
}

void CFtpCmdCon::BanUser(bool byName)
{
	SendStr("530 You are banned from the server.\r\n");
//	in_addr addr;
//	addr.S_un.S_addr = inet_addr( m_Address );
	unsigned long addr = inet_addr( m_Address );
	AppConfig.m_BannedIP.Add(addr);
	AppConfig.SaveIPFilter();
	delete this;
}

bool CFtpCmdCon::CanLogin()
{
	if( ! m_pUser->m_Enabled )
		return false;

	if( AppConfig.m_MaxCon && m_UserCount >= AppConfig.m_MaxCon 
		|| m_pUser->GetUserCount() >= m_pUser->GetMaxCon() )
	{
		SendStr("530 There are too many users.\r\n");
		return false;
	}

	POSITION pos = m_FtpCmdConMap.GetStartPosition();
	UINT num = 0;
	while( pos )
	{
		SOCKET key;
		CFtpCmdCon *con;
		m_FtpCmdConMap.GetNextAssoc( pos, key, (CFtpSock*&)con );
		if( con->GetAddress() == m_Address )
			num++;
	}
	if( num <= m_pUser->GetMaxConPerIP() )
		return true;

//	SendStr("530 Max concurrent connection reached.\r\n");
	SendStr("530 No more connections are allowed for this IP.\r\n");
	return false;
}

void CFtpCmdCon::WriteToTransferLog(bool is_download, const char* file)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	char LogStr[ 2048 ];
	size_t len = sprintf( LogStr,"%s %d/%d/%d [%02d:%02d] %s %s\r\n",
		(is_download ? "Download":"Upload  "), st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute, 
		(LPCTSTR)m_pUser->m_UserName, file );

	DWORD wc;
	if(AppConfig.m_TransferLogFile != INVALID_HANDLE_VALUE)
		WriteFile(AppConfig.m_TransferLogFile,LogStr,len,&wc,NULL);

//	if( AppConfig.m_LogToScreen && GetGUI() )
//		GetGUI()->WriteLog( RGB(0,0,0), LogStr );
}


void CFtpCmdCon::OnSIZE(const char* param)
{
	CString virt_path = GetFullPath( param );
	CFtpFilePerm* perm = m_pUser->QueryPerm( virt_path );

	__int64 size = 0;

	bool found = false;
	if( perm->CanList() )
	{
		CString real_path;
		if( perm->m_VirtPath == virt_path )
			real_path = perm->m_RealPath;
		else
			real_path = m_pUser->GetRealPath( virt_path );

		if( !real_path.IsEmpty() )
		{
			WIN32_FIND_DATA find;
			HANDLE finder = FindFirstFile( real_path, &find );
			if( INVALID_HANDLE_VALUE == finder )
			{
				ReplyFileUnavailable();
				return;
			}
			else
			{
				size = __int64(find.nFileSizeHigh) << 32;
				size |= find.nFileSizeLow;
				FindClose( finder );
			}
		}

		char cmd[32];
		sprintf(cmd, "213 %I64u\r\n", size);	// I64: 64 bit integer
		SendStr( cmd );
	}
	else
	{
		ReplyFileUnavailable();
		return;
	}
}

