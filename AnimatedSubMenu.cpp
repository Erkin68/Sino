#include "windows.h"
#include "AnimatedSubMenu.h"
#include "strsafe.h"
#include <malloc.h>
#include "Wingdi.h"
#include "MenuBar.h"

extern HINSTANCE hInst;

int SubMenu::iTotal = 0;
SubMenu *subMenus = NULL;

LRESULT CALLBACK SubMenuWndProc(HWND, UINT, WPARAM, LPARAM);


SubMenu::SubMenu()
{
}

SubMenu::~SubMenu()
{
}

bool SubMenu::ReserveTotalSubMenus(int total)
{
	subMenus = (SubMenu*)malloc(total*sizeof(SubMenu));
	if(!subMenus) return false;
	iTotal = total;
	return true;
}

bool SubMenu::InitEachSubMenu(long hwnd,long dc,int n,int TotElems,char* cptns, ...)
{
va_list args;
va_start(args, cptns);
	char *CPTNS = cptns;

	subMenus[n].elements = (Element*)malloc(TotElems*sizeof(Element));
	if(!subMenus[n].elements) return false;
	subMenus[n].totElems = TotElems;
	subMenus[n].staticWidth = 0;
	for(int e=0; e<TotElems; e++)
	{	if(S_OK!=StringCchLength(CPTNS,MAX_SUBMENU_ELEM_TEXT,(size_t*)&subMenus[n].elements[e].ln))
			return false;
		StringCchPrintf(subMenus[n].elements[e].caption,MAX_SUBMENU_ELEM_TEXT,CPTNS);
		long s[2];GetTextExtentPoint32((HDC)dc,CPTNS,subMenus[n].elements[e].ln,(LPSIZE)s);
		if(subMenus[n].staticWidth < s[0])subMenus[n].staticWidth = s[0];
		subMenus[n].elements[e].txtExtnt[0]   = s[0];
		subMenus[n].elements[e].txtExtnt[1]   = s[1];
		subMenus[n].elements[e].type.checked  = 0;
		subMenus[n].elements[e].type.disabled = 0;
		subMenus[n].elements[e].type.imaged   = 0;
		subMenus[n].elements[e].type.parent   = 0;
		subMenus[n].elements[e].type.separator= 0;
		subMenus[n].elements[e].hotKey = 0;
		if(e<TotElems-1)
			CPTNS = va_arg(args, char*);
	}
	va_end (args);

	subMenus[n].staticWidth += SUBMENU_LEFT_SPACE;
	subMenus[n].staticHeight = TotElems * HEIGHT_OF_EACH_SUBMENUELEMENT;

	subMenus[n].hwnd = CreateWindowEx(	WS_EX_TOPMOST,//|WS_EX_STATICEDGE|WS_EX_CLIENTEDGE,
										"STATIC", "",WS_CHILD|WS_BORDER,//WS_OVERLAPPEDWINDOW,//|WS_DLGFRAME,
										barElems[n].x,
										MENU_BAR_HEIGHT-5,
										subMenus[n].staticWidth,
										subMenus[n].staticHeight,
										(HWND)hwnd, NULL, hInst, &n);
	SetWindowLong(subMenus[n].hwnd, GWL_WNDPROC, (DWORD) SubMenuWndProc); 
	ShowWindow(subMenus[n].hwnd, SW_SHOW);
	return true;
}

bool SubMenu::CloseAnimatedSubMenu(int i)
{
	return true;
}

bool SubMenu::OpenAnimatedSubMenu(int n)
{
	return true;
}

void SubMenu::SetChecked(int n,int e,bool b)
{
	subMenus[n].elements[e].type.checked = (b?1:1);
}

void SubMenu::SetDisabled(int n,int e,bool b)
{
	subMenus[n].elements[e].type.disabled = (b?1:1);
}

void SubMenu::SetImaged(int n,int e,bool b)
{
	subMenus[n].elements[e].type.imaged = (b?1:1);
}

void SubMenu::SetParented(int n,int e,bool b)
{
	subMenus[n].elements[e].type.parent = (b?1:1);
}

void SubMenu::SetSeparated(int n,int e,bool b)
{
	subMenus[n].elements[e].type.separator = (b?1:1);
}

//SubMenulistlarichiqishi:
LRESULT CALLBACK SubMenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HDC hdc;

	switch (message)
	{
	case WM_SHOWWINDOW://WM_CREATE:
		if(TRUE==wParam)
		{	hdc = GetDC(hWnd);
			int x; x = 12;
			for(int e=0; e<NUM_MENU_ELEMS; e++)
			{	barElems[e].CalcExtnt((long)hdc, x, 4);
				x += barElems[e].txtExtnt[0]+14;
			}
			ReleaseDC(hWnd, hdc);			
		}
		break;
	case WM_COMMAND:
		 return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PRINT:
	case WM_PRINTCLIENT:
		DrawMyMenuBar(hWnd,(HDC)wParam);
		break;
	case WM_PAINT:
		DrawMyMenuBar(hWnd,NULL);
		break;

	case WM_MOUSEMOVE:
		mouseXY = MAKEPOINTS(lParam);
		for(int e=0; e<NUM_MENU_ELEMS; e++)
		{	bool oldHover; oldHover = false;
			if(mouseXY.x>barElems[e].x)
			{if(mouseXY.x<barElems[e].x+barElems[e].txtExtnt[0])
			{if(mouseXY.y>barElems[e].y)
			{if(mouseXY.y<barElems[e].y+barElems[e].txtExtnt[1])
			{
				oldHover = true;
			}}}}
			if(oldHover != barElems[e].bMouHover)
			{	barElems[e].bMouHover = oldHover;
				barElems[e].iAnimTime = 0;
				if(barElems[e].bMouHover)
				{	if(barElems[e].state != BarElem::hilightToOn)
					if(barElems[e].state != BarElem::hilight)
					SetTimer(hWnd,IDT_HILIGHT_ON+e,ELEM_HILIGHT_TICK,NULL);
					KillTimer(hWnd,IDT_HILIGHT_OFF+e);
					barElems[e].state = BarElem::hilightToOn;
				}
				else
				{	if(barElems[e].state != BarElem::hilightToOff)
					if(barElems[e].state != BarElem::normal)
					SetTimer(hWnd,IDT_HILIGHT_OFF+e,ELEM_HILIGHT_TICK,NULL);
					KillTimer(hWnd,IDT_HILIGHT_ON+e);
					barElems[e].state = BarElem::hilightToOff;
		}	}	}
		break;
	case WM_TIMER:
		if(wParam>=IDT_HILIGHT_ON && wParam<=IDT_HILIGHT_ON+NUM_MENU_ELEMS)
		{	int e = wParam-IDT_HILIGHT_ON;
			barElems[e].iAnimTime += ELEM_HILIGHT_TICK;
			if(barElems[e].iAnimTime > ELEM_HILIGHT_ON_TIME)
			{	KillTimer(hWnd, wParam);
				barElems[e].state = BarElem::hilight;
			}
		} else if(wParam>=IDT_HILIGHT_OFF && wParam<=IDT_HILIGHT_OFF+NUM_MENU_ELEMS)
		{	int e = wParam-IDT_HILIGHT_OFF;
			barElems[e].iAnimTime += ELEM_HILIGHT_TICK;
			if(barElems[e].iAnimTime > ELEM_HILIGHT_OFF_TIME)
			{	KillTimer(hWnd, wParam);
				barElems[e].state = BarElem::normal;
		}	}
		RedrawWindow(hWnd,NULL,NULL,RDW_UPDATENOW|RDW_INVALIDATE);//DrawMyMenuBar(hWnd);
		break;
	case WM_LBUTTONDOWN:
		bool lftClck; lftClck = false;
		for(int e=0; e<NUM_MENU_ELEMS; e++)
		{	if(mouseXY.x>barElems[e].x)
			{if(mouseXY.x<barElems[e].x+barElems[e].txtExtnt[0])
			{if(mouseXY.y>barElems[e].y)
			{if(mouseXY.y<barElems[e].y+barElems[e].txtExtnt[1])
			{lftClck=true;BarElem::iOpened=e;break;
		}}}}}
		if(lftClck)//Open or close submenu item:
		{	BarElem::bOpened = !BarElem::bOpened;
			if(BarElem::bOpened) SubMenu::OpenAnimatedSubMenu(BarElem::iOpened);
			else SubMenu::CloseAnimatedSubMenu(BarElem::iOpened);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
