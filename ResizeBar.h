#ifndef __RESIZE_BAR_H__
#define __RESIZE_BAR_H__

#include "windows.h"
#include "Panel.h"




class ResizeBar
{
static short OldX,X,OldCopyX;
static POINT crsrLBTNDWNSinoWndPos;
static ATOM  rcl;

public:
friend BOOL				ResizePanelresizeBar();
friend LRESULT CALLBACK ResizeBarWndProc(HWND,UINT,WPARAM,LPARAM);

			ResizeBar();
	BOOL	Create(HINSTANCE,HWND,int,int,int,int,int);
	VOID	Move(int,int,int,int);
	VOID	Destroy(HINSTANCE,int);

protected:
	HWND	hWnd;
	int		barNum,left,top,width,height;
	BOOL	OnResize(int,short,short,int);
	MyToolTip toolTip;
};

extern ResizeBar resizeBar[MAX_PANELS-1];

#endif