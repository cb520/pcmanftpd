// DlgLayout.cpp: implementation of the CDlgLayout class.
//

#include "stdafx.h"
#include "DlgLayout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
/*
	Class Name:		CDlgLayout
	
	Purpose:		Provide an really easy way to manage dialog controls

	Limitation:		Everyone can freely use this class for any purpose but sale.

	Author:			PCMan (HZY) ,a college student from Taiwan, R.O.C
					I'm a student at National Yang Ming University and I
					major in medicine.
	
	E-mail:			hzysoft@sinamail.com

	Homepage:		http://pcman.ptt.cc/

	Description:	This class is trying to provide programmers with a
					simple way to manage dialog controls.
					The original idea is from VCL of Delphi.
					In Delphi, people use anchors to position controls and
					I found there is no similiar component in MFC, so I
					write this class myself.
					Though it has limited functionality, it's very easy
					to use and maintain.

	Features:		Easy to use and maintain.
					Minimal memmory usage and, most important,
					can be "used without MFC."

*/
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgLayout::CDlgLayout()
{
	tab=tab_end=NULL;
	mincx=mincy=0;
	oldcx=oldcy=0;
}

CDlgLayout::~CDlgLayout()
{

}

void CDlgLayout::Load(HWND _dlg, DLGLAYOUT *table, int size)
{
	dlg=_dlg;
	tab=table;
	tab_end=tab+size;
	RECT rc;
	GetClientRect(dlg,&rc);
	oldcx=rc.right-rc.left;
	oldcy=rc.bottom-rc.top;
	GetWindowRect(dlg,&rc);
	mincx=rc.right-rc.left;
	mincy=rc.bottom-rc.top;

/*	DLGLAYOUT* ptab;
	for( ptab=tab; ptab!=tab_end ; ptab++)
		ptab->id=(DWORD)GetDlgItem( dlg, ptab->id );
*/
}

void CDlgLayout::OnSize(int cx, int cy)
{
	RECT crc;
	int l,t,r,b;	//Control Rect
	int w,h;
	HDWP hdwp=BeginDeferWindowPos(tab_end-tab);
	DLGLAYOUT* ptab;
	for( ptab=tab; ptab!=tab_end ; ptab++)
	{
		HWND ctrl=GetDlgItem( dlg, ptab->id );
//		HWND ctrl=(HWND)ptab->id;
		GetWindowRect(ctrl,&crc);	//Control rect
		MapWindowPoints(HWND_DESKTOP,dlg,(LPPOINT)&crc,2);	//Screen to client
		l=crc.left;	t=crc.top;	r=oldcx-crc.right;	b=oldcy-crc.bottom;	//Calculate margins
		w=crc.right-crc.left;	h=crc.bottom-crc.top;	//Calculate size

		if(ptab->layout & DLA_RIGHT)	//right anchor
		{
			r=cx-r;	//right pos
			if(! (ptab->layout & DLA_LEFT)	) //if no left anchor
				l= r-w;
		}
		else
		{
			if(ptab->layout & DLA_LEFT)	//left anchor
			{
				//l=l;
				r=l+w;
			}
			else	// no horizontal anchors
			{

			}
		}

		if(ptab->layout & DLA_BOTTOM)	//bottom anchor
		{
			b=cy-b;
			if( !(ptab->layout & DLA_TOP) )	//if no top anchor
				t= b - h;
		}
		else
		{
			if(ptab->layout & DLA_TOP)	//top anchor
			{
				//t=t;
				b=t+h;
			}
			else	// no vertical anchors
			{

			}
		}
		w=r-l;
		h=b-t;

		DeferWindowPos(hdwp,ctrl,NULL,l,t,w,h,SWP_NOZORDER);
	}
	EndDeferWindowPos(hdwp);
	oldcx=cx;	oldcy=cy;
}
