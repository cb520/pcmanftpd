// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__195C6469_6862_4729_8E00_FF33CEF59FAA__INCLUDED_)
#define AFX_INIFILE_H__195C6469_6862_4729_8E00_FF33CEF59FAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFile : public CFile  
{
public:

	inline void WriteStr(CString Name, LPCTSTR val)
	{
		Name+='=';	Name+=val;
		WriteLine(Name);
	}

	inline void WriteInt(LPCTSTR Name, DWORD val)
	{
		char buf[12];
		WriteStr(CString(Name),ltoa(val,buf,10));
	}

	inline void WriteLine(CString Line)
	{
		Line+="\x0d\x0a";
		Write(LPCTSTR(Line),Line.GetLength());
	}

	CIniFile(){}
	~CIniFile(){}
};




#endif // !defined(AFX_INIFILE_H__195C6469_6862_4729_8E00_FF33CEF59FAA__INCLUDED_)
