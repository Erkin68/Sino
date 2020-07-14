// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "SinoHlp32.h"

#include "SinoHlp32Doc.h"
#include "LeftView.h"
#include "MainFrm.h"
#include "SinoHlp32View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLeftView *listView=NULL;
ListType listType=contents;


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT;
	BOOL r = CTreeView::PreCreateWindow(cs);
	return r;
}

/*BOOL CLeftView::OnNotify(WPARAM wParam,LPARAM lParam,LRESULT *pResult)
{
	NMHDR *pnmh = (LPNMHDR)lParam;
/*	if(TVN_KEYDOWN==pnmh->code)
	{	NMTREEVIEW *pnmtv = (NMTREEVIEW*)pResult;
		TVITEM tvi;ZeroMemory(&tvi,sizeof(tvi));
		tvi.hItem = pnmtv->itemOld.hItem;
		tvi.mask = TVIF_TEXT;
		char s[128]="";
		LPSTR ps = &s[0];
		tvi.pszText=ps;
		tvi.cchTextMax=128;
		GetTreeCtrl().GetItem(&tvi);
	}
	if(lParam==0x21f684)
	{	static int i=0;char s[260];sprintf(s,"\n%d %x %x",i++,pnmh->code,wParam);
		OutputDebugString(s);
	}
	return TRUE;
}*/

/*BOOL CLeftView::OnCmdMsg(UINT nID,int nCode,void* pExtra,AFX_CMDHANDLERINFO *pHandlerInfo)
{
static int i=0;char s[260];sprintf(s,"\n%d %x %x %x %x",i++,nID,nCode,pExtra,pHandlerInfo);
	OutputDebugString(s);
	if(nID==0xe140)
		nID=nID;
	if(nID==0xe900)
		nID=nID;
	if(nID==0xe701)
		nID=nID;
	if(nID==0xe702)
		nID=nID;
	if(nID==0xe703)
		nID=nID;
	if(nID==0x3)
		nID=nID;
	if(nID==0x4)
		nID=nID;
	if(nID==0x5)
		nID=nID;
	if(nID==0xe100)
		nID=nID;
	if(nID==0xe101)
		nID=nID;
	if(nID==0xe103)
		nID=nID;
	if(nID==0xe123)
		nID=nID;
	if(nID==0xe122)
		nID=nID;
	if(nID==0xe125)
		nID=nID;
	if(nID==0xe107)
		nID=nID;
	return TRUE;
}*/

void CLeftView::OnSelChanged(NMHDR *pNMHDR,LRESULT *plResult)
{
	if(!htmlView)return;
	if(htmlView->beforeNavigateType == htmlView->selectListItem)return;
	switch(listType)
	{	case contents:
			OnContentsSelChanged(pNMHDR,plResult);
			break;
		case index:
			OnIndexSelChanged(pNMHDR,plResult);
			break;
		case search:
			OnSearchSelChanged(pNMHDR,plResult);
			break;
}	}

void CLeftView::OnContentsSelChanged(NMHDR *pNMHDR,LRESULT *plResult)
{
	htmlView->beforeNavigateType = htmlView->fromContentsList;
	if(pNMHDR->code==TVN_SELCHANGED)
	{	TVITEMA tvi;ZeroMemory(&tvi,sizeof(tvi));
		tvi.mask = TVIF_TEXT;
		TCHAR s[128]="";
		tvi.pszText=&s[0];
		tvi.cchTextMax=128;
		HWND h = pNMHDR->hwndFrom;
		tvi.hItem = TreeView_GetSelection(pNMHDR->hwndFrom);
		TreeView_GetItem(h,&tvi);
		if(!strcmp(s,htmlView->GetHTMLName(0,0)))
			htmlView->LoadFromResource(IDR_HTML_MAIN);
		else if(!strcmp(s,htmlView->GetHTMLName(0,1)))
			htmlView->LoadFromResource(IDR_HTML_SEL_SOZDANIYA);
		else if(!strcmp(s,htmlView->GetHTMLName(0,2)))
			htmlView->LoadFromResource(IDR_HTML_CHEM_OTLICHAYETCYA);
		else if(!strcmp(s,htmlView->GetHTMLName(0,3)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE);
		else if(!strcmp(s,htmlView->GetHTMLName(0,4)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_7ZIP);
		else if(!strcmp(s,htmlView->GetHTMLName(0,5)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_CHM);
		else if(!strcmp(s,htmlView->GetHTMLName(0,6)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_JPG);
		else if(!strcmp(s,htmlView->GetHTMLName(0,7)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_MS);
		else if(!strcmp(s,htmlView->GetHTMLName(0,8)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_PNG);
		else if(!strcmp(s,htmlView->GetHTMLName(0,9)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_UNRAR);
		else if(!strcmp(s,htmlView->GetHTMLName(0,10)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_ZLIB);
		else if(!strcmp(s,htmlView->GetHTMLName(0,11)))
			htmlView->LoadFromResource(IDR_HTML_KRATKAYA_INFO);
		else if(!strcmp(s,htmlView->GetHTMLName(0,12)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_UDEFRAG);
		else if(!strcmp(s,htmlView->GetHTMLName(0,13)))
			htmlView->LoadFromResource(IDR_HTML_GNU_LICENSE);
		else if(!strcmp(s,htmlView->GetHTMLName(0,14)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_WINPCAP);
		else if(!strcmp(s,htmlView->GetHTMLName(0,15)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_SINO);
		else if(!strcmp(s,htmlView->GetHTMLName(0,16)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_GIF);
		else if(!strcmp(s,htmlView->GetHTMLName(0,17)))
			htmlView->LoadFromResource(IDR_HTML_HOT_KEYS);
		else if(!strcmp(s,htmlView->GetHTMLName(0,18)))
			htmlView->LoadFromResource(IDR_HTML_CHANGE_HOT_KEYS);
		else if(!strcmp(s,htmlView->GetHTMLName(0,19)))
			htmlView->LoadFromResource(IDR_HTML_FILE_ASSOCIATIONS);
		else if(!strcmp(s,htmlView->GetHTMLName(0,20)))
			htmlView->LoadFromResource(IDR_HTML_VIEW_FILES);
		else if(!strcmp(s,htmlView->GetHTMLName(0,21)))
			htmlView->LoadFromResource(IDR_HTML_QUICK_VIEW_FILES);
		else if(!strcmp(s,htmlView->GetHTMLName(0,22)))
			htmlView->LoadFromResource(IDR_HTML_GUID_FOLDERS);
		else if(!strcmp(s,htmlView->GetHTMLName(0,23)))
			htmlView->LoadFromResource(IDR_HTML_CONNECT_VIA_TCP_IP);
		else if(!strcmp(s,htmlView->GetHTMLName(0,24)))
			htmlView->LoadFromResource(IDR_HTML_IP4_ADAPTER_SCANER);
		else if(!strcmp(s,htmlView->GetHTMLName(0,25)))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_NOTEPAD);
}	}

void CLeftView::OnIndexSelChanged(NMHDR *pNMHDR,LRESULT *plResult)
{
	htmlView->beforeNavigateType = htmlView->fromIndexList;
	if(pNMHDR->code==TVN_SELCHANGED)
	{	TVITEMA tvi;ZeroMemory(&tvi,sizeof(tvi));
		tvi.mask = TVIF_TEXT;
		TCHAR s[128]="";
		tvi.pszText=&s[0];
		tvi.cchTextMax=128;
		HWND h = pNMHDR->hwndFrom;
		tvi.hItem = TreeView_GetSelection(pNMHDR->hwndFrom);
		TreeView_GetItem(h,&tvi);
		if(!strcmp(s,"Contents"))
			htmlView->LoadFromResource(IDR_HTML_MAIN);
		else if(!strcmp(s,"Aids about creating this program"))
			htmlView->LoadFromResource(IDR_HTML_SEL_SOZDANIYA);
		else if(!strcmp(s,"Program futuries"))
			htmlView->LoadFromResource(IDR_HTML_CHEM_OTLICHAYETCYA);
		else if(!strcmp(s,"Licenses"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE);
		else if(!strcmp(s,"7-Zip license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_7ZIP);
		else if(!strcmp(s,"CHM license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_CHM);
		else if(!strcmp(s,"JPG license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_JPG);
		else if(!strcmp(s,"Microsoft license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_MS);
		else if(!strcmp(s,"PNG license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_PNG);
		else if(!strcmp(s,"UNRAR license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_UNRAR);
		else if(!strcmp(s,"ZLIB license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_ZLIB);
		else if(!strcmp(s,"ULTRADEFRAG license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_UDEFRAG);
		else if(!strcmp(s,"GNU GENERAL PUBLIC LICENSE"))
			htmlView->LoadFromResource(IDR_HTML_GNU_LICENSE);
		else if(!strcmp(s,"WinPCap license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_WINPCAP);
		else if(!strcmp(s,"Sino license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_SINO);
		else if(!strcmp(s,"Short information about the current version of program"))
			htmlView->LoadFromResource(IDR_HTML_KRATKAYA_INFO);
		else if(!strcmp(s,"GIF license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_GIF);
		else if(!strcmp(s,"Notepad license"))
			htmlView->LoadFromResource(IDR_HTML_LICENSE_NOTEPAD);
}	}

void CLeftView::OnSearchSelChanged(NMHDR *pNMHDR,LRESULT *plResult)
{
	htmlView->beforeNavigateType = htmlView->fromSearchList;
	if(pNMHDR->code==TVN_SELCHANGED)
	{	TVITEMA tvi;ZeroMemory(&tvi,sizeof(tvi));
		tvi.mask = TVIF_PARAM;
		HWND h = pNMHDR->hwndFrom;
		tvi.hItem = TreeView_GetSelection(pNMHDR->hwndFrom);
		TreeView_GetItem(h,&tvi);
		htmlView->LoadFromResource((int)tvi.lParam);
		//mainFrame->SelectAllFindStrInCrntPage();
}	}

void CLeftView::AddSearchPage(int foundedPages,UINT uiRes)//NMHDR *pNMHDR,LRESULT *plResult)
{
	CTreeCtrl &t = GetTreeCtrl();
	if(0==foundedPages)
	{	if(htmlView->beforeNavigateType != htmlView->fromSearchList)//First entranse in list initing mode:
		{	htmlView->beforeNavigateType = htmlView->fromSearchList;
			listType=search;
			t.DeleteAllItems();
	}	}
	HTREEITEM item=t.InsertItem(TVIF_TEXT|TVIF_PARAM,htmlView->GetHTMLName(uiRes,0), 2, 2, 0, 0, uiRes, NULL, NULL);//child
	t.SetItemImage(item,2,2);
}

void CLeftView::OnInitialUpdate()
{
static BOOL bContetntsInited=FALSE;
	CTreeView::OnInitialUpdate();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));// clear out structure
	GetFont()->GetLogFont(&lf);
	lf.lfHeight = 12;               // request a 12-pixel-height font
	CFont font;
	font.CreateFontIndirect(&lf);  // create the font
	SetFont(&font,1);
	font.DeleteObject();  

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	if(!bContetntsInited)
	{	listView = this;
		CImageList *myImageList = new CImageList();
		myImageList->Create(16,16,ILC_COLOR16,3,3);
		myImageList->SetBkColor(RGB(0,32,96));//255,255,255));//CLR_NONE);
		HICON icb = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_BOOK));
		myImageList->Add(icb);
		HICON icob = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_OPEN_BOOK));
		myImageList->Add(icob);
		HICON icp = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_PAPER));
		myImageList->Add(icp);

		CTreeCtrl &t = GetTreeCtrl();
		CImageList *pim=t.SetImageList(myImageList,TVSIL_NORMAL);

		t.SetItemHeight(20);
		t.SetBkColor(RGB(0,32,96));//255,255,255
		t.SetTextColor(RGB(255,255,255));
		//t.SetInsertMarkColor(RGB(10,255,10));
		SetPage(contents,0);
		bContetntsInited = TRUE;
		/*RECT rc; GetClientRect(&rc);
		HWND hwndButton = CreateWindow( 
			"BUTTON",   // Predefined class; Unicode assumed. 
			"Contents",       // Button text. 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles. 
			rc.left+2,   // x position. 
			rc.bottom-76,// y position. 
			70,			// Button width.
			22,         // Button height.
			m_hWnd,     // Parent window.
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE), 
			NULL);      // Pointer not needed.*/
		//SetForegroundColor(
}	}

void CLeftView::SetPage(ListType contType,int sel)
{
	switch(contType)
	{	case contents:
			SetContentsPage(sel);
			break;
		case index:
			SetIndexPage(sel);
			break;
		case search:
			SetSearchPage(sel);
			break;
}	}

void CLeftView::SetContentsPage(int sel)
{
BOOL bDel=FALSE;
	CTreeCtrl &t = GetTreeCtrl();
	if(listType==contents)
	if(t.GetCount()!=0)
		goto Select;
	t.DeleteAllItems();
	bDel=TRUE;

	HTREEITEM root,item,licen,usng,hotkeys,netit;
	root=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,0), 0, 0, 0, 0, 0, NULL, NULL);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,1), 2, 2, 0, 0, 2, root, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,2), 2, 2, 0, 0, 2, root, NULL);
	t.SetItemImage(item,2,2);
	licen=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,3), 2, 2, 0, 0, 2, root, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,4), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,5), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,6), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,7), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,16), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,8), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,9), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,10), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,12), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,13), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,14), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,25), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,15), 2, 2, 0, 0, 2, licen, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,11), 2, 2, 0, 0, 2, root, NULL);
	t.SetItemImage(item,2,2);

	usng=t.InsertItem(TVIF_TEXT,"Using program", 2, 2, 0, 0, 2, root, NULL);
	hotkeys=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,17), 2, 2, 0, 0, 2, usng, NULL);//Hot keys
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,18), 2, 2, 0, 0, 2, hotkeys, NULL);
	t.SetItemImage(item,2,2);

	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,19), 2, 2, 0, 0, 2, usng, NULL);//File associations
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,20), 2, 2, 0, 0, 2, usng, NULL);//"Viewing of the files"
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,21), 2, 2, 0, 0, 2, usng, NULL);//"Quick viewing of the files"
	t.SetItemImage(item,2,2);
	
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,22), 2, 2, 0, 0, 2, usng, NULL);//"Working with GUID folders"
	t.SetItemImage(item,2,2);

	netit=t.InsertItem(TVIF_TEXT,"Working on the network", 2, 2, 0, 0, 2, usng, NULL);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,23), 2, 2, 0, 0, 2, netit, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,htmlView->GetHTMLName(0,24), 2, 2, 0, 0, 2, netit, NULL);//"IP4 adapter scaner virtual panel"
	t.SetItemImage(item,2,2);
	
	listType = contents;

	t.Expand(root,TVE_EXPAND);//oxirgi prnt->Kak soz

Select:
	item = t.GetRootItem();
	if(sel>0 && sel<MAX_HTMLS)
	{	for(int i=0; i<sel; i++)
		{	//t.Expand(item,TVE_EXPAND);
			item=t.GetNextVisibleItem(item);
			t.Expand(item,TVE_EXPAND);
		}
		t.SelectItem(item);
	}
	else if(!bDel)
		t.SelectItem(item);
}

void CLeftView::SelectItem(int sel)//Eng 1-marta bu yerga kirish mumkin emas;
{
	if(listType!=contents)
		return;

	CSinoSHlp32View::BeforeNavigateType saveType = htmlView->beforeNavigateType;
	htmlView->beforeNavigateType=htmlView->selectListItem;

	CTreeCtrl &t = GetTreeCtrl();
	HTREEITEM item = t.GetRootItem();
	if(sel>0 && sel<MAX_HTMLS+2)//2 ta lishniysi bor;
	{	for(int i=0; i<sel; i++)
		{	if(!t.EnsureVisible(item))
				t.Expand(item,TVE_EXPAND);
			item=t.GetNextVisibleItem(item);//item=t.GetNextItem(item,TVGN_NEXT);
		}
		t.SelectItem(item);
	}
	else
		t.SelectItem(item);
	t.EnsureVisible(item);
	//t.SetFocus();
	RECT rc;t.GetClientRect(&rc);
	t.SendMessage(WM_LBUTTONDOWN,0,MAKELONG(rc.right,rc.bottom));//
	//t.SetFocus();//t.ValidateRect(NULL);//t.UpUpdateWindow();//
	
	htmlView->beforeNavigateType=saveType;
}

void CLeftView::SetIndexPage(int sel)
{
BOOL bDel = FALSE;
	CTreeCtrl &t = GetTreeCtrl();
	if(listType==index)
	if(t.GetCount()!=0)
		goto Select;
	t.DeleteAllItems();
	bDel = TRUE;

	HTREEITEM item=t.InsertItem(TVIF_TEXT,_T("Contents"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("Aids about creating this program"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("Program futuries"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);

	item=t.InsertItem(TVIF_TEXT,_T("Licenses"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("7-Zip license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("CHM license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("JPG license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("Microsoft license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("GIF license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("PNG license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("UNRAR license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("ZLIB license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("ULTRADEFRAG license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("GNU GENERAL PUBLIC LICENSE"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("WinPCap license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("Notepad license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	item=t.InsertItem(TVIF_TEXT,_T("Sino license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);

	listType = index;
Select:
	item = t.GetRootItem();
	if(sel>0 && sel<MAX_HTMLS)
	{	for(int i=0; i<sel; i++)
		{	item=t.GetNextVisibleItem(item);
			t.Expand(item,TVE_EXPAND);
		}
		t.SelectItem(item);
	}
	else if(!bDel)
		t.SelectItem(item);
}

void CLeftView::SetSearchPage(int sel)
{
BOOL bDel = FALSE;
	CTreeCtrl &t = GetTreeCtrl();
	if(listType==search)
	if(t.GetCount()!=0)
		goto Select;
	t.DeleteAllItems();
	bDel = TRUE;

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;//|TVIF_SELECTEDIMAGE|TVIF_IMAGE;
	//tvInsert.item.iSelectedImage = 0;
	//tvInsert.item.iImage = 0;
	//tvInsert.item.stateMask = 1;//TVIS_OVERLAYMASK;

HTREEITEM item=t.InsertItem(TVIF_TEXT,_T("Contents"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("Aids about creating this program"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("Program futuries"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("Licenses"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("7-Zip license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("CHM license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("JPG license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("Microsoft license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("PNG license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("GIF license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("UNRAR license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("ZLIB license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("ULTRADEFRAG license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("GNU GENERAL PUBLIC LICENSE"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("WinPCap license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);
	t.InsertItem(TVIF_TEXT,_T("Sino license"), 0, 0, 0, 0, 0, NULL, NULL);
	t.SetItemImage(item,2,2);

	listType = search;
Select:
	if(sel>0)
	{	for(int i=0; i<sel; i++)
		{	item=t.GetNextVisibleItem(item);
			t.Expand(item,TVE_EXPAND);
		}
		t.SelectItem(item);
	}
	else if(!bDel)
		t.SelectItem(item);
}

void CLeftView::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if(pNMTreeView->action == TVE_EXPAND)
	{	CTreeCtrl &t = GetTreeCtrl();
		int tot = t.GetCount();
		if(!tot)return;
		HTREEITEM item = t.GetRootItem();
		for(int i=0; i<tot; i++)
		{	if(item==pNMTreeView->itemNew.hItem)
			{	if(0==pNMTreeView->itemNew.lParam)
				{	t.SetItemImage(item,1,1);
					break;
			}	}
			item=t.GetNextVisibleItem(item);
	}	}
	else if(pNMTreeView->action == TVE_COLLAPSE)
	{	CTreeCtrl &t = GetTreeCtrl();
		int tot = t.GetCount();
		if(!tot)return;
		HTREEITEM item = t.GetRootItem();
		for(int i=0; i<tot; i++)
		{	if(item==pNMTreeView->itemNew.hItem)
			{	if(0==pNMTreeView->itemNew.lParam)
				{	t.SetItemImage(item,0,0);
					break;
			}	}
			item=t.GetNextVisibleItem(item);
	}	}
	*pResult = 0;
}

// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CSinoSHlp32Doc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSinoSHlp32Doc)));
	return (CSinoSHlp32Doc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView message handlers
