#include "Profiler.h"
#include "Toolcha.h"
#include "strsafe.h"
#include "Panel.h"
#include "sino.h"
#include "Operations\MyErrors.h"
#include "Operations\MyShell\MyShell.h"


#define TIME_FOR_SLEEPPING_IF_INTENSIVE_MOUSE_MOVING 2500
#define SLEEP_FOR_INTENSIVE_MOUSE_MOVING_TIME	     2000

ATOM MyToolTip::rcl=0;
HBRUSH MyToolTip::bckGrndBrush = NULL;

MyToolTip::MyToolTip():
	txt(NULL),
	rowPtrs(NULL),
	tickCntForIntensiveMouseMoving(0),
	bSleepForIntensiveMouseMoving(FALSE)
{
}

MyToolTip::~MyToolTip()
{
	if(bckGrndBrush ){ DeleteObject(bckGrndBrush); bckGrndBrush = NULL; }
	Destroy();
	if(txt) free(txt);
	txt = NULL;//[MAX_TOOLTIP_TEXT]
	if(rowPtrs) free(rowPtrs);
	rowPtrs = NULL;
}

BOOL MyToolTip::CalcDCSizes()
{
//BOOL resize=FALSE; MoveWind ni o'zi qiladi;
HDC dc = GetDC(hWnd);
int l;SIZE sz;
	DCMaxSz.cx = DCMaxSz.cy = 0;
	for(int r=0; r<iRows; r++)
	{	if(r<iRows-1)
			l = rowPtrs[r+1]-rowPtrs[r];
		else
			l = txtLn - rowPtrs[r];
		GetTextExtentPoint32(dc,&txt[rowPtrs[r]],l,&sz);
		if(DCMaxSz.cx < sz.cx)DCMaxSz.cx = sz.cx;
		if(DCMaxSz.cy < sz.cy)DCMaxSz.cy = sz.cy;
	}
	//int w
	width = DCMaxSz.cx+6;//2 tarafidan 2 tadan joy;
	//int h 
	height = iRows*(DCMaxSz.cy+2)+6;//2 tarafidan 2 tadan joy;
	ReleaseDC(hWnd,dc);
	//if(w > width) r = TRUE;
	//else if(h > height) r = TRUE;
	//width = w; height = h;	
	return TRUE;//r;
}

static LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 if(WM_CREATE==message)
 {	CREATESTRUCT* p=(CREATESTRUCT*)lParam;
	SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)p->lpCreateParams);
 }
 else
 {	MyToolTip *tt = (MyToolTip*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	if(tt)
		return tt->WndProc(hWnd,message,wParam,lParam);
 }
 return DefWindowProc(hWnd,message,wParam,lParam);	
}

VOID MyToolTip::ChangeText(wchar_t* t,int ln)
{
	SetText(t);
	CalcDCSizes();
	if(!bVisible)
		ShowWindow(hWnd,SW_SHOWNA);
	Render(NULL);
}

BOOL MyToolTip::Create(HINSTANCE hInst,HWND pr,int id,int x,int y,wchar_t* Txt,int alignType)
{
	SetText(Txt);

wchar_t toolWndText[16]=L"#";
wchar_t toolNumbText[16]; StringCchPrintf(toolNumbText,16,L"%d",id);
					  MyStringCat(toolWndText,16,toolNumbText);
wchar_t toolClassText[64]=L"Sino tooltip class #";
					  MyStringCpy(&toolClassText[21],64-21,toolNumbText);//MyStringCat(toolClassText,64,toolNumbText);

	if(!bckGrndBrush) bckGrndBrush = CreateSolidBrush(RGB(220,220,255));
  if(!rcl)
  {	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= ToolWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= bckGrndBrush;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= toolClassText;
	wcex.hIconSm		= NULL;
	rcl = RegisterClassEx(&wcex);
  }
	
	POINT pt; pt.x=x;pt.y=y;ClientToScreen(pr,&pt);
	left = pt.x; top = pt.y;
	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED,
						  (LPCWSTR)rcl,//toolClassText,
						  toolWndText,
                          WS_POPUP|WS_BORDER,
                          left,
						  top,
						  width,
						  height,
                          pr,
						  NULL,
						  hInst,
						  this);//NULL);
	if(!hWnd) return FALSE;
	SetLayeredWindowAttributes(hWnd, 0, 200, LWA_ALPHA);
	//SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG)this);
	align = (TAlign)alignType;
	userParam = id;
	prnt = pr;
	bVisible = FALSE;
	ShowWindow(hWnd,SW_HIDE);
	UpdateWindow(hWnd);
	return TRUE;
}

VOID MyToolTip::Destroy()
{
	DestroyWindow(hWnd);
}

VOID MyToolTip::Hide()
{
	if(bVisible)
	{	//profilerPrintf1("\nTooltip hide via ::Hide %d\n",userParam)
		KillTimer(NULL,userParam);
		ShowWindow(hWnd,SW_HIDE);
		bVisible = FALSE;
}	}

BOOL MyToolTip::Render(HDC DC)
{
HDC dc = DC;RECT rc;
	if(!DC) dc = GetDC(hWnd);

	//SetTextColor(dc, RGB(0,0,55));
	//SetBkColor(dc, RGB(255,255,255));

	rc.left=0;rc.top=0;rc.right=width;rc.bottom=height;
	FillRect(dc, &rc, (HBRUSH) (COLOR_WINDOW+1));

	int y=2,a = align, l;
	for(int r=0; r<iRows; r++)
	{	l = (r<iRows-1)?(rowPtrs[r+1]-rowPtrs[r]):(txtLn-rowPtrs[r]);
		switch(a)
		{case leftAligned:
			rc.left=2;rc.top=y;rc.right=2+DCMaxSz.cx;rc.bottom=y+DCMaxSz.cy;
			TextOut(dc, 2, y, &txt[rowPtrs[r]], l);
			y += DCMaxSz.cy+2;
		 break;
		 case centerAligned:
			rc.left=2;rc.top=y;rc.right=2+DCMaxSz.cx;rc.bottom=y+DCMaxSz.cy;
			DrawText(dc, &txt[rowPtrs[r]], l, &rc, DT_CENTER);
			y += DCMaxSz.cy+2;
		 break;
		 case rightAligned:
			rc.left=2;rc.top=y;rc.right=2+DCMaxSz.cx;rc.bottom=y+DCMaxSz.cy;
			DrawText(dc, &txt[rowPtrs[r]], l, &rc, DT_RIGHT);
			y += DCMaxSz.cy+2;
		 break;
	}	}

	if(!DC) ReleaseDC(hWnd,dc);
	return TRUE;
}

//0x0d + 0x0a ret caret, 00 endStr
BOOL MyToolTip::SetText(wchar_t* Txt)
{
int i,ln;
	//Calc total rows:
	iRows = 1;
	ln = lstrlen(Txt);
	if(!ln) return FALSE;

	for(i=0; i<ln; i++)
	{	if(0x0d==Txt[i])if(0x0a==Txt[i+1])
			{++iRows; i+=2;}
	}

	if(ln>txtLn)
	{	if(txt) free(txt); txt = NULL;//[MAX_TOOLTIP_TEXT]
		txt = (wchar_t*)malloc(2*(ln+1));
		if(!txt) return FALSE;
	}
	txtLn = ln;
	for(i=0; i<txtLn; i++) txt[i] = Txt[i];
	txt[i] = '\0';

	if(rowPtrs) free(rowPtrs); rowPtrs = NULL;
	rowPtrs = (int*)malloc(iRows*sizeof(int));
	if(!rowPtrs) return FALSE;

	int irows=0;rowPtrs[0]=0;
	for(i=0; i<txtLn; i++)
	{	if(0x0d==Txt[i])if(0x0a==Txt[i+1])
		{	rowPtrs[++irows]=i+2;
			i+=2;
	}	}
	return TRUE;
}

VOID MyToolTip::TrackPosition(int x, int y, wchar_t *cptn, BOOL bToScreen, BOOL bIntensive)
{
static POINT pt;
	if(bIntensive)
	{	if(bSleepForIntensiveMouseMoving)
		{	tickCntForIntensiveMouseMoving += tickProced-tickForIntensiveMouseMoving;
			if(tickCntForIntensiveMouseMoving > SLEEP_FOR_INTENSIVE_MOUSE_MOVING_TIME)
			{	tickCntForIntensiveMouseMoving = 0;
				bSleepForIntensiveMouseMoving = FALSE;
			} else { tickForIntensiveMouseMoving = tickProced; return; }
		}
		//If mouse track intensive for any time:
		if(200>tickProced-tickForIntensiveMouseMoving)
		{	tickCntForIntensiveMouseMoving += tickProced-tickForIntensiveMouseMoving;
			if(tickCntForIntensiveMouseMoving > TIME_FOR_SLEEPPING_IF_INTENSIVE_MOUSE_MOVING)
			{	tickCntForIntensiveMouseMoving = 0;
				Hide(); bSleepForIntensiveMouseMoving = TRUE; return;
		}	}
		else
			tickCntForIntensiveMouseMoving = 0;
		tickForIntensiveMouseMoving = tickProced;
	}
	if(cptn)
	{	SetText(cptn);
		CalcDCSizes();
	} else
	{	if(!bVisible)
		{	if(tickProced-hideTime < TIME_DELAY_FOR_SLEEP/100)
			{	Hide();
				return;
	}	}	}

	pt.x=x;pt.y=y;
	if(bToScreen) ClientToScreen(prnt,&pt);
	if(pt.x==left)if(pt.y==top)return;
	left = pt.x; top = pt.y;
	MoveWindow(hWnd, left, top, width, height, TRUE);

	if(!bVisible)
		ShowWindow(hWnd,SW_SHOWNA);
	Render(NULL);
}

LRESULT CALLBACK MyToolTip::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
		if(WA_CLICKACTIVE==LOWORD(wParam))
		{	SetActiveWindow((HWND)lParam);
			return 0;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_SHOWWINDOW:
		if(TRUE==wParam && (!bVisible))
		{	SetTimer(::hWnd,userParam+1,TIME_DELAY_FOR_SLEEP,NULL);
			bVisible = TRUE;
			//profilerPrintf1("\nTooltip WM_SHOWWINDOW %d\n",userParam)
		}else
		{	KillTimer(::hWnd,userParam+1);//not zero ident must be;
			hideTime = tickProced;
			bVisible = FALSE;
			//profilerPrintf1("\nTooltip WM_SHOWWINDOW HIDE %d\n",userParam)
		}
		break;
	case WM_PAINT:
		PAINTSTRUCT	ps;
		HDC			dc;
		dc= BeginPaint(hWnd,&ps);
		Render(dc);
		EndPaint(hWnd,&ps);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

#undef TIME_FOR_SLEEPPING_IF_INTENSIVE_MOUSE_MOVING
#undef SLEEP_FOR_INTENSIVE_MOUSE_MOVING_TIME





/************************************************************** UsingToolTip.cpp : Defines the entry point for the application.

	case WM_MOUSEMOVE:
		//static BOOL bChTTRc;
		if(bTTVisible)
		{	SendMessage(hwndTT,TTM_UPDATE,0,0);
			//bChTTRc = TRUE;
		}
		else// if(bChTTRc)
		{	//ti.cbSize = sizeof(TOOLINFO);
			//ti.uFlags = TTF_SUBCLASS;
			//ti.hwnd = hWnd;
			//ti.uId = 0;
			//ti.rect.left = LOWORD(lParam);    
			//ti.rect.top = HIWORD(lParam);
			//ti.rect.right = LOWORD(lParam)+35;
			//ti.rect.bottom = HIWORD(lParam)+25;
			//SendMessage(hwndTT,TTM_NEWTOOLRECT,(WPARAM)TRUE,0);
			//bChTTRc = FALSE;
			SendMessage(hwndTT,TTM_POPUP,0,0);			
		}
		break;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
        {	case TTN_SHOW:
				bTTVisible = TRUE;
				break;
        	case TTN_POP:
				bTTVisible = FALSE;
				break;
		}
		break;


HWND WINAPI CreateTT(HWND hwndOwner)
{
	INITCOMMONCONTROLSEX iccex;
	TOOLINFO ti;
    unsigned int uid = 0;       // for ti initialization
    char strTT[30] = "This is your ToolTip string.";
    LPTSTR lptstr = strTT;
    RECT rect;                  // for client area coordinates

    iccex.dwICC = ICC_WIN95_CLASSES;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);

    if(!InitCommonControlsEx(&iccex))
       return NULL;
	   
    // Create the ToolTip control.
    hwndTT = CreateWindow(TOOLTIPS_CLASS, TEXT(""),
                          WS_POPUP,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          hwndOwner, (HMENU)NULL, hInst,
                          NULL);

	SetWindowPos(hwndTT,
				HWND_TOPMOST,
				0,
				0,
				0,
				0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    CmnCntrl::GetClientRect (hwndOwner, &rect);

    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hwndOwner;
    ti.hinst = hInst;
    ti.uId = uid;
    ti.lpszText = lptstr;
        // ToolTip control will cover the whole window
    ti.rect.left = rect.left;    
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

    // Add the tool to the control, displaying an error if needed.
    if(!SendMessage(hwndTT,TTM_ADDTOOL,0,(LPARAM)&ti))
	{   MessageBox(hwndOwner,"Couldn't create the ToolTip control.","Error",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
        return NULL;
    }

    SendMessage(hwndTT,TTM_ACTIVATE,(WPARAM)TRUE,(LPARAM)&ti);
	SendMessage(hwndTT,TTM_SETDELAYTIME,TTDT_AUTOPOP,1000);
	SendMessage(hwndTT,TTM_SETDELAYTIME,TTDT_INITIAL,100);
	SendMessage(hwndTT,TTM_SETDELAYTIME,TTDT_RESHOW,100);
	//SendMessage(hwndTT,TTDT_AUTOMATIC,TTDT_RESHOW,500);
    //bTTVisible = TRUE;

    return(hwndTT);    
}
*********************************************************************************************************************************/