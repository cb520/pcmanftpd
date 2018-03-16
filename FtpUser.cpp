// FtpUser.cpp: implementation of the CFtpUser class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "FtpUser.h"
#include <mbstring.h>
#include <afxtempl.h>
#include "mbstring.h"
#include "AppConfig.h"

#include "Blowfish/Blowfish.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CList<CFtpUser,CFtpUser&> CFtpUser::m_AllUsers;
const char CFtpUser::m_Anonymous[] = "Anonymous";

CMapStringToPtr CFtpUser::m_AllUsers;
CMapStringToPtr CFtpUser::m_AllGroups;

CFtpUser::CFtpUser()
{
	m_pParent=NULL;

	m_MaxUSpeed=0;
	m_MaxDSpeed=0;
	m_MaxCon=0;
	m_MaxConPerIp=0;
	m_TimeOut=0;
	m_Banned=FALSE;
	m_Enabled=TRUE;

	m_PermChanged = FALSE;
	m_RefCount = 1;
	m_UserCount = 0;
	m_PasswdShadowL = 0;
	m_PasswdShadowR = 0;				 
}

CFtpUser::~CFtpUser()
{
	m_FilePerm.RemoveAll();
//	SaveFilePerm();
}

//DEL UINT CFtpUser::Release()
//DEL {
//DEL 	if( 0 == --m_RefCount )
//DEL 	{
//DEL 		m_AllUsers.RemoveKey( m_UserName );
//DEL 		delete this;
//DEL 	}
//DEL 	return m_RefCount;
//DEL }

//DEL CFtpFilePerm* CFtpUser::_QueryAccessVirtual(const char* VirtPath)
//DEL {
//DEL 	for(POSITION it=m_FilePerm.GetHeadPosition(); it; m_FilePerm.GetNext(it))
//DEL 	{
//DEL 		if(strcmpi(m_FilePerm.GetAt(it).m_VirtPath,VirtPath)==0)
//DEL 			return &m_FilePerm.GetAt(it);
//DEL 	}
//DEL 	return m_pParent ? m_pParent->_QueryAccessVirtual(VirtPath):NULL;
//DEL }

//DEL CFtpFilePerm* CFtpUser::QueryAccessVirtual(const CString VirtPath)
//DEL {
//DEL 	if(VirtPath.IsEmpty())
//DEL 		return NULL;
//DEL 
//DEL 	CFtpFilePerm* Perm;
//DEL 	CString query_path=VirtPath;
//DEL 	int oldl=0;
//DEL 	do{
//DEL 		Perm=_QueryAccessVirtual(query_path);
//DEL 		if(Perm)
//DEL 		{
//DEL 			if(query_path!=VirtPath && !Perm->m_CanSubDir)
//DEL 				Perm=NULL;
//DEL 			break;
//DEL 		}
//DEL 		oldl=query_path.GetLength();
//DEL 		query_path=CFtpFilePerm::GetParentPath(query_path);
//DEL 	}while(query_path.GetLength() != oldl);
//DEL 	return Perm;
//DEL }

CString CFtpUser::GetRealPath(const char *pVirtPath)
{
	CFtpFilePerm* Perm = QueryPerm( pVirtPath );
	return GetRealPath(Perm, pVirtPath);
}


void CFtpUser::LoadFilePerm()
{
	m_FilePerm.RemoveAll();
	// Load File Perm right
	CString path= AppPath + (m_IsUser ? "users\\":"groups\\");
	path += m_UserName;
	path += ".ini";
	CFile file;
	if( file.Open(path,CFile::modeRead) )
	{
		CString line;
		CArchive ar(&file,CArchive::load);
		while( ar.ReadString(line) )
		{
			char* tmpbuf = line.GetBuffer(line.GetLength()+1);
			int len = strlen(tmpbuf);
			if(len && tmpbuf[len-1]==0x0a)
				tmpbuf[len-1]=0;

			char* pVirtPath = tmpbuf + strspn(tmpbuf," \t");
			if( !*pVirtPath )	continue;

			char sep = '=';
			if( *pVirtPath=='\"' )
			{
				sep='\"';
				pVirtPath++;
			}

			char* eos;
			char* pRealPath;
			pRealPath=eos=strchr( pVirtPath, sep );
			if(!eos)	continue;
			if( sep=='\"' )
				pRealPath++;
			pRealPath += strspn( pRealPath, "= \t" );
			*eos=0;

			char* pPerm = eos = strchr(pRealPath, *pRealPath=='\"' ? '\"' : ',' );
			if(eos)
			{
				pPerm += strspn(pPerm,"\", \t");
				*eos=0;
			}

			CFtpFilePerm* pFilePerm = new CFtpFilePerm(pVirtPath,pRealPath);
			pFilePerm->m_IsUser = m_IsUser;

			if(eos)
			{
				if( *pPerm )
				{
					pFilePerm->m_CanRead	= !!strchr(pPerm,'r');
					pFilePerm->m_CanWrite	= !!strchr(pPerm,'w');
					pFilePerm->m_CanDelete	= !!strchr(pPerm,'d');
					pFilePerm->m_CanAppend	= !!strchr(pPerm,'a');
					pFilePerm->m_CanList	= !!strchr(pPerm,'L');
					pFilePerm->m_CanMkDir	= !!strchr(pPerm,'M');
					pFilePerm->m_CanRmDir	= !!strchr(pPerm,'D');
					pFilePerm->m_CanSubDir	= !!strchr(pPerm,'S');
				}
			}
	//		m_FilePerm.AddTail(FilePerm);
			m_FilePerm[pFilePerm->m_VirtPath] = pFilePerm;
			line.ReleaseBuffer();
		}
		ar.Close();
		file.Close();
	}

	// No permission setting will cause access violation when QueryPerm get called.
	// There must be at least one permission object existing in the map.
	if( !m_pParent && 0 == m_FilePerm.GetCount() )
		m_FilePerm["/"] = new CFtpFilePerm("/", "");
}

//DEL void CFtpUser::SaveSetting()
//DEL {
//DEL 	CString path=AppPath+ (m_IsUser?"Users.ini":"Groups.ini");
//DEL 	WritePrivateProfileString(m_UserName ,"pass",m_Password,path);
//DEL 	char tmp[16];
//DEL 	itoa(m_Banned,tmp,10);
//DEL 	WritePrivateProfileString(m_UserName ,"banned",tmp,path);
//DEL 
//DEL 	itoa(m_Enabled,tmp,10);
//DEL 	WritePrivateProfileString(m_UserName ,"enabled",tmp,path);
//DEL 
//DEL 	itoa(m_MaxUSpeed/1024,tmp,10);
//DEL 	WritePrivateProfileString(m_UserName ,"maxuspeed",tmp,path);
//DEL 
//DEL 	itoa(m_MaxDSpeed/1024,tmp,10);
//DEL 	WritePrivateProfileString(m_UserName ,"maxdspeed",tmp,path);
//DEL 
//DEL 	itoa(m_MaxCon,tmp,10);
//DEL 	WritePrivateProfileString(m_UserName ,"maxcon",tmp,path);
//DEL 
//DEL 	itoa(m_TimeOut,tmp,10);
//DEL 	WritePrivateProfileString(m_UserName ,"timeout",tmp,path);
//DEL 
//DEL 	if( m_IsUser )
//DEL 		WritePrivateProfileString(m_UserName ,"group",
//DEL 		(m_pParent ? m_pParent->m_UserName : NULL), path);
//DEL }

BOOL CFtpUser::IsPathExist(CFtpFilePerm *Perm, CString VirtPath, bool show_hidden)
{
	// Check if VirtPath is a virtual folder.
	// Every virtual folder must be defined in the access list, so the Perm
	// object must belong to VirtPath, otherwise, this virtual path doesn't exist.
	if( Perm->m_VirtPath == VirtPath )	// the Perm object belongs to VirtPath
		return TRUE;
	else if( Perm->m_RealPath.IsEmpty() )
		return FALSE;	// It's impossible to mount a file under a virtual folder without it's own Perm object.
	// whether it's a virtual folder without real path or not, a defined path must exist.

	// If this Perm object doesn't belong to VirtPath, it mustn't be a virtual path.
	// Check if the real path exists on the disk.
	CString RealPath = GetRealPath(Perm, VirtPath);
	DWORD attrib=GetFileAttributes(RealPath);
	return ( attrib != 0xFFFFFFFF && (show_hidden || !(attrib & FILE_ATTRIBUTE_HIDDEN)) );
}

CString CFtpUser::GetRealPath(const CFtpFilePerm *Perm, const char* pVirtPath)
{
    typedef const BYTE* LPCBYTE;
	CString RealPath;
	if(Perm)
	{
		RealPath=Perm->m_RealPath;
		int pos=RealPath.GetLength();
		const char* pRest = pVirtPath + Perm->m_VirtPath.GetLength();
		if( !RealPath.IsEmpty() )
		{
			if( mbslastchr(RealPath) == '\\' )
			{
				if( *pRest == '/' )
					pRest++;
			}
			else
			{
				if( *pRest && *pRest != '/' )
					RealPath += '\\';
			}
		}
		RealPath+=pRest;

		char* ppath=(char*)LPCTSTR(RealPath) + pos;
		for(; *ppath; ppath=(char*)_mbsinc( LPCBYTE(ppath) ) )
		{
			if(*ppath=='/')
				*ppath='\\';
		}
	}
	return RealPath;
}

void CFtpUser::LoadSettings()
{
	char* fn[]={"Groups.ini","Users.ini"};
	CMapStringToPtr* maps[] = {&m_AllGroups, &m_AllUsers};
	char* buf = new char[2048];
	char* val = buf + 1024;
	CString fpath;
	for( int i = 0; i < 2; i++ )
	{
		CMapStringToPtr& map = *maps[i];
		map.RemoveAll();
		fpath = AppPath + fn[i];
		GetPrivateProfileSectionNames(buf, 1024, fpath );
		for( char* pbuf=buf; *pbuf; pbuf += (strlen(pbuf) + 1) )
		{
			CFtpUser* user = new CFtpUser();
			user->m_IsUser= (i==1) ;	//¬OgroupÁÙ¬Ouser?;
			map[pbuf] = user;
			user->LoadUser(pbuf, fpath, val);
		}
	}
	delete []buf;
}

void CFtpUser::LoadUser( const char *Name, const char *path, char* val )
{
	m_UserName = stricmp(m_UserName, m_Anonymous) ? Name: m_Anonymous;

	if(m_IsUser)
	{
		GetPrivateProfileString( Name, "group", "", val, 1024, path );
		if(*val)
		{
			m_GroupName = val;
			m_AllGroups.Lookup( val, (void*&)m_pParent);
		}
	}

	GetPrivateProfileString(Name,"password","",val,1024,path);
//	m_Password	=val;
	//m_PasswdShadowL = 0;
	//m_PasswdShadowR = 0;
	if( *val )
		sscanf( val, "%20lu,%20lu", &m_PasswdShadowL, &m_PasswdShadowR);
	else if( m_pParent )
	{
		m_PasswdShadowL = m_pParent->m_PasswdShadowL;
		m_PasswdShadowR = m_pParent->m_PasswdShadowR;
	}

	m_Banned		= GetPrivateProfileInt(Name,"banned",
		m_pParent ? m_pParent->m_Banned:0,path);
	m_Enabled		= GetPrivateProfileInt(Name,"enabled",
		m_pParent ? m_pParent->m_Enabled:0,path);
	m_MaxUSpeed	= GetPrivateProfileInt(Name,"maxuspeed",
		m_pParent ? m_pParent->m_MaxUSpeed/1024:0,path);
	m_MaxDSpeed=(UINT)GetPrivateProfileInt(Name,"MaxDSpeed",0,path);
	m_MaxUSpeed	= m_MaxUSpeed ? m_MaxUSpeed*1024 : DWORD(-1);

	m_MaxDSpeed	= GetPrivateProfileInt(Name,"maxdspeed",
		m_pParent ? m_pParent->m_MaxDSpeed/1024:0,path);
	m_MaxDSpeed	= m_MaxDSpeed ? m_MaxDSpeed*1024 : DWORD(-1);

	m_MaxCon		=GetPrivateProfileInt(Name,"maxcon",
		m_pParent ? m_pParent->m_MaxCon:0,path);

	m_MaxConPerIp	=GetPrivateProfileInt(Name,"maxconperip",
		m_pParent ? m_pParent->m_MaxConPerIp:0,path);

	m_TimeOut		=GetPrivateProfileInt(Name,"timeout",
		m_pParent ? m_pParent->m_TimeOut:0,path);

	LoadFilePerm();
}


void CFtpUser::SaveFilePerm()
{
	CIniFile file;
	CString fPath = AppPath + (m_IsUser?"Users\\":"Groups\\");
	fPath += m_UserName;	fPath+=".ini";
	if( !file.Open( fPath, CFile::modeWrite|CFile::modeCreate ) )
		return;

	for(POSITION pos = m_FilePerm.GetStartPosition(); pos; )
	{
		CFtpFilePerm* Perm;
		CString VirtPath;
		m_FilePerm.GetNextAssoc( pos, VirtPath, (void*&)Perm );
		CString RealPath(Perm->m_RealPath);

		if( _mbschr( (LPBYTE)LPCTSTR(Perm->m_VirtPath),'=') )
		{
			VirtPath = '\"' + Perm->m_VirtPath;
			VirtPath += '\"';
		}
		if( _mbschr( (LPBYTE)LPCTSTR(Perm->m_RealPath),',') )
		{
			RealPath = '\"' + Perm->m_RealPath;
			RealPath += '\"';
		}
		// Add permission
		RealPath += ',';
		if(Perm->m_CanRead)
			RealPath += 'r';
		if(Perm->m_CanWrite)
			RealPath += 'w';
		if(Perm->m_CanDelete)
			RealPath += 'd';
		if(Perm->m_CanAppend)
			RealPath += 'a';
		if(Perm->m_CanList)
			RealPath += 'L';
		if(Perm->m_CanMkDir)
			RealPath += 'M';
		if(Perm->m_CanRmDir)
			RealPath += 'D';
		if(Perm->m_CanSubDir)
			RealPath += 'S';

		file.WriteStr(VirtPath, RealPath );
	}
	file.Close();
}


CFtpFilePerm* CFtpUser::QueryPerm(const char *pVirtPath)
{
	size_t len = strlen(pVirtPath)+1;
	if( 0 == len )
		len++;

	char *virt_path = new char[ len + 1 ];
	memcpy( virt_path, pVirtPath, len );
	if( !*virt_path )
		strcpy( virt_path, "/");

	if( len > 1 && virt_path[len-1] == '/' )	// get rid of tailing '/'
	{
		len--;
		virt_path[len] = '\0';
	}

	CFtpFilePerm* pPerm = NULL;
	while( !pPerm )
	{
		if( m_FilePerm.Lookup( virt_path, (void*&)pPerm ) 
			|| (m_pParent && m_pParent->m_FilePerm.Lookup( virt_path, (void*&)pPerm )) )
			break;

		BYTE* p = _mbsrchr( (const BYTE*)virt_path, '/' );
		if( (char*)p > virt_path )
			*p = '\0';
		else
			p[1] = '\0';
	}

	delete []virt_path;
	return pPerm;
}

CFtpUser* CFtpUser::FindUser(const char *name)
{
	CFtpUser* pUser;
	if( !m_AllUsers.Lookup( name, (void*&)pUser ) )	// User found!
		return NULL;
	return pUser;
}

CFtpUser* CFtpUser::FindGroup(const char *name)
{
	return NULL;
}

void CFtpUser::Unload()
{
	CMapStringToPtr* maps[] = {&m_AllGroups, &m_AllUsers};
	CString key;
	CFtpUser* value = NULL;
	for( int i = 0; i < 2; i++ )
	{
		CMapStringToPtr& map = *maps[i];
		for( POSITION pos = map.GetStartPosition(); pos; )
		{
			map.GetNextAssoc( pos, key, (void*&)value );
			delete value;
		}
		map.RemoveAll();
	}
}

void CFtpUser::DelFilePerm(CFtpFilePerm *perm)
{
	m_FilePerm.RemoveKey( perm->m_VirtPath );
	delete perm;
	SaveFilePerm();
}


void CFtpUser::ModifyPerm(CFtpFilePerm* perm, CString virt_path, CString real_path, DWORD perm_val)
{
	m_FilePerm.RemoveKey( perm->m_VirtPath );
	perm->m_VirtPath = virt_path;
	if( !real_path.IsEmpty() )
		perm->m_RealPath = real_path;
	if( perm_val != -1 )
		perm->m_Perm = perm_val;
	m_FilePerm [ perm->m_VirtPath ] = perm;
	SaveFilePerm();
}

UINT CFtpUser::GetMaxCon()
{
	UINT num = ( m_MaxCon > AppConfig.m_MaxCon ? m_MaxCon : AppConfig.m_MaxCon);
	return num > 0 ? num : (UINT)-1;
}

UINT CFtpUser::GetMaxConPerIP()
{
	UINT num = ( m_MaxConPerIp > 0 /*> AppConfig.m_MaxConPerIp*/ ? m_MaxConPerIp : AppConfig.m_MaxConPerIp);
	return num > 0 ? num : (UINT)-1;
}


UINT CFtpUser::GetTimeOut()
{
	return (m_TimeOut ? m_TimeOut : (m_pParent ? m_pParent->GetTimeOut() : 0));
}


CFtpUser::CFtpUser (CFtpUser &user)
{
	m_GroupName = user.m_GroupName;
	m_UserName = user.m_UserName;
//	m_Password = user.m_Password;
	m_PasswdShadowL = user.m_PasswdShadowL;
	m_PasswdShadowR = user.m_PasswdShadowR;

	m_IsUser = user.m_IsUser;
	if( m_IsUser && !m_AllGroups.Lookup( m_GroupName, (void*&)m_pParent ) )
		m_pParent = NULL;

	m_MaxUSpeed = user.m_MaxUSpeed;
	m_MaxDSpeed = user.m_MaxDSpeed;
	m_MaxCon = user.m_MaxCon;
	m_MaxConPerIp = user.m_MaxConPerIp;
	m_TimeOut = user.m_TimeOut;
	m_Banned = user.m_Banned;
	m_Enabled = user.m_Enabled;
	m_FilePerm = user.m_FilePerm;

	m_PermChanged = FALSE;
	m_RefCount = 0;
	m_UserCount = 0;
}


bool CFtpUser::VerifyPasswd(const char *passwd)
{
	if( 0 == m_PasswdShadowL && 0 == m_PasswdShadowR )
		return ('\0' == *passwd);

	BLOWFISH_CTX* bfc = new BLOWFISH_CTX;
	Blowfish_Init( bfc, (unsigned char*)passwd, strlen(passwd) );
	unsigned long l = m_PasswdShadowL, r = m_PasswdShadowR;
	Blowfish_Decrypt( bfc, &l, &r );
	return (l == SHADOWER_L && r == SHADOWER_R);
}
