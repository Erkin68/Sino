// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "strsafe.h"
#include "SinoHlp32.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "SinoHlp32View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMainFrame *mainFrame=NULL;


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame():iTotalHtmlResources(0),iSearchProcessBegin(0)
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
	if(resNamesBuf)
		free(resNamesBuf);
	resNamesBuf=0;
}

void CMainFrame::EnableToolbarBtn(int iBtn,BOOL bEn)
{
	UINT st = m_wndToolBar.GetToolBarCtrl().GetState(m_wndToolBar.GetItemID(iBtn));
	if(bEn)
	{	st &= ~(TBSTATE_INDETERMINATE|TBSTATE_PRESSED);
		st |= TBSTATE_ENABLED;
	}
	else
	{	st &= ~TBSTATE_ENABLED;
		st |= TBSTATE_INDETERMINATE|TBSTATE_PRESSED;
	}
	m_wndToolBar.GetToolBarCtrl().SetState(m_wndToolBar.GetItemID(iBtn),st);
}

BOOL CALLBACK EnumResNameProc(HMODULE hModule,LPCTSTR lpszType,LPTSTR lpszName,LONG_PTR lParam)
{
	if(IS_INTRESOURCE(lpszType))
	{	if(RT_HTML==lpszType)
			++mainFrame->iTotalHtmlResources;
	}
	else// if(strcmp(lpszType,"HTML"))
	{	//++iTotalHtmlResources;
		//iNameBufLn+=strlen(lpszName)+1;
		ASSERT(0);
	}
	return TRUE;
}
BOOL CALLBACK EnumFillResNameProc(HMODULE hModule,LPCTSTR lpszType,LPTSTR lpszName,LONG_PTR lParam)
{
	if(IS_INTRESOURCE(lpszType))
	{	int **nameBuf=(int**)lParam;
		if(RT_HTML==lpszType)
		{	**nameBuf = (int)lpszName;
			++(*nameBuf);
	}	}
	/*else if(strcmp(lpszType,"HTML"))
	{	char **nameBuf=(char**)lParam;
		lstrcpy(*nameBuf,lpszName);
		*nameBuf+=strlen(lpszName)+1;
		*(*nameBuf) = 0;
		++(*nameBuf);
	}*/
	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
//	m_wndContents.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	tooltip = new CToolTipCtrl();
	if(tooltip->Create(this))
	{	int id = IDS_STRING_CONTENTS;
		CRect r;
		m_wndToolBar.GetItemRect(0, &r);
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		m_wndToolBar.GetItemRect(1, &r);
		id = IDS_STRING_INDEX;
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		m_wndToolBar.GetItemRect(2, &r);
		id = IDS_STRING_SEARCH;
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		m_wndToolBar.GetItemRect(4, &r);
		id = IDS_STRING_PREV;
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		m_wndToolBar.GetItemRect(5, &r);
		id = IDS_STRING_NEXT;
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		m_wndToolBar.GetItemRect(6, &r);
		id = IDS_STRING_PREV_STEP;
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		m_wndToolBar.GetItemRect(7, &r);
		id = IDS_STRING_NEXT_STEP;
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		m_wndToolBar.GetItemRect(9, &r);
		id = IDS_STRING_FIND;
		VERIFY(tooltip->AddTool(&m_wndToolBar, id, &r, id));
		tooltip->Activate(TRUE);
	}

   // Associate the tooltip control to the tab control
   // of CMyPropertySheet.
//	DockControlBar(&m_wndContents);

	m_wndStatusBar.m_cyTopBorder+=5;
	m_wndStatusBar.m_cyBottomBorder+=5;

	//m_wndStatusBar.m_cxLeftBorder+=50;


/*	RECT rc={0,0,150,20};
	if (!m_wndContents.CreateEx(TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|
		CBRS_FLYBY|CBRS_SIZE_DYNAMIC,rc,&m_wndStatusBar,1))// ||
		//!m_wndContents.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndContents.AddStrings(_T("Contents\\0\\Index\\0Search\\0"));
	TBBUTTON tbb;
	tbb.iBitmap = -1; 
	tbb.idCommand = 0; 
	tbb.fsState = TBSTATE_ENABLED; 
	tbb.fsStyle = BTNS_BUTTON; 
	tbb.dwData = 0; 
	tbb.iString = 0;
	m_wndContents.AddButtons(1,&tbb);*/

	if(!EnumResourceNames(AfxGetInstanceHandle(),RT_HTML,(ENUMRESNAMEPROC)EnumResNameProc,0))
		return -1;
	resNamesBuf = (int*)malloc(iTotalHtmlResources*sizeof(int));
	int *iSpareNameBuf=resNamesBuf;
	if(!EnumResourceNames(AfxGetInstanceHandle(),RT_HTML,(ENUMRESNAMEPROC)EnumFillResNameProc,(LPARAM)&iSpareNameBuf))
		return -1;

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(200, 100), pContext) ||
		//!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CContents), CSize(200, 10), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSinoSHlp32View), CSize(200, 100), pContext))// ||
		//!m_wndSplitter.CreateView(1, 1, RUNTIME_CLASS(CToolBtns), CSize(200, 10), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	mainFrame = this;
	return TRUE;
}

int CMainFrame::FindText(CString &searchText,int flg,BOOL bNNF)
{
static CString sLastSearch;
static BSTR lastBookmark = NULL;

	if(sLastSearch != searchText)
		lastBookmark = NULL;
	sLastSearch = searchText;
	

	IHTMLDocument2 *lpHtmlDocument = NULL;
	LPDISPATCH lpDispatch = NULL;
	lpDispatch = htmlView->GetHtmlDocument();
	ASSERT(lpDispatch);

	lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&lpHtmlDocument);
	ASSERT(lpHtmlDocument);

	lpDispatch->Release();

	IHTMLElement *lpBodyElm;
	IHTMLBodyElement *lpBody;
	IHTMLTxtRange *lpTxtRange;

	lpHtmlDocument->get_body(&lpBodyElm);
	ASSERT(lpBodyElm);
	lpHtmlDocument->Release();
	lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
	ASSERT(lpBody);
	lpBodyElm->Release();
	lpBody->createTextRange(&lpTxtRange);
	ASSERT(lpTxtRange);
	lpBody->Release();

	CComBSTR search(searchText.GetLength()+1,(LPCTSTR)searchText);
	VARIANT_BOOL bFound=VARIANT_FALSE,
				 bTest =VARIANT_FALSE;
	long t;

	if(lastBookmark!=NULL)
	{	lpTxtRange->moveToBookmark(lastBookmark,(VARIANT_BOOL*)&bTest);
		if(!bTest)
		{	lastBookmark=NULL;
			lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),1,&t);
			lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
		} else
		{	lpTxtRange->moveStart((BSTR)CComBSTR("Character"),1,&t);
			lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
		}
	} else
	{	lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),0,&t);
		lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
	}
	lpTxtRange->findText((BSTR)search,0,4,(VARIANT_BOOL*)&bFound);//flag-Match case

	if(VARIANT_FALSE==bFound)
	{	if(lastBookmark==NULL && !bNNF)
		{	CString message;
			message.Format("Cannot find the string: '%s'",searchText);
			if(IDNO==MessageBox(message,"Want you to continue?",MB_YESNO))
			{	lpTxtRange->Release();
				return -1;
		}	}
		else if(lastBookmark!=NULL)
		{	lastBookmark = NULL;
			//FindText(searchText,flg,TRUE);
			lpTxtRange->Release();
			return 0;
		}
	} else
	{	if(lpTxtRange->getBookmark(&lastBookmark)!=S_OK)
			lastBookmark=NULL;
		lpTxtRange->select();
		lpTxtRange->scrollIntoView(TRUE);
	}

	lpTxtRange->Release();
	return 1;
}

BOOL CMainFrame::IsConsistFindText(CString &searchText)
{
	IHTMLDocument2 *lpHtmlDocument = NULL;
	LPDISPATCH lpDispatch = NULL;
	lpDispatch = htmlView->GetHtmlDocument();
	ASSERT(lpDispatch);

	lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&lpHtmlDocument);
	ASSERT(lpHtmlDocument);

	lpDispatch->Release();

	IHTMLElement *lpBodyElm;
	IHTMLBodyElement *lpBody;
	IHTMLTxtRange *lpTxtRange;

	lpHtmlDocument->get_body(&lpBodyElm);
	ASSERT(lpBodyElm);
	lpHtmlDocument->Release();
	lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
	ASSERT(lpBody);
	lpBodyElm->Release();
	lpBody->createTextRange(&lpTxtRange);
	ASSERT(lpTxtRange);
	lpBody->Release();

	CComBSTR search(searchText.GetLength()+1,(LPCTSTR)searchText);
	VARIANT_BOOL bFound=VARIANT_FALSE,
				 bTest =VARIANT_FALSE;
	long t;//BSTR p; lpTxtRange->get_text(&p);
	lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),0,&t);
	//lpTxtRange->get_text(&p);
	lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
	//lpTxtRange->get_text(&p);
	lpTxtRange->findText((BSTR)search,0,4,(VARIANT_BOOL*)&bFound);//flag-Match case
	lpTxtRange->Release();
	return bFound==VARIANT_TRUE?TRUE:FALSE;
}

/*VOID CMainFrame::SelectAllFindStrInCrntPage()
{
	if(iSearchProcessBegin!=0) return;
	if(crntSearchRes==0) return;
BSTR lastBookmark = NULL;
	
	IHTMLDocument2 *lpHtmlDocument = NULL;
	LPDISPATCH lpDispatch = NULL;
	lpDispatch = htmlView->GetHtmlDocument();
	ASSERT(lpDispatch);

	lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&lpHtmlDocument);
	ASSERT(lpHtmlDocument);

	lpDispatch->Release();

	IHTMLElement *lpBodyElm;
	IHTMLBodyElement *lpBody;
	IHTMLTxtRange *lpTxtRange;

	lpHtmlDocument->get_body(&lpBodyElm);
	ASSERT(lpBodyElm);
	lpHtmlDocument->Release();
	lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
	ASSERT(lpBody);
	lpBodyElm->Release();
	lpBody->createTextRange(&lpTxtRange);
	ASSERT(lpTxtRange);
	lpBody->Release();

	CComBSTR search(schText.GetLength()+1,(LPCTSTR)schText);
	VARIANT_BOOL bFound=VARIANT_FALSE,
				 bTest =VARIANT_FALSE;
	long t;//BSTR p; lpTxtRange->get_text(&p);
	//lpTxtRange->moveStart(lastBookmark,0,&t);
	do
	{	if(lastBookmark!=NULL)
		{	lpTxtRange->moveToBookmark(lastBookmark,(VARIANT_BOOL*)&bTest);
			if(VARIANT_FALSE==bTest)
			{	lastBookmark=NULL;
				lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),1,&t);
				lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
			} else
			{	lpTxtRange->moveStart((BSTR)CComBSTR("Character"),1,&t);
				lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
			}
		} else
		{	lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),0,&t);
			lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
		}
		lpTxtRange->findText((BSTR)search,0,4,(VARIANT_BOOL*)&bFound);//flag-Match case

		if(VARIANT_FALSE==bFound)
			break;
		else
		{	if(lpTxtRange->getBookmark(&lastBookmark)!=S_OK)
				lastBookmark=NULL;
			lpTxtRange->select();
	}	} while(VARIANT_TRUE==bFound);

	lpTxtRange->Release();
}*/
VOID CMainFrame::SelectAllFindStrInCrntPage()
{
long lFlags = 2;
CString matchStyle = "color: white; background-color: darkblue";
CString searchID = "CHtmlView_Search";

	ClearSearchResults();
	IHTMLDocument2 *lpHtmlDocument = NULL;
	LPDISPATCH lpDispatch = NULL;
	lpDispatch = htmlView->GetHtmlDocument();
	ASSERT(lpDispatch);

	lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&lpHtmlDocument);
	ASSERT(lpHtmlDocument);

	lpDispatch->Release();

	IHTMLElement *lpBodyElm;
	IHTMLBodyElement *lpBody;
	IHTMLTxtRange *lpTxtRange;

	lpHtmlDocument->get_body(&lpBodyElm);
	ASSERT(lpBodyElm);
	lpHtmlDocument->Release();
	lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
	ASSERT(lpBody);
	lpBodyElm->Release();
	lpBody->createTextRange(&lpTxtRange);
	ASSERT(lpTxtRange);
	lpBody->Release();

	CComBSTR html;
	CComBSTR newhtml;
	CComBSTR search(schText.GetLength()+1,(LPCTSTR)schText);

	long t;
	VARIANT_BOOL bFound;
	while(lpTxtRange->findText(search,0,lFlags,(VARIANT_BOOL*)&bFound),bFound)
	{	newhtml.Empty();
		lpTxtRange->get_htmlText(&html);
		newhtml.Append("<span id='");
		newhtml.Append((LPCTSTR)searchID);
		newhtml.Append("' style='");
		newhtml.Append((LPCTSTR)matchStyle);
		newhtml.Append("'>");
		if(schText==" ")
			newhtml.Append("&nbsp;"); // doesn't work very well, but prevents (some) shit
		else 
			newhtml.AppendBSTR(html);
		newhtml.Append("</span>");
		lpTxtRange->pasteHTML(newhtml);
				
		lpTxtRange->moveStart((BSTR)CComBSTR("Character"),1,&t);
		lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
	}
	lpTxtRange->Release();
}

void CMainFrame::ClearSearchResults()
{
CString searchID = "CHtmlView_Search";
	CComBSTR testid(searchID.GetLength()+1,searchID);
	CComBSTR testtag(5,"SPAN");
	IHTMLDocument2 *lpHtmlDocument = NULL;
	LPDISPATCH lpDispatch = NULL;
	lpDispatch = htmlView->GetHtmlDocument();
	ASSERT(lpDispatch);

	lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&lpHtmlDocument);
	ASSERT(lpHtmlDocument);
	lpDispatch->Release();

	IHTMLElementCollection *lpAllElements;
	lpHtmlDocument->get_all(&lpAllElements);
	ASSERT(lpAllElements);
	lpHtmlDocument->Release();

	IUnknown *lpUnk;
	IEnumVARIANT *lpNewEnum;
	if (SUCCEEDED(lpAllElements->get__newEnum(&lpUnk)) && lpUnk != NULL)
	{
		lpUnk->QueryInterface(IID_IEnumVARIANT,(void**)&lpNewEnum);
		ASSERT(lpNewEnum);
		VARIANT varElement;
		IHTMLElement *lpElement;

		while (lpNewEnum->Next(1, &varElement, NULL) == S_OK)
		{
			_ASSERTE(varElement.vt == VT_DISPATCH);
			varElement.pdispVal->QueryInterface(IID_IHTMLElement,(void**)&lpElement);
			ASSERT(lpElement);
			if (lpElement)
			{
				CComBSTR id;
				CComBSTR tag;
				lpElement->get_id(&id);
				lpElement->get_tagName(&tag);
				if((id==testid)&&(tag==testtag))
				{
					BSTR innerText;
					lpElement->get_innerHTML(&innerText);
					lpElement->put_outerHTML(innerText);
				}
			}
			VariantClear(&varElement);
		}
	}
}

INT_PTR CALLBACK PrgrsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT rc,rcPrnt;
//char s[260];
int width,left,height,top;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		GetWindowRect(hDlg, &rc);
		HWND prnt;prnt = GetParent(hDlg);
		if(!prnt)prnt=GetDesktopWindow();
		GetWindowRect(prnt, &rcPrnt);
		width = rc.right - rc.left;
		left = rcPrnt.left+(rcPrnt.right-rcPrnt.left)/2-width/2;
		height = rc.bottom - rc.top;
		top = rcPrnt.top+(rcPrnt.bottom-rcPrnt.top)/2-height/2;
		MoveWindow(hDlg, left, top+20, width, height, TRUE);
		SendMessage(GetDlgItem(hDlg,IDC_PROGRESS),PBM_SETPOS,0,0);  
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

//Hamma resurslar yoki int tipida bo'lishi kerak, yoki LPCSTR tipida, ya'ni "" ichida bo'lishi shart;
//1 ta undan, 1ta bundan bo'lmasligi kerak, hozir mammasi int bo'lishi shart:
BOOL CMainFrame::BeginFindTextInAllHTMLs(CString &searchText,int flg,BOOL bNNF)
{
BOOL bFindText=FALSE;
	crntSearchRes=0;
	foundedPages=0;
	seachPrgsDlg=NULL;

	if(iTotalHtmlResources>0)
	{	//Save each html in crnt document loading:
		saveuiRes=htmlView->uiCrntRes;
		schText = searchText;
		seachPrgsDlg=CreateDialog(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDD_DIALOG_PROGRESS),m_hWnd,PrgrsDlgProc);
		::ShowWindow(seachPrgsDlg,SW_SHOW);
		iSearchProcessBegin=1;
		//1 ta chaqirib olamiz, bo'lmasa boshlamaydur;
		htmlView->LoadFromResource(resNamesBuf[0]);
	}
	return TRUE;
}

VOID CMainFrame::ContinueFindTextInAllHTMLs()
{
	if(crntSearchRes<iTotalHtmlResources)//for(int htm=0; htm<iTotalHtmlResources; htm++)
	{	if(IsConsistFindText(schText))
			listView->AddSearchPage(foundedPages++,resNamesBuf[crntSearchRes]);
		char s[128];StringCchPrintf(s,128,"Res. num: 0x%x",resNamesBuf[crntSearchRes]);
		::SetDlgItemText(seachPrgsDlg,IDC_EDIT_PROGRESS,s);
		::SendMessage(::GetDlgItem(seachPrgsDlg,IDC_PROGRESS),PBM_SETPOS,100*(resNamesBuf[crntSearchRes])/iTotalHtmlResources,0);
		++crntSearchRes;
		htmlView->LoadFromResource(resNamesBuf[crntSearchRes]);
	}
	else EndFindTextInAllHTMLs();
}

VOID CMainFrame::EndFindTextInAllHTMLs()
{
	if(seachPrgsDlg)
	{	htmlView->LoadFromResource(saveuiRes);//Restore previous page:	//	FindText(searchText,flg,bNNF);
		::DestroyWindow(seachPrgsDlg);
		iSearchProcessBegin=2;
}	}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers