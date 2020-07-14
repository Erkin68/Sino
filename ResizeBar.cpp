#include "DiskToolbar.h"
#include "ResizeBar.h"
#include "Profiler.h"
#include "strsafe.h"
#include "config.h"
#include "Sino.h"
#include "Operations\LinkSocket.h"


ATOM  ResizeBar::rcl=0;
short ResizeBar::OldX=0;
short ResizeBar::X=0;
short ResizeBar::OldCopyX=0;
POINT ResizeBar::crsrLBTNDWNSinoWndPos={0,0};

ResizeBar resizeBar[MAX_PANELS-1];

ResizeBar::ResizeBar()
{
}

BOOL ResizeBar::Create(HINSTANCE inst,HWND prnt,int BarNum,int x,int y,int w,int h)
{
wchar_t barWindText[16]=L"#";
wchar_t barNumbText[16]; StringCchPrintf(barNumbText,16,L"%d",BarNum);
					  MyStringCat(barWindText,16,barNumbText);
wchar_t barClassText[64]=L"Sino resize bar class #";
					  MyStringCpy(&barClassText[23],64-23,barNumbText);//MyStringCat(barClassText,64,barNumbText);

 if(!rcl)
 {	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= ResizeBarWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= inst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_SIZEWE);
	wcex.hbrBackground  = (HBRUSH)GetStockObject(GRAY_BRUSH); 
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= barClassText,
	wcex.hIconSm		= NULL;
	rcl = RegisterClassEx(&wcex);
 }

HWND hwnd = CreateWindowEx( WS_EX_LEFT|WS_EX_CLIENTEDGE,
							(LPCWSTR)rcl,//barClassText,
							barWindText,
							WS_VISIBLE |// | WS_CLIPSIBLINGS |
							WS_CHILDWINDOW,
							x,
							y,
							w,
							h,
							prnt,
							NULL,
							inst,
							NULL);
	if(NULL==hwnd) return FALSE;
	SetWindowLongPtr(hwnd,GWLP_USERDATA,BarNum);//SetWindowLong(hwnd,GWLP_USERDATA,BarNum);
	hWnd = hwnd; barNum = BarNum; left = x; top = y; width = w; height = h;
	toolTip.Create(inst,hWnd,barNum,10,10,L"...",1);
	return TRUE;
}

VOID ResizeBar::Destroy(HINSTANCE hinst,int BarNum)
{	
	toolTip.Destroy();
	DestroyWindow(hWnd);
wchar_t barWindText[16]=L"#";
wchar_t barNumbText[16]; StringCchPrintf(barNumbText,16,L"%d",BarNum);
					  MyStringCat(barWindText,16,barNumbText);
wchar_t barClassText[64]=L"Sino resize bar class #";
					  MyStringCpy(&barClassText[24],64-24,barNumbText);//MyStringCat(barClassText,64,barNumbText);
	UnregisterClass(barClassText,hinst);
}

VOID ResizeBar::Move(int x, int y, int w, int h)
{
//profilerOutStr("Beg of ResizeBar::Move")
//profilerPrintf4("x = %d, y = %d, w = %d, h = %d", x, y, w, h)

	if(!hWnd) return;
	left = x; top = y; width = w; height = h;
	MoveWindow(hWnd,x,y,w,h,TRUE);

//profilerOutStr("End of ResizeBar::Move")
}

BOOL ResizeBar::OnResize(int id, short x, short y, int horDX)
{
//profilerOutStr("Beg of ResizeBar::OnResize");
//profilerPrintf3("\npan=%d horDX=%d width=%d",id,horDX,panel[id].width)

static int cnt=0; cnt++;

int Width = ::rcClient.right - ::rcClient.left;//w
POINT ptScrn={(LONG)x,(LONG)y};
	ClientToScreen(hWnd,&ptScrn);

	switch(conf::Dlg.iTotPanels)
	{	default:
			//profilerOutStr("	ResizeBar::OnResize1");
		break;
		case 2:
			//profilerOutStr("	ResizeBar::OnResize2");
			//w = (Width-15)/2;
			//conf::resizeBarPersnt[0] = 50.0f*(panel[0].GetClientRight()+width/2-horDX)/w;
			conf::resizeBarPersnt[0] = 100.0f*(crsrLBTNDWNSinoWndPos.x-horDX)/Width;
			if(conf::resizeBarPersnt[0]<10.0f) {conf::resizeBarPersnt[0]=10.0f; return FALSE;};
			if(conf::resizeBarPersnt[0]>90.0f) {conf::resizeBarPersnt[0]=90.0f; return FALSE;}
			//profilerOutStr("	ResizeBar::OnResize3");
		break;
		case 3:
			//w = (Width-20)/3;
			if(0==id)
			{	//conf::resizeBarPersnt[0] = 33.33f*(panel[0].GetClientRight()-horDX)/w;
				conf::resizeBarPersnt[0] = 100.0f*(crsrLBTNDWNSinoWndPos.x-horDX)/Width;
				//profilerOutStr("	ResizeBar::OnResize4");
				if(conf::resizeBarPersnt[0]<10.0f) {conf::resizeBarPersnt[0]=10.0f; return FALSE;}
				if(conf::resizeBarPersnt[0]>conf::resizeBarPersnt[1]-10.0f) {conf::resizeBarPersnt[0]=conf::resizeBarPersnt[1]-10.0f; return FALSE;}
			} else if(1==id)
			{	//conf::resizeBarPersnt[1] = conf::resizeBarPersnt[0]+33.33f*(panel[1].GetClientRight()-horDX)/w;
				conf::resizeBarPersnt[1] = 100.0f*(crsrLBTNDWNSinoWndPos.x-horDX)/Width;
				//profilerOutStr("	ResizeBar::OnResize41");
				if(conf::resizeBarPersnt[1]<conf::resizeBarPersnt[0]+10.0f) {conf::resizeBarPersnt[1]=conf::resizeBarPersnt[0]+10.0f; return FALSE;}
				//profilerOutStr("	ResizeBar::OnResize42");
				if(conf::resizeBarPersnt[1]>90.0f) {conf::resizeBarPersnt[1]=90.0f; return FALSE;}
			}
			//profilerOutStr("	ResizeBar::OnResize5");
		break;
		case 4:
			//profilerOutStr("	ResizeBar::OnResize6");
			//w = (Width-25)/4;
			if(0==id)
			{	//conf::resizeBarPersnt[0] = 25.0f*(panel[0].GetClientRight()-horDX)/w;
				conf::resizeBarPersnt[0] = 100.0f*(crsrLBTNDWNSinoWndPos.x-horDX)/Width;
				if(conf::resizeBarPersnt[0]<10.0f) {conf::resizeBarPersnt[0]=10.0f; return FALSE;}
				if(conf::resizeBarPersnt[0]>conf::resizeBarPersnt[1]-10.0f) {conf::resizeBarPersnt[0]=conf::resizeBarPersnt[1]-10.0f; return FALSE;}
			} else if(1==id)
			{	//conf::resizeBarPersnt[1] = conf::resizeBarPersnt[0]+25.0f*(panel[1].GetClientRight()-horDX)/w;
				conf::resizeBarPersnt[1] = 100.0f*(crsrLBTNDWNSinoWndPos.x-horDX)/Width;
				if(conf::resizeBarPersnt[1]<conf::resizeBarPersnt[0]+10.0f) {conf::resizeBarPersnt[1]=conf::resizeBarPersnt[0]+10.0f; return FALSE;}
				if(conf::resizeBarPersnt[1]>conf::resizeBarPersnt[2]-10.0f) {conf::resizeBarPersnt[1]=conf::resizeBarPersnt[2]-10.0f; return FALSE;}
			} else if(2==id)
			{	//conf::resizeBarPersnt[2] = conf::resizeBarPersnt[1]+25.0f*(panel[2].GetClientRight()-horDX)/w;
				conf::resizeBarPersnt[2] = 100.0f*(crsrLBTNDWNSinoWndPos.x-horDX)/Width;
				if(conf::resizeBarPersnt[2]<conf::resizeBarPersnt[1]+10.0f) {conf::resizeBarPersnt[2]=conf::resizeBarPersnt[1]+10.0f; return FALSE;}
				if(conf::resizeBarPersnt[2]>90.0f) {conf::resizeBarPersnt[2]=90.0f; return FALSE;}
			}
			//profilerOutStr("	ResizeBar::OnResize7");
		break;
	}

	MoveWindow( hWnd,
				panel[barNum+1].GetLeft()-horDX-4,
				top,
				3,
				height,
				TRUE);

    //GetCursorPos(&ptScrn);
#pragma warning(disable:4129)
	wchar_t s[32];StringCchPrintf(s,32,L"%.2f %s",conf::resizeBarPersnt[id],L"\%");
#pragma warning(default:4129)
	toolTip.TrackPosition(ptScrn.x + 12, ptScrn.y+ 15, s, FALSE, FALSE);

	cnt--;
//profilerPrintf1("\nOnResize cnt= %d",cnt)
	return TRUE;
}

LRESULT CALLBACK ResizeBarWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int barNumber = (int)GetWindowLongPtr(hWnd,GWLP_USERDATA);//GetWindowLong(hWnd,GWLP_USERDATA);
		//static int i=0;
		//char ss[32];sprintf(ss,"\n %d ",i++);
		//OutputDebugStringA(ss);
		//OutputDebugStringA(GetWinNotifyText(message));

 switch (message)
 {	case WM_LBUTTONDOWN:
		SetCapture(resizeBar[barNumber].hWnd);
		resizeBar[barNumber].OldX = (short)(LOWORD(lParam));
		resizeBar[barNumber].crsrLBTNDWNSinoWndPos.x = (LONG)(LOWORD(lParam));
		resizeBar[barNumber].crsrLBTNDWNSinoWndPos.y = (LONG)(HIWORD(lParam));
		ClientToScreen(hWnd,&resizeBar[barNumber].crsrLBTNDWNSinoWndPos);
		ScreenToClient(::hWnd,&resizeBar[barNumber].crsrLBTNDWNSinoWndPos);
		WINDOWPLACEMENT winPos;
        GetWindowPlacement(hWnd , &winPos);
        resizeBar[barNumber].OldX += (short)winPos.rcNormalPosition.left;		
		//GetWindowRect(hWnd,&resizeBar[barNumber].rcOnMLBTNDWN);
        POINT pt; GetCursorPos(&pt);
#pragma warning(disable:4129)
		wchar_t s[32];StringCchPrintf(s,32,L"%.2f %s",conf::resizeBarPersnt[barNumber],L"\%");
#pragma warning(default:4129)
		resizeBar[barNumber].toolTip.TrackPosition(pt.x + 12, pt.y+ 15, s, FALSE, FALSE);
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();
		resizeBar[barNumber].toolTip.Hide();
		CmnCntrl::ResizeControls();
		CmnCntrl::DrawDskSpace(NULL,-1);
		return 0;
	case WM_LBUTTONDBLCLK:
		switch(conf::Dlg.iTotPanels)
		{	case 2:
				conf::resizeBarPersnt[0] = 50.0f;
				return CmnCntrl::ResizeControls();
			case 3:
				conf::resizeBarPersnt[0] = 33.333f;
				conf::resizeBarPersnt[1] = 66.667f;
				return CmnCntrl::ResizeControls();
			case 4:
				conf::resizeBarPersnt[0] = 25.0f;
				conf::resizeBarPersnt[1] = 50.0f;
				conf::resizeBarPersnt[2] = 75.0f;
				return CmnCntrl::ResizeControls();
		}				
		return 0;
	case WM_RBUTTONDOWN:int r;HMENU hMenu;
		switch(conf::Dlg.iTotPanels)
		{	case 2:
				pt.x=LOWORD(lParam);pt.y=HIWORD(lParam);
				ClientToScreen(hWnd,&pt);
				hMenu=GetSubMenu(LoadMenu(hInst,MAKEINTRESOURCE(IDM_MENU_RESIZE_RBTNDWN2)),0);
				r=TrackPopupMenu(hMenu,TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTALIGN,pt.x,pt.y,0,hWnd,0);
				DestroyMenu(hMenu);
				if(IDM_RESISING_50==r)
					conf::resizeBarPersnt[0] = 50.0f;
				else if(IDM_RESISING_75==r)
					conf::resizeBarPersnt[0] = 75.0f;
				else if(IDM_RESISING_25==r)
					conf::resizeBarPersnt[0] = 25.0f;
				else if(IDM_RESISING_70==r)
					conf::resizeBarPersnt[0] = 70.0f;
				else if(IDM_RESISING_30==r)
					conf::resizeBarPersnt[0] = 30.0f;
				else if(IDM_RESISING_60==r)
					conf::resizeBarPersnt[0] = 60.0f;
				else if(IDM_RESISING_40==r)
					conf::resizeBarPersnt[0] = 40.0f;
				else if(IDM_RESISING_90==r)
					conf::resizeBarPersnt[0] = 90.0f;
				else if(IDM_RESISING_10==r)
					conf::resizeBarPersnt[0] = 10.0f;
				return CmnCntrl::ResizeControls();
			case 3:
				pt.x=LOWORD(lParam);pt.y=HIWORD(lParam);
				ClientToScreen(hWnd,&pt);
				hMenu=GetSubMenu(LoadMenu(hInst,MAKEINTRESOURCE(IDM_MENU_RESIZE_RBTNDWN3)),0);
				r=TrackPopupMenu(hMenu,TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTALIGN,pt.x,pt.y,0,hWnd,0);
				DestroyMenu(hMenu);
				if(IDM_RESIZING_33==r)
				{	conf::resizeBarPersnt[0] = 33.333f;
					conf::resizeBarPersnt[1] = 66.667f;
				}
				else if(IDM_RESIZING_40==r)
				{	conf::resizeBarPersnt[0] = 40.0f;
					conf::resizeBarPersnt[1] = 70.0f;
				}
				else if(IDM_RESIZING_30==r)
				{	conf::resizeBarPersnt[0] = 30.0f;
					conf::resizeBarPersnt[1] = 70.0f;
				}
				else if(IDM_RESIZING_31==r)
				{	conf::resizeBarPersnt[0] = 30.0f;
					conf::resizeBarPersnt[1] = 60.0f;
				}
				else if(IDM_RESIZING_50==r)
				{	conf::resizeBarPersnt[0] = 50.0f;
					conf::resizeBarPersnt[1] = 75.0f;
				}
				else if(IDM_RESIZING_25==r)
				{	conf::resizeBarPersnt[0] = 25.0f;
					conf::resizeBarPersnt[1] = 75.0f;
				}
				else if(IDM_RESIZING_26==r)
				{	conf::resizeBarPersnt[0] = 25.0f;
					conf::resizeBarPersnt[1] = 50.0f;
				}
				else if(IDM_RESIZING_60==r)
				{	conf::resizeBarPersnt[0] = 60.0f;
					conf::resizeBarPersnt[1] = 80.0f;
				}
				else if(IDM_RESIZING_20==r)
				{	conf::resizeBarPersnt[0] = 20.0f;
					conf::resizeBarPersnt[1] = 80.0f;
				}
				else if(IDM_RESIZING_21==r)
				{	conf::resizeBarPersnt[0] = 20.0f;
					conf::resizeBarPersnt[1] = 40.0f;
				}
				else if(IDM_RESIZING_70==r)
				{	conf::resizeBarPersnt[0] = 70.0f;
					conf::resizeBarPersnt[1] = 85.0f;
				}
				else if(IDM_RESIZING_15==r)
				{	conf::resizeBarPersnt[0] = 15.0f;
					conf::resizeBarPersnt[1] = 85.0f;
				}
				else if(IDM_RESIZING_16==r)
				{	conf::resizeBarPersnt[0] = 15.0f;
					conf::resizeBarPersnt[1] = 30.0f;
				}
				else if(IDM_RESIZING_80==r)
				{	conf::resizeBarPersnt[0] = 80.0f;
					conf::resizeBarPersnt[1] = 90.0f;
				}
				else if(IDM_RESIZING_10==r)
				{	conf::resizeBarPersnt[0] = 10.0f;
					conf::resizeBarPersnt[1] = 90.0f;
				}
				else if(IDM_RESIZING_11==r)
				{	conf::resizeBarPersnt[0] = 10.0f;
					conf::resizeBarPersnt[1] = 20.0f;
				}
				return CmnCntrl::ResizeControls();
			case 4:
				pt.x=LOWORD(lParam);pt.y=HIWORD(lParam);
				ClientToScreen(hWnd,&pt);
				hMenu=GetSubMenu(LoadMenu(hInst,MAKEINTRESOURCE(IDM_MENU_RESIZE_RBTNDWN4)),0);
				r=TrackPopupMenu(hMenu,TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTALIGN,pt.x,pt.y,0,hWnd,0);
				DestroyMenu(hMenu);
				if(IDM_RESIZING_27==r)
				{	conf::resizeBarPersnt[0] = 25.0f;
					conf::resizeBarPersnt[1] = 50.0f;
					conf::resizeBarPersnt[2] = 75.0f;
				}
				else if(IDM_RESIZING_41==r)
				{	conf::resizeBarPersnt[0] = 40.0f;
					conf::resizeBarPersnt[1] = 60.0f;
					conf::resizeBarPersnt[2] = 80.0f;
				}
				else if(IDM_RESIZING_22==r)
				{	conf::resizeBarPersnt[0] = 20.0f;
					conf::resizeBarPersnt[1] = 60.0f;
					conf::resizeBarPersnt[2] = 80.0f;
				}
				else if(IDM_RESIZING_23==r)
				{	conf::resizeBarPersnt[0] = 20.0f;
					conf::resizeBarPersnt[1] = 40.0f;
					conf::resizeBarPersnt[2] = 80.0f;
				}
				else if(IDM_RESIZING_24==r)
				{	conf::resizeBarPersnt[0] = 20.0f;
					conf::resizeBarPersnt[1] = 40.0f;
					conf::resizeBarPersnt[2] = 60.0f;
				}
				else if(IDM_RESIZING_55==r)
				{	conf::resizeBarPersnt[0] = 55.0f;
					conf::resizeBarPersnt[1] = 70.0f;
					conf::resizeBarPersnt[2] = 85.0f;
				}
				else if(IDM_RESIZING_17==r)
				{	conf::resizeBarPersnt[0] = 15.0f;
					conf::resizeBarPersnt[1] = 70.0f;
					conf::resizeBarPersnt[2] = 85.0f;
				}
				else if(IDM_RESIZING_18==r)
				{	conf::resizeBarPersnt[0] = 15.0f;
					conf::resizeBarPersnt[1] = 30.0f;
					conf::resizeBarPersnt[2] = 85.0f;
				}
				else if(IDM_RESIZING_19==r)
				{	conf::resizeBarPersnt[0] = 15.0f;
					conf::resizeBarPersnt[1] = 30.0f;
					conf::resizeBarPersnt[2] = 45.0f;
				}
				else if(IDM_RESIZING_71==r)
				{	conf::resizeBarPersnt[0] = 70.0f;
					conf::resizeBarPersnt[1] = 80.0f;
					conf::resizeBarPersnt[2] = 90.0f;
				}
				else if(IDM_RESIZING_12==r)
				{	conf::resizeBarPersnt[0] = 10.0f;
					conf::resizeBarPersnt[1] = 80.0f;
					conf::resizeBarPersnt[2] = 90.0f;
				}
				else if(IDM_RESIZING_13==r)
				{	conf::resizeBarPersnt[0] = 10.0f;
					conf::resizeBarPersnt[1] = 20.0f;
					conf::resizeBarPersnt[2] = 90.0f;
				}
				else if(IDM_RESIZING_14==r)
				{	conf::resizeBarPersnt[0] = 10.0f;
					conf::resizeBarPersnt[1] = 50.0f;
					conf::resizeBarPersnt[2] = 60.0f;
				}
				return CmnCntrl::ResizeControls();
		}
		return 0;
	case WM_MOUSEMOVE:
		if(hWnd!=GetCapture())break;
		if(!(wParam & MK_LBUTTON)) break;
		resizeBar[barNumber].X = (short)(LOWORD(lParam));
        GetWindowPlacement(hWnd , &winPos);
        resizeBar[barNumber].X += (short)winPos.rcNormalPosition.left;
		if(resizeBar[barNumber].X != resizeBar[barNumber].OldX)
		{	if(resizeBar[barNumber].OnResize(barNumber,(short)(LOWORD(lParam)),
								(short)(HIWORD(lParam)),resizeBar[barNumber].OldX-resizeBar[barNumber].X))
				resizeBar[barNumber].OldCopyX = resizeBar[barNumber].X;
		}
		break;
	default:
	break;
 }
 return DefWindowProc(hWnd, message, wParam, lParam);
}
