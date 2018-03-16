// NotifyIcon.h: interface for the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYICON_H__EBAC35DD_6F98_4623_AC69_D23F7E2F623C__INCLUDED_)
#define AFX_TRAYICON_H__EBAC35DD_6F98_4623_AC69_D23F7E2F623C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const UINT WM_NOTIFY_ICON = WM_APP+102;

class CTrayIcon : public NOTIFYICONDATA
{
public:

	CTrayIcon(HWND _hWnd, UINT _uID)
	{
		cbSize = sizeof(NOTIFYICONDATA);
		hWnd = _hWnd;
		uID = _uID;
	}

	inline void Add(HICON _hIcon, const char* tip)
	{
		hIcon = _hIcon;
		strcpy(szTip,tip?tip:"");
		uCallbackMessage = WM_NOTIFY_ICON;
		uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, this);
	}

	inline void Remove()
	{
		uFlags = 0;
		Shell_NotifyIcon(NIM_DELETE, this);
	}

	void Modify(HICON _hIcon, const char *tip)
	{
		uFlags = (_hIcon?NIF_ICON:0);
		if(tip)
		{
			strcpy(szTip,tip);
			uFlags|=NIF_TIP;
		}
		hIcon = _hIcon;
		Shell_NotifyIcon(NIM_MODIFY, this);
	}

};

#endif // !defined(AFX_TRAYICON_H__EBAC35DD_6F98_4623_AC69_D23F7E2F623C__INCLUDED_)
