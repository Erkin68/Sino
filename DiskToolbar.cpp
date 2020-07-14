#include "WindowsManagmentInstrumentation.h"
#include "windows.h"
#include "commctrl.h"
#include "strsafe.h"
#include "config.h"
#include "sino.h"



namespace DskToolBar
{
int		totLogDsks;
TBADDBITMAP tbab[7];
HWND	bar[4];
DWORD	DskDrivesBitMask=NULL;
wchar_t	logDiskNames[32][2];
int		iSameDskPushd[32]={0,0,0,0,0,0,0,0,0,0,
						   0,0,0,0,0,0,0,0,0,0,
						   0,0,0,0,0,0,0,0,0,0,
						   0,0};
wchar_t	oldPathsOnDiskPanels[32][MAX_PATH]={L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
											L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
											L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",
											L"",L""};
BOOL Create(HWND hWnd,int *y)
{
int w,width;
	width = rcClient.right - rcClient.left;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			bar[0] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								0,*y,width-4,42,hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[0]) return FALSE;
			*y += 42;
		break;
		case 2:
 			w = (width-9)/2;
			bar[0] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								0,*y,w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[0]) return FALSE;
			bar[1] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),*y,
								w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[1]) return FALSE;
			*y += 42;
		break;
		case 3:
			w = (width-14)/3;
			bar[0] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								0,*y,w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[0]) return FALSE;
			bar[1] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),*y,
								(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[1]) return FALSE;
			bar[2] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								9 + (int)(w*0.03f*conf::resizeBarPersnt[1]),*y,
								(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[2]) return FALSE;
			*y += 42;
		break;
		case 4:
			bar[0] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								0,*y,(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f)),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[0]) return FALSE;
			bar[1] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),*y,
								(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[1]) return FALSE;
			bar[2] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),*y,
								(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f)),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[2]) return FALSE;
			bar[3] = CreateWindowEx(0, TOOLBARCLASSNAME, (LPWSTR)NULL,
								WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CHECK|CCS_NORESIZE,
								17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19)),*y,
								(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f)),42,
								hWnd,(HMENU)IDB_DSK_BTNSTOOLBAR,hInst,NULL);
			if(!bar[3]) return FALSE;
			*y += 42;
		break;
	}

	DskDrivesBitMask = GetLogicalDrives();
	if(!DskDrivesBitMask) return FALSE;
	if(-1==myWMI::BuildLogicalDrives())
	{	if(-1==myWMI::BuildLogicalDrivesWithoutAccess())
			return FALSE;
	}
	for(int d=0; d<32; d++)
		{logDiskNames[d][0]=0;logDiskNames[d][1]=0;}

	totLogDsks=0;TBBUTTON tbb[32];
	for(int d=0; d<32; d++)
	{	DWORD mask = (DskDrivesBitMask>>d) & 0x00000001;
		if(mask)
		{	logDiskNames[totLogDsks][0]='a'+d;logDiskNames[totLogDsks][1]=0;
			for(int i=0; i<conf::Dlg.iTotPanels; i++)
				SendMessage(bar[i],TB_ADDSTRING,0,(LPARAM)(LPSTR)&logDiskNames[totLogDsks][0]);
			tbb[totLogDsks].idCommand = IDB_DSK_BTN0+totLogDsks;
			tbb[totLogDsks].fsState = TBSTATE_ENABLED;
			tbb[totLogDsks].fsStyle = BTNS_BUTTON;
			tbb[totLogDsks].dwData = 0;
			tbb[totLogDsks].iString = totLogDsks;//(int)logDiskNames[totLogDsks];
			switch(myWMI::GetLogicalDriveMediaType(totLogDsks))
			{	case 11: tbb[totLogDsks].iBitmap = 2; break;//Othe then FDD
				case 12: tbb[totLogDsks].iBitmap = 0; break;//HDD
				case 10: tbb[totLogDsks].iBitmap = 1; break;//FDD
				case 2 : 
				default: tbb[totLogDsks].iBitmap = 3; break;//USB
				case 4:  tbb[totLogDsks].iBitmap = 5; break;//Network Drive
				case 5:  tbb[totLogDsks].iBitmap = 2; break;//Compact Disc
				case 6:  tbb[totLogDsks].iBitmap = 4; break;//RAM Disk
			}
			++totLogDsks;
	}	}

	for(int bmp=0; bmp<7; bmp++)
	{	tbab[bmp].hInst=NULL;
		tbab[bmp].nID=(UINT_PTR)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP_DISK+bmp));
	}//int n;
	for(int i=0; i<conf::Dlg.iTotPanels; i++)
	{	SendMessage(bar[i], TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0);
		for(int bmp=0; bmp<6; bmp++)
			SendMessage(bar[i],TB_ADDBITMAP,1,(LPARAM)&tbab[bmp]);
		SendMessage(bar[i], TB_ADDBUTTONS,(WPARAM)totLogDsks,(LPARAM)(LPTBBUTTON)&tbb[0]);
		SendMessage(bar[i], TB_SETBUTTONSIZE, 0,(LPARAM)MAKELONG(32, 24));
		SendMessage(bar[i], TB_SETBITMAPSIZE, 0,(LPARAM)MAKELONG(32, 16));
		ShowWindow (bar[i], SW_SHOW); 
		//SendMessage(bar[i], TB_MARKBUTTON, IDB_DSK_BTN0, TRUE);
		//n=SendMessage(bar[i],TB_BUTTONCOUNT,0,0);
	}
	return TRUE;
}

VOID Destroy()
{
	for(int i=0; i<MAX_PANELS/*conf::Dlg.iTotPanels*/; i++)
	{	ImageList_Destroy((HIMAGELIST)SendMessage(bar[i],TB_GETIMAGELIST,0,0));
		int iTotBtns = (int)SendMessage(bar[i],TB_BUTTONCOUNT,0,0);
		for(int b=0; b<iTotBtns; b++)
			SendMessage(bar[i],TB_DELETEBUTTON,b,0);
		DestroyWindow(bar[i]);
		bar[i]=NULL;
	}
	for(int k=0; k<7; k++)
		DeleteObject((HBITMAP)tbab[k].nID);
}

int FindDiskFromPath(wchar_t* pth)
{
int c=-1;
	if(pth[0]>='A' && pth[0]<='Z')
		c=pth[0]-'A';
	else if(pth[0]>='a' && pth[0]<='z')
		c=pth[0]-'a';
	for(int d=0; d<32; d++)
	{	if(logDiskNames[d][0]=='A'+c)
			return d;
		if(logDiskNames[d][0]=='a'+c)
			return d;
	}
	return c;
}

int GetBtn(POINT* pt)
{
POINT rc; int r=-1;
	rc.x = pt->x;
	rc.y = pt->y;
	int btnCnt = (int)SendMessage(bar[0],TB_BUTTONCOUNT,0,0);
	DWORD btnSz = (DWORD)SendMessage(bar[0],TB_GETBUTTONSIZE,0,0);
	int btnWdth = LOWORD(btnSz);
	int btnHeight = HIWORD(btnSz);
	int tbWdth = btnCnt * btnWdth;
	switch(conf::Dlg.iTotPanels)//TB_BUTTONCOUNT TB_GETBUTTONINFO
	{	case 1:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = rc.x/btnWdth;
		break;
		case 2: default:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = rc.x/btnWdth;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[1],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = rc.x/btnWdth;
			}
		break;
		case 3:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = rc.x/btnWdth;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[1],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = rc.x/btnWdth;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[2],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = rc.x/btnWdth;
			}
		break;
		case 4:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = rc.x/btnWdth;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[1],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = rc.x/btnWdth;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[2],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = rc.x/btnWdth;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[3],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = rc.x/btnWdth;
			}
		break;
	}
	return r;
}

int IsPointInsideRect(POINT* pt)
{
POINT rc; int r=-1;
	rc.x = pt->x;
	rc.y = pt->y;
	int btnCnt = (int)SendMessage(bar[0],TB_BUTTONCOUNT,0,0);
	DWORD btnSz = (DWORD)SendMessage(bar[0],TB_GETBUTTONSIZE,0,0);
	int btnWdth = LOWORD(btnSz);
	int btnHeight = HIWORD(btnSz);
	int tbWdth = btnCnt * btnWdth;
	switch(conf::Dlg.iTotPanels)//TB_BUTTONCOUNT TB_GETBUTTONINFO
	{	case 1:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = 0;
		break;
		case 2: default:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[1],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = 1;
			}
		break;
		case 3:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[1],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = 1;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[2],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = 2;
			}
		break;
		case 4:
			ScreenToClient(bar[0],&rc);
			if(rc.x>0)
			if(rc.x<tbWdth)
			if(rc.y>0)
			if(rc.y<btnHeight)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[1],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = 1;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[2],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = 2;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(bar[3],&rc);
				if(rc.x>0)
				if(rc.x<tbWdth)
				if(rc.y>0)
				if(rc.y<btnHeight)
					r = 3;
			}
		break;
	}
	return r;
}

BOOL Resize(int *y)
{
int w,width;
	width = rcClient.right - rcClient.left;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			MoveWindow( bar[0],
						0,*y,width-4,42,TRUE);
			*y += 42;
		break;
		case 2:
 			w = (width-9)/2;
			MoveWindow( bar[0],
						0,*y,w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),42,TRUE);
			MoveWindow( bar[1],
						w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),*y,
						w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),42,TRUE);
			*y += 42;
		break;
		case 3:
			w = (width-14)/3;
			MoveWindow( bar[0],
						0,*y,w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),42,TRUE);
			MoveWindow( bar[1],
						7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),*y,
						(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),42,TRUE);
			MoveWindow( bar[2],
						12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),*y,
						(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),42,TRUE);
			*y += 42;
		break;
		case 4:
			w = (width-19)/4;
			MoveWindow( bar[0],
						0,*y,(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f)),42,TRUE);
			MoveWindow( bar[1],
						7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),*y,
						(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),42,TRUE);
			MoveWindow( bar[2],
						12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),*y,
						(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-19.0f)),42,TRUE);
			MoveWindow( bar[3],
						20 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-25)),*y,
						(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-25.0f)),42,TRUE);
			*y += 42;
		break;
	}
	return TRUE;
}

}//namespace DskBtnTB