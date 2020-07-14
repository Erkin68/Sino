#pragma once

#include "windows.h"
#include "panel.h"
#include <stdio.h>
#include "Plugins\\Auxilary\\SinoSearchF7\\SinSchF7.h"

#define HEIGHT_OF_DISK_SPACE_IMAGE 16

#ifdef _WIN64
#define MyShellName L"MyShell64.dll"
#define MyShellNameLen 13
#else
#define MyShellName L"MyShell.dll"
#define MyShellNameLen 11
#endif

extern int  tick;
extern HWND hWnd;

namespace conf
{
extern  INT_PTR CALLBACK passwordDlgProc(HWND,UINT,WPARAM,LPARAM);
extern  INT_PTR CALLBACK subMenuAccessDlgProc(HWND,UINT,WPARAM,LPARAM);
extern	VOID	AddCopyDlgFiltrStr(wchar_t*);
extern	BOOL	ChangeTotPanelsNum(int);
extern  BOOL	CheckPrevWindIns();
extern  INT_PTR CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM);
extern	VOID	GetCopyDlgFiltrStr(HWND,UINT);
extern	bool	Read (wchar_t*);
//extern HBITMAP ReadWindImgFrDisk(char*,HDC*);
extern	bool	Write(wchar_t*);	
//extern bool	SaveWindImgToDisk(HWND,char*);
extern	int 	wndLeft;	//window left,
extern	int 	wndTop;		//window top,
extern	int 	wndWidth;	//window width,
extern	int 	wndHeight;	//window height.
extern	int 	scrWidth;	//desktop width, changes when SetDisplaySettings, trap wia wm_size;
extern	int 	scrHeight;	//desktop height, changes when SetDisplaySettings, trap wia wm_size;
extern	int		nonClientAreaWidth; //Window edge width, without client region;
extern	int		nonClientAreaHeight;//Window upper menu region height
extern	float	scrRatio;	//1024 * 768 = 1.3333; 1280 * 1024 = 1.25;
extern	RECT	wndRc;
extern	DWORD	MaxTextureWidth;
extern  DWORD	MaxTextureHeight;
extern	BOOL	bCancelAnimation[2];//0->in start,1-in fin;
extern  wchar_t tmpPath[MAX_PATH];

extern	DWORD   winVerMajor;
extern	DWORD   winVerMinor;
typedef	struct STBools
{	int	bAdmin				: 1;
	int bSSE				: 1;
} TBools;
typedef	struct STKey
{
public:
	STKey():bCtrl(0),bAlt(0),bShft(0),key(0){}
	STKey(int k,int c,int a,int s):key(k),bCtrl(c),bAlt(a),bShft(s){}
	BOOL operator==(STKey k){return (*((int*)&k))==(*((int*)this));}
	unsigned __int32 bCtrl	: 1;
	unsigned __int32 bAlt	: 1;
	unsigned __int32 bShft	: 1;
	unsigned __int32 key	: 29;
} TKey;
#define MAX_KEYS 58
#define TKEYDLGBGNNUM 51
#define TKEYDLGTAILNUM 109

typedef struct TCstmItemDef
{
public:
				TCstmItemDef();
	wchar_t     nameW[24];
	int			sortMthd;
	LOGFONT		fnt;
	HFONT		hFnt;
	HBRUSH      txtBkBrsh;//color[2] edtBrsh
	HBRUSH      bkBrsh;//color[0]
	HBRUSH      hotBkBrsh;//color[5]
	COLORREF    clrRGBs[6];
	SearchItem	itm;
} CstmItemDef;


class DLG
{
public:
	static VOID InitLOGFONT(LOGFONTW*,int);
	static VOID Init6RGB(COLORREF[6],int);
				DLG();
			   ~DLG();
	//VOID		ApplyToDialog();
	VOID		ApplyToSystem(int);
	BOOL		IsKeyExist(int);
	BOOL		IsKeyExist(STKey*);
	BOOL		Cmp(DLG*);
	VOID		CopyTo(DLG*);
	BOOL		freadFromChar(FILE*,char,char*,char*,void*,int);
	BOOL		freadFromChar(FILE*,wchar_t,wchar_t*,wchar_t*,void*,int);
	BOOL		freadRGBFromChar(FILE*,char,char*,COLORREF*);
	BOOL		freadRGBFromChar(FILE*,wchar_t,wchar_t*,COLORREF*);
	int		    GetCodeKey(int);
	wchar_t*	GetCodeName(int);
	COLORREF	GetColor(int,int);
	COLORREF*	GetColorPtr(int,int);
	int		    GetKeyCode(int);
	char*		GetKeyName(int);
	int			KeyToText(int,wchar_t*);
	BOOL		LoadFrom();
	BOOL		ReallocCustomItems(int,int,BOOL bInit=TRUE);
	VOID		SaveAs();
	VOID		SetColor(int,int,COLORREF);

	int		 iTotPanels;
	int		 iActivePanel;
	int		 iNumInstans;
	int		 iStartPosition;
	int		 iHideMethod;
	int		 iEnumMethod;
	int		 iIconSize[MAX_PANELS];
	int		 interval [MAX_PANELS];
	wchar_t  panBckgrndImgPth[MAX_PANELS][MAX_PATH];
	int		 panBckgrndImgType[MAX_PANELS];
	DWORD    panBckgrndImgAnimTime[MAX_PANELS];
	LOGFONTW panFnts[MAX_PANELS];
	LOGFONTW dlgFnts[MAX_DLG];
	LOGFONTW menuFnt;
	LOGFONTW srtBtnFnt[MAX_PANELS];
	LOGFONTW editFnt[MAX_PANELS];
	LOGFONTW cmdBtnFnt;
	COLORREF panRGBs[MAX_PANELS][6];
	COLORREF dlgRGBs[MAX_DLG][6];
	COLORREF menuRGB[6];
	COLORREF srtBtnRGB[MAX_PANELS][6];
	COLORREF editRGB[MAX_PANELS][6];
	COLORREF cmdBtnRGB[6];
	TKey	 keys[MAX_KEYS];
	int		 iBtnsType;

	//socket for direct linking:
	DWORD	 dwSockTimeout;
	DWORD    dwSockBufSize;
	int		 iCustPanItemDefs[MAX_PANELS];
	CstmItemDef* cstmItms[MAX_PANELS];

	//inner archiver:
	int crntArchvr;
	int totalAllovedArchvr;

	int crntSchplg;
	int totalAllovedSchPlg;

	int crntMnutplg;
	int totalAllovedMnutPlg;

	int crntQVplg;
	int totalAllovedQVPlg;

	//other bits:
	unsigned __int32 bUsePaswordForSubMenu : 1;
	unsigned __int32 bBtnsPanelType        : 2;//0-fixed,1-floating,2-not used
	unsigned __int32 bShowBtmPnl		   : 1;

	bool bShowDiskSpace[MAX_PANELS];
	wchar_t subMenuPassword[33];
};
extern DLG Dlg;

	//Panels informations:
extern	wchar_t	panel_path[MAX_PANELS][MAX_PATH];
extern	float	resizeBarPersnt[MAX_PANELS-1];
extern	int		folderAndFileMixingSortType[MAX_PANELS];
extern	int		folderSortType[MAX_PANELS];
extern	int		fileSortType[MAX_PANELS];
extern	int		iSheetTabs[MAX_PANELS];
extern	int		iCrntSheetTab[MAX_PANELS];
extern	int		iTabSheetRowCount[MAX_PANELS];
extern	wchar_t *sheet_path[MAX_PANELS];
extern	wchar_t *sheet_altPath[MAX_PANELS];
extern  U32		*sheet_viewMenuBitsU32[MAX_PANELS];

	//Uses in copy dlg:
extern	wchar_t	fileCopyDlgFiltrStrs[25][MAX_PATH/4];
extern	int		iCopyDlgBtnPlaces;
extern	int     bckgrndCpyDlgPozX,bckgrndCpyDlgPozY;
extern	BOOL	bCopyDlgChkBtn[2];
extern FILETIME strtTm;

//NetSock:
extern BYTE		IP4Addr[4];
extern DWORD	IP4_6_Port;

//Archive cmq level;
extern char		cmqLevPerCent;
extern wchar_t	modulePath[MAX_PATH];

//Alt+F7:
extern char    bEnumSubDir;
};