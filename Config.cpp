#include "Config.h"	
#include <stdio.h>
#include "shlobj.h"
#include "math.h"	
#include "Sino.h"	
#include "strsafe.h"	
#include "SheetTab.h"	
#include "FillManager.h"	
#include "MyCommonControls.h"
#include "WindowsManagmentInstrumentation.h"
#include "Operations\Language.h"
#include "Operations\MyErrors.h"
#include "Operations\QView.h"
#include "Operations\VirtualPanel.h"
#include "Operations\MenuUtils.h"	
#include "Operations\CreateFolder.h"	
#include "Operations\MyShell\MyShell.h"
//#include "Operations\MyShell\MyButtonC++.h"
#include "Operations\Backgrnd thread copy operation.h"	
#include "Operations\BtnBar.h"	



extern HWND	btns[7];//extern MyButton btns[7];
extern HFONT	hfCmdBtns;
extern HBRUSH	brCmdBtns;
extern HBRUSH	brHtBk;


namespace conf
{
#define MAX_FONT_INTERVAL  200
#define MIN_FONT_INTERVAL -200
#define TOT_CHILD_DLGS 118
#define REAL_TOT_CHILD_DLGS 8
HWND stdhDlg[REAL_TOT_CHILD_DLGS]={0};

int 	scrWidth=GetSystemMetrics(SM_CXSCREEN);//desktop width, changes when SetDisplaySettings, trap wia wm_size;
int 	scrHeight=GetSystemMetrics(SM_CYSCREEN);//desktop height, changes when SetDisplaySettings, trap wia wm_size;
int 	wndWidth=scrWidth>940?940:2*scrWidth/3;	//window width,
int 	wndHeight=scrHeight>780?780:2*scrHeight/3;//window height,
int 	wndLeft=(scrWidth - wndWidth)/2;//window left,
int 	wndTop=(scrHeight - wndHeight)/2;//window top,
int		nonClientAreaWidth; //Window edge width, without client region;
int		nonClientAreaHeight;//Window upper menu region height
float	scrRatio=(float)scrWidth / scrHeight;//1024 * 768 = 1.3333; 1280 * 1024 = 1.25;
RECT	wndRc={wndLeft,wndTop,wndLeft + wndWidth,wndTop + wndHeight};
DWORD	MaxTextureWidth;
DWORD	MaxTextureHeight;
BOOL	bCancelAnimation[2];//0->in start,1-in fin;
wchar_t modulePath[MAX_PATH];

DWORD   winVerMajor=4;
DWORD   winVerMinor=0;
TBools  Bools={0,0};
HWND	hTmpDlg=0;

//NetSock:
BYTE	IP4Addr[4]={128,108,100,2};
DWORD	IP4_6_Port=5001;

//Panels informations:
wchar_t	panel_path[MAX_PANELS][MAX_PATH]={L"",L"",L"",L""};
float	resizeBarPersnt[MAX_PANELS-1]={50.0f};//33.33 66.66    25 50 75
int		folderAndFileMixingSortType[MAX_PANELS]={0,0,0,0};
int		folderSortType[MAX_PANELS]={0,0,0,0};
int		fileSortType[MAX_PANELS]={7,7,7,7};//alphabeticalUp
int		iSheetTabs[MAX_PANELS]={0,0,0,0};
int		iCrntSheetTab[MAX_PANELS];
int		iTabSheetRowCount[MAX_PANELS];
U32		viewMenuBitsU32[MAX_PANELS];

wchar_t *sheet_path[MAX_PANELS]={L"",L"",L"",L""};
wchar_t *sheet_altPath[MAX_PANELS]={L"",L"",L"",L""};
U32		*sheet_viewMenuBitsU32[MAX_PANELS]={0,0,0,0};
//char*	sheet_archPath[MAX_PANELS]={"","","",""};
//int*	sheet_archPlgNum[MAX_PANELS]={0,0,0,0};


//Uses in copy dlg:
wchar_t	fileCopyDlgFiltrStrs[25][MAX_PATH/4]={L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L"",L""};
int		iCopyDlgBtnPlaces=0;
int     bckgrndCpyDlgPozX=0,bckgrndCpyDlgPozY=0;
BOOL	bCopyDlgChkBtn[2]={FALSE,FALSE};
FILETIME strtTm;

//Archive add dialog:
char    cmqLevPerCent=75;
wchar_t tmpPath[MAX_PATH]=L"d:\\temp";

//Alt+F7:
char    bEnumSubDir=1;

class Chld
{
public:
	static int		pos[2];
	static int		sz[2];
	static int		iSelectedDlg;
//	static int		resIds[TOT_CHILD_DLGS];
//	static DLGPROC	chldDlgs[TOT_CHILD_DLGS];

	static VOID		Change(int);
	static VOID		Create(HWND);
	static VOID		Destroy();
	static LOGFONT*	GetColorFont();

	int			iWnd;
	LOGFONT		oldFont;
	COLORREF	oldColor[6];

	VOID		SetFontFont(HFONT);
	VOID		SetDestColor(int,COLORREF*);
	VOID		SetDestFont(HFONT,LOGFONT*);
};
Chld chlds[TOT_CHILD_DLGS];
DLG Dlg,DlgRestore;

struct
{	HTREEITEM	cmn;
	HTREEITEM	fntNode;
	HTREEITEM	fntPanel;
	HTREEITEM	fntPanelChlds[MAX_PANELS];
	HTREEITEM	fntDialogs;
	HTREEITEM	fntDialogChlds[9];
	HTREEITEM	fntSrtBtns;
	HTREEITEM	fntSrtBtnChlds[MAX_PANELS];
	HTREEITEM	fntEdtCntrl;
	HTREEITEM	fntEdtCntrlChlds[MAX_PANELS];
	HTREEITEM	fntCmndBtns;
	HTREEITEM	clrNode;
	HTREEITEM	clrPanel;
	HTREEITEM	clrPanelChlds[MAX_PANELS];
	HTREEITEM	clrDialogs;
	HTREEITEM	clrDialogChlds[9];
	HTREEITEM	clrSrtBtns;
	HTREEITEM	clrSrtBtnChlds[MAX_PANELS];
	HTREEITEM	clrEdtCntrl;
	HTREEITEM	clrEdtCntrlChlds[MAX_PANELS];
	HTREEITEM	clrCmndBtns;
	HTREEITEM	iconNode;
	HTREEITEM	iconPanel;
	HTREEITEM	iconPanelChlds[MAX_PANELS];
	HTREEITEM	hotKeys;
	HTREEITEM	hotKeyChlds[MAX_KEYS];
	HTREEITEM	custPanItems;
	HTREEITEM	custPanItemChlds[MAX_PANELS];
	HTREEITEM	plgns;
	HTREEITEM	plgnChlds[3];//archive, search, menu utils
} hTree;

TCstmItemDef::TCstmItemDef():sortMthd(0)
{
	nameW[0]=0;
	DLG::InitLOGFONT(&fnt,1);
	clrRGBs[0] = 0x00ebe0eb;//Back
	clrRGBs[1] = 0x008a0000;//Front
	clrRGBs[2] = 0x00ece1ec;//Select back
	clrRGBs[3] = 0x000000ff;//Select
	clrRGBs[4] = 0x00ff0000;//Hot
	clrRGBs[5] = 0x00eeaed7;//Hot back
}

//Har qanday prolni(lParamda to'g'ri parolning xeshi) MD5
//xesh qilib tekshirish,to'g'ri kiritdimi, yoki noto'g'ri;
INT_PTR CALLBACK passwordDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
UNREFERENCED_PARAMETER(lParam);
static wchar_t chkPasswordHash[MAX_PATH];
wchar_t st[MAX_PATH];
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; GetWindowRect(hDlg, &rc);
		int width,left,height,top;
		width = rc.right - rc.left;
		left = conf::wndLeft + (conf::wndWidth - width)/2;
		height = rc.bottom - rc.top;
		top = conf::wndTop + (conf::wndHeight - height)/2;
		MoveWindow(hDlg, left, top, width, height, TRUE);
		LoadString(hInst,IDS_STRINGSW_320,st,MAX_PATH);
		SetWindowText(hDlg,st);
		LoadString(hInst,IDS_STRINGSW_41,st,MAX_PATH);
		SetDlgItemText(hDlg,IDOK,st);
		LoadString(hInst,IDS_STRINGSW_13,st,MAX_PATH);
		SetDlgItemText(hDlg,IDCANCEL,st);
		MyStringCpy(chkPasswordHash,MAX_PATH,(wchar_t*)lParam);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDCANCEL:
				EndDialog(hDlg,0);
				return (INT_PTR)TRUE;
			case IDOK:
				GetDlgItemText(hDlg,IDC_EDIT_DATA1,st,33);				
				if(wcscmp(chkPasswordHash,calcMD5_MAX_PATH_string(st)))
				{	MessageBox(hDlg,L"Wrong pasword!!!",L"Error...",MB_OK);
					return 0;
				}
				//else
				EndDialog(hDlg,1);
				return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK subMenuAccessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//IDD_DIALOG_LOCK_VIA_PASSWORD
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; GetWindowRect(hDlg, &rc);
		int width,left,height,top;
		width = rc.right - rc.left;
		left = conf::wndLeft + (conf::wndWidth - width)/2;
		height = rc.bottom - rc.top;
		top = conf::wndTop + (conf::wndHeight - height)/2;
		MoveWindow(hDlg, left, top, width, height, TRUE);

		wchar_t st[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_319,st,MAX_PATH);
		SetWindowText(hDlg,st);
		LoadString(hInst,IDS_STRINGSW_41,st,MAX_PATH);
		SetDlgItemText(hDlg,IDOK,st);
		LoadString(hInst,IDS_STRINGSW_320,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK1 ,st);
		if(0==conf::Dlg.bUsePaswordForSubMenu)
		{	EnableWindow(GetDlgItem(hDlg,IDC_EDIT_PASSWORD),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_EDIT_PASSWORD2),FALSE);
		}SendMessage(GetDlgItem(hDlg,IDC_CHECK1),BM_SETCHECK,0==conf::Dlg.bUsePaswordForSubMenu?BST_UNCHECKED:BST_CHECKED,0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDC_CHECK1:
				conf::Dlg.bUsePaswordForSubMenu = 0;
				if(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK1),BM_GETCHECK,0,0))
					conf::Dlg.bUsePaswordForSubMenu = 1;
				if(1==conf::Dlg.bUsePaswordForSubMenu)
				{	EnableWindow(GetDlgItem(hDlg,IDC_EDIT_PASSWORD),TRUE);
					EnableWindow(GetDlgItem(hDlg,IDC_EDIT_PASSWORD2),TRUE);
				}
				else
				{	EnableWindow(GetDlgItem(hDlg,IDC_EDIT_PASSWORD),FALSE);
					EnableWindow(GetDlgItem(hDlg,IDC_EDIT_PASSWORD2),FALSE);
				}
				return 0;
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			case IDOK:
				if(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK1),BM_GETCHECK,0,0))
				{	wchar_t s1[33],s2[33];
					int l1=GetDlgItemText(hDlg,IDC_EDIT_PASSWORD,s1,33);
					int l2=GetDlgItemText(hDlg,IDC_EDIT_PASSWORD,s2,33);
					if(l1!=l2 || wcscmp(s1,s2))
					{	MessageBox(hDlg,L"pasword strings are not identically",L"Error...",MB_OK);
						conf::Dlg.bUsePaswordForSubMenu=0;
						SendMessage(GetDlgItem(hDlg,IDC_CHECK1),BM_SETCHECK,BST_UNCHECKED,0);
						return 0;
					}
					MyStringCpy(Dlg.subMenuPassword,32,calcMD5_MAX_PATH_string(s1));
				}
				//else
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
		}
		break;
	case WM_DESTROY:
		language::ChangeMenuBitmap(::hWnd,2,3,conf::Dlg.bUsePaswordForSubMenu==1?1:2);
		break;
	}
	return (INT_PTR)FALSE;
}

DLG::DLG():	iTotPanels(2),iActivePanel(0),iNumInstans(1),iStartPosition(0),
			iHideMethod(0),iEnumMethod(1),dwSockTimeout(35000),dwSockBufSize(524288),
			crntArchvr(-1),crntSchplg(-1),crntMnutplg(-1),
			totalAllovedArchvr(40),totalAllovedSchPlg(10),totalAllovedMnutPlg(40),
			crntQVplg(-1),totalAllovedQVPlg(40),
			iBtnsType(0),//other bits:
			bUsePaswordForSubMenu(0),bBtnsPanelType(0),bShowBtmPnl(1)//0-fixed,1-floating,2-not used
{
	for(int p=0; p<MAX_PANELS; p++)
	{	iIconSize[p]=16;
		interval[p]=7;
		InitLOGFONT(&panFnts[p],0);
		Init6RGB(panRGBs[p],0);
		panBckgrndImgPth[p][0]=0;
		panBckgrndImgType[p]=2;
		panBckgrndImgAnimTime[p]=0;
		bShowDiskSpace[p]=1;
	}
	for(int d=0; d<MAX_DLG; d++)
	{	InitLOGFONT(&dlgFnts[d],1);
		Init6RGB(dlgRGBs[d],1);
	}
	InitLOGFONT(&menuFnt,2);
	for(int p=0; p<MAX_PANELS; p++)
	{	InitLOGFONT(&srtBtnFnt[p],2);
		InitLOGFONT(&editFnt[p],2);
	}
	InitLOGFONT(&cmdBtnFnt,2);
	Init6RGB(menuRGB,2);

	for(int p=0; p<MAX_PANELS; p++)
	{	Init6RGB(srtBtnRGB[p],3);
		Init6RGB(editRGB[p],4);
	}

	Init6RGB(cmdBtnRGB,5);

	keys[0].bCtrl=1;keys[0].bAlt=keys[0].bShft=0;keys[0].key = 65;//A
	keys[1].bCtrl=1;keys[1].bAlt=keys[1].bShft=0;keys[1].key = 84;//T
	keys[2].bCtrl=1;keys[2].bAlt=keys[2].bShft=0;keys[2].key = 87;//W
	keys[3].bCtrl=0;keys[3].bAlt=keys[3].bShft=0;keys[3].key = 112;//F1
	keys[4].bCtrl=1;keys[4].bAlt=keys[4].bShft=0;keys[4].key = 112;//F1
	keys[5].bCtrl=1;keys[5].bAlt=keys[5].bShft=0;keys[5].key = 113;//F2
	keys[6].bCtrl=1;keys[6].bAlt=keys[6].bShft=0;keys[6].key = 114;//F3
	keys[7].bCtrl=1;keys[7].bAlt=keys[7].bShft=0;keys[7].key = 115;//F4
	keys[8].bCtrl=0;keys[8].bAlt=keys[8].bShft=0;keys[8].key = 113;//F2
	keys[9].bCtrl=0;keys[9].bAlt=keys[9].bShft=0;keys[9].key = 114;//F3 View
	keys[10].bCtrl=0;keys[10].bAlt=keys[10].bShft=0;keys[10].key=115;//F4
	keys[11].bCtrl=0;keys[11].bAlt=keys[11].bShft=0;keys[11].key=116;//F5 Copy
	keys[12].bCtrl=0;keys[12].bAlt=keys[12].bShft=0;keys[12].key=117;//F6 Ren-move
	keys[13].bCtrl=0;keys[13].bAlt=1;keys[13].bShft=0;keys[13].key=118;//F7 Search
	keys[14].bCtrl=0;keys[14].bAlt=0;keys[14].bShft=0;keys[14].key=118;//F7 Create folder
	keys[15].bCtrl=0;keys[15].bAlt=0;keys[15].bShft=0;keys[15].key=8;//Back
	keys[16].bCtrl=0;keys[16].bAlt=0;keys[16].bShft=0;keys[16].key=107;//Numpad +
	keys[17].bCtrl=0;keys[17].bAlt=0;keys[17].bShft=0;keys[17].key=32;//Space
	keys[18].bCtrl=0;keys[18].bAlt=0;keys[18].bShft=0;keys[18].key=45;//Insert
	keys[19].bCtrl=0;keys[19].bAlt=0;keys[19].bShft=0;keys[19].key=33;//PgUp
	keys[20].bCtrl=0;keys[20].bAlt=0;keys[20].bShft=0;keys[20].key=34;//PgDwn
	keys[21].bCtrl=0;keys[21].bAlt=0;keys[21].bShft=0;keys[21].key=35;//End  
	keys[22].bCtrl=0;keys[22].bAlt=0;keys[22].bShft=0;keys[22].key=36;//Home
	keys[23].bCtrl=0;keys[23].bAlt=0;keys[23].bShft=0;keys[23].key=38;//Up
	keys[24].bCtrl=0;keys[24].bAlt=0;keys[24].bShft=0;keys[24].key=40;//Dwn
	keys[25].bCtrl=0;keys[25].bAlt=0;keys[25].bShft=0;keys[25].key=27;//Esc
	keys[26].bCtrl=0;keys[26].bAlt=1;keys[26].bShft=0;keys[26].key=115;//Exit Alt+F4
	keys[27].bCtrl=0;keys[27].bAlt=0;keys[27].bShft=0;keys[27].key=37;//<- 2-PgUp
	keys[28].bCtrl=0;keys[28].bAlt=0;keys[28].bShft=0;keys[28].key=39;//-> 2-PgDwn
	keys[29].bCtrl=0;keys[29].bAlt=1;keys[29].bShft=0;keys[29].key=13;//Properties
	keys[30].bCtrl=0;keys[30].bAlt=0;keys[30].bShft=0;keys[30].key=13;//Enter
	keys[31].bCtrl=0;keys[31].bAlt=0;keys[31].bShft=1;keys[31].key=33;//Select Pg Up
	keys[32].bCtrl=0;keys[32].bAlt=0;keys[32].bShft=1;keys[32].key=34;//Select PgDwn
	keys[33].bCtrl=0;keys[33].bAlt=0;keys[33].bShft=1;keys[33].key=37;//Select Pg Up,2-key
	keys[34].bCtrl=0;keys[34].bAlt=0;keys[34].bShft=1;keys[34].key=39;//Select Pg Up,2-key
	keys[35].bCtrl=0;keys[35].bAlt=0;keys[35].bShft=1;keys[35].key=35;//Select to End  
	keys[36].bCtrl=0;keys[36].bAlt=0;keys[36].bShft=1;keys[36].key=36;//Select to Home
	keys[37].bCtrl=0;keys[37].bAlt=0;keys[37].bShft=1;keys[37].key=38;//Select,Up
	keys[38].bCtrl=0;keys[38].bAlt=0;keys[38].bShft=1;keys[38].key=40;//Select,Dwn
	keys[39].bCtrl=0;keys[39].bAlt=0;keys[39].bShft=0;keys[39].key=46;//Delete
	keys[40].bCtrl=0;keys[40].bAlt=0;keys[40].bShft=0;keys[40].key=119;//Delete,2-key
	keys[41].bCtrl=0;keys[41].bAlt=0;keys[41].bShft=1;keys[41].key=46;//Unrestorable delete
	keys[42].bCtrl=0;keys[42].bAlt=0;keys[42].bShft=1;keys[42].key=119;//Unrestorable delete,2-key
	keys[43].bCtrl=1;keys[43].bAlt=0;keys[43].bShft=0;keys[43].key=13;//Insert to command line
	keys[44].bCtrl=0;keys[44].bAlt=1;keys[44].bShft=0;keys[44].key=116;//Add to archive
	keys[45].bCtrl=1;keys[45].bAlt=0;keys[45].bShft=0;keys[45].key=34;//Add to archive

	keys[46].bCtrl=0;keys[46].bAlt=1;keys[46].bShft=0;keys[46].key=0x72;//View with...(from plugins)
	keys[47].bCtrl=0;keys[47].bAlt=0;keys[47].bShft=1;keys[47].key=0x72;//View with...(open)
	keys[48].bCtrl=1;keys[48].bAlt=1;keys[48].bShft=0;keys[48].key=0x72;//View with...(browse) F3
	keys[49].bCtrl=0;keys[49].bAlt=1;keys[49].bShft=0;keys[49].key=0x73;//Edit with...(from plugins)
	keys[50].bCtrl=0;keys[50].bAlt=0;keys[50].bShft=1;keys[50].key=0x73;//Edit with...(open)F4
	keys[51].bCtrl=1;keys[51].bAlt=1;keys[51].bShft=0;keys[51].key=0x73;//Edit with...(browse)

	keys[52].bCtrl=1;keys[52].bAlt=0;keys[52].bShft=0;keys[52].key=0x43;//Copy to clpbrd C
	keys[53].bCtrl=1;keys[53].bAlt=0;keys[53].bShft=0;keys[53].key=0x58;//Cut to clpbrd X
	keys[54].bCtrl=1;keys[54].bAlt=0;keys[54].bShft=0;keys[54].key=0x56;//Paste from clpbrd V
	keys[55].bCtrl=1;keys[55].bAlt=1;keys[55].bShft=0;keys[55].key=0x43;//Append copy
	keys[56].bCtrl=1;keys[56].bAlt=1;keys[56].bShft=0;keys[56].key=0x58;//Append cut
	keys[57].bCtrl=1;keys[57].bAlt=0;keys[57].bShft=0;keys[57].key=0x51;//qView Ctrl + Q

	subMenuPassword[0]=0;
	for(int p=0; p<MAX_PANELS; p++)
	{	iCustPanItemDefs[p]=0;
		cstmItms[p]=NULL;
}	}

DLG::~DLG()
{
	for(int p=0; p<MAX_PANELS; p++)
	{	for(int j=0; j<iCustPanItemDefs[p]; j++)
		{	DeleteObject(cstmItms[p][j].hFnt);
			DeleteObject(cstmItms[p][j].txtBkBrsh);
			DeleteObject(cstmItms[p][j].bkBrsh);
			DeleteObject(cstmItms[p][j].hotBkBrsh);
		}
		if(iCustPanItemDefs[p])
			free(cstmItms[p]);
		cstmItms[p] = NULL;
		iCustPanItemDefs[p]=0;
}	}

/*VOID DLG::ApplyToDialog()
{
	SendMessage(GetDlgItem(::chlds[0].hWnd,IDC_COMBO_TOT_PANELS_CONFIG),
				CB_SETCURSEL,
				iTotPanels,
				0);
	SendMessage(GetDlgItem(::chlds[0].hWnd,IDC_COMBO_TOT_INSTANCE_CONFIG),
				CB_SETCURSEL,
				iNumInstans,
				0);
	SendMessage(GetDlgItem(::chlds[0].hWnd,IDC_COMBO_START_POS_CONFIG),
				CB_SETCURSEL,
				iNumInstans,
				0);
	SendMessage(GetDlgItem(::chlds[0].hWnd,IDC_COMBO_HIDE_METHOD_CONFIG),
				CB_SETCURSEL,
				iNumInstans,
				0);
	/*for(int p=0; p<MAX_PANELS; p++)
	{	to->iIconSize[p]=iIconSize[p];
		to->interval[p]=interval[p];
		CopyMemory(&to->panFnts[p],&panFnts[p],sizeof(LOGFONT));
		for(int c=0; c<6; c++)
			to->panRGBs[p][c] = panRGBs[p][c];
	}
	for(int d=0; d<9; d++)
	{	CopyMemory(&to->dlgFnts[d],&dlgFnts[d],sizeof(LOGFONT));
		for(int c=0; c<6; c++)
			to->dlgRGBs[d][c] = dlgRGBs[d][c];
	}
	CopyMemory(&to->menuFnt,&menuFnt,sizeof(LOGFONT));
	CopyMemory(&to->editFnt,&editFnt,sizeof(LOGFONT));
	CopyMemory(&cmdBtnFnt,&cmdBtnFnt,sizeof(LOGFONT));
	for(int c=0; c<6; c++)
		to->menuRGB[c] = menuRGB[c];
	for(int c=0; c<6; c++)
		to->editRGB[c] = editRGB[c];
	for(int c=0; c<6; c++)
		to->cmdBtnRGB[c] = cmdBtnRGB[c];
}*/

VOID DLG::ApplyToSystem(int iOldTotPans)
{
	/*to->iTotPanels=iTotPanels;
	to->iNumInstans=iNumInstans;
	to->iStartPosition=iStartPosition;
	to->iHideMethod=iHideMethod;
	for(int p=0; p<MAX_PANELS; p++)
	{	to->iIconSize[p]=iIconSize[p];
		to->interval[p]=interval[p];
		CopyMemory(&to->panFnts[p],&panFnts[p],sizeof(LOGFONT));
		for(int c=0; c<6; c++)
			to->panRGBs[p][c] = panRGBs[p][c];
	}
	for(int d=0; d<9; d++)
	{	CopyMemory(&to->dlgFnts[d],&dlgFnts[d],sizeof(LOGFONT));
		for(int c=0; c<6; c++)
			to->dlgRGBs[d][c] = dlgRGBs[d][c];
	}
	CopyMemory(&to->menuFnt,&menuFnt,sizeof(LOGFONT));
	CopyMemory(&to->editFnt,&editFnt,sizeof(LOGFONT));
	CopyMemory(&cmdBtnFnt,&cmdBtnFnt,sizeof(LOGFONT));

	for(int c=0; c<6; c++)
		to->menuRGB[c] = menuRGB[c];
	for(int c=0; c<6; c++)
		to->editRGB[c] = editRGB[c];
	for(int c=0; c<6; c++)
		to->cmdBtnRGB[c] = cmdBtnRGB[c];*/
	HDC dc = GetDC(NULL);
	for(int i=0; i<MAX_PANELS; i++)
	{	int fntHeight=-MulDiv(Dlg.panFnts[i].lfHeight,72,GetDeviceCaps(dc,LOGPIXELSY));
		panel[i].SetItemHeight(fntHeight+Dlg.interval[i]);
		panel[i].ChangeFont(&Dlg.panFnts[i],0);//changefontni o'zi chizadu;
		//if(i<conf::Dlg.iTotPanels)
		//	panel[i].Render();
	}
	ReleaseDC(NULL,dc);
	if(iOldTotPans!=iTotPanels)
	{	conf::ChangeTotPanelsNum(iTotPanels);
		CmnCntrl::DestroyControls(::hWnd);
		CmnCntrl::CreateControls(::hWnd,TRUE);
		for(int i=0; i<Dlg.iTotPanels; i++)
		{	panel[i].folderAndFileMixingSortType=(FolderAndFileMixingSortType)folderAndFileMixingSortType[i];
			panel[i].folderSortType=(SortType)folderSortType[i];
			panel[i].fileSortType=(SortType)fileSortType[i];
			if(!IsDirExist(panel_path[i]))
				MyStringCpy(panel_path[i],MAX_PATH-1,MyStringAddModulePath(L"*"));
}	}	}

BOOL DLG::IsKeyExist(int k)
{
BOOL r=FALSE;
	for(int i=0; i<MAX_KEYS; i++)
	{	if(i==k)continue;
		if(keys[i].key==keys[k].key)
		if(keys[i].bCtrl==keys[k].bCtrl)
		if(keys[i].bAlt==keys[k].bAlt)
		if(keys[i].bShft==keys[k].bShft)
			return TRUE;
	}
	return FALSE;
}

BOOL DLG::IsKeyExist(STKey *k)
{
BOOL r=FALSE;
	for(int i=0; i<MAX_KEYS; i++)
	{	if(keys[i].key==k->key)
		if(keys[i].bCtrl==k->bCtrl)
		if(keys[i].bAlt==k->bAlt)
		if(keys[i].bShft==k->bShft)
			return TRUE;
	}
	if(88==k->key && k->bCtrl && (!k->bAlt) && (!k->bShft)) return FALSE;//IDM_EDIT_CUT:
	if(67==k->key && k->bCtrl && (!k->bAlt) && (!k->bShft)) return FALSE;//IDM_EDIT_COPY:
	if(86==k->key && k->bCtrl && (!k->bAlt) && (!k->bShft)) return FALSE;//IDM_EDIT_PAST:
	if(46==k->key && (!k->bCtrl) && (!k->bAlt) && (!k->bShft)) return FALSE;//IDM_EDIT_DELETE:
	if(65==k->key && k->bCtrl && (!k->bAlt) && (!k->bShft)) return FALSE;//IDM_EDIT_SELECTALL:

	return FALSE;
}

BOOL DLG::Cmp(DLG* with)
{	
int sz = sizeof(DLG);
	BYTE* pthis = (BYTE*)this;
	BYTE* pwith = (BYTE*)with;
	for(int b=0; b<sz; b++)
	{	if(*(pthis+b) != *(pwith+b))
			return FALSE;
	}
	return TRUE;
}

VOID DLG::CopyTo(DLG* to)
{
	to->iTotPanels=iTotPanels;
	to->iNumInstans=iNumInstans;
	to->iStartPosition=iStartPosition;
	to->iHideMethod=iHideMethod;
	to->iEnumMethod=iEnumMethod;
	to->dwSockBufSize=dwSockBufSize;
	to->dwSockTimeout=dwSockTimeout;
	for(int p=0; p<MAX_PANELS; p++)
	{	to->iIconSize[p]=iIconSize[p];
		to->interval[p]=interval[p];
		CopyMemory(&to->panFnts[p],&panFnts[p],sizeof(LOGFONT));
		for(int c=0; c<6; c++)
			to->panRGBs[p][c] = panRGBs[p][c];
	}
	for(int d=0; d<MAX_DLG; d++)
	{	CopyMemory(&to->dlgFnts[d],&dlgFnts[d],sizeof(LOGFONT));
		for(int c=0; c<6; c++)
			to->dlgRGBs[d][c] = dlgRGBs[d][c];
	}
	CopyMemory(&to->menuFnt,&menuFnt,sizeof(LOGFONT));
	for(int p=0; p<MAX_PANELS; p++)
	{	CopyMemory(&to->srtBtnFnt[p],&srtBtnFnt[p],sizeof(LOGFONT));	
		CopyMemory(&to->editFnt[p],&editFnt[p],sizeof(LOGFONT));
	}
	CopyMemory(&cmdBtnFnt,&cmdBtnFnt,sizeof(LOGFONT));
	for(int c=0; c<6; c++)
		to->menuRGB[c] = menuRGB[c];
	for(int p=0; p<MAX_PANELS; p++)
	{	for(int c=0; c<6; c++)
		{	to->srtBtnRGB[p][c] = srtBtnRGB[p][c];
			to->editRGB[p][c] = editRGB[p][c];
	}	}
	for(int c=0; c<6; c++)
		to->cmdBtnRGB[c] = cmdBtnRGB[c];
	to->iBtnsType = iBtnsType;		
}

// char c = bo'lsa, o'shangacha o'qib, undan keyingisini int & ga ko'chir:
BOOL DLG::freadFromChar(FILE* f,char c,char* chckStr,char* fmt,void* iOut,int szOut)
{
/*char r[8]="";
	do 
	{	if(1!=fread(r,1,1,f))
			return FALSE;
	}
	while(c!=r[0]);
	int R=fscanf_s(f,"%d",iOut);
	if(0==R)
		return FALSE;
	if(EOF==R)
		return FALSE;
	return TRUE;  Check version:*/
char r[MAX_PATH]="";int i=0;
	do 
	{	if(1!=fread(&r[i],1,1,f))
			return FALSE;
		if(0x0a==r[i])//Vozvrat karetki;
			--i;
	}
	while(c!=r[i++]);
	r[i]=0;
	if(strcmp(r,chckStr))
	{	Err::msg2A(hWnd,-1,"Err. when reading from file for checking string:",chckStr,r);
		return FALSE;
	}

	__int32 dOut,*pdOut=&dOut; int R=1;
	if(!strcmp(fmt,"%d"))
	{	R=fscanf_s(f,fmt,&dOut);
		switch(szOut)
		{	case 1://char or BYTE
				*((BYTE*)iOut) = *((BYTE*)pdOut);
				break;
			case 2://int16
				*((unsigned __int16*)iOut) = *((unsigned __int16*)pdOut);
				break;
			case 4://int32 or long
				*((unsigned __int32*)iOut) = *((unsigned __int32*)pdOut);
				break;
	}	}
	else if(!strcmp(fmt,"%s"))
	{	//R=fscanf_s(f,fmt,iOut);
		if(1!=fread(&r[0],1,1,f))
			return FALSE;
		if('"'!=r[0])//Vozvrat karetki;
			return FALSE;
		char *piOut = (char*)iOut;
		do 
		{	if(1!=fread(piOut,1,1,f))
				return FALSE;
		}
		while('"'!=*piOut++);
		*(piOut-1)=0;
	}
	if(0==R)
		return FALSE;
	if(EOF==R)
		return FALSE;
	return TRUE;
}

BOOL DLG::freadFromChar(FILE* f,wchar_t c,wchar_t* chckStr,wchar_t* fmt,void* iOut,int szOut)
{
/*char r[8]="";
	do 
	{	if(1!=fread(r,1,1,f))
			return FALSE;
	}
	while(c!=r[0]);
	int R=fscanf_s(f,"%d",iOut);
	if(0==R)
		return FALSE;
	if(EOF==R)
		return FALSE;
	return TRUE;  Check version:*/
wchar_t r[MAX_PATH]=L"";int i=0;
	do 
	{	if(1!=fread(&r[i],1,2,f))
			return FALSE;
		if(0x0a==r[i])//Vozvrat karetki;
			--i;
	}
	while(c!=r[i++]);
	r[i]=0;
	if(wcscmp(r,chckStr))
	{	Err::msg2(hWnd,-1,L"Err. when reading from file for checking string:",chckStr,r);
		return FALSE;
	}

	__int32 dOut,*pdOut=&dOut; int R=1;
	if(!wcscmp(fmt,L"%d"))
	{	R=fwscanf_s(f,fmt,&dOut);
		switch(szOut)
		{	case 1://char or BYTE
				*((BYTE*)iOut) = *((BYTE*)pdOut);
				break;
			case 2://int16
				*((unsigned __int16*)iOut) = *((unsigned __int16*)pdOut);
				break;
			case 4://int32 or long
				*((unsigned __int32*)iOut) = *((unsigned __int32*)pdOut);
				break;
	}	}
	else if(!wcscmp(fmt,L"%s"))
	{	//R=fscanf_s(f,fmt,iOut);
		if(1!=fread(&r[0],1,1,f))
			return FALSE;
		if('"'!=r[0])//Vozvrat karetki;
			return FALSE;
		wchar_t *piOut = (wchar_t*)iOut;
		do 
		{	if(1!=fread(piOut,1,1,f))
				return FALSE;
		}
		while('"'!=*piOut++);
		*(piOut-1)=0;
	}
	if(0==R)
		return FALSE;
	if(EOF==R)
		return FALSE;
	return TRUE;
}

BOOL DLG::freadRGBFromChar(FILE* f,char c,char* chckStr,COLORREF* iOut)
{
char r[MAX_PATH]="";int i=0;
	do 
	{	if(1!=fread(&r[i],1,1,f))
			return FALSE;
		if(0x0a==r[i])//Vozvrat karetki;
			--i;
	}
	while(c!=r[i++]);
	r[i]=0;
	if(strcmp(r,chckStr))
	{	Err::msg2A(hWnd,-1,"Err. when reading from file for checking string:",chckStr,r);
		return FALSE;
	}

	int bOut[3]; int R=1;
	for(int b=0; b<3; b++)
	{	R=fscanf_s(f,"%d",&bOut[b]);
		if(0==R)
			return FALSE;
		if(EOF==R)
			return FALSE;
	}
	*iOut = RGB(bOut[0],bOut[1],bOut[2]);
	return TRUE;
}

BOOL DLG::freadRGBFromChar(FILE* f,wchar_t c,wchar_t* chckStr,COLORREF* iOut)
{
wchar_t r[MAX_PATH]=L"";int i=0;
	do 
	{	if(1!=fread(&r[i],1,2,f))
			return FALSE;
		if(0x0a==r[i])//Vozvrat karetki;
			--i;
	}
	while(c!=r[i++]);
	r[i]=0;
	if(wcscmp(r,chckStr))
	{	Err::msg2(hWnd,-1,L"Err. when reading from file for checking string:",chckStr,r);
		return FALSE;
	}

	int bOut[3]; int R=1;
	for(int b=0; b<3; b++)
	{	R=fwscanf_s(f,L"%d",&bOut[b]);
		if(0==R)
			return FALSE;
		if(EOF==R)
			return FALSE;
	}
	*iOut = RGB(bOut[0],bOut[1],bOut[2]);
	return TRUE;
}

int DLG::GetCodeKey(int k)
{
	switch(k)
	{	case 81:return 0;//"q"
		case 87:return 1;//"w"
		case 69:return 2;//"e"
		case 82:return 3;//"r"
		case 84:return 4;//"t"
		case 89:return 5;//"y"
		case 85:return 6;//"u"
		case 73:return 7;//"i"
		case 79:return 8;//"o"
		case 80:return 9;//"p"
		case 65:return 10;//"a"
		case 83:return 11;//"s"
		case 68:return 12;//"d"
		case 70:return 13;//"f"
		case 71:return 14;//"g"
		case 72:return 15;//"h"
		case 74:return 16;//"j"
		case 75:return 17;//"k"
		case 76:return 18;//"l"
		case 90:return 19;//"z"
		case 88:return 20;//"x"
		case 67:return 21;//"c"
		case 86:return 22;//"v"
		case 66:return 23;//"b"
		case 78:return 24;//"n"
		case 77:return 25;//"m"
		case 219:return 26;//"["
		case 221:return 27;//"]"
		case 222:return 28;//" " "
		case 220:return 29;// "\"
		case 188:return 30;//"<"
		case 190:return 31;//">"
		case 191:return 32;//"?"
		case 49:return 33;//"1"
		case 50:return 34;//"2"
		case 51:return 35;//"3"
		case 52:return 36;//"4"
		case 53:return 37;//"5"
		case 54:return 38;//"6"
		case 55:return 39;//"7"
		case 56:return 40;//"8"
		case 57:return 41;//"9"
		case 48:return 42;//"0"
		case 189:return 43;//"-"
		case 187:return 44;//"="
		case 8:return 45;//"\\backslash
		case 192:return 46;//~
		case 9:return 47;//tab
		case 112:return 48;//F1
		case 113:return 49;//F2
		case 114:return 50;//F3
		case 115:return 51;//F4
		case 116:return 52;//F5
		case 117:return 53;//F6
		case 118:return 54;//F7
		case 119:return 55;//F8
		case 120:return 56;//F9
		case 121:return 57;//F10
		case 122:return 58;//F11
		case 123:return 59;//F12
		case 13:return 60;//Enter
		case 45:return 61;//Insert
		case 46:return 62;//Delete
		case 36:return 63;//Home
		case 35:return 64;//End
		case 33:return 65;//PgUp
		case 34:return 66;//PgDwn
		case 37:return 67;//Left
		case 39:return 68;//Right
		case 38:return 69;//Up
		case 40:return 70;//Dwn
		case 96:return 71;//Numpad 0
		case 97:return 72;//Numpad 1
		case 98:return 73;//Numpad 2
		case 99:return 74;//Numpad 3
		case 100:return 75;//Numpad 4
		case 101:return 76;//Numpad 5
		case 102:return 77;//Numpad 6
		case 103:return 78;//Numpad 7
		case 104:return 79;//Numpad 8
		case 105:return 80;//Numpad 9
		case 110:return 81;//Numpad Del
		case 111:return 82;//Numpad /
		case 106:return 83;//Numpad *
		case 109:return 84;//Numpad -
		case 107:return 85;//Numpad +
		case 27:return 86;//Escape
		case 32:return 87;//Space
	}				   
	return 0;		   
}

wchar_t* DLG::GetCodeName(int k)
{
	switch(k)
	{	case 81:return L"q";
		case 87:return L"w";
		case 69:return L"e";
		case 82:return L"r";
		case 84:return L"t";
		case 89:return L"y";
		case 85:return L"u";
		case 73:return L"i";
		case 79:return L"o";
		case 80:return L"p";
		case 65:return L"a";
		case 83:return L"s";
		case 68:return L"d";
		case 70:return L"f";
		case 71:return L"g";
		case 72:return L"h";
		case 74:return L"j";
		case 75:return L"k";
		case 76:return L"l";
		case 90:return L"z";
		case 88:return L"x";
		case 67:return L"c";
		case 86:return L"v";
		case 66:return L"b";
		case 78:return L"n";
		case 77:return L"m";
		case 219:return L"[";
		case 221:return L"]";
		case 222:return L":";
		case 220:return L"\"";
		case 188:return L"<";
		case 190:return L">";
		case 191:return L"?";
		case 49:return L"1";
		case 50:return L"2";
		case 51:return L"3";
		case 52:return L"4";
		case 53:return L"5";
		case 54:return L"6";
		case 55:return L"7";
		case 56:return L"8";
		case 57:return L"9";
		case 48:return L"0";
		case 189:return L"-";
		case 187:return L"=";
		case 8:return L"backslash";
		case 192:return L"~";
		case 9:return L"tab";
		case 112:return L"F1";
		case 113:return L"F2";
		case 114:return L"F3";
		case 115:return L"F4";
		case 116:return L"F5";
		case 117:return L"F6";
		case 118:return L"F7";
		case 119:return L"F8";
		case 120:return L"F9";
		case 121:return L"F10";
		case 122:return L"F11";
		case 123:return L"F12";
		case 13:return L"Enter";
		case 45:return L"Insert";
		case 46:return L"Delete";
		case 36:return L"Home";
		case 35:return L"End";
		case 33:return L"PgUp";
		case 34:return L"PgDwn";
		case 37:return L"Left";
		case 39:return L"Right";
		case 38:return L"Up";
		case 40:return L"Down";
		case 96:return L"Numpad 0";
		case 97:return L"Numpad 1";
		case 98:return L"Numpad 2";
		case 99:return L"Numpad 3";
		case 100:return L"Numpad 4";
		case 101:return L"Numpad 5";
		case 102:return L"Numpad 6";
		case 103:return L"Numpad 7";
		case 104:return L"Numpad 8";
		case 105:return L"Numpad 9";
		case 110:return L"Numpad Del";
		case 111:return L"Numpad /";
		case 106:return L"Numpad *";
		case 109:return L"Numpad -";
		case 107:return L"Numpad +";
		case 27:return L"Escape";
		case 32:return L"Space";
	}				   
	return 0;		   
}

COLORREF DLG::GetColor(int colNum, int dlgNum)
{
	switch(dlgNum)
	{	case 24://panel[0]
			return panRGBs[0][colNum];
		case 25://panel[1]
			return panRGBs[1][colNum];
		case 26://panel[2]
			return panRGBs[2][colNum];
		case 27://panel[3]
			return panRGBs[3][colNum];
		case 28:
			return dlgRGBs[0][colNum];
		case 29:
			return dlgRGBs[1][colNum];
		case 30:
			return dlgRGBs[2][colNum];
		case 31:
			return dlgRGBs[3][colNum];
		case 32:
			return dlgRGBs[4][colNum];
		case 33:
			return dlgRGBs[5][colNum];
		case 34:
			return dlgRGBs[6][colNum];
		case 35:
			return dlgRGBs[7][colNum];
		case 36:
			return dlgRGBs[8][colNum];
		case 37:
			return menuRGB[colNum];
		case 38:
			return srtBtnRGB[0][colNum];
		case 39:
			return srtBtnRGB[1][colNum];
		case 40:
			return srtBtnRGB[2][colNum];
		case 41:
			return srtBtnRGB[3][colNum];
		case 42:
			return editRGB[0][colNum];
		case 43:
			return editRGB[1][colNum];
		case 44:
			return editRGB[2][colNum];
		case 45:
			return editRGB[3][colNum];
		case 46:
			return cmdBtnRGB[colNum];
	}
	return panRGBs[dlgNum][colNum];
}

COLORREF* DLG::GetColorPtr(int colNum, int dlgNum)
{
	switch(dlgNum)
	{	case 24://panel[0]
			return &panRGBs[0][colNum];
		case 25://panel[1]
			return &panRGBs[1][colNum];
		case 26://panel[2]
			return &panRGBs[2][colNum];
		case 27://panel[3]
			return &panRGBs[3][colNum];
		case 28:
			return &dlgRGBs[0][colNum];
		case 29:
			return &dlgRGBs[1][colNum];
		case 30:
			return &dlgRGBs[2][colNum];
		case 31:
			return &dlgRGBs[3][colNum];
		case 32:
			return &dlgRGBs[4][colNum];
		case 33:
			return &dlgRGBs[5][colNum];
		case 34:
			return &dlgRGBs[6][colNum];
		case 35:
			return &dlgRGBs[7][colNum];
		case 36:
			return &dlgRGBs[8][colNum];
		case 37:
			return &menuRGB[colNum];
		case 38:
			return &srtBtnRGB[0][colNum];
		case 39:
			return &srtBtnRGB[1][colNum];
		case 40:
			return &srtBtnRGB[2][colNum];
		case 41:
			return &srtBtnRGB[3][colNum];
		case 42:
			return &editRGB[0][colNum];
		case 43:
			return &editRGB[1][colNum];
		case 44:
			return &editRGB[2][colNum];
		case 45:
			return &editRGB[3][colNum];
		case 46:
			return &cmdBtnRGB[colNum];
	}
	return &panRGBs[dlgNum][colNum];
}

int DLG::GetKeyCode(int k)
{
	switch(k)
	{	case 0:return 81;//"q"
		case 1:return 87;//"w"
		case 2:return 69;//"e"
		case 3:return 82;//"r"
		case 4:return 84;//"t"
		case 5:return 89;//"y"
		case 6:return 85;//"u"
		case 7:return 73;//"i"
		case 8:return 79;//"o"
		case 9:return 80;//"p"
		case 10:return 65;//"a"
		case 11:return 83;//"s"
		case 12:return 68;//"d"
		case 13:return 70;//"f"
		case 14:return 71;//"g"
		case 15:return 72;//"h"
		case 16:return 74;//"j"
		case 17:return 75;//"k"
		case 18:return 76;//"l"
		case 19:return 90;//"z"
		case 20:return 88;//"x"
		case 21:return 67;//"c"
		case 22:return 86;//"v"
		case 23:return 66;//"b"
		case 24:return 78;//"n"
		case 25:return 77;//"m"
		case 26:return 219;//"["
		case 27:return 221;//"]"
		case 28:return 222;//;
		case 29:return 220;//"
		case 30:return 188;//"<"
		case 31:return 190;//">"
		case 32:return 191;//"?"
		case 33:return 49;//"1"
		case 34:return 50;//"2"
		case 35:return 51;//"3"
		case 36:return 52;//"4"
		case 37:return 53;//"5"
		case 38:return 54;//"6"
		case 39:return 55;//"7"
		case 40:return 56;//"8"
		case 41:return 57;//"9"
		case 42:return 48;//"0"
		case 43:return 189;//"-"
		case 44:return 187;//"="
		case 45:return 8;//"\\backslash
		case 46:return 192;//~
		case 47:return 9;//tab
		case 48:return 112;//F1
		case 49:return 113;//F2
		case 50:return 114;//F3
		case 51:return 115;//F4
		case 52:return 116;//F5
		case 53:return 117;//F6
		case 54:return 118;//F7
		case 55:return 119;//F8
		case 56:return 120;//F9
		case 57:return 121;//F10
		case 58:return 122;//F11
		case 59:return 123;//F12
		case 60:return 13;//Enter
		case 61:return 45;//Insert
		case 62:return 46;//Delete
		case 63:return 36;//Home
		case 64:return 35;//End
		case 65:return 33;//PgUp
		case 66:return 34;//PgDwn
		case 67:return 37;//Left
		case 68:return 39;//Right
		case 69:return 38;//Up
		case 70:return 40;//Dwn
		case 71:return 96;//Numpad 0
		case 72:return 97;//Numpad 1
		case 73:return 98;//Numpad 2
		case 74:return 99;//Numpad 3
		case 75:return 100;//Numpad 4
		case 76:return 101;//Numpad 5
		case 77:return 102;//Numpad 6
		case 78:return 103;//Numpad 7
		case 79:return 104;//Numpad 8
		case 80:return 105;//Numpad 9
		case 81:return 110;//Numpad Del
		case 82:return 111;//Numpad /
		case 83:return 106;//Numpad *
		case 84:return 109;//Numpad -
		case 85:return 107;//Numpad +
		case 86:return 27;//Escape
		case 87:return 32;//Space
	}				   
	return 0;		   
}

char* DLG::GetKeyName(int k)
{
	switch(k)
	{case 0:
		return "Select all";
	 case 1:
		return "Create tab sheet";
	 case 2:
		return "Delete tab sheet";
	 case 3:
		return "Help";
	 case 4:
		return "Disk select panel 1";
	 case 5:
		return "Disk select panel 2";
	 case 6:
		return "Disk select panel 3";
	 case 7:
		return "Disk select panel 4";
	 case 8:
		return "Edit name";
	 case 9:
		return "View";
	 case 10:
		return "Edit";
	 case 11:
		return "Copy";
	 case 12:
		return "Rename-move";
	 case 13:
		return "Search";
	 case 14:
		return "Crate folder";
	 case 15:
		return "Folder Up";
	 case 16:
		return "Fast select";
	 case 17:
		return "Change hot selection";
	 case 18:
		return "Select item";
	 case 19:
		return "Page Up";
	 case 20:
		return "Page Down";
	 case 21:
		return "To end";
	 case 22:
		return "To begin";
	 case 23:
		return "Up";
	 case 24:
		return "Down";
	 case 25:
		return "Escape";
	 case 26:
		return "Exit";
	 case 27:
		return "Page Up,2-key";
	 case 28:
		return "Page Down,2-key";
	 case 29:
		return "Properies";
	 case 30:
		return "Enter";
	 case 31:
		return "Select Page Up";
	 case 32:
		return "Select Page Down";
	 case 33:
		return "Select Page Up,2-key";
	 case 34:
		return "Select Page Down,2-key";
	 case 35:
		return "Select to end";
	 case 36:
		return "Select to begin";
	 case 37:
		return "Select, up";
	 case 38:
		return "Select, down";
	 case 39:
		return "Delete";
	 case 40:
		return "Delete,2-key";
	 case 41:
		return "Unrestorable delete";
	 case 42:
		return "Unrestorable delete,2-key";
	}
	return NULL;
}

int DLG::KeyToText(int k,wchar_t *s)
{
int ln=0;
	if(keys[k].bAlt)
	{	s[0]='A';s[1]='l';s[2]='t';
		if(keys[k].bCtrl)
		{	s[3]='+';s[4]='C';s[5]='t';s[6]='l';
			if(keys[k].bShft)
			{	s[7]='+';s[8]='S';s[9]='f';s[10]='t';s[11]='+';
				ln=12+MyStringCpy(&s[12],32,GetCodeName(keys[k].key));
				return ln;
			}
			else
			{	s[7]='+';
				ln=8+MyStringCpy(&s[8],32,GetCodeName(keys[k].key));
				return ln;
		}	}
		else
		{	s[3]='+';
			ln=4+MyStringCpy(&s[4],32,GetCodeName(keys[k].key));
			return ln;
	}	}
	//else
	ln=MyStringCpy(s,32,GetCodeName(keys[k].key));
	return ln;
}

VOID DLG::InitLOGFONT(LOGFONTW* lf,int t)
{
	switch(t)
	{case 0://panel
		lf->lfCharSet=204;//Kirill
		lf->lfClipPrecision=2;
		lf->lfEscapement=0;
		StringCchPrintf(lf->lfFaceName,LF_FACESIZE,L"Arial");//Arial Rounded MT Bold");//Tahoma");//"Arial");//Arial Narrow");
		lf->lfHeight=-11;
		lf->lfItalic=0;
		lf->lfOrientation=0;
		lf->lfOutPrecision=3;
		lf->lfPitchAndFamily=34;
		lf->lfQuality=1;
		lf->lfStrikeOut=0;
		lf->lfUnderline=0;
		lf->lfWeight=700;
		lf->lfWidth=0;
		break;
	case 1://Dialogs:
		lf->lfCharSet=204;//Kirill
		lf->lfClipPrecision=0;
		lf->lfEscapement=0;
		StringCchPrintf(lf->lfFaceName,LF_FACESIZE,L"MS Shell Dlg 2");
		lf->lfHeight=0xfffffff5;
		lf->lfItalic=0;
		lf->lfOrientation=0;
		lf->lfOutPrecision=0;
		lf->lfPitchAndFamily=0;
		lf->lfQuality=0;
		lf->lfStrikeOut=0;
		lf->lfUnderline=0;
		lf->lfWeight=700;
		lf->lfWidth=0;
		break;
	case 2://SrtBtns:
		lf->lfCharSet=204;//Kirill
		lf->lfClipPrecision=0;
		lf->lfEscapement=0;
		StringCchPrintf(lf->lfFaceName,LF_FACESIZE,L"MS Shell Dlg 2");
		lf->lfHeight=0xfffffff5;
		lf->lfItalic=0;
		lf->lfOrientation=0;
		lf->lfOutPrecision=0;
		lf->lfPitchAndFamily=0;
		lf->lfQuality=0;
		lf->lfStrikeOut=0;
		lf->lfUnderline=0;
		lf->lfWeight=0x190;
		lf->lfWidth=0;
		break;
}	}

VOID DLG::Init6RGB(COLORREF r[6],int type)
{
	switch(type)
	{	case 0://panel
			r[0] = 0x00c19317;//0x00f1d587;//715e11;//0x00ebe0eb;//Back
			r[1] = 0x00000000;//0x00400040;//ffffff;//210201;//0x008a0000;//Front
			r[2] = 0x00b1851d;//0x00ebcc83;//0x00ebe0eb;//Select back
			r[3] = 0x00ff0000;//Select
			r[4] = 0x008000ff;//0x00ff0000;//Hot
			r[5] = 0x00eeaed7;//Hot back
			break;
		case 1://dlg
			r[0] = 0x00c98193;//0x00ebe0eb;//Back
			r[1] = 0x008a0000;//0x008a0000;//Front
			r[2] = 0x00c08197;//Select back
			r[3] = 0x000000ff;//Select
			r[4] = 0x00ff0000;//Hot
			r[5] = 0x00748975;//Hot back
			break;
		case 2://menu
			r[0] = 0x00ebe0eb;//0x00ebe0eb;//Back
			r[1] = 0x008a0000;//0x008a0000;//Front
			r[2] = 0x00ece1ec;//Select back
			r[3] = 0x000000ff;//Select
			r[4] = 0x00ff0000;//Hot
			r[5] = 0x00eeaed7;//Hot back
			break;
		case 3://srt btn
			r[0] = 0x00ebe0eb;//0x00ebe0eb;//Back
			r[1] = 0x008a0000;//0x008a0000;//Front
			r[2] = 0x00ffff80;//Select back
			r[3] = 0x000000ff;//Select
			r[4] = 0x00ff0000;//Hot
			r[5] = 0x00ffff80;//Hot back
			break;
		case 4://edit
			r[0] = 0x00ebe0eb;//0x00ebe0eb;//Back
			r[1] = 0x00000000;//0x008a0000;//Front
			r[2] = 0x00ffc142;//Select back
			r[3] = 0x000000ff;//Select
			r[4] = 0x00ff0000;//Hot
			r[5] = 0x00ece1ec;//Hot back
			break;
		case 5://cmdBtn
			r[0] = 0x00ebe0eb;//0x00ebe0eb;//Back
			r[1] = 0x00370000;//0x008a0000;//Front
			r[2] = 0x00ece1ec;//Select back
			r[3] = 0x000000ff;//Select
			r[4] = 0x00ff0000;//Hot
			r[5] = 0x00eeaed7;//Hot back
			break;
}	}

BOOL DLG::LoadFrom()
{
OPENFILENAMEW of;
wchar_t s[MAX_PATH]=L"";
	memset(&of, 0, sizeof (OPENFILENAME));
	of.lStructSize      = sizeof(OPENFILENAME);
	of.lpstrFilter      = L"All text files (*.txt)\0*.txt\0All files\0*.*\0";
	of.lpstrFile        = s;
	of.lpstrTitle       = L"Open configuration file from text file.";
	of.lpstrFileTitle   = L"Configuration.";
	of.Flags            = OFN_EXPLORER|OFN_FILEMUSTEXIST;
	of.nMaxFile			= MAX_PATH;
	if(GetOpenFileName(&of))
	{	FILE *f;
		if(_wfopen_s(&f,s,L"r"))
		{	Err::msg1(hWnd,-1,L"Opening file",s);
			return FALSE;
		}

		if(!freadFromChar(f,'=',L"Total panels=",L"%d",&iTotPanels,sizeof(int)))goto End;
		if(!freadFromChar(f,'=',L"Total instances=",L"%d",&iNumInstans,sizeof(int)))goto End;
		if(!freadFromChar(f,'=',L"Start position=",L"%d",&iStartPosition,sizeof(int)))goto End;
		if(!freadFromChar(f,'=',L"Hide method=",L"%d",&iHideMethod,sizeof(int)))goto End;
		if(!freadFromChar(f,'=',L"Enum method=",L"%d",&iEnumMethod,sizeof(int)))goto End;

		wchar_t s[64];

		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"Icon size for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&iIconSize[p],sizeof(int)))goto End;
		}

		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"Interval for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&interval[p],sizeof(int)))goto End;
		}

		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"Font character set for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfCharSet,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font clip precision for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfClipPrecision,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font escapement for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfEscapement,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font face name for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%s",&panFnts[p].lfFaceName,64))goto End;
			
			StringCchPrintf(s,64,L"Font height for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfHeight,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font italic for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfItalic,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font orientation for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfOrientation,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font out precision for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfOutPrecision,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font pitch and family for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfPitchAndFamily,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font quality for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfQuality,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font strike out for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfStrikeOut,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font underline for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfUnderline,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font weight for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfWeight,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font width for panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&panFnts[p].lfWidth,sizeof(LONG)))goto End;
		}

		for(int p=0; p<9; p++)
		{	StringCchPrintf(s,64,L"Font character set for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfCharSet,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font clip precision for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfClipPrecision,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font escapement for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfEscapement,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font face name for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%s",&dlgFnts[p].lfFaceName,64))goto End;

			StringCchPrintf(s,64,L"Font height for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfHeight,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font italic for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfItalic,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font orientation for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfOrientation,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font out precision for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfOutPrecision,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font pitch and family for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfPitchAndFamily,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font quality for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfQuality,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font strike out for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfStrikeOut,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font underline for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfUnderline,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font weight for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfWeight,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font width for dialog %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&dlgFnts[p].lfWidth,sizeof(LONG)))goto End;
		}
		//LOGFONT	 menuFnt;
		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"Font character set for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfCharSet,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font clip precision for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfClipPrecision,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font escapement for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfEscapement,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font face name for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%s",&srtBtnFnt[p].lfFaceName,64))goto End;

			StringCchPrintf(s,64,L"Font height for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfHeight,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font italic for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfItalic,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font orientation for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfOrientation,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font out precision for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfOutPrecision,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font pitch and family for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfPitchAndFamily,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font quality for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfQuality,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font strike out for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfStrikeOut,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font underline for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfUnderline,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font weight for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfWeight,sizeof(LONG)))goto End;

			StringCchPrintf(s,64,L"Font width for sort button of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&srtBtnFnt[p].lfWidth,sizeof(LONG)))goto End;
		}
		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"Font character set for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfCharSet,sizeof(BYTE)))goto End;

			StringCchPrintf(s,64,L"Font clip precision for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfClipPrecision,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font escapement for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfEscapement,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font face name for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%s",&editFnt[p].lfFaceName,64))goto End;
			
			StringCchPrintf(s,64,L"Font height for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfHeight,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font italic for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfItalic,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font orientation for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfOrientation,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font out precision for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfOutPrecision,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font pitch and family for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfPitchAndFamily,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font quality for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfQuality,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font strike out for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfStrikeOut,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font underline for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfUnderline,sizeof(BYTE)))goto End;
			
			StringCchPrintf(s,64,L"Font weight for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfWeight,sizeof(LONG)))goto End;
			
			StringCchPrintf(s,64,L"Font width for edit controls of panel %d=",p+1);
			if(!freadFromChar(f,'=',s,L"%d",&editFnt[p].lfWidth,sizeof(LONG)))goto End;
		}
		StringCchPrintf(s,64,L"Font character set for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfCharSet,sizeof(BYTE)))goto End;

		StringCchPrintf(s,64,L"Font clip precision for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfClipPrecision,sizeof(BYTE)))goto End;
		
		StringCchPrintf(s,64,L"Font escapement for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfEscapement,sizeof(LONG)))goto End;
		
		StringCchPrintf(s,64,L"Font face name for command buttons =");
		if(!freadFromChar(f,'=',s,L"%s",&cmdBtnFnt.lfFaceName,64))goto End;
		
		StringCchPrintf(s,64,L"Font height for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfHeight,sizeof(LONG)))goto End;
		
		StringCchPrintf(s,64,L"Font italic for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfItalic,sizeof(BYTE)))goto End;
		
		StringCchPrintf(s,64,L"Font orientation for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfOrientation,sizeof(LONG)))goto End;
		
		StringCchPrintf(s,64,L"Font out precision for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfOutPrecision,sizeof(BYTE)))goto End;
		
		StringCchPrintf(s,64,L"Font pitch and family for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfPitchAndFamily,sizeof(BYTE)))goto End;
		
		StringCchPrintf(s,64,L"Font quality for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfQuality,sizeof(BYTE)))goto End;
		
		StringCchPrintf(s,64,L"Font strike out for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfStrikeOut,sizeof(BYTE)))goto End;
		
		StringCchPrintf(s,64,L"Font underline for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfUnderline,sizeof(BYTE)))goto End;
		
		StringCchPrintf(s,64,L"Font weight for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfWeight,sizeof(LONG)))goto End;
		
		StringCchPrintf(s,64,L"Font width for command buttons =");
		if(!freadFromChar(f,'=',s,L"%d",&cmdBtnFnt.lfWidth,sizeof(LONG)))goto End;

		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"%d-panel back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&panRGBs[p][0]))goto End;

			StringCchPrintf(s,64,L"%d-panel front color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&panRGBs[p][1]))goto End;

			StringCchPrintf(s,64,L"%d-panel selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&panRGBs[p][2]))goto End;

			StringCchPrintf(s,64,L"%d-panel selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&panRGBs[p][3]))goto End;

			StringCchPrintf(s,64,L"%d-panel hot selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&panRGBs[p][4]))goto End;

			StringCchPrintf(s,64,L"%d-panel hot selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&panRGBs[p][5]))goto End;
		}
		for(int p=0; p<9; p++)
		{	StringCchPrintf(s,64,L"%d-dialog back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&dlgRGBs[p][0]))goto End;

			StringCchPrintf(s,64,L"%d-dialog front color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&dlgRGBs[p][1]))goto End;

			StringCchPrintf(s,64,L"%d-dialog selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&dlgRGBs[p][2]))goto End;

			StringCchPrintf(s,64,L"%d-dialog selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&dlgRGBs[p][3]))goto End;

			StringCchPrintf(s,64,L"%d-dialog hot selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&dlgRGBs[p][4]))goto End;

			StringCchPrintf(s,64,L"%d-dialog hot selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&panRGBs[p][5]))goto End;
		}
		//COLORREF menuRGB[6];
		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"%d-panel sort buttons back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&srtBtnRGB[p][0]))goto End;

			StringCchPrintf(s,64,L"%d-panel sort buttons front color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&srtBtnRGB[p][1]))goto End;

			StringCchPrintf(s,64,L"%d-panel sort buttons selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&srtBtnRGB[p][2]))goto End;

			StringCchPrintf(s,64,L"%d-panel sort buttons selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&srtBtnRGB[p][3]))goto End;

			StringCchPrintf(s,64,L"%d-panel sort buttons hot selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&srtBtnRGB[p][4]))goto End;

			StringCchPrintf(s,64,L"%d-panel sort buttons hot selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&srtBtnRGB[p][5]))goto End;
		}
		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"%d-panel edit controls back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&editRGB[p][0]))goto End;

			StringCchPrintf(s,64,L"%d-panel edit controls front color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&editRGB[p][1]))goto End;

			StringCchPrintf(s,64,L"%d-panel edit controls selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&editRGB[p][2]))goto End;

			StringCchPrintf(s,64,L"%d-panel edit controls selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&editRGB[p][3]))goto End;

			StringCchPrintf(s,64,L"%d-panel edit controls hot selected color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&editRGB[p][4]))goto End;

			StringCchPrintf(s,64,L"%d-panel edit controls hot selected back color =",p+1);
			if(!freadRGBFromChar(f,'=',s,&editRGB[p][5]))goto End;
		}
		if(!freadRGBFromChar(f,'=',L"Command buttons back color =",&cmdBtnRGB[0]))goto End;
		if(!freadRGBFromChar(f,'=',L"Command buttons front color =",&cmdBtnRGB[1]))goto End;
		if(!freadRGBFromChar(f,'=',L"Command buttons selected back color =",&cmdBtnRGB[2]))goto End;
		if(!freadRGBFromChar(f,'=',L"Command buttons selected color =",&cmdBtnRGB[3]))goto End;
		if(!freadRGBFromChar(f,'=',L"Command buttons hot selected color =",&cmdBtnRGB[4]))goto End;
		if(!freadRGBFromChar(f,'=',L"Command buttons hot selected back color =",&cmdBtnRGB[5]))goto End;

		for(int k=0; k<MAX_KEYS; k++)
		{	StringCchPrintf(s,64,L"Key[%d]-name=",k+1);
			char st[64];
			if(!freadFromChar(f,'=',s,L"%s",st,64))goto End;
			BYTE btns[4];
			if(!freadFromChar(f,'=',L";control pressed =",L"%d",&btns[0],sizeof(BYTE)))goto End;
			if(!freadFromChar(f,'=',L",alt.pressed =",L"%d",&btns[1],sizeof(BYTE)))goto End;
			if(!freadFromChar(f,'=',L",shift pressed =",L"%d",&btns[2],sizeof(BYTE)))goto End;
			if(!freadFromChar(f,'=',L",code=",L"%d",&btns[3],sizeof(BYTE)))goto End;
			if(!freadFromChar(f,'=',L"; code name=",L"%s",st,64))goto End;
			keys[k].bCtrl = btns[0];
			keys[k].bAlt = btns[1];
			keys[k].bShft = btns[2];
			keys[k].key = btns[3];
		}

		for(int p=0; p<MAX_PANELS; p++)
		{	StringCchPrintf(s,64,L"\n\nTotal panel custom panel item definitions for panel %d =%d",p+1);
			int cnt;
			if(!freadFromChar(f,'=',s,L"%d",&cnt,sizeof(int)))goto End;
			ReallocCustomItems(p,cnt,FALSE);
			for(int i=0; i<iCustPanItemDefs[p]; i++)
			{	StringCchPrintf(s,64,L"\n\n %d=",i+1);
				if(!freadFromChar(f,'=',s,L"%s",cstmItms[p][i].nameW,24))goto End;
				
				if(!freadFromChar(f,'=',L"\n	Custom item sort method=",L"%d",&cstmItms[p][i].sortMthd,sizeof(int)))goto End;

				if(!freadFromChar(f,'=',L"\n	Font character set for custom item =",L"%d",&cstmItms[p][i].fnt.lfCharSet,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font clip precision for custom item =",L"%d",&cstmItms[p][i].fnt.lfClipPrecision,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font escapement for custom item =",L"%d",&cstmItms[p][i].fnt.lfEscapement,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font height for custom item =",L"%d",&cstmItms[p][i].fnt.lfHeight,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font italic for custom item =",L"%d",&cstmItms[p][i].fnt.lfItalic,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font orientation for custom item =",L"%d",&cstmItms[p][i].fnt.lfOrientation,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font out precision for custom item =",L"%d",&cstmItms[p][i].fnt.lfOutPrecision,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font pitch and family for custom item =",L"%d",&cstmItms[p][i].fnt.lfPitchAndFamily,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font quality for custom item =",L"%d",&cstmItms[p][i].fnt.lfQuality,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font strike out for custom item =",L"%d",&cstmItms[p][i].fnt.lfStrikeOut,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font underline for custom item =",L"%d",&cstmItms[p][i].fnt.lfUnderline,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font weight for custom item =",L"%d",&cstmItms[p][i].fnt.lfWeight,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',L"\n	Font width for custom item =",L"%d",&cstmItms[p][i].fnt.lfWidth,sizeof(int)))goto End;

				if(!freadRGBFromChar(f,'=',L"\n	Custom item back color =",&cstmItms[p][i].clrRGBs[0]))goto End;
				if(!freadRGBFromChar(f,'=',L"\n	Custom item front color =",&cstmItms[p][i].clrRGBs[1]))goto End;
				if(!freadRGBFromChar(f,'=',L"\n	Custom item selected back color =",&cstmItms[p][i].clrRGBs[2]))goto End;
				if(!freadRGBFromChar(f,'=',L"\n	Custom item selected color =",&cstmItms[p][i].clrRGBs[3]))goto End;
				if(!freadRGBFromChar(f,'=',L"\n	Custom item hot selected color =",&cstmItms[p][i].clrRGBs[4]))goto End;
				if(!freadRGBFromChar(f,'=',L"\n	Custom item hot selected back color =",&cstmItms[p][i].clrRGBs[5]))goto End;

				/* shifted to search plugin:
				if(!freadFromChar(f,'=',"\n	Custom item attrib:alter.name =","%s",&cstmItms[p][i].itm.altName,260))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:unicode alter.name =","%s",&cstmItms[p][i].itm.altName,260))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:after create time =","%d",&cstmItms[p][i].itm.bCrTimeAft,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:before create time =","%d",&cstmItms[p][i].itm.bCrTimeBef,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:between create time =","%d",&cstmItms[p][i].itm.bCrTimeBet,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:archive =","%d",&cstmItms[p][i].itm.bFileAttArchive,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:compression =","%d",&cstmItms[p][i].itm.bFileAttCompr,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:device =","%d",&cstmItms[p][i].itm.bFileAttDevice,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:directory =","%d",&cstmItms[p][i].itm.bFileAttDir,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:encryption =","%d",&cstmItms[p][i].itm.bFileAttEncr,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:hidden =","%d",&cstmItms[p][i].itm.bFileAttHidden,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:normal =","%d",&cstmItms[p][i].itm.bFileAttNormal,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:not ind. =","%d",&cstmItms[p][i].itm.bFileAttNotInd,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:offline =","%d",&cstmItms[p][i].itm.bFileAttOffl,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib: =","%d",&cstmItms[p][i].itm.bFileAttr,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:read only =","%d",&cstmItms[p][i].itm.bFileAttReadOnly,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:reparse point =","%d",&cstmItms[p][i].itm.bFileAttRepPt,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:sparse file =","%d",&cstmItms[p][i].itm.bFileAttSparseFile,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:system =","%d",&cstmItms[p][i].itm.bFileAttSys,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:temporary =","%d",&cstmItms[p][i].itm.bFileAttTemp,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:virtual =","%d",&cstmItms[p][i].itm.bFileAttVirt,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:size =","%d",&cstmItms[p][i].itm.bFileSz,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for alt.name =","%d",&cstmItms[p][i].itm.bFindForAlterName,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for exclude text =","%d",&cstmItms[p][i].itm.bFindForExcldText,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for exclude text ASCII =","%d",&cstmItms[p][i].itm.bFindForExcldText_ASCII,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for exclude text Unicode =","%d",&cstmItms[p][i].itm.bFindForExcldText_Unicode,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for exclude text Upper case =%","%d",&cstmItms[p][i].itm.bFindForExcldText_UpperCase,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for text =%","%d",&cstmItms[p][i].itm.bFindForText,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for text ASCII =%","%d",&cstmItms[p][i].itm.bFindForText_ASCII,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for text Unicode =%","%d",&cstmItms[p][i].itm.bFindForText_Unicode,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for text Upper case =%","%d",&cstmItms[p][i].itm.bFindForText_UpperCase,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:between last access time =%","%d",&cstmItms[p][i].itm.bLastAccTimeBet,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:after last access time =%","%d",&cstmItms[p][i].itm.bLstAccTimeAft,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:before last access time =%","%d",&cstmItms[p][i].itm.bLstAccTimeBef,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:after last write time =%","%d",&cstmItms[p][i].itm.bLstWrTimeAft,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:before last write time =%","%d",&cstmItms[p][i].itm.bLstWrTimeBef,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:between last write time =%","%d",&cstmItms[p][i].itm.bLstWrTimeBet,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:after create time =","%d",&cstmItms[p][i].itm.CrTimeAft.dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.CrTimeAft.dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:before create time =","%d",&cstmItms[p][i].itm.CrTimeBef.dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.CrTimeBef.dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:between create time =","%d",&cstmItms[p][i].itm.CrTimeBet[0].dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.CrTimeBet[0].dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.CrTimeBet[1].dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.CrTimeBet[1].dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:filter =","%s",&cstmItms[p][i].itm.filtr,520))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:exclude text =","%s",&cstmItms[p][i].itm.FindForExcldText,520))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:exclude text length =","%d",&cstmItms[p][i].itm.FindForExcldTextLn,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for text =","%s",&cstmItms[p][i].itm.FindForText,520))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:find for text length =","%d",&cstmItms[p][i].itm.FindForTextLn,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:file size equation =","%d",&cstmItms[p][i].itm.iFileSzQual,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:between last access time =","%d",&cstmItms[p][i].itm.LastAccTimeBet[0].dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LastAccTimeBet[0].dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LastAccTimeBet[1].dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LastAccTimeBet[1].dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:after last acces time =","%d",&cstmItms[p][i].itm.LstAccTimeAft.dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LstAccTimeAft.dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:before last acces time =","%d",&cstmItms[p][i].itm.LstAccTimeBef.dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LstAccTimeBef.dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:after last write time =","%d",&cstmItms[p][i].itm.LstWrTimeAft.dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LstWrTimeAft.dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:before last write time =","%d",&cstmItms[p][i].itm.LstWrTimeBef.dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LstWrTimeBef.dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:between last write time =","%d",&cstmItms[p][i].itm.LstWrTimeBet[0].dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LstWrTimeBet[0].dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LstWrTimeBet[1].dwLowDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',", =","%d",&cstmItms[p][i].itm.LstWrTimeBet[1].dwHighDateTime,sizeof(int)))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:root path =","%s",&cstmItms[p][i].itm.rootPath,520))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:file size =","%s",&cstmItms[p][i].itm.sFileSz,260))goto End;
				if(!freadFromChar(f,'=',"\n	Custom item attrib:file size equation =","%s",&cstmItms[p][i].itm.sFileSzEqu,4))goto End;
				*/
		}	}
		//freadFromChar(f,'=',"\n STD view for buttons:","%d",conf::Dlg.bBtnsStdView?L"TRUE":L"FALSE");
		fclose(f);
		return TRUE;
End:	//Error occured:
		fclose(f);
	}
	return FALSE;
}

BOOL DLG::ReallocCustomItems(int iPan,int sz,BOOL bInit)
{
	if(!cstmItms[iPan])
		cstmItms[iPan] = (CstmItemDef*)malloc(sz*sizeof(CstmItemDef));
	else
	{	if(bInit)
		{	for(int j=0; j<iCustPanItemDefs[iPan]; j++)
			{	if(cstmItms[iPan][j].hFnt)
				{	DeleteObject(cstmItms[iPan][j].hFnt);
					DeleteObject(cstmItms[iPan][j].txtBkBrsh);
					DeleteObject(cstmItms[iPan][j].bkBrsh);
					DeleteObject(cstmItms[iPan][j].hotBkBrsh);
		}	}	}
		cstmItms[iPan] = (CstmItemDef*)realloc(cstmItms[iPan],sz*sizeof(CstmItemDef));
	}
	for(int s=bInit?0:iCustPanItemDefs[iPan]; s<sz; s++)
	{	cstmItms[iPan][s].sortMthd=0;
		cstmItms[iPan][s].nameW[0]=0;
		DLG::InitLOGFONT(&cstmItms[iPan][s].fnt,1);
		cstmItms[iPan][s].hFnt = CreateFontIndirect(&cstmItms[iPan][s].fnt);
		Init6RGB(cstmItms[iPan][s].clrRGBs,0);
		cstmItms[iPan][s].itm.SearchItem::TSearchItem();
		cstmItms[iPan][s].txtBkBrsh = CreateSolidBrush(cstmItms[iPan][s].clrRGBs[2]);
		cstmItms[iPan][s].bkBrsh = CreateSolidBrush(cstmItms[iPan][s].clrRGBs[0]);
		cstmItms[iPan][s].hotBkBrsh = CreateSolidBrush(cstmItms[iPan][s].clrRGBs[5]);
	}
	if(cstmItms[iPan])
	{	iCustPanItemDefs[iPan]=sz;
		return TRUE;
	}
	return FALSE;
}

VOID DLG::SaveAs()
{
OPENFILENAMEW of;
wchar_t s[MAX_PATH]=L"";
	memset(&of, 0, sizeof (OPENFILENAME));
	of.lStructSize      = sizeof(OPENFILENAME);
	of.lpstrFilter      = L"All text files (*.txt)\0*.txt\0All files\0*.*\0";
	of.lpstrFile        = s;
	of.lpstrTitle       = L"Save configuration file as text file.";
	of.lpstrFileTitle   = L"Configuration.";
	of.Flags            = OFN_EXPLORER|OFN_OVERWRITEPROMPT;
	of.nMaxFile			= MAX_PATH;
	if(GetSaveFileName(&of))
	{	FILE *f;
		if(_wfopen_s(&f,s,L"w"))
		{	Err::msg1(hWnd,-1,L"Opening file",s);
			return;
		}
		fwprintf(f,L"Total panels=%d", iTotPanels);
		fwprintf(f,L"\nTotal instances=%d", iNumInstans);
		fwprintf(f,L"\nStart position=%d", iStartPosition);
		fwprintf(f,L"\nHide method=%d", iHideMethod);
		fwprintf(f,L"\nEnum method=%d\n", iEnumMethod);
		for(int p=0; p<MAX_PANELS; p++)
			fwprintf(f,L"\nIcon size for panel %d=%d", p+1,iIconSize[p]);
		for(int p=0; p<MAX_PANELS; p++)
			fwprintf(f,L"\nInterval for panel %d=%d", p+1,interval[p]);

		for(int p=0; p<MAX_PANELS; p++)
		{	fwprintf(f,L"\n\nFont character set for panel %d=%d", p+1,panFnts[p].lfCharSet);
			fwprintf(f,L"\nFont clip precision for panel %d=%d", p+1,panFnts[p].lfClipPrecision);
			fwprintf(f,L"\nFont escapement for panel %d=%d", p+1,panFnts[p].lfEscapement);
			fwprintf(f,L"\nFont face name for panel %d=\"%s\"", p+1,panFnts[p].lfFaceName);
			fwprintf(f,L"\nFont height for panel %d=%d", p+1,panFnts[p].lfHeight);
			fwprintf(f,L"\nFont italic for panel %d=%d", p+1,panFnts[p].lfItalic);
			fwprintf(f,L"\nFont orientation for panel %d=%d", p+1,panFnts[p].lfOrientation);
			fwprintf(f,L"\nFont out precision for panel %d=%d", p+1,panFnts[p].lfOutPrecision);
			fwprintf(f,L"\nFont pitch and family for panel %d=%d", p+1,panFnts[p].lfPitchAndFamily);
			fwprintf(f,L"\nFont quality for panel %d=%d", p+1,panFnts[p].lfQuality);
			fwprintf(f,L"\nFont strike out for panel %d=%d", p+1,panFnts[p].lfStrikeOut);
			fwprintf(f,L"\nFont underline for panel %d=%d", p+1,panFnts[p].lfUnderline);
			fwprintf(f,L"\nFont weight for panel %d=%d", p+1,panFnts[p].lfWeight);
			fwprintf(f,L"\nFont width for panel %d=%d", p+1,panFnts[p].lfWidth);
		}
		for(int p=0; p<MAX_DLG; p++)
		{	fwprintf(f,L"\n\nFont character set for dialog %d=%d", p+1,dlgFnts[p].lfCharSet);
			fwprintf(f,L"\nFont clip precision for dialog %d=%d", p+1,dlgFnts[p].lfClipPrecision);
			fwprintf(f,L"\nFont escapement for dialog %d=%d", p+1,dlgFnts[p].lfEscapement);
			fwprintf(f,L"\nFont face name for dialog %d=\"%s\"", p+1,dlgFnts[p].lfFaceName);
			fwprintf(f,L"\nFont height for dialog %d=%d", p+1,dlgFnts[p].lfHeight);
			fwprintf(f,L"\nFont italic for dialog %d=%d", p+1,dlgFnts[p].lfItalic);
			fwprintf(f,L"\nFont orientation for dialog %d=%d", p+1,dlgFnts[p].lfOrientation);
			fwprintf(f,L"\nFont out precision for dialog %d=%d", p+1,dlgFnts[p].lfOutPrecision);
			fwprintf(f,L"\nFont pitch and family for dialog %d=%d", p+1,dlgFnts[p].lfPitchAndFamily);
			fwprintf(f,L"\nFont quality for dialog %d=%d", p+1,dlgFnts[p].lfQuality);
			fwprintf(f,L"\nFont strike out for dialog %d=%d", p+1,dlgFnts[p].lfStrikeOut);
			fwprintf(f,L"\nFont underline for dialog %d=%d", p+1,dlgFnts[p].lfUnderline);
			fwprintf(f,L"\nFont weight for dialog %d=%d", p+1,dlgFnts[p].lfWeight);
			fwprintf(f,L"\nFont width for dialog %d=%d", p+1,dlgFnts[p].lfWidth);
		}
		//LOGFONT	 menuFnt;
		for(int p=0; p<MAX_PANELS; p++)
		{	fwprintf(f,L"\n\nFont character set for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfCharSet);
			fwprintf(f,L"\nFont clip precision for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfClipPrecision);
			fwprintf(f,L"\nFont escapement for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfEscapement);
			fwprintf(f,L"\nFont face name for sort button of panel %d=\"%s\"", p+1,srtBtnFnt[p].lfFaceName);
			fwprintf(f,L"\nFont height for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfHeight);
			fwprintf(f,L"\nFont italic for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfItalic);
			fwprintf(f,L"\nFont orientation for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfOrientation);
			fwprintf(f,L"\nFont out precision for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfOutPrecision);
			fwprintf(f,L"\nFont pitch and family for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfPitchAndFamily);
			fwprintf(f,L"\nFont quality for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfQuality);
			fwprintf(f,L"\nFont strike out for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfStrikeOut);
			fwprintf(f,L"\nFont underline for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfUnderline);
			fwprintf(f,L"\nFont weight for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfWeight);
			fwprintf(f,L"\nFont width for sort button of panel %d=%d", p+1,srtBtnFnt[p].lfWidth);
		}
		for(int p=0; p<MAX_PANELS; p++)
		{	fwprintf(f,L"\n\nFont character set for edit controls of panel %d=%d", p+1,editFnt[p].lfCharSet);
			fwprintf(f,L"\nFont clip precision for edit controls of panel %d=%d", p+1,editFnt[p].lfClipPrecision);
			fwprintf(f,L"\nFont escapement for edit controls of panel %d=%d", p+1,editFnt[p].lfEscapement);
			fwprintf(f,L"\nFont face name for edit controls of panel %d=\"%s\"", p+1,editFnt[p].lfFaceName);
			fwprintf(f,L"\nFont height for edit controls of panel %d=%d", p+1,editFnt[p].lfHeight);
			fwprintf(f,L"\nFont italic for edit controls of panel %d=%d", p+1,editFnt[p].lfItalic);
			fwprintf(f,L"\nFont orientation for edit controls of panel %d=%d", p+1,editFnt[p].lfOrientation);
			fwprintf(f,L"\nFont out precision for edit controls of panel %d=%d", p+1,editFnt[p].lfOutPrecision);
			fwprintf(f,L"\nFont pitch and family for edit controls of panel %d=%d", p+1,editFnt[p].lfPitchAndFamily);
			fwprintf(f,L"\nFont quality for edit controls of panel %d=%d", p+1,editFnt[p].lfQuality);
			fwprintf(f,L"\nFont strike out for edit controls of panel %d=%d", p+1,editFnt[p].lfStrikeOut);
			fwprintf(f,L"\nFont underline for edit controls of panel %d=%d", p+1,editFnt[p].lfUnderline);
			fwprintf(f,L"\nFont weight for edit controls of panel %d=%d", p+1,editFnt[p].lfWeight);
			fwprintf(f,L"\nFont width for edit controls of panel %d=%d", p+1,editFnt[p].lfWidth);
		}
		fwprintf(f,L"\n\nFont character set for command buttons =%d", cmdBtnFnt.lfCharSet);
		fwprintf(f,L"\nFont clip precision for command buttons =%d", cmdBtnFnt.lfClipPrecision);
		fwprintf(f,L"\nFont escapement for command buttons =%d", cmdBtnFnt.lfEscapement);
		fwprintf(f,L"\nFont face name for command buttons =\"%s\"", cmdBtnFnt.lfFaceName);
		fwprintf(f,L"\nFont height for command buttons =%d", cmdBtnFnt.lfHeight);
		fwprintf(f,L"\nFont italic for command buttons =%d", cmdBtnFnt.lfItalic);
		fwprintf(f,L"\nFont orientation for command buttons =%d", cmdBtnFnt.lfOrientation);
		fwprintf(f,L"\nFont out precision for command buttons =%d", cmdBtnFnt.lfOutPrecision);
		fwprintf(f,L"\nFont pitch and family for command buttons =%d", cmdBtnFnt.lfPitchAndFamily);
		fwprintf(f,L"\nFont quality for command buttons =%d", cmdBtnFnt.lfQuality);
		fwprintf(f,L"\nFont strike out for command buttons =%d", cmdBtnFnt.lfStrikeOut);
		fwprintf(f,L"\nFont underline for command buttons =%d", cmdBtnFnt.lfUnderline);
		fwprintf(f,L"\nFont weight for command buttons =%d", cmdBtnFnt.lfWeight);
		fwprintf(f,L"\nFont width for command buttons =%d", cmdBtnFnt.lfWidth);
		for(int p=0; p<MAX_PANELS; p++)
		{	fwprintf(f,L"\n\n%d-panel back color =%d %d %d",p+1, GetRValue(panRGBs[p][0]),GetGValue(panRGBs[p][0]), GetBValue(panRGBs[p][0]));
			fwprintf(f,L"\n%d-panel front color =%d %d %d",p+1, GetRValue(panRGBs[p][1]),GetGValue(panRGBs[p][1]), GetBValue(panRGBs[p][1]));
			fwprintf(f,L"\n%d-panel selected back color =%d %d %d",p+1, GetRValue(panRGBs[p][2]),GetGValue(panRGBs[p][2]), GetBValue(panRGBs[p][2]));
			fwprintf(f,L"\n%d-panel selected color =%d %d %d",p+1, GetRValue(panRGBs[p][3]),GetGValue(panRGBs[p][3]), GetBValue(panRGBs[p][3]));
			fwprintf(f,L"\n%d-panel hot selected color =%d %d %d",p+1, GetRValue(panRGBs[p][4]),GetGValue(panRGBs[p][4]), GetBValue(panRGBs[p][4]));
			fwprintf(f,L"\n%d-panel hot selected back color =%d %d %d",p+1, GetRValue(panRGBs[p][5]),GetGValue(panRGBs[p][5]), GetBValue(panRGBs[p][5]));
		}
		for(int p=0; p<MAX_DLG; p++)
		{	fwprintf(f,L"\n\n%d-dialog back color =%d %d %d",p+1, GetRValue(dlgRGBs[p][0]),GetGValue(dlgRGBs[p][0]), GetBValue(dlgRGBs[p][0]));
			fwprintf(f,L"\n%d-dialog front color =%d %d %d",p+1, GetRValue(dlgRGBs[p][1]),GetGValue(dlgRGBs[p][1]), GetBValue(dlgRGBs[p][1]));
			fwprintf(f,L"\n%d-dialog selected back color =%d %d %d",p+1, GetRValue(dlgRGBs[p][2]),GetGValue(dlgRGBs[p][2]), GetBValue(dlgRGBs[p][2]));
			fwprintf(f,L"\n%d-dialog selected color =%d %d %d",p+1, GetRValue(dlgRGBs[p][3]),GetGValue(dlgRGBs[p][3]), GetBValue(dlgRGBs[p][3]));
			fwprintf(f,L"\n%d-dialog hot selected color =%d %d %d",p+1, GetRValue(dlgRGBs[p][4]),GetGValue(dlgRGBs[p][4]), GetBValue(dlgRGBs[p][4]));
			fwprintf(f,L"\n%d-dialog hot selected back color =%d %d %d",p+1, GetRValue(dlgRGBs[p][5]),GetGValue(dlgRGBs[p][5]), GetBValue(dlgRGBs[p][5]));
		}
		//COLORREF menuRGB[6];
		for(int p=0; p<MAX_PANELS; p++)
		{	fwprintf(f,L"\n\n%d-panel sort buttons back color =%d %d %d",p+1, GetRValue(srtBtnRGB[p][0]),GetGValue(srtBtnRGB[p][0]), GetBValue(srtBtnRGB[p][0]));
			fwprintf(f,L"\n%d-panel sort buttons front color =%d %d %d",p+1, GetRValue(srtBtnRGB[p][1]),GetGValue(srtBtnRGB[p][1]), GetBValue(srtBtnRGB[p][1]));
			fwprintf(f,L"\n%d-panel sort buttons selected back color =%d %d %d",p+1, GetRValue(srtBtnRGB[p][2]),GetGValue(srtBtnRGB[p][2]), GetBValue(srtBtnRGB[p][2]));
			fwprintf(f,L"\n%d-panel sort buttons selected color =%d %d %d",p+1, GetRValue(srtBtnRGB[p][3]),GetGValue(srtBtnRGB[p][3]), GetBValue(srtBtnRGB[p][3]));
			fwprintf(f,L"\n%d-panel sort buttons hot selected color =%d %d %d",p+1, GetRValue(srtBtnRGB[p][4]),GetGValue(srtBtnRGB[p][4]), GetBValue(srtBtnRGB[p][4]));
			fwprintf(f,L"\n%d-panel sort buttons hot selected back color =%d %d %d",p+1, GetRValue(srtBtnRGB[p][5]),GetGValue(srtBtnRGB[p][5]), GetBValue(srtBtnRGB[p][5]));
		}
		for(int p=0; p<MAX_PANELS; p++)
		{	fwprintf(f,L"\n\n%d-panel edit controls back color =%d %d %d",p+1, GetRValue(editRGB[p][0]),GetGValue(editRGB[p][0]), GetBValue(editRGB[p][0]));
			fwprintf(f,L"\n%d-panel edit controls front color =%d %d %d",p+1, GetRValue(editRGB[p][1]),GetGValue(editRGB[p][1]), GetBValue(editRGB[p][1]));
			fwprintf(f,L"\n%d-panel edit controls selected back color =%d %d %d",p+1, GetRValue(editRGB[p][2]),GetGValue(editRGB[p][2]), GetBValue(editRGB[p][2]));
			fwprintf(f,L"\n%d-panel edit controls selected color =%d %d %d",p+1, GetRValue(editRGB[p][3]),GetGValue(editRGB[p][3]), GetBValue(editRGB[p][3]));
			fwprintf(f,L"\n%d-panel edit controls hot selected color =%d %d %d",p+1, GetRValue(editRGB[p][4]),GetGValue(editRGB[p][4]), GetBValue(editRGB[p][4]));
			fwprintf(f,L"\n%d-panel edit controls hot selected back color =%d %d %d",p+1, GetRValue(editRGB[p][5]),GetGValue(editRGB[p][5]), GetBValue(editRGB[p][5]));
		}
		fwprintf(f,L"\n\nCommand buttons back color =%d %d %d", GetRValue(cmdBtnRGB[0]),GetGValue(cmdBtnRGB[0]), GetBValue(cmdBtnRGB[0]));
		fwprintf(f,L"\nCommand buttons front color =%d %d %d", GetRValue(cmdBtnRGB[1]),GetGValue(cmdBtnRGB[1]), GetBValue(cmdBtnRGB[1]));
		fwprintf(f,L"\nCommand buttons selected back color =%d %d %d", GetRValue(cmdBtnRGB[2]),GetGValue(cmdBtnRGB[2]), GetBValue(cmdBtnRGB[2]));
		fwprintf(f,L"\nCommand buttons selected color =%d %d %d", GetRValue(cmdBtnRGB[3]),GetGValue(cmdBtnRGB[3]), GetBValue(cmdBtnRGB[3]));
		fwprintf(f,L"\nCommand buttons hot selected color =%d %d %d", GetRValue(cmdBtnRGB[4]),GetGValue(cmdBtnRGB[4]), GetBValue(cmdBtnRGB[4]));
		fwprintf(f,L"\nCommand buttons hot selected back color =%d %d %d", GetRValue(cmdBtnRGB[5]),GetGValue(cmdBtnRGB[5]), GetBValue(cmdBtnRGB[5]));
		fwprintf(f,L"\n");
		for(int k=0; k<MAX_KEYS; k++)
		{	fwprintf(f,L"\nKey[%d]-name=\"%s\";",k+1,GetKeyName(k));
			fwprintf(f,L"control pressed =%d,alt.pressed =%d,shift pressed =%d,code=%d; ",keys[k].bCtrl,keys[k].bAlt,keys[k].bShft,keys[k].key);
			fwprintf(f,L"code name=\"%s\"",GetCodeName(keys[k].key));
		}
		for(int p=0; p<MAX_PANELS; p++)
		{	fwprintf(f,L"\n\nTotal panel custom panel item definitions for panel %d =%d",p+1, iCustPanItemDefs[p]);
			for(int i=0; i<iCustPanItemDefs[p]; i++)
			{	fwprintf(f,L"\n\n %d=\"%s\"", i+1,cstmItms[p][i].nameW);

				fwprintf(f,L"\n	Custom item sort method=%d", cstmItms[p][i].sortMthd);

				fwprintf(f,L"\n	Font character set for custom item =%d", cstmItms[p][i].fnt.lfCharSet);
				fwprintf(f,L"\n	Font clip precision for custom item =%d", cstmItms[p][i].fnt.lfClipPrecision);
				fwprintf(f,L"\n	Font escapement for custom item =%d", cstmItms[p][i].fnt.lfEscapement);
				fwprintf(f,L"\n	Font face name for custom item =\"%s\"", cstmItms[p][i].fnt.lfFaceName);
				fwprintf(f,L"\n	Font height for custom item =%d", cstmItms[p][i].fnt.lfHeight);
				fwprintf(f,L"\n	Font italic for custom item =%d", cstmItms[p][i].fnt.lfItalic);
				fwprintf(f,L"\n	Font orientation for custom item =%d", cstmItms[p][i].fnt.lfOrientation);
				fwprintf(f,L"\n	Font out precision for custom item =%d", cstmItms[p][i].fnt.lfOutPrecision);
				fwprintf(f,L"\n	Font pitch and family for custom item =%d", cstmItms[p][i].fnt.lfPitchAndFamily);
				fwprintf(f,L"\n	Font quality for custom item =%d", cstmItms[p][i].fnt.lfQuality);
				fwprintf(f,L"\n	Font strike out for custom item =%d", cstmItms[p][i].fnt.lfStrikeOut);
				fwprintf(f,L"\n	Font underline for custom item =%d", cstmItms[p][i].fnt.lfUnderline);
				fwprintf(f,L"\n	Font weight for custom item =%d", cstmItms[p][i].fnt.lfWeight);
				fwprintf(f,L"\n	Font width for custom item =%d", cstmItms[p][i].fnt.lfWidth);

				fwprintf(f,L"\n	Custom item back color =%d %d %d",p+1, GetRValue(cstmItms[p][i].clrRGBs[0]),GetGValue(cstmItms[p][i].clrRGBs[0]), GetBValue(cstmItms[p][i].clrRGBs[0]));
				fwprintf(f,L"\n	Custom item front color =%d %d %d",p+1, GetRValue(cstmItms[p][i].clrRGBs[1]),GetGValue(cstmItms[p][i].clrRGBs[1]), GetBValue(cstmItms[p][i].clrRGBs[1]));
				fwprintf(f,L"\n	Custom item selected back color =%d %d %d",p+1, GetRValue(cstmItms[p][i].clrRGBs[2]),GetGValue(cstmItms[p][i].clrRGBs[2]), GetBValue(cstmItms[p][i].clrRGBs[2]));
				fwprintf(f,L"\n	Custom item selected color =%d %d %d",p+1, GetRValue(cstmItms[p][i].clrRGBs[3]),GetGValue(cstmItms[p][i].clrRGBs[3]), GetBValue(cstmItms[p][i].clrRGBs[3]));
				fwprintf(f,L"\n	Custom item hot selected color =%d %d %d",p+1, GetRValue(cstmItms[p][i].clrRGBs[4]),GetGValue(cstmItms[p][i].clrRGBs[4]), GetBValue(cstmItms[p][i].clrRGBs[4]));
				fwprintf(f,L"\n	Custom item hot selected back color =%d %d %d",p+1, GetRValue(cstmItms[p][i].clrRGBs[5]),GetGValue(cstmItms[p][i].clrRGBs[5]), GetBValue(cstmItms[p][i].clrRGBs[5]));

				/* shifted to search plugin:
				fwprintf(f,L"\n	Custom item attrib:alter.name =\"%s\"", cstmItms[p][i].itm.altName);
				fwprintf(f,L"\n	Custom item attrib:unicode alter.name =\"%s\"", cstmItms[p][i].itm.altName);
				fwprintf(f,L"\n	Custom item attrib:after create time =%d", cstmItms[p][i].itm.bCrTimeAft);
				fwprintf(f,L"\n	Custom item attrib:before create time =%d", cstmItms[p][i].itm.bCrTimeBef);
				fwprintf(f,L"\n	Custom item attrib:between create time =%d", cstmItms[p][i].itm.bCrTimeBet);
				fwprintf(f,L"\n	Custom item attrib:archive =%d", cstmItms[p][i].itm.bFileAttArchive);
				fwprintf(f,L"\n	Custom item attrib:compression =%d", cstmItms[p][i].itm.bFileAttCompr);
				fwprintf(f,L"\n	Custom item attrib:device =%d", cstmItms[p][i].itm.bFileAttDevice);
				fwprintf(f,L"\n	Custom item attrib:directory =%d", cstmItms[p][i].itm.bFileAttDir);
				fwprintf(f,L"\n	Custom item attrib:encryption =%d", cstmItms[p][i].itm.bFileAttEncr);
				fwprintf(f,L"\n	Custom item attrib:hidden =%d", cstmItms[p][i].itm.bFileAttHidden);
				fwprintf(f,L"\n	Custom item attrib:normal =%d", cstmItms[p][i].itm.bFileAttNormal);
				fwprintf(f,L"\n	Custom item attrib:not ind. =%d", cstmItms[p][i].itm.bFileAttNotInd);
				fwprintf(f,L"\n	Custom item attrib:offline =%d", cstmItms[p][i].itm.bFileAttOffl);
				fwprintf(f,L"\n	Custom item attrib: =%d", cstmItms[p][i].itm.bFileAttr);
				fwprintf(f,L"\n	Custom item attrib:read only =%d", cstmItms[p][i].itm.bFileAttReadOnly);
				fwprintf(f,L"\n	Custom item attrib:reparse point =%d", cstmItms[p][i].itm.bFileAttRepPt);
				fwprintf(f,L"\n	Custom item attrib:sparse file =%d", cstmItms[p][i].itm.bFileAttSparseFile);
				fwprintf(f,L"\n	Custom item attrib:system =%d", cstmItms[p][i].itm.bFileAttSys);
				fwprintf(f,L"\n	Custom item attrib:temporary =%d", cstmItms[p][i].itm.bFileAttTemp);
				fwprintf(f,L"\n	Custom item attrib:virtual =%d", cstmItms[p][i].itm.bFileAttVirt);
				fwprintf(f,L"\n	Custom item attrib:size =%d", cstmItms[p][i].itm.bFileSz);
				fwprintf(f,L"\n	Custom item attrib:find for alt.name =%d", cstmItms[p][i].itm.bFindForAlterName);
				fwprintf(f,L"\n	Custom item attrib:find for exclude text =%d", cstmItms[p][i].itm.bFindForExcldText);
				fwprintf(f,L"\n	Custom item attrib:find for exclude text ASCII =%d", cstmItms[p][i].itm.bFindForExcldText_ASCII);
				fwprintf(f,L"\n	Custom item attrib:find for exclude text Unicode =%d", cstmItms[p][i].itm.bFindForExcldText_Unicode);
				fwprintf(f,L"\n	Custom item attrib:find for exclude text Upper case =%d", cstmItms[p][i].itm.bFindForExcldText_UpperCase);
				fwprintf(f,L"\n	Custom item attrib:find for text =%d", cstmItms[p][i].itm.bFindForText);
				fwprintf(f,L"\n	Custom item attrib:find for text ASCII =%d", cstmItms[p][i].itm.bFindForText_ASCII);
				fwprintf(f,L"\n	Custom item attrib:find for text Unicode =%d", cstmItms[p][i].itm.bFindForText_Unicode);
				fwprintf(f,L"\n	Custom item attrib:find for text Upper case =%d", cstmItms[p][i].itm.bFindForText_UpperCase);
				fwprintf(f,L"\n	Custom item attrib:between last access time =%d", cstmItms[p][i].itm.bLastAccTimeBet);
				fwprintf(f,L"\n	Custom item attrib:after last access time =%d", cstmItms[p][i].itm.bLstAccTimeAft);
				fwprintf(f,L"\n	Custom item attrib:before last access time =%d", cstmItms[p][i].itm.bLstAccTimeBef);
				fwprintf(f,L"\n	Custom item attrib:after last write time =%d", cstmItms[p][i].itm.bLstWrTimeAft);
				fwprintf(f,L"\n	Custom item attrib:before last write time =%d", cstmItms[p][i].itm.bLstWrTimeBef);
				fwprintf(f,L"\n	Custom item attrib:between last write time =%d", cstmItms[p][i].itm.bLstWrTimeBet);
				fwprintf(f,L"\n	Custom item attrib:after create time =%d, =%d", cstmItms[p][i].itm.CrTimeAft.dwLowDateTime,cstmItms[p][i].itm.CrTimeAft.dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:before create time =%d, =%d", cstmItms[p][i].itm.CrTimeBef.dwLowDateTime,cstmItms[p][i].itm.CrTimeBef.dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:between create time =%d, =%d, =%d, =%d", cstmItms[p][i].itm.CrTimeBet[0].dwLowDateTime,cstmItms[p][i].itm.CrTimeBet[0].dwHighDateTime,cstmItms[p][i].itm.CrTimeBet[1].dwLowDateTime,cstmItms[p][i].itm.CrTimeBet[1].dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:filter =\"%s\"", cstmItms[p][i].itm.filtr);
				fwprintf(f,L"\n	Custom item attrib:exclude text =\"%s\"", cstmItms[p][i].itm.FindForExcldText);
				fwprintf(f,L"\n	Custom item attrib:exclude text length =%d", cstmItms[p][i].itm.FindForExcldTextLn);
				fwprintf(f,L"\n	Custom item attrib:find for text =\"%s\"", cstmItms[p][i].itm.FindForText);
				fwprintf(f,L"\n	Custom item attrib:find for text length =%d", cstmItms[p][i].itm.FindForTextLn);
				fwprintf(f,L"\n	Custom item attrib:file size equation =%d", cstmItms[p][i].itm.iFileSzQual);
				fwprintf(f,L"\n	Custom item attrib:between last access time =%d, =%d, =%d, =%d", cstmItms[p][i].itm.LastAccTimeBet[0].dwLowDateTime,cstmItms[p][i].itm.LastAccTimeBet[0].dwHighDateTime,cstmItms[p][i].itm.LastAccTimeBet[1].dwLowDateTime,cstmItms[p][i].itm.LastAccTimeBet[1].dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:after last acces time =%d", cstmItms[p][i].itm.LstAccTimeAft.dwLowDateTime,cstmItms[p][i].itm.LstAccTimeAft.dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:before last acces time =%d", cstmItms[p][i].itm.LstAccTimeBef.dwLowDateTime,cstmItms[p][i].itm.LstAccTimeBef.dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:after last write time =%d", cstmItms[p][i].itm.LstWrTimeAft.dwLowDateTime,cstmItms[p][i].itm.LstWrTimeAft.dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:before last write time =%d", cstmItms[p][i].itm.LstWrTimeBef.dwLowDateTime,cstmItms[p][i].itm.LstWrTimeBef.dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:between last write time =%d, =%d, =%d, =%d", cstmItms[p][i].itm.LstWrTimeBet[0].dwLowDateTime,cstmItms[p][i].itm.LstWrTimeBet[0].dwHighDateTime,cstmItms[p][i].itm.LstWrTimeBet[1].dwLowDateTime,cstmItms[p][i].itm.LstWrTimeBet[1].dwHighDateTime);
				fwprintf(f,L"\n	Custom item attrib:root path =\"%s\"", cstmItms[p][i].itm.rootPath);
				fwprintf(f,L"\n	Custom item attrib:file size =\"%s\"", cstmItms[p][i].itm.sFileSz);
				fwprintf(f,L"\n	Custom item attrib:file size equation =\"%s\"", cstmItms[p][i].itm.sFileSzEqu);
				*/
		}	}
		fwprintf(f,L"\nSTD view for buttons: %d", iBtnsType);
		fclose(f);
}	}

VOID DLG::SetColor(int colNum, int dlgNum, COLORREF c)
{
	switch(dlgNum)
	{	case 24://panel[0]
			panRGBs[0][colNum] = c;break;
		case 25://panel[1]
			panRGBs[1][colNum] = c;break;
		case 26://panel[2]
			panRGBs[2][colNum] = c;break;
		case 27://panel[3]
			panRGBs[3][colNum] = c;break;
		case 28:
			dlgRGBs[0][colNum] = c;break;
		case 29:
			dlgRGBs[1][colNum] = c;break;
		case 30:
			dlgRGBs[2][colNum] = c;break;
		case 31:
			dlgRGBs[3][colNum] = c;break;
		case 32:
			dlgRGBs[4][colNum] = c;break;
		case 33:
			dlgRGBs[5][colNum] = c;break;
		case 34:
			dlgRGBs[6][colNum] = c;break;
		case 35:
			dlgRGBs[7][colNum] = c;break;
		case 36:
			dlgRGBs[8][colNum] = c;break;
		case 37:
			menuRGB[colNum] = c;break;
		case 38:
			srtBtnRGB[0][colNum] = c;break;
		case 39:
			srtBtnRGB[1][colNum] = c;break;
		case 40:
			srtBtnRGB[2][colNum] = c;break;
		case 41:
			srtBtnRGB[3][colNum] = c;break;
		case 42:
			editRGB[0][colNum] = c;break;
		case 43:
			editRGB[1][colNum] = c;break;
		case 44:
			editRGB[2][colNum] = c;break;
		case 45:
			editRGB[3][colNum] = c;break;
		case 46:
			cmdBtnRGB[colNum] = c;break;
}	}

VOID ZeroLOGFONT(LOGFONT& lf)
{
	lf.lfCharSet=0;
	lf.lfClipPrecision=0;
	lf.lfEscapement=0;
	lf.lfFaceName[0]=0;
	lf.lfHeight=0;
	lf.lfItalic=0;
	lf.lfOrientation=0;
	lf.lfOutPrecision=0;
	lf.lfPitchAndFamily=0;
	lf.lfQuality=0;
	lf.lfStrikeOut=0;
	lf.lfUnderline=0;
	lf.lfWeight=0;
	lf.lfWidth=0;
}

BOOL ChangeTotPanelsNum(int newNum)
{
	switch(newNum)
	{	case 1: break;
		default: case 2:
			resizeBarPersnt[0] = 50.0f;
		break;
		case 3:
			resizeBarPersnt[0] = 33.33f;
			resizeBarPersnt[1] = 66.66f;
		break;
		case 4:
			resizeBarPersnt[0] = 25.0f;
			resizeBarPersnt[1] = 50.0f;
			resizeBarPersnt[2] = 75.0f;
		break;
	}
	Dlg.iTotPanels = newNum;
	for(int i=0; i<Dlg.iTotPanels; ++i)
		panel[i].SetOpponent();
	return TRUE;
}

static BOOL CALLBACK EnumCheckPrevWindInsProc(HWND hwnd,LPARAM lParam)
{
wchar_t s[MAX_PATH]=L"";
int* iIns = (int*)lParam;
	GetWindowModuleFileName(hwnd,s,MAX_PATH);
	if(wcsstr(s,L".exe"))
	if(wcsstr(s,L"Sino"))
	{	if(!GetWindowText(hwnd,s,MAX_PATH))
			return TRUE;
		if(!wcscmp(s,L"Sino"))
		{	(*iIns)++;
			return TRUE;
	}	}
	return TRUE;
}

//Programmaning nechta nusxasi borligini aniqlab, cfg bo'yicha cheklaydi.
BOOL CheckPrevWindIns()
{
	if(Dlg.iNumInstans>10)return TRUE;
int iIns=0;

	//HMODULE hm = LoadLibrary("Psapi.dll");
	//if(!hm)
	{	if(!EnumWindows(EnumCheckPrevWindInsProc,(LPARAM)&iIns))
			return TRUE;
		return (Dlg.iNumInstans>iIns?TRUE:FALSE);
	}

	//FreeLibrary(hm);

	//Quyidagi metod ham ishlaydi(#include <Psapi.h> ni va libini qo'sh)
/*DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    if(!EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded))
        return TRUE;

	//Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    //Find my processes:
    for(i = 0; i<cProcesses; i++)
	{if(aProcesses[i] != 0)
	 {TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	  // Get a handle to the process.
      HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, aProcesses[i]);
	  //Get the process name.
      if(NULL != hProcess)
      {HMODULE hMod;DWORD cbNeeded;
       if(EnumProcessModules(hProcess,&hMod,sizeof(hMod),&cbNeeded))
       {GetModuleBaseName(hProcess,hMod,szProcessName,
				sizeof(szProcessName)/sizeof(TCHAR));
     } }
     //Check:
#ifdef _DEBUG
     if(!strcmp(szProcessName,"SinoDbg.exe"))
#else
     if(!strcmp(szProcessName,"SinoRel.exe"))
#endif
		++iIns;
     CloseHandle( hProcess );	 
	}}
	return (Dlg.iNumInstans+1>iIns?TRUE:FALSE);*/
}

int oldBtnPanelType=0;
INT_PTR CALLBACK CmnConfDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);
		wchar_t st[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_119,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC,st);
		LoadString(hInst,IDS_STRINGSW_120,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC1,st);
		LoadString(hInst,IDS_STRINGSW_121,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC2,st);
		LoadString(hInst,IDS_STRINGSW_122,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC3,st);
		LoadString(hInst,IDS_STRINGSW_20,st,MAX_PATH);//Temporary folder path:
		SetDlgItemText(hDlg,IDC_STATIC5,st);
		LoadString(hInst,IDS_STRINGSW_45,st,MAX_PATH);//Browse:
		SetDlgItemText(hDlg,IDC_BUTTON_TEMPORARY_PATH_BROWSE,st);
		SetDlgItemText(hDlg,IDC_EDIT_TEMPORARY_PATH,conf::tmpPath);
		LoadString(hInst,IDS_STRINGSW_44,st,MAX_PATH);//panel:
		SetDlgItemText(hDlg,IDC_STATIC8,st);
		LoadString(hInst,IDS_STRINGSW_337,st,MAX_PATH);//Panels configurations:
		SetDlgItemText(hDlg,IDC_STATIC9,st);
		LoadString(hInst,IDS_STRINGSW_338,st,MAX_PATH);//For each:
		SetDlgItemText(hDlg,IDC_STATIC7,st);
		LoadString(hInst,IDS_STRINGSW_339,st,MAX_PATH);//Disk space prompt
		SetDlgItemText(hDlg,IDC_CHECK_DISK_SP_PRMT,st);
		LoadString(hInst,IDS_STRINGSW_340,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_HIDE_HIDDEN,st);
					
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_PANELS_CONFIG),CB_ADDSTRING,0,(LPARAM)L"1");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_PANELS_CONFIG),CB_ADDSTRING,0,(LPARAM)L"2");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_PANELS_CONFIG),CB_ADDSTRING,0,(LPARAM)L"3");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_PANELS_CONFIG),CB_ADDSTRING,0,(LPARAM)L"4");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_PANELS_CONFIG),CB_SETCURSEL,conf::Dlg.iTotPanels-1,0);

		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BTN_PANEL_TYPE),CB_ADDSTRING,0,(LPARAM)L"Fixed(new style actions bar)");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BTN_PANEL_TYPE),CB_ADDSTRING,0,(LPARAM)L"Floating(old style)");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BTN_PANEL_TYPE),CB_ADDSTRING,0,(LPARAM)L"Not used");
		if(0==conf::Dlg.bBtnsPanelType)//struct 32 dagi bitpole bo'lgani uchun ifladim;
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_BTN_PANEL_TYPE),CB_SETCURSEL,0,0);
		else if(1==conf::Dlg.bBtnsPanelType)
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_BTN_PANEL_TYPE),CB_SETCURSEL,1,0);
		else SendMessage(GetDlgItem(hDlg,IDC_COMBO_BTN_PANEL_TYPE),CB_SETCURSEL,2,0);

		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_ADDSTRING,0,(LPARAM)L"1");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_ADDSTRING,0,(LPARAM)L"2");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_ADDSTRING,0,(LPARAM)L"3");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_ADDSTRING,0,(LPARAM)L"4");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_SETCURSEL,0,0);

		SendMessage(GetDlgItem(hDlg,IDC_CHECK_DISK_SP_PRMT),BM_SETCHECK,
					conf::Dlg.bShowDiskSpace[0]==1?BST_CHECKED:BST_UNCHECKED,0);
		SendMessage(GetDlgItem(hDlg,IDC_CHECK_HIDE_HIDDEN),BM_SETCHECK,
					1==panel[0].bHideHiddenFiles?BST_CHECKED:BST_UNCHECKED,0);
		SendMessage(GetDlgItem(hDlg,IDC_CHECK1),BM_SETCHECK,
					1==conf::Dlg.bShowBtmPnl?BST_CHECKED:BST_UNCHECKED,0);

	  /*SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"12");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"16");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"24");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"32");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"48");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_SETCURSEL,3,0);*/

		LoadString(hInst,IDS_STRINGSW_123,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_START_POS_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"From save position");
		LoadString(hInst,IDS_STRINGSW_124,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_START_POS_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"Center screen");
		LoadString(hInst,IDS_STRINGSW_125,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_START_POS_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"Random position");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_START_POS_CONFIG),CB_SETCURSEL,conf::Dlg.iStartPosition,0);

		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"1");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"2");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"3");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"4");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"5");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"6");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"7");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"8");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"9");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)L"10");
		LoadString(hInst,IDS_STRINGSW_160,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"Unlimited");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_SETCURSEL,conf::Dlg.iNumInstans-1,0);

		LoadString(hInst,IDS_STRINGSW_126,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_HIDE_METHOD_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"In the task panel");
		LoadString(hInst,IDS_STRINGSW_127,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_HIDE_METHOD_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"In the system tray");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_HIDE_METHOD_CONFIG),CB_SETCURSEL,conf::Dlg.iHideMethod,0);

		LoadString(hInst,IDS_STRINGSW_165,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC4,st);
		LoadString(hInst,IDS_STRINGSW_166,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOLDER_LIST_METHOD_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"Via <FindFirstFile>");
		LoadString(hInst,IDS_STRINGSW_167,st,MAX_PATH);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOLDER_LIST_METHOD_CONFIG),CB_ADDSTRING,0,(LPARAM)st);//"Via Kernel");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOLDER_LIST_METHOD_CONFIG),CB_SETCURSEL,conf::Dlg.iEnumMethod,0);

		oldBtnPanelType = conf::Dlg.bBtnsPanelType;

		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDC_COMBO_FOLDER_LIST_METHOD_CONFIG:
				if(CBN_SELCHANGE==HIWORD(wParam))
					Dlg.iEnumMethod=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOLDER_LIST_METHOD_CONFIG),CB_GETCURSEL,0,0);
				return 0;
			case IDC_COMBO_TOT_INSTANCE_CONFIG:
				if(CBN_SELCHANGE==HIWORD(wParam))
					Dlg.iNumInstans = 1+(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_INSTANCE_CONFIG),CB_GETCURSEL,0,0);
				return 0;
			case IDC_COMBO_TOT_PANELS_CONFIG:
				if(CBN_SELCHANGE==HIWORD(wParam))
				{	if(QView::IsExistAny() || vrtlPanels::IsExistAny())
					{	MessageBox(hWnd,L"Close qView or virtual panel first,please",L"Warn.",MB_OK);
						return 0;
					}					
					for(int i=0; i<Dlg.iTotPanels; i++)
					{	MyStringCpy(panel_path[i],MAX_PATH-1,panel[i].GetPath());
						panel[i].GetEntry()->Reset();
					}
					for(int i=0; i<Dlg.iTotPanels; i++)
					{	panel[i].folderAndFileMixingSortType=(FolderAndFileMixingSortType)folderAndFileMixingSortType[i];
						panel[i].folderSortType=(SortType)folderSortType[i];
						panel[i].fileSortType=(SortType)fileSortType[i];
						if(!IsDirExist(panel_path[i]))
							MyStringCpy(panel_path[i],MAX_PATH-1,MyStringAddModulePath(L"*"));
					}
					CmnCntrl::DestroyControls(hWnd);
					ChangeTotPanelsNum(1+(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_TOT_PANELS_CONFIG),CB_GETCURSEL,0,0));
					CmnCntrl::CreateControls(hWnd,TRUE);
					for(int i=0; i<Dlg.iTotPanels; i++)
						panel[i].AdjustScrollity();
				}
				return 0;
			case IDC_COMBO_START_POS_CONFIG:
				if(CBN_SELCHANGE==HIWORD(wParam))
					Dlg.iStartPosition = (int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_START_POS_CONFIG),CB_GETCURSEL,0,0);
				return 0;
			case IDC_COMBO_HIDE_METHOD_CONFIG:
				if(CBN_SELCHANGE==HIWORD(wParam))
					Dlg.iHideMethod = (int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_HIDE_METHOD_CONFIG),CB_GETCURSEL,0,0);
				return 0;
			case IDC_BUTTON_TEMPORARY_PATH_BROWSE:				
				BROWSEINFOW bi; 
				bi.hwndOwner = hDlg;
				bi.pszDisplayName = st;
				bi.lpszTitle = L"Browse for temporary folder path ...";
				bi.ulFlags = BIF_NONEWFOLDERBUTTON|BIF_DONTGOBELOWDOMAIN|BIF_NEWDIALOGSTYLE|
					BIF_NOTRANSLATETARGETS|BIF_RETURNFSANCESTORS;
				bi.lpfn = NULL;
				LPITEMIDLIST pidlRoot;pidlRoot = NULL;
				bi.pidlRoot = pidlRoot;
				LPITEMIDLIST pidlSelected;pidlSelected = SHBrowseForFolder(&bi);
				if(pidlRoot)
					CoTaskMemFree(pidlRoot);
				if(pidlSelected)
				{	SHGetPathFromIDList(pidlSelected,st);
					//MyStringCat(st,MAX_PATH,"\\");//"\\*"
					SetDlgItemText(hDlg,IDC_EDIT_TEMPORARY_PATH,st);
					CoTaskMemFree(pidlSelected);
				}
				return 0;
			case IDC_EDIT_TEMPORARY_PATH:
				if(EN_CHANGE==HIWORD(wParam))
				{	GetDlgItemText(hDlg,IDC_EDIT_TEMPORARY_PATH,st,MAX_PATH-1);
					if(!IsDirExist(st))CreateAllDirs(st);
					MyStringCpy(conf::tmpPath,MAX_PATH-1,st);
				}
				return 0;
			case IDC_COMBO_BTN_PANEL_TYPE:
				int c;c = (int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_BTN_PANEL_TYPE),CB_GETCURSEL,0,0);
				if(CB_ERR!=c)//bit pole ligi uchun ifladim;
				{	if(0==c)conf::Dlg.bBtnsPanelType=0;
					else if(1==c)conf::Dlg.bBtnsPanelType=1;
					else conf::Dlg.bBtnsPanelType=2;
				}
				return 0;
			case IDC_COMBO_FOR_EACH_PANEL:
				c=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_GETCURSEL,0,0);
				if(CB_ERR!=c)
				{	SendMessage(GetDlgItem(hDlg,IDC_CHECK_DISK_SP_PRMT),BM_SETCHECK,
					conf::Dlg.bShowDiskSpace[c] ? BST_CHECKED : BST_UNCHECKED,0);
					SendMessage(GetDlgItem(hDlg,IDC_CHECK_HIDE_HIDDEN),BM_SETCHECK,
								1==panel[c].bHideHiddenFiles ? BST_CHECKED : BST_UNCHECKED,0);
				}
				return 0;
			case IDC_CHECK_DISK_SP_PRMT:
				c=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_GETCURSEL,0,0);
				if(CB_ERR!=c)
					conf::Dlg.bShowDiskSpace[c] =
						(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_DISK_SP_PRMT),BM_GETCHECK,0,0)) ? 1 : 0;
				return 0;
			case IDC_CHECK_HIDE_HIDDEN:
				c=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_FOR_EACH_PANEL),CB_GETCURSEL,0,0);
				if(CB_ERR!=c)
					panel[c].bHideHiddenFiles =
						(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_HIDE_HIDDEN),BM_GETCHECK,0,0)) ? 1 : 0;
				return 0;
			case IDC_CHECK1:
				conf::Dlg.bShowBtmPnl=(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK1),BM_GETCHECK,0,0)) ? 1 : 0;
				return 0;
		}
		return 0;
	case WM_DESTROY:
//		Dlg.ApplyToSystem(Dlg.iTotPanels);
		if(1==conf::Dlg.bBtnsPanelType && 1!=oldBtnPanelType)
			topBar.Destroy();
		else if(0==conf::Dlg.bBtnsPanelType && 0!=oldBtnPanelType)
			topBar.Create(::hWnd,0,0,rcClient.right,102);
		CmnCntrl::ResizeControls();
		RedrawWindow(::hWnd,NULL,NULL,RDW_INVALIDATE);
//		CmnCntrl::DestroyControls(::hWnd);
//		CmnCntrl::CreateControls(::hWnd,TRUE);
		return 0;
	}
	return (INT_PTR)FALSE;
}

UINT_PTR CALLBACK CCHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uiMsg)
	{	case WM_COMMAND:
			if(EN_CHANGE==HIWORD(wParam))
			switch(LOWORD(wParam))
			{	case 703://Оттенок
					return 0;
				case 704://Контрасть
					return 0;
				case 705://Яркость
					return 0;
				case 706://R edit
					char s[8];GetDlgItemTextA(hdlg,LOWORD(wParam),s,8);
					SendMessage(stdhDlg[chlds[Chld::iSelectedDlg].iWnd],WM_USER+5,0,atoi(s));
					return 0;
				case 707://G edit
					GetDlgItemTextA(hdlg,LOWORD(wParam),s,8);
					SendMessage(stdhDlg[chlds[Chld::iSelectedDlg].iWnd],WM_USER+5,1,atoi(s));
					return 0;
				case 708://B edit
					GetDlgItemTextA(hdlg,LOWORD(wParam),s,8);
					SendMessage(stdhDlg[chlds[Chld::iSelectedDlg].iWnd],WM_USER+5,2,atoi(s));
					return 0;
			}
			return 0;
	}
	return 0;//Pass to default;
}

VOID ShowChooseColor(HWND hDlg,int cNum)
{	DWORD rgbStore[6];for(int c=0; c<6; c++)rgbStore[c]=Dlg.GetColor(c,Chld::iSelectedDlg);
	COLORREF acrCustClr[16]; 
	CHOOSECOLOR chc;ZeroMemory(&chc,sizeof(chc));
	chc.lStructSize=sizeof(chc);chc.hwndOwner=hDlg;
	chc.lpCustColors = (LPDWORD)acrCustClr;
	chc.rgbResult = rgbStore[cNum];chc.lpfnHook = CCHookProc;
	chc.Flags = CC_FULLOPEN|CC_RGBINIT|CC_ENABLEHOOK;

	switch(Chld::iSelectedDlg)
	{case 28://CopyDlg:
	  hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_MASTER),hWnd,fCopyOper::CopyDlgProc,0);
	  ShowWindow(hTmpDlg,SW_SHOW);					
	  break;
	 case 29://Copy queue:
	  hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_QUEUE),hWnd,fCopyOper::CopyQueueDlgProc,0);
	  ShowWindow(hTmpDlg,SW_SHOW);					
	  break;
	 case 30://Background queue:
	  hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_BACKGROUND_QUEUE),hWnd,fBckgrndCopyOper::BckgrndCopyDlgProc,0);
	  ShowWindow(hTmpDlg,SW_SHOW);					
	  break;
	 case 31://Create folder:
	  hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_CREA_FOLDER),hWnd,CreaFolder::DlgProc,0);
	  ShowWindow(hTmpDlg,SW_SHOW);					
	  break;
	 case 33://Change font:
	  hTmpDlg=hDlg;
	  break;
	 case 34://Search:
	  hTmpDlg=hDlg;//CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_SEACH_VIA_F7),hWnd,fSearchViaF7::FindFileDlgProc,0);
	  //ShowWindow(hTmpDlg,SW_SHOW);					
	  break;
	 case 35://Disk select:
	  hTmpDlg=hDlg;//CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_SELECT_DISKS),hWnd,fSearchViaF7::AddDiskDlgProc,0);
	  //ShowWindow(hTmpDlg,SW_SHOW);					
	 break;
	 case 36://Other:
	  hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_LANGUAGE),hWnd,language::DlgProc,0);
	  ShowWindow(hTmpDlg,SW_SHOW);					
	 break;
	}

	chlds[Chld::iSelectedDlg].oldColor[cNum] = rgbStore[cNum];
	if(!ChooseColor(&chc))
	{	Dlg.SetColor(cNum,Chld::iSelectedDlg,rgbStore[cNum]);
		chlds[Chld::iSelectedDlg].SetDestColor(0,rgbStore);
		PostMessage(hDlg,WM_PAINT,0,0);
	}
	if(hTmpDlg)
	if(33!=Chld::iSelectedDlg)//Change font:
	{	DestroyWindow(hTmpDlg);
		hTmpDlg = 0;
}	}

INT_PTR CALLBACK ClrConfDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
static int numbSelctdBtn=-1;
static HBRUSH hBrshWM_CTLCOLORSTATIC=NULL;
static HFONT hf=0;
static HBRUSH br=0;
static HBRUSH brHtBk=0;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_CTLCOLORSTATIC:
		if((HWND)lParam!=GetDlgItem(hDlg,IDC_STATIC))
		if((HWND)lParam!=GetDlgItem(hDlg,IDC_STATIC1))
		if((HWND)lParam!=GetDlgItem(hDlg,IDC_STATIC2))
			return DefWindowProc(hDlg,message,wParam,lParam);

		SetTextColor((HDC)wParam,Dlg.GetColor(1,Chld::iSelectedDlg));
		SetBkColor((HDC)wParam,Dlg.GetColor(2,Chld::iSelectedDlg));

		if(hBrshWM_CTLCOLORSTATIC)
			DeleteObject(hBrshWM_CTLCOLORSTATIC);
		hBrshWM_CTLCOLORSTATIC = CreateSolidBrush(Dlg.GetColor(2,Chld::iSelectedDlg));
		HFONT HF;HF = CreateFontIndirect(Chld::GetColorFont());
		SelectObject((HDC)wParam,HF);
		DeleteObject(HF);
		return (INT_PTR)hBrshWM_CTLCOLORSTATIC;
	case WM_INITDIALOG:RECT rc;wchar_t st[MAX_PATH];
		HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);		
		LoadString(hInst,IDS_STRINGSW_128,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC6,st);
		LoadString(hInst,IDS_STRINGSW_129,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC5,st);
		LoadString(hInst,IDS_STRINGSW_130,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC4,st);
		LoadString(hInst,IDS_STRINGSW_114,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG,st);		 
		SetDlgItemText(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG2,st);		 
		SetDlgItemText(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG3,st);		 
		LoadString(hInst,IDS_STRINGSW_131,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG,st);		 
		LoadString(hInst,IDS_STRINGSW_161,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC8,st);
		LoadString(hInst,IDS_STRINGSW_162,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC9,st);
		LoadString(hInst,IDS_STRINGSW_164,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC7,st);
		LoadString(hInst,IDS_STRINGSW_291,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_ANIM_BCKIMG_TIME,st);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCKGRND_IMG_TYPE),CB_ADDSTRING,0,(LPARAM)L"full");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCKGRND_IMG_TYPE),CB_ADDSTRING,0,(LPARAM)L"one to all panels");//cont.from prev.");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCKGRND_IMG_TYPE),CB_ADDSTRING,0,(LPARAM)L"not used");
		SendMessage(hDlg,WM_USER+2,0,0);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDC_CHECK_ANIM_BCKIMG_TIME:long bch;
				bch=(int)SendMessage(GetDlgItem(hDlg,IDC_CHECK_ANIM_BCKIMG_TIME),BM_GETCHECK,0,0);
				EnableWindow(GetDlgItem(hDlg,IDC_EDIT_BCKING_ANIM_TIME),BST_CHECKED==bch);
				if(BST_CHECKED!=bch)
				{	conf::Dlg.panBckgrndImgAnimTime[Chld::iSelectedDlg-24]=0;
					SetDlgItemText(hDlg,IDC_EDIT_BCKGRND_IMG,L"");
				}
				break;
			case IDC_EDIT_BCKING_ANIM_TIME:
				if(EN_KILLFOCUS ==HIWORD(wParam))
				{	GetDlgItemText(hDlg,IDC_EDIT_BCKING_ANIM_TIME,st,MAX_PATH);
					bch=_wtoi(st);
					conf::Dlg.panBckgrndImgAnimTime[Chld::iSelectedDlg-24]=(bch==-1?0xffffffff:bch);
					panel[Chld::iSelectedDlg-24].imgBckgrnd[1].Load(&panel[Chld::iSelectedDlg-24],NULL);
					panel[Chld::iSelectedDlg-24].imgBckgrnd[1].CloneOnWM_SIZE(
											&panel[Chld::iSelectedDlg-24],
											&panel[Chld::iSelectedDlg-24].imgBckgrnd[0]);
				}
				break;
			case IDC_BUTTON_CHANGE_COLOR_CONFIG: //Back
				numbSelctdBtn=0;
				ShowChooseColor(hDlg,0);
				break;
			case IDC_BUTTON_CHANGE_COLOR_CONFIG2://Front
				numbSelctdBtn=1;
				ShowChooseColor(hDlg,1);
				break;
			case IDC_BUTTON_CHANGE_COLOR_CONFIG3://Selected back
				numbSelctdBtn=2;
				ShowChooseColor(hDlg,2);
				break;
			case IDC_BUTTON_CHANGE_COLOR_CONFIG4://Select
				numbSelctdBtn=3;
				ShowChooseColor(hDlg,3);
				break;
			case IDC_BUTTON_CHANGE_COLOR_CONFIG5://Hot
				numbSelctdBtn=4;
				ShowChooseColor(hDlg,4);
				break;
			case IDC_BUTTON_CHANGE_COLOR_CONFIG6://Hot back
				numbSelctdBtn=5;
				ShowChooseColor(hDlg,5);
				break;
			case IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG:
				int iFrom;iFrom = Chld::iSelectedDlg>37?37:(Chld::iSelectedDlg>28?28:24);
				for(int j=0; j<6; j++)
					Dlg.SetColor(j,Chld::iSelectedDlg,Dlg.GetColor(j,iFrom));
				PostMessage(hDlg,WM_PAINT,0,0);
				break;
			case IDC_BUTTON_BCKGRND_IMG_BROWSE:
				OPENFILENAME of;
				memset(&of, 0, sizeof(OPENFILENAME));
				of.lStructSize      = sizeof(OPENFILENAME);//OPENFILENAME_SIZE_VERSION_400;
				of.hInstance        = hInst;
				of.hwndOwner        = hDlg;
				of.lpstrFilter      = L"*jpg;*jpeg;*.png;*.bmp\0*jpg;*jpeg;*.png;*.bmp\0*.jpg;\0*.jpg;\0*.jpeg;\0*.jpeg;\0*.png;\0*.png;\0*.bmp;\0";
				st[0]=st[1]=0;of.lpstrFile= st;
				of.lpstrTitle       = L"Panel background image,cancel-shift to background color mode.";
				of.lpstrFileTitle   = L"Configuration.";
				of.Flags            = OFN_EXPLORER|OFN_FILEMUSTEXIST;
				of.nMaxFile			= MAX_PATH;
				//of.lpstrInitialDir  = panel[0].GetPath();
				if(GetOpenFileName(&of))
				{	SetDlgItemText(hDlg,IDC_EDIT_BCKGRND_IMG,st);
					//if(conf::Dlg.panBckgrndImgType[Chld::iSelectedDlg-24]<2)
					 if(2==conf::Dlg.panBckgrndImgType[Chld::iSelectedDlg-24])
					 {SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCKGRND_IMG_TYPE),CB_SETCURSEL,0,0);
					  conf::Dlg.panBckgrndImgType[Chld::iSelectedDlg-24]=0;
					 }
					 panel[Chld::iSelectedDlg-24].imgBckgrnd[1].Load(&panel[Chld::iSelectedDlg-24],st);
				     panel[Chld::iSelectedDlg-24].imgBckgrnd[1].CloneOnWM_SIZE(
											&panel[Chld::iSelectedDlg-24],
											&panel[Chld::iSelectedDlg-24].imgBckgrnd[0]);
					 if(0==Chld::iSelectedDlg-24 && 1==Dlg.panBckgrndImgType[0])
					 {for(int p=1; p<conf::Dlg.iTotPanels; ++p)
					  {panel[p].imgBckgrnd[1].CloneOnWM_SIZE(&panel[p],&panel[p].imgBckgrnd[0]);
					   panel[p].Render();				  
				}	} }
				else 
				{	SetDlgItemText(hDlg,IDC_EDIT_BCKGRND_IMG,L"");
					panel[Chld::iSelectedDlg-24].imgBckgrnd[0].Free();
					panel[Chld::iSelectedDlg-24].imgBckgrnd[1].Free();
					if(24==Chld::iSelectedDlg && 1==conf::Dlg.panBckgrndImgType[0])
					{	for(int p=1; p<conf::Dlg.iTotPanels; ++p)
						{	panel[p].imgBckgrnd[0].Free();
							panel[p].imgBckgrnd[1].Free();
				}	}	}
				//panel[Chld::iSelectedDlg-24].Render();
				//if(24==Chld::iSelectedDlg]
				//if(1==conf::Dlg.panBckgrndImgType[0])
				for(int p=0; p<conf::Dlg.iTotPanels; ++p)
				 panel[p].Render();
			break;
			case IDC_COMBO_BCKGRND_IMG_TYPE:
				if(CBN_SELCHANGE==HIWORD(wParam))
				{Dlg.panBckgrndImgType[Chld::iSelectedDlg-24]=(int)SendMessage(
						GetDlgItem(hDlg,IDC_COMBO_BCKGRND_IMG_TYPE),CB_GETCURSEL,0,0);
				 if(2==Dlg.panBckgrndImgType[Chld::iSelectedDlg-24])
				 {Dlg.panBckgrndImgPth[Chld::iSelectedDlg-24][0]=0;
				  panel[Chld::iSelectedDlg-24].imgBckgrnd[0].Free();
				  panel[Chld::iSelectedDlg-24].imgBckgrnd[1].Free();
				  panel[Chld::iSelectedDlg-24].Render();
  				  if(0==Chld::iSelectedDlg-24)
				  {for(int p=1; p<conf::Dlg.iTotPanels; ++p)
				   {panel[p].imgBckgrnd[0].Free();
				    panel[p].imgBckgrnd[1].Free();
				    panel[p].Render();
				  }}
				  SetDlgItemText(hDlg,IDC_EDIT_BCKGRND_IMG,L"");
				 }
				 else//if(1==Dlg.panBckgrndImgType[Chld::iSelectedDlg-24])
				 {//panel[Chld::iSelectedDlg-24].imgBckgrnd[1].Load(&panel[Chld::iSelectedDlg-24],
				  //									conf::Dlg.panBckgrndImgPth[Chld::iSelectedDlg-24]);
				  panel[Chld::iSelectedDlg-24].imgBckgrnd[1].CloneOnWM_SIZE(
											&panel[Chld::iSelectedDlg-24],
											&panel[Chld::iSelectedDlg-24].imgBckgrnd[0]);
				  panel[Chld::iSelectedDlg-24].Render();
				  if(0==Chld::iSelectedDlg-24 && 1==Dlg.panBckgrndImgType[0])
				  {for(int p=1; p<conf::Dlg.iTotPanels; ++p)
				   {panel[p].imgBckgrnd[1].CloneOnWM_SIZE(&panel[p],&panel[p].imgBckgrnd[0]);
					panel[p].Render();				  
				}}}}
				break;
		}
		break;
	//case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:HDC dc;dc = (HDC)wParam;
		SetTextColor(dc,conf::Dlg.dlgRGBs[5][1]);
		SetBkColor(dc,conf::Dlg.dlgRGBs[5][2]);
		return (INT_PTR)br;
	case WM_CTLCOLOREDIT:dc = (HDC)wParam;
		SetTextColor(dc,conf::Dlg.dlgRGBs[4][1]);
		SetBkColor(dc,conf::Dlg.dlgRGBs[5][2]);
		return (INT_PTR)br;
	case WM_CTLCOLORBTN:dc=(HDC)wParam;
		SetTextColor(dc,conf::Dlg.dlgRGBs[5][4]);
		SetBkColor(dc,conf::Dlg.dlgRGBs[5][5]);
		return (INT_PTR)brHtBk;
	case WM_DRAWITEM://WM_CTLCOLORBTN dagi knopkalar:
		LPDRAWITEMSTRUCT lpdis;lpdis = (LPDRAWITEMSTRUCT)lParam;
		wchar_t s[64];GetWindowText(lpdis->hwndItem,s,64);
		UINT uStyle;uStyle = DFCS_BUTTONPUSH;
		rc = lpdis->rcItem;
		if(lpdis->itemState & ODS_SELECTED)
		{	uStyle |= DFCS_PUSHED;
			rc.left+=2;rc.top+=2;
		}
		DrawFrameControl(lpdis->hDC, &lpdis->rcItem, DFC_BUTTON, uStyle);
		if(lpdis->itemState & ODS_SELECTED)
			{rc.left+=1;rc.top+=1;rc.bottom-=2;rc.right-=3;}
		else
			{rc.left+=1;rc.top+=2;rc.bottom-=3;rc.right-=3;}
		FillRect(lpdis->hDC,&rc,brHtBk);//DrawText(lpdis->hDC,"                                                  ",50,&rc,DT_SINGLELINE|DT_VCENTER|DT_INTERNAL|DT_LEFT);
		if(lpdis->itemState & ODS_SELECTED)
			{rc.left-=1;rc.top-=1;rc.bottom+=3;rc.right+=3;}
		else
			{rc.left-=1;rc.top-=2;rc.bottom+=2;rc.right+=3;}
		DrawText(lpdis->hDC,s,MyStringLength(s,64),&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		return TRUE;
	case WM_USER+2://hf obshiy uchun, dinamik o'zgartirish uchun emas:
		if(hf)DeleteObject(hf);
		if(br)DeleteObject(br);
		if(brHtBk)DeleteObject(brHtBk);
		hf = CreateFontIndirect(&conf::Dlg.dlgFnts[5]);
		br = CreateSolidBrush(conf::Dlg.dlgRGBs[5][0]);
		brHtBk = CreateSolidBrush(conf::Dlg.dlgRGBs[5][5]);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC4),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC8),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC5),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC10),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC6),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC7),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG4),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG2),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG5),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG3),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG6),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_BCKGRND_IMG_BROWSE),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC10),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		return 0;//GWLP_USERDATA
	case WM_DESTROY:
		DeleteObject(hf);
		DeleteObject(br);
		DeleteObject(brHtBk);
		DeleteObject(hBrshWM_CTLCOLORSTATIC);
		hBrshWM_CTLCOLORSTATIC = NULL;
		return 0;
	case WM_USER+3://Dinamik o'zgartirish uchun:
		DeleteObject(hf);
		hf = CreateFontIndirect(&conf::Dlg.dlgFnts[5]);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC4),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC8),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC5),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC10),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC6),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC7),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG4),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG2),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG5),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG3),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_COLOR_CONFIG6),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		return 0;//GWLP_USERDATA
	case WM_USER+4://Dinamik o'zgartirish uchun, colorni:
		DeleteObject(br);
		DeleteObject(brHtBk);
		br = CreateSolidBrush(conf::Dlg.dlgRGBs[5][0]);
		brHtBk = CreateSolidBrush(conf::Dlg.dlgRGBs[5][5]);
		RedrawWindow(hDlg,NULL,NULL,RDW_INVALIDATE|RDW_ALLCHILDREN);
		return 0;//GWLP_USERDATA*/
	case WM_USER+5:COLORREF* c;
		c=Dlg.GetColorPtr(numbSelctdBtn,Chld::iSelectedDlg);
		switch(wParam)
		{	case 0://R
				*c = RGB(lParam,GetGValue(*c),GetBValue(*c));
				break;
			case 1://G
				*c = RGB(GetRValue(*c),lParam,GetBValue(*c));
				break;
			case 2://B
				*c = RGB(GetRValue(*c),GetGValue(*c),lParam);
				break;
		}
		Dlg.SetColor(numbSelctdBtn,Chld::iSelectedDlg,*c);
		PostMessage(hDlg,WM_PAINT,0,0);
		chlds[Chld::iSelectedDlg].SetDestColor(numbSelctdBtn,c);
		break;
	case WM_USER+6:
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCKGRND_IMG_TYPE),CB_SETCURSEL,
							conf::Dlg.panBckgrndImgType[Chld::iSelectedDlg-24],0);
		SendMessage(GetDlgItem(hDlg,IDC_CHECK_ANIM_BCKIMG_TIME),BM_SETCHECK,
							conf::Dlg.panBckgrndImgAnimTime[Chld::iSelectedDlg-24]!=0?BST_CHECKED:BST_UNCHECKED,0);
		EnableWindow(GetDlgItem(hDlg,IDC_EDIT_BCKING_ANIM_TIME),
							conf::Dlg.panBckgrndImgAnimTime[Chld::iSelectedDlg-24]!=0);
		StringCchPrintf(st,MAX_PATH-1,L"%d",conf::Dlg.panBckgrndImgAnimTime[Chld::iSelectedDlg-24]==0xffffffff?-1:conf::Dlg.panBckgrndImgAnimTime[Chld::iSelectedDlg-24]);
		SetDlgItemText(hDlg,IDC_EDIT_BCKING_ANIM_TIME,st);
		numbSelctdBtn=-1;
		SetDlgItemText(hDlg,IDC_EDIT_BCKGRND_IMG,conf::Dlg.panBckgrndImgPth[Chld::iSelectedDlg-24]);
		return 0;
	case WM_PAINT:HBRUSH BR;HWND wn;
		for(int j=0; j<6; j++)
		{	c = Dlg.GetColorPtr(j,Chld::iSelectedDlg);
			BR = CreateSolidBrush(*c);
			switch(j)
			{case 0:wn = GetDlgItem(hDlg,IDC_BACK_COLOR_CONFIG);break;
			 case 1:wn = GetDlgItem(hDlg,IDC_FRONT_COLOR_CONFIG);break;
			 case 2:wn = GetDlgItem(hDlg,IDC_SELECT_BACK_COLOR_CONFIG);break;
			 case 3:wn = GetDlgItem(hDlg,IDC_SELECT_COLOR_CONFIG);break;
			 case 4:wn = GetDlgItem(hDlg,IDC_HOT_COLOR_CONFIG);break;
			 case 5:wn = GetDlgItem(hDlg,IDC_HOT_BACK_COLOR_CONFIG);break;
			}					
			dc = GetDC(wn);GetClientRect(wn,&rc);
			FillRect(dc,&rc,BR);
			DeleteObject(BR);
			ReleaseDC(wn,dc);
		}
		InvalidateRect(GetDlgItem(hDlg,IDC_STATIC),NULL,FALSE);
		InvalidateRect(GetDlgItem(hDlg,IDC_STATIC1),NULL,FALSE);
		InvalidateRect(GetDlgItem(hDlg,IDC_STATIC2),NULL,FALSE);
		return 0;//DefWindowProc(hDlg,message,wParam,lParam);
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CustomDefineDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:int iPan;
		RECT rc; HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);
		wchar_t st[MAX_PATH];
		/*LoadString(hInst,IDS_STRINGSW_119,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC,st);
		LoadString(hInst,IDS_STRINGSW_120,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC1,st);
		LoadString(hInst,IDS_STRINGSW_121,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC2,st);
		LoadString(hInst,IDS_STRINGSW_122,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC3,st);*/
		SetDlgItemText(hDlg,IDC_STATIC5,L"");//Font
		SetDlgItemText(hDlg,IDC_STATIC7,L"");//Color
		SetDlgItemText(hDlg,IDC_STATIC9,L"");//Color
		SetDlgItemText(hDlg,IDC_STATIC11,L"");//Color
		SetDlgItemText(hDlg,IDC_STATIC13,L"");//Color
		SetDlgItemText(hDlg,IDC_STATIC15,L"");//Color
		SetDlgItemText(hDlg,IDC_STATIC17,L"");//Color

		SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_ADDSTRING,0,(LPARAM)L"Not sorting");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_ADDSTRING,0,(LPARAM)L"To begin");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_ADDSTRING,0,(LPARAM)L"To end");
		//SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_SETCURSEL,0,0);

		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDC_BUTTON_ADD_CUSTOM:
				iPan = Chld::iSelectedDlg-TKEYDLGTAILNUM;
				if(Dlg.iCustPanItemDefs[iPan]>254)
				{	Err::FlipMsg(L"Do'nt allocate above 512 items.",1500);
					break;
				}
				Dlg.ReallocCustomItems(iPan,Dlg.iCustPanItemDefs[iPan]+1,FALSE);
				GetDlgItemText(hDlg,IDC_EDIT_ADD_NAME,st,MAX_PATH);
				if(MyStringLength(st,MAX_PATH)<1)
					StringCchPrintf(st,MAX_PATH-1,L"%d",Dlg.iCustPanItemDefs[iPan]);//uje 1taga ko'p;
				MyStringCpy(Dlg.cstmItms[iPan][Dlg.iCustPanItemDefs[iPan]-1].nameW,24,st);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_ADDSTRING,0,(LPARAM)Dlg.cstmItms[iPan][Dlg.iCustPanItemDefs[iPan]-1].nameW);
				//SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_SETCURSEL,Dlg.iCustPanItemDefs[iPan]-1,0);
				break;
			case IDC_COMBO_CUSTOM:int iItem;
				if(CBN_SELCHANGE==HIWORD(wParam))
				{	iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
					if(CB_ERR!=iItem)
					{	iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
						SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),
							CB_SETCURSEL,
							Dlg.cstmItms[iPan][iItem].sortMthd,0);
						SetDlgItemText(hDlg,IDC_STATIC5,Dlg.cstmItms[iPan][iItem].fnt.lfFaceName);
						StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
							GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[0]),
							GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[0]),
							GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[0]));
						SetDlgItemText(hDlg,IDC_STATIC7,st);
						StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
							GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[1]),
							GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[1]),
							GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[1]));
						SetDlgItemText(hDlg,IDC_STATIC9,st);
						StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
							GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[2]),
							GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[2]),
							GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[2]));
						SetDlgItemText(hDlg,IDC_STATIC11,st);
						StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
							GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[3]),
							GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[3]),
							GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[3]));
						SetDlgItemText(hDlg,IDC_STATIC13,st);
						StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
							GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[4]),
							GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[4]),
							GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[4]));
						SetDlgItemText(hDlg,IDC_STATIC15,st);
						StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
							GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[5]),
							GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[5]),
							GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[5]));
						SetDlgItemText(hDlg,IDC_STATIC17,st);
				}	}
				break;
			case IDC_COMBO_SORT_METHOD_CUSTOM:
				if(CBN_SELCHANGE==HIWORD(wParam))
				{	iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
					iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
					if(CB_ERR==iItem)break;
					Dlg.cstmItms[iPan][iItem].sortMthd=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_GETCURSEL,0,0);
				}
				break;
			case IDC_BUTTON_CHANGE_FONT_CUSTOM:CHOOSEFONT chf;
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				ZeroMemory(&chf,sizeof(chf));
				chf.lStructSize=sizeof(chf);chf.hwndOwner=hDlg;
				chf.Flags = CF_INITTOLOGFONTSTRUCT;
				chf.lpLogFont = &Dlg.cstmItms[iPan][iItem].fnt;
				if(ChooseFont(&chf))
				{	SetDlgItemText(hDlg,IDC_STATIC5,Dlg.cstmItms[iPan][iItem].fnt.lfFaceName);
					DeleteObject(Dlg.cstmItms[iPan][iItem].hFnt);
					Dlg.cstmItms[iPan][iItem].hFnt = CreateFontIndirect(&Dlg.cstmItms[iPan][iItem].fnt);
				}
				break;
			case IDC_BUTTON_CHANGE_BACK_COLOR_CUSTOM:CHOOSECOLOR chc;COLORREF acrCustClr[16];
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				ZeroMemory(&chc,sizeof(chc));
				chc.lStructSize=sizeof(chc);chc.hwndOwner=hDlg;
				chc.rgbResult = Dlg.cstmItms[iPan][iItem].clrRGBs[0];
				chc.Flags = CC_FULLOPEN|CC_RGBINIT;
				chc.lpCustColors = (LPDWORD)acrCustClr;
				if(ChooseColor(&chc))
				{	Dlg.cstmItms[iPan][iItem].clrRGBs[0]=chc.rgbResult;
					StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
						GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[0]),
						GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[0]),
						GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[0]));
					SetDlgItemText(hDlg,IDC_STATIC7,st);
					DeleteObject(Dlg.cstmItms[iPan][iItem].bkBrsh);
					Dlg.cstmItms[iPan][iItem].txtBkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][iItem].clrRGBs[2]);
				}
				break;
			case IDC_BUTTON_CHANGE_FRONT_COLOR_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				ZeroMemory(&chc,sizeof(chc));
				chc.lStructSize=sizeof(chc);chc.hwndOwner=hDlg;
				chc.rgbResult = Dlg.cstmItms[iPan][iItem].clrRGBs[1];
				chc.Flags = CC_FULLOPEN|CC_RGBINIT;
				chc.lpCustColors = (LPDWORD)acrCustClr;
				if(ChooseColor(&chc))
				{	Dlg.cstmItms[iPan][iItem].clrRGBs[1]=chc.rgbResult;
					StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
						GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[1]),
						GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[1]),
						GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[1]));
					SetDlgItemText(hDlg,IDC_STATIC9,st);
				}
				break;
			case IDC_BUTTON_CHANGE_SELECT_BACK_COLOR_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				ZeroMemory(&chc,sizeof(chc));
				chc.lStructSize=sizeof(chc);chc.hwndOwner=hDlg;
				chc.rgbResult = Dlg.cstmItms[iPan][iItem].clrRGBs[2];
				chc.Flags = CC_FULLOPEN|CC_RGBINIT;
				chc.lpCustColors = (LPDWORD)acrCustClr;
				if(ChooseColor(&chc))
				{	Dlg.cstmItms[iPan][iItem].clrRGBs[2]=chc.rgbResult;
					StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
						GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[2]),
						GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[2]),
						GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[2]));
					SetDlgItemText(hDlg,IDC_STATIC11,st);
					DeleteObject(Dlg.cstmItms[iPan][iItem].txtBkBrsh);
					Dlg.cstmItms[iPan][iItem].txtBkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][iItem].clrRGBs[2]);
				}
				break;
			case IDC_BUTTON_CHANGE_SELECT_COLOR_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				ZeroMemory(&chc,sizeof(chc));
				chc.lStructSize=sizeof(chc);chc.hwndOwner=hDlg;
				chc.rgbResult = Dlg.cstmItms[iPan][iItem].clrRGBs[3];
				chc.Flags = CC_FULLOPEN|CC_RGBINIT;
				chc.lpCustColors = (LPDWORD)acrCustClr;
				if(ChooseColor(&chc))
				{	Dlg.cstmItms[iPan][iItem].clrRGBs[3]=chc.rgbResult;
					StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
						GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[3]),
						GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[3]),
						GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[3]));
					SetDlgItemText(hDlg,IDC_STATIC13,st);
				}
				break;
			case IDC_BUTTON_CHANGE_HOT_COLOR_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				ZeroMemory(&chc,sizeof(chc));
				chc.lStructSize=sizeof(chc);chc.hwndOwner=hDlg;
				chc.rgbResult = Dlg.cstmItms[iPan][iItem].clrRGBs[4];
				chc.Flags = CC_FULLOPEN|CC_RGBINIT;
				chc.lpCustColors = (LPDWORD)acrCustClr;
				if(ChooseColor(&chc))
				{	Dlg.cstmItms[iPan][iItem].clrRGBs[4]=chc.rgbResult;
					StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
						GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[4]),
						GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[4]),
						GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[4]));
					SetDlgItemText(hDlg,IDC_STATIC15,st);
				}
				break;
			case IDC_BUTTON_CHANGE_HOT_BACK_COLOR_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				ZeroMemory(&chc,sizeof(chc));
				chc.lStructSize=sizeof(chc);chc.hwndOwner=hDlg;
				chc.rgbResult = Dlg.cstmItms[iPan][iItem].clrRGBs[5];
				chc.Flags = CC_FULLOPEN|CC_RGBINIT;
				chc.lpCustColors = (LPDWORD)acrCustClr;
				if(ChooseColor(&chc))
				{	Dlg.cstmItms[iPan][iItem].clrRGBs[5]=chc.rgbResult;
					StringCchPrintf(st,MAX_PATH-1,L"RGB(%d,%d,%d);",
						GetRValue(Dlg.cstmItms[iPan][iItem].clrRGBs[5]),
						GetGValue(Dlg.cstmItms[iPan][iItem].clrRGBs[5]),
						GetBValue(Dlg.cstmItms[iPan][iItem].clrRGBs[5]));
					SetDlgItemText(hDlg,IDC_STATIC17,st);
					DeleteObject(Dlg.cstmItms[iPan][iItem].hotBkBrsh);
					Dlg.cstmItms[iPan][iItem].hotBkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][iItem].clrRGBs[5]);
				}
				break;
			case IDC_BUTTON_CHANGE_CONDITION_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				if(CSearchPlgn::myOwnPlgnNum<0) break;
				//*fSearchViaF7::item = Dlg.cstmItms[iPan][iItem].itm;
				fSearchViaF7::plgns[CSearchPlgn::myOwnPlgnNum].LoadPlugin();
				int r;r=(int)DialogBoxParam(fSearchViaF7::plgns[CSearchPlgn::myOwnPlgnNum].hm,//hInst,
							MAKEINTRESOURCE(101),//IDD_DIALOG_SEACH_VIA_F7
							panel[iPan].GetHWND(),fSearchViaF7::CstmItemAttrbsDlgProc,0);
				fSearchViaF7::plgns[CSearchPlgn::myOwnPlgnNum].FreePlugin();
				if(1==r)//Bizniki 1 qaytargan:
					Dlg.cstmItms[iPan][iItem].itm = fSearchViaF7::item;
				break;
			case IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				//1.Free custom items for crnt panel:
				for(int it=0; it<Dlg.iCustPanItemDefs[iPan]; it++)
				{	DeleteObject(Dlg.cstmItms[iPan][it].hFnt);
					DeleteObject(Dlg.cstmItms[iPan][it].txtBkBrsh);
					DeleteObject(Dlg.cstmItms[iPan][it].bkBrsh);
					DeleteObject(Dlg.cstmItms[iPan][it].hotBkBrsh);
				}
				free(Dlg.cstmItms[iPan]);Dlg.cstmItms[iPan]=NULL;
				Dlg.iCustPanItemDefs[iPan]=0;
				//2.Free CB:
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_RESETCONTENT,0,0);
				//3.Realloc:
				Dlg.ReallocCustomItems(iPan,Dlg.iCustPanItemDefs[0],FALSE);
				//4.Copy from first:
				for(int it=0; it<Dlg.iCustPanItemDefs[0]; it++)
				{	SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_ADDSTRING,0,
						(LPARAM)Dlg.cstmItms[0][it].nameW);
					MyStringCpy(Dlg.cstmItms[iPan][it].nameW,24,Dlg.cstmItms[0][it].nameW);
					Dlg.cstmItms[iPan][it].sortMthd=Dlg.cstmItms[0][it].sortMthd;
					Dlg.cstmItms[iPan][it].fnt=Dlg.cstmItms[0][it].fnt;
					Dlg.cstmItms[iPan][it].hFnt=CreateFontIndirect(&Dlg.cstmItms[iPan][it].fnt);
					//Dlg.cstmItms[iPan][it].itm=Dlg.cstmItms[0][it].itm;
					for(int r=0; r<6; r++)
					 Dlg.cstmItms[iPan][it].clrRGBs[r]=Dlg.cstmItms[0][it].clrRGBs[r];
					Dlg.cstmItms[iPan][it].txtBkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][it].clrRGBs[2]);
					Dlg.cstmItms[iPan][it].bkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][it].clrRGBs[0]);
					Dlg.cstmItms[iPan][it].hotBkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][it].clrRGBs[5]);
				}
				//3.Change controls:
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_SETCURSEL,-1,0);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_SETCURSEL,-1,0);
				StringCchPrintf(st,128,L"%d",Dlg.iCustPanItemDefs[iPan]);
				SetDlgItemText(hDlg,IDC_EDIT_TOT_CUSTOM_ITEMS,st);
				SetDlgItemText(hDlg,IDC_EDIT_ADD_NAME,L"");
				SetDlgItemText(hDlg,IDC_STATIC5,L"");
				SetDlgItemText(hDlg,IDC_STATIC7,L"");
				SetDlgItemText(hDlg,IDC_STATIC9,L"");
				SetDlgItemText(hDlg,IDC_STATIC11,L"");
				SetDlgItemText(hDlg,IDC_STATIC13,L"");
				SetDlgItemText(hDlg,IDC_STATIC15,L"");
				SetDlgItemText(hDlg,IDC_STATIC17,L"");				
				break;
			case IDC_BUTTON_DELETE_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				iItem=(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_GETCURSEL,0,0);
				if(CB_ERR==iItem)break;
				//Delete item from CB:
				if(CB_ERR!=SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_DELETESTRING,iItem,0))
				{	for(int it=iItem; it<Dlg.iCustPanItemDefs[iPan]-1; it++)
					{	MyStringCpy(Dlg.cstmItms[iPan][it].nameW,24,Dlg.cstmItms[iPan][it+1].nameW);
						Dlg.cstmItms[iPan][it].sortMthd=Dlg.cstmItms[iPan][it+1].sortMthd;
						Dlg.cstmItms[iPan][it].fnt=Dlg.cstmItms[iPan][it+1].fnt;
						DeleteObject(Dlg.cstmItms[iPan][it].hFnt);
						Dlg.cstmItms[iPan][it].hFnt = CreateFontIndirect(&Dlg.cstmItms[iPan][it].fnt);
						//Dlg.cstmItms[iPan][it].itm=Dlg.cstmItms[iPan][it+1].itm;
						for(int r=0; r<6; r++)
							Dlg.cstmItms[iPan][it].clrRGBs[r]=Dlg.cstmItms[iPan][it+1].clrRGBs[r];
						DeleteObject(Dlg.cstmItms[iPan][it].txtBkBrsh);
						DeleteObject(Dlg.cstmItms[iPan][it].bkBrsh);
						DeleteObject(Dlg.cstmItms[iPan][it].hotBkBrsh);
						Dlg.cstmItms[iPan][it].txtBkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][it].clrRGBs[2]);
						Dlg.cstmItms[iPan][it].bkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][it].clrRGBs[0]);
						Dlg.cstmItms[iPan][it].hotBkBrsh = CreateSolidBrush(Dlg.cstmItms[iPan][it].clrRGBs[5]);
					}
					DeleteObject(Dlg.cstmItms[iPan][Dlg.iCustPanItemDefs[iPan]-1].hFnt);
					DeleteObject(Dlg.cstmItms[iPan][Dlg.iCustPanItemDefs[iPan]-1].txtBkBrsh);
					DeleteObject(Dlg.cstmItms[iPan][Dlg.iCustPanItemDefs[iPan]-1].bkBrsh);
					DeleteObject(Dlg.cstmItms[iPan][Dlg.iCustPanItemDefs[iPan]-1].hotBkBrsh);
					Dlg.ReallocCustomItems(iPan,Dlg.iCustPanItemDefs[iPan]-1,FALSE);
				}
				//3.Change controls:
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_SETCURSEL,-1,0);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_SETCURSEL,-1,0);
				StringCchPrintf(st,128,L"%d",Dlg.iCustPanItemDefs[iPan]);
				SetDlgItemText(hDlg,IDC_EDIT_TOT_CUSTOM_ITEMS,st);
				SetDlgItemText(hDlg,IDC_EDIT_ADD_NAME,L"");
				SetDlgItemText(hDlg,IDC_STATIC5,L"");
				SetDlgItemText(hDlg,IDC_STATIC7,L"");
				SetDlgItemText(hDlg,IDC_STATIC9,L"");
				SetDlgItemText(hDlg,IDC_STATIC11,L"");
				SetDlgItemText(hDlg,IDC_STATIC13,L"");
				SetDlgItemText(hDlg,IDC_STATIC15,L"");
				SetDlgItemText(hDlg,IDC_STATIC17,L"");				
				break;
			case IDC_BUTTON_DELETE_ALL_CUSTOM:
				iPan=Chld::iSelectedDlg-TKEYDLGTAILNUM;
				//1.Free custom items for crnt panel:
				for(int it=0; it<Dlg.iCustPanItemDefs[iPan]; it++)
				{	DeleteObject(Dlg.cstmItms[iPan][it].hFnt);
					DeleteObject(Dlg.cstmItms[iPan][it].txtBkBrsh);
					DeleteObject(Dlg.cstmItms[iPan][it].bkBrsh);
					DeleteObject(Dlg.cstmItms[iPan][it].hotBkBrsh);
				}
				free(Dlg.cstmItms[iPan]);Dlg.cstmItms[iPan]=NULL;
				Dlg.iCustPanItemDefs[iPan]=0;
				//2.Free CB:
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_RESETCONTENT,0,0);
				//3.Clean controls:
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_SORT_METHOD_CUSTOM),CB_SETCURSEL,-1,0);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_SETCURSEL,-1,0);
				SetDlgItemText(hDlg,IDC_EDIT_TOT_CUSTOM_ITEMS,L"");
				SetDlgItemText(hDlg,IDC_EDIT_ADD_NAME,L"");
				SetDlgItemText(hDlg,IDC_STATIC5,L"");
				SetDlgItemText(hDlg,IDC_STATIC7,L"");
				SetDlgItemText(hDlg,IDC_STATIC9,L"");
				SetDlgItemText(hDlg,IDC_STATIC11,L"");
				SetDlgItemText(hDlg,IDC_STATIC13,L"");
				SetDlgItemText(hDlg,IDC_STATIC15,L"");
				SetDlgItemText(hDlg,IDC_STATIC17,L"");				
				break;
		}
		return 0;
		case WM_USER+6:
			iPan = Chld::iSelectedDlg-TKEYDLGTAILNUM;
			StringCchPrintf(st,MAX_PATH-1,L"%d",Dlg.iCustPanItemDefs[iPan]);
			SetDlgItemText(hDlg,IDC_EDIT_TOT_CUSTOM_ITEMS,st);
			for(int i=0; i<Dlg.iCustPanItemDefs[iPan]; i++)
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_CUSTOM),CB_ADDSTRING,0,
							(LPARAM)Dlg.cstmItms[iPan][i].nameW);
			return 0;
	}
	return (INT_PTR)FALSE;
}

const UINT ChngFntTimerID = 0x14ff15cc;
VOID CALLBACK ChngFntTimerProc(HWND hwnd,UINT message,UINT idTimer,DWORD dwTime)
{ 
static LOGFONT oldLf={0,0,0,0,0,0,0,0,0,0,0,0,0,L""};
	if(hwnd==NULL)
	{	KillTimer(hwnd,ChngFntTimerID);
		ZeroLOGFONT(oldLf);
		return;
	}
	LOGFONT lf;
	SendMessage(hwnd,WM_CHOOSEFONT_GETLOGFONT,0,(LPARAM)&lf);
	if(lf.lfCharSet==oldLf.lfCharSet)
	if(lf.lfClipPrecision==oldLf.lfClipPrecision)
	if(lf.lfEscapement==oldLf.lfEscapement)
	if(!wcscmp(lf.lfFaceName,oldLf.lfFaceName))
	if(lf.lfHeight==oldLf.lfHeight)
	if(lf.lfItalic==oldLf.lfItalic)
	if(lf.lfOrientation==oldLf.lfOrientation)
	if(lf.lfOutPrecision==oldLf.lfOutPrecision)
	if(lf.lfPitchAndFamily==oldLf.lfPitchAndFamily)
	if(lf.lfQuality==oldLf.lfQuality)
	if(lf.lfStrikeOut==oldLf.lfStrikeOut)
	if(lf.lfUnderline==oldLf.lfUnderline)
	if(lf.lfWeight==oldLf.lfWeight)
	if(lf.lfWidth==oldLf.lfWidth)
	{
		return;
	}
	//else
	oldLf = lf;
	SendMessage(stdhDlg[chlds[Chld::iSelectedDlg].iWnd],WM_USER+5,0,(LPARAM)&lf);
	KillTimer(hwnd,ChngFntTimerID);
}

UINT_PTR CALLBACK CFHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{LOGFONT lf;
	switch(uiMsg)
	{	case WM_INITDIALOG:
			RECT rcPar; HWND prnt;prnt = GetParent(hdlg);if(!prnt)prnt=GetDesktopWindow();
			GetWindowRect(prnt, &rcPar);
			int parWidth,parHeight;
			parWidth = rcPar.right - rcPar.left;
			parHeight = rcPar.bottom - rcPar.top;
			RECT rc; GetWindowRect(hdlg, &rc);
			int width,height;
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			MoveWindow(hdlg,rcPar.right-75,rcPar.top,width,height,TRUE);
			return 0;//Process default;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{	//LOGFONT lf;
				case IDABORT:
				case IDCANCEL:
					//SendMessage(chlds[Chld::iSelectedDlg].hWnd,WM_USER+2,0,(LPARAM)&lf);//Cancel
					return 0;
				case IDOK:
					SendMessage(hdlg,WM_CHOOSEFONT_GETLOGFONT,0,(LPARAM)&lf);
					SendMessage(stdhDlg[chlds[Chld::iSelectedDlg].iWnd],WM_USER+1,0,(LPARAM)&lf);
					return 0;
				case 1136://Font CB:
					if(HIWORD(wParam)==CBN_SELCHANGE)
						SetTimer(hdlg,ChngFntTimerID,15,(TIMERPROC)ChngFntTimerProc);//SendMessage(hdlg,WM_CHOOSEFONT_GETLOGFONT,0,(LPARAM)&lf);
					return 0;
				case 1137://Font style CB:
					if(HIWORD(wParam)==CBN_SELCHANGE)
						SetTimer(hdlg,ChngFntTimerID,15,(TIMERPROC)ChngFntTimerProc);
					return 0;
				case 1138://Size CB: CHOOSEFONT chf.iPointSize=CB.q(shu)*10 ga teng;
					if(HIWORD(wParam)==CBN_SELCHANGE)
						SetTimer(hdlg,ChngFntTimerID,15,(TIMERPROC)ChngFntTimerProc);
					return 0;
				case 1140://Script CB:
					if(HIWORD(wParam)==CBN_SELCHANGE)
						SetTimer(hdlg,ChngFntTimerID,15,(TIMERPROC)ChngFntTimerProc);
					return 0;
			}
			return 0;
		case WM_DESTROY:
			KillTimer(hdlg,ChngFntTimerID);
			return 0;
	}
	return 0;
}

VOID ShowFontFamilly(HWND hDlg,int id,LOGFONT* lf)
{
wchar_t s[MAX_PATH/4];HDC dc = GetDC(hDlg);
int PntSz = -MulDiv(lf->lfHeight,72,GetDeviceCaps(dc,LOGPIXELSY));
	ReleaseDC(hDlg,dc);
	StringCchPrintf(s,MAX_PATH/4,L"%s %d",
					lf->lfFaceName,PntSz);
	SetDlgItemText(hDlg,id,s);
}

INT_PTR CALLBACK FntConfDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
static HFONT hf=0;
//static HBRUSH br=0;
//static HBRUSH brHtBk=0;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);
		wchar_t st[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_117,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC,st);
		LoadString(hInst,IDS_STRINGSW_114,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_BUTTON_CHANGE_FONT_CONFIG,st);
		LoadString(hInst,IDS_STRINGSW_131,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG,st);
		SendMessage(hDlg,WM_USER+2,0,0);
		break;	
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDC_BUTTON_CHANGE_FONT_CONFIG:
				CHOOSEFONT chf;
				ZeroMemory(&chf,sizeof(chf));
				chf.lStructSize=sizeof(chf);chf.hwndOwner=hDlg;
				chf.lpfnHook = CFHookProc;
				if(Chld::iSelectedDlg>0 && Chld::iSelectedDlg<5)
				{chf.lpLogFont = &Dlg.panFnts[Chld::iSelectedDlg-1];
				 chf.Flags = CF_ENABLEHOOK|CF_INITTOLOGFONTSTRUCT;
				}else if(Chld::iSelectedDlg>4 && Chld::iSelectedDlg<14)
				{chf.lpLogFont = &Dlg.dlgFnts[Chld::iSelectedDlg-5];
				 chf.Flags = CF_ENABLEHOOK|CF_INITTOLOGFONTSTRUCT;

				 switch(Chld::iSelectedDlg)
				 {case 5://CopyDlg:
					hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_MASTER),hWnd,fCopyOper::CopyDlgProc,0);
					ShowWindow(hTmpDlg,SW_SHOW);					
					break;
				  case 6://Copy queue:
					hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_QUEUE),hWnd,fCopyOper::CopyQueueDlgProc,0);
					ShowWindow(hTmpDlg,SW_SHOW);					
					break;
				  case 7://Background copy queue:
					hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_BACKGROUND_QUEUE),hWnd,fBckgrndCopyOper::BckgrndCopyDlgProc,0);
					ShowWindow(hTmpDlg,SW_SHOW);					
					break;
				  case 8://CreateFolder:
					hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_CREA_FOLDER),hWnd,CreaFolder::DlgProc,0);
					ShowWindow(hTmpDlg,SW_SHOW);					
					break;
				  case 9://Change font:
					hTmpDlg=hDlg;
					break;
				  case 11://Search:
				    hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_CREA_FOLDER),hWnd,CreaFolder::DlgProc,0);
					ShowWindow(hTmpDlg,SW_SHOW);					
					break;
				  case 12://Select disk:
					hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_CREA_FOLDER),hWnd,CreaFolder::DlgProc,0);
					ShowWindow(hTmpDlg,SW_SHOW);					
					break;
				  case 13://Others:
				    hTmpDlg=CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_LANGUAGE),hWnd,language::DlgProc,0);
				    ShowWindow(hTmpDlg,SW_SHOW);
				    break;
				 }

				}else if(Chld::iSelectedDlg==14)
				{chf.lpLogFont = &Dlg.menuFnt;
				 chf.Flags = CF_ENABLEHOOK|CF_INITTOLOGFONTSTRUCT;
				}else if(Chld::iSelectedDlg>14 && Chld::iSelectedDlg<19)
				{chf.lpLogFont = &Dlg.srtBtnFnt[Chld::iSelectedDlg-15];
				 chf.Flags = CF_ENABLEHOOK|CF_INITTOLOGFONTSTRUCT;
				}else if(Chld::iSelectedDlg>18 && Chld::iSelectedDlg<23)
				{chf.lpLogFont = &Dlg.editFnt[Chld::iSelectedDlg-19];
				 chf.Flags = CF_ENABLEHOOK|CF_INITTOLOGFONTSTRUCT;
				}else if(Chld::iSelectedDlg==23)
				{chf.lpLogFont = &Dlg.cmdBtnFnt;
				 chf.Flags = CF_ENABLEHOOK|CF_INITTOLOGFONTSTRUCT;
				}else
				 chf.Flags = CF_ENABLEHOOK;
				if(!ChooseFont(&chf))
				{	//Otmena:
					HFONT hf;hf = CreateFontIndirect(&chlds[Chld::iSelectedDlg].oldFont);
					chlds[Chld::iSelectedDlg].SetFontFont(hf);
					chlds[Chld::iSelectedDlg].SetDestFont(hf,&chlds[Chld::iSelectedDlg].oldFont);
					ShowFontFamilly(hDlg,IDC_STATIC1,&chlds[Chld::iSelectedDlg].oldFont);
					DeleteObject(hf);
				}
				if(hTmpDlg)
				if(9!=Chld::iSelectedDlg)//Change font:
				{	DestroyWindow(hTmpDlg);
					hTmpDlg = 0;
				}
				break;
			case IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG:
				int iFrom;iFrom = Chld::iSelectedDlg>14?14:(Chld::iSelectedDlg>5?5:1);
				HFONT hf;hf = CreateFontIndirect(&chlds[iFrom].oldFont);
				chlds[Chld::iSelectedDlg].SetFontFont(hf);
				chlds[Chld::iSelectedDlg].SetDestFont(hf,&chlds[iFrom].oldFont);
				ShowFontFamilly(hDlg,IDC_STATIC1,&chlds[iFrom].oldFont);
				DeleteObject(hf);
				break;
			case IDC_COMBO_BUTTONS_TYPE:
				if(CBN_SELCHANGE==HIWORD(wParam))
					Dlg.iBtnsType = (int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_BUTTONS_TYPE),CB_GETCURSEL,0,0);
				break;
		}
		return 0;
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
		/*HDC dc;dc = (HDC)wParam;
		SetTextColor(dc,conf::Dlg.dlgRGBs[4][1]);
		SetBkColor(dc,conf::Dlg.dlgRGBs[4][2]);
		return (INT_PTR)br;*/
		return 0;
	case WM_CTLCOLOREDIT:
		/*dc = (HDC)wParam;
		SetTextColor(dc,conf::Dlg.dlgRGBs[4][1]);
		SetBkColor(dc,conf::Dlg.dlgRGBs[4][2]);
		return (INT_PTR)br;*/
		return 0;
	case WM_CTLCOLORBTN:
		/*dc=(HDC)wParam;
		SetTextColor(dc,conf::Dlg.dlgRGBs[4][4]);
		SetBkColor(dc,conf::Dlg.dlgRGBs[4][5]);
		return (INT_PTR)brHtBk;*/
		return 0;
	case WM_DRAWITEM://WM_CTLCOLORBTN dagi knopkalar:
		LPDRAWITEMSTRUCT lpdis;lpdis = (LPDRAWITEMSTRUCT)lParam;
		wchar_t s[64];GetWindowText(lpdis->hwndItem,s,64);
		UINT uStyle;uStyle = DFCS_BUTTONPUSH;
		rc = lpdis->rcItem;
		if(lpdis->itemState & ODS_SELECTED)
		{	uStyle |= DFCS_PUSHED;
			rc.left+=2;rc.top+=2;
		}
		DrawFrameControl(lpdis->hDC, &lpdis->rcItem, DFC_BUTTON, uStyle);
		if(lpdis->itemState & ODS_SELECTED)
			{rc.left+=1;rc.top+=1;rc.bottom-=2;rc.right-=3;}
		else
			{rc.left+=1;rc.top+=2;rc.bottom-=3;rc.right-=3;}
		FillRect(lpdis->hDC,&rc,brHtBk);//DrawText(lpdis->hDC,"                                                  ",50,&rc,DT_SINGLELINE|DT_VCENTER|DT_INTERNAL|DT_LEFT);
		if(lpdis->itemState & ODS_SELECTED)
			{rc.left-=1;rc.top-=1;rc.bottom+=3;rc.right+=3;}
		else
			{rc.left-=1;rc.top-=2;rc.bottom+=2;rc.right+=3;}
		DrawText(lpdis->hDC,s,MyStringLength(s,64),&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		return TRUE;
	case WM_USER+2://hf obshiy uchun, dinamik o'zgartirish uchun emas:
		if(hf)DeleteObject(hf);
		//if(br)DeleteObject(br);
		//if(brHtBk)DeleteObject(brHtBk);
		hf = CreateFontIndirect(&conf::Dlg.dlgFnts[4]);
		//br = CreateSolidBrush(conf::Dlg.dlgRGBs[4][0]);
		//brHtBk = CreateSolidBrush(conf::Dlg.dlgRGBs[4][5]);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC1),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_FONT_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		return 0;//GWLP_USERDATA
	case WM_DESTROY:
		DeleteObject(hf);
		//DeleteObject(br);
		//DeleteObject(brHtBk);
		return 0;
	case WM_USER+3://Dinamik o'zgartirish uchun:
		DeleteObject(hf);
		hf = CreateFontIndirect(&conf::Dlg.dlgFnts[4]);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_STATIC1),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_CHANGE_FONT_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),WM_SETFONT,(WPARAM)hf,TRUE);
		return 0;//GWLP_USERDATA
	case WM_USER+4://Dinamik o'zgartirish uchun, colorni:
		//DeleteObject(br);
		//DeleteObject(brHtBk);
		//br = CreateSolidBrush(conf::Dlg.dlgRGBs[4][0]);
		//brHtBk = CreateSolidBrush(conf::Dlg.dlgRGBs[4][5]);
		RedrawWindow(hDlg,NULL,NULL,RDW_INVALIDATE|RDW_ALLCHILDREN);
		return 0;//GWLP_USERDATA*/
	case WM_USER+5:
		HFONT hf;hf = CreateFontIndirect((LOGFONT*)lParam);
		chlds[Chld::iSelectedDlg].SetFontFont(hf);
		chlds[Chld::iSelectedDlg].SetDestFont(hf,(LOGFONT*)lParam);
		ShowFontFamilly(hDlg,IDC_STATIC1,(LOGFONT*)lParam);
		DeleteObject(hf);
		return 0;
	case WM_USER+1://OK LOGFONT
		hf = CreateFontIndirect((LOGFONT*)lParam);
		GetObject(hf,sizeof(LOGFONT),&chlds[Chld::iSelectedDlg].oldFont);
		Dlg.panFnts[Chld::iSelectedDlg-1]=chlds[Chld::iSelectedDlg].oldFont;
		DeleteObject(hf);
		return 0;
	case WM_USER+6://Dialog change
		if(Chld::iSelectedDlg<5)
		{LoadString(hInst,IDS_STRINGSW_163,st,MAX_PATH);
		 SetDlgItemText(hDlg,IDC_STATIC6,st);
		 wchar_t s[8];//StringCchPrintf(s,8,"%d",MIN_FONT_INTERVAL+(MAX_FONT_INTERVAL-MIN_FONT_INTERVAL) / 2);
		 StringCchPrintf(s,8,L"%d",Dlg.interval[Chld::iSelectedDlg-1]);
		 SetDlgItemText(hDlg,IDC_EDIT_FONT_INTERVAL_CONFIG,s);
		 SendMessage(GetDlgItem(hDlg,IDC_SPIN_FONT_INTERVAL_CONFIG),UDM_SETRANGE32,0,MAX_FONT_INTERVAL-MIN_FONT_INTERVAL);
		 SendMessage(GetDlgItem(hDlg,IDC_SPIN_FONT_INTERVAL_CONFIG),UDM_SETPOS32,0,Dlg.interval[Chld::iSelectedDlg-1]-MIN_FONT_INTERVAL);
		}
		else
		{	ShowWindow(GetDlgItem(hDlg,IDC_STATIC6),SW_HIDE);
			ShowWindow(GetDlgItem(hDlg,IDC_EDIT_FONT_INTERVAL_CONFIG),SW_HIDE);
			ShowWindow(GetDlgItem(hDlg,IDC_SPIN_FONT_INTERVAL_CONFIG),SW_HIDE);
		}
		if(23==Chld::iSelectedDlg)//CmndsBtns
		{	SendMessage(GetDlgItem(hDlg,IDC_COMBO_BUTTONS_TYPE),CB_ADDSTRING,0,(LPARAM)L"From my shell-1...");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_BUTTONS_TYPE),CB_ADDSTRING,0,(LPARAM)L"Standard buttons...");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_BUTTONS_TYPE),CB_ADDSTRING,0,(LPARAM)L"Ownedraw buttons...");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_BUTTONS_TYPE),CB_SETCURSEL,Dlg.iBtnsType,0);
		}
		else
		{	ShowWindow(GetDlgItem(hDlg,IDC_COMBO_BUTTONS_TYPE),SW_HIDE);
			//ShowWindow(GetDlgItem(hDlg,IDC_STATIC6),SW_HIDE);DestroyWindow(GetDlgItem(hDlg,IDC_STATIC6));
		}
		return 0;//On dialog changed:
	case WM_NOTIFY:
		NMHDR *phdr=(NMHDR*)lParam;
		if(UDN_DELTAPOS==phdr->code)//from up-down control-change pos;
		if(IDC_SPIN_FONT_INTERVAL_CONFIG==phdr->idFrom)
		{	//DefWindowProc(hDlg,message,wParam,lParam);
			//int iPos = SendMessage(GetDlgItem(hDlg,IDC_SPIN_FONT_INTERVAL_CONFIG),UDM_GETPOS32,0,NULL);
			int step = ((LPNMUPDOWN)lParam)->iDelta>0?1:-1;
			Dlg.interval[Chld::iSelectedDlg-1] = MIN_FONT_INTERVAL+((LPNMUPDOWN)lParam)->iPos+step;
			wchar_t s[8];StringCchPrintf(s,8,L"%d",Dlg.interval[Chld::iSelectedDlg-1]);
			SetDlgItemText(hDlg,IDC_EDIT_FONT_INTERVAL_CONFIG,s);

			HDC dc = GetDC(hDlg);
			int fntHeight=-MulDiv(Dlg.panFnts[Chld::iSelectedDlg-1].lfHeight,72,GetDeviceCaps(dc,LOGPIXELSY));
			ReleaseDC(hDlg,dc);

			panel[Chld::iSelectedDlg-1].SetItemHeight(fntHeight+Dlg.interval[Chld::iSelectedDlg-1]);
			panel[Chld::iSelectedDlg-1].Render();
		}
		return 0;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK HotKeyConfDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
int iSelctd=0;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);
		wchar_t st[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_187,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG,st);
		LoadString(hInst,IDS_STRINGSW_188,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG,st);
		LoadString(hInst,IDS_STRINGSW_189,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG,st);
		LoadString(hInst,IDS_STRINGSW_190,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC,st);

			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"q");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"w");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"e");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"r");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"t");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"y");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"u");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"i");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"o");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"p");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"a");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"s");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"d");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"f");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"g");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"h");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"j");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"k");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"l");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"z");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"x");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"c");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"v");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"b");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"n");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"m");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"[");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"]");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"\"");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"\\");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"<");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L">");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"?");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"1");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"2");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"3");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"4");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"5");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"6");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"7");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"8");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"9");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"0");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"-");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"=");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"backslash");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"~");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Tab");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F1");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F2");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F3");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F4");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F5");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F6");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F7");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F8");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F9");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F10");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F11");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F12");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Enter");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Insert");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Delete");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Home");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"End");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Page Up");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Page Down");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Left");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Right");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Up");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Down");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:0");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:1");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:2");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:3");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:4");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:5");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:6");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:7");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:8");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:9");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:Del");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:/");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:*");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:-");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:+");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Escape");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Space");
		return (INT_PTR)TRUE;
	
	case WM_COMMAND:
		iSelctd = Chld::iSelectedDlg-TKEYDLGBGNNUM;
		switch(LOWORD(wParam))
		{	case IDC_CHECK_CTL_PRESSED_CONFIG:
				Dlg.keys[iSelctd].bCtrl = (BST_CHECKED ==
					SendMessage(GetDlgItem(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG),
							BM_GETCHECK,0,0)) ? 1: 0;
				if(Dlg.IsKeyExist(iSelctd))
					MessageBox(hDlg,L"This key already exists!!!",L"Please,change Ctrl. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				break;
			case IDC_CHECK_ALT_PRESSED_CONFIG:
				Dlg.keys[iSelctd].bAlt = (BST_CHECKED ==
					SendMessage(GetDlgItem(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG),
							BM_GETCHECK,0,0)) ? 1: 0;
				if(Dlg.IsKeyExist(iSelctd))
					MessageBox(hDlg,L"This key already exists!!!",L"Please,change Alt. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				break;
			case IDC_CHECK_SHIFT_PRESSED_CONFIG:
				Dlg.keys[iSelctd].bShft = (BST_CHECKED ==
					SendMessage(GetDlgItem(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG),
							BM_GETCHECK,0,0)) ? 1: 0;
				if(Dlg.IsKeyExist(iSelctd))
					MessageBox(hDlg,L"This key already exists!!!",L"Please,change Sft. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				break;
			case IDC_COMBO_KEY_CONFIG:
				if(CBN_SELCHANGE==HIWORD(wParam))
				{	Dlg.keys[iSelctd].key = Dlg.GetKeyCode(
						(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),
									CB_GETCURSEL,0,0));
					if(Dlg.IsKeyExist(iSelctd))
						MessageBox(hDlg,L"This key already exists!!!",L"Please,change key.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				}
				break;
		}
		break;
	case WM_USER+6:
		iSelctd = Chld::iSelectedDlg-TKEYDLGBGNNUM;
		SendMessage(GetDlgItem(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG),BM_SETCHECK,
					Dlg.keys[iSelctd].bCtrl?BST_CHECKED:BST_UNCHECKED,0);
		SendMessage(GetDlgItem(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG),BM_SETCHECK,
					Dlg.keys[iSelctd].bAlt?BST_CHECKED:BST_UNCHECKED,0);
		SendMessage(GetDlgItem(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG),BM_SETCHECK,
					Dlg.keys[iSelctd].bShft?BST_CHECKED:BST_UNCHECKED,0);
		iSelctd = Dlg.GetCodeKey(Dlg.keys[iSelctd].key);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_SETCURSEL,iSelctd,0);
		return 0;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DirectLinkOptnDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);
		wchar_t s[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_233,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC6,s);
		LoadString(hInst,IDS_STRINGSW_234,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC7,s);

		StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.dwSockTimeout);
		SetDlgItemText(hDlg,IDC_EDIT_LINK_TIMEOUT_CONFIG,s);
		StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.dwSockBufSize/1024);
		SetDlgItemText(hDlg,IDC_EDIT_LINK_BUFFER_SIZE_CONFIG,s);

		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_TIMEOT_CONFIG),UDM_SETRANGE32,0,10000);
		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_TIMEOT_CONFIG),UDM_SETPOS32,0,10000);
		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_BUFFER_SIZE_CONFIG),UDM_SETRANGE32,0,10000);
		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_BUFFER_SIZE_CONFIG),UDM_SETPOS32,0,10000);

		return (INT_PTR)TRUE;
	
	case WM_COMMAND:
		if(EN_CHANGE==HIWORD(wParam))
		{	switch(LOWORD(wParam))
			{	case IDC_EDIT_LINK_TIMEOUT_CONFIG:
					GetDlgItemTextA(hDlg,IDC_EDIT_LINK_TIMEOUT_CONFIG,(LPSTR)s,MAX_PATH);
					int tmo;tmo = atoi((const char*)s);
					tmo = 10*(tmo / 10);
					if(tmo<1000) tmo=1000;
					else if(tmo>100000)tmo=100000;
					Dlg.dwSockTimeout = tmo;
					break;
				case IDC_EDIT_LINK_BUFFER_SIZE_CONFIG:
					GetDlgItemTextA(hDlg,IDC_EDIT_LINK_BUFFER_SIZE_CONFIG,(LPSTR)s,MAX_PATH);
					int bfsz;bfsz = atoi((const char*)s);
					bfsz = 32 * (bfsz / 32);
					if(bfsz<256) bfsz=256;
					else if(bfsz>10000000)bfsz=10000000;
					Dlg.dwSockBufSize = bfsz*1024;
					break;
		}	}
		break;
	case WM_NOTIFY:
		NMHDR *phdr=(NMHDR*)lParam;
		if(UDN_DELTAPOS==phdr->code)//from up-down control-change pos;
		{	int step = ((LPNMUPDOWN)lParam)->iDelta>0?-1:1;
			if(IDC_SPIN_TIMEOT_CONFIG==phdr->idFrom)
			{	Dlg.dwSockTimeout += step*10;
				StringCchPrintf(s,8,L"%d",Dlg.dwSockTimeout);
				SetDlgItemText(hDlg,IDC_EDIT_LINK_TIMEOUT_CONFIG,s);
			}
			else if(IDC_SPIN_BUFFER_SIZE_CONFIG==phdr->idFrom)
			{	Dlg.dwSockBufSize += 1024*step*32;
				StringCchPrintf(s,8,L"%d",Dlg.dwSockBufSize/1024);
				SetDlgItemText(hDlg,IDC_EDIT_LINK_BUFFER_SIZE_CONFIG,s);
		}	}
		return 0;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK plgnOptnDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);
		wchar_t s[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_17,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC,s);
		
		LoadString(hInst,IDS_STRINGSW_16,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC2,s);
		
		LoadString(hInst,IDS_STRINGSW_18,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC3,s);
		
		LoadString(hInst,IDS_STRINGSW_15,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC1,s);
		
		//SetWindowLong(hDlg,GWLP_USERDATA,iSelectedDlg);

		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_NUMBER_PLUGINS),UDM_SETRANGE32,0,10000);
		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_NUMBER_PLUGINS),UDM_SETPOS32,0,10000);
		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_NUMBER_PLUGINS),UDM_SETRANGE32,0,10000);
		//SendMessage(GetDlgItem(hDlg,IDC_SPIN_NUMBER_PLUGINS),UDM_SETPOS32,0,10000);

		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if(EN_CHANGE==HIWORD(wParam))
		{	switch(LOWORD(wParam))
			{	case IDC_EDIT_NUMBER_PLUGINS:
					GetDlgItemTextA(hDlg,IDC_EDIT_NUMBER_PLUGINS,(LPSTR)s,MAX_PATH);
					int tplg;tplg = atoi((LPSTR)s);
					if(tplg<1) tplg=1;
					else if(tplg>100)tplg=100;
					if(TKEYDLGTAILNUM+5==Chld::iSelectedDlg)//GetWindowLong(hDlg,GWLP_USERDATA))
						Dlg.totalAllovedArchvr = tplg;
					else if(TKEYDLGTAILNUM+6==Chld::iSelectedDlg)
						Dlg.totalAllovedSchPlg = tplg;
					else if(TKEYDLGTAILNUM+7==Chld::iSelectedDlg)
						Dlg.totalAllovedMnutPlg = tplg;
					else if(TKEYDLGTAILNUM+8==Chld::iSelectedDlg)
						Dlg.totalAllovedQVPlg = tplg;
					break;
				case IDC_EDIT_CURRENT_PLUGIN:
					GetDlgItemTextA(hDlg,IDC_EDIT_CURRENT_PLUGIN,(LPSTR)s,MAX_PATH);
					tplg = atoi((LPSTR)s);
					if(tplg<0) tplg=0;
					else if(tplg>100)tplg=100;
					if(TKEYDLGTAILNUM+5==Chld::iSelectedDlg)//GetWindowLong(hDlg,GWLP_USERDATA))
					{	Dlg.crntArchvr = tplg;
						StringCchPrintf(s,MAX_PATH,L"%d",archive::numPlugins);
						SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);
						if(archive::numPlugins>0)
							SetDlgItemText(hDlg,IDC_EDIT_MODULE_NAME,archive::plgns[Dlg.crntArchvr].descrpn);//GetPluginDescription());
					}
					else if(TKEYDLGTAILNUM+6==Chld::iSelectedDlg)
					{	Dlg.crntSchplg = tplg;
						StringCchPrintf(s,MAX_PATH,L"%d",fSearchViaF7::numPlugins);
						SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);
						if(fSearchViaF7::numPlugins>0)
							SetDlgItemText(hDlg,IDC_EDIT_MODULE_NAME,fSearchViaF7::plgns[Dlg.crntSchplg].pathAndName);
					}
					else if(TKEYDLGTAILNUM+7==Chld::iSelectedDlg)
					{	Dlg.crntMnutplg = tplg;
						StringCchPrintf(s,MAX_PATH,L"%d",menuUtils::numPlugins);
						SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);
						if(menuUtils::numPlugins>0)
							SetDlgItemText(hDlg,IDC_EDIT_MODULE_NAME,menuUtils::plgns[Dlg.crntMnutplg].pathAndName);
					}
					else if(TKEYDLGTAILNUM+8==Chld::iSelectedDlg)
					{	Dlg.crntQVplg = tplg;
						StringCchPrintf(s,MAX_PATH,L"%d",QView::numPlugins);
						SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);
						if(QView::numPlugins>0)
							SetDlgItemText(hDlg,IDC_EDIT_MODULE_NAME,QView::plgns[Dlg.crntQVplg].pathAndName);
					}
					break;
			}
			break;
		}
		switch(LOWORD(wParam))
		{	case IDC_CHECK_CTL_PRESSED_CONFIG:
				GetDlgItemTextA(hDlg,IDC_EDIT_CURRENT_PLUGIN,(LPSTR)s,MAX_PATH);
				int tplg;tplg = atoi((LPSTR)s);
				if(tplg<0) tplg=0;
				else if(tplg>100)tplg=100;
				menuUtils::plgns[tplg].key.bCtrl = (BST_CHECKED ==
					SendMessage(GetDlgItem(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG),
							BM_GETCHECK,0,0)) ? 1: 0;
				if(Dlg.IsKeyExist(&menuUtils::plgns[tplg].key))
					MessageBox(hDlg,L"This key already exists!!!",L"Please,change Ctrl. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				if(menuUtils::IsKeyExist(tplg))
					MessageBox(hDlg,L"This key already exists in plugins!!!",L"Please,change Ctrl. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				break;
			case IDC_CHECK_ALT_PRESSED_CONFIG:
				GetDlgItemText(hDlg,IDC_EDIT_CURRENT_PLUGIN,s,MAX_PATH);
				tplg = atoi((LPSTR)s);
				if(tplg<0) tplg=0;
				else if(tplg>100)tplg=100;
				menuUtils::plgns[tplg].key.bAlt = (BST_CHECKED ==
					SendMessage(GetDlgItem(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG),
							BM_GETCHECK,0,0)) ? 1: 0;
				if(Dlg.IsKeyExist(&menuUtils::plgns[tplg].key))
					MessageBox(hDlg,L"This key already exists!!!",L"Please,change Alt. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				if(menuUtils::IsKeyExist(tplg))
					MessageBox(hDlg,L"This key already exists in plugins!!!",L"Please,change Ctrl. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				break;
			case IDC_CHECK_SHIFT_PRESSED_CONFIG:
				GetDlgItemTextA(hDlg,IDC_EDIT_CURRENT_PLUGIN,(LPSTR)s,MAX_PATH);
				tplg = atoi((LPSTR)s);
				if(tplg<0) tplg=0;
				else if(tplg>100)tplg=100;
				menuUtils::plgns[tplg].key.bShft = (BST_CHECKED ==
					SendMessage(GetDlgItem(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG),
							BM_GETCHECK,0,0)) ? 1: 0;
				if(Dlg.IsKeyExist(&menuUtils::plgns[tplg].key))
					MessageBox(hDlg,L"This key already exists!!!",L"Please,change Sft. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				if(menuUtils::IsKeyExist(tplg))
					MessageBox(hDlg,L"This key already exists in plugins!!!",L"Please,change Ctrl. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				break;
			case IDC_COMBO_KEY_CONFIG:
				if(CBN_SELCHANGE==HIWORD(wParam))
				{	GetDlgItemTextA(hDlg,IDC_EDIT_CURRENT_PLUGIN,(LPSTR)s,MAX_PATH);
					tplg = atoi((LPSTR)s);
					if(tplg<0) tplg=0;
					else if(tplg>100)tplg=100;
					menuUtils::plgns[tplg].key.key = Dlg.GetKeyCode(
						(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_GETCURSEL,0,0));
					if(Dlg.IsKeyExist(&menuUtils::plgns[tplg].key))
						MessageBox(hDlg,L"This key already exists!!!",L"Please,change key.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
					if(menuUtils::IsKeyExist(tplg))
						MessageBox(hDlg,L"This key already exists in plugins!!!",L"Please,change Ctrl. state.",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
				}
				break;
		}
		break;
	case WM_NOTIFY:
		NMHDR *phdr;phdr=(NMHDR*)lParam;
		if(UDN_DELTAPOS==phdr->code)//from up-down control-change pos;
		{	int step = ((LPNMUPDOWN)lParam)->iDelta>0?-1:1;
			if(IDC_SPIN_NUMBER_PLUGINS==phdr->idFrom)
			{	if(TKEYDLGTAILNUM+5==Chld::iSelectedDlg)//GetWindowLong(hDlg,GWLP_USERDATA))
				{	Dlg.totalAllovedArchvr += step;
					if(Dlg.totalAllovedArchvr < 1)
						Dlg.totalAllovedArchvr = 1;
					StringCchPrintf(s,8,L"%d",Dlg.totalAllovedArchvr);
				}
				else if(TKEYDLGTAILNUM+6==Chld::iSelectedDlg)
				{	Dlg.totalAllovedSchPlg += step;
					if(Dlg.totalAllovedSchPlg < 1)
						Dlg.totalAllovedSchPlg = 1;
					StringCchPrintf(s,8,L"%d",Dlg.totalAllovedSchPlg);
				}
				else if(TKEYDLGTAILNUM+7==Chld::iSelectedDlg)
				{	Dlg.totalAllovedMnutPlg += step;
					if(Dlg.totalAllovedMnutPlg < 1)
						Dlg.totalAllovedMnutPlg = 1;
					StringCchPrintf(s,8,L"%d",Dlg.totalAllovedMnutPlg);
				}
				else if(TKEYDLGTAILNUM+8==Chld::iSelectedDlg)
				{	Dlg.totalAllovedQVPlg += step;
					if(Dlg.totalAllovedQVPlg < 1)
						Dlg.totalAllovedQVPlg = 1;
					StringCchPrintf(s,8,L"%d",Dlg.totalAllovedQVPlg);
				}
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_PLUGINS,s);
			}
			else if(IDC_SPIN_CURRENT_PLUGIN==phdr->idFrom)
			{	if(TKEYDLGTAILNUM+5==Chld::iSelectedDlg)//GetWindowLong(hDlg,GWLP_USERDATA))
				{	Dlg.crntArchvr += step;
					if(Dlg.crntArchvr > archive::numPlugins-1)
						Dlg.crntArchvr = archive::numPlugins-1;
					else if(Dlg.crntArchvr < 0)
						Dlg.crntArchvr = 0;
					StringCchPrintf(s,8,L"%d",Dlg.crntArchvr);
				}
				else if(TKEYDLGTAILNUM+6==Chld::iSelectedDlg)
				{	Dlg.crntSchplg += step;
					if(Dlg.crntSchplg > fSearchViaF7::numPlugins-1)
						Dlg.crntSchplg = fSearchViaF7::numPlugins-1;
					else if(Dlg.crntSchplg < 0)
						Dlg.crntSchplg = 0;
					StringCchPrintf(s,8,L"%d",Dlg.crntSchplg);
				}
				else if(TKEYDLGTAILNUM+7==Chld::iSelectedDlg)
				{	Dlg.crntMnutplg += step;
					if(Dlg.crntMnutplg > menuUtils::numPlugins-1)
						Dlg.crntMnutplg = menuUtils::numPlugins-1;
					else if(Dlg.crntMnutplg < 0)
						Dlg.crntMnutplg = 0;
					StringCchPrintf(s,8,L"%d",Dlg.crntMnutplg);
				}
				else if(TKEYDLGTAILNUM+8==Chld::iSelectedDlg)
				{	Dlg.crntQVplg += step;
					if(Dlg.crntQVplg > QView::numPlugins-1)
						Dlg.crntQVplg = QView::numPlugins-1;
					else if(Dlg.crntQVplg < 0)
						Dlg.crntQVplg = 0;
					StringCchPrintf(s,8,L"%d",Dlg.crntQVplg);
				}
				SetDlgItemText(hDlg,IDC_EDIT_CURRENT_PLUGIN,s);
		}	}
		return 0;
	case WM_USER+6:
		if(Chld::iSelectedDlg==TKEYDLGTAILNUM+5)
		{	if(archive::numPlugins>0)
			{	SetDlgItemText(hDlg,IDC_EDIT_MODULE_NAME,archive::plgns[Dlg.crntArchvr].descrpn);//GetPluginDescription());
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.totalAllovedArchvr);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_PLUGINS,s);
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.crntArchvr);
				SetDlgItemText(hDlg,IDC_EDIT_CURRENT_PLUGIN,s);
				StringCchPrintf(s,MAX_PATH,L"%d",archive::numPlugins);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);
			}
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),FALSE);
		}
		else if(Chld::iSelectedDlg==TKEYDLGTAILNUM+6)
		{	if(fSearchViaF7::numPlugins>0)
			{	SetDlgItemText( hDlg,IDC_EDIT_MODULE_NAME,fSearchViaF7::plgns[Dlg.crntSchplg].pathAndName);
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.totalAllovedSchPlg);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_PLUGINS,s);
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.crntSchplg);
				SetDlgItemText(hDlg,IDC_EDIT_CURRENT_PLUGIN,s);
				StringCchPrintf(s,MAX_PATH,L"%d",fSearchViaF7::numPlugins);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);
			}
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),FALSE);
		}
		else if(Chld::iSelectedDlg==TKEYDLGTAILNUM+7)
		{	if(menuUtils::numPlugins>0)
			{	SetDlgItemText( hDlg,IDC_EDIT_MODULE_NAME,menuUtils::plgns[Dlg.crntMnutplg].pathAndName);
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.totalAllovedMnutPlg);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_PLUGINS,s);
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.crntMnutplg);
				SetDlgItemText(hDlg,IDC_EDIT_CURRENT_PLUGIN,s);
				StringCchPrintf(s,MAX_PATH,L"%d",menuUtils::numPlugins);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);

				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_RESETCONTENT,0,0);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"q");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"w");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"e");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"r");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"t");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"y");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"u");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"i");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"o");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"p");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"a");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"s");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"d");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"f");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"g");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"h");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"j");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"k");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"l");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"z");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"x");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"c");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"v");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"b");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"n");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"m");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"[");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"]");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"\"");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"\\");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"<");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L">");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"?");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"1");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"2");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"3");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"4");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"5");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"6");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"7");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"8");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"9");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"0");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"-");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"=");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"backslash");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"~");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Tab");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F1");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F2");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F3");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F4");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F5");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F6");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F7");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F8");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F9");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F10");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F11");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"F12");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Enter");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Insert");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Delete");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Home");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"End");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Page Up");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Page Down");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Left");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Right");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Up");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Down");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:0");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:1");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:2");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:3");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:4");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:5");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:6");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:7");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:8");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:9");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:Del");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:/");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:*");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:-");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Numpad:+");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Escape");
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_ADDSTRING,0,(LPARAM)L"Space");


				SendMessage(GetDlgItem(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG),BM_SETCHECK,
							menuUtils::plgns[Dlg.crntMnutplg].key.bCtrl?BST_CHECKED:BST_UNCHECKED,0);
				SendMessage(GetDlgItem(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG),BM_SETCHECK,
							menuUtils::plgns[Dlg.crntMnutplg].key.bAlt?BST_CHECKED:BST_UNCHECKED,0);
				SendMessage(GetDlgItem(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG),BM_SETCHECK,
							menuUtils::plgns[Dlg.crntMnutplg].key.bShft?BST_CHECKED:BST_UNCHECKED,0);
				int iSelctd = Dlg.GetCodeKey(menuUtils::plgns[Dlg.crntMnutplg].key.key);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),CB_SETCURSEL,iSelctd,0);
		}	}
		else if(Chld::iSelectedDlg==TKEYDLGTAILNUM+8)
		{	if(QView::numPlugins>0)
			{	SetDlgItemText( hDlg,IDC_EDIT_MODULE_NAME,QView::plgns[Dlg.crntQVplg].pathAndName);
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.totalAllovedQVPlg);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_PLUGINS,s);
				StringCchPrintf(s,MAX_PATH-1,L"%d",Dlg.crntQVplg);
				SetDlgItemText(hDlg,IDC_EDIT_CURRENT_PLUGIN,s);
				StringCchPrintf(s,MAX_PATH,L"%d",QView::numPlugins);
				SetDlgItemText(hDlg,IDC_EDIT_NUMBER_TOTAL,s);
			}
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_CTL_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_ALT_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_CHECK_SHIFT_PRESSED_CONFIG),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_COMBO_KEY_CONFIG),FALSE);
		}
		return 0;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK PanIconConfDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
int iSelctd=0;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; HWND prnt;prnt = GetParent(hDlg);if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rc);
		TITLEBARINFO ti;ti.cbSize=sizeof(ti);GetTitleBarInfo(GetParent(hDlg),&ti);
		MoveWindow(hDlg,rc.left+Chld::pos[0],
						rc.top+Chld::pos[1]-ti.rcTitleBar.top+ti.rcTitleBar.bottom,
						Chld::sz[0],Chld::sz[1],TRUE);
		wchar_t st[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_118,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC,st);
		return (INT_PTR)TRUE;
	
	case WM_COMMAND:
		if(IDC_COMBO_ICONS_SIZE_CONFIG==LOWORD(wParam))
		if(CBN_SELCHANGE==HIWORD(wParam))
		if(Chld::iSelectedDlg>46)
		{	iSelctd = (int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_GETCURSEL,0,0);
			SendMessageA(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_GETLBTEXT,iSelctd,(LPARAM)(LPSTR)st);
			Dlg.iIconSize[Chld::iSelectedDlg-47] = atoi((LPSTR)st);
			if(Chld::iSelectedDlg-47 < Dlg.iTotPanels)
				panel[Chld::iSelectedDlg-47].Render();
		}
		break;
	case WM_USER+6:
		iSelctd=0;
		for(int is=8; is<64; is++)
		{StringCchPrintf(st,MAX_PATH-1,L"%d",is);
		 SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_ADDSTRING,0,(LPARAM)st);
		 if(is==Dlg.iIconSize[Chld::iSelectedDlg-47])
			 iSelctd = is-8;
		}
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_ICONS_SIZE_CONFIG),CB_SETCURSEL,iSelctd,0);
		return 0;
	}

	return (INT_PTR)FALSE;
}

int Chld::pos[2]={220,6};
int Chld::sz[2]={334,410};
int Chld::iSelectedDlg = -1;
/*int Chld::resIds[TOT_CHILD_DLGS]={IDD_DIALOG_COMMON_CONFIG,//0
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,//10
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,//20
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_FONT_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,//30
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,//40
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_COLOR_PANEL_CONFIG,
									IDD_DIALOG_ICON_SIZE_CONFIG,
									IDD_DIALOG_ICON_SIZE_CONFIG,
									IDD_DIALOG_ICON_SIZE_CONFIG,
									IDD_DIALOG_ICON_SIZE_CONFIG,//50
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,//60
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,//70
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,//80
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,//90
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,//100
									IDD_DIALOG_HOT_KEY_CONFIG,
									IDD_DIALOG_HOT_KEY_CONFIG,//102
									IDD_DIALOG_CUSTOM_PANEL_ITEM,
									IDD_DIALOG_CUSTOM_PANEL_ITEM,
									IDD_DIALOG_CUSTOM_PANEL_ITEM,
									IDD_DIALOG_CUSTOM_PANEL_ITEM,
									IDD_DIALOG_DIRECT_LINK_OPTION_CONFIG,
									IDD_DIALOG_PLUGIN_OPTION_CONFIG,
									IDD_DIALOG_PLUGIN_OPTION_CONFIG,
									IDD_DIALOG_PLUGIN_OPTION_CONFIG,
									IDD_DIALOG_PLUGIN_OPTION_CONFIG//110
								};
DLGPROC Chld::chldDlgs[TOT_CHILD_DLGS]={CmnConfDlgProc,//0
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,//10
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,//20
										FntConfDlgProc,
										FntConfDlgProc,
										FntConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,//30
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,//40
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										ClrConfDlgProc,
										PanIconConfDlgProc,
										PanIconConfDlgProc,
										PanIconConfDlgProc,
										PanIconConfDlgProc,//50
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,//60
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,//70
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,//80
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,//90
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,//100
										HotKeyConfDlgProc,
										HotKeyConfDlgProc,//102
										CustomDefineDlgProc,
										CustomDefineDlgProc,
										CustomDefineDlgProc,
										CustomDefineDlgProc,
										DirectLinkOptnDlgProc,
										plgnOptnDlgProc,
										plgnOptnDlgProc,
										plgnOptnDlgProc,//110
										plgnOptnDlgProc
										};*/
VOID Chld::Change(int sel)
{	if(sel==14 || sel==37)//menu
		return;	
	if(sel<0)
	{	for(int i=0; i<REAL_TOT_CHILD_DLGS/*TOT_CHILD_DLGS*/; i++)
			ShowWindow(stdhDlg[i]/*chlds[i].hWnd*/, SW_HIDE);
	}
	else
	{	if(iSelectedDlg>-1)
			ShowWindow(stdhDlg[chlds[iSelectedDlg].iWnd],SW_HIDE);
		ShowWindow(stdhDlg[chlds[sel].iWnd],SW_SHOW);
		iSelectedDlg = sel;
		for(int i=0; i<TOT_CHILD_DLGS; i++)
		{	if(i==sel)
			{	//ShowWindow(chlds[i].hWnd, SW_SHOW);
				HFONT hf;
				if(i>0 && i<5)
				{	hf = CreateFontIndirect(&Dlg.panFnts[i-1]);
					ShowFontFamilly(stdhDlg[chlds[i].iWnd],IDC_STATIC1,&Dlg.panFnts[i-1]);
					chlds[i].oldFont = Dlg.panFnts[i-1];
					for(int c=0; c<6; c++)
						chlds[i].oldColor[c] = Dlg.panRGBs[i-1][c];
				}
				else if(i>4 && i<14)//dlgs
				{	hf = CreateFontIndirect(&Dlg.dlgFnts[i-5]);
					ShowFontFamilly(stdhDlg[chlds[i].iWnd],IDC_STATIC1,&Dlg.dlgFnts[i-5]);
					chlds[i].oldFont = Dlg.dlgFnts[i-5];
					for(int c=0; c<6; c++)
						chlds[i].oldColor[c] = Dlg.dlgRGBs[i-5][c];
				}
				else if(i==14)//menu
				{	hf = CreateFontIndirect(&Dlg.menuFnt);
					ShowFontFamilly(stdhDlg[chlds[i].iWnd],IDC_STATIC1,&Dlg.menuFnt);
					chlds[i].oldFont = Dlg.menuFnt;
					for(int c=0; c<6; c++)
						chlds[i].oldColor[c] = Dlg.menuRGB[c];
				}
				else if(i>14 && i<19)//SrtBtns
				{	hf = CreateFontIndirect(&Dlg.srtBtnFnt[i-15]);
					ShowFontFamilly(stdhDlg[chlds[i].iWnd],IDC_STATIC1,&Dlg.srtBtnFnt[i-15]);
					chlds[i].oldFont = Dlg.srtBtnFnt[i-15];
					for(int c=0; c<6; c++)
						chlds[i].oldColor[c] = Dlg.srtBtnRGB[i-15][c];
				}
				else if(i>18 && i<23)//edits
				{	hf = CreateFontIndirect(&Dlg.editFnt[i-19]);
					ShowFontFamilly(stdhDlg[chlds[i].iWnd],IDC_STATIC1,&Dlg.editFnt[i-19]);
					chlds[i].oldFont = Dlg.editFnt[i-19];
					for(int c=0; c<6; c++)
						chlds[i].oldColor[c] = Dlg.editRGB[i-19][c];
				}
				else if(i==23)//cmndBtns
				{	hf = CreateFontIndirect(&Dlg.cmdBtnFnt);
					ShowFontFamilly(stdhDlg[chlds[i].iWnd],IDC_STATIC1,&Dlg.cmdBtnFnt);
					chlds[i].oldFont = Dlg.cmdBtnFnt;
					for(int c=0; c<6; c++)
						chlds[i].oldColor[c] = Dlg.cmdBtnRGB[c];
				}
				else
					hf = CreateFontIndirect(&chlds[i].oldFont);
				chlds[i].SetFontFont(hf);
				DeleteObject(hf);
				SendMessage(stdhDlg[chlds[i].iWnd],WM_USER+6,0,0);
			}
			//else
			//	ShowWindow(chlds[i].hWnd, SW_HIDE);
}	}	}

VOID Chld::Create(HWND hDlg)
{	iSelectedDlg=0;

	stdhDlg[0]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_COMMON_CONFIG),hDlg,CmnConfDlgProc);//0
	stdhDlg[1]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_FONT_PANEL_CONFIG),hDlg,FntConfDlgProc);//1-23
	stdhDlg[2]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_COLOR_PANEL_CONFIG),hDlg,ClrConfDlgProc);//24-46
	stdhDlg[3]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_ICON_SIZE_CONFIG),hDlg,PanIconConfDlgProc);//47-50
	stdhDlg[4]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_HOT_KEY_CONFIG),hDlg,HotKeyConfDlgProc);//51-TKEYDLGTAILNUM
	stdhDlg[5]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_CUSTOM_PANEL_ITEM),hDlg,CustomDefineDlgProc);//TKEYDLGTAILNUM+1 - TKEYDLGTAILNUM+4
	stdhDlg[6]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_DIRECT_LINK_OPTION_CONFIG),hDlg,DirectLinkOptnDlgProc);//TKEYDLGTAILNUM+5
	stdhDlg[7]=CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_PLUGIN_OPTION_CONFIG),hDlg,plgnOptnDlgProc);//TKEYDLGTAILNUM+6  TKEYDLGTAILNUM+9
	if((!stdhDlg[0]) || (!stdhDlg[1]) || (!stdhDlg[2]) || (!stdhDlg[3]) ||
	   (!stdhDlg[4]) || (!stdhDlg[5]) || (!stdhDlg[6]) || (!stdhDlg[7])  )
	{	Err::msg(NULL,-1,L"CreateDialog");
		return;
	}

	for(int i=0; i<TOT_CHILD_DLGS; i++)
	{	/*chlds[i].hWnd=CreateDialog(hInst,MAKEINTRESOURCE(resIds[i]),hDlg,chldDlgs[i]);
		if(!chlds[i].hWnd)
		{	Err::msg(NULL,-1,L"CreateDialog");
			return;
		}*/
		if(0==i) chlds[i].iWnd = 0;
		else if(i>0 && i<24) chlds[i].iWnd = 1;
		else if(i>23 && i<47) chlds[i].iWnd = 2;
		else if(i>46 && i<TKEYDLGBGNNUM) chlds[i].iWnd = 3;//else if(i>46 && i<51) chlds[i].iWnd = 3;
		else if(i>TKEYDLGBGNNUM-1 && i<TKEYDLGTAILNUM) chlds[i].iWnd = 4;//Hot keys
		else if(i>TKEYDLGTAILNUM-1 && i<TKEYDLGTAILNUM+4) chlds[i].iWnd = 5;//Custom items
		else if(TKEYDLGTAILNUM+4==i) chlds[i].iWnd = 6;//Direct link
		else if(i>TKEYDLGTAILNUM+4 && i<TKEYDLGTAILNUM+9) chlds[i].iWnd = 7;//arch/search/menUt plugins/quick view

/*		if(1==i||i==5||i==14||i==24||i==28||i==37||i==TKEYDLGTAILNUM)
			ShowWindow(GetDlgItem(chlds[i].hWnd,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),SW_HIDE);
		if(i>4 && i<47)
		{	wchar_t s[MAX_PATH];
			if(i>37)
			{	LoadString(hInst,IDS_STRINGSW_133,s,MAX_PATH);
				SetWindowText(GetDlgItem(chlds[i].hWnd,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),s);//"Copy fr.menus");
			}
			else if(i>28)
			{	LoadString(hInst,IDS_STRINGSW_132,s,MAX_PATH);
				SetWindowText(GetDlgItem(chlds[i].hWnd,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),s);//"Copy fr.1-st dlg.");
			}
			else if(i>24)
			{	LoadString(hInst,IDS_STRINGSW_131,s,MAX_PATH);
				SetWindowText(GetDlgItem(chlds[i].hWnd,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),s);//"Copy fr.1-st panel");
			}
			else if(i>14)
			{	LoadString(hInst,IDS_STRINGSW_133,s,MAX_PATH);
				SetWindowText(GetDlgItem(chlds[i].hWnd,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),s);//"Copy fr.menus");
			}
			else
			{	LoadString(hInst,IDS_STRINGSW_132,s,MAX_PATH);
				SetWindowText(GetDlgItem(chlds[i].hWnd,IDC_BUTTON_COPY_FROM_FIRST_FONT_CONFIG),s);//"Copy fr.1-st dlg.");
		}	}
*/		GetObject((HFONT)SendMessage(GetDlgItem(stdhDlg[chlds[i].iWnd],IDC_STATIC2),WM_GETFONT,0,0),
				  sizeof(LOGFONTW),&chlds[i].oldFont);
		//for(int c=0; c<6; c++)
		//	chlds[i].oldColor[c]=0;
		iSelectedDlg++;
	}
	iSelectedDlg=-1;
}

LOGFONT* Chld::GetColorFont()
{
	switch(iSelectedDlg)
	{	case 24://panel[0]
			return &chlds[1].oldFont;
		case 25://panel[1]
			return &chlds[2].oldFont;
		case 26://panel[2]
			return &chlds[3].oldFont;
		case 27://panel[3]
			return &chlds[4].oldFont;
		case 28://"Copy master";
			return &chlds[5].oldFont;
		case 29://"Copy queue";
			return &chlds[6].oldFont;
		case 30://"Copy background queue";
			return &chlds[7].oldFont;
		case 31://"Create folder";
			return &chlds[8].oldFont;
		case 32://"Change font";
			return &chlds[9].oldFont;
		case 33://"Change color";
			return &chlds[10].oldFont;
		case 34://"Search";
			return &chlds[11].oldFont;
		case 35://"Select disk";
			return &chlds[12].oldFont;
		case 36://"Others";
			return &chlds[13].oldFont;
		case 37://"For menus";
			return &chlds[14].oldFont;
		case 38://"For sort buttons pan[0]";
			return &chlds[15].oldFont;
		case 39://"For sort buttons pan[1]";
			return &chlds[16].oldFont;
		case 40://"For sort buttons pan[2]";
			return &chlds[17].oldFont;
		case 41://"For sort buttons pan[3]";
			return &chlds[18].oldFont;
		case 42://"For edit controls pan[0]";
			return &chlds[19].oldFont;
		case 43://"For edit controls pan[1]";
			return &chlds[20].oldFont;
		case 44://"For edit controls pan[2]";
			return &chlds[21].oldFont;
		case 45://"For edit controls pan[3]";
			return &chlds[22].oldFont;
		case 46://"For command buttons";
			return &chlds[23].oldFont;
	}
	return &chlds[1].oldFont;
}

VOID Chld::SetDestColor(int colType,COLORREF *col)
{
int s=(int)(this-&chlds[0]);
	switch(s)
	{	case 24://panel[0]
			panel[0].ChangeColor(col-colType,0);
			break;
		case 25://panel[1]
			panel[1].ChangeColor(col-colType,0);
			break;
		case 26://panel[2]
			panel[2].ChangeColor(col-colType,0);
			break;
		case 27://panel[3]
			panel[3].ChangeColor(col-colType,0);
			break;
		case 28://"Copy master";
		case 29://"Copy queue";
		case 30://"Copy background queue";
		case 31://"Create folder";
			SendMessage(hTmpDlg,WM_USER+3,0,0);
			SendMessage(hTmpDlg,WM_USER+3,0,0);
			break;
		case 32://"Change font";
			SendMessage(hTmpDlg,WM_USER+4,0,0);
			break;
		case 33://"Change color";
			SendMessage(hTmpDlg,WM_USER+4,0,0);
			break;
		case 34://"Search";
			SendMessage(hTmpDlg,WM_USER+3,0,0);
			break;
		case 35://"Select disk";
			SendMessage(hTmpDlg,WM_USER+3,0,0);
			break;
		case 36://"Others";
			SendMessage(hTmpDlg,WM_USER+3,0,0);
			break;
		case 37://"For menus";
			break;
		case 38://"For sort buttons pan[0]";
			panel[0].ChangeColor(col-colType,2);
			break;
		case 39://"For sort buttons pan[1]";
			panel[1].ChangeColor(col-colType,2);
			break;
		case 40://"For sort buttons pan[2]";
			panel[2].ChangeColor(col-colType,2);
			break;
		case 41://"For sort buttons pan[3]";
			panel[3].ChangeColor(col-colType,2);
			break;
		case 42://"For edit controls pan[0]";
			panel[0].ChangeColor(col-colType,1);
			break;
		case 43://"For edit controls pan[0]";
			panel[1].ChangeColor(col-colType,1);
			break;
		case 44://"For edit controls pan[0]";
			panel[2].ChangeColor(col-colType,1);
			break;
		case 45://"For edit controls pan[0]";
			panel[3].ChangeColor(col-colType,1);
			break;
		case 46://"For command buttons";
			DeleteObject(brCmdBtns);
			brCmdBtns=CreateSolidBrush(conf::Dlg.cmdBtnRGB[2]);
			DeleteObject(brHtBk);
			brHtBk=CreateSolidBrush(conf::Dlg.cmdBtnRGB[5]);
			for(int b=0; b<7; b++)
				RedrawWindow(btns[b],NULL,NULL,RDW_INVALIDATE);//RedrawWindow(btns[b].GetHWND(),NULL,NULL,RDW_INVALIDATE);
			break;
}	}

VOID Chld::SetDestFont(HFONT hf,LOGFONT* lf)
{
int s=(int)(this-&chlds[0]);
	switch(s)
	{	case 0://Common config
			break;
		case 1://panel[0]
			panel[0].ChangeFont(lf,0);
			break;
		case 2://panel[1]
			panel[1].ChangeFont(lf,0);
			break;
		case 3://panel[2]
			panel[2].ChangeFont(lf,0);
			break;
		case 4://panel[3]
			panel[3].ChangeFont(lf,0);
			break;
		case 5://"Copy master";
		case 6://"Copy queue";
		case 7://"Copy background queue";
		case 8://"Create folder";
			SendMessage(hTmpDlg,WM_USER+2,0,0);
			break;
		case 9://"Change font";
			SendMessage(hTmpDlg,WM_USER+3,0,0);
			break;
		case 10://"Change color";
			break;
		case 11://"Search";
			SendMessage(hTmpDlg,WM_USER+2,0,0);
			break;
		case 12://"Select disk";
			SendMessage(hTmpDlg,WM_USER+2,0,0);
			break;
		case 13://"Others";
			SendMessage(hTmpDlg,WM_USER+2,0,0);
			break;
		case 14://"For menus";
			break;
		case 15://"For sort buttons pan[0]";
			Dlg.srtBtnFnt[0] = *lf;
			panel[0].ChangeFont(lf,2);
			break;
		case 16://"For sort buttons pan[1]";
			Dlg.srtBtnFnt[1] = *lf;
			panel[1].ChangeFont(lf,2);
			break;
		case 17://"For sort buttons pan[2]";
			Dlg.srtBtnFnt[2] = *lf;
			panel[2].ChangeFont(lf,2);
			break;
		case 18://"For sort buttons pan[3]";
			Dlg.srtBtnFnt[3] = *lf;
			panel[3].ChangeFont(lf,2);
			break;
		case 19://"For edit controls pan[0]";
			Dlg.editFnt[0] = *lf;
			panel[0].ChangeFont(lf,1);
			break;
		case 20://"For edit controls pan[1]";
			Dlg.editFnt[1] = *lf;
			panel[1].ChangeFont(lf,1);
			break;
		case 21://"For edit controls pan[2]";
			Dlg.editFnt[2] = *lf;
			panel[2].ChangeFont(lf,1);
			break;
		case 22://"For edit controls pan[3]";
			Dlg.editFnt[3] = *lf;
			panel[3].ChangeFont(lf,1);
			break;
		case 23://"For command buttons";
			Dlg.cmdBtnFnt = *lf;
			DeleteObject(hfCmdBtns);
			hfCmdBtns=CreateFontIndirect(&conf::Dlg.cmdBtnFnt);
			//SetFnt(&hfCmdBtns);//To MyShell
			for(int b=0; b<7; b++)
				RedrawWindow(btns[b],NULL,NULL,RDW_INVALIDATE);//RedrawWindow(btns[b].GetHWND(),NULL,NULL,RDW_INVALIDATE);
			break;
}	}

VOID Chld::SetFontFont(HFONT hf)
{	SendMessage(GetDlgItem(stdhDlg[iWnd],IDC_STATIC2),WM_SETFONT,(WPARAM)hf,TRUE);
	SendMessage(GetDlgItem(stdhDlg[iWnd],IDC_STATIC3),WM_SETFONT,(WPARAM)hf,TRUE);
	SendMessage(GetDlgItem(stdhDlg[iWnd],IDC_STATIC5),WM_SETFONT,(WPARAM)hf,TRUE);
}

VOID Chld::Destroy()
{	for(int i=0; i<REAL_TOT_CHILD_DLGS/*TOT_CHILD_DLGS*/; i++)
	{	DestroyWindow(stdhDlg[i]/*chlds[i].hWnd*/);
		stdhDlg[i]=0;//chlds[i].hWnd = NULL;
}	}

VOID AddCopyDlgFiltrStr(wchar_t *s)//CopyOperation uchun
{
BOOL bFindEmpty = FALSE;
	for(int i=0; i<25; i++)
	{	if(0==fileCopyDlgFiltrStrs[i][0])
		{	StringCchPrintf(fileCopyDlgFiltrStrs[i],MAX_PATH/4,s);
			bFindEmpty = TRUE;
			break;
		} else
		{	if(0==wcscmp(fileCopyDlgFiltrStrs[i],s))
				return;//Already exist;
	}	}
	if(!bFindEmpty)//shift exists:
	{	for(int i=1; i<25; i++)
			StringCchPrintf(fileCopyDlgFiltrStrs[i-1],MAX_PATH/4,fileCopyDlgFiltrStrs[i]);
		StringCchPrintf(fileCopyDlgFiltrStrs[24],MAX_PATH/4,s);
}	}

HTREEITEM InsTreeNode(HWND tree,int strId,HTREEITEM root,int lParam)
{
TVITEMW tvi;TVINSERTSTRUCTW tvins;HTREEITEM htiRet; 
	tvi.mask = TVIF_TEXT|TVIF_PARAM;// | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvins.hInsertAfter = TVI_ROOT; 
	wchar_t s[128];int l=0;
	if(lParam>50 && lParam<109)
	{	StringCchPrintf(s,128,L"%d.",lParam-50);
		l=MyStringLength(s,128);
		l+=LoadString(hInst,strId,&s[l],128-l);
	}else LoadString(hInst,strId,s,128);
	tvins.hParent = root;
	tvi.cchTextMax = l;//MyStringLength(s,128);
	tvi.pszText = s;//"Common";
	tvi.lParam = lParam;
	tvins.item = tvi;
	//htiRet = TreeView_InsertItem(tree,&tvins);
	htiRet = (HTREEITEM)SendMessage(tree,TVM_INSERTITEM,0,(LPARAM)&tvins);
	return htiRet;
}

HTREEITEM InsTreeNodeChSym(HWND tree,int strId,HTREEITEM root,int lParam,char c)
{
TVITEMW tvi;TVINSERTSTRUCTW tvins;HTREEITEM htiRet; 
	tvi.mask = TVIF_TEXT|TVIF_PARAM;// | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvins.hInsertAfter = TVI_ROOT; 

	wchar_t s[128];LoadString(hInst,strId,s,128);
	s[0]=c;
	tvins.hParent = root;
	tvi.cchTextMax = MyStringLength(s,128);
	tvi.pszText = s;//"Common";
	tvi.lParam = lParam;
	tvins.item = tvi;
	//htiRet = TreeView_InsertItem(tree,&tvins);
	htiRet = (HTREEITEM)SendMessage(tree,TVM_INSERTITEM,0,(LPARAM)&tvins);
	return htiRet;
}

BOOL FillConfigDlgLeftTree(HWND hDlg)
{
	HWND tree = GetDlgItem(hDlg,IDC_TREE_CONFIGURATION);
hTree.cmn=//HTREEITEM cmn =
	InsTreeNode(tree,IDS_STRINGSW_75,TVI_ROOT,0);//"Common";
hTree.fntNode=//HTREEITEM fntNode =
	InsTreeNode(tree,IDS_STRINGSW_134,TVI_ROOT,-1);//"Fonts";
hTree.fntPanel=//HTREEITEM fntPanel = 
	InsTreeNode(tree,IDS_STRINGSW_135,hTree.fntNode,-2);//"For panels";
hTree.fntPanelChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.fntPanel,1);//"1-panel";
hTree.fntPanelChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntPanel,2,'2');//"2-panel";
hTree.fntPanelChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntPanel,3,'3');//"3-panel";
hTree.fntPanelChlds[3]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntPanel,4,'4');//"4-panel";
hTree.fntDialogs = InsTreeNode(tree,IDS_STRINGSW_137,hTree.fntNode,-3);//"For dialogs";
hTree.fntDialogChlds[0]=InsTreeNode(tree,IDS_STRINGSW_138,hTree.fntDialogs,5);//"Copy master";
hTree.fntDialogChlds[1]=InsTreeNode(tree,IDS_STRINGSW_139,hTree.fntDialogs,6);//"Copy queue";
hTree.fntDialogChlds[2]=InsTreeNode(tree,IDS_STRINGSW_140,hTree.fntDialogs,7);//"Copy background queue";
hTree.fntDialogChlds[3]=InsTreeNode(tree,IDS_STRINGSW_141,hTree.fntDialogs,8);//"Create folder";
hTree.fntDialogChlds[4]=InsTreeNode(tree,IDS_STRINGSW_142,hTree.fntDialogs,9);//"Change font";
hTree.fntDialogChlds[5]=InsTreeNode(tree,IDS_STRINGSW_143,hTree.fntDialogs,10);//"Change color";
hTree.fntDialogChlds[6]=InsTreeNode(tree,IDS_STRINGSW_144,hTree.fntDialogs,11);//"Search";
hTree.fntDialogChlds[7]=InsTreeNode(tree,IDS_STRINGSW_145,hTree.fntDialogs,12);//"Select disk";
hTree.fntDialogChlds[8]=InsTreeNode(tree,IDS_STRINGSW_146,hTree.fntDialogs,13);//"Others";
//HTREEITEM fntMenus = 
//	InsTreeNode(tree,IDS_STRINGSW_147,fntNode,14);//"For menus";
//	if(winVerMajor>5)
//		TreeView_SetItemState(tree,fntMenus,TVIS_EX_DISABLED,TVIS_EX_DISABLED);
hTree.fntSrtBtns = InsTreeNode(tree,IDS_STRINGSW_148,hTree.fntNode,-4);//"For sort buttons";
hTree.fntSrtBtnChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.fntSrtBtns,15);//"1-panel";
hTree.fntSrtBtnChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntSrtBtns,16,'2');//"2-panel";
hTree.fntSrtBtnChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntSrtBtns,17,'3');//"3-panel";
hTree.fntSrtBtnChlds[3]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntSrtBtns,18,'4');//"4-panel";
hTree.fntEdtCntrl =	InsTreeNode(tree,IDS_STRINGSW_149,hTree.fntNode,-5);//"For edit controls";
hTree.fntEdtCntrlChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.fntEdtCntrl,19);//"1-panel";
hTree.fntEdtCntrlChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntEdtCntrl,20,'2');//"2-panel";
hTree.fntEdtCntrlChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntEdtCntrl,21,'3');//"3-panel";
hTree.fntEdtCntrlChlds[4]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.fntEdtCntrl,22,'4');//"4-panel";
hTree.fntCmndBtns=InsTreeNode(tree,IDS_STRINGSW_150,hTree.fntNode,23);//"For command buttons";
	TreeView_Expand(tree,hTree.fntNode,TVE_EXPAND);//Fonts


hTree.clrNode = InsTreeNode(tree,IDS_STRINGSW_151,TVI_ROOT,-6);//"Colors";
hTree.clrPanel = InsTreeNode(tree,IDS_STRINGSW_135,hTree.clrNode,-7);//"For panels";
hTree.clrPanelChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.clrPanel,24);//"1-panel";
hTree.clrPanelChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrPanel,25,'2');//"2-panel";
hTree.clrPanelChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrPanel,26,'3');//"3-panel";
hTree.clrPanelChlds[3]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrPanel,27,'4');//"4-panel";
hTree.clrDialogs = InsTreeNode(tree,IDS_STRINGSW_137,hTree.clrNode,-8);//"For dialogs";
hTree.clrDialogChlds[0]=InsTreeNode(tree,IDS_STRINGSW_138,hTree.clrDialogs,28);//"Copy master";
hTree.clrDialogChlds[1]=InsTreeNode(tree,IDS_STRINGSW_139,hTree.clrDialogs,29);//"Copy queue";
hTree.clrDialogChlds[2]=InsTreeNode(tree,IDS_STRINGSW_140,hTree.clrDialogs,30);//"Copy background queue";
hTree.clrDialogChlds[3]=InsTreeNode(tree,IDS_STRINGSW_141,hTree.clrDialogs,31);//"Create folder";
hTree.clrDialogChlds[4]=InsTreeNode(tree,IDS_STRINGSW_142,hTree.clrDialogs,32);//"Change font";
hTree.clrDialogChlds[5]=InsTreeNode(tree,IDS_STRINGSW_143,hTree.clrDialogs,33);//"Change color";
hTree.clrDialogChlds[6]=InsTreeNode(tree,IDS_STRINGSW_144,hTree.clrDialogs,34);//"Search";
hTree.clrDialogChlds[7]=InsTreeNode(tree,IDS_STRINGSW_145,hTree.clrDialogs,35);//"Select disk";
hTree.clrDialogChlds[8]=InsTreeNode(tree,IDS_STRINGSW_146,hTree.clrDialogs,36);//"Others";
//HTREEITEM clrMenus = 
//	InsTreeNode(tree,IDS_STRINGSW_147,clrNode,37);//"For menus";
//	if(winVerMajor>5)
//		TreeView_SetItemState(tree,clrMenus,TVIS_EX_DISABLED,TVIS_EX_DISABLED);
hTree.clrSrtBtns = InsTreeNode(tree,IDS_STRINGSW_148,hTree.clrNode,-9);//"For sort buttons";
hTree.clrSrtBtnChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.clrSrtBtns,38);//"1-panel";
hTree.clrSrtBtnChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrSrtBtns,39,'2');//"2-panel";
hTree.clrSrtBtnChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrSrtBtns,40,'3');//"3-panel";
hTree.clrSrtBtnChlds[3]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrSrtBtns,41,'4');//"4-panel";
hTree.clrEdtCntrl = InsTreeNode(tree,IDS_STRINGSW_149,hTree.clrNode,-10);//"For edit controls";
hTree.clrEdtCntrlChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.clrEdtCntrl,42);//"1-panel";
hTree.clrEdtCntrlChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrEdtCntrl,43,'2');//"2-panel";
hTree.clrEdtCntrlChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrEdtCntrl,44,'3');//"3-panel";
hTree.clrEdtCntrlChlds[3]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.clrEdtCntrl,45,'4');//"4-panel";
	TreeView_Expand(tree,hTree.clrNode,TVE_EXPAND);//Colors
hTree.clrCmndBtns=InsTreeNode(tree,IDS_STRINGSW_150,hTree.clrNode,46);//"For command buttons";
	TreeView_Expand(tree,hTree.clrNode,TVE_EXPAND);//Fonts


hTree.iconNode = InsTreeNode(tree,IDS_STRINGSW_152,TVI_ROOT,-11);//"Icons";
hTree.iconPanel = InsTreeNode(tree,IDS_STRINGSW_135,hTree.iconNode,-12);//"For panels";
hTree.iconPanelChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.iconPanel,47);//"1-panel";
hTree.iconPanelChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.iconPanel,48,'2');//"2-panel";
hTree.iconPanelChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.iconPanel,49,'3');//"3-panel";
hTree.iconPanelChlds[3]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.iconPanel,50,'4');//"4-panel";
	TreeView_Expand(tree,hTree.iconNode,TVE_EXPAND);//Fonts


hTree.hotKeys = InsTreeNode(tree,IDS_STRINGSW_159,TVI_ROOT,-13);//"Hot keys";
hTree.hotKeyChlds[0]=InsTreeNode(tree,IDS_STRINGSW_112,hTree.hotKeys,51);//"Select all";
hTree.hotKeyChlds[1]=InsTreeNode(tree,IDS_STRINGSW_168,hTree.hotKeys,52);//"Create tab sheet" 168
hTree.hotKeyChlds[2]=InsTreeNode(tree,IDS_STRINGSW_169,hTree.hotKeys,53);//"Delete tab sheet" 169
hTree.hotKeyChlds[3]=InsTreeNode(tree,IDS_STRINGSW_170,hTree.hotKeys,54);//"Help"
hTree.hotKeyChlds[4]=InsTreeNode(tree,IDS_STRINGSW_171,hTree.hotKeys,55);//"Disk select panel 1"
hTree.hotKeyChlds[5]=InsTreeNode(tree,IDS_STRINGSW_172,hTree.hotKeys,56);//"Disk select panel 2"
hTree.hotKeyChlds[6]=InsTreeNode(tree,IDS_STRINGSW_173,hTree.hotKeys,57);//"Disk select panel 3"
hTree.hotKeyChlds[7]=InsTreeNode(tree,IDS_STRINGSW_174,hTree.hotKeys,58);//"Disk select panel 4"
hTree.hotKeyChlds[8]=InsTreeNode(tree,IDS_STRINGSW_175,hTree.hotKeys,59);//"Edit name"
hTree.hotKeyChlds[9]=InsTreeNode(tree,IDS_STRINGSW_156,hTree.hotKeys,60);//"View"
hTree.hotKeyChlds[10]=InsTreeNode(tree,IDS_STRINGSW_157,hTree.hotKeys,61);//"Edit"
hTree.hotKeyChlds[11]=InsTreeNode(tree,IDS_STRINGSW_36,hTree.hotKeys,62);//"Copy"
hTree.hotKeyChlds[12]=InsTreeNode(tree,IDS_STRINGSW_176,hTree.hotKeys,63);//"Rename-move"
hTree.hotKeyChlds[13]=InsTreeNode(tree,IDS_STRINGSW_144,hTree.hotKeys,64);//"Search via F7 "
hTree.hotKeyChlds[14]=InsTreeNode(tree,IDS_STRINGSW_141,hTree.hotKeys,65);//"Create folder"
hTree.hotKeyChlds[15]=InsTreeNode(tree,IDS_STRINGSW_177,hTree.hotKeys,66);//"Folder up"
hTree.hotKeyChlds[16]=InsTreeNode(tree,IDS_STRINGSW_178,hTree.hotKeys,67);//"Fast select"
hTree.hotKeyChlds[17]=InsTreeNode(tree,IDS_STRINGSW_179,hTree.hotKeys,68);//"Change hot selected"
hTree.hotKeyChlds[18]=InsTreeNode(tree,IDS_STRINGSW_180,hTree.hotKeys,69);//"Select items"
hTree.hotKeyChlds[19]=InsTreeNode(tree,IDS_STRINGSW_181,hTree.hotKeys,70);//"Page up" 1 - knopka
hTree.hotKeyChlds[20]=InsTreeNode(tree,IDS_STRINGSW_182,hTree.hotKeys,71);//"Page down" 1 - knopka
hTree.hotKeyChlds[21]=InsTreeNode(tree,IDS_STRINGSW_183,hTree.hotKeys,72);//"Goto end"
hTree.hotKeyChlds[22]=InsTreeNode(tree,IDS_STRINGSW_184,hTree.hotKeys,73);//"Goto begin"
hTree.hotKeyChlds[23]=InsTreeNode(tree,IDS_STRINGSW_185,hTree.hotKeys,74);//"Up"
hTree.hotKeyChlds[24]=InsTreeNode(tree,IDS_STRINGSW_186,hTree.hotKeys,75);//"Down"
hTree.hotKeyChlds[25]=InsTreeNode(tree,IDS_STRINGSW_191,hTree.hotKeys,76);//"Escape"
hTree.hotKeyChlds[26]=InsTreeNode(tree,IDS_STRINGSW_2,hTree.hotKeys,77);//"Exit Alt+F4"
hTree.hotKeyChlds[27]=InsTreeNode(tree,IDS_STRINGSW_192,hTree.hotKeys,78);//"PgUp 2-key"
hTree.hotKeyChlds[28]=InsTreeNode(tree,IDS_STRINGSW_193,hTree.hotKeys,79);//"PgDwn 2-key"
hTree.hotKeyChlds[29]=InsTreeNode(tree,IDS_STRINGSW_67,hTree.hotKeys,80);//"Properties"
hTree.hotKeyChlds[30]=InsTreeNode(tree,IDS_STRINGSW_194,hTree.hotKeys,81);//"Enter Пуск Kirish"
hTree.hotKeyChlds[31]=InsTreeNode(tree,IDS_STRINGSW_195,hTree.hotKeys,82);//"Select Page Up"
hTree.hotKeyChlds[32]=InsTreeNode(tree,IDS_STRINGSW_196,hTree.hotKeys,83);//"Select Page Dwn"
hTree.hotKeyChlds[33]=InsTreeNode(tree,IDS_STRINGSW_197,hTree.hotKeys,84);//"Select Page Up,2-key"
hTree.hotKeyChlds[34]=InsTreeNode(tree,IDS_STRINGSW_198,hTree.hotKeys,85);//"Select Page Dwn,2-key"
hTree.hotKeyChlds[35]=InsTreeNode(tree,IDS_STRINGSW_199,hTree.hotKeys,86);//"Select Goto end"
hTree.hotKeyChlds[36]=InsTreeNode(tree,IDS_STRINGSW_200,hTree.hotKeys,87);//"Select Goto begin"
hTree.hotKeyChlds[37]=InsTreeNode(tree,IDS_STRINGSW_201,hTree.hotKeys,88);//"Select, Up"
hTree.hotKeyChlds[38]=InsTreeNode(tree,IDS_STRINGSW_202,hTree.hotKeys,89);//"Select, Down"
hTree.hotKeyChlds[39]=InsTreeNode(tree,IDS_STRINGSW_203,hTree.hotKeys,90);//"Delete"
hTree.hotKeyChlds[40]=InsTreeNode(tree,IDS_STRINGSW_204,hTree.hotKeys,91);//"Delete,2-key"
hTree.hotKeyChlds[41]=InsTreeNode(tree,IDS_STRINGSW_205,hTree.hotKeys,92);//"Unrestorable delete"
hTree.hotKeyChlds[42]=InsTreeNode(tree,IDS_STRINGSW_206,hTree.hotKeys,93);//"Unrestorable delete,2-key"
hTree.hotKeyChlds[43]=InsTreeNode(tree,IDS_STRINGSW_208,hTree.hotKeys,94);//"Insert to command line"
hTree.hotKeyChlds[44]=InsTreeNode(tree,IDS_STRINGSW_235,hTree.hotKeys,95);//"Add to archive"
hTree.hotKeyChlds[45]=InsTreeNode(tree,IDS_STRINGSW_241,hTree.hotKeys,96);//"Entrance to archive,select unpack plugin"
hTree.hotKeyChlds[46]=InsTreeNode(tree,IDS_STRINGSW_361,hTree.hotKeys,97);//"View with...(from plugins)"
hTree.hotKeyChlds[47]=InsTreeNode(tree,IDS_STRINGSW_362,hTree.hotKeys,98);//"View with...(open)"
hTree.hotKeyChlds[48]=InsTreeNode(tree,IDS_STRINGSW_363,hTree.hotKeys,99);//"View with...(browse)"
hTree.hotKeyChlds[49]=InsTreeNode(tree,IDS_STRINGSW_364,hTree.hotKeys,100);//"Edit with...(from plugins)"
hTree.hotKeyChlds[50]=InsTreeNode(tree,IDS_STRINGSW_365,hTree.hotKeys,101);//"Edit with...(open)"
hTree.hotKeyChlds[51]=InsTreeNode(tree,IDS_STRINGSW_366,hTree.hotKeys,102);//"Edit with...(browse)"

hTree.hotKeyChlds[52]=InsTreeNode(tree,IDS_STRINGSW_253,hTree.hotKeys,103);//Ctrl + C
hTree.hotKeyChlds[53]=InsTreeNode(tree,IDS_STRINGSW_251,hTree.hotKeys,104);//Ctrl + X
hTree.hotKeyChlds[54]=InsTreeNode(tree,IDS_STRINGSW_252,hTree.hotKeys,105);//Ctrl + V
hTree.hotKeyChlds[55]=InsTreeNode(tree,IDS_STRINGSW_367,hTree.hotKeys,106);//Ctrl + Alt + C
hTree.hotKeyChlds[56]=InsTreeNode(tree,IDS_STRINGSW_368,hTree.hotKeys,107);//Ctrl + Alt + C
hTree.hotKeyChlds[57]=InsTreeNode(tree,IDS_STRINGSW_369,hTree.hotKeys,108);//Ctrl + Q

hTree.custPanItems = InsTreeNode(tree,IDS_STRINGSW_207,TVI_ROOT,-14);//"Custom panel item definition"
hTree.custPanItemChlds[0]=InsTreeNode(tree,IDS_STRINGSW_136,hTree.custPanItems,TKEYDLGTAILNUM);//"1-panel";
hTree.custPanItemChlds[1]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.custPanItems,TKEYDLGTAILNUM+1,'2');//"2-panel";
hTree.custPanItemChlds[2]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.custPanItems,TKEYDLGTAILNUM+2,'3');//"3-panel";
hTree.custPanItemChlds[3]=InsTreeNodeChSym(tree,IDS_STRINGSW_136,hTree.custPanItems,TKEYDLGTAILNUM+3,'4');//"4-panel";

	InsTreeNode(tree,IDS_STRINGSW_93,TVI_ROOT,TKEYDLGTAILNUM+4);//"Direct link";

hTree.plgns=InsTreeNode(tree,IDS_STRINGSW_242,TVI_ROOT,-15);//"Plugins";
	hTree.plgnChlds[0]=InsTreeNode(tree,IDS_STRINGSW_243,hTree.plgns,TKEYDLGTAILNUM+5);//"Archive";
	hTree.plgnChlds[1]=InsTreeNode(tree,IDS_STRINGSW_244,hTree.plgns,TKEYDLGTAILNUM+6);//"Search";
	hTree.plgnChlds[2]=InsTreeNode(tree,IDS_STRINGSW_19,hTree.plgns,TKEYDLGTAILNUM+7);//"Menu utils";
	hTree.plgnChlds[2]=InsTreeNode(tree,IDS_STRINGSW_369,hTree.plgns,TKEYDLGTAILNUM+8);//"Quick view";

/*	InsTreeNode(tree,IDS_STRINGSW_153,TVI_ROOT,0xffff);//"Assign commands";
	InsTreeNode(tree,IDS_STRINGSW_154,TVI_ROOT,0xffff);//"Command buttons";
HTREEITEM opertn = 
	InsTreeNode(tree,IDS_STRINGSW_155,TVI_ROOT,0xffff);//"Operations";
	InsTreeNode(tree,IDS_STRINGSW_156,opertn,0xffff);//"View";
	InsTreeNode(tree,IDS_STRINGSW_157,opertn,0xffff);//"Edit";
	InsTreeNode(tree,IDS_STRINGSW_158,TVI_ROOT,0xffff);//"Archive";*/

	TreeView_SelectItem(tree,hTree.cmn);//Common
	return TRUE;
}

VOID GetCopyDlgFiltrStr(HWND dlg, UINT comboBoxId)//CopyOperation uchun
{
BOOL bFindEmpty = FALSE;
	for(int i=0; i<25; i++)
	{	if(0==fileCopyDlgFiltrStrs[i][0])
			break;
		SendMessage(GetDlgItem(dlg,comboBoxId),CB_INSERTSTRING,(WPARAM)-1,(LPARAM)fileCopyDlgFiltrStrs[i]);
}	}

int ReadString(HANDLE f,wchar_t *str)
{
wchar_t *p=str;DWORD rd;
	do
	{	ReadFile(f,p,1,&rd,NULL);
		++p;
	} while(*(p-1) && rd!=0);
	if(rd==0){*p=0;return (int)(p-str-1);}
	return (int)(p-str-1);
}

int WriteString(HANDLE f,wchar_t *str)
{
wchar_t *p=str;DWORD rd;
	do
	{	WriteFile(f,p,1,&rd,NULL);
		++p;
	} while(*(p-1));
	return (int)(p-str-1);
}

BOOL CorrectPathForCapitals(wchar_t *pathAndName)
{BOOL r=FALSE;
wchar_t *p = wcsrchr(pathAndName,'\\');
	if(!p)return r;
	wchar_t nm[MAX_PATH];int lnm=MyStringCpy(nm,MAX_PATH,p+1);
	MyStringCpy(p+1,3,L"*.*");
	WIN32_FIND_DATA ff;
	HANDLE h=FindFirstFile(pathAndName,&ff);
	if(INVALID_HANDLE_VALUE==h)goto Endd;
	do
	{	if(0==_wcsnicmp(ff.cFileName,nm,lnm))
		{	MyStringCpy(nm,MAX_PATH,ff.cFileName);
			r = TRUE;
			break;
	}	}
	while(FindNextFile(h,&ff));
	FindClose(h);
Endd:
	MyStringCpy(p+1,MAX_PATH,nm);
	return r;
}

bool ReadDynamicPart(HANDLE f)
{
DWORD nr;
	for(int i=0; i<Dlg.iTotPanels; i++)
	{	if(iSheetTabs[i])
		{	sheet_path[i] = (wchar_t*)malloc(iSheetTabs[i]*2*MAX_PATH);
			wchar_t* ps	= &sheet_path[i][0];
			if(!ReadFile(f,sheet_path[i],iSheetTabs[i]*2*MAX_PATH,&nr,NULL))
				return FALSE;
			sheet_altPath[i] = (wchar_t*)malloc(iSheetTabs[i]*2*MAX_PATH);
			//ps	= &sheet_altPath[i][0];
			if(!ReadFile(f,sheet_altPath[i],iSheetTabs[i]*2*MAX_PATH,&nr,NULL))
				return FALSE;
			sheet_viewMenuBitsU32[i] = (U32*)malloc(iSheetTabs[i]*sizeof(U32));
			if(!ReadFile(f,&sheet_viewMenuBitsU32[i][0],iSheetTabs[i]*sizeof(U32),&nr,NULL))
				return FALSE;
	}	}
	for(int i=0; i<Dlg.iTotPanels; i++)
	{	//Dlg.cstmItms[i] = NULL;//malloc uchun;
		//Dlg.ReallocCustomItems(i,Dlg.iCustPanItemDefs[i],FALSE);
		if(Dlg.iCustPanItemDefs[i])
			Dlg.cstmItms[i] = (CstmItemDef*)malloc(Dlg.iCustPanItemDefs[i]*sizeof(CstmItemDef));
		for(int j=0; j<Dlg.iCustPanItemDefs[i]; j++)
		{	ReadFile(f,&Dlg.cstmItms[i][j],sizeof(TCstmItemDef),&nr,NULL);
			Dlg.cstmItms[i][j].hFnt = CreateFontIndirect(&Dlg.cstmItms[i][j].fnt);
			Dlg.cstmItms[i][j].txtBkBrsh = CreateSolidBrush(Dlg.cstmItms[i][j].clrRGBs[2]);
			Dlg.cstmItms[i][j].bkBrsh = CreateSolidBrush(Dlg.cstmItms[i][j].clrRGBs[0]);
			Dlg.cstmItms[i][j].hotBkBrsh = CreateSolidBrush(Dlg.cstmItms[i][j].clrRGBs[5]);
	}	}
	//Uzunligi mavhum bo'lgan strlar:
	ReadString(f,tmpPath);//WIN32_FIND_DATA ff;HANDLE hf=FindFirstFile(tmpPath,&ff);if(hf)FindClose(hf);else
	if(!IsDirExist(tmpPath))
		CreateAllDirs(tmpPath);
	if(!CorrectPathForCapitals(tmpPath))
		CreateDirectory(tmpPath,NULL);
	return true;
}

bool Read(wchar_t* fName)
{
DWORD nr;
	SYSTEMTIME st;GetSystemTime(&st);
	/*if(st.wSecond<55) st.wSecond += 5;
	else
	{	if(st.wMinute<59)
		{	st.wMinute += 1;
			st.wSecond = 5;
	}	}//uni else i kerakmas;*/
	SystemTimeToFileTime(&st,&strtTm);

//FILE *f; if(fopen_s(&f, fName, "rb"))

	FillManager::SetNillPaths();

	HANDLE f = MyFopenViaCrF(MyStringAddModulePath(L"Config\\Conf.bin"),L"r");
	if(!f) return false;
	//if(!fread(this, sizeof(Conf), 1, f))
	/*if(!ReadFile(f,this,sizeof(Conf),&nr,NULL))
	{	//fclose(f);
		CloseHandle(f);
		return false;
	}*/

	ReadFile(f,&wndLeft  ,sizeof(wndLeft  ),&nr,NULL);
	ReadFile(f,&wndTop   ,sizeof(wndTop   ),&nr,NULL);
	ReadFile(f,&wndWidth ,sizeof(wndWidth ),&nr,NULL);
	ReadFile(f,&wndHeight,sizeof(wndHeight),&nr,NULL);
	ReadFile(f,&scrWidth ,sizeof(scrWidth ),&nr,NULL);
	ReadFile(f,&scrHeight,sizeof(scrHeight),&nr,NULL);
	ReadFile(f,&nonClientAreaWidth,sizeof(nonClientAreaWidth),&nr,NULL);
	ReadFile(f,&nonClientAreaHeight,sizeof(nonClientAreaHeight),&nr,NULL);
	ReadFile(f,&scrRatio,sizeof(scrRatio),&nr,NULL);
	ReadFile(f,&wndRc    ,sizeof(wndRc),&nr,NULL);
	ReadFile(f,&MaxTextureHeight,sizeof(MaxTextureHeight),&nr,NULL);
	ReadFile(f,&bCancelAnimation[0],2*sizeof(bCancelAnimation[0]),&nr,NULL);
	ReadFile(f,&winVerMajor,sizeof(winVerMajor),&nr,NULL);
	ReadFile(f,&winVerMinor,sizeof(winVerMinor),&nr,NULL);
	ReadFile(f,&Bools,sizeof(TBools),&nr,NULL);
	ReadFile(f,&panel_path[0],MAX_PANELS*sizeof(panel_path[0]),&nr,NULL);

	for(int i=0; i<MAX_PANELS; i++)
	{	if(!IsDirExist(panel_path[i]))
			MyStringCpy(panel_path[i],259,L"C:\\*");
	}

	ReadFile(f,&resizeBarPersnt[0],(MAX_PANELS-1)*sizeof(resizeBarPersnt[0]),&nr,NULL);
	//ReadFile(f,&iOpponent[0],MAX_PANELS*sizeof(int),&nr,NULL);
	ReadFile(f,&Dlg,sizeof(Dlg),&nr,NULL);
	ReadFile(f,&folderAndFileMixingSortType[0],MAX_PANELS*sizeof(folderAndFileMixingSortType[0]),&nr,NULL);
	ReadFile(f,&folderSortType[0],MAX_PANELS*sizeof(folderSortType[0]),&nr,NULL);
	ReadFile(f,&fileSortType[0],MAX_PANELS*sizeof(fileSortType[0]),&nr,NULL);
	ReadFile(f,&iSheetTabs[0],MAX_PANELS*sizeof(iSheetTabs[0]),&nr,NULL);
	ReadFile(f,&iCrntSheetTab[0],MAX_PANELS*sizeof(iCrntSheetTab[0]),&nr,NULL);
	ReadFile(f,&iTabSheetRowCount[0],MAX_PANELS*sizeof(iTabSheetRowCount[0]),&nr,NULL);
	ReadFile(f,&sheet_path[0],MAX_PANELS*sizeof(sheet_path[0]),&nr,NULL);
	ReadFile(f,&sheet_altPath[0],MAX_PANELS*sizeof(sheet_altPath[0]),&nr,NULL);
	ReadFile(f,&viewMenuBitsU32[0],MAX_PANELS*sizeof(U32),&nr,NULL);
	ReadFile(f,&fileCopyDlgFiltrStrs[0],25*sizeof(fileCopyDlgFiltrStrs[0]),&nr,NULL);
	ReadFile(f,&iCopyDlgBtnPlaces,sizeof(iCopyDlgBtnPlaces),&nr,NULL);
	ReadFile(f,&bckgrndCpyDlgPozX,sizeof(bckgrndCpyDlgPozX),&nr,NULL);
	ReadFile(f,&bckgrndCpyDlgPozY,sizeof(bckgrndCpyDlgPozY),&nr,NULL);
	ReadFile(f,&bCopyDlgChkBtn[0],2*sizeof(bCopyDlgChkBtn[0]),&nr,NULL);
	ReadFile(f,&IP4Addr[0],4*sizeof(BYTE),&nr,NULL);
	ReadFile(f,&IP4_6_Port,sizeof(DWORD),&nr,NULL);
	ReadFile(f,&cmqLevPerCent,sizeof(char),&nr,NULL);
	ReadFile(f,&bEnumSubDir,sizeof(char),&nr,NULL);
		
	for(int i=0; i<Dlg.iTotPanels; i++)
	{	panel[i].folderAndFileMixingSortType=(FolderAndFileMixingSortType)folderAndFileMixingSortType[i];
		panel[i].folderSortType=(SortType)folderSortType[i];
		panel[i].fileSortType=(SortType)fileSortType[i];
		panel[i].viewMenuBitsU32=viewMenuBitsU32[i];
		panel[i].SetOpponent();
		//panel[i].iOpponent = iOpponent[i];
		if(Dlg.panFnts[i].lfWidth<0 || Dlg.panFnts[i].lfEscapement>180 || Dlg.panFnts[i].lfOrientation>180 ||
		   Dlg.panFnts[i].lfWeight>1000 || Dlg.panFnts[i].lfWeight<0 || Dlg.panFnts[i].lfFaceName[0]==0)
			Dlg.InitLOGFONT(&Dlg.panFnts[i],0);
		if(Dlg.srtBtnFnt[i].lfWidth<0 || Dlg.srtBtnFnt[i].lfEscapement>180 || Dlg.srtBtnFnt[i].lfOrientation>180 ||
		   Dlg.srtBtnFnt[i].lfWeight>1000 || Dlg.srtBtnFnt[i].lfWeight<0 || Dlg.srtBtnFnt[i].lfFaceName[0]==0)
			Dlg.InitLOGFONT(&Dlg.srtBtnFnt[i],0);
		if(Dlg.editFnt[i].lfWidth<0 || Dlg.editFnt[i].lfEscapement>180 || Dlg.editFnt[i].lfOrientation>180 ||
		   Dlg.editFnt[i].lfWeight>1000 || Dlg.editFnt[i].lfWeight<0 || Dlg.editFnt[i].lfFaceName[0]==0)
			Dlg.InitLOGFONT(&Dlg.editFnt[i],0);
	}
	for(int i=0; i<9; i++)
	{	if(Dlg.dlgFnts[i].lfWidth<0 || Dlg.dlgFnts[i].lfEscapement>180 || Dlg.dlgFnts[i].lfOrientation>180 ||
		   Dlg.dlgFnts[i].lfWeight>1000 || Dlg.dlgFnts[i].lfWeight<0 || Dlg.dlgFnts[i].lfFaceName[0]==0)
			Dlg.InitLOGFONT(&Dlg.dlgFnts[i],0);
	}
	if(Dlg.menuFnt.lfWidth<0 || Dlg.menuFnt.lfEscapement>180 || Dlg.menuFnt.lfOrientation>180 ||
	   Dlg.menuFnt.lfWeight>1000 || Dlg.menuFnt.lfWeight<0 || Dlg.menuFnt.lfFaceName[0]==0)
		Dlg.InitLOGFONT(&Dlg.menuFnt,0);
	if(Dlg.cmdBtnFnt.lfWidth<0 || Dlg.cmdBtnFnt.lfEscapement>180 || Dlg.cmdBtnFnt.lfOrientation>180 ||
	   Dlg.cmdBtnFnt.lfWeight>1000 || Dlg.cmdBtnFnt.lfWeight<0 || Dlg.cmdBtnFnt.lfFaceName[0]==0)
		Dlg.InitLOGFONT(&Dlg.cmdBtnFnt,0);

	bCancelAnimation[0] = bCancelAnimation[1];
	bCancelAnimation[1] = FALSE;
	if(wndLeft < 0)
	{	wndRc.left  += -wndLeft;
		wndRc.right += -wndLeft;
		wndLeft = 0;
		bCancelAnimation[1] = TRUE;
	}
	if(wndTop < 0)
	{	wndRc.top += -wndTop;
		wndRc.top += -wndTop;
		wndTop = 0;
		bCancelAnimation[1] = TRUE;
	}

	switch(Dlg.iStartPosition)
	{	case 0:break;//from save position:
		case 1://center screen:
			wndRc.left=wndLeft=(GetSystemMetrics(SM_CXSCREEN) - wndWidth)/2;
			wndRc.top=wndTop=(GetSystemMetrics(SM_CYSCREEN) - wndHeight)/2;
			wndRc.right = wndLeft + wndWidth;
			wndRc.bottom = wndTop + wndHeight;
			break;
		case 2://random position:
			int rndNum[2];
			srand(GetTickCount());
			rndNum[0] = rand();
			rndNum[1] = rand();

			double l=(double)(GetSystemMetrics(SM_CXSCREEN)-wndWidth);
			double h=(double)(GetSystemMetrics(SM_CYSCREEN)-wndHeight);

			wndLeft=(unsigned int)(l*(double)rndNum[0] / (double) RAND_MAX) + 1;
			wndTop=(unsigned int)(h*(double)rndNum[1] / (double) RAND_MAX) + 1;
			wndRc.right = wndLeft + wndWidth;
			wndRc.bottom = wndTop + wndHeight;
			break;
	}

	if(wndTop + wndHeight > scrHeight)
		bCancelAnimation[1] = TRUE;
	else if(wndLeft + wndWidth > scrWidth)
		bCancelAnimation[1] = TRUE;
	if(!bCancelAnimation[0] && bCancelAnimation[1])
		bCancelAnimation[0] = TRUE;

	//Windows version from OS:
	OSVERSIONINFOEX ov; ov.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(GetVersionEx((LPOSVERSIONINFO)&ov))
	{	winVerMajor = ov.dwMajorVersion;
		winVerMinor = ov.dwMinorVersion;
	}

	Bools.bAdmin = myWMI::RunningAsAdministrator()?1:0;

	if(!Bools.bAdmin)
	{
	}
	if(IsSSESupport())
	{	Bools.bSSE = 1;
		SetSSELib();
	} else Bools.bSSE = 0;

	ReadDynamicPart(f);

	//fclose(f);
	CloseHandle(f);
	return true;
}

/*HBITMAP ReadWindImgFrDisk(char* fName, HDC *bmpDC)
{
	HDC dc = GetDC(GetDesktopWindow());
	if(!dc) return NULL;
	*bmpDC = CreateCompatibleDC(dc);
	if(!*bmpDC) return NULL;
	HBITMAP bm = OpenBMPFile(fName, dc, *bmpDC);
	ReleaseDC(GetDesktopWindow(),dc);
	return bm;
}

bool SaveWindImgToDisk(HWND hwnd,char* fName)
{
	HDC dc = GetDC(GetDesktopWindow());
	if(!dc) return false;
	HDC memDC = CreateCompatibleDC(dc);
	if(!memDC) return false;
	HBITMAP memBM = CreateCompatibleBitmap(dc,wndWidth,wndHeight);
	HGDIOBJ oldObj = SelectObject(memDC, memBM);
	PBITMAPINFO bmi = CreateBitmapInfoStruct(hWnd, memBM);
	BitBlt(memDC,0,0,wndWidth,wndHeight,dc,wndLeft,wndTop,SRCCOPY);
	CreateBMPFile(hWnd,fName,bmi,memBM,memDC);
	SelectObject(memDC, oldObj);
	DeleteDC(memDC);
	DeleteObject(memBM);
	ReleaseDC(GetDesktopWindow(),dc);
	return true;
}*/

bool WriteDynamicPart(HANDLE f)
{
DWORD nr;
	for(int i=0; i<Dlg.iTotPanels; i++)
	{	int iTabSheets=TabCtrl_GetItemCount(sheetTab[i]);
		TCITEM item;item.mask=TCIF_PARAM;			
		for(int j=0; j<iTabSheets; j++)
		{	if(!TabCtrl_GetItem(sheetTab[i],j,&item))return FALSE;
			SheetTab* sheet = (SheetTab*)item.lParam;
			if(sheet)
				WriteFile(f,sheet->path,2*MAX_PATH,&nr,NULL);
			else
			{	wchar_t s[MAX_PATH]=L"C:";
				WriteFile(f,s,2*MAX_PATH,&nr,NULL);
		}	}
		for(int j=0; j<iTabSheets; j++)
		{	if(!TabCtrl_GetItem(sheetTab[i],j,&item))return FALSE;
			SheetTab* sheet = (SheetTab*)item.lParam;
			if(sheet)
				WriteFile(f,sheet->altPath,2*MAX_PATH,&nr,NULL);
			else
			{	wchar_t s[MAX_PATH]=L"";
				WriteFile(f,s,2*MAX_PATH,&nr,NULL);
		}	}
		for(int j=0; j<iTabSheets; j++)
		{	if(!TabCtrl_GetItem(sheetTab[i],j,&item))return FALSE;
			SheetTab* sheet = (SheetTab*)item.lParam;
			if(sheet)
				WriteFile(f,&sheet->viewMenuBitsU32,sizeof(U32),&nr,NULL);
			else
			{	U32 s=0;
				WriteFile(f,&s,sizeof(U32),&nr,NULL);
		}	}
		/*for(int j=0; j<iTabSheets; j++)
		{	if(!TabCtrl_GetItem(sheetTab[i],j,&item))return FALSE;
			SheetTab* sheet = (SheetTab*)item.lParam;
			if(sheet)
				WriteFile(f,sheet->archPath,MAX_PATH,&nr,NULL);
			else
			{	char s[4]="C:";
				WriteFile(f,s,4,&nr,NULL);
		}	}
		for(int j=0; j<iTabSheets; j++)
		{	if(!TabCtrl_GetItem(sheetTab[i],j,&item))return FALSE;
			SheetTab* sheet = (SheetTab*)item.lParam;
			if(sheet)
				WriteFile(f,&sheet->archPlgNum,sizeof(int),&nr,NULL);
			else
			{	int iplg=0;
				WriteFile(f,&iplg,sizeof(int),&nr,NULL);
	}	}*/	}
	for(int i=0; i<Dlg.iTotPanels; i++)
	{	for(int j=0; j<Dlg.iCustPanItemDefs[i]; j++)
			WriteFile(f,&Dlg.cstmItms[i][j],sizeof(TCstmItemDef),&nr,NULL);
	}

	//Uzunligi mavhum bo'lgan strlar:
	WriteString(f,tmpPath);
	return true;
}

bool Write(wchar_t* fName)
{
DWORD nr;
	bCancelAnimation[1] = FALSE;
	for(int i=0; i<Dlg.iTotPanels; i++)
	{	folderAndFileMixingSortType[i]=panel[i].folderAndFileMixingSortType;
		folderSortType[i]=panel[i].folderSortType;
		fileSortType[i]=panel[i].fileSortType;
		viewMenuBitsU32[i]=panel[i].viewMenuBitsU32;
		//iOpponent[i] = panel[i].iOpponent;
		if(IsDirExist(panel[i].GetPath()))
			MyStringCpy(panel_path[i],MAX_PATH-1,panel[i].GetPath());
		else
			bCancelAnimation[1] = TRUE;
		iSheetTabs[i] = TabCtrl_GetItemCount(sheetTab[i]);
		iCrntSheetTab[i] = TabCtrl_GetCurSel(sheetTab[i]);
		iTabSheetRowCount[i] = TabCtrl_GetRowCount(sheetTab[i]);
	}
	Dlg.iActivePanel = Panel::iActivePanel;
//FILE *f; if(fopen_s(&f, fName, "wb")) return false;
	HANDLE f = MyFopenViaCrF(MyStringAddModulePath(L"Config\\Conf.bin"),L"w");
	if(!f)return false;
	//if(!fwrite(this, sizeof(Conf), 1, f))
	/*if(!WriteFile(f,this,sizeof(Conf),&nr,NULL))
	{	//fclose(f);
		CloseHandle(f);
		return false;
	}*/
	WriteFile(f,&wndLeft  ,sizeof(wndLeft  ),&nr,NULL);
	WriteFile(f,&wndTop   ,sizeof(wndTop   ),&nr,NULL);
	WriteFile(f,&wndWidth ,sizeof(wndWidth ),&nr,NULL);
	WriteFile(f,&wndHeight,sizeof(wndHeight),&nr,NULL);
	WriteFile(f,&scrWidth ,sizeof(scrWidth ),&nr,NULL);
	WriteFile(f,&scrHeight,sizeof(scrHeight),&nr,NULL);
	WriteFile(f,&nonClientAreaWidth,sizeof(nonClientAreaWidth),&nr,NULL);
	WriteFile(f,&nonClientAreaHeight,sizeof(nonClientAreaHeight),&nr,NULL);
	WriteFile(f,&scrRatio,sizeof(scrRatio),&nr,NULL);
	WriteFile(f,&wndRc    ,sizeof(wndRc),&nr,NULL);
	WriteFile(f,&MaxTextureHeight,sizeof(MaxTextureHeight),&nr,NULL);
	WriteFile(f,&bCancelAnimation[0],2*sizeof(bCancelAnimation[0]),&nr,NULL);
	WriteFile(f,&winVerMajor,sizeof(winVerMajor),&nr,NULL);
	WriteFile(f,&winVerMinor,sizeof(winVerMinor),&nr,NULL);
	WriteFile(f,&Bools,sizeof(TBools),&nr,NULL);
	WriteFile(f,&panel_path[0],MAX_PANELS*sizeof(panel_path[0]),&nr,NULL);
	WriteFile(f,&resizeBarPersnt[0],(MAX_PANELS-1)*sizeof(resizeBarPersnt[0]),&nr,NULL);
	//WriteFile(f,&iOpponent[0],MAX_PANELS*sizeof(int),&nr,NULL);
	WriteFile(f,&Dlg,sizeof(Dlg),&nr,NULL);
	WriteFile(f,&folderAndFileMixingSortType[0],MAX_PANELS*sizeof(folderAndFileMixingSortType[0]),&nr,NULL);
	WriteFile(f,&folderSortType[0],MAX_PANELS*sizeof(folderSortType[0]),&nr,NULL);
	WriteFile(f,&fileSortType[0],MAX_PANELS*sizeof(fileSortType[0]),&nr,NULL);
	WriteFile(f,&iSheetTabs[0],MAX_PANELS*sizeof(iSheetTabs[0]),&nr,NULL);
	WriteFile(f,&iCrntSheetTab[0],MAX_PANELS*sizeof(iCrntSheetTab[0]),&nr,NULL);
	WriteFile(f,&iTabSheetRowCount[0],MAX_PANELS*sizeof(iTabSheetRowCount[0]),&nr,NULL);
	WriteFile(f,&sheet_path[0],MAX_PANELS*sizeof(sheet_path[0]),&nr,NULL);
	WriteFile(f,&sheet_altPath[0],MAX_PANELS*sizeof(sheet_altPath[0]),&nr,NULL);
	WriteFile(f,&viewMenuBitsU32[0],MAX_PANELS*sizeof(U32),&nr,NULL);
	WriteFile(f,&fileCopyDlgFiltrStrs[0],25*sizeof(fileCopyDlgFiltrStrs[0]),&nr,NULL);
	WriteFile(f,&iCopyDlgBtnPlaces,sizeof(iCopyDlgBtnPlaces),&nr,NULL);
	WriteFile(f,&bckgrndCpyDlgPozX,sizeof(bckgrndCpyDlgPozX),&nr,NULL);
	WriteFile(f,&bckgrndCpyDlgPozY,sizeof(bckgrndCpyDlgPozY),&nr,NULL);
	WriteFile(f,&bCopyDlgChkBtn[0],2*sizeof(bCopyDlgChkBtn[0]),&nr,NULL);
	WriteFile(f,&IP4Addr[0],4*sizeof(BYTE),&nr,NULL);
	WriteFile(f,&IP4_6_Port,sizeof(DWORD),&nr,NULL);
	WriteFile(f,&cmqLevPerCent,sizeof(char),&nr,NULL);
	WriteFile(f,&bEnumSubDir,sizeof(char),&nr,NULL);

	WriteDynamicPart(f);
	//fclose(f);
	CloseHandle(f);
	return true;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		Dlg.CopyTo(&DlgRestore);
		RECT rc; GetWindowRect(hDlg, &rc);
		int width,left,height,top;

		width = rc.right - rc.left;
		left = conf::wndLeft + (conf::wndWidth - width)/2;

		height = rc.bottom - rc.top;
		top = conf::wndTop + (conf::wndHeight - height)/2;
		
		MoveWindow(hDlg, left, top, width, height, TRUE);
		//Chld::Init();

		wchar_t st[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_4,st,MAX_PATH);
		SetWindowText(hDlg,st);
		LoadString(hInst,IDS_STRINGSW_115,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_BUTTON_SAVE_AS,st);
		LoadString(hInst,IDS_STRINGSW_116,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_BUTTON_LOAD_FROM ,st);
		LoadString(hInst,IDS_STRINGSW_41,st,MAX_PATH);
		SetDlgItemText(hDlg,IDOK,st);
		LoadString(hInst,IDS_STRINGSW_13,st,MAX_PATH);
		SetDlgItemText(hDlg,IDCANCEL,st);

		Chld::Create(hDlg);
		FillConfigDlgLeftTree(hDlg);

		//MyButtonFrRCBtn(hDlg,IDC_BUTTON_SAVE_AS,conf::Dlg.iBtnsType);
		//MyButtonFrRCBtn(hDlg,IDC_BUTTON_LOAD_FROM,conf::Dlg.iBtnsType);
		//MyButtonFrRCBtn(hDlg,IDOK,conf::Dlg.iBtnsType);
		//MyButtonFrRCBtn(hDlg,IDCANCEL,conf::Dlg.iBtnsType);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDCANCEL:
				int iTotPans;iTotPans = Dlg.iTotPanels;
				DlgRestore.CopyTo(&Dlg);
				Dlg.ApplyToSystem(iTotPans);
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			case IDOK:
				if(!Dlg.Cmp(&DlgRestore))
					conf::Write(MyStringAddModulePath(L"Conf.bin"));
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			case IDC_BUTTON_SAVE_AS:
				Dlg.SaveAs();
				break;
			case IDC_BUTTON_LOAD_FROM:
				if(DlgRestore.LoadFrom())
				{	if(!Dlg.Cmp(&DlgRestore))
					{	DlgRestore.CopyTo(&Dlg);
						Dlg.ApplyToSystem(Dlg.iTotPanels);
				}	}
				break;
		}
		break;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{	case TVN_SELCHANGED:
				wchar_t itemText[64];
				TVITEMW tvItem;ZeroMemory(&tvItem,sizeof(TVITEMA));
				tvItem.mask = TVIF_TEXT|TVIF_PARAM;
				tvItem.cchTextMax = ARRAYSIZE(itemText);
				tvItem.pszText = itemText;
				tvItem.hItem = TreeView_GetSelection(GetDlgItem(hDlg,IDC_TREE_CONFIGURATION));
				//if(!TreeView_GetItem(GetDlgItem(hDlg,IDC_TREE_CONFIGURATION), &tvItem))
				if(!SendMessage(GetDlgItem(hDlg,IDC_TREE_CONFIGURATION),TVM_GETITEM,0,(LPARAM)&tvItem))
					break;
				Chld::Change((int)tvItem.lParam);
				SetFocus(GetDlgItem(hDlg,IDC_TREE_CONFIGURATION));
				break;
		}		
		break;
	case WM_MOVE:
		int x;x = (int)(short) LOWORD(lParam);
		int y;y = (int)(short) HIWORD(lParam);
		for(int i=0; i<REAL_TOT_CHILD_DLGS/*TOT_CHILD_DLGS*/; i++)
			MoveWindow(	stdhDlg[i]/*chlds[i].hWnd*/,x+Chld::pos[0]-5,
						y+Chld::pos[1]-4,
						Chld::sz[0],Chld::sz[1],TRUE);
		break;
	case WM_DESTROY:
		Chld::Destroy();
		//int f = 0;if((viewMenuBitsU32[0] & 0x1000) >0)f=1;
		//int ff = 0;if(1==panel[0].bHideHiddenFiles)ff=1;
		//if(f!=ff)
		//	MessageBox(hDlg,L"For apply changes you should restart <Sino>.",L"About show/hide files...",MB_OK);
		return 0;
	}
	return (INT_PTR)FALSE;
}


//

}//end of namespace conf