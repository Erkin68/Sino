#ifndef _Toolcha_h
#define _Toolcha_h

#include "windows.h"
#include "commctrl.h"


#define MAX_TOOLTIP_TEXT		1024
//Shundan teziga TRACKPOSITION QILMASUN:
#define TIME_TICK_PUMP			50
#define TIME_DELAY_FOR_SLEEP	3000

class MyToolTip
{
static ATOM rcl;
public:
friend LRESULT CALLBACK ToolWndProc(HWND,UINT,WPARAM,LPARAM);
friend LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

		MyToolTip();
	   ~MyToolTip();
		VOID ChangeText(wchar_t*,int);
		BOOL Create(HINSTANCE,HWND,int,int,int,wchar_t*,int);
		VOID Destroy();
		VOID Hide();
		VOID TrackPosition(int,int,wchar_t* cptn=NULL,BOOL bToScreen=TRUE,BOOL bIntensive=TRUE);

typedef enum TAlign
{
	leftAligned=0,
	centerAligned=1,
	rightAligned=2
} Align;

protected:
static HBRUSH bckGrndBrush;

	HWND	hWnd,prnt;
	BOOL    bVisible;
	wchar_t	*txt;
	int     *rowPtrs;
	int		txtLn;
	//float delayTime;
	int     iRows;
	int		userParam;//user param;
	int     left,top,width,height;
	SIZE	DCMaxSz;
	Align   align;
	DWORD   hideTime;
	DWORD	tickForIntensiveMouseMoving;
	DWORD	tickCntForIntensiveMouseMoving;
	BOOL	bSleepForIntensiveMouseMoving;

	BOOL	CalcDCSizes();
	BOOL	Render(HDC);
	BOOL	SetText(wchar_t*);
	LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
};

#endif
