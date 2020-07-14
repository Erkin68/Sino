// SinoSHlp32View.cpp : implementation of the CSinoSHlp32View class
//

#include "stdafx.h"
#include "SinoHlp32.h"

#include "SinoHlp32Doc.h"
#include "SinoHlp32View.h"
#include "LeftView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSinoSHlp32View *htmlView=NULL;

// CSinoSHlp32View

IMPLEMENT_DYNCREATE(CSinoSHlp32View, CHtmlView)

BEGIN_MESSAGE_MAP(CSinoSHlp32View, CHtmlView)
END_MESSAGE_MAP()

// CSinoSHlp32View construction/destruction

CSinoSHlp32View::CSinoSHlp32View():iPage(0),beforeNavigateType(undefined),uiCrntRes(0),findTxtDlg(NULL)//iStep(0),
{
	// TODO: add construction code here
//	for(int i=0; i<MAX_STEPS-1; i++)
//		steps[i] = -1;
	tchCrntRes[0]=0;
}

CSinoSHlp32View::~CSinoSHlp32View()
{
}

/*BOOL CSinoSHlp32View::PushStep(int page)
{
	if(iStep<MAX_STEPS-2)
	{	steps[iStep]=page;
		++iStep;
	}
	else
	{	for(int i=0; i<MAX_STEPS-2; i++)
			steps[i]=steps[i+1];
		steps[iStep]=page;
	}
	return TRUE;
}

BOOL CSinoSHlp32View::PopStep(int page)
{
	if(iStep>0)
	{	steps[iStep]=page;
		--iStep;
	}
	else
	{	steps[iStep]=page;
	}
	return TRUE;
}*/

void CSinoSHlp32View::DocumentComplete(LPDISPATCH pDisp,VARIANT *URL)
{
	CHtmlView::DocumentComplete(pDisp,URL);
	if(1==mainFrame->iSearchProcessBegin)
	{	mainFrame->ContinueFindTextInAllHTMLs();
	}
	else if(2==mainFrame->iSearchProcessBegin)
	{	mainFrame->iSearchProcessBegin = 0;
		mainFrame->SelectAllFindStrInCrntPage();
	}
	else if(0==mainFrame->iSearchProcessBegin && listType==search)
		mainFrame->SelectAllFindStrInCrntPage();
}

BOOL CSinoSHlp32View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CHtmlView::PreCreateWindow(cs);
}

void CSinoSHlp32View::OnBeforeNavigate2(LPCTSTR lpszURL,
										DWORD nFlags,
										LPCTSTR lpszTargetFrameName,
										CByteArray& baPostedData,
										LPCTSTR lpszHeaders,
										BOOL* pbCancel)
{
LPCTSTR lpszURLP=lpszURL,lpszURLPP;
LPCTSTR lpszTargetFrameNameP=lpszTargetFrameName,lpszTargetFrameNamePP;
BOOL myCancel=TRUE;
int p=iPage;
 iList=p;
 for(lpszURLPP=lpszURL; *lpszURLPP; lpszURLPP++)
 {	if(*lpszURLPP == '\\' || *lpszURLPP == '/')
 		lpszURLP = lpszURLPP+1;
 }
 for(lpszTargetFrameNamePP=lpszTargetFrameName; *lpszTargetFrameNamePP; lpszTargetFrameNamePP++)
 {	if(*lpszTargetFrameNamePP == '\\' || *lpszTargetFrameNamePP == '/')
 		lpszTargetFrameNameP = lpszTargetFrameNamePP+1;
 }

 int htmNameAsInt = atoi(lpszURLP);
	
 if(beforeNavigateType==fromHtmlElement)
 {	if(htmNameAsInt>0)
		goto Sl;//return;//This is inside html call
	if(!strcmp(lpszTargetFrameNameP,"Содержание%20справки.htm") ||
	        !strcmp(lpszURLP,"Содержание%20справки.htm"))
	{	if(0!=iPage)
		{	LoadFromResource(IDR_HTML_MAIN);
			pbCancel = &myCancel;
			iPage = 0;
			iList = 0;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Цель%20создания%20программы.htm") ||
	        !strcmp(lpszURLP,"Цель%20создания%20программы.htm"))
	{	if(1!=iPage)
		{	LoadFromResource(IDR_HTML_SEL_SOZDANIYA);
			pbCancel = &myCancel;
			iPage = 1;
			iList = 1;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Чем%20отличается%20программа%20от%20других%20аналогичных%20программ.htm") ||
	        !strcmp(lpszURLP,"Чем%20отличается%20программа%20от%20других%20аналогичных%20программ.htm"))
	{	if(2!=iPage)
		{	LoadFromResource(IDR_HTML_CHEM_OTLICHAYETCYA);
			pbCancel = &myCancel;
			iPage = 2;
			iList = 2;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии.htm") ||
	        !strcmp(lpszURLP,"Лицензии.htm"))
	{	if(3!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE);
			pbCancel = &myCancel;
			iPage = 3;
			iList = 3;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_7-Zip.htm") ||
	        !strcmp(lpszURLP,"Лицензии_7-Zip.htm"))
	{	if(4!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_7ZIP);
			pbCancel = &myCancel;
			iPage = 4;
			iList = 4;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_CHMLib.htm") ||
	        !strcmp(lpszURLP,"Лицензии_CHMLib.htm"))
	{	if(5!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_CHM);
			pbCancel = &myCancel;
			iPage = 5;
			iList = 5;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Jpg.htm") ||
	        !strcmp(lpszURLP,"Лицензии_Jpg.htm"))
	{	if(6!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_JPG);
			pbCancel = &myCancel;
			iPage = 6;
			iList = 6;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Microsoft.htm") ||
	        !strcmp(lpszURLP,"Лицензии_Microsoft.htm"))
	{	if(7!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_MS);
			pbCancel = &myCancel;
			iPage = 7;
			iList = 7;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Png.htm") ||
	        !strcmp(lpszURLP,"Лицензии_Png.htm"))
	{	if(8!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_PNG);
			pbCancel = &myCancel;
			iPage = 8;
			iList = 8;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Unrar.htm") ||
	        !strcmp(lpszURLP,"Лицензии_Unrar.htm"))
	{	if(9!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_UNRAR);
			pbCancel = &myCancel;
			iPage = 9;
			iList = 9;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Zlib.htm") ||
	        !strcmp(lpszURLP,"Лицензии_Zlib.htm"))
	{	if(10!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_ZLIB);
			pbCancel = &myCancel;
			iPage = 10;
			iList = 10;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Краткая%20информация%20о%20содержании%20текущей%20версии%20программы.htm") ||
	        !strcmp(lpszURLP,"Краткая%20информация%20о%20содержании%20текущей%20версии%20программы.htm"))
	{	if(11!=iPage)
		{	LoadFromResource(IDR_HTML_KRATKAYA_INFO);
			pbCancel = &myCancel;
			iPage = 11;
			iList = 11;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_ultradefrag.htm") ||
	        !strcmp(lpszURLP,"Лицензии_ultradefrag.htm"))
	{	if(12!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_UDEFRAG);
			pbCancel = &myCancel;
			iPage = 12;
			iList = 12;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_GNU_General_Public_License.htm") ||
	        !strcmp(lpszURLP,"Лицензии_GNU_General_Public_License.htm"))
	{	if(13!=iPage)
		{	LoadFromResource(IDR_HTML_GNU_LICENSE);
			pbCancel = &myCancel;
			iPage = 13;
			iList = 13;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_WinPCap") ||
	        !strcmp(lpszURLP,"Лицензии_WinPCap.htm"))
	{	if(14!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_WINPCAP);
			pbCancel = &myCancel;
			iPage = 14;
			iList = 14;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Sino") ||
	        !strcmp(lpszURLP,"Лицензии_Sino.htm"))
	{	if(15!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_SINO);
			pbCancel = &myCancel;
			iPage = 15;
			iList = 15;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Gif") ||
	        !strcmp(lpszURLP,"Лицензии_Gif.htm"))
	{	if(16!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_GIF);
			pbCancel = &myCancel;
			iPage = 16;
			iList = 16;
 	}	}
	else if(!strcmp(lpszTargetFrameNameP,"Лицензии_Notepad") ||
	        !strcmp(lpszURLP,"Лицензии_notepad.htm"))
	{	if(17!=iPage)
		{	LoadFromResource(IDR_HTML_LICENSE_NOTEPAD);
			pbCancel = &myCancel;
			iPage = 17;
			iList = 17;
 }	}	}
 else
 {	
Sl:	 
	if(htmNameAsInt>0)
	{	switch(htmNameAsInt)
		{	case IDR_HTML_MAIN:
				iPage = 0; iList = 0; break;
			case IDR_HTML_SEL_SOZDANIYA:
				iPage = 1; iList = 1; break;
			case IDR_HTML_CHEM_OTLICHAYETCYA:
				iPage = 2; iList = 2; break;
			case IDR_HTML_LICENSE:
				iPage = 3; iList = 3; break;
			case IDR_HTML_LICENSE_7ZIP:
				iPage = 4; iList = 4; break;
			case IDR_HTML_LICENSE_CHM:
				iPage = 5; iList = 5; break;
			case IDR_HTML_LICENSE_JPG:
				iPage = 6; iList = 6; break;
			case IDR_HTML_LICENSE_MS:
				iPage = 7; iList = 7; break;
			case IDR_HTML_LICENSE_PNG:
				iPage = 8; iList = 8; break;
			case IDR_HTML_LICENSE_UNRAR:
				iPage = 9; iList = 9; break;
			case IDR_HTML_LICENSE_ZLIB:
				iPage = 10; iList = 10; break;
			case IDR_HTML_KRATKAYA_INFO:
				iPage = 11; iList = 11; break;
			case IDR_HTML_LICENSE_UDEFRAG:
				iPage = 12; iList = 12; break;
			case IDR_HTML_GNU_LICENSE:
				iPage = 13; iList = 13; break;
			case IDR_HTML_LICENSE_WINPCAP:
				iPage = 14; iList = 14; break;
			case IDR_HTML_LICENSE_SINO:
				iPage = 15; iList = 15; break;
			case IDR_HTML_LICENSE_GIF:
				iPage = 16; iList = 16; break;
			case IDR_HTML_LICENSE_NOTEPAD:
				iPage = 17; iList = 14; break;
}	}	}

 if(listType==contents)//Qaytib otasini bossa Navigate ishlashi uhun;
 {	if(htmNameAsInt==0)
	{	if(beforeNavigateType==fromHtmlElement || beforeNavigateType==fromContentsButton)
			listView->SelectItem(iList);
 }	}

//if(p!=iPage)
//if(!(beforeNavigateType==fromContentsButton || beforeNavigateType==fromIndexButton || beforeNavigateType==fromSearchButton))
//	PushStep(p);

 beforeNavigateType = fromHtmlElement;
}

void CSinoSHlp32View::OnInitialUpdate()
{
	htmlView = this;
	CHtmlView::OnInitialUpdate();
	//LoadFromResource(IDR_HTML_SODERJANIYE);//Navigate2(_T("http://www.msdn.microsoft.com/visualc/"),NULL,NULL);
}

BOOL CSinoSHlp32View::OnPrevBtnPressed()
{
	if(iPage>0)
	{	NavigatePage(iPage-1);//,TRUE);
		return TRUE;
	}
/*	switch(listType)
	{	case contents:
			if(iPage>0)
			{	NavigatePage(iPage-1,TRUE,contents);
				return TRUE;
			}
			break;
		case index:
			if(iPage>0)
			{	NavigatePage(iPageIndex-1,TRUE,index);
				return TRUE;
			}
			break;
		case search:
			if(iPageSearch>0)
			{	NavigatePage(iPageSearch-1,TRUE,search);
				return TRUE;
			}
			break;
	}*/
	return FALSE;
}

BOOL CSinoSHlp32View::OnNextBtnPressed()
{
	if(iPage<MAX_HTMLS)
	{	NavigatePage(iPage+1);//,TRUE);
		return TRUE;
	}
/*	switch(listType)
	{	case contents:
			if(iPage<MAX_HTMPLS)
			{	NavigatePage(iPage+1,TRUE,contents);
				return TRUE;
			}
			break;
		case index:
			if(iPageIndex<MAX_HTMPLS)
			{	NavigatePage(iPageIndex+1,TRUE,index);
				return TRUE;
			}
			break;
		case search:
			if(iPageSearch<MAX_HTMPLS)
			{	NavigatePage(iPageSearch+1,TRUE,search);
				return TRUE;
			}
			break;
	}*/
	return FALSE;
}

BOOL CSinoSHlp32View::OnPrevStepBtnPressed()
{
/*	if(iStep>0)
	{	PopStep(iPage);
		//switch(listType)
		//{	case contents:
		//		PopStep(iPage);
		//		break;
		//	case index:
		//		PopStep(index,iPageIndex);
		//		break;
		//	case search:
		//		PopStep(search,iPageSearch);
		//		break;
		//}
		NavigatePage(steps[iStep],FALSE);
		//listView->SetPage(steps[iStep].listType,steps[iStep].i); nav gapening o'zida bor;
		return TRUE;
	}
	*/
	GoBack();
	/*CString s = GetLocationName();
	const char *p=strrchr(s,'/');
	if(p)
	{	int ip = atoi(p+1);
		if(ip>0 && ip<200)
			GoBack();//htmlView->LoadFromResource(ip);
	}*/
	listView->SelectItem(iList);
	return TRUE;//FALSE;
}

BOOL CSinoSHlp32View::OnNextStepBtnPressed()
{
/*	if(iStep<MAX_STEPS && steps[iStep+1]>-1)
	{	++iStep;
		NavigatePage(steps[iStep],FALSE);
		//listView->SetPage(steps[iStep].listType,steps[iStep]);
	}*/
	GoForward();
	/*CString s = GetLocationName();
	const char *p=strrchr(s,'/');
	if(p)
	{	int ip = atoi(p+1);
		if(ip>0 && ip<200)
			;//GoForward();//htmlView->LoadFromResource(ip);
	}*/
	listView->SelectItem(iList);
	return TRUE;//FALSE;
}

INT_PTR CALLBACK FindTextDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		left = rcPrnt.right- width;
		height = rc.bottom - rc.top;
		top = rcPrnt.top;
		MoveWindow(hDlg, left, top+20, width, height, TRUE);

		SetDlgItemText(hDlg,IDC_EDIT_FIND_TEXT,mainFrame->schText);
		SendMessage(GetDlgItem(hDlg,IDC_EDIT_FIND_TEXT),EM_SETSEL,0,-1);
		return (INT_PTR)TRUE;
	case WM_DESTROY:
		htmlView->findTxtDlg = NULL;
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDCANCEL:
				DestroyWindow(hDlg);
				return (INT_PTR)FALSE;
			case IDOK://Find next
				char s[128];if(GetDlgItemText(hDlg,IDC_EDIT_FIND_TEXT,s,128))
				{	CString c(s);
					int fr=mainFrame->FindText(c,4,FALSE);
					if(-1==fr) DestroyWindow(hDlg);
					else if(0==fr)
					{	if(IDYES!=MessageBox(hDlg,"Want you to continue find process?","Search rich to end mark.",MB_YESNO))
							DestroyWindow(hDlg);
				}	}
				return (INT_PTR)FALSE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK FindTextInAllHTMLsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT rc;
//char s[260];
int width,left,height,top;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		GetWindowRect(hDlg, &rc);
		width = rc.right - rc.left;		
		left = (GetSystemMetrics(SM_CXFULLSCREEN) - width)/2;
		height = rc.bottom - rc.top;
		top = (GetSystemMetrics(SM_CYFULLSCREEN) - height)/2;
		MoveWindow(hDlg, left, top+20, width, height, TRUE);

		SetDlgItemText(hDlg,IDOK,"Find all");
		SetDlgItemText(hDlg,IDC_EDIT_FIND_TEXT,mainFrame->schText);
		SendMessage(GetDlgItem(hDlg,IDC_EDIT_FIND_TEXT),EM_SETSEL,0,-1);
		return (INT_PTR)TRUE;
	case WM_DESTROY:
		htmlView->findTxtDlg = NULL;
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDCANCEL:
				DestroyWindow(hDlg);
				return (INT_PTR)FALSE;
			case IDOK://Find next
				char s[128];if(GetDlgItemText(hDlg,IDC_EDIT_FIND_TEXT,s,128))
				{	CString c(s);
					DestroyWindow(hDlg);
					mainFrame->BeginFindTextInAllHTMLs(c,4,FALSE);
				}
				return (INT_PTR)FALSE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CSinoSHlp32View::OnFindTextBtnPressed()
{
	findTxtDlg=CreateDialog(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDD_DIALOG_FIND_TEXT),
							m_hWnd,FindTextDlgProc);
	::ShowWindow(findTxtDlg,SW_SHOW);
	//GoSearch(); internetdan;
	//TCHAR vinp[] = _T("");
	//ExecWB(OLECMDID_FIND,OLECMDEXECOPT_PROMPTUSER,(VARIANT*)&vinp,NULL);
	return TRUE;
}

BOOL CSinoSHlp32View::OnSearchTextBtnPressed()
{
	findTxtDlg=CreateDialog(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDD_DIALOG_FIND_TEXT),
							m_hWnd,FindTextInAllHTMLsDlgProc);
	::ShowWindow(findTxtDlg,SW_SHOW);
	return TRUE;
}

LPCTSTR CSinoSHlp32View::GetHTMLName(UINT uiRes,int t)
{
	if(uiRes!=0)
	{	switch(uiRes)
		{	case IDR_HTML_MAIN:
				return "Contents";
			case IDR_HTML_SEL_SOZDANIYA:
				return "Aids about creating this program";
			case IDR_HTML_CHEM_OTLICHAYETCYA:
				return "Program futuries";
			case IDR_HTML_LICENSE:
				return "Licenses";
			case IDR_HTML_LICENSE_7ZIP:
				return "7-Zip license";
			case IDR_HTML_LICENSE_CHM:
				return "CHM license";
			case IDR_HTML_LICENSE_JPG:
				return "JPG license";
			case IDR_HTML_LICENSE_MS:
				return "Microsoft license";
			case IDR_HTML_LICENSE_PNG:
				return "PNG license";
			case IDR_HTML_LICENSE_UNRAR:
				return "UNRAR license";
			case IDR_HTML_LICENSE_ZLIB:
				return "ZLIB license";
			case IDR_HTML_KRATKAYA_INFO:
				return "Short information about the current version of program";
			case IDR_HTML_LICENSE_UDEFRAG:
				return "ULTRADEFRAG license";
			case IDR_HTML_GNU_LICENSE:
				return "GNU GENERAL PUBLIC LICENSE";
			case IDR_HTML_LICENSE_WINPCAP:
				return "WinPCap license";
			case IDR_HTML_LICENSE_SINO:
				return "Sino license";
			case IDR_HTML_LICENSE_GIF:
				return "GIF license";
			case IDR_HTML_HOT_KEYS:
				return "Hot keys";
			case IDR_HTML_CHANGE_HOT_KEYS:
				return "Changing hot keys";
			case IDR_HTML_FILE_ASSOCIATIONS:
				return "File associations";
			case IDR_HTML_VIEW_FILES:
				return "Viewing of the files";
			case IDR_HTML_QUICK_VIEW_FILES:
				return "Quick viewing of the files";
			case IDR_HTML_GUID_FOLDERS:
				return "Working with GUID folders";
			case IDR_HTML_CONNECT_VIA_TCP_IP:
				return "Direct connect via TCP/IP";
			case IDR_HTML_IP4_ADAPTER_SCANER:
				return "Scanning on the NET by the IP4 scaner plugin";
			case IDR_HTML_LICENSE_NOTEPAD:
				return "Notepad license";
	}	}
	else
	{	switch(t)
		{	case 0:
				return "Contents";
			case 1:
				return "Aids about creating this program";
			case 2:
				return "Program futuries";
			case 3:
				return "Licenses";
			case 4:
				return "7-Zip license";
			case 5:
				return "CHM license";
			case 6:
				return "JPG license";
			case 7:
				return "Microsoft license";
			case 8:
				return "PNG license";
			case 9:
				return "UNRAR license";
			case 10:
				return "ZLIB license";
			case 11:
				return "Short information about the current version of program";
			case 12:
				return "ULTRADEFRAG license";
			case 13:
				return "GNU GENERAL PUBLIC LICENSE";
			case 14:
				return "WinPCap license";
			case 15:
				return "Sino license";
			case 16:
				return "GIF license";
			case 17:
				return "Hot keys";
			case 18:
				return "Changing hot keys";
			case 19:
				return "File associations";				
			case 20:
				return "Viewing of the files";
			case 21:
				return "Quick viewing of the files";
			case 22:
				return "Working with GUID folders";
			case 23:
				return "Direct connect via TCP/IP";
			case 24:
				return "Scanning on the NET by the IP4 scaner plugin";
			case 25:
				return "Notepad license";
	}	}
	return NULL;
}

BOOL CSinoSHlp32View::LoadFromResource(LPCTSTR lpszResource)
{
	lstrcpy(tchCrntRes,lpszResource);
	uiCrntRes=0;
	return CHtmlView::LoadFromResource(lpszResource);
}

BOOL CSinoSHlp32View::LoadFromResource(UINT nRes)
{
	uiCrntRes=nRes;
	tchCrntRes[0]=0;
	return CHtmlView::LoadFromResource(nRes);
}

void CSinoSHlp32View::NavigateComplete2(LPDISPATCH pDisp,VARIANT *URL)
{
	return CHtmlView::NavigateComplete2(pDisp,URL);
}

/*void CSinoSHlp32View::MyNavigateComplete2(UINT uiRes)
{
	CHtmlView::NavigateComplete2(GetIDispatch(FALSE),(VARIANT*)GetHTMLName(uiRes,0));
}*/

/*void CSinoSHlp32View::Invalidate(BOOL bErase=1)
{
	return CHtmlView::Invalidate(bErase);
}*/

/*void CSinoSHlp32View::DoDataExchange(CDataExchange *pDX)
{
	return CHtmlView::DoDataExchange(pDX);
}*/

/*void CSinoSHlp32View::NotifyWinEvent(DWORD event,LONG idObjectType,LONG idObject)
{
	return CHtmlView::NotifyWinEvent(event,idObjectType,idObject);
}*/

/*BOOL CSinoSHlp32View::OnCmdMsg(UINT nID,int nCode,void *pExtra,AFX_CMDHANDLERINFO *pHandlerInfo)
{
	return CHtmlView::OnCmdMsg(nID,nCode,pExtra,pHandlerInfo);
}*/

void CSinoSHlp32View::NavigatePage(int p)//,BOOL bRegisterStep)
{
	switch(p)
	{	case 0:
			listView->SelectItem(0);
			LoadFromResource(IDR_HTML_MAIN);			
			break;
	}
	//if(bRegisterStep)
	//	PushStep(iPage);
	iPage = p;
}

void CSinoSHlp32View::ChangeFont(int fontSize)
{
/*HRESULT hr;
LPDISPATCH pDisp = NULL;
LPOLECOMMANDTARGET pCmdTarg = NULL;

	if(!m_hWnd)
		return;

	pDisp = GetHtmlDocument();

	if(!pDisp)
	{	TRACE("Unable to get document from Web Browser.\n");
		return;
	}

	// The document controls the availability of commands items,
	// so get the OLE command target interface from the document
	hr = pDisp->QueryInterface(IID_IOleCommandTarget,(LPVOID*)&pCmdTarg);
	if(pCmdTarg)
	{	// Now use the command target to do something useful
		// like (un-)zoom the page
		OLECMD rgCmd[1] = {{OLECMDID_ZOOM, 0}};
		// Is the command available for execution?
		hr = pCmdTarg->QueryStatus(NULL, 1, rgCmd, NULL);
		if(SUCCEEDED(hr))// && OLECMDF_ENABLED == rgCmd[0].cmdf)
		{	TRACE("Zoom enabled.\n");
			VARIANT vaZoomFactor;   // Input arguments
			VariantInit(&vaZoomFactor);
			V_VT(&vaZoomFactor) = VT_I4;
			V_I4(&vaZoomFactor) = fontSize;
			hr = pCmdTarg->Exec(NULL,OLECMDID_ZOOM,OLECMDEXECOPT_DONTPROMPTUSER,&vaZoomFactor, NULL);
			VariantClear(&vaZoomFactor);
		}
		else
			TRACE("Unable to query for status of command ; (OLECMDID_ZOOM).\n");
	}
	else
		TRACE("Unable to get command target from Web Browser ; document.\n");

	if(pCmdTarg) pCmdTarg->Release(); // release document's command target
	if(pDisp) pDisp->Release(); // release document's dispatch interface
	*/




	/*LPDISPATCH pDisp = GetHtmlDocument();
	if(pDisp)
	{	LPOLECOMMANDTARGET pCmdTarg = NULL;
		pDisp->QueryInterface(IID_IOleCommandTarget, (void**)&pCmdTarg);
		if(pCmdTarg)
		{	COleVariant vaZoomFactor;   // input argument
			V_VT(&vaZoomFactor) = VT_I4;
			V_I4(&vaZoomFactor) = fontSize;   // 0 - 4
			// Change the text size.
			pCmdTarg->Exec(	NULL,
							OLECMDID_ZOOM,
							OLECMDEXECOPT_DONTPROMPTUSER,
							&vaZoomFactor,
							NULL);
			pCmdTarg->Release();
		}
		pDisp->Release();
}	}*/


	ENSURE(m_pBrowserApp != NULL);
	HRESULT hr = E_FAIL;
	CComPtr<IDispatch> spDispDocument;
	spDispDocument=GetHtmlDocument();
	CComQIPtr<IHTMLDocument2> spDoc = spDispDocument;
	if(spDoc != NULL)
	{
		CComQIPtr<IOleCommandTarget> spCmdTarget = spDoc;
		if(spCmdTarget != NULL)
		{	COleVariant vaZoomFactor;   // input argument
			V_VT(&vaZoomFactor) = VT_I4;
			V_I4(&vaZoomFactor) = fontSize;   // 0 - 4			
			hr = spCmdTarget->Exec(NULL, OLECMDID_ZOOM,
				OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
	}	}	}


// CSinoSHlp32View diagnostics

#ifdef _DEBUG
void CSinoSHlp32View::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CSinoSHlp32View::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CSinoSHlp32Doc* CSinoSHlp32View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSinoSHlp32Doc)));
	return (CSinoSHlp32Doc*)m_pDocument;
}
#endif //_DEBUG


// CSinoSHlp32View message handlers
