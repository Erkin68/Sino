#pragma once

#include "windows.h"
#include "commctrl.h"
#include "resource.h"
#include "toolcha.h"
#include "EntryStep.h"
#include "FillManager.h"
#include "FileAssociation.h"
#include "shobjidl.h"
#include "MyCommonControls.h"
#include "Operations\CopyOperation.h"
#include "Operations\KernelInUserSpace.h"
#include "Plugins\Auxilary\SearchViaF7.h"
#include "Plugins\Auxilary\SinoSearchF7\SchItem.h"
#include "Plugins\Archive\Archive.h"
#include "Plugins\Image\Image.h"
//#include <gdiplus.h>
//using namespace Gdiplus;


#define IDPANELTIMER_SCRLMOUSE		 1111
#define IDPANELTIMER_CHANGEBCKIMG	 1112
#define IDPANELTIMER_VERTSCRLBTNUP	 1113
#define IDPANELTIMER_VERTSCRLBTNDOWN 1114
#define IDPANELTIMER_HORSCRLBTNLEFT	 1115
#define IDPANELTIMER_HORSCRLBTNRIGHT 1116

#define MAX_DLG								9
#define MAX_PANELS							4
#define MAX_GUID_CHAR						527//sizeof(PanelItem) 32 taga karrali bo'lishi kerak;					
#define GUID_CHAR_POS						232//
#define GUID_CHAR_LEN						294
#define TOTAL_ICONS							9
#define HEAP_INC_ITEM_NUMBER				512
#define MIN_DIST_FROM_NAME_AND_EXTENSION	25
#define MIN_DIST_FROM_COLUMNS               2
#define MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS 27

#define XBITMAP (2+conf::Dlg.iIconSize[iThis])
#define XBITMAP_prnt (2+conf::Dlg.iIconSize[prnt->iThis])

typedef char     		 C8;
typedef char     		 S8;
typedef unsigned char    U8;

typedef short    		 S16;
typedef unsigned short   U16;

typedef int      		 S32;
typedef unsigned int     U32;

typedef __int64			 S64;
typedef unsigned __int64 U64;

typedef float            F32;
typedef double           F64;

typedef enum TFolderAndFileMixingSortType
{	foldersUp = 0,
	filesUp = 1,
	mixing = 2
} FolderAndFileMixingSortType;
typedef enum TSortType
{	alphabeticalUp = 0,
	alphabeticalDwn = 1,
	forEnums = 2,
	forCreateTimeUp = 3,
	forCreateTimeDwn = 4,
	forSizeUp = 5,
	forSizeDwn = 6,
	forTypeUp = 7,
	forTypeDwn = 8
} SortType;

enum
{	normal=0,
	selected=1
};
enum
{	header,//".."
	folder,
	file,
	guidElement,
	shlLnkElement//*.lnk files;
//	shellLink,
//	zipArchive
};
typedef struct TFileMenuPopUp
{	HMENU hMenu;
	HMENU hSubmenu;
	int idFile;
	int posX,posY;
}FileMenuPopUp;

class Panel;
namespace linkSock
{
extern BOOL ReceiveFrClientMsg(int,WPARAM,LPARAM);
}extern wchar_t* MakeGuidPath(wchar_t*,wchar_t*);

#pragma pack(1)
class PanelItem//sizeof->640 ta(32*20)
{
public:
		  PanelItem();
		 ~PanelItem();
	//Eng birinchi parametrga nimani qo'yamiz, nomi bo'lmasa,nima bo'ladi????
	wchar_t Name[MAX_GUID_CHAR];//ext ni ham ichida, 2 lasining orasida NULL char ham bor;
	//oxirida qo'yib packing ni maxga qo'yamiz, oxiridan olaveradi.Lekin birinchi qo'yib, packingni 1 qilishga
	//qaror qildik.Bu degani har safar razmerni o'zgartirsak,packing bilan o'zimiz shig'ullanishimiz kerak,
	//degani,MAX_GUID_CHAR,GUID_CHAR_POS va GUID_CHAR_LEN larni o'zimiz sozlaymiz degani;
	U64   size;//in bytes;
	U64   allSize;//in bytes;
	FILETIME ftWrite;
	FILETIME ftCreate;//20.01.14 qo'shduk;
	FILETIME ftLastAccess;//20.01.14 qo'shduk;
	DWORD    dwAttribute;//20.01.14 qo'shduk;
	S32   assocNum;//In myComp as type of media drive:
	S32   itemId;  //Qo'shimcha parametrlar uchun;  
	//U8    padd;    //Qo'shimcha parametrlar uchun;  
	HICON hMasterIcon;//if own icon or cursor file;
	U16   NameCharLen;//U8 edi, U16 qilamiz, paddingni yeydi;
	U8    ExtCharLen;
	U8    attribute;//see enums above;
	U8    state;//see enums above;
	U8    iCstmItem;//see enums above;
protected:
	S32   nameWidth;
	S32   extWidth;
	RECT  rc;
public:
	//oxirida shular qolsun;
	S32   nColumn;
	S32   nRow;				
	VOID  CheckForCstmItems(Panel*,WIN32_FIND_DATA*);
	VOID  CheckForCstmItems(Panel*,NSKERNEL::PFILE_BOTH_DIR_INFORMATION);
	BOOL  CheckForOtherCondtns(SearchItem*,Panel*,WIN32_FIND_DATA*);
	BOOL  CheckForOtherCondtns(SearchItem*,Panel*,NSKERNEL::PFILE_BOTH_DIR_INFORMATION);
	VOID  FreeMem();

	S32	  GetNameWidth();
	S32	  GetExtWidth();
	VOID  SetNameWidth(S32);
	VOID  SetExtWidth(S32);

	VOID  Render(HDC,Panel*,BOOL=FALSE);//int
	VOID  RenderFullCnfg(HDC,Panel*,BOOL=FALSE);//int
	VOID  RenderCustomCnfg(HDC,Panel*,BOOL=FALSE);//int	
	VOID  RenderTransparent(HDC,Panel*,BOOL=FALSE);//int
	VOID  RenderTransparentFullCnfg(HDC,Panel*,BOOL=FALSE);//int
	VOID  RenderTransparentCustomCnfg(HDC,Panel*,BOOL=FALSE);//int	
	VOID  RenderCustomItem(HDC,Panel*,BOOL=FALSE);//int
	VOID  RenderCustomItemTransparent(HDC,Panel*,BOOL=FALSE);//int
	VOID  RenderFullIcon(HDC,Panel*);
	VOID  RenderFullIconTransparent(HDC,Panel*);
	VOID  SetName(Panel*,wchar_t*,BOOL);
	VOID  SetGuidName(Panel*,wchar_t*,wchar_t*,BOOL);
	wchar_t* GetExt(){return &Name[NameCharLen+1];}
	//char* GetNameA();
	VOID  GetRect(RECT*,Panel*,BOOL);
	RECT* GetRectPtr(Panel*,BOOL);
	BOOL  DrawOtsech(Panel*);
};
#pragma pack(8)//default

//class Search;
class SheetTab;

class Panel
{
public:
static HICON	hIcons[TOTAL_ICONS];
static HDC	    hIconsDC[TOTAL_ICONS];
static int		iActivePanel;
static int		GetPanelFromPoint(POINT*);
static int		IsPointInsidePanelRect(POINT*);
static int		scrlBarHeight;
static ATOM		rcl;

friend BOOL CmnCntrl::CreatePanelSelFilesInfoEdits(HWND,int*);
friend BOOL CmnCntrl::CreatePatnAndNameEdit(HWND,int,int*);
friend BOOL CmnCntrl::ResizePanelSelFilesInfoEdits(int*);
friend BOOL CmnCntrl::ResizePathAndNamesEdit(int, int*);
friend BOOL CmnCntrl::DestroyControls(HWND);
friend int  CmnCntrl::IsPointInsidePatnAndNameEditRect(POINT*);
friend int  CmnCntrl::IsPointInsideCmndCBEdit(POINT*);
friend BOOL CmnCntrl::CmndsCBMsgQueue(MSG*);
friend BOOL linkSock::ReceiveFrClientMsg(int,WPARAM,LPARAM);

friend LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

friend class PanelItem;
friend class SheetTab;
friend class FillManager;

			Panel();
		   ~Panel();
	VOID	SetOpponent();


	BOOL	AddGuidItem(LPWSTR, LPWSTR, HICON);
	//BOOL	AddItemA(LPSTR, S32, HICON ic=NULL,WIN32_FIND_DATAA* ff=NULL);
	BOOL	AddItem(LPWSTR, S32, HICON ic=NULL,WIN32_FIND_DATAW* ff=NULL,DWORD itemId=0,BOOL bRedraw=FALSE);
	BOOL	AddItemFrRandomList(LPWSTR, int);
	BOOL	AddItemFrRandomListArj(LPWSTR, int, int);
	BOOL	AddItemFrSock(VOID*);
	BOOL	AddItemFrKrnl(LPWSTR lpwstr, int, NSKERNEL::PFILE_BOTH_DIR_INFORMATION);
	VOID	AddToSelection(int);
	VOID	DeleteFromSelection(int);
	VOID	AdjustScrollity();
	VOID	CalcFoldersSizes();
	BOOL	CancelQuickSearchMode(BOOL);
	VOID	ChangeActivePanel();
	VOID	ChangeColor(COLORREF*,int,int init=0);
	VOID	ChangeFont(LOGFONT*,int,int init=0);
	//BOOL	ChangePathA(char*);
	BOOL	ChangePath(wchar_t*);
	BOOL	SetVirtual(wchar_t*,LPVOID,int);
	BOOL	FolderUpVirtualPanel(int,BOOL);
	//BOOL	FolderEnumInVirtualPanel(wchar_t*);
	BOOL	ChangeToGuidPath(wchar_t*);
	BOOL	ChangeSheetTabPath();
	BOOL	ChangeTooltipText(wchar_t*,int,int);
	VOID	ClrScr();
	BOOL	Create(HINSTANCE,HWND,int,int,int,int,int,wchar_t*);
	BOOL	DeleteCrntSheetTab();
	BOOL	DeleteSheetTab(int);
	VOID	DropFiles(HDROP);//Drop from sino self
	BOOL	IsDstWindowDraggable(HWND);
	BOOL	DragFiles(HWND);
	VOID	FreeSelection();
	VOID	Destroy();
	VOID	EnterDragFiles(int);
	VOID	EnableInput(BOOL);
	BOOL	ExecuteHotKey(int);
	VOID	FreeMem();
	//int	GetFullPathAndNameA(int,char*,int);
	int		GetFullPathAndName(int,wchar_t*,int);
	wchar_t *GetFullPathAndName(int);
	wchar_t *GetArcFilePathAndName();
	wchar_t *GetArcPath();
	wchar_t *GetArcItPathAndName(int);
	wchar_t *GetArcItPathAndName(wchar_t*);
	wchar_t *GetArcItFullPathAndName(int);
	wchar_t *GetArcItFullPathAndName(wchar_t*);

	BOOL	ClpbrdCpy();
	BOOL	ClpbrdCut();
	BOOL	ClpbrdPst();
	BOOL	ClpbrdApndCpy();
	BOOL	ClpbrdApndCut();

	int			GetItemNum(int, int);
	PanelItem*	GetItem(int s);
	PanelItem*	GetItems();
	const int   GetTotItems();
	void		SetTotItems(int s);
	const int	GetIdForTooltip();
	void		SetIdForTooltip(int);
	const int	GetNColumns();
	void		SetNColumns(int);
	const int   GetNRows();
	void		SetNRows(int);
	const int   GetItemMaxWidth();
	void		SetItemMaxWidth(int);
	const int   GetLeft();
	void		SetLeft(int);
	const int	GetTop();
	void		SetTop(int);
	const int   GetHeight();
	const int	GetWidth();
	const int	GetScrollOffst();
	void		SetWidth(int);
	const int	GetClientLeft();
	const int	GetClientRight();
	void		SetClientRight(int);
	const int   GetClientBottom();
	void		SetClientBottom(int);
	const int	GetItemHeight();
	VOID		ScrollItemToView(int);
	void		SetItemHeight(int);
	const int   GetTotSelects();
	const int   GetTotSelectsFiles();
	const int   GetTotSelectsFolders();
	//char*		GetPathA();
	wchar_t*	GetPath();
	int			GetPath(wchar_t*);
	void 		SetPathLn(int);
	int			GetHot();
	VOID		SetHot(int,BOOL bRender=TRUE);
	const HWND	GetHWND();
	const HDC	GetDC();
	const BOOL	GetRenderFull();
	void 		ReleaseDC(HDC);
	VOID		RecalcItemsRects();
	LRESULT		PostMessage(UINT,WPARAM,LPARAM);
	LRESULT		SendMessage(UINT,WPARAM,LPARAM);
	void		OnSetFocus();
	void		SetFocus(){::SetFocus(hWnd);}
	const HWND	GetSelFilesInfoEdt();
	int			GetSelectedItemNum(int);
	const HWND	GetPathAndNameEdt(int);
	LPVOID		GetVPPlgObj();
	int			GetiVPPlg();

	BOOL		AnalyzeDirChangeNotify();
	VOID		ChangeViewMenu(int);
	BOOL		CreateChangeNotify(wchar_t*);
	VOID		DestroyChangeNotify();
	//BOOL		SetChangeNotifyPathA(char*);
	BOOL		SetChangeNotifyPath(wchar_t*);

	const int	GetPathLn();
	//void		SetPathA(char*,int);
	void		SetPath(wchar_t*,int,BOOL bCheckExisting=TRUE);
	wchar_t*	GetStrForTooltip(int,int* ln=0);
	MyToolTip*	GetToolTip();
	BOOL		FillArchItems(wchar_t*);
	BOOL		FillDirectFolder();
	BOOL		FillGuidFolder();
	BOOL		FillRandomPathList(LPVOID);
	BOOL		FillRandomPathListFrArj(LPVOID);
	BOOL		FillRandomPathListIP4(LPVOID);
	//int		FindItemA(char*,int cmpMethod=0);
	int			FindItem(wchar_t*,int cmpMethod=0);
	int			FindNearestItem(wchar_t*,int);
	BOOL		FolderIn(int);
	BOOL		FolderInToArch(int,int);
	BOOL		FolderUp(int);
	BOOL		OtherControlMsgQueue(MSG*);
	CArch*		GetArch();


	//VOID	ShowPathAndNameA(int,char*);
	VOID	ShowPathAndName(int,wchar_t* excptTxt=NULL);
	EntryStep* GetEntry(){return &entry;}

	VOID	Move(int, int, int, int);
	BOOL	ReadToPathAndNameCombo(int);
	VOID	Render(PAINTSTRUCT* ps=NULL);
	VOID	RenderItem(int);
	VOID	RenderFullIcons();
	BOOL	SetIcons(HINSTANCE);
	BOOL	SetAllItemsListToClipboard(BOOL);
	BOOL	SetSelectedItemsListToClipboard(BOOL);
	BOOL	SetSortBtnState(int,int);
	VOID	ShowSelectedFilesInfo();
	VOID	SintezKey(int);
	VOID	Sort();
	VOID	SortWithCustomItems();
	LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
	BOOL	WriteToDiskFrPathAndNameCombo(int);
	int		GetItemsMaxWidth();

	typedef enum TViewType
	{	shortest = 1,	//filename
		full = 5,		//filename,size,create date,create time,attribute
		custom
	}ViewType;
	ViewType viewType;
	struct
	{	wchar_t pathFor[MAX_PATH];//Bekor turganida papkalarning razmerini hisoblab turishi uchun;
		int  crntItemFor;
	} FoldersSizes;

	int     iTabSheet;//TAB crnt sheeti;
	FolderAndFileMixingSortType folderAndFileMixingSortType;
	SortType		folderSortType,fileSortType;
	wchar_t			findItemName[MAX_PATH];

	//BOOL bDrgDrpLDwn;
	//BOOL bDidDrgDrp;

	//BackgroundImage:
	typedef struct TIimgBckgrnd
	{	HDC dc;
		HBITMAP bm;
		int width;
		int height;
		int bpp;
		UINT_PTR hTimer;
		VOID Free();
		BOOL Load(Panel*,wchar_t* imgPth=NULL);
		BOOL CloneOnWM_SIZE(Panel*,TIimgBckgrnd*);
		VOID OnTimer(Panel*);
	} ImgBckgrnd;
	ImgBckgrnd imgBckgrnd[2];

	int	iDrgDwn;
	int iThis,iOpponent,iAntiOpponent;
	Panel* GetOpponent();

	union
	{U32 viewMenuBitsU32;
	 struct
	 {	U32 bItemsViewType        : 2;//00-short,01-full,10-custom,...etc.
		U32 bItemsCustomName	  : 1;//View->Custom b-sa, Name i
		U32 bItemsCustomExt		  : 1;//View->Custom b-sa, Ext i
		U32 bItemsCustomCrTime	  : 1;//View->Custom b-sa, ...
		U32 bItemsCustomWrTime	  : 1;
		U32 bItemsCustomSz		  : 1;
		U32 bItemsCustomTotSz	  : 1;
		U32 bItemsCustomAtrb	  : 1;
		U32 bItemsCustomState	  : 1;
		U32 bItemsCustomNameLn	  : 1;
		U32 bItemsCustomLstAccTime: 1;
		U32 bHideHiddenFiles	  : 1;// & 1 0000 0000 0000b 0x1000
	 };
	};

	struct//Boolsi:
	{	//Panelning o'ziniki:
		U32 bDrag			:1;
		U32 bMouseCapture	:1;
		U32 bInput			:1;
		U32 bRenderFull		:1;
		U32 bQView			:1;
	};

protected:
	HFONT		hf,hfEdit,hfSrtBtn;
	COLORREF	color[6];
	HBRUSH		bkBrsh,txtBkBrsh,hotBkBrsh,hBrshSrtBtn,brEdit;

	PanelItem*  pItems;
	__int32		ScrollOffst;
	int			iMaxItems;
	int         totItems,totSelects,totSelectsFiles,totSelectsFolders;
	U64			selectedItemsSizes;
	int			idForTooltip;
	int			iHot,iOldHot;

	int		top;
	int		left;
	int		width;
	int		height;
	int		pathLn;
	wchar_t	path[MAX_PATH],oldPathW[MAX_PATH];
	int		nColumns;
	int		nRows;
	int		itemMaxWidth;//,itemMaxWidthSave;
	int		itemHeight;
	HWND	hWnd;
	HWND	selFilesInfoEdt;
	HWND	pathAndNameEdt[2];//[0]->Combo, [1]-edit of the combo
	RECT	rcClient;
	//int   hscrollHeight; hscroll bo'lsa-bo'lmasa orqasini 1 xil chizish uchun;

	INT	 charsExts[0xffff];
	int  charsExtra;
	TEXTMETRIC tm;
	VOID My3StrCpyBoundWidth(wchar_t*,wchar_t*,int,int,int,int,int);
	int  MyGetTextExtentPoint32(wchar_t*, int);
	int  MyGetMaxWidthTextExtentPoint32(wchar_t*,int,int,int*);
	VOID ResetGetTextExtentPoint32();

	SCROLLINFO	scrlInfo;
	IShellLink* shlLnk; 
	IPersistFile* persFileShlLnk;

	//DWORD timeForDblClicking;

	MyToolTip		toolTip;
	FillManager		fManager;
	FileAssociation fAssociation;
	EntryStep		entry;
	FileMenuPopUp	fileMenuPopup;
	HWND			hEditItem,hQuickSearchEdit,hDiskCB;
	PanelItem*      iEditItem;
	S32				EditItemId;

	//Archive object:
	CArch   arch;
	LPVOID  VPPlgObj;
	int     iVPPlg;

	BOOL	AllocGuidFolder(int);
	BOOL	AllocVirtualGuidFolder(int);
	BOOL	AddNewSheetTab();
	BOOL	AddShellLinkItem(LPWSTR, LPWSTR, HICON);
	BOOL	AddStartupSheetTabs();
	VOID	AdjustHorScrollity();
	VOID	AdjustVerScrollity();
	BOOL	CancelEditItemMode(BOOL);
	BOOL	CancelDiskCBMode();
	BOOL	ComboPathAndNameMsgQueue(WPARAM,LPARAM);
	BOOL	CreateHeap();
	VOID	CreateObjects();
	BOOL	DecHeap(int);
	VOID	EditItemMsgQueue(MSG*);
	VOID	EditPathAndNameMsgQueue(MSG*);
	VOID	EditQuickSearchMsgQueue(MSG*);
	BOOL	EnterEditItemMode();
	BOOL	EnterDiskCBSelectMode();
	BOOL	EnterQuickSearchMode(wchar_t);
	BOOL	IncHeap();
	VOID	OnDirectoryChangeNotify();
	BOOL	OnKeybPressed(WPARAM, LPARAM);
	VOID	OnMouseClick(int);
	VOID	OnMouseCtrlClick(int);
	VOID	OnMouseRClick(int,int,int);
	VOID	OnMouseShftClick(int);
	VOID	OnSheetTabChanged();
	VOID	OnSheetTabChanging();
	VOID	RedSelectAllWithout(int);
	VOID	Select(int,int);
};
extern Panel panel[MAX_PANELS];


inline PanelItem* Panel::GetItem(int s){return &pItems[s];}
inline PanelItem* Panel::GetItems(){return pItems;}
inline const int Panel::GetTotItems(){return totItems;}
inline void		Panel::SetTotItems(int s){totItems=s;}
inline const int Panel::GetIdForTooltip(){return idForTooltip;}
inline void		Panel::SetIdForTooltip(int s){idForTooltip=s;}
inline const int Panel::GetNColumns(){return nColumns;}
inline void		Panel::SetNColumns(int s){nColumns=s;}
inline const int Panel::GetNRows(){return nRows;}
inline void		Panel::SetNRows(int s){nRows=s;}
inline const int Panel::GetItemMaxWidth(){return itemMaxWidth;}
inline void		Panel::SetItemMaxWidth(int s){itemMaxWidth=s;}
inline const int Panel::GetLeft(){return left;}
inline void		Panel::SetLeft(int s){left=s;}
inline const int Panel::GetTop(){return top;}
inline void		Panel::SetTop(int s){top=s;}
inline const int Panel::GetHeight(){return height;}
inline const int Panel::GetWidth(){return width;}
inline const int Panel::GetScrollOffst(){return ScrollOffst;}
inline void		Panel::SetWidth(int s){width=s;}
inline const int Panel::GetClientLeft(){return rcClient.left;}
inline const int Panel::GetClientRight(){return rcClient.right;}
inline void		Panel::SetClientRight(int s){rcClient.right=s;}
inline const int Panel::GetClientBottom(){return rcClient.bottom;}
inline void		Panel::SetClientBottom(int s){rcClient.bottom=s;}
inline const int Panel::GetItemHeight(){return itemHeight;}
inline void		Panel::SetItemHeight(int s){itemHeight=s;}
inline const int Panel::GetTotSelects(){return totSelects;}
inline const int Panel::GetTotSelectsFiles(){return totSelectsFiles;}
inline const int Panel::GetTotSelectsFolders(){return totSelectsFolders;}
inline wchar_t*	Panel::GetPath(){return path;}
inline int		Panel::GetPath(wchar_t* p){return MyStringCpy(p,MAX_PATH-1,path);}
inline void 	Panel::SetPathLn(int l){pathLn=l;}
inline int		Panel::GetHot(){return iHot;}
inline const HWND Panel::GetHWND(){return hWnd;}
inline const HDC  Panel::GetDC(){return ::GetDC(hWnd);}
inline const BOOL Panel::GetRenderFull(){return (bRenderFull==1?TRUE:FALSE);}
inline void 	Panel::ReleaseDC(HDC dc){::ReleaseDC(hWnd,dc);}
inline LRESULT	Panel::PostMessage(UINT message,WPARAM wParam=0,LPARAM lParam = 0)
			{ return ::PostMessage(hWnd,message,wParam,lParam);}
inline LRESULT	Panel::SendMessage(UINT message,WPARAM wParam=0,LPARAM lParam = 0)
			{ return ::SendMessage(hWnd,message,wParam,lParam);}
inline const HWND Panel::GetSelFilesInfoEdt(){return selFilesInfoEdt;}
inline const HWND Panel::GetPathAndNameEdt(int i){return pathAndNameEdt[i];}
inline BOOL		Panel::AnalyzeDirChangeNotify(){return fManager.AnalyzeDirChangeNotify();}
inline BOOL		Panel::CreateChangeNotify(wchar_t* p){fManager.CreateChangeNotify(p);}
inline VOID		Panel::DestroyChangeNotify(){fManager.DestroyChangeNotify();}
inline BOOL		Panel::SetChangeNotifyPath(wchar_t* p){return fManager.SetChangeNotifyPath(p);}
inline const int Panel::GetPathLn(){return pathLn;}
inline MyToolTip* Panel::GetToolTip(){return &toolTip;}
inline BOOL		Panel::FillArchItems(wchar_t *path){return fManager.FillArchItems(path);}
inline BOOL		Panel::FillDirectFolder(){return fManager.FillDirectFolder();}
inline BOOL		Panel::FillGuidFolder(){return fManager.FillGuidFolder();}
inline BOOL		Panel::FillRandomPathList(LPVOID p){return fManager.FillRandomPathList(p);}
inline BOOL		Panel::FillRandomPathListFrArj(LPVOID p){return fManager.FillRandomPathListFrArj(p);}
inline BOOL		Panel::FillRandomPathListIP4(LPVOID p){return fManager.FillRandomPathListIP4(p);}
inline BOOL		Panel::FolderIn(int i){return fManager.FolderIn(i);}
inline BOOL		Panel::FolderInToArch(int i,int id){return fManager.FolderInToArch(i,id);}
inline BOOL		Panel::FolderUp(int i){return fManager.FolderUp(i);}
inline CArch*	Panel::GetArch(){return &arch;}
inline wchar_t*	Panel::GetArcFilePathAndName(){return arch.name;}
inline wchar_t*	Panel::GetArcPath(){return entry.GetCrntRecArchPath();}
inline BOOL		Panel::FolderUpVirtualPanel(int id,BOOL bCloseVirtual){return fManager.FolderUpVirtualPanel(id,bCloseVirtual);}
//inline BOOL		Panel::FolderEnumInVirtualPanel(wchar_t *path){return fManager.FolderEnumInVirtualPanel(path);}
inline LPVOID Panel::GetVPPlgObj(){return VPPlgObj;}
inline int Panel::GetiVPPlg(){return iVPPlg;}
inline VOID Panel::AdjustScrollity(){if(0==bItemsViewType)AdjustHorScrollity();else AdjustVerScrollity();}
inline Panel* Panel::GetOpponent(){return &panel[iOpponent];}


inline S32 PanelItem::GetNameWidth(){return nameWidth;}
inline VOID PanelItem::SetNameWidth(S32 newWidth){nameWidth = newWidth;}
inline S32 PanelItem::GetExtWidth(){return extWidth;}
inline VOID PanelItem::SetExtWidth(S32 newExtWidth){extWidth = newExtWidth;}