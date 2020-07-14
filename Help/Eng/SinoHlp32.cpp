// SinoSHlp32.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SinoHlp32.h"
#include "MainFrm.h"

#include "SinoHlp32Doc.h"
#include "LeftView.h"
#include "SinoHlp32View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSinoSHlp32App

BEGIN_MESSAGE_MAP(CSinoSHlp32App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSinoSHlp32App::OnAppAbout)
	//My appends:
	ON_COMMAND(ID_CONTENTS, &CSinoSHlp32App::OnContentsBtnPressed)
	ON_COMMAND(ID_INDEX, &CSinoSHlp32App::OnIndexBtnPressed)
	ON_COMMAND(ID_SEARCH, &CSinoSHlp32App::OnSearchBtnPressed)
	ON_COMMAND(ID_PREV, &CSinoSHlp32App::OnPrevBtnPressed)
	ON_COMMAND(ID_NEXT, &CSinoSHlp32App::OnNextBtnPressed)
	ON_COMMAND(ID_PREV_STEP, &CSinoSHlp32App::OnPrevStepBtnPressed)
	ON_COMMAND(ID_NEXT_STEP, &CSinoSHlp32App::OnNextStepBtnPressed)
	ON_COMMAND(ID_FIND, &CSinoSHlp32App::OnFindTextBtnPressed)
	ON_COMMAND(ID_FILE_PRINT, &CSinoSHlp32App::OnFilePrint)
	ON_COMMAND(IDM_EDIT_COPY,  &CSinoSHlp32App::OnEditCopy)
	ON_COMMAND(ID_EDIT_SELECT_ALL,&CSinoSHlp32App::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_CLEARSELECTION,&CSinoSHlp32App::OnClearSelection)
	/*ON_COMMAND(ID_FONTS_SMALLEST,&CSinoSHlp32App::OnFontsSmallest)
	ON_COMMAND(ID_FONTS_SMALL,&CSinoSHlp32App::OnFontsSmall)
	ON_COMMAND(ID_FONTS_MEDIUM,&CSinoSHlp32App::OnFontsMedium)
	ON_COMMAND(ID_FONTS_LARGE,  &CSinoSHlp32App::OnFontsLarge)
	ON_COMMAND(ID_FONTS_LARGEST,  &CSinoSHlp32App::OnFontsLargest)*/
END_MESSAGE_MAP()


// CSinoSHlp32App construction
CSinoSHlp32App::CSinoSHlp32App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSinoSHlp32App object

CSinoSHlp32App theApp;


// CSinoSHlp32App initialization

BOOL CSinoSHlp32App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSinoSHlp32Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	m_pMainWnd->SetWindowTextA("'Sino' help system");//"Sino help queue");
	m_pMainWnd->SetIcon(LoadIcon(MAKEINTRESOURCE(IDI_ICON_QUEST)),TRUE);
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	m_lpCmdLine = GetCommandLine();

	int loadPageNumFromCmndArg=atoi(m_lpCmdLine);
	if(loadPageNumFromCmndArg>0 && loadPageNumFromCmndArg<MAX_HTMLS)
		listView->SetPage(contents,loadPageNumFromCmndArg);
	else if(loadPageNumFromCmndArg==-1)
		listView->SetPage(index,0);
	else if(loadPageNumFromCmndArg==-2)
		OnSearchBtnPressed();
	else if(loadPageNumFromCmndArg==-3)
		listView->SetPage(contents,3);//IDR_HTML_LICENSE_CHM);
	else
	{	char *p = strchr(m_lpCmdLine,' ');
		if(p)
		{	loadPageNumFromCmndArg=atoi(p+1);
			if(loadPageNumFromCmndArg>-1 && loadPageNumFromCmndArg<MAX_HTMLS)
				listView->SetPage(contents,loadPageNumFromCmndArg);
			else if(loadPageNumFromCmndArg==-1)
				listView->SetPage(index,0);
			else if(loadPageNumFromCmndArg==-2)
				OnSearchBtnPressed();
	}	}	
	return TRUE;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CSinoSHlp32App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CSinoSHlp32App::OnContentsBtnPressed()
{
	htmlView->beforeNavigateType = htmlView->fromContentsButton;
	listView->SetPage(contents,htmlView->iPage);
	mainFrame->ClearSearchResults();
}

void CSinoSHlp32App::OnIndexBtnPressed()
{
	htmlView->beforeNavigateType = htmlView->fromIndexButton;
	listView->SetPage(index,0);
	mainFrame->ClearSearchResults();
}

void  CSinoSHlp32App::OnSearchBtnPressed()
{
	htmlView->beforeNavigateType = htmlView->fromSearchButton;
	htmlView->OnSearchTextBtnPressed();
}

void CSinoSHlp32App::OnPrevBtnPressed()
{
	if(htmlView->OnPrevBtnPressed())
		mainFrame->EnableToolbarBtn(5,TRUE);//Next btn , calc with separator
	else
		mainFrame->EnableToolbarBtn(4,FALSE);//Prev btn;
}

void CSinoSHlp32App::OnNextBtnPressed()
{
	if(htmlView->OnNextBtnPressed())
		mainFrame->EnableToolbarBtn(4,TRUE);//Prev btn;
	else
		mainFrame->EnableToolbarBtn(5,FALSE);//Next btn
}

void CSinoSHlp32App::OnPrevStepBtnPressed()
{
	if(htmlView->OnPrevStepBtnPressed())
		mainFrame->EnableToolbarBtn(7,TRUE);//Next step btn , calc with separator
	else
	{
		mainFrame->EnableToolbarBtn(6,FALSE);//Prev step btn;
		mainFrame->EnableToolbarBtn(7,TRUE);//Next step btn;
}	}

void CSinoSHlp32App::OnNextStepBtnPressed()
{
	if(htmlView->OnNextStepBtnPressed())
		mainFrame->EnableToolbarBtn(6,TRUE);//Next step btn , calc with separator
	else
	{
		mainFrame->EnableToolbarBtn(6,TRUE);//Prev step btn , calc with separator
		mainFrame->EnableToolbarBtn(7,FALSE);//Next step btn;
}	}

void CSinoSHlp32App::OnFindTextBtnPressed()
{
	htmlView->OnFindTextBtnPressed();
}

// CSinoSHlp32App message handlers
BOOL CSinoSHlp32App::PreTranslateMessage(MSG *pMsg)
{
	if(0x100==pMsg->message && (0xd==pMsg->wParam || 0x1b==pMsg->wParam))// || (findTxtDlg && pMsg->hwnd==findTxtDlg))
	{	if(htmlView->findTxtDlg)
		if(pMsg->hwnd==GetDlgItem(htmlView->findTxtDlg,IDC_EDIT_FIND_TEXT))
		{	if(IsDialogMessage(htmlView->findTxtDlg, pMsg))
				return TRUE;
	}	}
	if(WM_USER==pMsg->message)
		htmlView->NavigatePage((int)pMsg->wParam);//,FALSE);
	return CWinApp::PreTranslateMessage(pMsg);
}

void CSinoSHlp32App::OnFilePrint()
{
	htmlView->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, 0, 0);
}

void CSinoSHlp32App::OnEditCopy()
{
	htmlView->ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, 0, 0);
}

void CSinoSHlp32App::OnEditSelectAll()
{
	htmlView->ExecWB(OLECMDID_SELECTALL, OLECMDEXECOPT_DONTPROMPTUSER, 0, 0);
}

void CSinoSHlp32App::OnFontsSmallest()
{
//	COleVariant vaZoomFactor(0l);
//	htmlView->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
	htmlView->ChangeFont(0);
}

void CSinoSHlp32App::OnFontsSmall()
{
//	COleVariant vaZoomFactor(1l);
//	htmlView->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
	htmlView->ChangeFont(1);
}

void CSinoSHlp32App::OnFontsMedium()
{
//	COleVariant vaZoomFactor(2l);
//	htmlView->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
	htmlView->ChangeFont(2);
}

void CSinoSHlp32App::OnFontsLarge()
{
//	COleVariant vaZoomFactor(3l);
//	htmlView->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
	htmlView->ChangeFont(3);
}

void CSinoSHlp32App::OnFontsLargest()
{
//	COleVariant vaZoomFactor(4l);
//	htmlView->ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, &vaZoomFactor, NULL);
	htmlView->ChangeFont(4);
}

void CSinoSHlp32App::OnClearSelection()
{
	htmlView->ExecWB(OLECMDID_CLEARSELECTION, OLECMDEXECOPT_DONTPROMPTUSER, 0, 0);
}