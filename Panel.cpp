#include <malloc.h>
#include "Sino.h"
#include "config.h"
#include "strsafe.h"
#include "SheetTab.h"
#include "MyCommonControls.h"
#include "DiskToolBar.h"
#include "Operations\BtnBar.h"
#include "Operations\RenMove.h"
#include "Operations\MyErrors.h"
#include "Operations\MyStreams.h"
#include "Operations\Execution.h"
#include "Operations\MenuUtils.h"
#include "Operations\VirtualPanel.h"
#include "Operations\LinkSocket.h"
#include "Operations\FolderSize.h"
#include "Operations\CreateFolder.h"
#include "Operations\ButtonsPanel.h"
#include "Operations\FilesToCBEdit.h"
#include "Operations\DragInterface.h"
#include "Operations\DeleteOperation.h"
#include "Operations\qView.h"
#include "Operations\MyShell\ComboToDisk.h"
#include "Operations\MyShell\FindStrWthFltr.h"
#include "Operations\MyShell\MyShell.h"
#include "WindowsManagmentInstrumentation.h"
#include "Plugins\Executables\ViewFileAssoc.h"
#include "Plugins\Auxilary\SelectViaPlus.h"
#include "Plugins\Archive\Archive.h"
#include "Profiler.h"
#include <gdiplus.h>
#include "License.h"
//#include "Richedit.h"//EM_SETBKGNDCOLOR

using namespace Gdiplus;

extern VOID OnToolbarDiskBtnClicked(int,int);
extern HRESULT CreateDropSource(IDropSource**,int);
extern HRESULT CreateDataObject(FORMATETC*,STGMEDIUM*,UINT,IDataObject**);

static DWORD scrlLineSpeedTick=0;static char scrlBtnPressedDir=0;

Panel	panel[MAX_PANELS];

//HBRUSH  Panel::blueBrush = NULL;
//HBRUSH  Panel::iconBckBrush = NULL;
ATOM	Panel::rcl=0;
HICON	Panel::hIcons[TOTAL_ICONS] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
int		Panel::iActivePanel = -1;
int		Panel::scrlBarHeight = 0;
//int	Panel::eraseForFirtsEntr = -1;

typedef struct _MY_WIN32_FIND_DATA
{ DWORD    dwFileAttributes; 
  FILETIME ftCreationTime; 
  FILETIME ftLastAccessTime; 
  FILETIME ftLastWriteTime; 
  DWORD    nFileSizeHigh; 
  DWORD    nFileSizeLow; 
  DWORD    dwReserved0; 
  DWORD    dwReserved1; 
} MY_WIN32_FIND_DATA;

wchar_t* MakeGuidPath(wchar_t* guid,wchar_t* nick)
{
static wchar_t s[MAX_GUID_CHAR];
MyStringCpy(s,MAX_GUID_CHAR,guid);
MyStringCpy(&s[GUID_CHAR_POS],GUID_CHAR_LEN,nick);
return s;
}

Panel::Panel() : 
	hWnd(NULL),
	viewType(shortest),
	left(0),top(0),width(100),height(100),
	iHot(-1),
	hf(NULL),
	bkBrsh(0),txtBkBrsh(0),hotBkBrsh(0),hBrshSrtBtn(0),
	hfEdit(0),brEdit(0),hfSrtBtn(0),
	pItems(NULL),
	iMaxItems(HEAP_INC_ITEM_NUMBER),
	itemMaxWidth(0),itemHeight(20),
	idForTooltip(0),
	folderAndFileMixingSortType(foldersUp),folderSortType(alphabeticalUp),
	fileSortType(forTypeUp),
	totSelects(0),totSelectsFiles(0),totSelectsFolders(0),
	//timeForDblClicking(0),
	bRenderFull(0),bInput(1),
	hEditItem(NULL),hQuickSearchEdit(NULL),hDiskCB(NULL),iEditItem(NULL),EditItemId(0),
	iTabSheet(0),selectedItemsSizes(0),
	iDrgDwn(-1),bDrag(0),bMouseCapture(0),
	bItemsViewType(0),	  //00-short,01-full,10-custom,...etc.
	bItemsCustomName(0),  //View->Custom b-sa, Name i
	bItemsCustomExt(0),	  //View->Custom b-sa, Ext i
	bItemsCustomCrTime(0),//View->Custom b-sa, ...
	bItemsCustomWrTime(0),
	bItemsCustomSz(0),
	bItemsCustomTotSz(0),
	bItemsCustomAtrb(0),
	bItemsCustomState(0),
	bItemsCustomNameLn(0),
	bItemsCustomLstAccTime(0),
	bHideHiddenFiles(0)
	//bDrgDrpLDwn(0),bDidDrgDrp(0)
	//hscrollHeight(0)
	//bHorScrl(FALSE),
{
	pathAndNameEdt[0]=NULL;
	pathAndNameEdt[1]=NULL;
	findItemName[0]=0;
	fManager.SetParent(this);
	scrlInfo.cbSize = sizeof(SCROLLINFO);
	rcClient.left = rcClient.top = 0;
	scrlInfo.nMin   = 0; 
	scrlInfo.nPage  = 1;
	color[0] = 0xffffffff;//BAck
	color[1] = 0xff000000;//Front
	color[2] = 0xffffffff;//Selected back
	color[3] = 0xffff0000;//Select
	color[4] = 0xff0000ff;//Hot
	color[5] = 0xff00ff00;//Hot back
	//profilerOutStr("Beg of Panel::Panel ")
	FoldersSizes.pathFor[0]=0;
	FoldersSizes.crntItemFor=0;
	iThis = (int)(this-&panel[0]);
	arch.srcPanel = iThis;
	imgBckgrnd[0].dc=imgBckgrnd[1].dc=0;//faqat dc ga qarab olamiz, qolganlarini ham;
	imgBckgrnd[0].hTimer=imgBckgrnd[1].hTimer=0;
}

VOID Panel::SetOpponent()
{	iThis = (int)(this-&panel[0]);
	switch(conf::Dlg.iTotPanels)
	{	case 1:iOpponent = -1; iAntiOpponent = -1; break;
		case 2:iOpponent = (0==iThis?1:0); iAntiOpponent = iOpponent;break;
		case 3:iOpponent = (0==iThis?1:(1==iThis?2:0)); iAntiOpponent = (0==iThis?2:(1==iThis?0:1)); break;
		case 4:iOpponent = (0==iThis?1:(1==iThis?2:(2==iThis?3:0))); iAntiOpponent = (0==iThis?3:(1==iThis?0:(2==iThis?1:2))); break;
}	}

Panel::~Panel()
{
	hWnd = NULL;
}

/*BOOL Panel::AddItemA(LPSTR lpstr,int attrib, HICON ic, WIN32_FIND_DATAA *ff)
{//WIN32_FIND_DATAA b-n WIN32_FIND_DATAW nomini hisobga olmaganda 1 xil;
wchar_t ws[MAX_PATH];
	MultiByteToWideChar(AreFileApisANSI()?CP_ACP:CP_OEMCP,MB_PRECOMPOSED,lpstr,MAX_PATH-1,ws,MAX_PATH);
	return AddItem(ws,attrib,ic,(WIN32_FIND_DATAW*)ff);
}*/

BOOL Panel::AddItem(LPWSTR lpwstr,int attrib, HICON ic, WIN32_FIND_DATAW *ff,DWORD itemId,BOOL bRedraw)
{
	if(ff && bHideHiddenFiles)
		if(ff->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			return FALSE;

	if(!(totItems < iMaxItems-1))
		{ if(!IncHeap()) return FALSE; }

	pItems[totItems].SetName(this,lpwstr,file == attrib);
	pItems[totItems].attribute = attrib;
	pItems[totItems].hMasterIcon = ic;
	pItems[totItems].itemId = itemId;
	if(ff && header!=attrib)
	{	pItems[totItems].ftWrite = ff->ftLastWriteTime;
		pItems[totItems].ftLastAccess = ff->ftLastAccessTime;
		pItems[totItems].ftCreate = ff->ftCreationTime;
		pItems[totItems].dwAttribute = ff->dwFileAttributes;
	}
	
	if(ff && file == attrib)
	{	pItems[totItems].size = (((U64)ff->nFileSizeHigh<<32) | ff->nFileSizeLow);
		if(pItems[totItems].ExtCharLen>0)
		{	if(!ic || (0xffffffff== *((DWORD*)&ic) ))//0xffffffff ni qo'shdim, fast icon uchun;
			{	pItems[totItems].assocNum = fAssociation.Add(pItems[totItems].GetExt());
				pItems[totItems].hMasterIcon = 0;
		}	}
		else
			if(!ic)pItems[totItems].assocNum = -1;
	}
	else//hammasiga: else if(myCompElement==attrib)
	{	if(!ic) pItems[totItems].assocNum = -1;
		pItems[totItems].size = 0;
	}
	//ReleaseDC(hWnd, dc); MyGetTextExtentPoint32 ga dc kerakmas;

	pItems[totItems].state = normal;

	int tmp = pItems[totItems].GetNameWidth() + pItems[totItems].GetExtWidth() + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;
	if(itemMaxWidth < tmp)//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS)
		itemMaxWidth = tmp;//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;

	//pItems[totItems].nColumn = rcClient.bottom ? ((totItems * itemHeight) / rcClient.bottom) : 1;
	pItems[totItems].nRow = rcClient.bottom ? (itemHeight ? (totItems % (rcClient.bottom / itemHeight)) : 1) : 1;
	if(totItems)
	{	if(0==pItems[totItems].nRow)
			++nColumns;
	}
	pItems[totItems].nColumn = nColumns ? nColumns-1 : 0;

	//if(nColumns < pItems[totItems].nColumn)
	//	nColumns = pItems[totItems].nColumn;

	//if(nRows < pItems[totItems].nRow+1)
	//	nRows = pItems[totItems].nRow+1;
	if(totItems)//Header(0)ga kerakmas;
		pItems[totItems].CheckForCstmItems(this, ff);
	//if(itemId)  virtual panel uchun qo'ygandik,itemId borligi uchun endi kerakmas;
	//	pItems[totItems].assocNum = itemId;
	totItems++;

	if(bRedraw)
	{	Sort();
		AdjustScrollity();
		ScrollItemToView(GetHot());
		ClrScr();
		Render(NULL);
	}
	return TRUE;
}

BOOL Panel::AddItemFrRandomList(LPWSTR lpstr, int attrib)
{
WIN32_FIND_DATAW ff;
	if(file == attrib)
	{	if(!GetFileAttributesEx(lpstr,GetFileExInfoStandard,&ff))
		{	ff.nFileSizeHigh = 0;
			ff.nFileSizeLow = 0;
	}	}
	return AddItem(lpstr,attrib,0,&ff,0);
}

BOOL Panel::AddItemFrRandomListArj(LPWSTR lpstr, int attrib, int lpstrLn)
{
MY_WIN32_FIND_DATA *ff = (MY_WIN32_FIND_DATA*)(lpstr + lpstrLn+1);
	if(file != attrib)
	{	pItems[totItems].ftWrite.dwHighDateTime = 0;
		pItems[totItems].ftWrite.dwLowDateTime = 0;
		return AddItem(lpstr,attrib,0,(WIN32_FIND_DATAW*)ff,0);
	}
	return AddItem(lpstr,attrib,NULL,0,0);
}

BOOL Panel::AddItemFrKrnl(LPWSTR lpwstr, int attrib, 
						  NSKERNEL::PFILE_BOTH_DIR_INFORMATION DirInformation)
{
	if(bHideHiddenFiles)
		if(DirInformation->FileAttributes & FILE_ATTRIBUTE_HIDDEN)
			return FALSE;

	if(!(totItems < iMaxItems-1))
		{ if(!IncHeap()) return FALSE; }

	pItems[totItems].SetName(this,lpwstr,file == attrib);
	pItems[totItems].attribute = attrib;
//	pItems[totItems].hMasterIcon = 0; konstruktorda bor;
	pItems[totItems].itemId = 0;
	if(header!=attrib)//if(file == attrib)
	{	*((DWORD*)&pItems[totItems].allSize)= *((DWORD*)(&DirInformation->AllocationSize));//memcpy(&pItems[totItems].allSize,&DirInformation->AllocationSize,sizeof(LARGE_INTEGER));
		*(((DWORD*)&pItems[totItems].allSize)+1)= *((DWORD*)(&DirInformation->AllocationSize)+1);
		*((DWORD*)&pItems[totItems].size)= *((DWORD*)(&DirInformation->EndOfFile));//memcpy(&pItems[totItems].size,&DirInformation->EndOfFile,sizeof(LARGE_INTEGER));
		*(((DWORD*)&pItems[totItems].size)+1)= *((DWORD*)(&DirInformation->EndOfFile)+1);
		*((DWORD*)&pItems[totItems].ftWrite)= *((DWORD*)(&DirInformation->LastWriteTime));//memcpy(&pItems[totItems].ftWrite,&DirInformation->LastWriteTime,sizeof(LARGE_INTEGER));
		*(((DWORD*)&pItems[totItems].ftWrite)+1)= *((DWORD*)(&DirInformation->LastWriteTime)+1);
		*((DWORD*)&pItems[totItems].ftLastAccess)= *((DWORD*)(&DirInformation->LastAccessTime ));//memcpy(&pItems[totItems].ftWrite,&DirInformation->LastWriteTime,sizeof(LARGE_INTEGER));
		*(((DWORD*)&pItems[totItems].ftLastAccess)+1)= *((DWORD*)(&DirInformation->LastAccessTime)+1);
		*((DWORD*)&pItems[totItems].ftCreate)= *((DWORD*)(&DirInformation->CreationTime));//memcpy(&pItems[totItems].ftWrite,&DirInformation->LastWriteTime,sizeof(LARGE_INTEGER));
		*(((DWORD*)&pItems[totItems].ftCreate)+1)= *((DWORD*)(&DirInformation->CreationTime)+1);
		*((DWORD*)&pItems[totItems].dwAttribute)= *((DWORD*)(&DirInformation->FileAttributes));
	}

	if(file == attrib)
	{	//pItems[totItems].size = (((U64)ff->nFileSizeHigh<<32) | ff->nFileSizeLow);
		if(pItems[totItems].ExtCharLen>0)
		{	//if(!ic || (0xffffffff== *((DWORD*)&ic) ))//0xffffffff ni qo'shdim, fast icon uchun;
			{	pItems[totItems].assocNum = fAssociation.Add(pItems[totItems].GetExt());
				pItems[totItems].hMasterIcon = 0;
		}	}
		else
			/*if(!ic)*/pItems[totItems].assocNum = -1;
	}
	else//hammasiga: else if(myCompElement==attrib)
	{	/*if(!ic)*/ pItems[totItems].assocNum = -1;
		//pItems[totItems].size = 0;
	}
	//ReleaseDC(hWnd, dc); MyGetTextExtentPoint32 ga dc kerakmas;

	pItems[totItems].state = normal;

	int tmp = pItems[totItems].GetNameWidth() + pItems[totItems].GetExtWidth() + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;
	if(itemMaxWidth < tmp)//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS)
		itemMaxWidth = tmp;//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;

	//pItems[totItems].nColumn = rcClient.bottom ? ((totItems * itemHeight) / rcClient.bottom) : 1;
	pItems[totItems].nRow = rcClient.bottom ? (itemHeight ? (totItems % (rcClient.bottom / itemHeight)) : 1) : 1;
	if(totItems)
	{	if(0==pItems[totItems].nRow)
			++nColumns;
	}
	pItems[totItems].nColumn = nColumns ? nColumns-1 : 0;

	//if(nColumns < pItems[totItems].nColumn)
	//	nColumns = pItems[totItems].nColumn;

	//if(nRows < pItems[totItems].nRow+1)
	//	nRows = pItems[totItems].nRow+1;
	if(totItems)//Header(0)ga kerakmas;
		pItems[totItems].CheckForCstmItems(this, DirInformation);
	//if(itemId)  virtual panel uchun qo'ygandik,itemId borligi uchun endi kerakmas;
	//	pItems[totItems].assocNum = itemId;
	totItems++;

/*	if(bRedraw)
	{	Sort();
		AdjustScrollity();
		ScrollItemToView(GetHot());
		ClrScr();
		Render(NULL);
	}*/
	return TRUE;
}

BOOL Panel::AddItemFrSock(VOID* pd)
{
MY_WIN32_FIND_DATA fff;
SOCKWIN32_FIND_DATA* ff = (SOCKWIN32_FIND_DATA*)pd;
char *pext = NULL;
	fff.nFileSizeHigh = ff->nFileSizeHigh;
	fff.nFileSizeLow = ff->nFileSizeLow;
	fff.ftLastWriteTime = ff->ftLastWriteTime;
	fff.ftLastAccessTime = ff->ftLastAccessTime;
	fff.ftCreationTime = ff->ftCreationTime;
	U8 attrib = (FILE_ATTRIBUTE_DIRECTORY & ff->dwFileAttributes) ?	folder : file;
	return AddItem(ff->cFileName,attrib,0,(WIN32_FIND_DATAW*)&ff,0);
}

BOOL Panel::AddGuidItem(LPWSTR name, LPWSTR guid, HICON ic)
{
//Guid *.lnk file bo'lishi mumkin:
	if(SUCCEEDED(persFileShlLnk->Load((LPCOLESTR)guid, STGM_READ)))
	{	HICON pic = ic;
		if(!ic)
		{	wchar_t icLoc[MAX_PATH];int ind;
			if(SUCCEEDED(shlLnk->GetIconLocation(icLoc,MAX_PATH-1,&ind)))
				pic = ExtractIcon(hInst,icLoc,ind);
		}
		AddShellLinkItem(name,guid,pic);
		shlLnk->Resolve(hWnd, 0);
		return TRUE;
	}

	if(!(totItems < iMaxItems-1))
		{ if(!IncHeap()) return FALSE; }

	pItems[totItems].attribute = guidElement;
	pItems[totItems].hMasterIcon = ic;
	
	pItems[totItems].SetGuidName(this,name,guid,FALSE);

	if(!ic) pItems[totItems].assocNum = -1;

	pItems[totItems].state = normal;

	int tmp = pItems[totItems].GetNameWidth() + pItems[totItems].GetExtWidth() + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;
	if(itemMaxWidth < tmp)//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS)
		itemMaxWidth = tmp;//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;

	pItems[totItems].nColumn = rcClient.bottom?((totItems * itemHeight) / rcClient.bottom):1;
	pItems[totItems].nRow = rcClient.bottom ? (totItems % (itemHeight?(rcClient.bottom / itemHeight):1)):1;

	if(nColumns < pItems[totItems].nColumn)
		nColumns = pItems[totItems].nColumn;
	//if(nRows < pItems[totItems].nRow+1)
	//	nRows = pItems[totItems].nRow+1;

	totItems++;
	return TRUE;
}

BOOL Panel::AddStartupSheetTabs()
{
wchar_t s[64]; TCITEMW tie;
tie.mask = TCIF_TEXT|TCIF_PARAM;
tie.cchTextMax = 64;

	for(int i=0; i<conf::iSheetTabs[iThis]; i++)//Insert teskari bo'lgani uchun:
	{	wchar_t *ps = &conf::sheet_altPath[iThis][i*MAX_PATH];
		if(*ps != 0)
		{	tie.pszText = ps;
		}
		else
		{	ps = &conf::sheet_path[iThis][i*MAX_PATH];
			if(':'==ps[1])
			{	s[0]=ps[0];
				s[1]=':';
				s[2]=0;
			}else s[0]=0;
			wchar_t* p=MyStrRChrN(ps,MAX_PATH-1,'\\',2);
			if(p)
				MyStringCat(s,64,p);
			MyStringRemoveLastChar(s,64,'*');
			MyStringRemoveLastChar(s,64,'\\');
			tie.pszText = s;
		}
		if(!IsDirExist(ps))
			continue;

		SheetTab* sheet = new SheetTab();
		sheet->entrType = directFolder;
		sheet->SetPath(&conf::sheet_path[iThis][i*MAX_PATH]);
		sheet->SetAltPath(&conf::sheet_altPath[iThis][i*MAX_PATH]);
		sheet->viewMenuBitsU32 = conf::sheet_viewMenuBitsU32[iThis][i];
		tie.lParam = (LPARAM)sheet;

		//TabCtrl_InsertItem(sheetTab[iThis], i, &tie);
		::SendMessage(sheetTab[iThis],TCM_INSERTITEM,i,(LPARAM)&tie);
	}
	TabCtrl_SetCurSel(sheetTab[iThis],conf::iCrntSheetTab[iThis]);
	return TRUE;
}

BOOL Panel::AddNewSheetTab()
{
	int rows = TabCtrl_GetRowCount(sheetTab[iThis]);
	if(rows>0.027*panelHeight[iThis]) return FALSE;

TCITEMW tie; tie.mask = TCIF_TEXT|TCIF_PARAM; tie.cchTextMax = 64;// | TCIF_IMAGE; 
wchar_t *p,s[MAX_PATH];//int l;

	if(virtualPanel==entry.GetCrntRecType())
	{	return FALSE;
		/*l=MyStringCpy(s,MAX_PATH-1,(wchar_t*)vrtlPanels::plgns[iVPPlg].GetPluginName());
		s[l++]=':';
		s[l++]=' ';
		vrtlPanels::plgns[arch.plgNum].GetItemPathAndName$16(
						  vrtlPanels::plgns[iVPPlg].plgObj,
						  &s[l],MAX_PATH-1-l,
						  pItems[1].itemId);
		p=wcsrchr(&s[l],'\\');
		if(p)*(p+1)=0;*/
	}
	else
	{	if(':'==path[1])
		{	s[0]=path[0];
			s[1]=':';
			s[2]=0;
		}else s[0]=0;
		p=MyStrRChrN(path,MAX_PATH-1,'\\',2);
		if(p)MyStringCat(s,MAX_PATH-1,p);
		MyStringRemoveLastChar(s,MAX_PATH-1,'*');
		MyStringRemoveLastChar(s,MAX_PATH-1,'\\');
		if(!s[0]) MyStringCpy(s,MAX_PATH-1,L"Desktop");
	}
	tie.pszText = s;
	
	SheetTab* sheet = new SheetTab();
	sheet->SetPath(path);
	sheet->viewMenuBitsU32=viewMenuBitsU32;
	tie.lParam = (LPARAM)sheet;

	int oldTid = TabCtrl_GetCurSel(sheetTab[iThis]);
	if(oldTid!=-1) SheetTab::Save(sheetTab[iThis],oldTid,this);

	int iTabSheets=TabCtrl_GetItemCount(sheetTab[iThis]);
	//TabCtrl_InsertItem(sheetTab[iThis], iTabSheets, &tie);
	::SendMessage(sheetTab[iThis],TCM_INSERTITEM,iTabSheets,(LPARAM)&tie);
	TabCtrl_SetCurSel(sheetTab[iThis],iTabSheets);

	int btmBarHeght = (1==conf::Dlg.bShowBtmPnl?bottomBar.CalcHeight(::rcClient.right):0);

	if(rows!=TabCtrl_GetRowCount(sheetTab[iThis]))
	{	int y=panelsTop;
		if(1==conf::Dlg.bShowDiskSpace[iThis])
			y += HEIGHT_OF_DISK_SPACE_IMAGE;		
		CmnCntrl::ResizeEachPanelControls(iThis,&y,btmBarHeght);//CmnCntrl::ResizeControls();
	}
	iTabSheets = TabCtrl_GetItemCount(sheetTab[iThis]);
	iTabSheet = iTabSheets-1;

	::SetFocus(hWnd);
	return TRUE;
}

BOOL Panel::AddShellLinkItem(LPWSTR name, LPWSTR shllnk, HICON ic)
{
	if(!(totItems < iMaxItems-1))
		{ if(!IncHeap()) return FALSE; }

	pItems[totItems].attribute = shlLnkElement;
	pItems[totItems].hMasterIcon = ic;
	
	MyStringCpy(pItems[totItems].Name,MAX_GUID_CHAR,name);
	MyStringCpy(&pItems[totItems].Name[GUID_CHAR_POS],GUID_CHAR_LEN,shllnk);

	size_t ln = MyStringLength(name, STRSAFE_MAX_CCH);//StringCchLength(lpstr, STRSAFE_MAX_CCH, &ln);
	pItems[totItems].NameCharLen = (U8)(ln);
	pItems[totItems].ExtCharLen = 0;
	pItems[totItems].SetNameWidth(MyGetTextExtentPoint32(pItems[totItems].Name,(int)ln));//GetTextExtentPoint32(dc,pItems[totItems].Name,ln,&szName);
	pItems[totItems].SetExtWidth(MyGetTextExtentPoint32(L"[]",2));//GetTextExtentPoint32(dc,"[]",2,&szExt);//szExt.cx = szExt.cy = 0;
	if(!ic) pItems[totItems].assocNum = -1;

	pItems[totItems].state = normal;

	int tmp = pItems[totItems].GetNameWidth() + pItems[totItems].GetExtWidth() + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;
	if(itemMaxWidth < tmp)//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS)
		itemMaxWidth = tmp;//szName.cx + szExt.cx + MIN_DIST_FROM_NAME_AND_EXTENSION_PLUS_MIN_DIST_FROM_COLUMNS;

	pItems[totItems].nColumn = rcClient.bottom?((totItems * itemHeight) / rcClient.bottom):1;
	pItems[totItems].nRow = rcClient.bottom ? (totItems % (itemHeight?(rcClient.bottom / itemHeight):1)):1;

	if(nColumns < pItems[totItems].nColumn)
		nColumns = pItems[totItems].nColumn;
	//if(nRows < pItems[totItems].nRow+1)
	//	nRows = pItems[totItems].nRow+1;

	totItems++;
	return TRUE;
}

VOID Panel::AddToSelection(int id)
{
	if(id<1)return;
	if(id>totItems-1)return;
	if(selected==pItems[id].state)return;
	pItems[id].state = selected;
	RenderItem(id);
	++totSelects;
	if(file==pItems[id].attribute)
	{	selectedItemsSizes += pItems[id].size;
		++totSelectsFiles;
	}
	else if(folder==pItems[id].attribute)
			++totSelectsFolders;
}

VOID Panel::AdjustHorScrollity()
{	//profilerOutStr("Beg of Panel::AdjustHorScrollity ")
	int HEIGHT = rcClient.bottom;//SetScrollInfodan so'ng ozgararkan;
	int oldnMax = scrlInfo.nMax;

	scrlInfo.fMask = SIF_POS|SIF_RANGE;
	scrlInfo.nMax = 0;
	scrlInfo.nMin = 0;
	scrlInfo.nPos = 0;
	SetScrollInfo(hWnd, SB_VERT, &scrlInfo, FALSE);

	nRows = itemHeight?(HEIGHT/itemHeight):1;
	int nWaitingColumns = nRows?(totItems / nRows):0;
	if(nWaitingColumns*nRows < totItems)++nWaitingColumns;
	if(oldnMax<1)
	if(scrlBarHeight>0)
	if(nWaitingColumns>1)
	{	nRows = itemHeight?((HEIGHT-scrlBarHeight)/itemHeight):1;
		nWaitingColumns = nRows?(totItems / nRows):0;
		if(nWaitingColumns*nRows < totItems)++nWaitingColumns;
	}

	scrlInfo.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;
	scrlInfo.nMax = itemMaxWidth*nWaitingColumns;
	scrlInfo.nPage = itemMaxWidth;
	scrlInfo.nPos = 0;
	ScrollOffst = 0;
	SetScrollInfo(hWnd, SB_HORZ, &scrlInfo, FALSE);
	if(oldnMax<1)
	if(0==scrlBarHeight)
	if(scrlInfo.nMax>0)
	{	SCROLLBARINFO sbi;sbi.cbSize=sizeof(SCROLLBARINFO);
		if(GetScrollBarInfo(hWnd,OBJID_HSCROLL,&sbi))
		if(sbi.dxyLineButton<height)
		{	scrlBarHeight = sbi.dxyLineButton;
			nRows = itemHeight?((HEIGHT-scrlBarHeight)/itemHeight):1;
			nWaitingColumns = nRows?(totItems / nRows):0;
			if(nWaitingColumns*nRows < totItems)++nWaitingColumns;
			scrlInfo.nMax = itemMaxWidth*nWaitingColumns;
			SetScrollInfo(hWnd, SB_HORZ, &scrlInfo, FALSE);
	}	}
	RecalcItemsRects();
	InvalidateRect(hWnd,NULL,TRUE);
	return;
}

VOID Panel::AdjustVerScrollity()
{	//profilerOutStr("Beg of Panel::AdjustVerScrollity ")
	scrlInfo.fMask = SIF_POS|SIF_RANGE;
	scrlInfo.nMin = 0;
	scrlInfo.nMax = 0;
	scrlInfo.nPos = 0;
	SetScrollInfo(hWnd, SB_HORZ, &scrlInfo, FALSE);

	nRows = itemHeight?(rcClient.bottom/itemHeight):1;

	scrlInfo.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;//hor da 1 columndan surilar edi;
	scrlInfo.nMin = 0;
	scrlInfo.nMax = totItems*itemHeight;//-rcClient.bottom;
	scrlInfo.nPage = rcClient.bottom;
	scrlInfo.nPos = 0;
	ScrollOffst = 0;
	SetScrollInfo(hWnd, SB_VERT, &scrlInfo, FALSE);

	RecalcItemsRects();
	InvalidateRect(hWnd,NULL,TRUE);
	return;
}

BOOL Panel::AllocGuidFolder(int clsid)
{
wchar_t path[MAX_PATH];
	if(!SHGetSpecialFolderPath(hWnd,path,clsid,FALSE))
	{	if(S_OK!=SHGetFolderPath(hWnd,clsid,NULL,SHGFP_TYPE_CURRENT,path))
		{	if(S_OK!=SHGetFolderPath(hWnd,clsid,NULL,SHGFP_TYPE_DEFAULT,path))
				return FALSE;
	}	}
	return ChangePath(path);//if(!IsDirExist(path)) return FALSE; o'zida bor;
}

BOOL Panel::AllocVirtualGuidFolder(int clsid)
{
/*LPITEMIDLIST pidl = NULL;
	if(S_OK!=SHGetSpecialFolderLocation(hWnd,clsid,&pidl))return FALSE;
	if(pidl)
		CoTaskMemFree(pidl);*/
	switch(clsid)
	{	case CSIDL_DRIVES://my computer
			return ChangeToGuidPath(MakeGuidPath(L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}",L"My Computer"));
		case 0:          //desktop
			return ChangeToGuidPath(MakeGuidPath(L"",L"Desktop"));
		case CSIDL_CONTROLS:
			return ChangeToGuidPath(MakeGuidPath(L"::{26EE0668-A00A-44D7-9371-BEB064C98683}",L"Control Panel"));//21EC2020-3AEA-1069-A2DD-08002B30309D}");
		case CSIDL_NETWORK:
			return ChangeToGuidPath(MakeGuidPath(L"::{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}",L"Network"));
		case CSIDL_BITBUCKET:
			return ChangeToGuidPath(MakeGuidPath(L"::{645FF040-5081-101B-9F08-00AA002F954E}",L"Recycle"));
		case CSIDL_PRINTERS:
			return ChangeToGuidPath(MakeGuidPath(L"::{2227A280-3AEA-1069-A2DE-08002B30309D}",L"Printers"));
	}
	return FALSE;//ChangeToGuidPath(reinterpret_cast<wchar_t*>(path));
}

//Bekor(idle)da turganida papkalarni ichma-ich razmerlarini hisoblab tursun:
VOID Panel::CalcFoldersSizes()//Yadro urovnida drayverga qilaman;
{
	if(wcscmp(path,FoldersSizes.pathFor))
	{	MyStringCpy(FoldersSizes.pathFor,MAX_PATH-1,path);
		FoldersSizes.crntItemFor = 0;//Boshidan boshlaymiz;
	}

	//Har bir kirishda faqat bitta folderni hisoblaymiz:
	for(int i=FoldersSizes.crntItemFor; i<totItems; i++)
	{	if(pItems[i].attribute==folder)
		{	if(FolderSize::CalcFldrSize(iThis,i))
				FoldersSizes.crntItemFor = i+1;
			else return;
		}
		else FoldersSizes.crntItemFor = i;
}	}

BOOL Panel::CancelDiskCBMode()
{
	if(!hDiskCB)return FALSE;
	DestroyWindow(hDiskCB);
	hDiskCB = NULL;
	::SetFocus(hWnd);
	return TRUE;
}

BOOL Panel::CancelEditItemMode(BOOL bRename)
{
	if(!hEditItem)return FALSE;
	if(bRename)
	{	wchar_t renm[MAX_PATH],src[MAX_PATH];//,dst[MAX_PATH];
		int ln=GetWindowText(hEditItem,renm,MAX_PATH);
		
		//Check for unallowed symbols: < > : " / \ | ? *
		wchar_t ch[MAX_PATH];ch[0]=' ';
		for(int i=0; i<ln; ++i)
		{	if('<'==renm[i])
				ch[0]='<';
			else if('>'==renm[i])
				ch[0]='>';
			else if(':'==renm[i])
				ch[0]=':';
			else if('"'==renm[i])
				ch[0]='"';
			else if('/'==renm[i])
				ch[0]='/';
			else if('|'==renm[i])
				ch[0]='|';
			else if('?'==renm[i])
				ch[0]='?';
			else if('*'==renm[i])
				ch[0]='*';
			if(ch[0]!=' ')
			{	MessageBox(hWnd,L"File name cannot contain character:",ch,MB_OK);
				return FALSE;
		}	}
		//.gacha tekshiramiz:
		MyStringCpy(ch,MAX_PATH,renm);
		wchar_t *p=wcsrchr(ch,'.');
		if(p)
		{	p=0;
			if(0==wcscmp(ch,L"CON"))p=&ch[0];
			else if(0==wcscmp(ch,L"PRN"))p=&ch[0];
			else if(0==wcscmp(ch,L"AUX"))p=&ch[0];
			else if(0==wcscmp(ch,L"NUL"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM1"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM2"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM3"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM4"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM5"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM6"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM7"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM8"))p=&ch[0];
			else if(0==wcscmp(ch,L"COM9"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT1"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT2"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT3"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT4"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT5"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT6"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT7"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT8"))p=&ch[0];
			else if(0==wcscmp(ch,L"LPT9"))p=&ch[0];
			if(p)
			{	MessageBox(hWnd,L"CON,PRN,AUX,NUL,COM1,COM2,COM3,COM4,COM5,COM6,COM7,COM8,COM9,LPT1,LPT2,LPT3,LPT4,LPT5,LPT6,LPT7,LPT8,LPT9",L"File name cannot contain words:",MB_OK);
				return FALSE;
		}	}

		DestroyWindow(hEditItem);
		hEditItem = NULL;

		//Build src:
		MyStringCpy(src,MAX_PATH-1,path);
		wchar_t *pch = wcsrchr(src,'*');
		if(pch) *pch = 0;
		MyStringCat(src,MAX_PATH-1,iEditItem->Name);

		//Build dst:
		MyStringCpy(/*dst*/findItemName,MAX_PATH-1,path);
		pch = wcsrchr(/*dst*/findItemName,'*');
		if(pch) *pch = 0;
		MyStringCat(/*dst*/findItemName,MAX_PATH-1,renm);

		BOOL r;
		if(socketCl==entry.GetCrntRecType())//o'tishi shart;
			r=linkSock::SendToServerMsg(iThis,LNKSCKMSG_RenameFileOrDirectory,src,findItemName);
		if(archElem==entry.GetCrntRecType() || 
		  (rndPathList==entry.GetCrntRecType() && arch.plgObj))
		{	int l=MyStringCpy(src,MAX_PATH-1,entry.GetCrntRecArchPath());
			MyStringCpy(&src[l],MAX_PATH-l-1,iEditItem->Name);
			if(arch.Rename$12(src,renm,folder==iEditItem->attribute))
			{	iEditItem->SetName(this,renm,file==iEditItem->attribute);
				iEditItem->Render(0,this);
		}	}
		else
		{r=MyMoveFile(src,findItemName);//dst))
		 if(!r)
			r=MySHFileOperation(FO_RENAME,src,findItemName,//dst,
				FOF_ALLOWUNDO|FOF_SILENT|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR,FALSE);
		}
		if(r)
		  MyStringCpy(findItemName,MAX_PATH-1,renm);
	}
	else
	{	DestroyWindow(hEditItem);
		hEditItem = NULL;
	}

	::SetFocus(hWnd);
	//HDC dc = ::GetDC(hWnd);
	//iEditItem->Render(dc,this,TRUE);
	//::ReleaseDC(hWnd,dc);
	Render(0);
	//SetHot(EditItemId);//::PostMessage(hWnd,WM_USER,2,EditItemId);//notify tekislaydur;Select item;
	EditItemId = 0;
	return TRUE;
}

BOOL Panel::CancelQuickSearchMode(BOOL escape)
{
int oldHot;
	if(!hQuickSearchEdit)return FALSE;
	if(escape)
		oldHot = EditItemId;//GetWindowLong(hQuickSearchEdit,GWLP_USERDATA);
	DestroyWindow(hQuickSearchEdit);
	hQuickSearchEdit = NULL;
	::SetFocus(hWnd);
	if(escape)
		SetHot(oldHot);
	return TRUE;
}

VOID Panel::ChangeColor(COLORREF *col,int type,int init)
{
	switch(type)
	{case 0:
		for(int i=0; i<6; i++)
			color[i] = col[i];
		if(bkBrsh)DeleteObject(bkBrsh);
		bkBrsh = CreateSolidBrush(color[0]);//CreateHatchBrush(HS_BDIAGONAL,color[0]);
		if(txtBkBrsh)DeleteObject(txtBkBrsh);
		txtBkBrsh = CreateSolidBrush(color[2]);
		if(hotBkBrsh)DeleteObject(hotBkBrsh);
		hotBkBrsh = CreateSolidBrush(color[5]);
		if(!init)Render();
		break;
	 case 1:
		if(brEdit)DeleteObject(brEdit);
		brEdit = CreateSolidBrush(col[2]);
		if(!init)
		{	RedrawWindow(pathAndNameEdt[1],NULL,NULL,RDW_INVALIDATE);
			RedrawWindow(selFilesInfoEdt,NULL,NULL,RDW_INVALIDATE);
		}
		break;
	 case 2:
		if(hBrshSrtBtn)DeleteObject(hBrshSrtBtn);
		hBrshSrtBtn = CreateSolidBrush(col[2]);
		if(!init)
		{	RedrawWindow(sortBtns[iThis][0],NULL,NULL,RDW_INVALIDATE);
			RedrawWindow(sortBtns[iThis][1],NULL,NULL,RDW_INVALIDATE);
			RedrawWindow(sortBtns[iThis][2],NULL,NULL,RDW_INVALIDATE);
		}
		break;	
}	}

VOID Panel::ChangeFont(LOGFONT *lf,int type,int init)
{
	switch(type)
	{case 0:
		if(hf)DeleteObject(hf);
		//lf->lfQuality = 1;//PROOF_QUALITY(2);//DRAFT_QUALITY(1);//DEFAULT_QUALITY(0);//ANTIALIASED_QUALITY(4);
		hf=CreateFontIndirect(lf);
		::SendMessage(sheetTab[iThis],WM_SETFONT,(WPARAM)hf,TRUE);
		ResetGetTextExtentPoint32();
		if(!init)Render();
		break;
	 case 1:
		if(hfEdit)DeleteObject(hfEdit);
		hfEdit = CreateFontIndirect(&conf::Dlg.editFnt[iThis]);
		if(!init)
		{	RedrawWindow(pathAndNameEdt[1],NULL,NULL,RDW_INVALIDATE);
			RedrawWindow(selFilesInfoEdt,NULL,NULL,RDW_INVALIDATE);
		}
	    break;
	 case 2:
		if(hfSrtBtn)DeleteObject(hfSrtBtn);
		hfSrtBtn = CreateFontIndirect(&conf::Dlg.srtBtnFnt[iThis]);
		if(!init)
		{	for(int b=0; b<3; b++)
				RedrawWindow(sortBtns[iThis][b],NULL,NULL,RDW_INVALIDATE);
		}
		break;
}	}

VOID Panel::DeleteFromSelection(int id)
{
	if(id<1)return;
	if(id>totItems-1)return;
	if(pItems[id].state == selected)
	{	pItems[id].state = normal;
		RenderItem(id);
		--totSelects;
		if(file==pItems[id].attribute)
		{	selectedItemsSizes -= pItems[id].size;
			--totSelectsFiles;
		}
		else if(folder==pItems[id].attribute)
			--totSelectsFolders;
}	}

BOOL Panel::ChangeTooltipText(wchar_t* t,int ln,int id)
{
	if(id!=idForTooltip)return FALSE;
	toolTip.ChangeText(t,ln);
	return TRUE;
}


/*BOOL Panel::ChangePathA(char *p)
{
	if(!IsDirExistA(p))return FALSE;
	FreeMem();
	entry.Reset();
	if(p && p[0])
	{	pathLn=MultiByteToWideChar(AreFileApisANSI()?CP_ACP:CP_OEMCP,MB_PRECOMPOSED,p,MAX_PATH-1,path,MAX_PATH);
		if(pathLn)--pathLn;
		if('*'!=path[pathLn-1])
		{	if('\\'!=p[pathLn-1])
			{	path[pathLn++]='\\';
				path[pathLn++]='*';
				path[pathLn]=0;
			}
			else
			{	path[pathLn++]='*';
				path[pathLn]=0;
		}	}
		else if('\\'!=p[pathLn-2])
		{	path[pathLn++]='\\';
			path[pathLn++]='*';
			path[pathLn]=0;
	}	}
	entry.SetPushEntryInStartup(path);
	FillDirectFolder();
	entry.Push(path,directFolder,0);
	MySetCurrentDirectoryA(p);
	AdjustScrollity();
	SetHot(0);
	ScrollItemToView(0);
	ClrScr();
	Render(NULL);
	ShowPathAndName(-1);
	return TRUE;
}*/

BOOL Panel::ChangePath(wchar_t *pW)
{
	if(!IsDirExist(pW))return FALSE;
	//if(MyStringRemoveLastCharCheckPre(p,MAX_PATH-1,'*','\\'))
	//	MyStringCat(p,MAX_PATH-1,"\\*");
	FreeMem();
	entry.Reset();
	if(pW && pW[0])
	{	pathLn=MyStringCpy(path,MAX_PATH-1,pW);//SetPath(p,MAX_PATH);
		if('*'!=path[pathLn-1])
		{	if('\\'!=pW[pathLn-1])
			{	path[pathLn++]='\\';
				path[pathLn++]='*';
				path[pathLn]=0;
			}
			else
			{	path[pathLn++]='*';
				path[pathLn]=0;
		}	}
		else if('\\'!=pW[pathLn-2])
		{	path[pathLn++]='\\';
			path[pathLn++]='*';
			path[pathLn]=0;
	}	}
	entry.SetPushEntryInStartup(path);
	FillDirectFolder();
	entry.Push(path,directFolder,0);
	MySetCurrentDirectory(pW);
	AdjustScrollity();
	SetHot(0);
	ScrollItemToView(0);
	ClrScr();
	Render(NULL);
	ShowPathAndName(-1);
	return TRUE;
}

BOOL Panel::SetVirtual(wchar_t *pW,LPVOID plgObj,int iplgn)
{
	FreeMem();
	pathLn=MyStringCpy(path,MAX_PATH-1,pW);//SetPath(p,MAX_PATH);
	if('*'!=path[pathLn-1])
	{	if('\\'!=pW[pathLn-1])
		{	path[pathLn++]='\\';
			path[pathLn++]='*';
			path[pathLn]=0;
		}
		else
		{	path[pathLn++]='*';
			path[pathLn]=0;
	}	}
	else if('\\'!=pW[pathLn-2])
	{	path[pathLn++]='\\';
		path[pathLn++]='*';
		path[pathLn]=0;
	}
	VPPlgObj = plgObj;
	iVPPlg = iplgn;
	entry.PushToVirtualPanel(path,0,plgObj,iplgn);
	AddItem(L"..",header);
	ClrScr();
	Render(0);
	return TRUE;
}

BOOL Panel::ChangeToGuidPath(wchar_t* p)
{
	FreeMem();
	entry.Reset();
	entry.SetPushGuidEntryInStartup(p);

	SetPath(p,MAX_PATH/2,FALSE);
	FillGuidFolder();
	entry.Push(p,guidFolder,0);//reinterpret_cast<char*> edi
	AdjustScrollity();
	SetHot(0);
	ScrollItemToView(0);
	ClrScr();
	Render(NULL);
	ShowPathAndName(-1);
	return TRUE;
}

//Pathi oz'garsa, o'zgartirsun:
BOOL Panel::ChangeSheetTabPath()
{
int iTabSheets = TabCtrl_GetItemCount(sheetTab[iThis]);
if(!iTabSheets) return FALSE;
int iCrntTabSheet = TabCtrl_GetCurSel(sheetTab[iThis]);
if(-1==iCrntTabSheet)return FALSE;

wchar_t *p,s[MAX_PATH];
TCITEMW tie;tie.cchTextMax=64;
	tie.mask = TCIF_TEXT|TCIF_PARAM;tie.pszText=s;
	//TabCtrl_GetItem(sheetTab[iThis],iCrntTabSheet,&tie);
	::SendMessage(sheetTab[iThis],TCM_GETITEM,iCrntTabSheet,(LPARAM)&tie);

	if(virtualPanel==entry.GetCrntRecType())
	{	int l=MyStringCpy(s,MAX_PATH-1,(wchar_t*)vrtlPanels::plgns[iVPPlg].GetPluginName());
		if(totItems>1)
		{	s[l++]=':';
			s[l++]=' ';
			vrtlPanels::plgns[iVPPlg].GetItemPathAndName$16(
						  VPPlgObj,
						  &s[l],MAX_PATH-1-l,
						  pItems[1].itemId);
			p=wcsrchr(&s[l],'\\');
			if(p)*(p+1)=0;
	}	}
	else
	{	if(':'==path[1])
		{	s[0]=path[0];
			s[1]=':';
			s[2]=0;
		}else s[0]=0;
		wchar_t* p=MyStrRChrN(path,MAX_PATH-1,'\\',2);
		if(p)
			MyStringCat(s,MAX_PATH-1,p);
		MyStringRemoveLastChar(s,MAX_PATH-1,'*');
		MyStringRemoveLastChar(s,MAX_PATH-1,'\\');
	}
	tie.pszText = s; 
	//TabCtrl_SetItem(sheetTab[iThis], iCrntTabSheet, &tie);
	::SendMessage(sheetTab[iThis],TCM_SETITEM,iCrntTabSheet,(LPARAM)&tie);

	SheetTab* sheet = (SheetTab*)tie.lParam;
	sheet->Save(sheetTab[iThis],iCrntTabSheet,this);
	return TRUE;
}

VOID Panel::ClrScr()//, PAINTSTRUCT *ps)
{
	//profilerOutStr("Beg of Panel::ClrScr:")
	HDC dc = ::GetDC(hWnd);
	FillRect(dc,&rcClient,bkBrsh);//(HBRUSH)GetStockObject(WHITE_BRUSH));
	::ReleaseDC(hWnd, dc);
}

BOOL Panel::ComboPathAndNameMsgQueue(WPARAM wParam,LPARAM lParam)
{
	switch(HIWORD(wParam))
	{	case CBN_CLOSEUP:
			wchar_t s[MAX_PATH];			
			int i; i=(int)::SendMessage(pathAndNameEdt[0],CB_GETCURSEL,0,0);
			if(CB_ERR==i) break;
			i=(int)::SendMessage(pathAndNameEdt[0],CB_GETLBTEXT,i,(LPARAM)s);
			if(CB_ERR==i) break;
			if(s[i-1]=='\\'){s[i]='*';s[i+1]=0;}
			else if(s[i-1]==':'){s[i]='\\';s[i+1]='*';s[i+2]=0;}
			else if(!(s[i-2]=='\\'&&s[i-1]=='*'))
			{s[i]='\\';s[i+1]='*';s[i+2]=0;}
			BOOL b;b = IsDirExist(s);
			if(!b) b = MySetCurrentDirectory(s);
			if(b)
			{	::DeselectAllWithout(-1);
				//FreeMem();
				entry.Reset();
				pathLn=MyStringCpy(path,MAX_PATH-1,s);
				entry.SetPushEntryInStartup(path);
				fManager.FillDirectFolder();
				entry.Push(path,directFolder,0);
				
				//MySetCurrentDirectory(s);
					
				AdjustScrollity();
				iHot = 0;
				ScrollItemToView(0);
				ClrScr();
				Render(NULL);
				::SetFocus(hWnd);
			}
			return TRUE;
		default:
			break;
	}
	return FALSE;
}

static LRESULT CALLBACK PanelWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
//profilerOutStr("Beg of PanelWndProc")
//profilerPrintf4("\nhWnd=%d message=%d wParam=%d lParam=%d",hWnd,message,wParam,lParam)

int panelNumber	= (int)GetWindowLongPtr(hWnd,GWLP_USERDATA);

	/*static int i=0;
	char s[260];sprintf(s,"\n %d ",i++);
	OutputDebugStringA(s);
	OutputDebugStringA(GetWinNotifyTextA(message));
	//sprintf(s," %x %x",wParam,lParam);
	//OutputDebugStringA(s);
	sprintf(s," iActivePanel:%d procPanel: %d PanelWndProc",Panel::iActivePanel,panelNumber);
	OutputDebugStringA(s);*/

 switch (message)
 {	case WM_CREATE:
		panelNumber	= *((int*)(lParam));
		break;
 	case WM_MOVE:
		break;
	case WM_MOUSEMOVE:
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
		{	if(p!=panelNumber)
				panel[p].GetToolTip()->Hide();
		}
		break;
	//case WM_LBUTTONDOWN://WM_MOUSEACT
	//	break;
	case WM_KEYDOWN:
		//if(lParam & 0x20000000)
		//{	return 0;
		//}
		if(9==wParam)//TAB
		{	PostMessage(::hWnd, WM_KEYDOWN, 9, 0);
			return 0;
		}
	//case WM_COMMAND:
	//	return panel[panelNumber].WndProc(hWnd,message,wParam,lParam);
	default:
	break;
 }
 return panel[panelNumber].WndProc(hWnd,message,wParam,lParam);
}

//wia panelNumber may define which panel ownerdraw was called:
BOOL Panel::Create(HINSTANCE inst,HWND prnt,int panelNumber,int x,int y,int w,int h,wchar_t* initPath)
{
//	profilerOutStr("Beg of Panel::Create ")
//	profilerPrintf8("inst=%x prnt=%x panN=%d x=%d y=%d w=%d h=%d pth=%s", inst,prnt,panelNumber,x,y,w,h,initPath)

//First create memory for object, if fail, return:
	if(!CreateHeap()) return FALSE;

//Create static parts:
	//if(!blueBrush) blueBrush = CreateSolidBrush(RGB(0,0,155));
	//if(!iconBckBrush)iconBckBrush = CreateSolidBrush(RGB(185,170,198));

wchar_t panWindText[16]=L"#";
wchar_t panNumbText[16]; StringCchPrintf(panNumbText,16,L"%d",panelNumber);
					  MyStringCat(panWindText,16,panNumbText);
wchar_t panClassText[32]=L"Sino panel class #";
					  MyStringCat(panClassText,32,panNumbText);

  if(!rcl)
  {	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= PanelWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= inst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= panClassText;
	wcex.hIconSm		= NULL;
	rcl = RegisterClassEx(&wcex);
  }

HWND hwnd = CreateWindowEx( WS_EX_LEFT|WS_EX_CLIENTEDGE,
							(LPCWSTR)rcl,//panClassText,
							panWindText,
							WS_GROUP|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP|WS_CHILDWINDOW,//See AdjustHorScrollity | WS_HSCROLL,
							x,
							y,
							w,
							h,
							prnt,
							NULL,
							inst,
							(LPVOID)panelNumber);
	if(NULL==hwnd)return FALSE;

	//hWnd = hwnd; -->WM_CREATE
	left = x; top = y; width = w; height = h;
	if(initPath)pathLn=MyStringCpy(path,MAX_PATH-1,initPath);//else InitPath b-n to'ldirilgan bo'lsa;
	if(!fManager.CreateChangeNotify(path))return FALSE;

	//Hozircha:
	rcClient.left=rcClient.top=0;
	rcClient.right=w;
	rcClient.bottom=h;

	SetIcons(inst);
	HDC dc = ::GetDC(hWnd);
	
	shlLnk = NULL;persFileShlLnk=NULL;
	CoInitialize(NULL);
	if(SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								  IID_IShellLink, (LPVOID*)&shlLnk)))
								  shlLnk->QueryInterface(IID_IPersistFile, (void**)&persFileShlLnk);
	CoUninitialize();
	CreateObjects();

	SetPath(initPath,MAX_PATH);
	entry.SetPushEntryInStartup(path);
	entry.PushFindType(path, 0, pathLn);
	if(!fManager.Fill()) return FALSE;

	::ReleaseDC(hWnd,dc);
	toolTip.Create(hInst,hWnd,panelNumber,10,10,pItems[0].Name,1);

	ReadToPathAndNameCombo(40);
	AddStartupSheetTabs();

	//RegisterShellHookWindow(hWnd);

	//profilerOutStr("Succsess end of Panel::Create")
	//CmnCntrl::ResizeControls();
	return TRUE;
}

BOOL Panel::CreateHeap()
{
	//profilerOutStr("Beg of Panel::CreateHeap ")

//growable. The heap's size is limited only by the available memory:
	pItems = (PanelItem*)malloc(iMaxItems*sizeof(PanelItem));
	if(!pItems)
		return FALSE;
	for(int it=0; it<iMaxItems; it++)
		pItems[it].PanelItem::PanelItem();
	totItems = 0;
	return TRUE;
}

VOID Panel::CreateObjects()
{
	HDC dc = ::GetDC(hWnd);
	int fntHeight=-MulDiv(conf::Dlg.panFnts[iThis].lfHeight,72,GetDeviceCaps(dc,LOGPIXELSY));
	SetItemHeight(fntHeight+conf::Dlg.interval[iThis]);
	ChangeFont(&conf::Dlg.panFnts[iThis],0,1);//own
	ChangeFont(&conf::Dlg.editFnt[iThis],1,1);//edt
	ChangeFont(&conf::Dlg.srtBtnFnt[iThis],2,1);//srtBtn
	ChangeColor(conf::Dlg.panRGBs[iThis],0,1);//Panel own
	ChangeColor(conf::Dlg.editRGB[iThis],1,1);//Panel edits
	ChangeColor(conf::Dlg.srtBtnRGB[iThis],2,1);//Panel edits
	imgBckgrnd[1].Load(this,conf::Dlg.panBckgrndImgPth[iThis]);
	imgBckgrnd[1].CloneOnWM_SIZE(this,&imgBckgrnd[0]);
	::ReleaseDC(hWnd,dc);
}


BOOL Panel::DecHeap(int minItems)
{
//profilerOutStr("Beg of Panel::DecHeap ")
//profilerPrintf1("\nminItems=%d",minItems)

	pItems = (PanelItem*)realloc(pItems,minItems * sizeof(PanelItem));
	if(!pItems)
		return FALSE;
	iMaxItems = minItems;
	for(int it=0; it<iMaxItems; it++)
		pItems[it].PanelItem::PanelItem();
	return TRUE;
}

BOOL Panel::DeleteCrntSheetTab()
{
int iCrntTabSheet = TabCtrl_GetCurSel(sheetTab[iThis]);
if(-1==iCrntTabSheet)return FALSE;

	int row = TabCtrl_GetRowCount(sheetTab[iThis]);

TCITEM tie;tie.mask=TCIF_PARAM;
	TabCtrl_GetItem(sheetTab[iThis],iCrntTabSheet,&tie);
	SheetTab* sheet = (SheetTab*)tie.lParam;
	int VrtlPnlPlgn = -1;
	if(virtualPanel==sheet->entrType)
		VrtlPnlPlgn = sheet->archPlgNum;

	sheet->Destroy();
	TabCtrl_DeleteItem(sheetTab[iThis],iCrntTabSheet);

	int btmBarHeght = (1==conf::Dlg.bShowBtmPnl?bottomBar.CalcHeight(::rcClient.right):0);

	if(iCrntTabSheet>0)
		TabCtrl_SetCurSel(sheetTab[iThis],--iCrntTabSheet);
	else
		TabCtrl_SetCurSel(sheetTab[iThis],iCrntTabSheet);
	int r = TabCtrl_GetRowCount(sheetTab[iThis]);
	if(r != row)
	{	int y=panelsTop;
		if(1==conf::Dlg.bShowDiskSpace[iThis])
			y += HEIGHT_OF_DISK_SPACE_IMAGE;		
		CmnCntrl::ResizeEachPanelControls(iThis, &y, btmBarHeght);//CmnCntrl::ResizeControls();
	}

	if(TabCtrl_GetItem(sheetTab[iThis],iCrntTabSheet,&tie))
	{	sheet = (SheetTab*)tie.lParam;
		sheet->Load(sheetTab[iThis],iCrntTabSheet,this);
	}
	//SetFocus(hWnd);
	if(VrtlPnlPlgn>-1)
	{	BOOL bPlgnLoaded=FALSE;
		for(int i=0; i<r; i++)
		{	if(TabCtrl_GetItem(sheetTab[iThis],iCrntTabSheet,&tie))
			{	sheet = (SheetTab*)tie.lParam;
				if(VrtlPnlPlgn==sheet->archPlgNum)
				{	bPlgnLoaded = TRUE;
					break;
		}	}	}
		if(!bPlgnLoaded)
			vrtlPanels::plgns[iVPPlg].FreePlugin();
	}
	return TRUE;
}

BOOL Panel::DeleteSheetTab(int shtbNum)
{
int iCrntTabSheet = TabCtrl_GetCurSel(sheetTab[iThis]);

	if(virtualPanel==entry.GetCrntRecType())
		return FALSE;
	int row = TabCtrl_GetRowCount(sheetTab[iThis]);

TCITEM tie;tie.mask=TCIF_PARAM;
	TabCtrl_GetItem(sheetTab[iThis],shtbNum,&tie);
	SheetTab* sheet = (SheetTab*)tie.lParam;
	sheet->Destroy();
	TabCtrl_DeleteItem(sheetTab[iThis],shtbNum);

	int btmBarHeght = (1==conf::Dlg.bShowBtmPnl?bottomBar.CalcHeight(::rcClient.right):0);

	int r = TabCtrl_GetRowCount(sheetTab[iThis]);
	if(r != row)
	{	int y=panelsTop;
		if(1==conf::Dlg.bShowDiskSpace[iThis])
			y += HEIGHT_OF_DISK_SPACE_IMAGE;		
		CmnCntrl::ResizeEachPanelControls(iThis, &y, btmBarHeght);//CmnCntrl::ResizeControls();
	}

	if(iCrntTabSheet==shtbNum)
	{	TabCtrl_SetCurSel(sheetTab[iThis],--iCrntTabSheet);
		if(TabCtrl_GetItem(sheetTab[iThis],iCrntTabSheet,&tie))
		{	sheet = (SheetTab*)tie.lParam;
			sheet->Load(sheetTab[iThis],iCrntTabSheet,this);
	}	}
	//SetFocus(hWnd);
	return TRUE;
}

//id dan boshqa hammasini qizil qilib belgilanganini olib tashlash:
VOID Panel::FreeSelection()
{
	for(int i=0; i<totItems; i++)
	{	if(pItems[i].state!=normal)
		{	pItems[i].state=normal;
			RenderItem(i);
	}	}
	totSelects = 0;
	totSelectsFolders = 0;
	totSelectsFiles = 0;
	selectedItemsSizes = 0;
}

VOID Panel::Destroy()
{
	if(!hWnd) return;
	if(hIcons[0])
	{	for(int i=0; i<TOTAL_ICONS; i++)
		{	DeleteObject(hIcons[i]);
			hIcons[i] = NULL;
	}	}
	toolTip.Destroy();
	if(hf)DeleteObject(hf);
	if(hfEdit)DeleteObject(hfEdit);
	if(hfSrtBtn)DeleteObject(hfSrtBtn);

	if(bkBrsh)DeleteObject(bkBrsh);
	if(txtBkBrsh)DeleteObject(txtBkBrsh);
	if(hotBkBrsh)DeleteObject(hotBkBrsh);
	if(hBrshSrtBtn)DeleteObject(hBrshSrtBtn);
	if(brEdit)DeleteObject(brEdit);
	
	hf=0;hfEdit=0;bkBrsh=0;hfSrtBtn=0;
	txtBkBrsh=0;hotBkBrsh=0;hBrshSrtBtn=0;brEdit=0;

	if(persFileShlLnk) persFileShlLnk->Release();persFileShlLnk = NULL;
	if(shlLnk) shlLnk->Release(); shlLnk=NULL;

	//Other parts:
	fManager.DestroyChangeNotify();//fManager.ExitNotifyThread();
	DeregisterShellHookWindow(hWnd);
	hWnd = NULL;
	return;
}

VOID Panel::DropFiles(HDROP hDrop)//Drop from sino self
{
wchar_t	szFile[MAX_PATH];      // Used to store name of dropped file
UINT	wNumFiles, cb;
wchar_t dst[MAX_PATH];

    //OutputDebugString("\nPanel::DropFiles");

	MyStringCpy(dst,MAX_PATH-1,path);
	MyStringRemoveLastChar(dst,MAX_PATH-1,'*');

	POINT pInDrag;DragQueryPoint(hDrop, &pInDrag);//In hDrop handled window client zone
	ClientToScreen(::hWnd,&pInDrag);//hDrop handled in global Sino hWnd
	ScreenToClient(hWnd,&pInDrag);//hDrop handled in global Sino hWnd
	BOOL bDragToOwnPanel=FALSE;
	if(pInDrag.x<rcClient.right)
		if(pInDrag.y<rcClient.bottom)
			bDragToOwnPanel=TRUE;

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hWnd,&p);
	int overItem = GetItemNum(p.x, p.y);

	if(pItems[overItem].attribute==folder)
	{	MyStringCat(dst,MAX_PATH-1,pItems[overItem].Name);
		MyStringCat(dst,MAX_PATH-1,L"\\");
	}
	else
	{	if(bDragToOwnPanel)//Bo'lmasa shu papkaning o'ziga tashla;
		{	//SetCursor(LoadCursor(hInst, (LPCSTR)IDC_CURSOR_NODROP2));
			//goto Fin;
	}	}
	int ln = MyStringLength(dst,MAX_PATH);

	wNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	for(UINT i=0; i<wNumFiles; i++)
	{	cb = DragQueryFile(hDrop, i, szFile, MAX_PATH);
		if (cb == 0) break;
		int sln=MyStringLength(szFile,MAX_PATH);
		szFile[sln+1]=0;
		wchar_t *p = wcsrchr(szFile,'\\');
		if(p)
		{	MyStringCat(dst,MAX_PATH-1,p+1);
			SHFILEOPSTRUCT fo;fo.hwnd=::hWnd;fo.wFunc=FO_COPY;
			fo.pFrom=szFile;fo.pTo=dst;fo.fFlags=FOF_WANTNUKEWARNING;
			SHFileOperation(&fo);
			dst[ln]=0;
	}	}
//Fin:
	DragFinish(hDrop);
}

BOOL Panel::IsDstWindowDraggable(HWND h)
{
int i;
	if(hWnd==h)return FALSE;
	if(GetDesktopWindow()==h)
		return TRUE;
	if(cmndsCBEdt==h)return TRUE;
	if(BtnsPanel::hBtnsPnlWnd==h)return TRUE;
	for(i=0; i<conf::Dlg.iTotPanels; ++i)
	{	if(panel[i].GetHWND()==h)return TRUE;
		if(DskToolBar::bar[i]==h)return TRUE;
		if(panel[i].GetPathAndNameEdt(1)==h)return TRUE;
	}
	for(i=0; i<7; ++i)
	{	if(btns[i]==h)return FALSE;
	}
	//if(0==::SendMessage(h,WM_DROPFILES,0,0))
	//	return TRUE;
	return FALSE;
}

/*BOOL Panel::DragFiles(HWND h)
{
	if(GetDesktopWindow()==h)
	{	
	}
}*/

VOID Panel::EditItemMsgQueue(MSG *msg)
{
	if(!hEditItem)//This means, where no in edit item mode!!!
		return;
	if(hEditItem!=msg->hwnd)
		return;
	if(WM_CHAR==msg->message)//102h
	{	if(13==msg->wParam)//Enter;
			CancelEditItemMode(TRUE);
		else if(27==msg->wParam)//Esc;
			CancelEditItemMode(FALSE);
	}
	//PostMessage(hEditItem,msg->message,msg->wParam,msg->lParam);
	/*else
	{	if(15==msg->message)return;//WM_PAINT
		if(257==msg->message)return;//101h WM_KEYUP
		if(280==msg->message)return;//118h 
		if(512==msg->message)return;//200h WM_MOUSEFIRST
		if(256==msg->message)return;//100h WM_KEYFIRST
		if(513==msg->message)return;//201h WM_MOUSEMOVE
		//int i=0;
		//i=9;
}*/	}

VOID Panel::EditPathAndNameMsgQueue(MSG* msg)
{
char ipstrA[32];DWORD dwIP4;
	//static int i=0;
	//char ss[32];sprintf(ss,"\n %d ",i++);
	//OutputDebugStringA(ss);
	//OutputDebugStringA(GetWinNotifyText(msg->message));
	if(!pathAndNameEdt[1])//This means, where no in edit item mode!!!
		return;
	if(pathAndNameEdt[1]!=msg->hwnd)//This means, where no in edit item mode!!!
		return;
	if(WM_CHAR==msg->message)//102h
	{	wchar_t s[MAX_PATH];int l = GetWindowText(pathAndNameEdt[1],s,MAX_PATH);
		if(13==msg->wParam)//Enter;
		{	//wchar_t s[MAX_PATH];int l = GetWindowText(pathAndNameEdt[1],s,MAX_PATH);
			if(l)
			{	if(s[l-1]=='\\'){s[l]='*';s[l+1]=0;l++;}
				else if(!('*'==s[l-1] && '\\'==s[l-2]))
					{s[l]='\\';s[l+1]='*';s[l+2]=0;l+=2;}
				if(vrtlPanels::numIP4Plgn>-1 && MyInet4StrToDword(s,ipstrA,&dwIP4))
				{	//if(wcscmp(vrtlPanels::plgns[iVPPlg].name,L"AdptrIP4Scan"))
					{	//::DeselectAllWithout(-1);
						//FreeMem();
						//entry.Reset();
						//entry.SetPushEntryInStartup(path);
						MyStringCpy(s,MAX_PATH,L"\\\\?\\UNC\\");
						l=8+MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,ipstrA,-1,&s[8],MAX_PATH)-1;
						s[l++]='\\';s[l++]='*';s[l]=0;

						//arch.plgNum = vrtlPanels::numIP4Plgn;
						//entry.Push(path,virtualPanel,0);
						//AddItem(L"..",header);
						if(vrtlPanels::AttachPanel$12(this,arch.plgNum,s))
						{	s[l-2]=0;s[6]='\\';
							CBToDisk::AddToCB(pathAndNameEdt[0],&s[6],TRUE);
				}	}	}
				else if(IsDirExist(s))
				{	::DeselectAllWithout(-1);
					FreeMem();
					entry.Reset();
					pathLn=MyStringCpy(path,MAX_PATH-1,s);
					entry.SetPushEntryInStartup(path);
					fManager.FillDirectFolder();
					entry.Push(path,directFolder,0);
					
					MySetCurrentDirectory(s);
					
					AdjustScrollity();
					iHot=0;
					ScrollItemToView(0);
					ClrScr();
					Render(NULL);
					::SetFocus(hWnd);

					//Combo,save to disk
					if('*'==s[l-1] && '\\'==s[l-2]) s[l-2]=0;
					else if('\\'==s[l-1]) s[l-1]=0;
					//Combo,save to disk
					CBToDisk::AddToCB(pathAndNameEdt[0],s,TRUE);
				}
				else if(iHot>0 && iHot<totItems)
				{	
					ShowPathAndName(iHot);
		}	}	}
		else if(27==msg->wParam)//Esc;
		{	::SetFocus(hWnd);
			DeselectAllWithout(-1);
			//iHot=oldSelectId;
		}
		else if(9==msg->wParam)//Tab;
		{	int toP;
			switch(conf::Dlg.iTotPanels)
			{	case 1:
				return;
				case 2:
					toP = (hWnd==panel[0].hWnd)?1:0;
				break;
				case 3:
					toP = (hWnd==panel[0].hWnd)?1:((hWnd==panel[1].hWnd)?2:0);
				break;
				case 4:
					toP = (hWnd==panel[0].hWnd)?1:((hWnd==panel[1].hWnd)?2:(hWnd==panel[2].hWnd)?3:0);
				break;
			}
			::SetFocus(panel[toP].pathAndNameEdt[0]);
			::SendMessage(panel[toP].pathAndNameEdt[1],EM_SETSEL,0,-1);
		}
		else
		{	int r=FilesToCBEdit::Attach(pathAndNameEdt[0],pathAndNameEdt[1],TRUE,TRUE,&s[0],0);
			::SetFocus(pathAndNameEdt[1]);
			::SendMessage(pathAndNameEdt[1],EM_SETSEL,-1,-1);
	}	}
	else if(WM_LBUTTONDBLCLK==msg->message)
	{	
		::SendMessage(pathAndNameEdt[1],EM_SETSEL,0,-1);
	}
	else if(WM_KEYDOWN==msg->message)
	{	if(0x28==msg->wParam)//Down
		{	//Agar CB ning listi ochilmagan bo'lsa:
			if(!::SendMessage(pathAndNameEdt[0],CB_GETDROPPEDSTATE,0,0))
			{	::DeselectAllWithout(-1);
				SetFocus();
	}	}	}
	else if(WM_LBUTTONDOWN==msg->message || WM_RBUTTONDOWN==msg->message || WM_MBUTTONDOWN==msg->message)
	{	DeselectAllWithout(-1);
}	}

INT_PTR CALLBACK QuickSrchDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		//Adjust to center:
		RECT rc; GetWindowRect(hDlg, &rc);
		RECT rcPrnt;GetClientRect(((Panel*)lParam)->GetHWND(), &rcPrnt);
		int width,height;

		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		rcPrnt.top = rcPrnt.bottom+25;
		ClientToScreen(((Panel*)lParam)->GetHWND(),(POINT*)&rcPrnt);
		MoveWindow(hDlg, rcPrnt.left+5, rcPrnt.top, width, height, TRUE);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}

VOID Panel::EditQuickSearchMsgQueue(MSG *msg)
{
	if(!hQuickSearchEdit)//This means, where no in edit item mode!!!
		return;
	if(GetDlgItem(hQuickSearchEdit,IDC_EDIT_SEARCH)!=msg->hwnd)
		return;
	if(WM_KEYDOWN==msg->message)//102h
	{	//::SendMessage(hQuickSearchEdit,WM_CHAR,msg->wParam,msg->lParam);
		if(13==msg->wParam || 27==msg->wParam)//Enter || Esc;
		{	CancelQuickSearchMode(27==msg->wParam);
			//if(13==msg->wParam)
			//	SintezKey(30);
			return;
		}
		//else:
		BYTE kb[256];WORD ws;
		GetKeyboardState(kb);
		if(0==ToAscii((UINT)msg->wParam,(msg->lParam & 0xff0000)>>16,kb,&ws,0))//UINT cp=GetKBCodePage();//keyboard code page;
			return;
		wchar_t s[128];int ln=GetDlgItemText(hQuickSearchEdit,IDC_EDIT_SEARCH,s,128);
		if(msg->wParam==8)//backspace
			s[--ln]=0;
		else if(!ln)
		{	s[0]=(wchar_t)ws;//msg->wParam;
			if(s[ln]>=0x60 && s[ln]<0x69)
			{	s[ln++] = '0' + s[ln] - 0x60;
				s[1]=0;
			}
			else
				s[++ln] = 0;
		}
		else
		{	s[ln++]=(wchar_t)ws;//msg->wParam;
			s[ln]=0;
		}
		int id;BOOL bFind = FALSE;
		if(ln)
		{	for(id=1; id<totItems; id++)
			{	if(MyStrCmpNotRelUpRegNW(pItems[id].Name,s,ln))
				{	bFind = TRUE;
					break;
			}	}
			if(bFind)
				SetHot(id,TRUE);
		}
		else
			SetHot(EditItemId);
}	}

VOID Panel::EnableInput(BOOL b)
{
	bInput=(b==TRUE?1:0);
	EnableWindow(sheetTab[iThis],b);
}

BOOL Panel::EnterDiskCBSelectMode()
{
const int IDM_CBDISK = 1003;
	hDiskCB = CreateWindowEx(WS_EX_CLIENTEDGE,
							L"COMBOBOX",
                            NULL,
                            CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
							rcClient.left,//::rcClient.left+XBITMAP/4,
							-5,//::rcClient.bottom-6*XBITMAP,
							100,
							180,
							hWnd,
                            (HMENU)IDM_CBDISK,
                            hInst,
                            NULL);
	if(!hDiskCB)return FALSE;
	int crntDive=0;
	for(int i=0; i<myWMI::GetTotalLogicalDrives(); i++)
	{	wchar_t s[MAX_PATH];
		int l=MyStringCpy(s,MAX_PATH-1,myWMI::GetLogicalDriveName(i));
		s[l++]=' ';//MyStringCat(s,MAX_PATH-1,"     ");
		s[l++]=' ';s[l++]=' ';s[l++]=' ';s[l++]=' ';s[l]=0;
		wchar_t* pl=myWMI::GetLogicalDriveLabel(i); if((!pl) || pl[0]==0)
		{	switch(myWMI::GetLogicalDriveMediaType(i))
			{	case 11: MyStringCat(s,MAX_PATH-1,L"FDD");break;
				case 12: MyStringCat(s,MAX_PATH-1,L"HDD");break;
				case 10: MyStringCat(s,MAX_PATH-1,L"FDD");break;
				case 2 : 
				default: MyStringCat(s,MAX_PATH-1,L"USB");break;
				case 4:  MyStringCat(s,MAX_PATH-1,L"NET");break;
				case 5:  MyStringCat(s,MAX_PATH-1,L"CD") ;break;
				case 6:  MyStringCat(s,MAX_PATH-1,L"RAM DISK");break;

		}	} else MyStringCat(s,MAX_PATH-1,pl);
		::SendMessage(hDiskCB,CB_INSERTSTRING,(WPARAM)-1,(LPARAM)s);
		if(s[0]==path[0])crntDive=i;
	}
	::SendMessage(hDiskCB,CB_SETCURSEL,(WPARAM)crntDive,0);
	::SendMessage(hDiskCB,CB_SHOWDROPDOWN,TRUE,0);
	::SetFocus(hDiskCB);
	return TRUE;
}

VOID Panel::EnterDragFiles(int ID)
{
DWORD		 dwEffect;
DWORD		 dwResult;
DWORD		 dwSz = sizeof(DROPFILES)+2;
IDataObject  *pDropDataObject;
IDropSource  *pDropSource;
FORMATETC fmtetc = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
STGMEDIUM stgmed = { TYMED_HGLOBAL, { 0 }, 0};

	if(totSelects<1 && (iHot<1 || iHot>totItems-1))
	{	if(!(ID>-1 && ID<totItems))
			return;
	}

	bDrag = 1;

	//Avval qancha joy ajratish kerakligini topamiz:
	if(totSelects>1)
	{	for(int i=0; i<totItems; i++)
		{	if(selected==pItems[i].state || iHot==i)
				dwSz += 2*MyStringLength(GetFullPathAndName(i),MAX_PATH)+2;
	}	}
	else
	{	if(iHot>0)
			dwSz += 2*MyStringLength(GetFullPathAndName(iHot),MAX_PATH)+2;
	}

	//8-byte boundary:
	stgmed.hGlobal = GlobalAlloc(GHND,dwSz);
	DROPFILES *p = (DROPFILES*)GlobalLock(stgmed.hGlobal);
	p->pFiles = sizeof(DROPFILES);
	wchar_t *pp = (wchar_t*)(((char*)p) + p->pFiles);//wsprintf((LPWSTR)pp,L"D:\\Temp\\gizmo120.bmp\0\0");
	if(totSelects>1)
	{	for(int i=0; i<totItems; i++)
		{	if(selected==pItems[i].state || iHot==i)
			{	pp += 1+MyStringCpy(pp,MAX_PATH,GetFullPathAndName(i));
	}	}	}
	else
	{	if(iHot>0)
			pp += 1+MyStringCpy(pp,MAX_PATH,GetFullPathAndName(iHot));
	}
	*pp++=0;//O'zi ZeroInit
	
	p->pt.x = p->pt.y = 0;
	p->fNC = TRUE;
	p->fWide = TRUE;
	GlobalUnlock(stgmed.hGlobal);

	// Create IDataObject and IDropSource COM objects
	CreateDropSource(&pDropSource,iThis);
	CreateDataObject(&fmtetc, &stgmed, 1, &pDropDataObject);

	//
	//	** ** ** The drag-drop operation starts here! ** ** **
	//
	dwResult = DoDragDrop(	pDropDataObject,
							pDropSource,
							DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_LINK,
							&dwEffect);
	// success!
	if(dwResult == DRAGDROP_S_DROP)
	{	if(dwEffect & DROPEFFECT_MOVE)
		{
			// remove selection from edit control
	}	}
	// cancelled
	else if(dwResult == DRAGDROP_S_CANCEL)
	{
	}

	pDropDataObject->Release();//1-martaga release b-n tugamaydur;2-marta mousni qo'yib yuborganda, o'zida b-sa,
	pDropSource->Release();//yoki boshqa joyda release qilganda sodir bo'ladur;
	bDrag = 0;//CDropSource -> Release da FALSE qilib ketadi;
	//OutputDebugString(L"\n On drag...");
}

BOOL Panel::EnterEditItemMode()
{
const int IDM_EDITITEM = 1000;
	if(hEditItem) return FALSE;
	if(iHot<1) return FALSE;
	if(iHot>totItems-1) return FALSE;
	EditItemId = iHot;
	if(EditItemId<1) return FALSE;
	if(EditItemId>totItems-1) return FALSE;
	iEditItem = &pItems[EditItemId];

RECT *rc =  iEditItem->GetRectPtr(this,FALSE);
	hEditItem = CreateWindowEx(	WS_EX_TOPMOST|WS_EX_CLIENTEDGE,
								L"EDIT",
								NULL,
                                WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL,
								rc->left+XBITMAP,
								rc->top-3,
								itemMaxWidth-XBITMAP,//rc.right-rc.left-XBITMAP,//itemMaxWidth
								rc->bottom-rc->top+6,
                                hWnd,
                                (HMENU)IDM_EDITITEM,
                                hInst,
                                NULL);
	if(!hEditItem)return FALSE;
	//::SendMessage(hEditItem,  EM_SETEDITSTYLE, SES_EXTENDBACKCOLOR, TRUE);
	//::SendMessage(hEditItem, EM_SETBKGNDCOLOR, 0, (LPARAM)RGB(250,80,180));
	::SendMessage(hEditItem, WM_SETTEXT, 0, (LPARAM)iEditItem->Name);
	::SendMessage(hEditItem, EM_SETSEL, 0, -1);
	::SetFocus(hEditItem);
	
	//1 ikonini chizib olamiz:
	HDC dc = ::GetDC(hWnd);
	iEditItem->Render(dc,this,TRUE);
	::ReleaseDC(hWnd, dc);
 	return TRUE;
}

BOOL Panel::EnterQuickSearchMode(wchar_t firstKey)
{
const int IDM_EDITQUICKSEARCH = 1001;
	if(hQuickSearchEdit) return FALSE;
	EditItemId = iHot;//As save old selected item;
	/*hQuickSearchEdit = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,//|WS_EX_TOPMOST,
								"EDIT",
								NULL,
                                WS_VISIBLE|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|WS_CHILD,
								2,
								rcClient.bottom-25,
								100,
								25,
								hWnd,
                                (HMENU)IDM_EDITQUICKSEARCH,
                                hInst,
                                NULL);*/
	hQuickSearchEdit = CreateDialogParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_QWICK_SEARCH),hWnd,QuickSrchDlgProc,(LPARAM)this);
    ShowWindow(hQuickSearchEdit,SW_SHOW);

	if(!hQuickSearchEdit)return FALSE;
	SetWindowLongPtr(hQuickSearchEdit,GWLP_USERDATA,(LONG)iHot);
	::SetFocus(hQuickSearchEdit);
	::PostMessage(GetDlgItem(hQuickSearchEdit,IDC_EDIT_SEARCH),WM_KEYDOWN,firstKey,0);
 	return TRUE;
}

BOOL Panel::ExecuteHotKey(int k)
{
int selTo;
	//License::ShowBetaInfo();
	switch(k)
	{	case 30://Enter
			if(0==iHot)
			{	fManager.FolderUp(iHot);
				return TRUE;
			}
			else if(iHot>0)
			{	if(!fManager.FolderIn(iHot)){}
				return TRUE;
			}
			break;
		case 23://Up
			if(iHot > -1)
			{	SetHot(iHot-1);
				if(iHot>0)viewFileAssoc::QuickView(GetFullPathAndName(iHot));
			}
			if(iHot==-1)
			{	::SetFocus(pathAndNameEdt[0]);
				::SendMessage(pathAndNameEdt[1],EM_SETSEL,0,-1);
			}
			return TRUE;
		case 37://Select,Up
			if(iHot > -1)
			{	SetHot(iHot-1);
				if(normal==pItems[iHot+1].state)
					AddToSelection(iHot+1);
				else
					DeleteFromSelection(iHot+1);
			}
			if(iHot==-1)
			{	::SetFocus(pathAndNameEdt[0]);
				::SendMessage(pathAndNameEdt[1],EM_SETSEL,0,-1);
			}
			return TRUE;
		case 24://Down:
			if(iHot < totItems)
			{	SetHot(iHot+1);
				if(iHot>0)viewFileAssoc::QuickView(GetFullPathAndName(iHot));
			}
			if(iHot == totItems)
				::SetFocus(cmndsCB);
			return TRUE;
		case 38://Select,Down:
			if(iHot < totItems)//oxirgisiyam kirsun;
			{	SetHot(iHot+1);
				if(normal==pItems[iHot-1].state)
					AddToSelection(iHot-1);
				else
					DeleteFromSelection(iHot-1);
			}
			if(iHot == totItems)
				::SetFocus(cmndsCB);
			return TRUE;
		case 19://PageUp
		case 27://PageUp,2-key
			selTo = iHot-nRows;
			if(selTo<0)
				selTo=0;
			SetHot(selTo);
			return TRUE;
		case 31://Select PageUp
		case 33://Select PageUp,2-key
			selTo = iHot-nRows;
			if(selTo<0)
				selTo=0;
			for(int i=iHot; i>=selTo; i--)
			{	if(i!=selTo && normal==pItems[i].state)
					AddToSelection(i);
				else
					DeleteFromSelection(i);
			}
			SetHot(selTo);
			return TRUE;
		case 20://PageDwn
		case 28://PageDwn,2-key
			selTo = iHot+nRows;
			if(selTo>totItems-1)
				selTo=totItems-1;
			SetHot(selTo);
			return TRUE;
		case 32://Select PageDwn
		case 34://Select PageDwn,2-key
			selTo = iHot+nRows;
			if(selTo>totItems-1)
				selTo=totItems-1;
			for(int i=iHot; i<=selTo; i++)
			{	if(i!=selTo && normal==pItems[i].state)
					AddToSelection(i);
				else
					DeleteFromSelection(i);
			}
			SetHot(selTo);
			return TRUE;
		case 21://End
			selTo = totItems-1;
			if(selected==pItems[iHot].state)
				DeleteFromSelection(iHot);
			else// sel-n 1 ta kamayyapti, lekin pSel ga ta'sir yo'q, kich.xatolik!!!!
				AddToSelection(iHot);
			SetHot(selTo);
			return TRUE;
		case 35://Select to end
			selTo = totItems-1;
			for(int i=iHot; i<=selTo; i++)
			{	if(i!=selTo && normal==pItems[i].state)
					AddToSelection(i);
				else
					DeleteFromSelection(i);
			}
			SetHot(selTo);
			return TRUE;
		case 22://Home
			selTo = 0;
			if(selected==pItems[iHot].state)
				DeleteFromSelection(iHot);
			SetHot(selTo);
			return TRUE;
		case 36://Select to home
			selTo = 0;
			for(int i=iHot; i>=selTo; i--)
			{	if(i!=selTo && normal==pItems[i].state)
					AddToSelection(i);
				else
					DeleteFromSelection(i);
			}
			SetHot(selTo);
			return TRUE;
		case 25://Esc
			DeselectAllWithout(-1);
			::SetFocus(pathAndNameEdt[0]);
			::SendMessage(pathAndNameEdt[1],EM_SETSEL,0,-1);
			return TRUE;
		case 0://Ctrl + A
			for(int i=0;i<totItems;i++)
				AddToSelection(i);
			return TRUE;
		case 1://Ctrl + T
			AddNewSheetTab();
			return TRUE;
		case 2://Ctrl + W
			DeleteCrntSheetTab();
			return TRUE;
		case 3://F1
			wchar_t mnuStr[64];wchar_t *p,sinoExe[MAX_PATH];GetModuleFileName(NULL,sinoExe,MAX_PATH);
			p=wcsrchr(sinoExe,'\\');if(p)*p=0;
			if(GetEnvironmentVariableW(L"languge",mnuStr,64))//Language instartup qo'yadur;
			{	if(!wcscmp(mnuStr,L"russian"))
#ifdef _WIN64
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64R.exe"),L"0",sinoExe);
				else if(!wcscmp(mnuStr,L"uzbekl"))
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64L.exe"),L"0",sinoExe);
				else if(!wcscmp(mnuStr,L"uzbekk"))
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64K.exe"),L"0",sinoExe);
				else//if(wcscmp(mnuStr,L"Exit")
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"0",sinoExe);
			}	
			else Execution::exePE(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"0");
#else
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32R.exe"),L"0",sinoExe);
				else if(!wcscmp(mnuStr,L"uzbekl"))
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32L.exe"),L"0",sinoExe);
				else if(!wcscmp(mnuStr,L"uzbekk"))
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32K.exe"),L"0",sinoExe);
				else//if(wcscmp(mnuStr,L"Exit")
					Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"0",sinoExe);
			}	
			else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"0",sinoExe);
#endif
			return TRUE;
		case 4://Ctrl + F1
			if(0==iThis)
				EnterDiskCBSelectMode();
			else
			{	int gh = iHot;
				::SendMessage(panel[0].hWnd, WM_LBUTTONDOWN, -1, -1);
				::SetFocus(panel[0].hWnd);
				panel[0].EnterDiskCBSelectMode();
			}
			return TRUE;
		case 5://Ctrl + F2
			if(1==iThis)
				EnterDiskCBSelectMode();
			else if(conf::Dlg.iTotPanels>1)
			{	int gh = iHot;
				::SendMessage(panel[1].hWnd, WM_LBUTTONDOWN, -1, -1);
				::SetFocus(panel[1].hWnd);
				panel[1].EnterDiskCBSelectMode();
			}
			return TRUE;
		case 6://Ctrl + F3
			if(this==&panel[2])
				EnterDiskCBSelectMode();
			else if(conf::Dlg.iTotPanels>2)
			{	int gh = iHot;
				::SendMessage(panel[2].hWnd, WM_LBUTTONDOWN, -1, -1);
				::SetFocus(panel[2].hWnd);
				panel[2].EnterDiskCBSelectMode();
			}
			return TRUE;
		case 7://Ctrl + F4
			if(this==&panel[3])
				EnterDiskCBSelectMode();
			else if(conf::Dlg.iTotPanels>3)
			{	int gh = iHot;
				::SendMessage(panel[3].hWnd, WM_LBUTTONDOWN, -1, -1);
				::SetFocus(panel[3].hWnd);
				panel[3].EnterDiskCBSelectMode();
			}
			return TRUE;
		case 8://F2
			EnterEditItemMode();
			return TRUE;
		case 9://F3 View
			if(archElem==entry.GetCrntRecType())
			{	CTempDir tmp(conf::tmpPath);
				tmp.UnpackSelected(this);
				wchar_t *pfName=tmp.GetItemPathAndName();
				viewFileAssoc::Execute(hWnd,pfName,0);
				CTempDir::AddToTempItemsLists(pfName);
			}
			else if(socketCl!=entry.GetCrntRecType())
			if(iHot>0)
				viewFileAssoc::Execute(hWnd,GetFullPathAndName(iHot),0);
			return TRUE;
		case 10://F4 Edit
			if(archElem==entry.GetCrntRecType())
			{	CTempDir tmp(conf::tmpPath);
				tmp.UnpackSelected(this);
				wchar_t *pfName=tmp.GetItemPathAndName();
				viewFileAssoc::Execute(hWnd,pfName,1);
				CTempDir::AddToTempItemsLists(pfName);
			}
			else if(socketCl!=entry.GetCrntRecType())
			if(iHot>0)
				viewFileAssoc::Execute(hWnd,GetFullPathAndName(iHot),1);
			return TRUE;
		case 26://Exit Alt+F4
			::PostMessage(::hWnd,WM_CLOSE,0,0);//PostQuitMessage(0);
			return TRUE;
		case 11://F5 Copy
			License::ShowBetaInfo();
			fCopyOper::ShowCopyDlg(this);
			return TRUE;
		case 44://Alt+F5
			License::ShowBetaInfo();
			archive::ShowDlg(this);
			return TRUE;
		case 45://Cntrl+PgDwn
			License::ShowBetaInfo();
			archive::ShowUnpckDlg(this);
			return TRUE;
		case 12://F6 Renmove
			License::ShowBetaInfo();
			fRenMove::ShowCopyDlg(this);
			return TRUE;
		case 13://Alt+F7 Search
			License::ShowBetaInfo();
			if(archElem==entry.GetCrntRecType())
				archive::ShowSearchDlg$12(this);
			else if(virtualPanel==entry.GetCrntRecType() &&
						vrtlPanels::plgns[panel[Panel::iActivePanel].iVPPlg].ShowSearchDlg$20)
				vrtlPanels::ShowSearchDlg$20(&panel[Panel::iActivePanel]);
			else if(socketCl!=entry.GetCrntRecType())
				fSearchViaF7::ShowSearchDlg$8(this);//false,if not guid path
			return TRUE;
		case 14://F7 CreateFolder
			License::ShowBetaInfo();
			CreaFolder::ShowDlg(this);
			return TRUE;
		case 15://Backspace
			fManager.FolderUp(0);
			return TRUE;
		case 43://Cntrl + Enter
			if(iHot<totItems)//0 ham
				CmnCntrl::InsertToCmndLine(this,iHot);
			return TRUE;
		case 29://Properties
			License::ShowBetaInfo();
			if(socketCl!=entry.GetCrntRecType())
			{	if(iHot>=0)
				if(iHot<totItems)
				{	wchar_t ws[MAX_PATH];
					GetFullPathAndName(iHot,ws,MAX_PATH);
					SHObjectProperties(hWnd,SHOP_FILEPATH,ws,NULL);
					//SetShellHook(TRUE);
					//WaitForShellHookDestroyMessage(HSHELL_WINDOWDESTROYED,0,0);
					//SetShellHook(FALSE);
			}	}
			return TRUE;
		case 18://Insert
			if(iHot<totItems && iHot>0)
			{	if(selected==pItems[iHot].state)
					DeleteFromSelection(iHot);
				else
					AddToSelection(iHot);
			}
			if(iHot<totItems-1)
				SetHot(iHot+1);
			return TRUE;
		case 17://Space
			if(iHot>0)
			if(iHot<totItems)
			{	if(selected==pItems[iHot].state)
					DeleteFromSelection(iHot);
				else
					AddToSelection(iHot);
				RenderItem(iHot);
			}
			return TRUE;
		case 39://Delete
			License::ShowBetaInfo();
			if(fDelOper::Delete(this,0))
				DeselectAllWithout(-1);
			return TRUE;
		case 16://Fast select, +
			fSelViaPlus.Select(this);
			return TRUE;
		case 41://Shift Delete
		case 42://Delete,2-key
			if(fDelOper::Delete(this,1))
				DeselectAllWithout(-1);
			return TRUE;
		case 46://View with...(from plugins)
			if(iHot>0)
				viewFileAssoc::ViewWithPlugins(hWnd,GetFullPathAndName(iHot));
			return TRUE;
		case 47://View with...(open)
			if(iHot>0)
				viewFileAssoc::ViewWithOpen(hWnd,GetFullPathAndName(iHot));
			return TRUE;
		case 48://View with...(browse)
			if(iHot>0)
				viewFileAssoc::ViewWithBrowse(hWnd,GetFullPathAndName(iHot));
			return TRUE;
		case 49://Edit with...(from plugins)
			if(iHot>0)
				viewFileAssoc::EditWithPlugins(hWnd,GetFullPathAndName(iHot));
			return TRUE;
		case 50://Edit with...(open)
			if(iHot>0)
				viewFileAssoc::ViewWithOpen(hWnd,GetFullPathAndName(iHot));
			return TRUE;
		case 51://Edit with...(browse)
			if(iHot>0)
				viewFileAssoc::ViewWithBrowse(hWnd,GetFullPathAndName(iHot));
			return TRUE;
		case 52://Ctrl + C:
			ClpbrdCpy();
			return TRUE;
		case 53://Ctrl + X
			ClpbrdCut();
			return TRUE;
		case 54://Ctrl+V
			ClpbrdPst();
			return TRUE;
		case 55://Ctrl+Alt+C
			ClpbrdApndCpy();
			return TRUE;
		case 56://Ctrl+Alt+X
			ClpbrdApndCut();
			return TRUE;
		case 57://Ctrl+Q
			if(conf::Dlg.iTotPanels>1)
			{	if(1==bQView)
				{	bQView = 0;//Close render ni ishlatgani uchun;
					QView::Close();
				}else
				{	if(QView::Open(iThis,iOpponent))
						bQView = 1;
			}	}
			return TRUE;
	}
	return FALSE;
}

/*int Panel::FindItemA(char* txt, int cmpMethod)
{
wchar_t s[MAX_PATH];
	MultiByteToWideChar(AreFileApisANSI()?CP_ACP:CP_OEMCP,MB_PRECOMPOSED,txt,MAX_PATH-1,s,MAX_PATH);
	return FindItem(s,cmpMethod);
}*/

int Panel::FindItem(wchar_t* txt, int cmpMethod)
{
	if(txt[0])
	for(int i=1; i<totItems; i++)
	{	switch (cmpMethod)
		{	case 0:
				if(!wcscmp(pItems[i].Name,txt))
					return i;
				break;
			case 1:
				if(!_wcsicmp(pItems[i].Name,txt))
					return i;
				break;
			case 2:
				if(wcsstr(pItems[i].Name,txt))
					return i;
				break;
	}	}
	return -1;
}

int Panel::FindNearestItem(wchar_t* txt,int in)//in->taxminan raqami;
{
int ltxt = MyStringLength(txt,MAX_PATH);
int nrst=0,inrst=-1;
	if(txt[0])
	for(int i=1; i<totItems; i++)
	{	for(int k=0; k<ltxt; k++)
		{	if(0==pItems[i].Name[k] || pItems[i].Name[k]!=txt[k] || 0==txt[k+1])
			{	if(nrst<k)
				{	nrst = k;
					inrst = i;
					if(inrst==in) return in;
					break;
				}
				//else if(in==i && k>0)
				//	return in;
	}	}	}
	return inrst;
}

VOID Panel::FreeMem()
{
	SetChangeNotifyPath(0);//stop waiting
	for(int i=0; i<totItems; i++)
		pItems[i].FreeMem();
	if(totItems > HEAP_INC_ITEM_NUMBER)
		DecHeap(HEAP_INC_ITEM_NUMBER);
	totItems = idForTooltip = nColumns = 0; nRows = itemMaxWidth = iHot = 0;
	fAssociation.FreeMem();
}

/*int Panel::GetFullPathAndNameA(int id, char *s, int lnBuf)
{
char c[MAX_PATH];
	WideCharToMultiByte(AreFileApisANSI()?CP_ACP:CP_OEMCP,WC_COMPOSITECHECK,path,-1,
	    			    c,MAX_WMI_CHAR,NULL,NULL);
	int l = MyStringCpyA(s,lnBuf,c);
	if('*'==s[l-1])//if(MyStringRemoveLastCharCheckPre(s,MAX_PATH-1,'*','\\'))
	if('\\'==s[l-2])
		s[--l]=0;//	--l;
	if(id>0)
	{	WideCharToMultiByte(AreFileApisANSI()?CP_ACP:CP_OEMCP,WC_COMPOSITECHECK,pItems[id].Name,-1,
							c,MAX_WMI_CHAR,NULL,NULL);
		l+=MyStringCpyA(&s[l],lnBuf-l,c);//MyStringCat(s,lnBuf,pItems[id].Name);
	}
	else//oxirgi '\' i ochiramiz:
		if('\\'==s[l-1]) s[--l]=0;
	return l;
}*/

int Panel::GetFullPathAndName(int id, wchar_t *s, int lnBuf)
{
int l = MyStringCpy(s,lnBuf,path);
	if('*'==s[l-1] && '\\'==s[l-2])//MyStringRemoveLastCharCheckPre(s,MAX_PATH-1,'*','\\');
		s[--l]=0;
	else if('\\'!=s[l-1])
		s[l++]='\\';
	if(id>0)
		l+=MyStringCpy(&s[l],lnBuf-l,pItems[id].Name);//MyStringCat(s,lnBuf,pItems[id].Name);
	else//oxirgi '\' i ochiramiz:
		if('\\'==s[l-1]) s[--l]=0;
	return l;
}

static wchar_t s[MAX_PATH];
wchar_t* Panel::GetFullPathAndName(int id)
{
	if(id<1) return path;
	if(id>totItems) return path;
//static char s[MAX_PATH];
	int l=MyStringCpy(s,MAX_PATH-1,path);
	if('*'==s[l-1] && '\\'==s[l-2])//MyStringRemoveLastCharCheckPre(s,MAX_PATH-1,'*','\\');
		s[--l]=0;
	else if('\\'!=s[l-1])
		s[l++]='\\';
	if(id>0)
		MyStringCpy(&s[l],MAX_PATH-l,pItems[id].Name);//MyStringCat(s,MAX_PATH-1,pItems[id].Name);
	return s;
}

wchar_t* Panel::GetArcItPathAndName(int id)
{
//static char s[MAX_PATH];
	int l=MyStringCpy(s,MAX_PATH-1,entry.GetCrntRecArchPath());
	if('*'==s[l-1] && ('\\'==s[l-2] || '/'==s[l-2]))//MyStringRemoveLastCharCheckPre(s,MAX_PATH-1,'*','\\');
		s[--l]=0;
	else if('\\'==s[l-2] || '/'==s[l-2])
		s[l++]='\\';
	if(id>0 && id<totItems)
		MyStringCpy(&s[l],MAX_PATH-l,pItems[id].Name);//MyStringCat(s,MAX_PATH-1,pItems[id].Name);
	return s;
}

wchar_t* Panel::GetArcItPathAndName(wchar_t *itName)
{
//static char s[MAX_PATH];
	int l=MyStringCpy(s,MAX_PATH-1,entry.GetCrntRecArchPath());
	if('*'==s[l-1] && ('\\'==s[l-2] || '/'==s[l-2]))//MyStringRemoveLastCharCheckPre(s,MAX_PATH-1,'*','\\');
		s[--l]=0;
	else if('\\'==s[l-2] || '/'==s[l-2])
		s[l++]='\\';
	MyStringCpy(&s[l],MAX_PATH-l,itName);
	return s;
}

wchar_t* Panel::GetArcItFullPathAndName(int id)
{
//static char s[MAX_PATH];
	int l=MyStringCpy(s,MAX_PATH-1,arch.name);//MyStringCat(s,MAX_PATH-1,pItems[id].Name);
	s[l++]='\\';
	l+=MyStringCpy(&s[l],MAX_PATH-l,entry.GetCrntRecArchPath());
	if('*'==s[l-1] || ('\\'==s[l-2] || '/'==s[l-2]))//MyStringRemoveLastCharCheckPre(s,MAX_PATH-1,'*','\\');
		s[--l]=0;
	else if('\\'!=s[l-1] || '/'!=s[l-1])
		s[l++]='\\';
	if(id>0 && id<totItems)
		MyStringCpy(&s[l],MAX_PATH-l,pItems[id].Name);//MyStringCat(s,MAX_PATH-1,pItems[id].Name);
	return s;
}

wchar_t* Panel::GetArcItFullPathAndName(wchar_t *itName)
{
//static char s[MAX_PATH];
	int l=MyStringCpy(s,MAX_PATH-1,arch.name);//MyStringCat(s,MAX_PATH-1,pItems[id].Name);
	s[l++]='\\';
	l=MyStringCpy(&s[l],MAX_PATH-l,entry.GetCrntRecArchPath());
	if('*'==s[l-1] && ('\\'==s[l-2] || '/'==s[l-2]))//MyStringRemoveLastCharCheckPre(s,MAX_PATH-1,'*','\\');
		s[--l]=0;
	else if('\\'!=s[l-1] || '/'!=s[l-1])
		s[l++]='\\';
	MyStringCpy(&s[l],MAX_PATH-l,itName);
	return s;
}

int Panel::GetItemNum(int x, int y)
{
//profilerOutStr("Beg of Panel::GetItem ")
if(0==bItemsViewType)
{int col = ((itemMaxWidth>0)?((x+1-ScrollOffst) / itemMaxWidth):0);
 int row = (itemHeight > 0) ? (y / itemHeight):0;
 if(0==col)
 {	 //profilerPrintf1("\nGetItem ret=%d",row)
	 return row<totItems?row:(-1);
 }
 int r = col * nRows + row;
 //profilerPrintf1("\nGetItem ret=%d",r)
 return r<totItems?r:(-1);
}
//else
 int row = ((itemHeight>0)?((y+1-ScrollOffst) / itemHeight):0);
 return row<totItems?row:(-1);
}

int Panel::GetPanelFromPoint(POINT *pt)//In screen coords, soon;
{
POINT rc[3];	int r;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			r = 0;
		break;
		case 2: default:
			rc[0].x = panel[0].rcClient.right;
			rc[0].y = panel[0].rcClient.top;
			ClientToScreen(panel[0].hWnd,&rc[0]);
			if(pt->x<rc[0].x)
				r=0;
			else
				r=1;
		break;
		case 3:
			rc[0].x = panel[0].rcClient.right;
			rc[0].y = panel[0].rcClient.top;
			ClientToScreen(panel[0].hWnd,&rc[0]);
			rc[1].x = panel[1].rcClient.right;
			rc[1].y = panel[1].rcClient.top;
			ClientToScreen(panel[1].hWnd,&rc[1]);
			if(pt->x<rc[0].x)
				r=0;
			else if(pt->x<rc[1].x)
				r=1;
			else
				r=2;
		break;
		case 4:
			rc[0].x = panel[0].rcClient.right;
			rc[0].y = panel[0].rcClient.top;
			ClientToScreen(panel[0].hWnd,&rc[0]);
			rc[1].x = panel[1].rcClient.right;
			rc[1].y = panel[1].rcClient.top;
			ClientToScreen(panel[1].hWnd,&rc[1]);
			rc[2].x = panel[2].rcClient.right;
			rc[2].y = panel[2].rcClient.top;
			ClientToScreen(panel[2].hWnd,&rc[2]);
			if(pt->x<rc[0].x)
				r=0;
			else if(pt->x<rc[1].x)
				r=1;
			else if(pt->x<rc[2].x)
				r=2;
			else
				r=3;
		break;
	}
	return r;
}

/*char *Panel::GetPathA()
{
static char s[MAX_PATH];
	WideCharToMultiByte(AreFileApisANSI()?CP_ACP:CP_OEMCP,WC_COMPOSITECHECK,path,-1,s,MAX_PATH-1,NULL,NULL);
	return s;
}*/

int Panel::GetSelectedItemNum(int numSel)
{	
int iFirstSelNum=0;
	for(int i=0; i<totItems; i++)
	{	if(selected==pItems[i].state)
		{	if(iFirstSelNum++==numSel)
				return i;
	}	}
	return 0;
}

wchar_t* Panel::GetStrForTooltip(int id,int* ln)
{
static wchar_t st[260];
DWORD sz=0,visz;
int l=0;
st[0]=0;

if(virtualPanel!=entry.GetCrntRecType())
{	sz = GetFileVersionInfoSize(GetFullPathAndName(id), &visz);
	if(sz)
	{	wchar_t *vi, SubBlock[128]; UINT uiVerSize; DWORD *infoptr;
		struct LANGANDCODEPAGE 
		{	WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;

		vi = (wchar_t*)_malloca(2*sz); vi[0] = 0;//Win7 uchun 2* qo'ydim;
		if(GetFileVersionInfo(GetFullPathAndName(id), 0, sz, vi))
		{	if(VerQueryValue(vi, L"\\VarFileInfo\\Translation",
						(LPVOID*)&lpTranslate, &uiVerSize))
			if(uiVerSize)
			{	StringCchPrintf(SubBlock,128, //Microsoft Direct3D bo'lishi kerak;
					 L"\\StringFileInfo\\%04x%04x\\FileDescription",
					 lpTranslate->wLanguage, lpTranslate->wCodePage);
				if(VerQueryValue( vi, SubBlock, (VOID**)&infoptr, &uiVerSize))
				{	l = MyStringCpy(st,MAX_PATH-1,(wchar_t*)infoptr);
					st[l++] = 0x0d; st[l++] = 0x0a;
				}
				StringCchPrintf(SubBlock,128, //"d3d9.dll"
					L"\\StringFileInfo\\%04x%04x\\ProductName",
					lpTranslate->wLanguage, lpTranslate->wCodePage);
			}
			if(VerQueryValue(vi, SubBlock, (VOID**)&infoptr, &uiVerSize))
			if(uiVerSize)
			{	l += MyStringCpy(&st[l],MAX_PATH-1,(wchar_t*)infoptr);
				st[l++] = 0x0d; st[l++] = 0x0a;
			
				StringCchPrintf(SubBlock,128, //"d3d9.dll"
					 L"\\StringFileInfo\\%04x%04x\\LegalCopyright",
					lpTranslate->wLanguage, lpTranslate->wCodePage);
			}
			if(VerQueryValue(vi, SubBlock, (VOID**)&infoptr, &uiVerSize))
				l += MyStringCpy(&st[l],MAX_PATH-1,(wchar_t*)infoptr);
		}
		_freea(vi);
}	}
if(0==sz)
	l = MyStringCpy(st,260,pItems[id].Name);

static wchar_t sWr[32]={0};
static wchar_t sSz[32]={0};
static wchar_t sInSDsk[32]={0};

	if(0==sSz[0]) LoadString(hInst,IDS_STRINGSW_34,sSz,32);
	if(0==sWr[0]) LoadString(hInst,IDS_STRINGSW_35,sWr,32);
	if(0==sInSDsk[0]) LoadString(hInst,IDS_STRINGSW_217,sInSDsk,32);

	if(file==pItems[id].attribute)
	{	st[l++] = 0x0d; st[l++] = 0x0a;
		l += MyStringCpy(&st[l],260-l,sSz);
		l += MyStringPrintDecSpace(&st[l],pItems[id].size);
		st[l++] = 0x0d; st[l++] = 0x0a;
		l += MyStringCpy(&st[l],260-l,sInSDsk);
		l += MyStringPrintDecSpace(&st[l],pItems[id].allSize);
		st[l++] = 0x0d; st[l++] = 0x0a;

		SYSTEMTIME stUTC,stLocal;
		FileTimeToSystemTime(&pItems[id].ftWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
		if(S_OK!=StringCchPrintf(&st[l], 32,
								L"%s %02d.%02d.%d  %02d:%02d",
								sWr,
								stLocal.wMonth, stLocal.wDay, stLocal.wYear,
								stLocal.wHour, stLocal.wMinute))
			st[l-1] = 0;
	}
	else if(folder==pItems[id].attribute)
	{	st[l++] = 0x0d; st[l++] = 0x0a;
		l += MyStringCpy(&st[l],260-l,sSz);
		l += MyStringPrintDecSpace(&st[l],pItems[id].size);
	}
	if(ln) *ln = MyStringLength(st,260);//oxirgi 0 uchun +1
	return st;//hozircha;
}

BOOL Panel::IncHeap()
{
//profilerOutStr("Beg of Panel::IncHeap ")

	pItems = (PanelItem*)realloc(pItems,(iMaxItems+HEAP_INC_ITEM_NUMBER)*
									sizeof(PanelItem));
	if(!pItems)
		return FALSE;
	iMaxItems += HEAP_INC_ITEM_NUMBER;
	for(int it=0; it<iMaxItems; it++)
		pItems[it].PanelItem::PanelItem();

//profilerPrintf1("\nIncHeap iMaxItems=%d",iMaxItems)
	return TRUE;
}

int Panel::IsPointInsidePanelRect(POINT *pt)//In screen coords, soon;
{
	HWND h = WindowFromPoint(*pt);
	if(!h)return -1;
	if(h==panel[0].GetHWND())return 0;
	if(h==panel[1].GetHWND())return 1;
	if(h==panel[2].GetHWND())return 2;
	if(h==panel[3].GetHWND())return 3;
	return -1;
/*POINT rc; int r=-1;
	rc.x = pt->x;
	rc.y = pt->y;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			ScreenToClient(panel[0].hWnd,&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<panel[0].rcClient.bottom)
				r = 0;
		break;
		case 2: default:
			ScreenToClient(panel[0].hWnd,&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<panel[0].rcClient.bottom)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[1].hWnd,&rc);
				if(rc.x>0)
				if(rc.x<panel[1].rcClient.right)
				if(rc.y>0)
				if(rc.y<panel[1].rcClient.bottom)
					r = 1;
			}
		break;
		case 3:
			ScreenToClient(panel[0].hWnd,&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<panel[0].rcClient.bottom)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[1].hWnd,&rc);
				if(rc.x>0)
				if(rc.x<panel[1].rcClient.right)
				if(rc.y>0)
				if(rc.y<panel[1].rcClient.bottom)
					r = 1;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[2].hWnd,&rc);
				if(rc.x>0)
				if(rc.x<panel[2].rcClient.right)
				if(rc.y>0)
				if(rc.y<panel[2].rcClient.bottom)
					r = 2;
			}
		break;
		case 4:
			ScreenToClient(panel[0].hWnd,&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<panel[0].rcClient.bottom)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[1].hWnd,&rc);
				if(rc.x>0)
				if(rc.x<panel[1].rcClient.right)
				if(rc.y>0)
				if(rc.y<panel[1].rcClient.bottom)
					r = 1;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[2].hWnd,&rc);
				if(rc.x>0)
				if(rc.x<panel[2].rcClient.right)
				if(rc.y>0)
				if(rc.y<panel[2].rcClient.bottom)
					r = 2;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[3].hWnd,&rc);
				if(rc.x>0)
				if(rc.x<panel[3].rcClient.right)
				if(rc.y>0)
				if(rc.y<panel[3].rcClient.bottom)
					r = 3;
			}
		break;
	}
	return r;*/
}

VOID Panel::Move(int x, int y, int w, int h)
{
//profilerOutStr("Beg of Panel::Move")
//profilerPrintf4("x = %d, y = %d, w = %d, h = %d", x, y, w, h)

	if(!hWnd) return;
	left = x; top = y; width = w; height = h;
	MoveWindow(hWnd,x,y,w,h,TRUE);
	AdjustScrollity();
	int id = iHot;
	if(id>-1)
	if(id<totItems)
		ScrollItemToView(id);
	ShowWindow(hWnd, SW_SHOW);

//profilerOutStr("End of Panel::Move")
}

VOID Panel::OnDirectoryChangeNotify()
{
	if(!hWnd)return;

	//socket, lekin client emas;
	if(linkSock::sockDlg[iThis] && socketCl!=entry.GetCrntRecType())
		linkSock::OnDirectoryChangeNotify(iThis,NULL);

//In future, check directory for founding changing elements.
int tp=3,id=-1;wchar_t s[MAX_PATH];
	if(&panel[iActivePanel]==this)
	{	if(findItemName[0]==0)
		{	id = iHot;//Qaytib shuni select qilmoq uchun;
			if(id>-1)
			if(id<totItems)
			{	MyStringCpy(s,MAX_PATH-1,pItems[id].Name);
				tp = pItems[id].state;
	}	}	}
	DeselectAllWithout(-1);
	FreeMem();

	if(archElem==entry.GetCrntRecType())
	{	//if(IsFileExist(GetArcFilePathAndName()))
			fManager.FillArchItems(entry.GetCrntRecArchPath());
		//else
		//	fManager.FillDirectFolder();
	}
	else
		fManager.FillDirectFolder();

	AdjustScrollity();
	if(findItemName[0])
	{	id = FindItem(findItemName);
		findItemName[0]=0;
	}
	else if(id>-1)
		id = FindNearestItem(s,id);
	else
		id = 0;
	if(&panel[iActivePanel]==this)
	{	if(id>-1)
		{	iHot=id;
			ScrollItemToView(id>-1?id:0);
	}	}
	ClrScr();
	Render(NULL);
	ShowSelectedFilesInfo();
}

//Agar obrabotka qilsa TRUE qaytaradi, aks holda FALSE;
BOOL Panel::OnKeybPressed(WPARAM wParam, LPARAM lParam)//WM_KEYDOWN obrabotkasi;
{
//profilerOutStr("Beg of Panel::OnKeybPressed")
//profilerPrintf2("\nwPar = %x, lPar = %x", wParam, lParam)

	//wchar_t s[260];static int i=0;
	//wsprintf(s,L"\n%d iActivePanel:%d Panel-%d OnKeybPressed",i++,iActivePanel,iThis);
	//OutputDebugString(s);

	if(0==bInput) return FALSE;
	if(1==panel[iAntiOpponent].bQView) return FALSE;

	toolTip.Hide();

int bShftPressed = 0; SHORT sh = GetKeyState(VK_LSHIFT);
	if(sh & 0x8000) bShftPressed = 1;
	else
	{	sh = GetKeyState(VK_RSHIFT);
		if(sh & 0x8000) bShftPressed = 1;
	}
	
	int bCtrlPressed = 0; sh = GetKeyState(VK_LCONTROL);
	if(sh & 0x8000) bCtrlPressed = 1;
	else
	{	sh = GetKeyState(VK_RCONTROL);
		if(sh & 0x8000) bCtrlPressed = 1;
	}
	
	int bAltPressed = 0; sh = GetKeyState(VK_MENU);
	if(sh & 0x8000) bAltPressed = 1;


	//wsprintf(s,L"\n%d OnKeybPressed, Cntrl: %s Alt: %s Code: %d",i++,bAltPressed?L"pressed":L"unpressed",
	//																 bCtrlPressed?L"pressed":L"unpressed",wParam);
	//OutputDebugString(s);

	conf::TKey ik((int)wParam,bCtrlPressed,bAltPressed,bShftPressed);
	int key = *((int*)&ik);
	__int32* pk=(int*)&conf::Dlg.keys[0];

	if(key==pk[30])//Enter
		ExecuteHotKey(30);
	else if(key==pk[23])//Up
		ExecuteHotKey(23);
	else if(key==pk[37])//Select,Up
		ExecuteHotKey(37);
	else if(key==pk[24])//Down:
		ExecuteHotKey(24);
	else if(key==pk[38])//Select,Down:
		ExecuteHotKey(38);
	else if(key==pk[19] ||//PageUp
			key==pk[27]  )//PageUp,2-key
		ExecuteHotKey(19);
	else if(key==pk[31] ||//Select PageUp
			key==pk[33]  )//Select PageUp,2-key
		ExecuteHotKey(31);
	else if(key==pk[20] ||//PageDwn
			key==pk[28]  )//PageDwn,2-key
		ExecuteHotKey(20);
	else if(key==pk[32] ||//Select PageDwn
			key==pk[34])//Select PageDwn,2-key
		ExecuteHotKey(32);
	else if(key==pk[21])//End
		ExecuteHotKey(21);
	else if(key==pk[35])//Select to end
		ExecuteHotKey(35);
	else if(key==pk[22])//Home
		ExecuteHotKey(22);
	else if(key==pk[36])//Select to home
		ExecuteHotKey(36);
	else if(key==pk[25])//Esc
		ExecuteHotKey(25);
	else if(key==pk[0])//Ctrl + A
		ExecuteHotKey(0);
	else if(key==pk[1])//Ctrl + T
		ExecuteHotKey(1);
	else if(key==pk[2])//Ctrl + W
		ExecuteHotKey(2);
	else if(key==pk[3])//F1
		ExecuteHotKey(3);
	else if(key==pk[4])//Ctrl + F1
		ExecuteHotKey(4);
	else if(key==pk[5])//Ctrl + F2
		ExecuteHotKey(5);
	else if(key==pk[6])//Ctrl + F3
		ExecuteHotKey(6);
	else if(key==pk[7])//Ctrl + F4
		ExecuteHotKey(7);
	else if(key==pk[8])//F2
		ExecuteHotKey(8);
	else if(key==pk[9])//F3 View
		ExecuteHotKey(9);
	else if(key==pk[10])//F4 Edit
		ExecuteHotKey(10);
	else if(key==pk[26])//Exit Alt+F4
		ExecuteHotKey(26);
	else if(key==pk[11])//F5 Copy
		ExecuteHotKey(11);
	else if(key==pk[44])//Alt+F5
		ExecuteHotKey(44);
	else if(key==pk[45])//Cntrl+PgDwn
		ExecuteHotKey(45);
	else if(key==pk[12])//F6 Renmove
		ExecuteHotKey(12);
	else if(key==pk[13])//Alt+F7 Search
		ExecuteHotKey(13);
	else if(key==pk[14])//F7 CreateFolder
		ExecuteHotKey(14);
	else if(key==pk[15])//Backspace
		ExecuteHotKey(15);
	else if(key==pk[43])//Cntrl + Enter
		ExecuteHotKey(43);
	else if(key==pk[29])//Properties
		ExecuteHotKey(29);
	else if(key==pk[18])//Insert
		ExecuteHotKey(18);
	else if(key==pk[17])//Space
		ExecuteHotKey(17);
	else if(key==pk[39] ||//Delete
			key==pk[40] )//Delete,2-key
		ExecuteHotKey(39);
	else if(key==pk[16])//Fast select, +
		ExecuteHotKey(16);
	else if(key==pk[41] ||//Shift Delete
			key==pk[42]  )//Delete,2-key
		ExecuteHotKey(41);
	else if(key==pk[46])//view with...
		ExecuteHotKey(46);
	else if(key==pk[47])//view with...
		ExecuteHotKey(47);
	else if(key==pk[48])//view with...
		ExecuteHotKey(48);
	else if(key==pk[49])//edit with...
		ExecuteHotKey(49);
	else if(key==pk[50])//edit with...
		ExecuteHotKey(50);
	else if(key==pk[51])//edit with...
		ExecuteHotKey(51);
	else if(key==pk[52])//Ctrl + C
		ExecuteHotKey(52);
	else if(key==pk[53])//Ctrl + X
		ExecuteHotKey(53);
	else if(key==pk[54])//Ctrl + V
		ExecuteHotKey(54);
	else if(key==pk[55])//Ctrl + Alt + C
		ExecuteHotKey(55);
	else if(key==pk[56])//Ctrl + Alt + X
		ExecuteHotKey(56);
	else if(key==pk[57])//Ctrl + Q
		ExecuteHotKey(57);
	else if(menuUtils::CheckKeys(hWnd,&ik))
	{}//ik=key
	else if(vrtlPanels::CheckKeys(&ik,this))
	{}//ik=key
	else if(wParam>0x2f && wParam<=0x6f || wParam>=0xb0 && wParam<=0xdc)////if(bAltPressed)
		EnterQuickSearchMode((wchar_t)wParam);
	ShowSelectedFilesInfo();
//profilerOutStr("End of Panel::OnKeybPressed")
	return TRUE;
#undef ifkey
}

//Ctrl, yoki Shft bosilmagan:
VOID Panel::OnMouseClick(int id)
{
	if(0==bInput) return;
	if(1==panel[iAntiOpponent].bQView) return;

	//profilerOutStr("Beg of Panel::OnMouseClick ")
	//profilerPrintf2("\nid=%d bOldPosSelected=%d",id,bOldPosSelected)
	DWORD t=GetTickCount();
	int oS = pItems[id].state;
	BOOL bSelections = (totSelects>0);
	if(bSelections) FreeSelection();
	
	if(id==iHot)//Eskisini ustiga 2-marta bosyapti;
	{	if(selected!=oS)
			AddToSelection(id);
	}

	SetHot(id);
	ShowSelectedFilesInfo();
	viewFileAssoc::QuickView(GetFullPathAndName(id));
	SetTimer(hWnd,IDPANELTIMER_SCRLMOUSE,1500,0);	
}

//Ctrl b-n birga bosilgan:
VOID Panel::OnMouseCtrlClick(int id)
{
	if(0==bInput) return;
	if(1==panel[iAntiOpponent].bQView) return;

	//profilerOutStr("Beg of Panel::OnMouseCtrlClick ")
	//profilerPrintf3("\nid=%d bOldPosSelected=%d bChngOld=%d",id,bOldPosSelected,bChngOld)
	if(selected!=pItems[id].state)
		AddToSelection(id);
	else
		DeleteFromSelection(id);

	SetHot(id);

	ShowSelectedFilesInfo();
}

//Ctrl, yoki Shft bosilmagan:
VOID Panel::OnMouseRClick(int x,int y,int id)
{
	if(0==bInput) return;
	if(1==panel[iAntiOpponent].bQView) return;

	OnMouseClick(id);

	fileMenuPopup.idFile = id;
/*	fileMenuPopup.hMenu = LoadMenu(hInst, (LPCSTR)(IDM_MENUBAR)); 
    if(fileMenuPopup.hMenu == NULL) return;

	if(conf::winVerMajor<6)
	{MENUITEMINFO mi;mi.cbSize=sizeof(MENUITEMINFO);
	 mi.fMask=MIIM_STATE;mi.fState=MFS_DISABLED;
	 SetMenuItemInfo(fileMenuPopup.hMenu,IDM_FILE_STREAM,FALSE,&mi);
	}
 
	fileMenuPopup.posX = x;fileMenuPopup.posY = y;
	POINT pt={x,y};
	ClientToScreen(hWnd,&pt);

	fileMenuPopup.hSubmenu = GetSubMenu(fileMenuPopup.hMenu, 0);
    TrackPopupMenu(fileMenuPopup.hSubmenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, 
        pt.x, pt.y, 0, hWnd, NULL);
	//DestroyMenu(fileMenuPopup.hMenu);*/
	POINT pt={x,y};    wchar_t s[11][MAX_PATH];
	myWMI::TAddtnMenuItems items[11];int iApndMenus=0;
	if(	file==pItems[id].attribute &&
		fileStream::IsFileConsistStream(GetFullPathAndName(id)))
	{	LoadString(hInst,IDS_STRINGSW_68,s[iApndMenus],MAX_PATH);
		items[iApndMenus].pNext = NULL;
		items[iApndMenus].itemStr = s[iApndMenus];
		items[iApndMenus].iPan = iThis;
		items[iApndMenus].iPanItem = id;
		items[iApndMenus++].iMsg = IDM_FILE_STREAM;
	}
	LoadString(hInst,IDS_STRINGSW_236,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = iApndMenus>0?(&items[iApndMenus-1]):NULL;
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_COPYALLITEMSASTEXT;
	
	LoadString(hInst,IDS_STRINGSW_237,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_COPYSELECTIONASTEXT;

	LoadString(hInst,IDS_STRINGSW_238,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_COPYALLITEMSFULLASTEXT;

	LoadString(hInst,IDS_STRINGSW_239,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_COPYSELECTIONFULLASTEXT;

	LoadString(hInst,IDS_STRINGSW_251,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_CUT;

	LoadString(hInst,IDS_STRINGSW_36,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_COPY;

	LoadString(hInst,IDS_STRINGSW_252,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_PAST;

	LoadString(hInst,IDS_STRINGSW_203,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_DELETE;

	LoadString(hInst,IDS_STRINGSW_112,s[iApndMenus],MAX_PATH);
	items[iApndMenus].pNext = &items[iApndMenus-1];
	items[iApndMenus].itemStr = s[iApndMenus];
	items[iApndMenus].iPan = iThis;
	items[iApndMenus++].iMsg = IDM_EDIT_SELECTALL;

	myWMI::DoExplorerMenu(hWnd,GetFullPathAndName(id),&pt,&items[iApndMenus-1]);
}

//Shift b-n birga bosilgan:
VOID Panel::OnMouseShftClick(int id)
{
	if(0==bInput) return;
	if(1==panel[iAntiOpponent].bQView) return;

	//profilerOutStr("Beg of Panel::OnMouseShftClick ")
	//profilerPrintf1("\nid=%d",id)
	if(iHot >-1 && iHot<id)
	{	for(int i=iHot; i<=id; i++)
		{	if(selected!=pItems[i].state)
				AddToSelection(i);
			else
				DeleteFromSelection(i);
	}	}
	else if(iHot > id)
	{	for(int i=id; i<=iHot; i++)
		{	if(selected!=pItems[i].state)
				AddToSelection(i);
			else
				DeleteFromSelection(i);
	}	}

	SetHot(id);

	ShowSelectedFilesInfo();
}

VOID Panel::OnSheetTabChanged()
{
	//::KillHotAllWithout(iThis);
	int i = TabCtrl_GetCurSel(sheetTab[iThis]);//yangisi;
	if(i>-1)
	if(i!=iTabSheet)
	{	//Yangisida arxiv bo'lmasa va eskisida bo'lsa yopsun:
		TCITEM item;item.mask=TCIF_PARAM;
		if(TabCtrl_GetItem(sheetTab[iThis],i,&item))
		{	SheetTab* sheet = (SheetTab*)item.lParam;
			if(archElem!=sheet->entrType)
			{	if(TabCtrl_GetItem(sheetTab[iThis],iTabSheet,&item))
				{	SheetTab* sheet = (SheetTab*)item.lParam;
					if(archElem==sheet->entrType)
						arch.Close$4();
		}	}	}
		SheetTab::Load(sheetTab[iThis],i,this);
		iTabSheet = i;
}	}

VOID Panel::OnSheetTabChanging()
{
	int i = TabCtrl_GetCurSel(sheetTab[iThis]);//eskisi;
	if(i>-1)
	SheetTab::Save(sheetTab[iThis],i,this);
}

BOOL Panel::OtherControlMsgQueue(MSG *m)
{
	if(m->hwnd==hEditItem)
	{	EditItemMsgQueue(m);
		return TRUE;
	}
	else if(m->hwnd==GetDlgItem(hQuickSearchEdit,IDC_EDIT_SEARCH))
	{	EditQuickSearchMsgQueue(m);
		return TRUE;
	}
	else if(m->hwnd==pathAndNameEdt[1])
	{	EditPathAndNameMsgQueue(m);
		return TRUE;
	}
	return FALSE;
}

BOOL Panel::ReadToPathAndNameCombo(int iMax)
{
	if(0==iThis)
		pnlPthAndNamesCB[0].Read(pnlPthAndNamesCB0FName,pathAndNameEdt[0],40,128);
	else if(1==iThis)
		pnlPthAndNamesCB[1].Read(pnlPthAndNamesCB1FName,pathAndNameEdt[0],40,128);
	else if(2==iThis)
		pnlPthAndNamesCB[2].Read(pnlPthAndNamesCB2FName,pathAndNameEdt[0],40,128);
	else// if(3==iThis)
		pnlPthAndNamesCB[3].Read(pnlPthAndNamesCB3FName,pathAndNameEdt[0],40,128);
	ShowPathAndName(0);
	return TRUE;
}

VOID Panel::RecalcItemsRects()
{int r = 0;
//profilerOutStr("Beg of Panel::RecalcItemsRects ")
 if(0==bItemsViewType)
 {//WARN: Crit.sect. nColumns will be changed:
  nColumns = 0;					   //*
  for(int i=0; i<totItems; i++)	   //*
  {	if(r > nRows-1)                //*
		{ r = 0; nColumns++; }	   //*
	pItems[i].nColumn = nColumns;  //*
	pItems[i].nRow = r++;		   //*
  } nColumns++;//******************//*
 }
 else
 {for(int i=0; i<totItems; i++)	   //*
  {//	if(r > nRows-1)            //*
//		r = 0;					   //*
	pItems[i].nRow = i;			   //*
	pItems[i].nColumn = 0;		   //*
 }}//******************************//*
 //End of crit section, nColumns recalced!!!
}

VOID Panel::RedSelectAllWithout(int id)
{
//profilerOutStr("Beg of Panel::RedSelectAllWithout ")
//profilerPrintf1("\nid=%d",id)

	for(int i=0; i<totSelects; i++)
	{	//Ctrl 1 marta bosilib qlgan, lekin qizil emas bo'lsa:
		//if(-1==pSelects[i]) continue;//zombi
		//if(hotunselected==pItems[pSelects[i]].state)
		//{	if(1==ChangeSelection(pSelects[i],normal,TRUE))
		//		--i;
		//}
		//else
		//	AddToSelection(pSelects[i],selected,TRUE);
	}
	if(-1!=id)
	{	if(normal==pItems[id].state)
			iHot=id;
		//else if(selected==pItems[id].state)
		//	ChangeSelection(id,hotselected,TRUE);
		//else if(hotselected==pItems[id].state)
		//	ChangeSelection(id,hotunselected,TRUE);
		//else if(hotunselected==pItems[id].state)
		//	ChangeSelection(id,hotselected,TRUE);
}	}

VOID Panel::Render(PAINTSTRUCT *ps)
{
	if(!hWnd) return;
	if(1==panel[iAntiOpponent].bQView)
	{	QView::Draw(panel[iAntiOpponent].GetFullPathAndName(panel[iAntiOpponent].GetHot()));
		return;
	}

	HDC dc;	if(!ps) dc = ::GetDC(hWnd); else dc = ps->hdc;

	HFONT oldFnt=(HFONT)SelectObject(dc,hf);

	if(imgBckgrnd[0].dc || (panel[0].imgBckgrnd[0].dc && 1==conf::Dlg.panBckgrndImgType[iThis]))
	{	FillRect(dc, &rcClient, bkBrsh);
		BitBlt(dc,0,0,width,height,imgBckgrnd[0].dc,0,0,SRCCOPY);
		SetBkMode(dc,TRANSPARENT);
		if(0==bItemsViewType)//switch qila olmaymiz,chunki bitpole;
		{	for(S32 i=0; i<totItems; i++)
				pItems[i].RenderTransparent(dc,this,TRUE);
		}
		else if(1==bItemsViewType)
		{	for(S32 i=0; i<totItems; i++)
				pItems[i].RenderTransparentFullCnfg(dc,this,TRUE);
		}
		else//2,custom
		{	for(S32 i=0; i<totItems; i++)
				pItems[i].RenderTransparentCustomCnfg(dc,this,TRUE);
	}	}
	else
	{	FillRect(dc, &rcClient, bkBrsh);
		SetBkMode(dc,OPAQUE);
		if(0==bItemsViewType)//switch qila olmaymiz,chunki bitpole;
		{	for(S32 i=0; i<totItems; i++)
				pItems[i].Render(dc,this,TRUE);
		}
		else if(1==bItemsViewType)
		{	for(S32 i=0; i<totItems; i++)
				pItems[i].RenderFullCnfg(dc,this,TRUE);
		}
		else//2,custom
		{	for(S32 i=0; i<totItems; i++)
				pItems[i].RenderCustomCnfg(dc,this,TRUE);
	}	}

	SelectObject(dc,oldFnt);
	if(!ps) ::ReleaseDC(hWnd, dc);
	bRenderFull = 0;
}

VOID Panel::RenderItem(int id)
{
//wchar_t s[260];static int i=0;
//OutputDebugString(L"\n");
//wsprintf(s,L"%d panel: %d %d %s selSt:%d",i++,iThis+1,id,pItems[id].Name,pItems[id].state);
//	OutputDebugString(s);
	if(1==panel[iAntiOpponent].bQView)
		return;

	HDC dc = GetDC();
	HFONT oldFnt=(HFONT)SelectObject(dc,hf);
	if(id>-1)
	if(id<totItems)
	{	if(imgBckgrnd[0].dc || (panel[0].imgBckgrnd[0].dc && 1==conf::Dlg.panBckgrndImgType[iThis]))
		{	SetBkMode(dc,TRANSPARENT);
			if(0==bItemsViewType)//switch qila olmaymiz,chunki bitpole;
				pItems[id].RenderTransparent(dc,this,TRUE);
			else if(1==bItemsViewType)
				pItems[id].RenderTransparentFullCnfg(dc,this,TRUE);
			else//2,custom
				pItems[id].RenderTransparentCustomCnfg(dc,this,TRUE);
		}
		else
		{	SetBkMode(dc,OPAQUE);
			if(0==bItemsViewType)//switch qila olmaymiz,chunki bitpole;
				pItems[id].Render(dc,this,TRUE);
			else if(1==bItemsViewType)
				pItems[id].RenderFullCnfg(dc,this,TRUE);
			else//2,custom
				pItems[id].RenderCustomCnfg(dc,this,TRUE);
	}	}
	SelectObject(dc,oldFnt);
	ReleaseDC(dc);
}

VOID Panel::RenderFullIcons()
{
	bRenderFull = 1;
	HDC dc = ::GetDC(hWnd);
	HFONT oldFnt=(HFONT)SelectObject(dc,hf);

	if(imgBckgrnd[0].dc || (panel[0].imgBckgrnd[0].dc && 1==conf::Dlg.panBckgrndImgType[iThis]))
	{	for(S32 i=0; i<totItems; i++)
			pItems[i].RenderFullIconTransparent(dc,this);
	}
	else
	{	for(S32 i=0; i<totItems; i++)
			pItems[i].RenderFullIcon(dc,this);
	}

	SelectObject(dc,oldFnt);
	::ReleaseDC(hWnd, dc);
}

/*BOOL Panel::SetChangeNotifyPathA(char* p)
{
wchar_t s[MAX_PATH];
	MultiByteToWideChar(AreFileApisANSI()?CP_ACP:CP_OEMCP,MB_PRECOMPOSED,p,MAX_PATH-1,s,MAX_PATH);
	return fManager.SetChangeNotifyPath(s);
}*/

/*VOID Panel::ShowPathAndNameA(int selId,char* excptTxt)
{
wchar_t s[MAX_PATH];
	MultiByteToWideChar(AreFileApisANSI()?CP_ACP:CP_OEMCP,MB_PRECOMPOSED,excptTxt,MAX_PATH-1,s,MAX_PATH);
	ShowPathAndName(selId,s);
}*/

VOID Panel::ShowPathAndName(int selId,wchar_t* excptTxt)
{
wchar_t st[MAX_PATH];int ln;

//	wchar_t s[260];static int i=0;
//	wsprintf(s,L"\n%d iActivePanel:%d Panel-%d ShowPathAndName",i++,iActivePanel,iThis);
//	OutputDebugString(s);


	if(excptTxt)
	{	SetWindowText(pathAndNameEdt[1],excptTxt);
		return;
	}
	switch(entry.GetCrntRecType())
	{	default:case unknown:
			break;
		case directFolder:
			if(selId>0 && selId<totItems)
				SetWindowText(pathAndNameEdt[1],GetFullPathAndName(selId));
			else if(iHot>0 && iHot<totItems-1)
				SetWindowText(pathAndNameEdt[1],GetFullPathAndName(iHot));
			else
				SetWindowText(pathAndNameEdt[1],path);
			break;
		case guidFolder:
			if(selId>0 && selId<totItems-1)
				SetWindowText(pathAndNameEdt[1],pItems[selId].Name);
			else if(iHot>0 && iHot<totItems-1)
				SetWindowText(pathAndNameEdt[1],pItems[iHot].Name);
			else
			{	if((':' == path[0] && ':' == path[1]) || 0 == path[0])
					SetWindowText(pathAndNameEdt[1],(LPCWSTR)&path[GUID_CHAR_POS]);
				else
					SetWindowText(pathAndNameEdt[1],(LPCWSTR)path);
			}
			break;
		case rndPathList:
			SetWindowText(pathAndNameEdt[1],L"Results of searches:");
			break;
		case socketCl://char s[MAX_PATH];
			//entry.GetCrnt(s,socketCl);
			//SetWindowText(pathAndNameEdt[1],s);
			break;
		case archElem:
			ln=MyStringCpy(st,MAX_PATH-1,entry.GetCrntRecPath());
			st[ln++]='\\';st[ln++]='\\';
			ln+=MyStringCpy(&st[ln],MAX_PATH-1,entry.GetCrntRecArchPath());
			if(selId>0)
			{	
RepAdd:			if('\\'!=st[ln-1])st[ln++]='\\';
				ln += MyStringCpy(&st[ln],MAX_PATH-1,pItems[selId].Name)+1;
				if(folder==pItems[selId].attribute)
					{st[ln]='\\';st[ln+1]='*';st[ln+2]=0;}
				else st[ln]=0;
			}
			else
			{	selId=FindItem(findItemName);
				if(selId>0)
					goto RepAdd;
			}
			SetWindowText(pathAndNameEdt[1],st);
			break;
		case virtualPanel:
			st[0]=0;
			if(selId>0)
			if(!vrtlPanels::plgns[iVPPlg].GetItemPathAndName$16(VPPlgObj,//vrtlPanels::plgns[arch.plgNum].plgObj,
															st,MAX_PATH,pItems[selId].itemId))
				GetFullPathAndName(selId,st,MAX_PATH);
			if('\\'==st[0] && '\\'==st[1] && '?'==st[2] && '\\'==st[3] && 
				'U'==st[4] && 'N'==st[5] && 'C'==st[6] && '\\'==st[7])
			{	st[6] = '\\';
				SetWindowText(pathAndNameEdt[1],&st[6]);
				::SendMessage(selFilesInfoEdt, WM_SETTEXT, 0, (LPARAM)&st[6]);
			}
			else
			{	SetWindowText(pathAndNameEdt[1],st);
				::SendMessage(selFilesInfoEdt, WM_SETTEXT, 0, (LPARAM)st);
			}
			break;
}	}

VOID Panel::ShowSelectedFilesInfo()
{
//	profilerOutStr("Beg of Panel::ShowSelectedFilesInfo ")
//	RECT r={0,0,width-4,INFO_EDT_HEIGHT_INSIDE};
	wchar_t s[MAX_PATH], st[MAX_PATH];
	if(virtualPanel==entry.GetCrntRecType())
	{	if(iHot<1)return;
		if(iHot>totItems-1)return;
		ShowPathAndName(iHot);		
		return;
	}
	if(totSelects<2)
	{	if(iHot<0 || iHot>totItems-1)
		{	::SendMessage(selFilesInfoEdt, WM_SETTEXT, 0, (LPARAM)L"...");
			return;
		}

		ShowPathAndName(iHot);
		int x,extPos=-1,szPos=-1;
		for(x=0; x<pItems[iHot].NameCharLen; x++)
			s[x] = pItems[iHot].Name[x];
		s[x++] = ' ';

		if(file==pItems[iHot].attribute)
		{	extPos = x-1;
			for(int i=0; i<pItems[iHot].ExtCharLen; i++)
				s[x++] = pItems[iHot].Name[pItems[iHot].NameCharLen+1+i];
			szPos = x;
			s[x++] = ' ';

			int sizesChars = MyStringPrintDecSpace(st,pItems[iHot].size);
			for(int i=0; i<sizesChars; i++)
				s[x++] = st[i];
			s[x++] = ' '; s[x++] = '(';

			sizesChars = MyStringPrintDecSpace(st,pItems[iHot].size/1024);
			for(int i=0; i<sizesChars-1; i++)
				s[x++] = st[i];
			s[x++] = ' '; s[x++] = 'k'; s[x++] = ')';
			s[x] = 0;

			My3StrCpyBoundWidth(s,st,width-4,extPos,szPos,(width-4)/2,(int)(0.65f*(width-4)));
			::SendMessage(selFilesInfoEdt, WM_SETTEXT, 0, (LPARAM)st);
		}
		else
		{	s[--x]=0;
			::SendMessage(selFilesInfoEdt, WM_SETTEXT, 0, (LPARAM)s);
		}
		selectedItemsSizes = 0;//Xatoni ko'rish uchun shu qatorga // chizib qo'y;
	}
	else
	{	ShowPathAndName(0);
		int l = MyStringPrintDecSpace(st,selectedItemsSizes);
		st[l] = 0;
		StringCchPrintf(s,MAX_PATH-1,L"%d items selected, folders:%d, files:%d,size of files:%s",
			totSelects,totSelectsFolders,totSelectsFiles,st);
		::SendMessage(selFilesInfoEdt, WM_SETTEXT, 0, (LPARAM)s);
}	}

VOID Panel::SintezKey(int k)
{
	conf::TKey *key = &conf::Dlg.keys[k];
	if(key->bAlt || key->bCtrl || key->bShft)
	{	BYTE kb[256];
		GetKeyboardState(kb);
		if(key->bAlt)
		{	kb[VK_LMENU] |= 0x1000;
			kb[VK_RMENU] &= 0x7fff;
		}
		else
		{	kb[VK_LMENU] &= 0x7fff;
			kb[VK_RMENU] &= 0x7fff;
		}
		if(key->bCtrl)
		{	kb[VK_LCONTROL] |= 0x1000;
			kb[VK_RCONTROL] &= 0x7fff;
		}
		else
		{	kb[VK_LCONTROL] &= 0x7fff;
			kb[VK_RCONTROL] &= 0x7fff;
		}
		if(key->bShft)
		{	kb[VK_LSHIFT] |= 0x1000;
			kb[VK_RSHIFT] &= 0x7fff;
		}
		else
		{	kb[VK_LSHIFT] &= 0x7fff;
			kb[VK_RSHIFT] &= 0x7fff;
		}
		SetKeyboardState(kb);
	}
	PostMessage(WM_KEYDOWN,key->key,0);
}

//Tanlangan item hamma vaqt viewda turishi shart:
VOID Panel::ScrollItemToView(int id)
{
//	profilerOutStr("Beg of Panel::ScrollItemToView ")
//	profilerPrintf1("\nid=%d",id)

	scrlLineSpeedTick=0;
	if(0==bItemsViewType)
	{	if(id<0) id=0;
		int pos = ScrollOffst+pItems[id].nColumn*itemMaxWidth;
		if(pos < 0)
		{	//for(int i=0; i<-pos/itemMaxWidth; i++)
			//	::SendMessage(hWnd,WM_HSCROLL,SB_PAGELEFT,1);//Post edi
			if(pos/itemMaxWidth < 0)
				::SendMessage(hWnd,WM_HSCROLL,SB_PAGELEFT,-pos/itemMaxWidth);//Post edi
		}
		else if(pos+itemMaxWidth > rcClient.right)
		{	//for(int i=0; i<pos/itemMaxWidth; i++)
			//	::SendMessage(hWnd,WM_HSCROLL,SB_PAGERIGHT,1);//Post edi
			if(pos/itemMaxWidth > 0)
				::SendMessage(hWnd,WM_HSCROLL,SB_PAGERIGHT,pos/itemMaxWidth);
	}	}
	else
	{	if(id<0) id=0;
		int pos = ScrollOffst+pItems[id].nRow*itemHeight;
		if(pos < 0)
		{	if(pos < 0)
				::SendMessage(hWnd,WM_VSCROLL,SB_LINEUP,-pos);
		}
		else if(pos+itemHeight > rcClient.bottom)
			::SendMessage(hWnd,WM_VSCROLL,SB_LINEDOWN,pos+itemHeight-rcClient.bottom);
}	}

VOID Panel::SetHot(int h,BOOL bDraw)
{
	if(h>-1)
	if(h<totItems)
	for(int i=0; i<conf::Dlg.iTotPanels; ++i)
	{	if(&panel[i]!=this)
		{	int poldHot = panel[i].iHot;
			panel[i].iHot = -1;//SetHot(-1,TRUE);
			//if(selected==panel[i].GetItem(poldHot))
			panel[i].RenderItem(poldHot);
	}	}

	int oldHot = iHot;
	iHot = h;

	if(oldHot!=iHot)
	{	if(iHot<0)
			ScrollItemToView(0);
		else if(iHot>totItems-1)
			ScrollItemToView(totItems-1);
		else
			ScrollItemToView(iHot);
	}
	if(bDraw)
	{	if(oldHot>-1)
		if(oldHot<totItems)
		if(oldHot!=iHot)
			RenderItem(oldHot);
		if(iHot>-2)//-1 edi
		if(iHot<totItems)
			RenderItem(iHot);
	}
	if(1==bQView)
		QView::Draw(GetFullPathAndName(iHot));
}

BOOL Panel::SetIcons(HINSTANCE inst)
{
//profilerOutStr("Beg of Panel::SetIcons ")
//profilerPrintf1("\ninst=%d",inst)

BOOL r = TRUE;
HGLOBAL hMem;HRSRC hResource; LPVOID lpResource;

 if(!hIcons[0])//static bo'lgani uchun 1 ta;
 { 	for(int i=0; i<TOTAL_ICONS; i++)
	{	hResource = FindResource(inst, MAKEINTRESOURCE(IDI_ICON_UP_ARROW+i),RT_GROUP_ICON);
		if(!hResource) {hIcons[i] = NULL; continue;}
		hMem = LoadResource(inst, hResource);
		if(!hMem) {hIcons[i] = NULL; continue;}
		lpResource = LockResource(hMem); 
		if(!lpResource) {hIcons[i] = NULL; continue;}
		lpResource = LockResource(hMem);
		if(!lpResource) {hIcons[i] = NULL; continue;}
		int nID = LookupIconIdFromDirectoryEx((PBYTE)lpResource,TRUE,conf::Dlg.iIconSize[0],conf::Dlg.iIconSize[0],LR_DEFAULTCOLOR);
		if(!nID) {hIcons[i] = NULL; continue;}
		hResource = FindResource(inst, MAKEINTRESOURCE(nID), MAKEINTRESOURCE(RT_ICON));
		if(!hResource) {hIcons[i] = NULL; continue;}
		hMem = LoadResource(inst, hResource);
		if(!hMem) {hIcons[i] = NULL; continue;}
		lpResource = LockResource(hMem);
		if(!lpResource) {hIcons[i] = NULL; continue;}
		hIcons[i] = CreateIconFromResourceEx((PBYTE)lpResource, SizeofResource(inst,hResource),
			//TRUE, 0x00030000, conf::Dlg.iIconSize[0], conf::Dlg.iIconSize[0], LR_DEFAULTCOLOR);
			TRUE, 0x00030000, 0, 0, LR_DEFAULTCOLOR);
 }	}
 return r;
}

BOOL Panel::SetAllItemsListToClipboard(BOOL bFullPathAndName)
{
	if(!OpenClipboard(hWnd))return FALSE; 
	EmptyClipboard();
int sz=0;
	if(totItems<2)
	{	CloseClipboard();
		return FALSE;
	}
	for(int i=0; i<totItems; i++)
	{	if(bFullPathAndName)
			sz += MyStringLength(GetFullPathAndName(i),MAX_PATH)+2;
		else
		{	if(pItems[i].ExtCharLen>0)
				sz += pItems[i].NameCharLen+pItems[i].ExtCharLen+3;
			else
				sz += pItems[i].NameCharLen+2;
	}	}
	//Calc the size of buufer:
	HGLOBAL gl = GlobalAlloc(GMEM_MOVEABLE,2*(sz+2) * sizeof(WCHAR));
	if(!gl)
	{	CloseClipboard();
		return FALSE;
	}
	wchar_t *buf = (wchar_t*)GlobalLock(gl);

	for(int i=1; i<totItems; i++)
	{	if(bFullPathAndName)
		{	sz = GetFullPathAndName(i,buf,MAX_PATH);
			buf += sz;
		}
		else
		{	if(pItems[i].ExtCharLen>0)
			{	memcpy(buf,pItems[i].Name,sizeof(wchar_t)*(pItems[i].NameCharLen+pItems[i].ExtCharLen+2));
				buf += pItems[i].NameCharLen+pItems[i].ExtCharLen+1;
			}
			else
			{	memcpy(buf,pItems[i].Name,sizeof(wchar_t)*(pItems[i].NameCharLen+pItems[i].ExtCharLen+1));
				buf += pItems[i].NameCharLen+pItems[i].ExtCharLen;
		}	}
		(*buf++) = 0x000d;
		(*buf++) = 0x000a;
	}
	(*buf) = 0;
	GlobalUnlock(gl);
	SetClipboardData(CF_UNICODETEXT,gl); 
	CloseClipboard();
	GlobalFree(gl);
	return TRUE;
}

/*void Panel::SetPathA(char *s,int l)
{	
	if(s && s[0])
	{	pathLn=MultiByteToWideChar(AreFileApisANSI()?CP_ACP:CP_OEMCP,MB_PRECOMPOSED,s,-1,path,MAX_PATH);
		if(pathLn)--pathLn;
}	}*/

void Panel::SetPath(wchar_t *sw,int l,BOOL bCheckExisting)
{	
	if(!bCheckExisting)
	{	pathLn=MyStringCpy(path,l,sw);
		if((':' == sw[0] && ':' == sw[1]) || 0 == sw[0])
			MyStringCpy(&path[GUID_CHAR_POS],GUID_CHAR_LEN,&sw[GUID_CHAR_POS]);
		/*else if('*'!=path[pathLn-1] && '\\'!=path[pathLn-2])
		{	path[pathLn++]='\\';
			path[pathLn++]='*';
			path[pathLn]=0;
	}*/	}
	else if(sw && sw[0])
	{	MyStringCpy(oldPathW,MAX_PATH,path);
		pathLn=MyStringCpy(path,l,sw);
		wchar_t *p = &path[1];
		while(p>&path[0])
		{	BOOL bAccessDenied=FALSE;
TryAccessCorrect:
			if(IsDirExist(path))
				return;
			if(ERROR_ACCESS_DENIED==GetLastError())
			{	if(!bAccessDenied)
				{	ChangePrivilege(0,17,TRUE);
					bAccessDenied=TRUE;
					goto TryAccessCorrect;
			}	}
			p=wcsrchr(path,'\\');
			if(p)*p--=0;
			p=wcsrchr(path,'\\');
			if(p){*(p+1)='*';*(p+2)=0;}
		}
		MyStringCpy(path,MAX_PATH-1,oldPathW);
}	}

BOOL Panel::SetSelectedItemsListToClipboard(BOOL bFullPathAndName)
{
if(!OpenClipboard(hWnd))return FALSE; 
	EmptyClipboard();
	int sz=0;
	if(totSelects<1 && (iHot<1 || iHot>totItems-1))
	{	CloseClipboard();
		return FALSE;
	}
	if(totSelects<1)
	{	if(bFullPathAndName)
			sz += MyStringLength(GetFullPathAndName(iHot),MAX_PATH)+2;
		else
		{	if(pItems[iHot].ExtCharLen>0)
				sz = pItems[iHot].NameCharLen+pItems[iHot].ExtCharLen+3;
			else
				sz = pItems[iHot].NameCharLen+2;
	}	}
	else
	{	for(int i=1; i<totItems; i++)
		{	if(selected==pItems[i].state)
			{	if(bFullPathAndName)
					sz += MyStringLength(GetFullPathAndName(iHot),MAX_PATH)+2;
				else
				{	if(pItems[i].ExtCharLen>0)
						sz += pItems[i].NameCharLen+pItems[i].ExtCharLen+3;
					else
						sz += pItems[i].NameCharLen+2;
	}	}	}	}
	//Calc the size of buufer:
	HGLOBAL gl = GlobalAlloc(GMEM_MOVEABLE,2*(sz+2) * sizeof(TCHAR));
	if(!gl)
	{	CloseClipboard();
		return FALSE;
	}
	wchar_t *buf = (wchar_t*)GlobalLock(gl);

	if(totSelects<1)
	{	if(bFullPathAndName)
		{	sz = GetFullPathAndName(iHot,buf,MAX_PATH);
			buf += sz;
		}
		else
		{	if(pItems[iHot].ExtCharLen>0)
			{	memcpy(buf,pItems[iHot].Name,sizeof(wchar_t)*(pItems[iHot].NameCharLen+pItems[iHot].ExtCharLen+1));
				buf += pItems[iHot].NameCharLen+pItems[iHot].ExtCharLen+1;
			}
			else
			{	memcpy(buf,pItems[iHot].Name,sizeof(wchar_t)*pItems[iHot].NameCharLen);
				buf += pItems[iHot].NameCharLen;
		}	}
		(*buf) = 0;
	}
	else
	{	for(int i=1; i<totItems; i++)
		{	if(selected==pItems[i].state)
			{	if(bFullPathAndName)
				{	sz = GetFullPathAndName(i,buf,MAX_PATH);
					buf += sz;
				}
				else
				{	if(pItems[i].ExtCharLen>0)
					{	memcpy(buf,pItems[i].Name,sizeof(wchar_t)*(pItems[i].NameCharLen+pItems[i].ExtCharLen+1));
						buf += pItems[i].NameCharLen+pItems[i].ExtCharLen+1;
					}
					else
					{	memcpy(buf,pItems[i].Name,sizeof(wchar_t)*pItems[i].NameCharLen);
						buf += pItems[i].NameCharLen;
				}	}
				(*buf++) = 0x0d;
				(*buf++) = 0x0a;
	}	}	}
	(*buf) = 0;
	GlobalUnlock(gl);
	SetClipboardData(CF_UNICODETEXT,gl); 
	CloseClipboard();
	GlobalFree(gl);
	return TRUE;
}

BOOL Panel::SetSortBtnState(int btn, int st)
{
BOOL r = FALSE;
wchar_t s[MAX_PATH];
	switch(btn)
	{	case 0://folder & file mixing mode:
			if(st<foldersUp) return FALSE;
			if(st>mixing) return FALSE;
			folderAndFileMixingSortType = (FolderAndFileMixingSortType)st;
			LoadString(hInst,IDS_STRINGSW_21+panel[iThis].folderAndFileMixingSortType,s,MAX_PATH);
			r = TRUE;
			break;
		case 1://folder sort mode:
			switch(st)
			{	case alphabeticalDwn:
					folderSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forEnums:
					folderSortType = (SortType)st;
					LoadString(hInst,IDS_STRINGSW_24+1,s,MAX_PATH-1);
					r = TRUE;
					break;
				case forCreateTimeUp:
					folderSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24+2,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forCreateTimeDwn:
					folderSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24+2,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forSizeUp:
					folderSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24+3,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forSizeDwn:
					folderSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24+3,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forTypeUp:
					folderSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24+4,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forTypeDwn:
					folderSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24+4,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case alphabeticalUp:
					folderSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_24,s+2,MAX_PATH-1);
					r = TRUE;
					break;
			}
			break;
		case 2://file:
			switch(st)
			{	case alphabeticalDwn:
					fileSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forEnums:
					fileSortType = (SortType)st;
					LoadString(hInst,IDS_STRINGSW_29+1,s,MAX_PATH-1);
					r = TRUE;
					break;
				case forCreateTimeUp:
					fileSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29+2,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forCreateTimeDwn:
					fileSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29+2,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forSizeUp:
					fileSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29+3,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forSizeDwn:
					fileSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29+3,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forTypeUp:
					fileSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29+4,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case forTypeDwn:
					fileSortType = (SortType)st;
					s[0] = L'↓'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29+4,s+2,MAX_PATH-1);
					r = TRUE;
					break;
				case alphabeticalUp:
					fileSortType = (SortType)st;
					s[0] = L'↑'; s[1] = L' ';
					LoadString(hInst,IDS_STRINGSW_29,s+2,MAX_PATH-1);
					r = TRUE;
					break;
			}
			break;
	}
	if(r)
	{	SetWindowText(sortBtns[iThis][btn],s);
		if(hWnd)
		{	if(pItems)
			{	Sort();
				RecalcItemsRects();//myQsort(swap)
				ScrollItemToView(iHot);
				Render();
			}
			SetFocus();
	}	}
	return r;
}

VOID Panel::ChangeViewMenu(int partOfMenu)//0-Up,1-Down part of 'View'-menu;
{
HMENU hm = GetMenu(::hWnd);
	if(!hm)return;
 switch(partOfMenu)
 {case 0:MENUITEMINFO mi;mi.cbSize=sizeof(mi);mi.fMask=MIIM_SUBMENU;mi.hSubMenu=0;
   GetMenuItemInfo(hm,3,TRUE,&mi);
   if(0==bItemsViewType)//00-short,01-full,10-custom;
   {CheckMenuItem(hm,IDM_VIEW_SHORT,MF_BYCOMMAND|MF_CHECKED);
    CheckMenuItem(hm,IDM_VIEW_FULL,MF_BYCOMMAND|MF_UNCHECKED);
	if(mi.hSubMenu)
		CheckMenuItem(mi.hSubMenu,2,MF_BYPOSITION|MF_UNCHECKED);
   }
   else if(1==bItemsViewType)//00-short,01-full,10-custom;
   {CheckMenuItem(hm,IDM_VIEW_SHORT,MF_BYCOMMAND|MF_UNCHECKED);
    CheckMenuItem(hm,IDM_VIEW_FULL,MF_BYCOMMAND|MF_CHECKED);
	if(mi.hSubMenu)
		CheckMenuItem(mi.hSubMenu,2,MF_BYPOSITION|MF_UNCHECKED);
   }
   else if(2==bItemsViewType)//00-short,01-full,10-custom;
   {CheckMenuItem(hm,IDM_VIEW_SHORT,MF_BYCOMMAND|MF_UNCHECKED);
    CheckMenuItem(hm,IDM_VIEW_FULL,MF_BYCOMMAND|MF_UNCHECKED);
	if(mi.hSubMenu)
		CheckMenuItem(mi.hSubMenu,2,MF_BYPOSITION|MF_CHECKED);
	CheckMenuItem(hm,IDM_CUSTOM_NAME,1==bItemsCustomName ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_EXTENSION,1==bItemsCustomExt ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_CREATE_TIME,1==bItemsCustomCrTime ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_WRITE_TIME,1==bItemsCustomWrTime ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_SIZE,1==bItemsCustomSz ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_TOTAL_SIZE,1==bItemsCustomTotSz ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_ATTRIBUTE,1==bItemsCustomAtrb ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_STATE,1==bItemsCustomState ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_NAME_LEN,1==bItemsCustomNameLn ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_CUSTOM_EXT_LEN,1==bItemsCustomLstAccTime ?
				 (MF_BYCOMMAND|MF_CHECKED):(MF_BYCOMMAND|MF_UNCHECKED));
   }
  return;	
  case 1:
	CheckMenuItem(hm,IDM_FOLDERSANDFILESMIXING_FOLDERSTOUP,(foldersUp==folderAndFileMixingSortType) ?
						(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSANDFILESMIXING_FILESTOUP,(filesUp==folderAndFileMixingSortType) ?
						(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSANDFILESMIXING_MIXED,(mixing==folderAndFileMixingSortType) ?
						(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));

	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYSIZEUP,(forSizeUp==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYSIZEDOWN,(forSizeDwn==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYALPHABETUP,(alphabeticalUp==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYALPHABETDOWN,(alphabeticalDwn==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYTIMEUP,(forCreateTimeUp==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYTIMEDOWN,(forCreateTimeDwn==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYTYPEUP,(forTypeUp==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_BYTYPEDOWN,(forTypeDwn==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FOLDERSSORTING_NOTSORTING,(forEnums==folderSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));								
	
	CheckMenuItem(hm,IDM_FILESSORTING_BYSIZEUP,(forSizeUp==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESSORTING_BYSIZEDOWN,(forSizeDwn==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESSORTING_BYALPHABETUP,(alphabeticalUp==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESSORTING_BYALPHABETDOWN,(alphabeticalDwn==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESSORTING_BYTIMEUP,(forCreateTimeUp==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) :	(MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESSORTING_BYTIMEDOWN,(forCreateTimeDwn==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESSORTING_BYTYPEUP,(forTypeUp==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESSORTING_BYTYPEDOWN,(forTypeDwn==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	CheckMenuItem(hm,IDM_FILESNOTSORTING,(forEnums==fileSortType) ?
			(MF_BYCOMMAND|MF_CHECKED) : (MF_BYCOMMAND|MF_UNCHECKED));
	return;
}}

LRESULT CALLBACK Panel::WndProc(HWND wnd,UINT message,WPARAM wParam,LPARAM lParam)
{
//profilerOutStr("Beg of Panel::WndProc")
//profilerPrintf5("\nwnd=%d message=%d wParam=%d lParam=%d bFirstEntr=%d",wnd,message,wParam,lParam,bFirstEntr)

PAINTSTRUCT			ps;
HDC					dc;
//TEXTMETRIC		tm; 

	/*static int i=0;
	wchar_t ss[64];wsprintf(ss,L"\n %d ",i++);
	OutputDebugString(ss);
	OutputDebugString(GetWinNotifyText(message));
	wsprintf(ss,L" %x %x",wParam,lParam);
	OutputDebugString(ss);*/

	//if(0xc02b==message)
	//{if(wParam==HSHELL_WINDOWDESTROYED)
	//	OutputDebugString("\nHSHELL_WINDOWDESTROYED");
	// else if(wParam==HSHELL_WINDOWCREATED)
	//	OutputDebugString("\nHSHELL_WINDOWCREATED");
	//}

	//wchar_t s[260];static int i=0;
	//wsprintf(s,L"\n%d iActivePanel:%d Panel-%d WndProc",i++,iActivePanel,iThis);
	//OutputDebugString(s);

	switch (message)
	{
	case WM_MOUSEACTIVATE:
		SetFocus();
		return 0;
	case WM_SETFOCUS:
		//if(0==iThis)
		//	OutputDebugString(L" Panel - 0 ::WM_SETFOCUS ");
		//else
		//	OutputDebugString(L" Panel - 1 ::WM_SETFOCUS ");
		OnSetFocus();//GetWindowLong(wnd,GWLP_USERDATA));
		return 0;
	case WM_KILLFOCUS:
		if(iHot>-1)
		if(iHot<totItems)
			RenderItem(iHot);
		iOldHot=iHot;
		return 0;
	case WM_CREATE:
		//DragAcceptFiles(hWnd, TRUE);
		hWnd = wnd;
		ResetGetTextExtentPoint32();
		int panelNumber; panelNumber = *((int*)(lParam));
		SetWindowLongPtr(wnd,GWLP_USERDATA,panelNumber);
		//if(!xScrlChar)
		{	//dc = GetDC(wnd);
			//GetTextMetrics(dc, &tm); 
			//xScrlChar = tm.tmAveCharWidth;
			//int xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xScrlChar/2;
			//ReleaseDC(wnd, dc);
			//xScrlClientMax = 48 * xScrlChar + 12 * xUpper;
		}
		return 0;
	case WM_PAINT:
		dc= BeginPaint(wnd,&ps);
		Render(&ps);
		EndPaint(wnd,&ps);
		return 0L;
	//case WM_ERASEBKGND:
		//return 1L;
	case WM_HSCROLL:
		//profilerOutStr("Beg of Panel::WndProc WM_HSCROLL")

		scrlInfo.fMask = SIF_ALL;
		// Save the position for comparison later on
		GetScrollInfo(wnd, SB_HORZ, &scrlInfo);
		DWORD dt;int xOldScrlPos; xOldScrlPos = scrlInfo.nPos;
		switch (LOWORD (wParam))
		{	// user clicked left arrow
			case SB_LINELEFT: 
				if(0==scrlLineSpeedTick)
				{scrlLineSpeedTick=GetTickCount();
				 scrlInfo.nPos -= (int)(lParam?lParam:1);
				}
				else
				{dt=GetTickCount()-scrlLineSpeedTick;
				 if(dt<250)
				  scrlInfo.nPos -= (int)(lParam?lParam:1);
				 else if(dt<5000)
				  scrlInfo.nPos -= (dt/250)*(int)(lParam?lParam:1);
				 else
				  scrlInfo.nPos -= 20*(int)(lParam?lParam:1);
				}
				scrlBtnPressedDir=1;//Left;
				break;
		 	// user clicked right arrow
			case SB_LINERIGHT: 
				if(0==scrlLineSpeedTick)
				{scrlLineSpeedTick=GetTickCount();
				 scrlInfo.nPos += (int)(lParam?lParam:1);
				}
				else
				{dt=GetTickCount()-scrlLineSpeedTick;
				 if(dt<250)
				  scrlInfo.nPos += (int)(lParam?lParam:1);
				 if(dt<5000)
				  scrlInfo.nPos += (dt/250)*(int)(lParam?lParam:1);
				 else
				  scrlInfo.nPos += 20*(int)(lParam?lParam:1);
				}
				scrlBtnPressedDir=2;//Down;
				break;
		 	// user clicked the scroll bar shaft left of the scroll box
			case SB_PAGELEFT:
				scrlInfo.nPos -= itemMaxWidth * (int)(lParam?lParam:1);//1;
				scrlBtnPressedDir=1;
				break;
		 	// user clicked the scroll bar shaft right of the scroll box
			case SB_PAGERIGHT:
				scrlInfo.nPos += itemMaxWidth * (int)(lParam?lParam:1);//1;
				scrlBtnPressedDir=2;
				break;
		 	// user dragged the scroll box
			case SB_THUMBTRACK: 
				scrlInfo.nPos = scrlInfo.nTrackPos;
				break;
			case SB_ENDSCROLL:
				scrlLineSpeedTick = 0;
				if(scrlBtnPressedDir==1)
					SetTimer(hWnd,IDPANELTIMER_HORSCRLBTNLEFT,50,NULL);
				else if(scrlBtnPressedDir==2)
					SetTimer(hWnd,IDPANELTIMER_HORSCRLBTNRIGHT,50,NULL);
				scrlBtnPressedDir=0;
				return 0;
		 	default :
				break;
		}
		// Set the position and then retrieve it.  Due to adjustments
		//   by Windows it may not be the same as the value set.
		scrlInfo.fMask = SIF_POS;//|SIF_RANGE;
		scrlInfo.nMax   = (nColumns>1)?nColumns:0;
		SetScrollInfo(wnd, SB_HORZ, &scrlInfo, TRUE);
		/*if(scrlInfo.nMax && (!bHorScrl))
		{	LONG st = GetWindowLong(wnd,GWL_STYLE);
			st |= WS_HSCROLL; bHorScrl = TRUE;
			SetWindowLong(wnd,GWL_STYLE,st);
		} else if((!scrlInfo.nMax) && bHorScrl)
		{	LONG st = GetWindowLong(wnd,GWL_STYLE);
			st &= ~WS_HSCROLL; bHorScrl = FALSE;
			SetWindowLong(wnd,GWL_STYLE,st);
		}*/
		GetScrollInfo(wnd, SB_HORZ, &scrlInfo);

		ScrollOffst = - scrlInfo.nPos;

		// If the position has changed, scroll the window 
		//if(scrlInfo.nPos != xOldScrlPos)
			Render();//InvalidateRect(wnd,NULL,TRUE);

		if(hEditItem)//Scroll editItem, imenno shunday bo'lishi shart;
		{RECT *rc = iEditItem->GetRectPtr(this,FALSE);
		 SetWindowPos(hEditItem,HWND_TOP,//MoveWindow(hEditItem,rc.left+XBITMAP,rc.top-3,itemMaxWidth-XBITMAP,rc.bottom-rc.top+6,TRUE);
					  rc->left+XBITMAP,rc->top-3,
					  itemMaxWidth-XBITMAP,
					  rc->bottom-rc->top+6,
					  SWP_DRAWFRAME|SWP_SHOWWINDOW);
		 RedrawWindow(hEditItem,NULL,NULL,RDW_FRAME|RDW_INVALIDATE);
		 ::SetFocus(hEditItem);
		}
		return 0;
	case WM_VSCROLL:
		//profilerOutStr("Beg of Panel::WndProc WM_VSCROLL")
		scrlInfo.fMask = SIF_ALL;
		// Save the position for comparison later on
		GetScrollInfo(wnd, SB_VERT, &scrlInfo);
		xOldScrlPos = scrlInfo.nPos;
		switch (LOWORD (wParam))
		{	// user clicked left arrow
			case SB_LINEUP:
				if(0==scrlLineSpeedTick)
				{scrlLineSpeedTick=GetTickCount();
				 scrlInfo.nPos -= (int)(lParam?lParam:1);
				}
				else
				{DWORD dt;dt=GetTickCount()-scrlLineSpeedTick;
				 if(dt<250)
				  scrlInfo.nPos -= (int)(lParam?lParam:1);
				 else if(dt<5000)
				  scrlInfo.nPos -= (dt/250)*(int)(lParam?lParam:1);
				 else
				  scrlInfo.nPos -= 20*(int)(lParam?lParam:1);
				}
				scrlBtnPressedDir=1;//Up;
				break;
		 	// user clicked right arrow
			case SB_LINEDOWN:
				if(0==scrlLineSpeedTick)
				{scrlLineSpeedTick=GetTickCount();
				 scrlInfo.nPos += (int)(lParam?lParam:1);
				}
				else
				{dt=GetTickCount()-scrlLineSpeedTick;
				 if(dt<250)
				  scrlInfo.nPos += (int)(lParam?lParam:1);
				 if(dt<5000)
				  scrlInfo.nPos += (dt/250)*(int)(lParam?lParam:1);
				 else
				  scrlInfo.nPos += 20*(int)(lParam?lParam:1);
				}
				scrlBtnPressedDir=2;//Down;
				break;
		 	// user clicked the scroll bar shaft left of the scroll box
			case SB_PAGEUP:
				scrlInfo.nPos -= (rcClient.bottom-itemHeight) * (int)(lParam?lParam:1);//1;
				scrlBtnPressedDir=1;
				break;
		 	// user clicked the scroll bar shaft right of the scroll box
			case SB_PAGEDOWN:
				scrlInfo.nPos += (rcClient.bottom-itemHeight) * (int)(lParam?lParam:1);//1;
				scrlBtnPressedDir=2;
				break;
		 	// user dragged the scroll box
			case SB_THUMBTRACK: 
				scrlInfo.nPos = scrlInfo.nTrackPos;
				scrlBtnPressedDir=1;
				break;
			case SB_ENDSCROLL:
				scrlLineSpeedTick = 0;
				if(scrlBtnPressedDir==1)
					SetTimer(hWnd,IDPANELTIMER_VERTSCRLBTNUP,50,NULL);
				else if(scrlBtnPressedDir==2)
					SetTimer(hWnd,IDPANELTIMER_VERTSCRLBTNDOWN,50,NULL);
				scrlBtnPressedDir=0;
				return 0;
		 	default :
				break;
		}
		// Set the position and then retrieve it.  Due to adjustments
		//   by Windows it may not be the same as the value set.
		scrlInfo.fMask = SIF_POS;//|SIF_RANGE;
		SetScrollInfo(wnd, SB_VERT, &scrlInfo, TRUE);
		GetScrollInfo(wnd, SB_VERT, &scrlInfo);

		ScrollOffst = - scrlInfo.nPos;

		// If the position has changed, scroll the window 
		//if(scrlInfo.nPos != xOldScrlPos)
			Render();//InvalidateRect(wnd,NULL,TRUE);

		if(hEditItem)//Scroll editItem, imenno shunday bo'lishi shart;
		{RECT *rc = iEditItem->GetRectPtr(this,FALSE);
		 SetWindowPos(hEditItem,HWND_TOP,//MoveWindow(hEditItem,rc.left+XBITMAP,rc.top-3,itemMaxWidth-XBITMAP,rc.bottom-rc.top+6,TRUE);
					  rc->left+XBITMAP,rc->top-3,
					  width-XBITMAP,
					  rc->bottom-rc->top+6,
					  SWP_DRAWFRAME|SWP_SHOWWINDOW);
		 RedrawWindow(hEditItem,NULL,NULL,RDW_FRAME|RDW_INVALIDATE);
		 ::SetFocus(hEditItem);
		}
		return 0;
	case WM_SIZE:
		rcClient.right = LOWORD(lParam);
		rcClient.bottom = HIWORD(lParam);
		width = rcClient.right;
		height = rcClient.bottom;
		imgBckgrnd[1].CloneOnWM_SIZE(this,&imgBckgrnd[0]);
		/*SCROLLBARINFO scbi;
		if(GetScrollBarInfo(hWnd,OBJIDM_HSCROLL,&scbi))
		{	if(scbi.rcScrollBar.bottom - scbi.rcScrollBar.top>0)
				hscrollHeight = scbi.rcScrollBar.bottom - scbi.rcScrollBar.top;
		}*/
		if(virtualPanel==entry.GetCrntRecType())
			vrtlPanels::EventPanelResizing$8(hWnd,iVPPlg,VPPlgObj);
		return 0;
	case WM_MOVE:
		/*if(bDrgDrpLDwn)
		{	IDataObject *pDataObject;
			IDropSource *pDropSource;
			DWORD		 dwEffect;
			DWORD		 dwResult;

			//FORMATETC fmtetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
			//STGMEDIUM stgmed = { TYMED_HGLOBAL, { 0 }, 0 };

			// transfer the current selection into the IDataObject
			//stgmed.hGlobal = CopySelection(hwndEdit);

			// Create IDataObject and IDropSource COM objects
			pDropSource = new DropSource();//CreateDropSource(&pDropSource);
			pDataObject = new DragDataObject(this);//CreateDataObject(&fmtetc, &stgmed, 1, &pDataObject);

			//
			//	** ** ** The drag-drop operation starts here! ** ** **
			//
			dwResult = DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY|DROPEFFECT_MOVE, &dwEffect);

			// success!
			if(dwResult == DRAGDROP_S_DROP)
			{	if(dwEffect & DROPEFFECT_MOVE)
				{
					// remove selection from edit control
				}
			}
			// cancelled
			else if(dwResult == DRAGDROP_S_CANCEL)
			{
			}

			pDataObject->Release();
			pDropSource->Release();

			ReleaseCapture();
			bDrgDrpLDwn = FALSE;
			bDidDrgDrp = TRUE;
		}*/

		if(virtualPanel==entry.GetCrntRecType())
			vrtlPanels::EventPanelResizing$8(hWnd,iVPPlg,VPPlgObj);
		return 0;
	case WM_MOUSEWHEEL:
		__int16 wl; wl = (__int16)HIWORD(wParam);
		if(0==bItemsViewType)
			::PostMessage(wnd,WM_HSCROLL,(wl>0)?SB_PAGELEFT:SB_PAGERIGHT,1);
		else
			::PostMessage(wnd,WM_VSCROLL,(wl>0)?SB_PAGEUP:SB_PAGEDOWN,1);
		return 0;
	case WM_MOUSEMOVE:
		BtnsPanel::Hide();
		//if(0==bInput) return 0;
		POINT p; p.x = (__int16)LOWORD(lParam); p.y = (__int16)HIWORD(lParam);
		if(bActivate)
		{	if(p.x<10 || p.x>rcClient.right-15 || p.y<10 || p.y>rcClient.bottom-25)
			{	toolTip.Hide();
				//ReleaseCapture();
				bMouseCapture=0;
			}
			else
			{	//if(0==bMouseCapture){SetCapture(hWnd);bMouseCapture=TRUE;}
				int id; id = GetItemNum(p.x,p.y);
				if(id<1)
					toolTip.Hide();
				else
				{	if(1!=panel[iOpponent].bQView)
						toolTip.TrackPosition(p.x+10,p.y+15,
					(idForTooltip == id)?NULL:GetStrForTooltip(id),TRUE,TRUE);
					idForTooltip = id;
		}	}	}
		if(iDrgDwn > -1 && MK_LBUTTON == wParam && (0==bDrag))
		{	ClientToScreen(hWnd,&p);
			if(hWnd!=WindowFromPoint(p))
			{	EnterDragFiles(iDrgDwn);
				ReleaseCapture();
				iDrgDwn = -1;
		}	}
		return 0;
	case WM_LBUTTONUP:
		//OutputDebugString(L"\nWM_LBUTTONUP");
		iDrgDwn = -1;
		ReleaseCapture();
		return 0;
	case WM_LBUTTONDOWN:
		//OutputDebugString(L"\nWM_LBUTTONDOWN");
	//wchar_t s[260];static int i;
	//wsprintf(s,L"\n%d iActivePanel:%d Panel-%d WM_LBUTTONDOWN",i++,iActivePanel,iThis);
	//OutputDebugString(s);
		if(hQuickSearchEdit)
			CancelQuickSearchMode(FALSE);
		else if(hDiskCB)
			CancelDiskCBMode();
		toolTip.Hide();
		if(0==bInput) return 0;
		//if(lParam!=-1)//TAB bosilmagan bo'lsa;
		//	if(wParam!=-1)
		//		if(timeForDblClicking)
		//			if(tickProced-timeForDblClicking < 500)
		//				return 0;
		//timeForDblClicking = tickProced;
		iDrgDwn=-1;
		if(MK_LBUTTON & wParam)
		{	__int16 x = (__int16)LOWORD(lParam); __int16 y = (__int16)HIWORD(lParam);
			iDrgDwn = GetItemNum(x,y);
			if(-1==iDrgDwn)
			{	iDrgDwn = iHot;
				return 0;
			}
			if(hEditItem && iEditItem != &pItems[iDrgDwn])
				CancelEditItemMode(FALSE);
		}
		if(0==bMouseCapture){SetCapture(hWnd);bMouseCapture=1;}
		if(MK_LBUTTON & wParam)
		{	if(MK_SHIFT & wParam)
				OnMouseShftClick(iDrgDwn);
			else if(MK_CONTROL & wParam)
				OnMouseCtrlClick(iDrgDwn);
			else OnMouseClick(iDrgDwn);
			iHot = iDrgDwn;
		}
		//ShowInfo(0,450);
		return 0;
	case WM_TIMER:
		switch(wParam)
		{	case IDPANELTIMER_SCRLMOUSE://LBUTTONDOWN timer:
				ScrollItemToView(iHot);
				KillTimer(hWnd,IDPANELTIMER_SCRLMOUSE);
				return 0;
			case IDPANELTIMER_CHANGEBCKIMG:
				imgBckgrnd[0].OnTimer(this);
				return 0;
			case IDPANELTIMER_VERTSCRLBTNUP:
				int md;md = scrlInfo.nPos%itemHeight;
				scrlLineSpeedTick=0;
				if(md==0)
					KillTimer(hWnd,IDPANELTIMER_VERTSCRLBTNUP);
				else
				{if(1==md%2)
				  md=1;
				 else md=2;
				 ::SendMessage(wnd,WM_VSCROLL,SB_LINEUP,md);
				}
				return 0;
			case IDPANELTIMER_VERTSCRLBTNDOWN:
				md = scrlInfo.nPos%itemHeight;
				scrlLineSpeedTick=0;
				if(md==0)
					KillTimer(hWnd,IDPANELTIMER_VERTSCRLBTNDOWN);
				else
				{if(1==(itemHeight-md)%2)
				  md=1;
				 else md=2;
				 ::SendMessage(wnd,WM_VSCROLL,SB_LINEDOWN,md);
				}
				return 0;
			case IDPANELTIMER_HORSCRLBTNLEFT:
				md = scrlInfo.nPos%itemMaxWidth;
				scrlLineSpeedTick=0;
				if(md==0)
					KillTimer(hWnd,IDPANELTIMER_HORSCRLBTNLEFT);
				else
				{ if(itemMaxWidth<60)md=1;
				  else if(md>itemMaxWidth-45)
				  {	int d=itemMaxWidth-md;
					if(d<3) md=1;
					else if(d<6) md=2;
					else if(d<14) md=4;
					else if(d<22) md=8;
					else if(d<36) md=16;
					else md=24;
				  }
				  else if(md>38) md=24;
				  else if(md>22) md=16;
				  else if(md>14) md=8;
				  else if(md>6) md=2;
				  else md=1;
				 ::SendMessage(wnd,WM_HSCROLL,SB_LINELEFT,md);
				}
				return 0;
			case IDPANELTIMER_HORSCRLBTNRIGHT:
				md = scrlInfo.nPos%itemMaxWidth;
				scrlLineSpeedTick=0;
				if(md==0)
					KillTimer(hWnd,IDPANELTIMER_HORSCRLBTNRIGHT);
				else
				{ if(itemMaxWidth<60)md=1;
				  else if(md<5)md=2;
				  else if(md<10)md=4;
				  else if(md<18)md=8;
				  else if(md<24)md=16;
				  else if(md<32)md=24;
				  else
				  { int d=itemMaxWidth-md;
				    if(d<10)//oxiri:
				    { if(d>3)md=2;
				       else md=1;
				    }
				    else if(d<16) md=4;
				    else if(d<24) md=8;
				    else if(d<32) md=16;
				    else if(d<42) md=20;
				    else md=24;
				  }
				 ::SendMessage(wnd,WM_HSCROLL,SB_LINERIGHT,md);
				}
				return 0;
		}
		return 0;
	case WM_RBUTTONDOWN:
		if(0==bInput) return 0;
		//profilerOutStr("\nBeg of Panel::WndProc WM_RBUTTONDOWN")
		if(MK_RBUTTON & wParam)
		{	int x = LOWORD(lParam); int y = HIWORD(lParam);
			int id = GetItemNum(x,y);
			if(-1==id)
			{	id = iHot;
				if(id<0)id=0;
				return 0;
			}
			OnMouseRClick(x,y,id);
			iHot = id;
		}
		return 0;
	case WM_LBUTTONDBLCLK:
		if(0==bInput) return 0;
		KillTimer(hWnd,IDPANELTIMER_SCRLMOUSE);
		if(MK_LBUTTON & wParam)
		{	int x = LOWORD(lParam); int y = HIWORD(lParam);
			int id = GetItemNum(x,y);
			if(0==id)
				fManager.FolderUp(id);
			else
			{	if(!fManager.FolderIn(id))
				{	
		}	}	}
		return 0;
	case WM_KEYDOWN:
		if(0==bInput) return 0;
		//if(9==wParam)//TAB
		//{	PostMessage(::wnd, WM_KEYDOWN, 9, 0);
		//	profilerOutStr("\nWM_KEYDOWN fr. pan.WNDPROC")
		//	return 0;
		//}
		//static int i;
		//wsprintf(s,L"\n%d OnKeybPressed, wParam: %d lParam: %d",i++,wParam,lParam);
		//OutputDebugString(s);
		if(OnKeybPressed(wParam, lParam)) return 0;
		return DefWindowProc(wnd, message, wParam, lParam);
	case WM_SYSKEYDOWN:
		if(0==bInput) return 0;
		if(VK_MENU==wParam)
			return DefWindowProc(wnd, message, wParam, lParam);
		else
		{	OnKeybPressed(wParam, lParam);
			return 0;
		}
	case WM_USER://FindFirstChangeNotification dan
		switch(wParam)
		{	//case 1://FirstChangeNotifications:
			//	OnDirectoryChangeNotify();
			//	break;
			case 2://Select item(masalan CancelEditItem dan so'ng);
				if((int)lParam > -1 && (int)lParam < totItems)
				{	DeselectAllWithout(-1);
					AddToSelection((int)lParam);
					Render(NULL);
				}
				break;
		}
		return 0;
	case WM_USER+1://Change panel font,fr.ChooseFontDlg;
		return 0;
	case WM_USER+2://From size calcing tread:
		if(forSizeUp==folderSortType || forSizeDwn==folderSortType)
		if(pItems)
		{	findItemName[0]=0;
			if(iHot>0 && iHot<totItems-1)
				MyStringCpy(findItemName,MAX_PATH-1,pItems[iHot].Name);
			Sort();
			if(findItemName[0])
				SetHot(FindItem(findItemName));
			Render();
		}
		return 0;
	case WM_USER+3://From size calcing tread,set item size:
		if(0==wcscmp(path,FolderSize::pth))
		if(totItems>(int)wParam)
			pItems[(int)wParam].size = *((unsigned __int64*)lParam);
		return 0;
	case MW_RECV_SERVER://MW_RECV_SERVER from direct link socket WM_USER+20
		if(!linkSock::ReceiveFrClientMsg(iThis,wParam,lParam))
			DestroyWindow(linkSock::sockDlg[iThis]);
		return 0;
	case MW_RECV_CLIENT://MW_RECV_CLIENT from direct link socket WM_USER+21
		if(!linkSock::ReceiveFrServerMsg(iThis,wParam,lParam))
			DestroyWindow(linkSock::sockDlg[iThis]);
		return 0;
	case WM_DESTROY:
		Destroy();
		//DragAcceptFiles(hWnd, FALSE);
		return 0;
	case WM_DEVICECHANGE:
		if(myWMI::OnDeviceChange(wnd,wParam, lParam))
			return 0;
		return DefWindowProc(wnd, message, wParam, lParam);
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDM_FILE_START:
				DestroyMenu(fileMenuPopup.hMenu);
				return 0;
			case IDM_FILE_PROPERTIES://popupMenudan;
				wchar_t ws[MAX_PATH];
				GetFullPathAndName(fileMenuPopup.idFile,ws,MAX_PATH);
				int ln; ln = MyStringLength(ws,MAX_PATH);
				SHObjectProperties(wnd,SHOP_FILEPATH,ws,NULL);
				return 0;
			case IDM_FILE_STREAM:
				if(conf::winVerMajor>5)
					fileStream::EnumFileStreams(wnd,GetFullPathAndName(fileMenuPopup.idFile));
				return 0;
			case IDM_EDIT_COPYSELECTIONASTEXT:
			//case IDM_EDIT_COPYSELECTIONASTEXT_APPENDMENU:
				SetSelectedItemsListToClipboard(FALSE);
				return 0;
			case IDM_EDIT_COPYALLITEMSASTEXT:
				SetAllItemsListToClipboard(FALSE);
				return 0;
			case IDM_EDIT_COPYALLITEMSFULLASTEXT:
				SetAllItemsListToClipboard(TRUE);
				return 0;
			case IDM_EDIT_COPYSELECTIONFULLASTEXT:
				SetSelectedItemsListToClipboard(TRUE);
				return 0;
			case IDM_FILE_EXIT:
				DestroyMenu(fileMenuPopup.hMenu);
				//PostMessage(wnd,WM_CLOSE,0,0);
				PostQuitMessage(0);
				return 0;
			case IDM_EDIT_CUT:
				ExecuteHotKey(53);
				return 0;
			case IDM_EDIT_COPY:
				ExecuteHotKey(52);
				return 0;
			case IDM_EDIT_PAST:
				ExecuteHotKey(54);
				return 0;
			case ID_EDIT_APPENDTOCLIPBOARDCOPYLIST:
				ExecuteHotKey(55);
				return 0;
			case ID_EDIT_APPENDTOCLIPBOARDCUTLIST:
				ExecuteHotKey(56);
				return 0;
			case IDM_EDIT_DELETE:
				if(fDelOper::Delete(this,0))
					DeselectAllWithout(-1);
				return 0;
 			case IDM_EDIT_SELECTALL:
				Select(0,0);return 0;
			case IDM_SELECTALL_FOLDERS:
				Select(0,1);return 0;
			case IDM_SELECTALL_FILES:
				Select(0,2);return 0;
			case IDM_SELECTFROMNAME_ALL:
				Select(0,3);return 0;
			case IDM_SELECTFROMNAME_FOLDERS:
				Select(0,4);return 0;
			case IDM_SELECTFROMNAME_FILES:
				Select(0,5);return 0;
			case IDM_SELECTFROMEXTENSION_ALL:
				Select(0,6);return 0;
			case IDM_SELECTFROMEXTENSION_FOLDERS:
				Select(0,7);return 0;
			case IDM_SELECTFROMEXTENSION_FILES:
				Select(0,8);return 0;
			case IDM_SAVESELECTION_ALL:
				Select(3,0);return 0;
			case IDM_SAVESELECTION_FOLDERS:
				Select(3,1);return 0;
			case IDM_SAVESELECTION_FILES:
				Select(3,2);return 0;
			case IDM_EDIT_RESTORESELECTION:
				Select(4,0);return 0;
			case IDM_UNSELECTALL_ALL:
				Select(1,0);return 0;
			case IDM_UNSELECTALL_FOLDERS:
				Select(1,1);return 0;
			case IDM_UNSELECTALL_FILES:
				Select(1,2);return 0;
			case IDM_UNSELECTFROMNAME_ALL:
				Select(1,3);return 0;
			case IDM_UNSELECTFROMNAME_FOLDERS:
				Select(1,4);return 0;
			case IDM_UNSELECTFROMNAME_FILES:
				Select(1,5);return 0;
			case IDM_UNSELECTFROMEXTENSION_ALL:
				Select(1,6);return 0;
			case IDM_UNSELECTFROMEXTENSION_FOLDERS:
				Select(1,7);return 0;
			case IDM_UNSELECTFROMEXTENSION_FILES:
				Select(1,8);return 0;
			case IDM_INVERTSELECTION_ALL:
				Select(2,0);return 0;
			case IDM_INVERTSELECTION_FOLDERS:
				Select(2,1);return 0;
			case IDM_INVERTSELECTION_FILES:
				Select(2,2);return 0;
			case IDM_GUIDFOLDERS_DESKTOP://Guid folders:
				AllocGuidFolder(CSIDL_DESKTOP);return 0;
			case IDM_GUIDFOLDERS_DESKTOPDIRECTORY:
				AllocGuidFolder(CSIDL_DESKTOPDIRECTORY);return 0;
			case IDM_GUIDFOLDERS_COMMONDESKTOPDIRECTORY:
				AllocGuidFolder(CSIDL_COMMON_DESKTOPDIRECTORY);return 0;
			case IDM_GUIDFOLDERS_COMMONDOCUMENTS:
				AllocGuidFolder(CSIDL_COMMON_DOCUMENTS);return 0;
			case IDM_GUIDFOLDERS_MYDOCUMENTS:
				AllocGuidFolder(CSIDL_PERSONAL);return 0;
			case IDM_GUIDFOLDERS_COMMONADMINTOOLS:
				AllocGuidFolder(CSIDL_COMMON_ADMINTOOLS);return 0;
			case IDM_GUIDFOLDERS_ADMINTOOLS:
				AllocGuidFolder(CSIDL_ADMINTOOLS);return 0;
			case IDM_GUIDFOLDERS_COMMONLOCALSTARTUP:
				AllocGuidFolder(CSIDL_COMMON_ALTSTARTUP);return 0;
			case IDM_GUIDFOLDERS_LOCALSTARTUP:
				AllocGuidFolder(CSIDL_STARTUP);return 0;
			case IDM_GUIDFOLDERS_COMMONSTARTUP:
				AllocGuidFolder(CSIDL_COMMON_STARTUP);return 0;
			case IDM_GUIDFOLDERS_NONLOCALSTARTUP:
				AllocGuidFolder(CSIDL_ALTSTARTUP);return 0;
			case IDM_GUIDFOLDERS_COMMONAPPLICATIONDATA:
				AllocGuidFolder(CSIDL_COMMON_APPDATA);return 0;
			case IDM_GUIDFOLDERS_APPLICATIONDATA:
				AllocGuidFolder(CSIDL_APPDATA);return 0;
			case IDM_GUIDFOLDERS_FAVORITES:
				AllocGuidFolder(CSIDL_COMMON_FAVORITES);return 0;
			case IDM_GUIDFOLDERS_COMMONPROGRAMS:
				AllocGuidFolder(CSIDL_COMMON_PROGRAMS);return 0;
			case IDM_GUIDFOLDERS_PROGRAMS:
				AllocGuidFolder(CSIDL_PROGRAMS);return 0;
			case IDM_GUIDFOLDERS_COMMONSTART:
				AllocGuidFolder(CSIDL_COMMON_STARTMENU);return 0;
			case IDM_GUIDFOLDERS_START:
				AllocGuidFolder(CSIDL_STARTMENU);return 0;
			case IDM_GUIDFOLDERS_TEMPLATES:
				AllocGuidFolder(CSIDL_TEMPLATES);return 0;
			case IDM_GUIDFOLDERS_COMMONTEMPLATE:
				AllocGuidFolder(CSIDL_COMMON_TEMPLATES);return 0;
			case IDM_GUIDFOLDERS_COOKIES:
				AllocGuidFolder(CSIDL_COOKIES);return 0;
			case IDM_GUIDFOLDERS_FONTS:
				AllocGuidFolder(CSIDL_FONTS);return 0;
			case IDM_GUIDFOLDERS_HISTORY:
				AllocGuidFolder(CSIDL_HISTORY);return 0;
			case IDM_GUIDFOLDERS_INTERNETCASHE:
				AllocGuidFolder(CSIDL_INTERNET_CACHE);return 0;
			case IDM_GUIDFOLDERS_LOCALAPPLICATIONDATA:
				AllocGuidFolder(CSIDL_LOCAL_APPDATA);return 0;
			case IDM_GUIDFOLDERS_MYPICTURES:
				AllocGuidFolder(CSIDL_MYPICTURES);return 0;
			case IDM_GUIDFOLDERS_NETHOOD:
				AllocGuidFolder(CSIDL_NETHOOD);return 0;
			case IDM_GUIDFOLDERS_PRINTHOOD:
				AllocGuidFolder(CSIDL_PRINTHOOD);return 0;
			case IDM_GUIDFOLDERS_PROFILE:
				AllocGuidFolder(CSIDL_PROFILE);return 0;
			case IDM_GUIDFOLDERS_PROGRAM_FILES:
				AllocGuidFolder(CSIDL_PROGRAM_FILES);return 0;
			case IDM_GUIDFOLDERS_COMMONPROGRAMFILES:
				AllocGuidFolder(CSIDL_PROGRAM_FILES_COMMON);return 0;
			case IDM_GUIDFOLDERS_RECENT:
				AllocGuidFolder(CSIDL_RECENT);return 0;
			case IDM_GUIDFOLDERS_SENDTO:
				AllocGuidFolder(CSIDL_SENDTO);return 0;
			case IDM_GUIDFOLDERS_SYSTEM:
				AllocGuidFolder(CSIDL_SYSTEM);return 0;
			case IDM_GUIDFOLDERS_WINDOWS:
				AllocGuidFolder(CSIDL_WINDOWS);return 0;
			case IDM_VIRTUALGUIDFOLDERS_MYCOMPUTER:
				AllocVirtualGuidFolder(CSIDL_DRIVES);return 0;
			case IDM_VIRTUALGUIDFOLDERS_DESKTOP:
				AllocVirtualGuidFolder(0);return 0;
			case IDM_VIRTUALGUIDFOLDERS_CONTROLPANEL:
				AllocVirtualGuidFolder(CSIDL_CONTROLS);return 0;
			case IDM_VIRTUALGUIDFOLDERS_NETWORK:
				AllocVirtualGuidFolder(CSIDL_NETWORK);return 0;
			case IDM_VIRTUALGUIDFOLDERS_RECYCLEBIN:
				AllocVirtualGuidFolder(CSIDL_BITBUCKET);return 0;
			case IDM_VIRTUALGUIDFOLDERS_PRINTERS:
				AllocVirtualGuidFolder(CSIDL_PRINTERS);return 0;
			case 1003://IDM_CBDISK =  Alohida Ctrl+F2, global disk toolbardan farqli!!!
				if(CBN_DBLCLK==HIWORD(wParam) || CBN_SELENDOK==HIWORD(wParam))
				{	int crnt=(int)::SendMessage(hDiskCB,CB_GETCURSEL,1003,0);
					if(CB_ERR!=crnt)
						OnToolbarDiskBtnClicked(iThis,crnt);
					CancelDiskCBMode();
				} else if(CBN_CLOSEUP==HIWORD(wParam))
					CancelDiskCBMode();
				return 0;
		}
		return 0;
	//case WM_DROPFILES:HDROP hDrop;hDrop = (HDROP)wParam; 
	//	OutputDebugString(L"WM_DROPFILES from panel");
	//	int iFiles; iFiles = DragQueryFile(hDrop, (DWORD)(-1), (LPWSTR)NULL, 0);
	//	DragFinish(hDrop);
	//	break;
	default:
		return DefWindowProc(wnd, message, wParam, lParam);
	}
	return DefWindowProc(wnd, message, wParam, lParam);
}

BOOL Panel::WriteToDiskFrPathAndNameCombo(int iMax)
{
	if(0==iThis)
		pnlPthAndNamesCB[0].Save(pnlPthAndNamesCB0FName,pathAndNameEdt[0],iMax);
	else if(1==iThis)
		pnlPthAndNamesCB[1].Save(pnlPthAndNamesCB1FName,pathAndNameEdt[0],iMax);
	else if(2==iThis)
		pnlPthAndNamesCB[2].Save(pnlPthAndNamesCB2FName,pathAndNameEdt[0],iMax);
	else// if(3==iThis)
		pnlPthAndNamesCB[3].Save(pnlPthAndNamesCB3FName,pathAndNameEdt[0],iMax);
	return TRUE;
}

VOID Panel::My3StrCpyBoundWidth(wchar_t *in, wchar_t* out, int bndWidth,
								int fsrtPos, int scndPos,
								int fsrtBndPos, int scndBndPos)
{
wchar_t *pIn = in; int p = 0, cx = 0;

	while(*pIn!=0)
	{
		if(' ' == *pIn)
			cx += tm.tmBreakChar;
		else
			cx += charsExts[*pIn];

		cx += charsExtra;

		out[p++] = *pIn++;

		if(p==fsrtPos && cx<fsrtBndPos)
		{	while(cx<fsrtBndPos)
			{	out[p++] = ' ';
				cx += charsExts[' '];
		}	}
		else if(p<fsrtPos && cx>fsrtBndPos) pIn = in+fsrtPos;

		if(pIn==in+scndPos)
		if(cx<scndBndPos)
		{	while(cx<scndBndPos)
			{	out[p++] = ' ';
				cx += charsExts[' '];
	}	}	}
	out[p]=0;
}

//Faqat Panel::AddItemda ishlataman::
int Panel::MyGetTextExtentPoint32(wchar_t* lpString, int cnt)
{
int i,cx = 0;// unsigned char CH;
//#define CH (unsigned char)lpString[i]
#define CH (int)(lpString[i])
	for(i=0; i<cnt; i++)
	{	//if(' ' == CH)
		//	cx += tm.tmBreakChar;
		//else
			cx += charsExts[CH];
		//cx += charsExtra;
	}
	//if(i>1) cx -= charsExtra;
	return cx;
#undef CH
}

int Panel::MyGetMaxWidthTextExtentPoint32(wchar_t* lpString, int cnt, int maxWidth, int *Cnt)
{
int i,cx = 0;// unsigned char CH;
//#define CH (unsigned char)lpString[i]
#define CH lpString[i]
	for(i=0; i<cnt; i++)
	{	//if(' ' == CH)
		//	cx += tm.tmBreakChar;
		//else
			cx += charsExts[CH];
		//cx += charsExtra;
		if(cx>maxWidth){*Cnt = i; return cx;}
	}
	//if(i>1) cx -= charsExtra;
	*Cnt = cnt;
	return cx;
#undef CH
}

VOID Panel::ResetGetTextExtentPoint32()
{
HDC dc = GetDC();
	HGDIOBJ oldO;
	if(hf)
		oldO = SelectObject(dc,hf);
	charsExtra = GetTextCharacterExtra(dc);
	GetTextMetrics(dc, &tm);
	GetCharWidth32(dc,0,0xffff,charsExts);
	if(hf)
		SelectObject(dc,oldO);
	SetGraphicsMode(dc,GM_ADVANCED);
	ReleaseDC(dc);
}

VOID Panel::OnSetFocus()
{	if(1==panel[iAntiOpponent].bQView)
		return;
	if(hWnd)
	{	if(iHot<0)
		if(iOldHot!=iHot)
		if(iOldHot>-1)
		if(iOldHot<totItems)
			iHot=iOldHot;
		if(iActivePanel != iThis)
		{	int oldAct=iActivePanel>-1?iActivePanel:0;
			iActivePanel = iThis;
			if(panel[oldAct].iHot>-1)
			if(panel[oldAct].iHot<panel[oldAct].totItems)
				panel[oldAct].RenderItem(panel[oldAct].iHot);
			panel[iActivePanel].ChangeViewMenu(0);
			panel[iActivePanel].ChangeViewMenu(1);
		}
		if(hEditItem)
		{	iEditItem->Render(0,this);
			::SetFocus(hEditItem);
			RedrawWindow(hEditItem,NULL,NULL,RDW_FRAME|RDW_INVALIDATE);
		}
		else
		{	if(iHot>-1)
			if(iHot<totItems)
				RenderItem(iHot);
	}	}
	//wchar_t s[260];static int i=0;
	//wsprintf(s,L"\n%d iActivePanel:%d Panel-%d OnSetFocus",i++,iActivePanel,iThis);
	//wsprintf(s,L"\n%d panel: %d, iHot: %d",i++,iThis,iHot);
	//OutputDebugString(s);
}


#undef XBITMAP
