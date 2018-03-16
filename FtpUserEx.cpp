// FtpUserEx.cpp: implementation of the CFtpUserEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCManFTPD2.h"
#include "FtpUserEx.h"
#include "Blowfish/Blowfish.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpUserEx::CFtpUserEx()
{
	m_bMaxCon = 0;
	m_bMaxConPerIp = 0;
	m_bMaxDSpeed = 0;
	m_bMaxUSpeed = 0;
	m_bPassword = 0;
	m_bTimeOut = 0;
	m_PasswdChanged = false;
}

CFtpUserEx::~CFtpUserEx()
{

}

void CFtpUserEx::LoadUser( const char *Name, const char *path, char* val )
{
	m_UserName = stricmp(m_UserName, m_Anonymous) ? Name: m_Anonymous;

	if(m_IsUser)
	{
		GetPrivateProfileString( Name, "group", "", val, 1024, path );
		if(*val)
		{
			m_GroupName = val;
//			m_AllGroups.Lookup( val, (void*&)m_pParent);
		}
	}

	GetPrivateProfileString(Name,"password","",val,1024,path);
	m_Password	=val;

	m_Enabled		= GetPrivateProfileInt(Name,"enabled",-1,path);
	if( m_Enabled == -1 )
	{
		if( m_pParent )
		m_Enabled=m_pParent->m_Enabled;
	}

	m_MaxUSpeed	= GetPrivateProfileInt(Name,"maxuspeed",-1,path);

	m_MaxDSpeed	= GetPrivateProfileInt(Name,"maxdspeed",-1,path);

	m_MaxCon		=GetPrivateProfileInt(Name,"maxcon",-1,path);

	m_MaxConPerIp	=GetPrivateProfileInt(Name,"maxconperip",-1,path);

	m_TimeOut		=GetPrivateProfileInt(Name,"timeout",-1,path);

	GetPrivateProfileString(Name,"note","",val,1024,path);
	m_Note = val;

	LoadFilePerm();

	m_bPassword = !m_Password.IsEmpty();

	if( m_MaxUSpeed==-1 )
		m_MaxUSpeed=0;
	else
	{
		m_bMaxUSpeed=1;
	}


	if( m_MaxDSpeed==-1 )
		m_MaxDSpeed=0;
	else
	{
		m_bMaxDSpeed=1;
	}

	if( m_MaxCon==-1 )
		m_MaxCon=0;
	else
		m_bMaxCon=1;

	if( m_MaxConPerIp==-1 )
		m_MaxConPerIp=0;
	else
		m_bMaxConPerIp=1;

	if( m_TimeOut==-1 )
		m_TimeOut=0;
	else
		m_bTimeOut=1;
}

void CFtpUserEx::SaveUser(CIniFile &file)
{
	if( 0 == stricmp(m_UserName, m_Anonymous) )
		m_UserName = m_Anonymous;

	file.WriteLine('['+m_UserName+"]");

	if( !m_Password.IsEmpty() )
	{
		if( m_PasswdChanged )
		{
			BLOWFISH_CTX* bfc = new BLOWFISH_CTX;
			Blowfish_Init( bfc, (unsigned char*)LPCTSTR(m_Password), m_Password.GetLength() );
			unsigned long l = SHADOWER_L , r = SHADOWER_R;
			Blowfish_Encrypt( bfc, &l, &r );
			m_Password.Format("%X,%X", l, r );
		}
		file.WriteStr("Password",m_Password);
	}

	if( m_IsUser && !m_GroupName.IsEmpty() )
		file.WriteStr("Group",m_GroupName);

	if( !m_Note.IsEmpty() )
		file.WriteStr("Note",m_Note);

	file.WriteInt("Enabled", m_Enabled);
	if(m_bMaxUSpeed)
		file.WriteInt("MaxUSpeed", m_MaxUSpeed);
	if(m_bMaxDSpeed)
		file.WriteInt("MaxDSpeed", m_MaxDSpeed);
	if(m_bMaxCon)
		file.WriteInt("MaxCon", m_MaxCon);
	if(m_bMaxConPerIp)
		file.WriteInt("MaxConPerIp", m_MaxConPerIp);
	if(m_bTimeOut)
		file.WriteInt("TimeOut", m_TimeOut);

	file.WriteLine("");

//	if( m_PermChanged )
	SaveFilePerm();
}

