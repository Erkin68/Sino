//	Maqsad sari shu darajada shitob va shiddat bilan intilginki,
//	lozim bo'lsa mening gardanimga oyog'ingni qo'yib
//	murodga yetgil.
//										Bahouddin Naqshband.

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(linker, "/NODEFAULTLIB:\"LIBCMTD.lib\"")

#define ISOLATION_AWARE_ENABLED 1

#include "Sino.h"
#include <stdio.h>
#include "Math.h"
#include "Config.h"
#include "windows.h"
#include <time.h>
#include "strsafe.h"
#include "shlobj.h"
#include "Profiler.h"
#include "MyCommonControls.h"
#include "Disktoolbar.h"
#include "SheetTab.h"
#include "commctrl.h"
//#include <gdiplus.h>
#include "Operations\RenMove.h"
#include "Operations\Language.h"
#include "Operations\Language.h"
#include "Operations\MyErrors.h"
#include "Operations\LinkSocket.h"
#include "Operations\FolderSize.h"
#include "Operations\CreateFolder.h"
#include "Operations\Execution.h"
#include "Operations\MenuUtils.h"
#include "Operations\VirtualPanel.h"
#include "Operations\MyShell\ComboToDisk.h"
#include "Operations\ButtonsPanel.h"
#include "Operations\BtnBar.h"
#include "Operations\CopyOperation.h"
#include "Operations\DeleteOperation.h"
#include "Operations\qView.h"
#include "Operations\KernelInUserSpace.h"
#include "Operations\MyShell\MyShell.h"
//#include "Operations\MyShell\MyButtonC++.h"
#include "WindowsManagmentInstrumentation.h"
#include "Plugins\Archive\Archive.h"
#include "Plugins\Executables\viewFileAssoc.h"
#include "Plugins\Executables\UtilityExe.h"
#include "License.h"

//using namespace Gdiplus;


#define MAX_LOADSTRING		100
#define CLOCK_TIMER_ID		1112
#define CLOCK_TIMER_ID_IMP	1113

extern LPDIRECT3D9 pD3D;
extern HWND menuBarHwnd;
extern HWND btns[7];//extern MyButton btns[7];
extern BOOL bFinEfct;//from SinoLib
RECT   rcClient;

HWND		hWnd=0,cmndsCB,cmndsCBEdt=0;
HINSTANCE	hInst;							// current instance
TCHAR		szTitle[MAX_LOADSTRING];		// The title bar text
TCHAR		szWindowClass[MAX_LOADSTRING];	// the main window class name
DWORD		dwTlsIndex;
DWORD		tickProced;

HFONT		hfCmdBtns=0;
HBRUSH		brCmdBtns=0;
HBRUSH		brHtBk=0;

CBToDisk arcCrNamesCB,pnlPthAndNamesCB[MAX_PANELS],cmnCmndsCB,selPlusCB1,
		 selPlusCB,cmdCB,btnPnlPropIconPthCB,btnPnlPropPthCB;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL				IsFinishInputMessage(UINT);
//BOOL				CreateMenuBar(HINSTANCE,HWND,HDC);
int					ReplaceDirectFolderPanels(int);
int					CallCommandConsole(int);
VOID				DrawClock(HDC);//HWND);
VOID				CreateNeedDirs();
BOOL				CheckRegistryForSino();
BOOL				CheckAdminPrivilegy();
BOOL				TrySetAdminPrivilegy();
BOOL				CheckOSBitRate();


typedef struct _DllVersionInfo{DWORD cbSize;DWORD dwMajorVersion;DWORD dwMinorVersion;DWORD dwBuildNumber;DWORD dwPlatformID;}DLLVERSIONINFO;
typedef HRESULT (CALLBACK *DllGetVersion_t)(DLLVERSIONINFO*);
DllGetVersion_t DllGetVersion=NULL;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
//int nn=sizeof(PanelItem);

	/*InitCommonControls();
	HMODULE hm=LoadLibrary(L"comctl32.dll");
	if(hm)
	{DllGetVersion=(DllGetVersion_t)GetProcAddress(hm,"DllGetVersion");
	 if(DllGetVersion)
	 {DLLVERSIONINFO di;di.cbSize=sizeof(di);
	  DllGetVersion(&di);
	  if(di.dwMajorVersion<6)MessageBox(NULL,L"CommCtrl vers.:<6",L"",MB_OK);
	}}*///DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_FILE_ASSOC_VIEW), NULL, (DLGPROC)viewFileAssoc::confDlgProc,2);

MSG msg={0,0,0};
DWORD sz=MAX_PATH;//,rType;

	if(!CheckOSBitRate())
		return 0;

	if(!CheckRegistryForSino())
		return 0;

	if(!License::OnEntWinMain(hInstance))
		return 0;

	if(!CheckAdminPrivilegy())
	{	if(!TrySetAdminPrivilegy())
			return 0;
	}

	conf::Read(MyStringAddModulePath(L"Config\\Conf.bin"));
	if(!conf::CheckPrevWindIns()) return 0;

	CreateNeedDirs();

	WSADATA ws;
	WSAStartup(MAKEWORD(2,2),&ws);
	
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//GdiplusStartupInput gdiplusStartupInput;ULONG_PTR gdiplusToken;
	//GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	//image = new Image(L"J0145272.jpg");

	hInst = hInstance;

	//if(!pD3D)//1 for all threads:
	//{	/*if( NULL == (*/ pD3D = Direct3DCreate9( D3D_SDK_VERSION );// ) )
			//return E_FAIL;
	//}
	//if(pD3D)
	//{	D3DCAPS9 dcaps;pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&dcaps);
	//	conf::MaxTextureWidth = dcaps.MaxTextureWidth;
	//	conf::MaxTextureHeight = dcaps.MaxTextureHeight;
	//}

	dwTlsIndex = TlsAlloc(); if(TLS_OUT_OF_INDEXES == dwTlsIndex) Err::msg(0,0,L"TlsAlloc");

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_SINO, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if(!InitInstance (hInstance, nCmdShow)) goto Fin;
	FolderSize::hThrd = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)FolderSize::mainThrd,0,0,&FolderSize::thrdId);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDS_SINO));
	ZeroMemory( &msg, sizeof(msg) );

	//Idle processniyam taymerda qilib GetMessagega o'tamiz:
	SetTimer(hWnd,102,1000,NULL);//250 edi

    while( msg.message!=WM_QUIT )
    {	
		if(GetMessage( &msg, NULL, 0U, 0U ))//if(PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{	
			/*static int i=0;
			wchar_t ss[32];wsprintf(ss,L"\n %d ",i++);
			OutputDebugStringW(ss);
			OutputDebugStringA(GetWinNotifyText(msg.message));*/

			//if(!ProcessMessage(&msg))    !!!!!!!!
			{	TranslateMessage(&msg);//Agar olib tashlasang, EDIT lar ishlamaydur!!!!
				DispatchMessage(&msg);
			}

			if(msg.hwnd)
			{	for(int p=0; p<MAX_PANELS; p++)
				{	if(panel[p].OtherControlMsgQueue(&msg))
						break;
				}
				if(cmndsCBEdt)
				if(cmndsCBEdt==msg.hwnd)
					CmnCntrl::CmndsCBMsgQueue(&msg);
			}
			/*if(msg.message==WM_KEYDOWN)
			{	static int i=0;
				char ss[32];sprintf(ss,"\n %d ",i++);
				//OutputDebugString(ss);
				//OutputDebugString(GetWinNotifyText(msg.message));
				sprintf(ss," %x %x",msg.wParam,msg.lParam);
				//OutputDebugString(ss);
				sprintf(ss," cmndsCB:%x msg.hWnd:%x",cmndsCB,msg.hwnd);
				//OutputDebugString(ss);
	}*/	}	}

Fin:
	if(!conf::bCancelAnimation[0])
	if(!bFinEfct)
	if(efctTLSs[0])
	if(efctTLSs[0]->evnts[0])
	{	SetEvent(efctTLSs[0]->evnts[0]);
		WaitForSingleObject(efctThrdHandle[0],INFINITE);
	}
	TlsFree(dwTlsIndex);
	//if(pD3D)pD3D->Release();

	if(FolderSize::hThrd)
	{	PostThreadMessage(FolderSize::thrdId,WM_USER+1,0,0);
		WaitForSingleObject(FolderSize::hThrd,5000);
	}

	ProfilerEnd
	WSACleanup();

	//delete image;
	//GdiplusShutdown(gdiplusToken);

	return (msg.message?(int)msg.wParam:0);
}

void MainIdleProcess()
{
	tickProced = GetTickCount();
	BOOL bNotifyYes = FALSE;
	//if(HIWORD(GetQueueStatus(QS_ALLEVENTS)) != 0)
	{	for(int p=0; p<conf::Dlg.iTotPanels; p++)
		{	if(panel[p].AnalyzeDirChangeNotify())
			{	bNotifyYes = TRUE;
				break;
	}	}	}
	//else
	//if(!bNotifyYes)
	{	//if(dIddleTime > (DWORD)(IsFinishInputMessage(msg.message)?0:100))//Oxirgisi input edi:
		{	for(int p=0; p<conf::Dlg.iTotPanels; p++)
			{	if(!panel[p].GetRenderFull())
					panel[p].RenderFullIcons();
		}	}
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
		{	if(forSizeUp!=panel[p].fileSortType)
			if(forSizeDwn!=panel[p].fileSortType)break;
			panel[p].CalcFoldersSizes();
}	}	}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SINO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDM_MENUBAR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SINO));;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

extern DWORD efctThrdID[NUM_EFFECT_THREADS];
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
int l=conf::wndLeft,t=conf::wndTop,w=conf::wndWidth,h=conf::wndHeight;
	hWnd = CreateWindowEx(	WS_EX_TRANSPARENT|WS_EX_LAYERED|WS_EX_OVERLAPPEDWINDOW|WS_EX_CONTROLPARENT,//|WS_EX_ACCEPTFILES,
							szWindowClass, szTitle,
							WS_OVERLAPPEDWINDOW,
							l,//-l,
							t,//-t,
							w,
							h,
							NULL,
							NULL,
							hInstance,
							NULL);
	if (!hWnd)	return FALSE;
	//Bunday bo'ldi:
	SetLayeredWindowAttributes(hWnd,0,0,LWA_ALPHA);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MENUINFO mi;mi.cbSize=sizeof(MENUINFO);mi.fMask=MIM_BACKGROUND;
	if(GetMenuInfo(GetMenu(hWnd),&mi))
	{	mi.hbrBack = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		SetMenuInfo(GetMenu(hWnd),&mi);
	}

	/*if(!conf::bCancelAnimation[0])
	{	HDC bmpDC; HBITMAP windImBMP = conf::ReadWindImgFrDisk("Wind.img", &bmpDC);
		if(windImBMP)
			//StartAndWaitEffect(0,windImBMP,bmpDC);
			StartAndWaitEffect(0,windImBMP,bmpDC);
		else conf::bCancelAnimation[0] = TRUE;
	}   edi; ******/

	HDC dc = GetWindowDC(hWnd);
	if(!dc) return false;
	HDC memDC = CreateCompatibleDC(dc);
	if(!memDC) return false;
	HBITMAP memBM = CreateCompatibleBitmap(dc,w,h);
	HGDIOBJ oldObj = SelectObject(memDC, memBM);
	PBITMAPINFO bmi = CreateBitmapInfoStruct(hWnd, memBM);
	BitBlt(memDC,0,0,w,h,dc,0,0,SRCCOPY);
	//CreateBMPFile(hWnd,"dsdf.bmp",bmi,memBM,memDC);
//	if(!conf::bCancelAnimation[0])
//		StartAndWaitEffect(1,memBM,memDC);
	SelectObject(memDC, oldObj);
	DeleteDC(memDC);
	DeleteObject(memBM);
	ReleaseDC(hWnd,dc);


	//SetLayeredWindowAttributes(hWnd,0,255,LWA_ALPHA);
	SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_OVERLAPPEDWINDOW|WS_EX_CONTROLPARENT);
	SetForegroundWindow(hWnd);
	//MoveWindow(hWnd,l,t,w,h,TRUE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	Sleep(250);			

	if((!conf::bCancelAnimation[0]) && efctTLSs[0])
	{	SetEvent(efctTLSs[0]->waitForShowParentEvnt);
		PostThreadMessage(efctThrdID[0],WM_QUIT,0,0);
	}
	return TRUE;
}

VOID OnToolbarDiskBtnClicked(int p,int driveNum)
{
int oldDsk = DskToolBar::FindDiskFromPath(panel[p].GetPath());
BOOL bSameDsk=FALSE;
	if(oldDsk==driveNum)
	{	if(++DskToolBar::iSameDskPushd[driveNum]<1)
			return;
		else bSameDsk = TRUE;
	} DskToolBar::iSameDskPushd[driveNum]=0;

	int otherPanelNum = 0;
	switch(p)
	{	case 0: otherPanelNum=1; break;
		case 1: otherPanelNum=0; break;
		case 2: otherPanelNum=1; break;
		case 3: otherPanelNum=2; break;
	}
	//Save old path;
	wchar_t oldPth[MAX_PATH]=L"";
	if(directFolder==panel[p].GetEntry()->GetCrntRecType())
		MyStringCpy(oldPth,MAX_PATH-1,panel[p].GetPath());
	if(virtualPanel==panel[p].GetEntry()->GetCrntRecType())
	{	vrtlPanels::plgns[panel[p].GetiVPPlg()].DetachPanel$8(panel[p].GetVPPlgObj(),&panel[p]);
		vrtlPanels::plgns[panel[p].GetiVPPlg()].FreePlugin();
	}

	//Clear all panel items:
	panel[p].FreeSelection();
	panel[p].FreeMem();
	panel[p].GetEntry()->Clear();

#define othPan panel[otherPanelNum]
	if((!bSameDsk) &&
	   directFolder==othPan.GetEntry()->GetCrntRecType() &&
	   driveNum==DskToolBar::FindDiskFromPath(othPan.GetPath()))//Diski 1 xil;
	{	panel[p].SetPath(othPan.GetPath(),MAX_PATH);
		panel[p].GetEntry()->CopyFrom(panel[otherPanelNum].GetEntry());
	} else
	{	if(bSameDsk || 0==DskToolBar::oldPathsOnDiskPanels[driveNum][0])
		{	if(panel[p].GetPath()[0]==DskToolBar::logDiskNames[driveNum][0] &&
			   panel[p].GetPath()[1]==':' && panel[p].GetPath()[2]=='\\' &&
			   panel[p].GetPath()[3]=='*' && panel[p].GetPath()[4]==0)
			{  panel[p].SetPath(panel[otherPanelNum].GetPath(),
								 panel[otherPanelNum].GetPathLn());
			}
			else
			{	panel[p].GetPath()[0]=DskToolBar::logDiskNames[driveNum][0];//-32;
				panel[p].GetPath()[1]=':';panel[p].GetPath()[2]='\\';
				panel[p].GetPath()[3]='*';panel[p].GetPath()[4]=0;
				panel[p].SetPathLn(4);
			}
			panel[p].GetEntry()->SetPushEntryInStartup(panel[p].GetPath());
			panel[p].GetEntry()->Push(panel[p].GetPath(),directFolder,0);
		} else //restore from old:
		{	panel[p].SetPathLn(
				MyStringCpy(panel[p].GetPath(),MAX_PATH-1,DskToolBar::oldPathsOnDiskPanels[driveNum]));
			panel[p].GetEntry()->SetPushEntryInStartup(panel[p].GetPath());
			panel[p].GetEntry()->Push(panel[p].GetPath(),directFolder,0);
	}	}
	//KillHotAllWithout(p);
	panel[p].FillDirectFolder();
	//MySetCurrentDirectory(st);

	//panel[p].Sort();Fillda turibdi;
	panel[p].ChangeSheetTabPath();
	panel[p].AdjustScrollity();
	panel[p].SetHot(0);
	panel[p].ClrScr();
	panel[p].Render();
	CmnCntrl::DrawDskSpace(NULL,p);

	//Save old path to disk path stack:
	if(oldDsk>-1)
		MyStringCpy(DskToolBar::oldPathsOnDiskPanels[oldDsk],MAX_PATH-1,oldPth);
#undef othPan
}

VOID OnToolbarDiskBtnRClicked(int p, int driveNum, POINT *pt)
{
/*int otherPanelNum = 0;
	switch(p)
	{	case 0: otherPanelNum=1; break;
		case 1: otherPanelNum=0; break;
		case 2: otherPanelNum=1; break;
		case 3: otherPanelNum=2; break;
	}
	//Clear all panel items:
	panel[p].DeselectAllWithout(-1);
	panel[p].FreeMem();
	panel[p].entry.Clear();

	BOOL bFindRoot = FALSE;
#define othPan panel[otherPanelNum]
	if(directFolder==othPan.entry.GetCrntRecType()==panel[p].entry.GetCrntRecType())
	if(othPan.path[0]==logDiskNames[driveNum][0]-32 ||//Diski 1 xil;
       othPan.path[0]==logDiskNames[driveNum][0])
		bFindRoot = TRUE;
	if(bFindRoot)
	{	MyStringCpy(panel[p].path,MAX_PATH-1,othPan.path);
		panel[p].entry.CopyFrom(&panel[otherPanelNum].entry);
	} else
	{	panel[p].path[0]=logDiskNames[driveNum][0];//-32;
		panel[p].path[1]=':';panel[p].path[2]='\\';
		panel[p].path[3]='*';panel[p].path[4]=0;
		panel[p].pathLn=4;
		panel[p].entry.Push(panel[p].path,directFolder,0);
	}
	//KillHotAllWithout(p);
	panel[p].fManager.FillDirectFolder();
	//MySetCurrentDirectory(st);

	//panel[p].Sort();Fillda turibdi;
	panel[p].AdjustScrollity();
	panel[p].AddToSelection(0,3,FALSE);
	panel[p].ClrScr();
	panel[p].Render();
	panel[p].oldSelectId = 0;
#undef othPan*/
	myWMI::DoExplorerMenu(hWnd,myWMI::GetLogicalDriveName(driveNum),pt);
}

VOID DoSystemTray(int type, HICON hi)
{
NOTIFYICONDATA Nim;Nim.cbSize = sizeof(Nim);
	Nim.hWnd = hWnd;
	Nim.uID = 1;
	Nim.uFlags = hi?(NIF_ICON | NIF_MESSAGE | NIF_TIP):(NIF_MESSAGE | NIF_TIP);
	Nim.hIcon = hi;
	Nim.uCallbackMessage = WM_USER;
	Nim.szTip[0]='S';Nim.szTip[1]='i';Nim.szTip[2]='n';
	Nim.szTip[3]='o';Nim.szTip[4]=0;
	switch(type)
	{	case 0:Shell_NotifyIcon(NIM_ADD,&Nim);
			break;
		case 1:Shell_NotifyIcon(NIM_DELETE,&Nim);
			break;
		case 2:Shell_NotifyIcon(NIM_MODIFY,&Nim);
			break;
}	}

LRESULT CALLBACK aboutDlgProc(HWND dlg, UINT message, WPARAM wParam, LPARAM lParam)
{
#pragma warning ( disable : 4309)
wchar_t s[MAX_PATH];int i;static DWORD dwAboutRollCntr;
	switch(message)
	{	case WM_INITDIALOG:
			LoadString(hInst,IDS_STRINGSW_14,s,MAX_PATH);
			SetWindowText(dlg,s);
			if(!wcscmp(s,L"Авторы ..."))
			{	wchar_t s1[8]={0x0411,0x0433,0x0443,0x043F,0x0441,0x044C,0x3B,0x01};for(i=0;i<8;i++)--s1[i];
				SetDlgItemText(dlg,IDC_STATIC1,s1);//"Авторы:");
				wchar_t s2[64]={0x0420,0x0441,0x043F,0x0434,0x0441,0x0431,0x043D,0x043D,0x0439,0x0442,0x0443,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x042E,0x0441,0x043B,0x0439,0x043E,0x21,0x0422,0x0431,0x0443,0x0443,0x043F,0x0441,0x0434,0x0431,0x2F,0x01};for(i=0;i<64;i++)--s2[i];
				SetDlgItemText(dlg,IDC_STATIC2,s2);//"Программист:                                   Эркин Сатторий.");
//				wchar_t s7[49]={0x041e,0x043f,0x0433,0x044c,0x436,0x21,0x0439,0x0435,0x0436,0x0439,0x21,0x0439,0x21,0x78,0x78,0x78,0x2e,0x43f,0x441,0x434,0x431,0x43e,0x439,0x438,0x431,0x447,0x439,0x450,0x3b,0x21,0x21,0x21,0x21,0x21,0x0421,0x0431,0x0433,0x0449,0x0431,0x43e,0x21,0x041b,0x0439,0x0452,0x043d,0x043f,0x0433,0x2F,0x01};for(i=0;i<49;i++)--s7[i];
				SetDlgItemText(dlg,IDC_STATIC7,L"");//s7);//"Новые идеи и www-организация:                  Равшан Киёмов.");
//				wchar_t s3[38]={0x0426,0x0444,0x0435,0x043F,0x0437,0x0436,0x0442,0x0443,0x0433,0x0436,0x043E,0x043E,0x043F,0x0436,0x21,0x043F,0x0445,0x043F,0x0441,0x043D,0x043C,0x0436,0x043E,0x0439,0x0436,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<38;i++)--s3[i];
				SetDlgItemText(dlg,IDC_STATIC3,L"");//s3);//"Художественное оформление:                  ...");
//				wchar_t s4[52]={0x0423,0x0436,0x0442,0x0443,0x0439,0x0441,0x043F,0x0433,0x0431,0x043E,0x0439,0x0436,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<52;i++)--s4[i];
				SetDlgItemText(dlg,IDC_STATIC4,L"");//s4);//"Тестирование:                               ...");
//				wchar_t s5[44]={0x0422,0x043F,0x0438,0x0435,0x0431,0x043E,0x0439,0x0436,0x21,0x0442,0x0440,0x0441,0x0431,0x0433,0x043F,0x0448,0x043E,0x0439,0x043B,0x0431,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<44;i++)--s5[i];
				SetDlgItemText(dlg,IDC_STATIC5,L"");//s5);//"Создание справочника:                       ...");
				wchar_t s6[32]={0x041B,0x0431,0x0441,0x0431,0x0444,0x043C,0x0432,0x0431,0x0438,0x0431,0x0441,0x2D,0x21,0x0424,0x0438,0x0432,0x0436,0x043B,0x0439,0x0442,0x0443,0x0431,0x043E,0x21,0x2D,0x21,0x33,0x31,0x32,0x35,0x2F,0x01};for(i=0;i<32;i++)--s6[i];
				SetDlgItemText(dlg,IDC_STATIC6,s6);//"Караулбазар, Узбекистан - 2014.");
			}
			else if(!wcscmp(s,L"Mualliflar ..."))
			{	wchar_t s1[12]={0x4E,0x76,0x62,0x6D,0x6D,0x6A,0x67,0x6D,0x62,0x73,0x3B,0x01};for(i=0;i<12;i++)--s1[i];
				SetDlgItemText(dlg,IDC_STATIC1,s1);//"Mualliflar:");
				wchar_t s2[65]={0x51,0x72,0x70,0x68,0x73,0x62,0x6E,0x6E,0x6A,0x74,0x75,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x46,0x73,0x6C,0x6A,0x6F,0x21,0x54,0x62,0x75,0x75,0x70,0x73,0x6a,0x7a,0x2F,0x01};for(i=0;i<65;i++)--s2[i];
				SetDlgItemText(dlg,IDC_STATIC2,s2);//"Programmist:                                   Erkin Sattoriy.");
//				wchar_t s7[54]={0x5a,0x62,0x6f,0x68,0x6a,0x21,0x68,0x28,0x70,0x7a,0x62,0x6d,0x62,0x73,0x21,0x77,0x62,0x21,0x78,0x78,0x78,0x2e,0x75,0x62,0x74,0x69,0x6c,0x6a,0x6d,0x21,0x66,0x75,0x6a,0x74,0x69,0x3b,0x21,0x53,0x62,0x77,0x74,0x69,0x62,0x6f,0x21,0x52,0x6a,0x7a,0x70,0x6e,0x70,0x77,0x2F,0x01};for(i=0;i<54;i++)--s7[i];
				SetDlgItemText(dlg,IDC_STATIC7,L"");//s7);//"Yangi g'oyalar va www-tashkil etish:           Ravshan Qiyomov.");
//				wchar_t s3[52]={0x43,0x62,0x65,0x6A,0x6A,0x7A,0x21,0x63,0x66,0x7B,0x62,0x74,0x69,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<52;i++)--s3[i];
				SetDlgItemText(dlg,IDC_STATIC3,L"");//s3);//"Badiiy bezash:                              ...");
//				wchar_t s4[49]={0x54,0x6A,0x6F,0x70,0x77,0x65,0x62,0x6F,0x21,0x70,0x28,0x75,0x6C,0x62,0x7B,0x6A,0x74,0x69,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<49;i++)--s4[i];
				SetDlgItemText(dlg,IDC_STATIC4,L"");//s4);//"Sinovdan o'tkazish:                         ...");
//				wchar_t s5[47]={0x5A,0x70,0x73,0x65,0x62,0x6E,0x21,0x65,0x62,0x74,0x75,0x76,0x73,0x6A,0x6F,0x6A,0x21,0x75,0x76,0x7B,0x6A,0x74,0x69,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<47;i++)--s5[i];
				SetDlgItemText(dlg,IDC_STATIC5,L"");//s5);//"Yordam dasturini tuzish:                    ...");
				wchar_t s6[34]={0x52,0x70,0x73,0x70,0x77,0x76,0x6D,0x63,0x70,0x7B,0x70,0x73,0x2D,0x21,0x50,0x28,0x7B,0x63,0x66,0x6C,0x6A,0x74,0x75,0x70,0x6F,0x21,0x2E,0x21,0x33,0x31,0x32,0x35,0x2F,0x01};for(i=0;i<34;i++)--s6[i];
				SetDlgItemText(dlg,IDC_STATIC6,s6);//"Qorovulbozor, O'zbekiston - 2014.");
			}
			else if(!wcscmp(s,L"Муаллифлар ..."))
			{	wchar_t s1[12]={0x041D,0x0444,0x0431,0x043C,0x043C,0x0439,0x0445,0x043C,0x0431,0x0441,0x003B,0x01};for(i=0;i<12;i++)s1[i]-=1;
				SetDlgItemTextW(dlg,IDC_STATIC1,s1);//"Муаллифлар:");
				wchar_t s2[64]={0x0420,0x0441,0x043F,0x0434,0x0441,0x0431,0x043D,0x043D,0x0439,0x0442,0x0443,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x042E,0x0441,0x043B,0x0439,0x043E,0x21,0x0422,0x0431,0x0443,0x0443,0x043F,0x0441,0x0434,0x0431,0x2F,0x01};for(i=0;i<64;i++)--s2[i];
				SetDlgItemText(dlg,IDC_STATIC2,s2);//"Программист:                                    Эркин Сатторий.");
//				wchar_t s7[48]={0x430,0x43e,0x434,0x439,0x21,0x494,0x43f,0x450,0x43c,0x431,0x441,0x21,0x433,0x431,0x21,0x78,0x78,0x78,0x2e,0x443,0x431,0x449,0x43b,0x439,0x43c,0x21,0x44e,0x443,0x439,0x449,0x3b,0x21,0x21,0x421,0x431,0x433,0x449,0x431,0x43e,0x21,0x49b,0x439,0x452,0x43d,0x43f,0x433,0x2F,0x01};for(i=0;i<48;i++)--s7[i];
				SetDlgItemText(dlg,IDC_STATIC7,L"");//s7);//"Янги гоялар ва www-ташкил этиш:                 Равшан Киёмов.");
//				wchar_t s3[51]={0x0412,0x0431,0x0435,0x0439,0x0439,0x043A,0x21,0x0432,0x0436,0x0438,0x0431,0x0449,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<51;i++)--s3[i];
				SetDlgItemText(dlg,IDC_STATIC3,L"");//s3);//"Бадиий безаш:                               ...");
//				wchar_t s4[47]={0x0422,0x0439,0x043E,0x043F,0x0433,0x0435,0x0431,0x043E,0x21,0x045F,0x0443,0x043B,0x0431,0x0438,0x0439,0x0449,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<47;i++)--s4[i];
				SetDlgItemText(dlg,IDC_STATIC4,L"");//s4);//"Синовдан ўтказиш:                           ...");
//				wchar_t s5[43]={0x0402,0x0441,0x0435,0x0431,0x043D,0x21,0x0435,0x0431,0x0442,0x0443,0x0444,0x0441,0x0439,0x043E,0x0439,0x21,0x0443,0x0444,0x0438,0x0439,0x0449,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<43;i++)--s5[i];
				SetDlgItemText(dlg,IDC_STATIC5,L"");//s5);//"Ёрдам дастурини тузиш:                      ...");
				wchar_t s6[33]={0x049B,0x043F,0x0441,0x043F,0x0433,0x0444,0x043C,0x0432,0x043F,0x0438,0x043F,0x0441,0x2D,0x21,0x040F,0x0438,0x0432,0x0436,0x043B,0x0439,0x0442,0x0443,0x043F,0x043E,0x21,0x2E,0x21,0x33,0x31,0x32,0x35,0x2F,0x01};for(i=0;i<33;i++)--s6[i];
				SetDlgItemText(dlg,IDC_STATIC6,s6);//"Қоровулбозор, Ўзбекистон - 2014.");
			}
			else//if(!wcscmp(s,L"About ...")) all others in engl version:
			{	wchar_t s1[9]={0x42,0x76,0x75,0x69,0x70,0x73,0x74,0x3B,0x01};for(i=0;i<9;i++)--s1[i];
				SetDlgItemText(dlg,IDC_STATIC1,s1);//"Authors:");
				wchar_t s2[62]={0x51,0x73,0x70,0x68,0x73,0x62,0x6E,0x6E,0x66,0x73,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x46,0x73,0x6C,0x6A,0x6F,0x21,0x54,0x62,0x75,0x75,0x70,0x73,0x6a,0x7a,0x2F,0x01};for(i=0;i<62;i++)--s2[i];
				SetDlgItemTextW(dlg,IDC_STATIC2,s2);//"Programmer:                                   Erkin Sattoriy.");
//				wchar_t s7[53]={0x4f,0x66,0x78,0x21,0x6a,0x65,0x66,0x62,0x74,0x21,0x62,0x6f,0x65,0x21,0x78,0x78,0x78,0x2e,0x68,0x76,0x6a,0x65,0x66,0x3b,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x53,0x62,0x77,0x74,0x69,0x62,0x6f,0x21,0x4c,0x6a,0x7a,0x70,0x6e,0x70,0x77,0x2F,0x01};for(i=0;i<53;i++)--s7[i];
				SetDlgItemText(dlg,IDC_STATIC7,L"");//s7);//"Yangi g'oyalar va www-tashkil etish:           Ravshan Kiyomov.");
//				wchar_t s3[48]={0x49,0x66,0x6D,0x71,0x21,0x67,0x70,0x73,0x21,0x62,0x73,0x75,0x21,0x65,0x66,0x74,0x6A,0x68,0x6F,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<48;i++)--s3[i];
				SetDlgItemTextW(dlg,IDC_STATIC3,L"");//s3);//"Help for art design:                        ...");
//				wchar_t s4[54]={0x55,0x66,0x74,0x75,0x6A,0x6F,0x68,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<54;i++)--s4[i];
				SetDlgItemTextW(dlg,IDC_STATIC4,L"");//s4);//"Testing:                                    ...");
//				wchar_t s5[56]={0x43,0x76,0x6A,0x6D,0x65,0x21,0x69,0x66,0x6D,0x71,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x2F,0x2F,0x2F,0x01};for(i=0;i<56;i++)--s5[i];
				SetDlgItemTextW(dlg,IDC_STATIC5,L"");//s5);//"Build help:                                 ...");
				wchar_t s6[32]={0x4C,0x62,0x73,0x62,0x76,0x6D,0x63,0x62,0x7B,0x62,0x73,0x2D,0x21,0x56,0x7B,0x63,0x66,0x6C,0x6A,0x74,0x75,0x62,0x6F,0x21,0x2E,0x21,0x33,0x31,0x32,0x35,0x2F,0x01};for(i=0;i<32;i++)--s6[i];
				SetDlgItemTextW(dlg,IDC_STATIC6,s6);//"Karaulbazar, Uzbekistan - 2014.");
			}
			//MyButtonFrRCBtn(dlg,IDOK,0);
			//HWND rollWnd;rollWnd=CreateWindow(L"STATIC",L"RollWnd",WS_CHILD,60,160,150,150,dlg,0,hInst,0);
#define IDTimer_AboutRoll 1
			SetTimer(dlg,IDTimer_AboutRoll,50,NULL);
			dwAboutRollCntr=0;
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{	case IDOK:
				case IDCANCEL:
					EndDialog(dlg,0);
					break;
			}
			break;
		case WM_DESTROY:
			KillTimer(dlg,IDTimer_AboutRoll);
			break;
#undef IDTimer_AboutRoll
		case WM_TIMER:int deI=0;
			dwAboutRollCntr+=1;
			if(dwAboutRollCntr>1700)dwAboutRollCntr=0;
			HDC dc;dc = GetDC(dlg);SetBkMode(dc,TRANSPARENT);
#define TOPROLL  90
#define DOWNROLL 395
#define DRW(x) rc.top=de+20*(deI++);rc.bottom=rc.top+20;if(rc.top<TOPROLL)rc.top=TOPROLL;\
			if(rc.bottom>DOWNROLL)rc.bottom=DOWNROLL;\
			if(rc.bottom>=TOPROLL && rc.top<=DOWNROLL)\
				DrawText(dc,x,(int)wcslen(x),&rc,DT_LEFT|DT_SINGLELINE|(rc.top<=TOPROLL?DT_BOTTOM:(rc.bottom>=DOWNROLL?DT_TOP:DT_TOP)));
#define DRWC(x) rc.top=de+20*(deI++);rc.bottom=rc.top+20;if(rc.top<TOPROLL)rc.top=TOPROLL;\
			if(rc.bottom>DOWNROLL)rc.bottom=DOWNROLL;\
			if(rc.bottom>=TOPROLL && rc.top<=DOWNROLL)\
				DrawText(dc,x,(int)wcslen(x),&rc,DT_CENTER|DT_SINGLELINE|(rc.top<=TOPROLL?DT_BOTTOM:(rc.bottom>=DOWNROLL?DT_TOP:DT_TOP)));
				//DrawText(dc,x,wcslen(x),&rc,DT_LEFT|DT_SINGLELINE);

			RECT rc={5,TOPROLL,400,DOWNROLL};
			FillRect(dc,&rc,(HBRUSH)(COLOR_WINDOW));//GetStockObject(WHITE_BRUSH));
			int de=DOWNROLL-dwAboutRollCntr;
#ifdef _WIN64
			DRWC(L"Contents(X64):")
			DRW(L" ")
			DRW(L"Main executable, Sino64.exe:       1 640 448 b.")
			DRW(L" ")
			DRW(L"Main library,        MyShell64.dll:   2 386 944 b.")
			DRW(L"                         ")
			DRWC(L"Archive plugins:")
			DRW(L"For 7zip files,      7zipRel64.dll:    643 072 b.")
			DRW(L"For cabinet files, Cab64Rel.dll:     252 416 b.")
			DRW(L"For chm-files,      Chm64Rel.dll:    175 616 b.")
			DRW(L"For exe-files,       Exe64Rel.dll:     170 496 b.")
			DRW(L"For iso-files,        Iso64Rel.dll:      219 648 b.")
			DRW(L"For nrg-files,        Nrg64Rel.dll:     220 672 b.")
			DRW(L"For rar-files,         Rar64Rel.dll:     321 536 b.")
			DRW(L"For tar-files,         Tar64Rel.dll:      249 344 b.")
			DRW(L"For zip-files,        mzlib64rel.dll:    507 392 b.")
			DRW(L" ")
			DRWC(L"Auxilary plugins:")
			DRW(L"File-append plugin,               FileAppendRel64.dll:    99 328 b.")
			DRW(L"File-compare plugin,             FileCompareRel64.dll: 130 560 b.")
			DRW(L"File-defragmentation plugin, FileDefragRel64.dll:     80 384 b.")
			DRW(L"File-split plugin,                    FileSplitRel64.dll:            97 792 b.")
			DRW(L"First net adapter sniffer plugin,SnifAdpt0Rel64.dll:   203 776 b.")
			DRW(L" ")
			DRW(L"Main search plugin,   SinSch64Rel.dll:  208 896 b.")
			DRW(L" ")
			DRWC(L"Executable plugins:")
			DRW(L"Text editor,                       notepad++.exe:      796 672 b.")
			DRW(L"Binary editor,                   BinEditRel.exe:      75 776 b.")
			DRW(L"Binary viewer,                  BinViewerRel.exe: 78 336 b.")
			DRW(L"Html-files viewer,             htmViewerRel.exe: 61 440 b.")
			DRW(L"Image-files viewer,           imgViewRel64.exe:139 776 b.")
			DRW(L"Media player-files viewer,mpviewerRel.exe:  25 600 b.")
			DRW(L"PE(exe)-files viewer,        PEViewerRel.exe:   181 760 b.")
			DRW(L"Exel-files viewer,             Spread32.exe:         550 400 b.")
			DRW(L"Text-files viewer,             TextViewerRel.exe: 88 576 b.")
			DRW(L"Zip-viewer,                       zipviewer.exe:        263 168 b.")
			DRW(L"DDS-view library,DXTDecompressorSource.dll: 1325 056 b.")
			DRW(L" ")
			DRWC(L"Image view plugins:")
			DRW(L"Bmp,dib,ico,cur-files viewer, DibRel.dll:          17 408 b.")
			DRW(L"Gif-files viewer,                      GifRel.dll:           17 408 b.")
			DRW(L"Jpg-files viewer,                     jpgFrFileRel64.dll: 226 304 b.")
			DRW(L"Png-files viewer,                    pngFrFileRel.dll: 229 888 b.")
			DRW(L"Targa-files viewer,                 TargaRel.dll:       23 552 b.")
			DRW(L"Tif-files viewer,                      TifRel.dll:             244 224 b.")
			DRW(L"Dds-files viewer,                    DdsRel.dll:          16 384 b.")
			DRW(L" ")
			DRWC(L"Quick view plugins:")
			DRW(L"Binary files viewer,               binViewRel64.dll:       79 872 b.")
			DRW(L"Directory viewer,                   dirViewD9Rel64.dll:   107 008 b.")
			DRW(L"Image files viewer,                imgViewRel64.dll:      90 648 b.")
			DRW(L"MP files viewer(Direct Show),mpViewDSRel64.dll:  47 104 b.")
			DRW(L"PE(exe) files viewer,              peViewRel64.dll:        228 864 b.")
			DRW(L"Text files viewer,                   txtViewRel64.dll:        110 080 b.")
			DRW(L"HTML files viewer,                 htmViewRel64.dll:      66 560 b.")
			DRW(L" ")
			DRWC(L"Executable utilities:")
			DRW(L"Commands console,  ConsoleRel.exe:             23 404 b.")
			DRW(L"Folder creating,          CreateFolderRel.exe:     21 504 b.")
			DRW(L"Disk cleaning,            DiskCleanRel.exe:         48 128 b.")
			DRW(L"Process array,           ProcessArraysRel.exe:  21 504 b.")
			DRW(L"Register cleaning,     RegCleanRel.exe:          61 952 b.")
			DRW(L"Undelete NTFS files, UndelNTFSRel.exe:        41 472 b.")
			DRW(L"WMI-informations,     WMIInfoRel.exe:             258 048 b.")
			DRW(L" ")
			DRWC(L"Virtual panel plugins:")
			DRW(L"IP4 adapter scaner,            AdptrIP4Scan64.dll:      252 416 b.")
			DRW(L"IP4 adapter scaner driver, npfSino64.sys:              79 616 b.")
			DRW(L"Win objects,                       WinObjRel64.dll:           97 792 b.")
			/*DRW(L" ")
			DRWC(L"Thanks:")
			DRW(L"Assistent:             Nilufar Sattorova")
			DRW(L"Internet site host: Elbek Sattorov")
			DRW(L"For consults:        Ravshan Kiyomov(Bukhara ref.)")
			DRW(L"For consults:        Ermat Kiyomov(UZINFOCOM)")*/
#else
			DRWC(L"Contents:")
			DRW(L" ")
			DRW(L"Main executable,        Sino.exe:       1 430 528 b.")
			DRW(L" ")
			DRW(L"Main library,        MyShell.dll:       2 366 464 b.")
			DRW(L"                         ")
			DRWC(L"Archive plugins:")
			DRW(L"For 7zip files,      7zipRel.dll:        555 520 b.")
			DRW(L"For cabinet files, Cab32Rel.dll:     207 872 b.")
			DRW(L"For chm-files,      Chm32Rel.dll:    148 480 b.")
			DRW(L"For exe-files,       Exe32Rel.dll:     147 456 b.")
			DRW(L"For iso-files,        Iso32Rel.dll:      179 712 b.")
			DRW(L"For nrg-files,        Nrg32Rel.dll:     181 248 b.")
			DRW(L"For rar-files,         Rar32Rel.dll:     258 048 b.")
			DRW(L"For tar-files,         Tar32Rel.dll:      189 440 b.")
			DRW(L"For zip-files,        mzlib32rel.dll:    459 264 b.")
			DRW(L" ")
			DRWC(L"Auxilary plugins:")
			DRW(L"File-append plugin,               FileAppendRel.dll:   90 112 b.")
			DRW(L"File-compare plugin,             FileCompareRel.dll: 116 736 b.")
			DRW(L"File-defragmentation plugin, FileDefragRel.dll:     73 728 b.")
			DRW(L"File-split plugin,                    FileSplitRel.dll:         89 600 b.")
			DRW(L"First net adapter sniffer plugin,SnifAdpt0Rel.dll:   175 616 b.")
			DRW(L" ")
			DRW(L"Main search plugin,   SinSch32Rel.dll:  174 080 b.")
			DRW(L" ")
			DRWC(L"Executable plugins:")
			DRW(L"Text editor,                       notepad++.exe:      796 672 b.")
			DRW(L"Binary editor,                   BinEditRel.exe:      68 608 b.")
			DRW(L"Binary viewer,                  BinViewerRel.exe: 72 704 b.")
			DRW(L"Html-files viewer,             htmViewerRel.exe: 55 296 b.")
			DRW(L"Image-files viewer,           imgViewRel.exe:    126 976 b.")
			DRW(L"Media player-files viewer,mpviewerRel.exe:  19 968 b.")
			DRW(L"PE(exe)-files viewer,        PEViewerRel.exe:   144 896 b.")
			DRW(L"Exel-files viewer,             Spread32.exe:         550 400 b.")
			DRW(L"Text-files viewer,             TextViewerRel.exe: 82 944 b.")
			DRW(L"Zip-viewer,                    zipviewer.exe: 260 608 b.")
			DRW(L" ")
			DRWC(L"Image view plugins:")
			DRW(L"Bmp,dib,ico,cur-files viewer, DibRel.dll:          14 848 b.")
			DRW(L"Gif-files viewer,                      GifRel.dll:           14 336 b.")
			DRW(L"Jpg-files viewer,                     jpgFrFileRel.dll: 198 144 b.")
			DRW(L"Png-files viewer,                    pngFrFileRel.dll: 178 688 b.")
			DRW(L"Targa-files viewer,                 TargaRel.dll:       18 944 b.")
			DRW(L"Tif-files viewer,                      TifRel.dll:             216 576 b.")
			DRW(L" ")
			DRWC(L"Quick view plugins:")
			DRW(L"Binary files viewer,               binViewRel.dll:       77 824 b.")
			DRW(L"Directory viewer,                   dirViewRel.dll:        178 688 b.")
			DRW(L"Image files viewer,                imgViewRel.dll:      86 016 b.")
			DRW(L"MP files viewer(Direct Show),mpViewDSRel.dll: 48 128 b.")
			DRW(L"MP files viewer(MCI device), mpViewRel.dll:      19 968 b.")
			DRW(L"PE(exe) files viewer,              peViewRel.dll:       193 024 b.")
			DRW(L"Text files viewer,                   txtViewRel.dll:        105 984 b.")
			DRW(L"HTML files viewer,                 htmViewRel.dll:        60 928 b.")
			DRW(L" ")
			DRWC(L"Executable utilities:")
			DRW(L"Commands console,  ConsoleRel.exe:             21 504 b.")
			DRW(L"Folder creating,          CreateFolderRel.exe:     20 992 b.")
			DRW(L"Disk cleaning,            DiskCleanRel.exe:         44 544 b.")
			DRW(L"Process array,           ProcessArraysRel.exe:  20 480 b.")
			DRW(L"Register cleaning,     RegCleanRel.exe:          53 248 b.")
			DRW(L"Undelete NTFS files, UndelNTFSRel.exe:        37 376 b.")
			DRW(L"WMI-informations,        WMIInfoRel.exe:        193 536 b.")
			DRW(L" ")
			DRWC(L"Virtual panel plugins:")
			DRW(L"IP4 adapter scaner,            AdptrIP4ScanRel.dll: 208 384 b.")
			DRW(L"IP4 adapter scaner driver, npfSino.sys:              29 440 b.")
			DRW(L"Win objects,                       WinObjRel.dll:           78 848 b.")
			/*DRW(L" ")
			DRWC(L"Thanks:")
			DRW(L"Assistent:             Nilufar Sattorova")
			DRW(L"Internet site host: Elbek Sattorov")
			DRW(L"For consults:        Ravshan Kiyomov(Bukhara ref.)")
			DRW(L"For consults:        Ermat Kiyomov(UZINFOCOM)")*/
#endif
#undef TOPROLL
#undef DOWNROLL
#undef DRW
#undef DRWC
			ReleaseDC(dlg,dc);
			break;
	}
	return FALSE;
#pragma warning ( default : 4309)
}

static int CountSettedCustomItem(Panel *p,int tp)
{int r=0; 
 if(1==p->bItemsCustomName)++r;
 if(1==p->bItemsCustomExt)++r;
 if(1==p->bItemsCustomCrTime)++r;
 if(1==p->bItemsCustomWrTime)++r;
 if(1==p->bItemsCustomSz)++r;
 if(1==p->bItemsCustomTotSz)++r;
 if(1==p->bItemsCustomAtrb)++r;
 if(1==p->bItemsCustomState)++r;
 if(1==p->bItemsCustomNameLn)++r;
 if(1==p->bItemsCustomLstAccTime)++r;
 if(0==tp)
 {if(r>3)MessageBox(hWnd,L"Can't set above 4 custom item.",L"Error",MB_OK);}
 else
	if(r<2)MessageBox(hWnd,L"Yuo must set for any custom item.",L"Error",MB_OK);
 return r;	
}

static int OnCustomMenuItemClick(HWND hWnd,int confBit)
{	if(Panel::iActivePanel<0)return 0;
HMENU hm = GetMenu(hWnd);
	if(!hm)return 0;
	CheckMenuItem(hm,IDM_VIEW_SHORT,MF_BYCOMMAND|MF_UNCHECKED);
	CheckMenuItem(hm,IDM_VIEW_FULL,MF_BYCOMMAND|MF_UNCHECKED);
	MENUITEMINFO mi;mi.cbSize=sizeof(mi);mi.fMask=MIIM_SUBMENU;mi.hSubMenu=0;
	GetMenuItemInfo(hm,3,TRUE,&mi);
	if(mi.hSubMenu)
		CheckMenuItem(mi.hSubMenu,2,MF_BYPOSITION|MF_CHECKED);
	switch(confBit)
	{case 0:
		if(0==panel[Panel::iActivePanel].bItemsCustomName)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomName=1;
			CheckMenuItem(hm,IDM_CUSTOM_NAME,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomName=0;
			CheckMenuItem(hm,IDM_CUSTOM_NAME,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 1:
		if(0==panel[Panel::iActivePanel].bItemsCustomExt)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomExt=1;
			CheckMenuItem(hm,IDM_CUSTOM_EXTENSION,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomExt=0;
			CheckMenuItem(hm,IDM_CUSTOM_EXTENSION,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 2:
		if(0==panel[Panel::iActivePanel].bItemsCustomCrTime)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomCrTime=1;
			CheckMenuItem(hm,IDM_CUSTOM_CREATE_TIME,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomCrTime=0;
			CheckMenuItem(hm,IDM_CUSTOM_CREATE_TIME,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 3:
		if(0==panel[Panel::iActivePanel].bItemsCustomWrTime)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomWrTime=1;
			CheckMenuItem(hm,IDM_CUSTOM_WRITE_TIME,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomWrTime=0;
			CheckMenuItem(hm,IDM_CUSTOM_WRITE_TIME,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 4:
		if(0==panel[Panel::iActivePanel].bItemsCustomSz)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomSz=1;
			CheckMenuItem(hm,IDM_CUSTOM_SIZE,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomSz=0;
			CheckMenuItem(hm,IDM_CUSTOM_SIZE,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 5:
		if(0==panel[Panel::iActivePanel].bItemsCustomTotSz)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomTotSz=1;
			CheckMenuItem(hm,IDM_CUSTOM_TOTAL_SIZE,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomTotSz=0;
			CheckMenuItem(hm,IDM_CUSTOM_TOTAL_SIZE,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 6:
		if(0==panel[Panel::iActivePanel].bItemsCustomAtrb)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomAtrb=1;
			CheckMenuItem(hm,IDM_CUSTOM_ATTRIBUTE,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomAtrb=0;
			CheckMenuItem(hm,IDM_CUSTOM_ATTRIBUTE,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 7:
		if(0==panel[Panel::iActivePanel].bItemsCustomState)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomState=1;
			CheckMenuItem(hm,IDM_CUSTOM_STATE,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomState=0;
			CheckMenuItem(hm,IDM_CUSTOM_STATE,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 8:
		if(0==panel[Panel::iActivePanel].bItemsCustomNameLn)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomNameLn=1;
			CheckMenuItem(hm,IDM_CUSTOM_NAME_LEN,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomNameLn=0;
			CheckMenuItem(hm,IDM_CUSTOM_NAME_LEN,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	 case 9:
		if(0==panel[Panel::iActivePanel].bItemsCustomLstAccTime)
		{	if(3<CountSettedCustomItem(&panel[Panel::iActivePanel],0))return 0;
			panel[Panel::iActivePanel].bItemsCustomLstAccTime=1;
			CheckMenuItem(hm,IDM_CUSTOM_EXT_LEN,MF_BYCOMMAND|MF_CHECKED);
		}else
		{	if(2>CountSettedCustomItem(&panel[Panel::iActivePanel],1))return 0;
			panel[Panel::iActivePanel].bItemsCustomLstAccTime=0;
			CheckMenuItem(hm,IDM_CUSTOM_EXT_LEN,MF_BYCOMMAND|MF_UNCHECKED);
		}
		break;
	}
	panel[Panel::iActivePanel].bItemsViewType=2;
	panel[Panel::iActivePanel].ChangeViewMenu(0);
	panel[Panel::iActivePanel].AdjustScrollity();
	panel[Panel::iActivePanel].Render();
	return 0;
}

BOOL bActivate = FALSE;
LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//HDC  hdc;
static BOOL bInitFor_WM_SIZE = FALSE;
static BOOL bInitFor_WM_MOVE = FALSE;

	/*static int i=0;
	wchar_t ss[32];wsprintf(ss,L"\n %d ",i++);
	OutputDebugString(ss);
	OutputDebugString(GetWinNotifyText(message));*/

	switch(message)
	{
	case WM_MOUSEMOVE:
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
			panel[p].GetToolTip()->Hide();
		return 0;
	case WM_MOUSEHOVER:
		bInitFor_WM_SIZE = FALSE;
		return 0;
	case WM_SHOWWINDOW:
		return 0;
	case WM_CREATE:
		SetLayeredWindowAttributes(wnd,0,0,LWA_ALPHA);
		hWnd = wnd;
		//hdc = GetDC(wnd);
		CmnCntrl::GetClientRect(wnd, &rcClient);
		//ResetGetTextExtentPoint32(hdc);
		//ReleaseDC(wnd,hdc);
		language::SetInStartup(wnd);//) return -1;xatoga chiqishi kerakmas;
		vrtlPanels::LoadPlugins(wnd);
 		menuUtils::LoadPlugins(wnd);//tilini aylantirgandan so'ng bo'lishi shart,chunki til menuni o'zgartiradi;
		NSKERNEL::LoadFromNTDLL();//orasiga plugindan tushgan bo'lsa xato o'zgartirib yuboradur;
		archive::LoadPlugins();	
		fSearchViaF7::LoadPlugins();
		image::LoadPlugins();
		viewFileAssoc::LoadPlugins(wnd);
		UtilityExe::Load(wnd);
		QView::LoadPlugins(wnd);

		if(!CmnCntrl::CreateControls(wnd,TRUE))
		{	Err::msg(wnd,-1,L"Init.common controls failed!!!");
			return -1;
		}
		hfCmdBtns=CreateFontIndirect(&conf::Dlg.cmdBtnFnt);
		//SetFnt(&hfCmdBtns);//To MyShell
		brCmdBtns=CreateSolidBrush(conf::Dlg.cmdBtnRGB[2]);
		brHtBk=CreateSolidBrush(conf::Dlg.cmdBtnRGB[5]);

		char tch[16];_strtime(tch);int sec;sec=(tch[6]-'0')*10+tch[7]-'0';
		if(sec>0 && sec<60)
			SetTimer(hWnd,CLOCK_TIMER_ID_IMP,1000*(60-sec),NULL);
		else
			SetTimer(hWnd,CLOCK_TIMER_ID,60000,NULL);
		DrawClock(0);//hWnd);

		if(conf::Dlg.iActivePanel>-1 && conf::Dlg.iTotPanels)
			panel[conf::Dlg.iActivePanel].SetFocus();
		return 0;
	case WM_DISPLAYCHANGE:
		//hdc = GetDC(wnd);
		//ResetGetTextExtentPoint32(hdc);
		//ReleaseDC(wnd,hdc);
		return 0;
	case WM_NCACTIVATE://1-act
		bActivate = (BOOL)wParam;
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
			panel[p].GetToolTip()->Hide();
		BtnsPanel::Hide();
		return DefWindowProc(wnd, message, wParam, lParam);
	case WM_ACTIVATE://2-act
		bActivate = (WA_INACTIVE!=wParam);
		if(bActivate)
			panel[Panel::iActivePanel>-1?Panel::iActivePanel:0].SetFocus();
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
			panel[p].GetToolTip()->Hide();
		BtnsPanel::Hide();
		return 0;
	case WM_ACTIVATEAPP://3-actapp
		bActivate = (WA_INACTIVE!=wParam);
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
			panel[p].GetToolTip()->Hide();
		return 0;
	case WM_KILLFOCUS://4-keladi;
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
			panel[p].GetToolTip()->Hide();
		return 0;
	case WM_NCMOUSEMOVE: POINT cursPos;
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
			panel[p].GetToolTip()->Hide();
		cursPos.x = (int)(short)LOWORD(lParam);
		cursPos.y = (int)(short)HIWORD(lParam);

		//conf::nonClientAreaHeight ni ham ishlatsa bo'ladur;

		TITLEBARINFO pti;pti.cbSize=sizeof(TITLEBARINFO);//={sizeof(TITLEBARINFO),0,0};
		if(GetTitleBarInfo(wnd,&pti))
			if(cursPos.x>pti.rcTitleBar.left)
				if(cursPos.x<pti.rcTitleBar.right)
					if(cursPos.y>pti.rcTitleBar.top)
						if(cursPos.y<pti.rcTitleBar.bottom)
		//ScreenToClient(hWnd,&cursPos);
		//if(cursPos.x>0)if(cursPos.x<rcClient.right-rcClient.left-100)
		//if(cursPos.y<-30)if(cursPos.y>-55)
			BtnsPanel::Create();
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDM_VIEW_SHORT:
				if(Panel::iActivePanel>-1)
				{HMENU hm = GetMenu(hWnd);
				 if(!hm)return 0;
				 CheckMenuItem(hm,IDM_VIEW_SHORT,MF_BYCOMMAND|MF_CHECKED);
				 CheckMenuItem(hm,IDM_VIEW_FULL,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_NAME,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_EXTENSION,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_CREATE_TIME,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_WRITE_TIME,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_SIZE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_TOTAL_SIZE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_ATTRIBUTE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_STATE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_NAME_LEN,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_EXT_LEN,MF_BYCOMMAND|MF_UNCHECKED);
				 MENUITEMINFO mi;mi.cbSize=sizeof(mi);mi.fMask=MIIM_SUBMENU;mi.hSubMenu=0;
				 GetMenuItemInfo(hm,4,TRUE,&mi);
				 if(mi.hSubMenu)
					CheckMenuItem(mi.hSubMenu,2,MF_BYPOSITION|MF_UNCHECKED);
				 panel[Panel::iActivePanel].bItemsViewType=0;
				 panel[Panel::iActivePanel].bItemsCustomName=0;
				 panel[Panel::iActivePanel].bItemsCustomExt=0;
				 panel[Panel::iActivePanel].bItemsCustomCrTime=0;
				 panel[Panel::iActivePanel].bItemsCustomWrTime=0;
				 panel[Panel::iActivePanel].bItemsCustomSz=0;
				 panel[Panel::iActivePanel].bItemsCustomTotSz=0;
				 panel[Panel::iActivePanel].bItemsCustomAtrb=0;
				 panel[Panel::iActivePanel].bItemsCustomState=0;
				 panel[Panel::iActivePanel].bItemsCustomNameLn=0;
				 panel[Panel::iActivePanel].bItemsCustomLstAccTime=0;
				 panel[Panel::iActivePanel].ChangeViewMenu(0);
			 	 panel[Panel::iActivePanel].scrlInfo.nMax = 0;
				 panel[Panel::iActivePanel].AdjustScrollity();//RecalcItemsRects();
				 panel[Panel::iActivePanel].Render();
				}
				return 0;				
			case IDM_VIEW_FULL:
				if(Panel::iActivePanel>-1)
				{HMENU hm = GetMenu(hWnd);
				 if(!hm)return 0;
				 CheckMenuItem(hm,IDM_VIEW_SHORT,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_VIEW_FULL,MF_BYCOMMAND|MF_CHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_NAME,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_EXTENSION,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_CREATE_TIME,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_WRITE_TIME,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_SIZE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_TOTAL_SIZE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_ATTRIBUTE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_STATE,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_NAME_LEN,MF_BYCOMMAND|MF_UNCHECKED);
				 CheckMenuItem(hm,IDM_CUSTOM_EXT_LEN,MF_BYCOMMAND|MF_UNCHECKED);
				 MENUITEMINFO mi;mi.cbSize=sizeof(mi);mi.fMask=MIIM_SUBMENU;mi.hSubMenu=0;
				 GetMenuItemInfo(hm,4,TRUE,&mi);
				 if(mi.hSubMenu)
					CheckMenuItem(mi.hSubMenu,2,MF_BYPOSITION|MF_UNCHECKED);
				 panel[Panel::iActivePanel].bItemsViewType=1;
				 panel[Panel::iActivePanel].bItemsCustomName=0;
				 panel[Panel::iActivePanel].bItemsCustomExt=0;
				 panel[Panel::iActivePanel].bItemsCustomCrTime=0;
				 panel[Panel::iActivePanel].bItemsCustomWrTime=0;
				 panel[Panel::iActivePanel].bItemsCustomSz=0;
				 panel[Panel::iActivePanel].bItemsCustomTotSz=0;
				 panel[Panel::iActivePanel].bItemsCustomAtrb=0;
				 panel[Panel::iActivePanel].bItemsCustomState=0;
				 panel[Panel::iActivePanel].bItemsCustomNameLn=0;
				 panel[Panel::iActivePanel].bItemsCustomLstAccTime=0;
				 panel[Panel::iActivePanel].ChangeViewMenu(0);
				 panel[Panel::iActivePanel].AdjustScrollity();//RecalcItemsRects();
				 panel[Panel::iActivePanel].Render();
				}
				return 0;				
			case IDM_CUSTOM_NAME:
				return OnCustomMenuItemClick(hWnd,0);
			case IDM_CUSTOM_EXTENSION:
				return OnCustomMenuItemClick(hWnd,1);
			case IDM_CUSTOM_CREATE_TIME:
				return OnCustomMenuItemClick(hWnd,2);
			case IDM_CUSTOM_WRITE_TIME:
				return OnCustomMenuItemClick(hWnd,3);
			case IDM_CUSTOM_SIZE:
				return OnCustomMenuItemClick(hWnd,4);
			case IDM_CUSTOM_TOTAL_SIZE:
				return OnCustomMenuItemClick(hWnd,5);
			case IDM_CUSTOM_ATTRIBUTE:
				return OnCustomMenuItemClick(hWnd,6);
			case IDM_CUSTOM_STATE:
				return OnCustomMenuItemClick(hWnd,7);
			case IDM_CUSTOM_NAME_LEN:
				return OnCustomMenuItemClick(hWnd,8);
			case IDM_CUSTOM_EXT_LEN:
				return OnCustomMenuItemClick(hWnd,9);
			case IDM_FOLDERSANDFILESMIXING_FOLDERSTOUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(0,foldersUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSANDFILESMIXING_FILESTOUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(0,filesUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSANDFILESMIXING_MIXED://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(0,mixing);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYSIZEUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,forSizeUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYSIZEDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,forSizeDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYALPHABETUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,alphabeticalUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYALPHABETDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,alphabeticalDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYTIMEUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,forCreateTimeUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYTIMEDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,forCreateTimeDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYTYPEUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,forTypeUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_BYTYPEDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,forTypeDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FOLDERSSORTING_NOTSORTING://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(1,forEnums);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYSIZEUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,forSizeUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYSIZEDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,forSizeDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYALPHABETUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,alphabeticalUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYALPHABETDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,alphabeticalDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYTIMEUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,forCreateTimeUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYTIMEDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,forCreateTimeDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYTYPEUP://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,forTypeUp);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESSORTING_BYTYPEDOWN://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,forTypeDwn);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDM_FILESNOTSORTING://Menyudan:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].SetSortBtnState(2,forEnums);
				 panel[Panel::iActivePanel].ChangeViewMenu(1);
				}
				return 0;
			case IDB_BTN0+7://sortirovka,1-st btn for all panels:
			case IDB_BTN0+10:
			case IDB_BTN0+13:
			case IDB_BTN0+16:
				int p,b; 
				p = (LOWORD(wParam) - (IDB_BTN0+7)) / 3;
				b=(LOWORD(wParam) - (IDB_BTN0+7)) % 3;
				if(foldersUp==panel[p].folderAndFileMixingSortType)
					panel[p].SetSortBtnState(0,filesUp);
				else if(filesUp==panel[p].folderAndFileMixingSortType)
					panel[p].SetSortBtnState(0,mixing);
				else//if(mixing==panel[p].folderAndFileMixingSortType)
					panel[p].SetSortBtnState(0,foldersUp);
				panel[p].ChangeViewMenu(1);
				return 0;
			case IDB_BTN0+8://sortirovka,2-nd btn for all panels:
			case IDB_BTN0+11:
			case IDB_BTN0+14:
			case IDB_BTN0+17:
				p = (LOWORD(wParam) - (IDB_BTN0+7)) / 3;
				b=(LOWORD(wParam) - (IDB_BTN0+7)) % 3;
				if(alphabeticalUp==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,alphabeticalDwn);
				else if(alphabeticalDwn==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,forEnums);
				else if(forEnums==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,forCreateTimeUp);
				else if(forCreateTimeUp==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,forCreateTimeDwn);
				else if(forCreateTimeDwn==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,forSizeUp);
				else if(forSizeUp==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,forSizeDwn);
				else if(forSizeDwn==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,forTypeUp);
				else if(forTypeUp==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,forTypeDwn);
				else//if(forTypeDwn==panel[p].folderSortType)
					panel[p].SetSortBtnState(1,alphabeticalUp);
				panel[p].ChangeViewMenu(1);
				return 0;
			case IDB_BTN0+9://sortirovka,3-d btn for all panels:
			case IDB_BTN0+12:
			case IDB_BTN0+15:
			case IDB_BTN0+18:
				p = (LOWORD(wParam) - (IDB_BTN0+7)) / 3;
				b=(LOWORD(wParam) - (IDB_BTN0+7)) % 3;
				if(alphabeticalUp==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,alphabeticalDwn);
				else if(alphabeticalDwn==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,forEnums);
				else if(forEnums==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,forCreateTimeUp);
				else if(forCreateTimeUp==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,forCreateTimeDwn);
				else if(forCreateTimeDwn==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,forSizeUp);
				else if(forSizeUp==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,forSizeDwn);
				else if(forSizeDwn==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,forTypeUp);
				else if(forTypeUp==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,forTypeDwn);
				else//if(forTypeDwn==panel[p].fileSortType)
					panel[p].SetSortBtnState(2,alphabeticalUp);
				panel[p].ChangeViewMenu(1);
				return 0;
			case IDB_BTN0+6:
			case IDM_FILE_EXIT:
				PostMessage(wnd,WM_CLOSE,0,0);//PostQuitMessage(0);
				return 0;
			case IDB_BTN0://View:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].ExecuteHotKey(9);
				 panel[Panel::iActivePanel].SetFocus();
				}
				return 0;
			case IDB_BTN0+1://Edit:
				if(Panel::iActivePanel>-1)
				{panel[Panel::iActivePanel].ExecuteHotKey(10);
				 panel[Panel::iActivePanel].SetFocus();
				}
				return 0;
			case IDB_BTN0+2://Copy:
				if(Panel::iActivePanel>-1)
				{fCopyOper::ShowCopyDlg(&panel[Panel::iActivePanel]);
				 panel[Panel::iActivePanel].SetFocus();
				}
				return 0;
			case IDB_BTN0+3://RenMove:
				if(Panel::iActivePanel>-1)
				{fRenMove::ShowCopyDlg(&panel[Panel::iActivePanel]);
				 panel[Panel::iActivePanel].SetFocus();
				}
				return 0;
			case IDB_BTN0+4://CreateFolder:
				if(Panel::iActivePanel>-1)
				{CreaFolder::ShowDlg(&panel[Panel::iActivePanel]);
				 panel[Panel::iActivePanel].SetFocus();
				}
				return 0;
			case IDB_BTN0+5://Delete:
				if(Panel::iActivePanel>-1)
				{p = (LOWORD(wParam) - (IDB_BTN0+5)) / 3;//b=(LOWORD(wParam) - (IDB_BTN0+7)) % 3;
				 BOOL bShftPressed;bShftPressed = FALSE;
				 SHORT sh;sh = GetKeyState(VK_LSHIFT);
				 if(sh & 0x8000) bShftPressed = TRUE;
				 else
				 {	sh = GetKeyState(VK_RSHIFT);
			 		if(sh & 0x8000) bShftPressed = TRUE;
				 }
				 if(bShftPressed)fDelOper::Delete(&panel[Panel::iActivePanel],1);
				 else fDelOper::Delete(&panel[Panel::iActivePanel],0);
				 panel[Panel::iActivePanel].SetFocus();
				}
				return 0;
			case pathAndNameEdtID0:
				if(panel[0].ComboPathAndNameMsgQueue(wParam,lParam))
				{	return 0;
				}
				return DefWindowProc(wnd, message, wParam, lParam);
			case pathAndNameEdtID1:
				if(panel[1].ComboPathAndNameMsgQueue(wParam,lParam))
				{	return 0;
				}
				return DefWindowProc(wnd, message, wParam, lParam);
			case pathAndNameEdtID2:
				if(panel[2].ComboPathAndNameMsgQueue(wParam,lParam))
				{	return 0;
				}
				return DefWindowProc(wnd, message, wParam, lParam);
			case pathAndNameEdtID3:
				if(panel[3].ComboPathAndNameMsgQueue(wParam,lParam))
				{	return 0;
				}
				return DefWindowProc(wnd, message, wParam, lParam);
			case IDM_CONFIGURATION_LANGUAGE:
				if(1==conf::Dlg.bUsePaswordForSubMenu)//hInst edi;
				{if(1==DialogBoxParam(LoadLibrary(MyShellName),MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),wnd,(DLGPROC)conf::passwordDlgProc,(LPARAM)&conf::Dlg.subMenuPassword[0]))
DB:				  DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_LANGUAGE), wnd, language::DlgProc);
				}else goto DB;
				return 0;
			case IDM_CONFIGURATION:
				if(1==conf::Dlg.bUsePaswordForSubMenu)
				{if(1==DialogBoxParam(LoadLibrary(MyShellName),MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),wnd,(DLGPROC)conf::passwordDlgProc,(LPARAM)&conf::Dlg.subMenuPassword[0]))
DB1:			  DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_CONFIGURATION), wnd, conf::DlgProc);
				 for(int p=0; p<conf::Dlg.iTotPanels; p++)
					panel[p].Render();
				}else goto DB1;
				return 0;
			case IDM_LINK_VIATCP:
				if(!linkSock::sockDlg[Panel::iActivePanel])
				if(Panel::iActivePanel>-1 && Panel::iActivePanel<4)
				{	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_LINK_TO_IP), wnd, linkSock::DlgProc);
					if(1==linkSock::dlgRetCode)//Bind as server;
						linkSock::DoServer();
					else if(2==linkSock::dlgRetCode)//Bind as client;
						linkSock::DoClient();
				}
				return 0;
			case IDM_ARCHIVE_ADD:
				if(Panel::iActivePanel>-1)
					archive::ShowDlg(&panel[Panel::iActivePanel]);
				return 0;
			case IDM_ARCHIVE_OPEN:
				if(Panel::iActivePanel>-1)
					archive::ShowUnpckDlg(&panel[Panel::iActivePanel]);
				return 0;
			case IDM_HELP_CONTENTS:wchar_t mnuStr[64];wchar_t *pp,sinoExe[MAX_PATH];
				GetModuleFileName(NULL,sinoExe,MAX_PATH);pp=wcsrchr(sinoExe,'\\');if(pp)*pp=0;
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
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"0",sinoExe);
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
				return 0;
			case IDM_HELP_INDEX:
				GetModuleFileName(NULL,sinoExe,MAX_PATH);pp=wcsrchr(sinoExe,'\\');if(pp)*pp=0;
				if(GetEnvironmentVariableW(L"languge",mnuStr,64))//Language instartup qo'yadur;
				{	if(!wcscmp(mnuStr,L"russian"))
#ifdef _WIN64
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64R.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64L.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64K.exe"),L"-1",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"-1",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"-1",sinoExe);
#else
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32R.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32L.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32K.exe"),L"-1",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"-1",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"-1",sinoExe);
#endif
				return 0;
			case IDM_HELP_SEARCH:
				GetModuleFileName(NULL,sinoExe,MAX_PATH);pp=wcsrchr(sinoExe,'\\');if(pp)*pp=0;
				if(GetEnvironmentVariableW(L"languge",mnuStr,64))//Language instartup qo'yadur;
				{	if(!wcscmp(mnuStr,L"russian"))
#ifdef _WIN64
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64R.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64L.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64K.exe"),L"-2",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"-2",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"-2",sinoExe);
#else
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32R.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32L.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32K.exe"),L"-2",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"-2",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"-2",sinoExe);
#endif
				return 0;
			case IDM_HELP_PLUGIN_CONTENTS:
				GetModuleFileName(NULL,sinoExe,MAX_PATH);pp=wcsrchr(sinoExe,'\\');if(pp)*pp=0;
				if(GetEnvironmentVariableW(L"languge",mnuStr,64))//Language instartup qo'yadur;
				{	if(!wcscmp(mnuStr,L"russian"))
#ifdef _WIN64
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64R.exe"),L"0",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64L.exe"),L"0",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64K.exe"),L"0",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64E.exe"),L"0",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64E.exe"),L"0",sinoExe);
#else
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32R.exe"),L"0",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32L.exe"),L"0",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32K.exe"),L"0",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32E.exe"),L"0",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32E.exe"),L"0",sinoExe);
#endif
				return 0;
			case IDM_HELP_PLUGIN_INDEX:
				GetModuleFileName(NULL,sinoExe,MAX_PATH);pp=wcsrchr(sinoExe,'\\');if(pp)*pp=0;
				if(GetEnvironmentVariableW(L"languge",mnuStr,64))//Language instartup qo'yadur;
				{	if(!wcscmp(mnuStr,L"russian"))
#ifdef _WIN64
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64R.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64L.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64K.exe"),L"-1",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64E.exe"),L"-1",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64E.exe"),L"-1",sinoExe);
#else
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32R.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32L.exe"),L"-1",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32K.exe"),L"-1",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32E.exe"),L"-1",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32E.exe"),L"-1",sinoExe);
#endif
				return 0;
			case IDM_HELP_PLUGIN_SEARCH:
				GetModuleFileName(NULL,sinoExe,MAX_PATH);pp=wcsrchr(sinoExe,'\\');if(pp)*pp=0;
				if(GetEnvironmentVariableW(L"languge",mnuStr,64))//Language instartup qo'yadur;
				{	if(!wcscmp(mnuStr,L"russian"))
#ifdef _WIN64
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64R.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64L.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64K.exe"),L"-2",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64E.exe"),L"-2",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp64E.exe"),L"-2",sinoExe);
#else
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32R.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32L.exe"),L"-2",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32K.exe"),L"-2",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32E.exe"),L"-2",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoPlgWrtrsHlp32E.exe"),L"-2",sinoExe);
#endif
				return 0;
			case IDM_LICENSES:
				GetModuleFileName(NULL,sinoExe,MAX_PATH);pp=wcsrchr(sinoExe,'\\');if(pp)*pp=0;
				if(GetEnvironmentVariableW(L"languge",mnuStr,64))//Language instartup qo'yadur;
				{	if(!wcscmp(mnuStr,L"russian"))
#ifdef _WIN64
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64R.exe"),L"-3",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64L.exe"),L"-3",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64K.exe"),L"-3",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"-3",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp64E.exe"),L"-3",sinoExe);
#else
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32R.exe"),L"-3",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekl"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32L.exe"),L"-3",sinoExe);
					else if(!wcscmp(mnuStr,L"uzbekk"))
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32K.exe"),L"-3",sinoExe);
					else//if(wcscmp(mnuStr,L"Exit")
						Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"-3",sinoExe);
				}	
				else Execution::exePERoot(MyStringAddModulePath(L"Help\\SinoHlp32E.exe"),L"-3",sinoExe);
#endif
				return 0;
			case IDM_HELP_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), wnd, (DLGPROC)aboutDlgProc);
				return 0;
			case IDM_FILE_PROPERTIES://menudan, popupdan emas;
				if(Panel::iActivePanel>-1)
				{int id;id = panel[Panel::iActivePanel].iHot;
				 if(id>0)
				 if(id<panel[Panel::iActivePanel].GetTotItems())
				 {	wchar_t s[MAX_PATH];
					panel[Panel::iActivePanel].GetFullPathAndName(id,s,MAX_PATH);
					SHObjectProperties(panel[Panel::iActivePanel].GetHWND(),SHOP_FILEPATH,s,NULL);
				}}
				return 0;
			case IDM_FILE_START:
			case IDM_FILE_STREAM:
			case IDM_EDIT_COPYSELECTIONASTEXT:
			case IDM_EDIT_COPYALLITEMSASTEXT:
			case IDM_EDIT_COPYALLITEMSFULLASTEXT:
			case IDM_EDIT_COPYSELECTIONFULLASTEXT:
			case IDM_EDIT_CUT:
			case IDM_EDIT_COPY:
			case IDM_EDIT_PAST:
			case IDM_EDIT_DELETE:
			case IDM_EDIT_SELECTALL:
			case ID_EDIT_APPENDTOCLIPBOARDCOPYLIST:
			case ID_EDIT_APPENDTOCLIPBOARDCUTLIST:
			case IDM_SELECTALL_FOLDERS:
			case IDM_SELECTALL_FILES:
			case IDM_SELECTFROMNAME_ALL:
			case IDM_SELECTFROMNAME_FOLDERS:
			case IDM_SELECTFROMNAME_FILES:
			case IDM_SELECTFROMEXTENSION_ALL:
			case IDM_SELECTFROMEXTENSION_FOLDERS:
			case IDM_SELECTFROMEXTENSION_FILES:
			case IDM_SAVESELECTION_ALL:
			case IDM_SAVESELECTION_FOLDERS:
			case IDM_SAVESELECTION_FILES:
			case IDM_EDIT_RESTORESELECTION:
			case IDM_UNSELECTALL_ALL:
			case IDM_UNSELECTALL_FOLDERS:
			case IDM_UNSELECTALL_FILES:
			case IDM_UNSELECTFROMNAME_ALL:
			case IDM_UNSELECTFROMNAME_FOLDERS:
			case IDM_UNSELECTFROMNAME_FILES:
			case IDM_UNSELECTFROMEXTENSION_ALL:
			case IDM_UNSELECTFROMEXTENSION_FOLDERS:
			case IDM_UNSELECTFROMEXTENSION_FILES:
			case IDM_INVERTSELECTION_ALL:
			case IDM_INVERTSELECTION_FOLDERS:
			case IDM_INVERTSELECTION_FILES:
			case IDM_GUIDFOLDERS_DESKTOP://alloc guid folders:
			case IDM_GUIDFOLDERS_COMMONDESKTOPDIRECTORY:
			case IDM_GUIDFOLDERS_DESKTOPDIRECTORY:
			case IDM_GUIDFOLDERS_COMMONDOCUMENTS:
			case IDM_GUIDFOLDERS_MYDOCUMENTS:
			case IDM_GUIDFOLDERS_COMMONADMINTOOLS:
			case IDM_GUIDFOLDERS_ADMINTOOLS:
			case IDM_GUIDFOLDERS_NONLOCALSTARTUP:
			case IDM_GUIDFOLDERS_LOCALSTARTUP:
			case IDM_GUIDFOLDERS_COMMONSTARTUP:
			case IDM_GUIDFOLDERS_COMMONLOCALSTARTUP:
			case IDM_GUIDFOLDERS_APPLICATIONDATA:
			case IDM_GUIDFOLDERS_FAVORITES:
			case IDM_GUIDFOLDERS_COMMONPROGRAMS:
			case IDM_GUIDFOLDERS_PROGRAMS:
			case IDM_GUIDFOLDERS_COMMONSTART:
			case IDM_GUIDFOLDERS_START:
			case IDM_GUIDFOLDERS_TEMPLATES:
			case IDM_GUIDFOLDERS_COMMONTEMPLATE:
			case IDM_GUIDFOLDERS_COOKIES:
			case IDM_GUIDFOLDERS_FONTS:
			case IDM_GUIDFOLDERS_HISTORY:
			case IDM_GUIDFOLDERS_INTERNETCASHE:
			case IDM_GUIDFOLDERS_LOCALAPPLICATIONDATA:
			case IDM_GUIDFOLDERS_MYPICTURES:
			case IDM_GUIDFOLDERS_NETHOOD:
			case IDM_GUIDFOLDERS_PRINTHOOD:
			case IDM_GUIDFOLDERS_PROFILE:
			case IDM_GUIDFOLDERS_PROGRAM_FILES:
			case IDM_GUIDFOLDERS_COMMONPROGRAMFILES:
			case IDM_GUIDFOLDERS_RECENT:
			case IDM_GUIDFOLDERS_SENDTO:
			case IDM_GUIDFOLDERS_SYSTEM:
			case IDM_GUIDFOLDERS_WINDOWS:
			case IDM_VIRTUALGUIDFOLDERS_MYCOMPUTER://virtual GUID folders:
			case IDM_VIRTUALGUIDFOLDERS_DESKTOP:
			case IDM_VIRTUALGUIDFOLDERS_CONTROLPANEL:
			case IDM_VIRTUALGUIDFOLDERS_NETWORK:
			case IDM_VIRTUALGUIDFOLDERS_RECYCLEBIN:
			case IDM_VIRTUALGUIDFOLDERS_INTERNET:
			case IDM_VIRTUALGUIDFOLDERS_PRINTERS:
				return panel[Panel::iActivePanel>-1?Panel::iActivePanel:0].WndProc(hWnd,message,wParam,lParam);
			case IDM_EDIT_REPLASEDIRECTFOLDERPANELS:
				return ReplaceDirectFolderPanels(Panel::iActivePanel>-1?Panel::iActivePanel:0);
			case IDM_EDIT_COMMANDCONSOLE:
				return CallCommandConsole(Panel::iActivePanel>-1?Panel::iActivePanel:0);
			case IDM_FASSOC_VIEW:
				if(1==conf::Dlg.bUsePaswordForSubMenu)
				{if(1==DialogBoxParam(LoadLibrary(MyShellName),MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),wnd,(DLGPROC)conf::passwordDlgProc,(LPARAM)&conf::Dlg.subMenuPassword[0]))
DB2:			  DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_FILE_ASSOC_VIEW), wnd, (DLGPROC)viewFileAssoc::confDlgProc,0);
				}else goto DB2;
				return 0;
			case IDM_FASSOC_EDIT:
				if(1==conf::Dlg.bUsePaswordForSubMenu)
				{if(1==DialogBoxParam(LoadLibrary(MyShellName),MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),wnd,(DLGPROC)conf::passwordDlgProc,(LPARAM)&conf::Dlg.subMenuPassword[0]))
DB3:			  DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_FILE_ASSOC_VIEW), wnd, (DLGPROC)viewFileAssoc::confDlgProc,1);
				}else goto DB3;
				return 0;
			case IDM_FASSOC_EXECUTE:
				if(1==conf::Dlg.bUsePaswordForSubMenu)
				{if(1==DialogBoxParam(LoadLibrary(MyShellName),MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),wnd,(DLGPROC)conf::passwordDlgProc,(LPARAM)&conf::Dlg.subMenuPassword[0]))
DB4:				  DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_FILE_ASSOC_VIEW), wnd, (DLGPROC)viewFileAssoc::confDlgProc,2);
				}else goto DB4;
				return 0;
			case IDM_CONFIGURATION_LOCKSUBMENUVIAPASSWORD:
				if(1==conf::Dlg.bUsePaswordForSubMenu)
				{if(1==DialogBoxParam(LoadLibrary(MyShellName),MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),wnd,(DLGPROC)conf::passwordDlgProc,(LPARAM)&conf::Dlg.subMenuPassword[0]))
DB5:			  DialogBox(LoadLibrary(MyShellName),MAKEINTRESOURCE(IDD_DIALOG_LOCK_VIA_PASSWORD),wnd,(DLGPROC)conf::subMenuAccessDlgProc);
				}else goto DB5;
				return 0;
			case IDM_LICENSE_KEY:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_LICENSE),wnd,(DLGPROC)License::keyDlgProc);
				return 0;
			default:
				if(!menuUtils::CheckFromMenuId(hWnd,wParam))
				{if(!vrtlPanels::CheckFromMenuId(hWnd,wParam,&panel[Panel::iActivePanel>-1?Panel::iActivePanel:0]))
				 {	
				}}
				return 0;
		}
		return 0;
	case WM_MOVE:int x,y;
		//x = lParam & 0x0000ffff; 
		//y = (lParam & 0xffff0000) >> 16; 
			
		//profilerOutStr("Gl WM_MOVE")
		//profilerPrintf2("\nwPar = %x, lPar = %x", wParam, lParam)
		if(!bInitFor_WM_MOVE)
		{	x = (lParam & 0x0000ffff) - conf::wndLeft;
			if(x>0)
			{	conf::nonClientAreaWidth = x;
				conf::nonClientAreaHeight = ((lParam & 0xffff0000) >> 16) - conf::wndTop;
		}	}
		else
		{	x = (lParam & 0x0000ffff) - conf::nonClientAreaWidth;
			if(x>0)
			{	conf::wndLeft = x;
				if(conf::wndLeft > conf::scrWidth)
					conf::wndLeft = -(0xffff-conf::wndLeft);
			}
			else if(x>-conf::nonClientAreaWidth) conf::wndLeft = 0;
			y = ((lParam & 0xffff0000) >> 16) - conf::nonClientAreaHeight;
			if(y>0)
			{	conf::wndTop = y;
				if(conf::wndTop > conf::scrHeight)
					conf::wndTop = -(0xffff-conf::wndTop);
			}
			else if(y>-conf::nonClientAreaHeight) conf::wndTop = 0;
		} bInitFor_WM_MOVE = TRUE;
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
		{	SendMessage(panel[p].hWnd,WM_MOVE,lParam,wParam);
			if(linkSock::sockDlg[p])
				SendMessage(linkSock::sockDlg[p],WM_USER,0,0);
		}
		BtnsPanel::Hide();
		return 0;
	case WM_SIZE:
		//profilerOutStr("Gl WM_SIZE")
		//profilerPrintf2("\nwPar = %x, lPar = %x", wParam, lParam)
		int w; w = GetSystemMetrics(SM_CXSCREEN);
		int h; h = GetSystemMetrics(SM_CYSCREEN);
		conf::scrRatio = (float)w / h;
		if(bInitFor_WM_SIZE)
		{	if(SIZE_MINIMIZED!=wParam)
			{	if(conf::scrWidth != w)
				{	if(conf::scrHeight !=h)
					{	conf::scrWidth = w;
						conf::scrHeight = h;
		}	}	}	} bInitFor_WM_SIZE = TRUE;
		if(CmnCntrl::GetClientRect(wnd, &rcClient))
			GetWindowRect(wnd, &conf::wndRc);//CmnCntrl::GetWindowRect(wnd, &conf::wndRc);
		conf::wndWidth  = conf::wndRc.right - conf::wndRc.left;
		conf::wndHeight = conf::wndRc.bottom - conf::wndRc.top;
		CmnCntrl::ResizeControls();
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
		{	if(linkSock::sockDlg[p])
				SendMessage(linkSock::sockDlg[p],WM_USER,0,0);
		}
		return 0;
	case WM_CLOSE:
		for(int p=0; p<conf::Dlg.iTotPanels; p++)
		{	if(virtualPanel==panel[p].GetEntry()->GetCrntRecType())
			{	MessageBox(hWnd,L"Please,close all virtual panels.",L"For quiting...",MB_OK);
				return 0;
			}
			panel[p].FreeSelection();//Rasmi to'g'ri chiqishi uchun;			
		}
		//conf::SaveWindImgToDisk(wnd,MyStringAddModulePath("Wind.img"));
		DestroyWindow(wnd);
		return 0;
	case WM_DESTROY:
		vrtlPanels::FreePlugins();//hammadan oldin turishi kerak,agar panel virtual b-sa, yopishi uchun;
		menuUtils::FreePlugins();
		archive::FreePlugins();
		fSearchViaF7::FreePlugins();
		image::FreePlugins();
		viewFileAssoc::FreePlugins();
		QView::FreePlugins();
		for(int i=0; i<conf::Dlg.iTotPanels; i++)
		{	panel[i].WriteToDiskFrPathAndNameCombo(40);
			panel[i].FreeMem();
		}
		conf::Write(MyStringAddModulePath(L"Conf.bin"));
		BtnsPanel::Free();
		CBtnBar::Free();
		UtilityExe::Free();
		CmnCntrl::DestroyControls(wnd);
		language::Destroy();//) return -1;xatoga chiqishi kerakmas;
		PostQuitMessage(0);
		DeleteObject(hfCmdBtns);
		DeleteObject(brCmdBtns);
		DeleteObject(brHtBk);
		CTempDir::FreeTempItems();
		return 0;
	case WM_KEYDOWN:
		//profilerOutStr("Gl WM_KEYDOWN")
		//profilerPrintf2("\nwPar = %x, lPar = %x", wParam, lParam)
		if(lParam & 0x20000000)
		{	return 0;
		}

		if(16==wParam || 17==wParam)//Shift & Ctrl
		{	return DefWindowProc(wnd, message, wParam, lParam);
		}
		if(9==wParam && Panel::iActivePanel>-1)//TAB
		{	if(Panel::iActivePanel<conf::Dlg.iTotPanels-1)
				panel[Panel::iActivePanel+1].SetFocus();//Panel::iActivePanel=0;
			else
				panel[0].SetFocus();//Panel::iActivePanel=0;
		}
		return 0;
	case WM_MOUSEWHEEL:
		if(Panel::iActivePanel>-1)
		 panel[Panel::iActivePanel].PostMessage(message, wParam, lParam);
		return 0;
	break;
	case WM_TIMER:
		switch(wParam)
		{	case CLOCK_TIMER_ID_IMP:
				KillTimer(hWnd,CLOCK_TIMER_ID_IMP);
				SetTimer(hWnd,CLOCK_TIMER_ID,60000,NULL);
				DrawClock(0);
			return 0;
			case CLOCK_TIMER_ID:DrawClock(0);  return 0;
			case 102:MainIdleProcess();		   return 0;
			case 101:BtnsPanel::toolTip.Hide();return 0;
			case 103:topBar.toolTip.Hide    ();return 0;
			case 104:bottomBar.toolTip.Hide ();return 0;
		}
		if((int)wParam<conf::Dlg.iTotPanels)
			ShowWindow(panel[wParam-1].GetToolTip()->hWnd,SW_HIDE);
		return 0;
	//case WM_CTLCOLORSCROLLBAR: ownerdrawga
	//	break;
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:HDC dc;dc=(HDC)wParam;
		for(int p=0; p<MAX_PANELS; p++)
		{	if(	lParam==(LPARAM)panel[p].GetPathAndNameEdt(1) ||
				lParam==(LPARAM)panel[p].GetSelFilesInfoEdt())
			{	HFONT oldFnt=(HFONT)SelectObject(dc,panel[p].hfEdit);
				SetBkColor(dc,conf::Dlg.editRGB[p][2]);
				SetTextColor(dc,conf::Dlg.editRGB[p][1]);
				if(oldFnt!=panel[p].hfEdit)
					DeleteObject(oldFnt);
				return (LRESULT)panel[p].brEdit;
		}	}
		return DefWindowProc(hWnd,message,wParam,lParam);
	case WM_CTLCOLORBTN:dc=(HDC)wParam;
		for(int p=0; p<MAX_PANELS; p++)//for(int b=0; b<3; b++)
		{	if(lParam==(LPARAM)sortBtns[p][0] || lParam==(LPARAM)sortBtns[p][1] || lParam==(LPARAM)sortBtns[p][2])
			{	HFONT oldFnt=(HFONT)SelectObject(dc,panel[p].hfSrtBtn);
				SetBkColor(dc,conf::Dlg.srtBtnRGB[p][5]);
				SetTextColor(dc,conf::Dlg.srtBtnRGB[p][1]);
				if(oldFnt!=panel[p].hfSrtBtn)
					DeleteObject(oldFnt);
				return (LRESULT)panel[p].hBrshSrtBtn;
		}	}//else, if it is no from panel;
		for(int b=0; b<7; b++)//Pastdagi knopkalardan bo'lsa:
		{	if(lParam==(LPARAM)btns[b])//.GetHWND())
			{	HFONT oldFnt=(HFONT)SelectObject(dc,hfCmdBtns);
				SetBkColor(dc,conf::Dlg.cmdBtnRGB[5]);
				SetTextColor(dc,conf::Dlg.cmdBtnRGB[1]);
				if(oldFnt!=hfCmdBtns)
					DeleteObject(oldFnt);
				return (LRESULT)brHtBk;
		}	}
		return FALSE;//DefWindowProc(hWnd,message,wParam,lParam); degani;
	case WM_MEASUREITEM:
		return FALSE;
	case WM_DRAWITEM://WM_CTLCOLORBTN dagi knopkalar:
		LPDRAWITEMSTRUCT lpdis;lpdis = (LPDRAWITEMSTRUCT)lParam;
		for(int p=0; p<MAX_PANELS; p++)for(int b=0; b<3; b++)
		{	if(lpdis->hwndItem==sortBtns[p][b])
			{	wchar_t s[64];GetWindowText(sortBtns[p][b],s,64);
				RECT rc = lpdis->rcItem;
				UINT uStyle;uStyle = DFCS_BUTTONPUSH;
				//FillRect(lpdis->hDC,&rc,panel[p].hBrshSrtBtn);
				if(ODS_SELECTED & lpdis->itemState)
				{	uStyle |= DFCS_PUSHED;
					rc.left+=2;rc.top+=2;
				}
				DrawFrameControl(lpdis->hDC, &lpdis->rcItem, DFC_BUTTON, uStyle);
				if(lpdis->itemState & ODS_SELECTED)
					{rc.left+=1;rc.top+=1;rc.bottom-=2;rc.right-=3;}
				else
					{rc.left+=1;rc.top+=2;rc.bottom-=3;rc.right-=3;}
				FillRect(lpdis->hDC,&rc,panel[p].hBrshSrtBtn);//DrawText(lpdis->hDC,"                                                  ",50,&rc,DT_SINGLELINE|DT_VCENTER|DT_INTERNAL|DT_LEFT);
				if(lpdis->itemState & ODS_SELECTED)
					{rc.left-=1;rc.top-=1;rc.bottom+=3;rc.right+=3;}
				else
					{rc.left-=1;rc.top-=2;rc.bottom+=2;rc.right+=3;}
				DrawText(lpdis->hDC,s,MyStringLength(s,64),&rc,DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				return FALSE;
		}	}
		for(int b=0; b<7; b++)
		{	if(lpdis->hwndItem==btns[b])//.GetHWND())
			{	wchar_t s[64];GetWindowText(btns[b],s,64);
				RECT rc = lpdis->rcItem;
				UINT uStyle;uStyle = DFCS_BUTTONPUSH;
				if(ODS_SELECTED & lpdis->itemState)
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
				return FALSE;
		}	}
		return TRUE;
	case WM_NOTIFY:
		int id;id = (int)(((LPNMHDR)lParam)->idFrom);
		if(id >= IDB_DSK_BTN0 && id < IDB_DSK_BTN0+DskToolBar::totLogDsks)
		{	if(((LPNMHDR)lParam)->code==TTN_GETDISPINFO)
			{	LPTOOLTIPTEXT lpttt; 
				lpttt = (LPTOOLTIPTEXT)lParam; 
				lpttt->hinst = hInst;
				lpttt->lpszText = myWMI::GetLogicalDriveLabel(DskToolBar::logDiskNames[lpttt->hdr.idFrom-IDB_DSK_BTN0]);
			}
			return TRUE;
		}

		if(((LPNMHDR)lParam)->code==TTN_GETDISPINFO)
		{	LPTOOLTIPTEXT lpttt;TCITEM ite;ite.mask=TCIF_PARAM;
			if(((LPNMHDR)lParam)->hwndFrom == TabCtrl_GetToolTips(sheetTab[0]))
				TabCtrl_GetItem(sheetTab[0],((LPNMHDR)lParam)->idFrom,&ite);
			else if(((LPNMHDR)lParam)->hwndFrom == TabCtrl_GetToolTips(sheetTab[1]))
				TabCtrl_GetItem(sheetTab[1],((LPNMHDR)lParam)->idFrom,&ite);
			else if(((LPNMHDR)lParam)->hwndFrom == TabCtrl_GetToolTips(sheetTab[2]))
				TabCtrl_GetItem(sheetTab[2],((LPNMHDR)lParam)->idFrom,&ite);
			else if(((LPNMHDR)lParam)->hwndFrom == TabCtrl_GetToolTips(sheetTab[3]))
				TabCtrl_GetItem(sheetTab[3],((LPNMHDR)lParam)->idFrom,&ite);
			else return TRUE;
			lpttt = (LPTOOLTIPTEXT)lParam; 
			lpttt->hinst = hInst;
			lpttt->lpszText = ((SheetTab*)ite.lParam)->path;
			return TRUE;
		}
		switch(id)
		{	case IDB_DSK_BTNSTOOLBAR:
				switch(((LPNMHDR)lParam)->code)
				{	case NM_CLICK://From DiskToolBar:
						NMMOUSE *nm;nm = (NMMOUSE*)lParam;
						if(nm->hdr.hwndFrom==DskToolBar::bar[0])panel[0].SetFocus();//Panel::iActivePanel=0;
						else if(nm->hdr.hwndFrom==DskToolBar::bar[1])panel[1].SetFocus();//Panel::iActivePanel=1;
						else if(nm->hdr.hwndFrom==DskToolBar::bar[2])panel[2].SetFocus();//Panel::iActivePanel=2;
						else if(nm->hdr.hwndFrom==DskToolBar::bar[3])panel[3].SetFocus();//Panel::iActivePanel=3;
						//else if(nm->hdr.hwndFrom==hWnd) break;
						if(nm->dwItemSpec>0 && nm->dwItemSpec!=0xffffffff)
						{	int driveNum;driveNum = (int)(nm->dwItemSpec - IDB_DSK_BTN0);
						OnToolbarDiskBtnClicked(Panel::iActivePanel>-1?Panel::iActivePanel:0,driveNum);
							panel[Panel::iActivePanel>-1?Panel::iActivePanel:0].SetFocus();
						}
					break;
					case NM_RCLICK:
						nm = (NMMOUSE*)lParam;POINT pt(nm->pt);
						if(nm->hdr.hwndFrom==DskToolBar::bar[0])panel[0].SetFocus();//Panel::iActivePanel=0;
						else if(nm->hdr.hwndFrom==DskToolBar::bar[1])panel[1].SetFocus();//Panel::iActivePanel=1;
						else if(nm->hdr.hwndFrom==DskToolBar::bar[2])panel[2].SetFocus();//Panel::iActivePanel=2;
						else if(nm->hdr.hwndFrom==DskToolBar::bar[3])panel[3].SetFocus();//Panel::iActivePanel=3;
						//else if(nm->hdr.hwndFrom==hWnd) break;
						if(nm->dwItemSpec>0 && nm->dwItemSpec!=0xffffffff)
						{ClientToScreen(nm->hdr.hwndFrom,&pt);
						 ScreenToClient(wnd,&pt);
						 int driveNum;driveNum = (int)(nm->dwItemSpec - IDB_DSK_BTN0);
						 OnToolbarDiskBtnRClicked(Panel::iActivePanel>-1?Panel::iActivePanel:0,driveNum,&pt);//nm->pt);
						}
					break;
					default: 
						break;
				}
			break;
			case ID_SHEET_TAB:
				if(TCN_SELCHANGING==((LPNMHDR)lParam)->code)
					panel[0].OnSheetTabChanging();
				else if(TCN_SELCHANGE==((LPNMHDR)lParam)->code)
					panel[0].OnSheetTabChanged();
				break;
			case ID_SHEET_TAB+1:
				if(TCN_SELCHANGING==((LPNMHDR)lParam)->code)
					panel[1].OnSheetTabChanging();
				else if(TCN_SELCHANGE==((LPNMHDR)lParam)->code)
					panel[1].OnSheetTabChanged();
				break;
			case ID_SHEET_TAB+2:
				if(TCN_SELCHANGING==((LPNMHDR)lParam)->code)
					panel[2].OnSheetTabChanging();
				else if(TCN_SELCHANGE==((LPNMHDR)lParam)->code)
					panel[2].OnSheetTabChanged();
				break;
			case ID_SHEET_TAB+3:
				if(TCN_SELCHANGING==((LPNMHDR)lParam)->code)
					panel[3].OnSheetTabChanging();
				else if(TCN_SELCHANGE==((LPNMHDR)lParam)->code)
					panel[3].OnSheetTabChanged();
				break;
			case IDB_BTN0://View
				if(((LPNMHDR)lParam)->code==BCN_DROPDOWN)
				{	NMBCDROPDOWN* pDropDown = (NMBCDROPDOWN*)lParam;
					POINT pt;pt.x = pDropDown->rcButton.left;pt.y = pDropDown->rcButton.bottom;
					ClientToScreen(pDropDown->hdr.hwndFrom, &pt);
					HMENU hViewWithMenu = CreatePopupMenu();wchar_t s[128];
					LoadString(hInst,IDS_STRINGSW_361,s,127);//View with...(from plugins)
					AppendMenu(hViewWithMenu, MF_BYPOSITION, 1, s);
					LoadString(hInst,IDS_STRINGSW_362,s,127);
					AppendMenu(hViewWithMenu, MF_BYPOSITION, 2, s);
					LoadString(hInst,IDS_STRINGSW_363,s,127);
					AppendMenu(hViewWithMenu, MF_BYPOSITION, 3, s);
					LoadString(hInst,IDS_STRINGSW_369,s,127);
					AppendMenu(hViewWithMenu, MF_BYPOSITION, 4, s);
					int r=TrackPopupMenu(hViewWithMenu,TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD,pt.x,pt.y,0,hWnd,NULL);
					switch(r)
					{	case 1:panel[Panel::iActivePanel].ExecuteHotKey(46);break;
						case 2:panel[Panel::iActivePanel].ExecuteHotKey(47);break;
						case 3:panel[Panel::iActivePanel].ExecuteHotKey(48);break;
						case 4:panel[Panel::iActivePanel].ExecuteHotKey(57);break;
				}	}
				break;
			case IDB_BTN0+1://Edit
				if(((LPNMHDR)lParam)->code==BCN_DROPDOWN)
				{	NMBCDROPDOWN* pDropDown = (NMBCDROPDOWN*)lParam;
					POINT pt;pt.x = pDropDown->rcButton.left;pt.y = pDropDown->rcButton.bottom;
					ClientToScreen(pDropDown->hdr.hwndFrom, &pt);
					HMENU hEditWithMenu = CreatePopupMenu();wchar_t s[128];
					LoadString(hInst,IDS_STRINGSW_364,s,127);//wchar_t langStr[64];GetEnvironmentVariableW(L"languge",langStr,64);if(!wcscmp(langStr,L"russian"))
					AppendMenu(hEditWithMenu, MF_BYPOSITION, 1, s);//L"Редактирование с помощью...(плагины)");
					LoadString(hInst,IDS_STRINGSW_365,s,127);
					AppendMenu(hEditWithMenu, MF_BYPOSITION, 2, s);
					LoadString(hInst,IDS_STRINGSW_366,s,127);
					AppendMenu(hEditWithMenu, MF_BYPOSITION, 3, s);
					int r=TrackPopupMenu(hEditWithMenu,TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD,pt.x,pt.y,0,hWnd,NULL);
					switch(r)
					{	case 1:panel[Panel::iActivePanel].ExecuteHotKey(49);break;
						case 2:panel[Panel::iActivePanel].ExecuteHotKey(50);break;
						case 3:panel[Panel::iActivePanel].ExecuteHotKey(51);break;
				}	}
				break;
			case IDB_BTN0+2://Copy
				if(((LPNMHDR)lParam)->code==BCN_DROPDOWN)
				{	NMBCDROPDOWN* pDropDown = (NMBCDROPDOWN*)lParam;
					POINT pt;pt.x = pDropDown->rcButton.left;pt.y = pDropDown->rcButton.bottom;
					ClientToScreen(pDropDown->hdr.hwndFrom, &pt);
					HMENU hClpbrdCpyMenu = CreatePopupMenu();wchar_t s[128];
					LoadString(hInst,IDS_STRINGSW_253,s,127);//Copy
					AppendMenu(hClpbrdCpyMenu, MF_BYPOSITION, 1, s);
					LoadString(hInst,IDS_STRINGSW_251,s,127);//Cut
					AppendMenu(hClpbrdCpyMenu, MF_BYPOSITION, 2, s);
					LoadString(hInst,IDS_STRINGSW_367,s,127);//App copy
					AppendMenu(hClpbrdCpyMenu, MF_BYPOSITION, 3, s);
					LoadString(hInst,IDS_STRINGSW_368,s,127);//Append Cut
					AppendMenu(hClpbrdCpyMenu, MF_BYPOSITION, 4, s);
					int r=TrackPopupMenu(hClpbrdCpyMenu,TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD,pt.x,pt.y,0,hWnd,NULL);
					switch(r)
					{	case 1:panel[Panel::iActivePanel].ExecuteHotKey(52);break;
						case 2:panel[Panel::iActivePanel].ExecuteHotKey(53);break;
						case 3:panel[Panel::iActivePanel].ExecuteHotKey(55);break;
						case 4:panel[Panel::iActivePanel].ExecuteHotKey(56);break;
				}	}
				break;
			case IDB_BTN0+5://Delete
				if(((LPNMHDR)lParam)->code==BCN_DROPDOWN)
				{	NMBCDROPDOWN* pDropDown = (NMBCDROPDOWN*)lParam;
					POINT pt;pt.x = pDropDown->rcButton.left;pt.y = pDropDown->rcButton.bottom;
					ClientToScreen(pDropDown->hdr.hwndFrom, &pt);
					HMENU hClpbrdCpyMenu = CreatePopupMenu();wchar_t s[128];
					LoadString(hInst,IDS_STRINGSW_205,s,127);//Unrestorable delete
					AppendMenu(hClpbrdCpyMenu, MF_BYPOSITION, 1, s);
					int r=TrackPopupMenu(hClpbrdCpyMenu,TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD,pt.x,pt.y,0,hWnd,NULL);
					switch(r)
					{	case 1:panel[Panel::iActivePanel].ExecuteHotKey(42);break;
				}	}
				break;
			default: break;
		}
		return DefWindowProc(wnd, message, wParam, lParam);
	case WM_DEVICECHANGE:
		if(myWMI::OnDeviceChange(wnd, wParam, lParam))
		{	RedrawWindow(wnd,NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
			return 0;//else
		}
		return DefWindowProc(wnd, message, wParam, lParam);
	case WM_SYSCOMMAND:
		if(1==conf::Dlg.iHideMethod && SC_MINIMIZE == wParam)
		{	DoSystemTray(0,(HICON)GetClassLongPtr(hWnd,GCLP_HICON));//GetClassLong(hWnd,GCL_HICON));//Добавляем значок в трей
			ShowWindow(hWnd,SW_HIDE);//Скрываем программу
			return 0;
		}
		else return DefWindowProc(hWnd,message,wParam,lParam);
	case WM_USER://From system tray:
		if(1==conf::Dlg.iHideMethod)
		{//GetCursorPos(p);//Запоминаем координаты курсора мыши
		 switch(lParam)//Проверяем какая кнопка была нажата
		 {case WM_LBUTTONUP:case WM_LBUTTONDBLCLK://{Действия, выполняемый по одинарному или двойному щел?ку левой кнопки мыши на зна?ке. В нашем слу?ае это просто активация приложения}
			DoSystemTray(1,NULL);//Удаляем значок из трея
			ShowWindow(hWnd,SW_SHOWNORMAL);//Восстанавливаем окно программы
			break; 
		 case WM_RBUTTONUP://{Действия, выполняемый по одинарному щел?ку правой кнопки мыши}
			//SetForegroundWindow(Handle); // Восстанавливаем программу в ка?естве переднего окна  
			//PopupMenu1.Popup(p.X,p.Y); // Заставляем всплыть тот самый TPopUp о котором я говорил ?уть раньше
			//PostMessage(Handle,WM_NULL,0,0) // Обнуляем сообщение 
			break;
		}}
		return 0;
	case WM_CONTEXTMENU:
		if(sheetTab[0]==(HWND)wParam) myWMI::DoExplorerSheetTabMenu(0,lParam);
		else if(sheetTab[1]==(HWND)wParam) myWMI::DoExplorerSheetTabMenu(1,lParam);
		else if(sheetTab[2]==(HWND)wParam) myWMI::DoExplorerSheetTabMenu(2,lParam);
		else if(sheetTab[3]==(HWND)wParam) myWMI::DoExplorerSheetTabMenu(3,lParam);
		return 0;
	//case WM_ENTERIDLE:
		//if(MSGF_DIALOGBOX==wParam)
		//	if(CmnCntrl::==lParam)
	//	return 0;
	//case WM_DROPFILES://Panelgamas,aynan otasiga olarkan;
	//	POINT pt;int iPan;iPan=Panel::iActivePanel;
	//	OutputDebugString(L"WM_DROPFILES from Sino");
	//	if(GetCursorPos(&pt))
	//		iPan = Panel::GetPanelFromPoint(&pt);
	//	panel[iPan].DropFiles((HDROP)wParam);
		//int i;i=0;
	//break;
	//case WM_CHAR://EditItem mode :
	//	break;
	case WM_PAINT:PAINTSTRUCT ps;
		BeginPaint(wnd,&ps);
		CmnCntrl::DrawDskSpace(ps.hdc,-1);
		EndPaint(wnd,&ps);
		return 0;
	default:
		return DefWindowProc(wnd, message, wParam, lParam);
	}
	return DefWindowProc(wnd, message, wParam, lParam);
}

BOOL IsFinishInputMessage(UINT msg)
{
	switch(msg)
	{	case WM_KEYUP:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_NCLBUTTONUP:
		case WM_NCMBUTTONUP:
		case WM_NCRBUTTONUP:
			return TRUE;
		case WM_HSCROLL:
			return FALSE;
	}
	return FALSE;
}

int ReplaceDirectFolderPanels(int crntPnl)
{
wchar_t pthW[MAX_PATH];int dst; static char tp=0,tp1=0;
	switch(conf::Dlg.iTotPanels)
	{	case 1: return 0;
		case 2:
			if(folder!=panel[0].GetEntry()->GetCrntRecType()) return 0;
			if(folder!=panel[1].GetEntry()->GetCrntRecType()) return 0;
			panel[0].GetPath(pthW);
			panel[0].ChangePath(panel[1].GetPath());
			panel[1].ChangePath(pthW);
			return 0;
		case 3:
			switch(crntPnl)
			{	case 0: dst=(0==tp?1:2); break;
				case 1: dst=(0==tp?0:2); break;
				case 2: dst=(0==tp?0:1); break;
			}
			if(folder!=panel[crntPnl].GetEntry()->GetCrntRecType()) return 0;
			if(folder!=panel[1].GetEntry()->GetCrntRecType()) return 0;
			panel[crntPnl].GetPath(pthW);
			panel[crntPnl].ChangePath(panel[dst].GetPath());
			panel[dst].ChangePath(pthW);
			if(tp++>1)tp=0;
			return 0;
		case 4:
			switch(crntPnl)
			{	case 0: dst=(0==tp1?1:(tp1==1?2:3)); break;
				case 1: dst=(0==tp1?0:(tp1==1?2:3)); break;
				case 2: dst=(0==tp1?0:(tp1==1?1:3)); break;
				case 3: dst=(0==tp1?0:(tp1==1?1:2)); break;
			}
			if(folder!=panel[crntPnl].GetEntry()->GetCrntRecType()) return 0;
			if(folder!=panel[1].GetEntry()->GetCrntRecType()) return 0;
			panel[crntPnl].GetPath(pthW);
			panel[crntPnl].ChangePath(panel[dst].GetPath());
			panel[dst].ChangePath(pthW);
			if(tp1++>2)tp=0;
			return 0;
	}
	return 0;
}

typedef struct TCmdArgs
{
wchar_t cmndLine[MAX_PATH];
HWND	prnt;
int     fontColor;
int     backColor;
bool    bEchoOnOff;
bool    bAutorunOnOff;
bool    bUnicode;
bool    bExtendedOnOff;
} CmdArgs;
INT_PTR CALLBACK AskCmdArgDlg(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch(message)
	{
	case WM_INITDIALOG:
		SetWindowLongPtr(hDlg,GWLP_USERDATA,lParam);//SetWindowLong(hDlg,GWLP_USERDATA,lParam);
		//Adjust to center:
		RECT rc,rcPrnt; GetWindowRect(hDlg, &rc);
		GetWindowRect(((CmdArgs*)lParam)->prnt,&rcPrnt);
		int width,left,height,top;

		width = rc.right - rc.left;
		left = rcPrnt.left + (rcPrnt.right-rcPrnt.left - width)/2;
		if(left>rcPrnt.right-rcPrnt.left-width)left=rcPrnt.left;

		height = rc.bottom - rc.top;
		top = rcPrnt.top + (rcPrnt.bottom-rcPrnt.top - height)/2;
		//if(top>rcPrnt.bottom-rcPrnt.top-top)top=rcPrnt.top;
		MoveWindow(hDlg, left, top+20, width, height, TRUE);

		//Load language strings:
		wchar_t s[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_270,s,MAX_PATH);//45
		SetWindowText(hDlg,s);
		LoadString(hInst,IDS_STRINGSW_41,s,MAX_PATH);
		SetDlgItemText(hDlg,IDOK,s);
		LoadString(hInst,IDS_STRINGSW_13,s,MAX_PATH);
		SetDlgItemText(hDlg,IDCANCEL,s);
		SetWindowLongPtr(hDlg,GWLP_USERDATA,lParam);//SetWindowLong(hDlg,GWLP_USERDATA,lParam);
		LoadString(hInst,IDS_STRINGSW_271,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC1,s);
		LoadString(hInst,IDS_STRINGSW_272,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC2,s);
		LoadString(hInst,IDS_STRINGSW_273,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_ECHO,s);
		LoadString(hInst,IDS_STRINGSW_274,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_AUTORUN,s);
		LoadString(hInst,IDS_STRINGSW_275,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_UNICODE,s);
		LoadString(hInst,IDS_STRINGSW_276,s,MAX_PATH);
		SetDlgItemText(hDlg,IDC_CHECK_EXTENDED,s);
	
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"0-black");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"1-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"2-green");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"3-sky-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"4-red");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"5-violet");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"6-yellow");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"7-white");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"8-gray");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"9-light-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"A-light-green");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"B-light-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"C-light-red");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"D-light-violet");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"E-light-yellow");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_ADDSTRING,0,(LPARAM)L"F-light-white");

		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"0-black");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"1-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"2-green");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"3-sky-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"4-red");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"5-violet");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"6-yellow");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"7-white");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"8-gray");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"9-light-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"A-light-green");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"B-light-blue");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"C-light-red");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"D-light-violet");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"E-light-yellow");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_ADDSTRING,0,(LPARAM)L"F-light-white");
		cmdCB.Read(cmdCbPATH,GetDlgItem(hDlg,IDC_EDIT_CMD_ARG),40,128);
		//MyButtonFrRCBtn(hDlg,IDOK,conf::Dlg.iBtnsType);
		//MyButtonFrRCBtn(hDlg,IDCANCEL,conf::Dlg.iBtnsType);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDOK://overwrite
				CBToDisk::GetCBEditText(GetDlgItem(hDlg,IDC_EDIT_CMD_ARG),
						((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->cmndLine,MAX_PATH-1);//GetWindowLong(hDlg,GWLP_USERDATA))->cmndLine,MAX_PATH-1);
				CBToDisk::AddToCB(GetDlgItem(hDlg,IDC_EDIT_CMD_ARG),((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->cmndLine);//GetWindowLong(hDlg,GWLP_USERDATA))->cmndLine);
				cmdCB.Save(cmdCbPATH,GetDlgItem(hDlg,IDC_EDIT_CMD_ARG),128);
				EndDialog(hDlg,1);
				return (INT_PTR)TRUE;
			case IDCANCEL:
				EndDialog(hDlg,0);
				return (INT_PTR)TRUE;
			case IDC_CHECK_ECHO:
				((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->bEchoOnOff = //GetWindowLong(hDlg,GWLP_USERDATA))->bEchoOnOff =
					(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_ECHO),BM_GETCHECK,0,0)) ? TRUE : FALSE;
				return (INT_PTR)FALSE;
			case IDC_CHECK_UNICODE:
				((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->bUnicode = 
					(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_UNICODE),BM_GETCHECK,0,0)) ? TRUE : FALSE;
				return (INT_PTR)FALSE;
			case IDC_CHECK_AUTORUN:
				((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->bAutorunOnOff = 
					(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_AUTORUN),BM_GETCHECK,0,0)) ? TRUE : FALSE;
				return (INT_PTR)FALSE;
			case IDC_CHECK_EXTENDED:
				((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->bExtendedOnOff = 
					(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_EXTENDED),BM_GETCHECK,0,0)) ? TRUE : FALSE;
				return (INT_PTR)FALSE;
			case IDC_COMBO_BCK_COLOR:
				((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->backColor = 
						(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_BCK_COLOR),CB_GETCURSEL,0,0);
				return (INT_PTR)FALSE;
			case IDC_COMBO_FONT_COLOR:
				((CmdArgs*)GetWindowLongPtr(hDlg,GWLP_USERDATA))->fontColor = 
						(int)SendMessage(GetDlgItem(hDlg,IDC_COMBO_FONT_COLOR),CB_GETCURSEL,0,0);
				return (INT_PTR)FALSE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int CallCommandConsole(int crntPnl)
{
wchar_t pth[MAX_PATH],sysPath[MAX_PATH];
	int ln=MyStringCpy(pth,MAX_PATH-1,panel[crntPnl].GetPath());
	if('*'==pth[ln-1])
		pth[--ln]=0;
	if('\\'==pth[ln-1])
		pth[--ln]=0;
	ln=GetSystemDirectory(sysPath,MAX_PATH-1);
	if(!ln)	return 0;
	if(sysPath[ln-1]!='\\')
		sysPath[ln++]='\\';
	ln+=MyStringCpy(&sysPath[ln],MAX_PATH-1-ln,L"cmd.exe");
	WIN32_FIND_DATA ff;HANDLE hf = FindFirstFile(sysPath,&ff);
	if(INVALID_HANDLE_VALUE==hf)return 0;
	FindClose(hf);
CmdArgs params; params.backColor=-1;params.fontColor=-1;params.prnt=hWnd;
	if(!DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_CMD_ARG),hWnd,AskCmdArgDlg,(LPARAM)&params))
		return 0;
wchar_t cmndLine[MAX_PATH];

	cmndLine[0] = '/'; 
	cmndLine[1] = params.bUnicode ? 'U' : 'A';
	cmndLine[2] = ' '; 
	ln = 3;
	if(params.bEchoOnOff)
	{	cmndLine[3] = '/'; 
		cmndLine[4] = 'Q';
		cmndLine[5] = ' '; 
		ln = 6;
	}
	if(params.bAutorunOnOff)
	{	cmndLine[ln++] = '/'; 
		cmndLine[ln++] = 'D';
		cmndLine[ln++] = ' '; 
	}
	if(params.fontColor>-1 || params.backColor>-1)
	{	if(params.backColor<0)params.backColor=0;
		if(params.fontColor<0)params.fontColor=0;
		cmndLine[ln++] = '/'; 
		cmndLine[ln++] = 'T';
		cmndLine[ln++] = ':'; 
		if(params.backColor < 10) cmndLine[ln++] = params.backColor + '0'; 
		else  					  cmndLine[ln++] = params.backColor + 'A'; 
		if(params.fontColor < 10) cmndLine[ln++] = params.fontColor + '0'; 
		else					  cmndLine[ln++] = params.fontColor + 'A'; 
		cmndLine[ln++] = ' '; 
	}
	if(params.bExtendedOnOff)
	{	cmndLine[ln++] = '/'; 
		cmndLine[ln++] = 'E';
		cmndLine[ln++] = ':'; 
		cmndLine[ln++] = 'O'; 
		cmndLine[ln++] = 'F'; 
		cmndLine[ln++] = 'F'; 
		cmndLine[ln++] = ' '; 
	}
	if(params.cmndLine[0])
	{	cmndLine[ln++] = '/'; 
		cmndLine[ln++] = 'K';
		cmndLine[ln++] = ' ';
		MyStringCpy(&cmndLine[ln],MAX_PATH-ln-1,params.cmndLine);
	}
	else cmndLine[ln] = 0;


	//ShellExecute(hWnd,L"open",sysPath,cmndLine,pth,SW_SHOWDEFAULT);
	
	
	PROCESS_INFORMATION pi;STARTUPINFO si;ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);
	//si.dwX = si.dwY = si.dwXSize = si.dwYSize = CW_USEDEFAULT;
	//si.dwFillAttribute = BACKGROUND_INTENSITY|FOREGROUND_BLUE;
	si.wShowWindow = SW_SHOWDEFAULT;
	si.dwFlags = STARTF_USESHOWWINDOW;//|STARTF_USEFILLATTRIBUTE;//| STARTF_USESIZE | STARTF_USECOUNTCHARS;
	BOOL r =CreateProcess(sysPath,cmndLine,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,pth,&si,&pi);
 	return TRUE;//r;
}

//HMODULE ip4Dll=0;
//typedef int (*GetAdapterMacAndGatewayIP4_t)(char*,char*,char*);
//GetAdapterMacAndGatewayIP4_t GetAdapterMacAndGatewayIP4=0;
VOID DrawClock(HDC DC)//HWND hWnd)
{	/*TITLEBARINFO pti;RECT rc;
	ZeroMemory(&pti,sizeof(TITLEBARINFO));//pti.rgstate=0;
	pti.cbSize=sizeof(TITLEBARINFO);
	GetTitleBarInfo(hWnd,&pti);
	GetWindowRect(hWnd,&rc);*/
	HWND h=bottomBar.GetWindow();
	HDC dc = DC ? DC:GetDC(h);
	SetBkMode(dc,TRANSPARENT);
	HFONT oldFnt=(HFONT)SelectObject(dc,hfCmdBtns);
	//HBRUSH oldBrsh=(HBRUSH)SelectObject(dc,(HBRUSH)(COLOR_WINDOW));
	HBRUSH oldBrsh=(HBRUSH)SelectObject(dc,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	RECT rc;GetClientRect(h,&rc);rc.right-=2;
	rc.left = rc.right-140;
	Rectangle(dc,rc.left,0,rc.right+2,rc.bottom-2);

	char t,s[260];
	/*if(!ip4Dll)
	{	GetModuleFileNameA(NULL,s,260);
		char *p=strrchr(s,'\\');
		if(!p){p=&s[0]+strlen(s);*p++='\\';}else ++p;
#ifdef _WIN64
		sprintf(p,"Plugins\\Virtual panel\\AdptrIP4Scan\\AdptrIP4Scan64.dll");
#else
		sprintf(p,"Plugins\\Virtual panel\\AdptrIP4Scan\\AdptrIP4ScanRel.dll");
#endif
		ip4Dll=LoadLibraryA(s);
		GetAdapterMacAndGatewayIP4=(GetAdapterMacAndGatewayIP4_t)GetProcAddress(ip4Dll,"GetAdapterMacAndGatewayIP4");
	}*/

	HOSTENT *pHostEnt;
	int      nAdapter = 0;
	struct sockaddr_in dest;
	int ln = gethostname(s, 260);
	if(ln!=0)
	{if(SOCKET_ERROR!=ln)
	 pHostEnt = gethostbyname(s);
	 ln=(int)strlen(s);s[ln]=':';
	 memcpy(&(dest.sin_addr), pHostEnt->h_addr_list[0], pHostEnt->h_length);
	 if(127==dest.sin_addr.S_un.S_un_b.s_b1 &&
	   0==dest.sin_addr.S_un.S_un_b.s_b2 &&
	   0==dest.sin_addr.S_un.S_un_b.s_b3 &&
	   1==dest.sin_addr.S_un.S_un_b.s_b4)
		StringCchPrintfA(&s[ln+1],260-ln-1,"net unconnected.");
	 else
	 	StringCchPrintfA(&s[ln+1],260-ln-1,"%s",inet_ntoa(dest.sin_addr));
	}else StringCchPrintfA(&s[ln+1],260-ln-1,"net unconnected.");
    ln=(int)strlen(s);
	//DrawTextA(dc,"                                       ",40,&rc,DT_RIGHT);
	DrawTextA(dc,s,ln,&rc,DT_RIGHT);

	/*char MAC[6],IP[6],ADPTRNAME[128];
	if(GetAdapterMacAndGatewayIP4)
	{	if(1>GetAdapterMacAndGatewayIP4(MAC,IP,ADPTRNAME))
		{
	}	}*/
	
	_strdate(s);
	t=s[0];s[0]=s[3];s[3]=t;
	t=s[1];s[1]=s[4];s[4]=t;
	s[2]=s[5]='.';s[8]=s[6];s[9]=s[7];s[6]='2';s[7]='0';s[10]=' ';
	_strtime(&s[11]);s[16]=0;
	rc.top=12;
	//DrawTextA(dc,"                                       ",40,&rc,DT_RIGHT);
	DrawTextA(dc,s,(int)strlen(s),&rc,DT_RIGHT);

	//InternetGetConnectedState

	SelectObject(dc,oldFnt);
	//SelectObject(dc,oldBrsh);
	if(!DC)
		ReleaseDC(h,dc);
}

VOID CreateNeedDirs()
{	wchar_t s[MAX_PATH];
	GetModuleFileName(NULL,conf::modulePath,MAX_PATH);*wcsrchr(conf::modulePath,'\\')=0;
	int l=MyStringCpy(s,MAX_PATH-1,MyStringAddModulePath(L"*"));
	s[--l]=0;MySetCurrentDirectory(s);//GetCurrentDirectory(256,s);

	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Config\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\Archive\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\Auxilary\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\Auxilary\\SinoSearchF7\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\Image\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\Virtual panel\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\Executables\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	//MyStringCpy(&s[l],MAX_PATH-1-l,L"Plugins\\Utility\\");if(!IsDirExist(s))CreateDirectory(s,NULL);
	if(!IsFileExist(L"Config\\PnlBtns.cnf"))
	{	FILE *f=NULL;_wfopen_s(&f,L"Config\\PnlBtns.cnf",L"w,ccs=UNICODE");
		fwprintf(f,L"Params: 1.1000 0.3933 0 0");
		fwprintf(f,L"\nTotal: 46");
		fwprintf(f,L"\n1 type: 0 cmndNum: 0  pos: 4 4 size: 32 32");
		fwprintf(f,L"\n2 type: 0 cmndNum: 1  pos: 43 4 size: 32 32");
		fwprintf(f,L"\n3 type: 0 cmndNum: 2  pos: 82 4 size: 32 32");
		fwprintf(f,L"\n4 type: 0 cmndNum: 3  pos: 121 4 size: 32 32");
		fwprintf(f,L"\n5 type: 0 cmndNum: 4  pos: 160 4 size: 32 32");
		fwprintf(f,L"\n6 type: 0 cmndNum: 5  pos: 199 4 size: 32 32");
		fwprintf(f,L"\n7 type: 0 cmndNum: 6  pos: 238 4 size: 32 32");
		fwprintf(f,L"\n8 type: 0 cmndNum: 7  pos: 277 4 size: 32 32");
		fwprintf(f,L"\n9 type: 0 cmndNum: 8  pos: 316 4 size: 32 32");
		fwprintf(f,L"\n10 type: 0 cmndNum: 9  pos: 355 4 size: 32 32");
		fwprintf(f,L"\n11 type: 0 cmndNum: 10  pos: 394 4 size: 32 32");
		fwprintf(f,L"\n12 type: 0 cmndNum: 11  pos: 433 4 size: 32 32");
		fwprintf(f,L"\n13 type: 0 cmndNum: 12  pos: 472 4 size: 32 32");
		fwprintf(f,L"\n14 type: 0 cmndNum: 13  pos: 511 4 size: 32 32");
		fwprintf(f,L"\n15 type: 0 cmndNum: 14  pos: 550 4 size: 32 32");
		fwprintf(f,L"\n16 type: 0 cmndNum: 15  pos: 589 4 size: 32 32");
		fwprintf(f,L"\n17 type: 0 cmndNum: 16  pos: 628 4 size: 32 32");
		fwprintf(f,L"\n18 type: 0 cmndNum: 17  pos: 667 4 size: 32 32");
		fwprintf(f,L"\n19 type: 0 cmndNum: 18  pos: 706 4 size: 32 32");
		fwprintf(f,L"\n20 type: 0 cmndNum: 19  pos: 745 4 size: 32 32");
		fwprintf(f,L"\n21 type: 0 cmndNum: 20  pos: 784 4 size: 32 32");
		fwprintf(f,L"\n22 type: 0 cmndNum: 21  pos: 823 4 size: 32 32");
		fwprintf(f,L"\n23 type: 0 cmndNum: 22  pos: 862 4 size: 32 32");
		fwprintf(f,L"\n24 type: 0 cmndNum: 23  pos: 0 43 size: 32 32");
		fwprintf(f,L"\n25 type: 0 cmndNum: 24  pos: 39 43 size: 32 32");
		fwprintf(f,L"\n26 type: 0 cmndNum: 25  pos: 78 43 size: 32 32");
		fwprintf(f,L"\n27 type: 0 cmndNum: 26  pos: 117 43 size: 32 32");
		fwprintf(f,L"\n28 type: 0 cmndNum: 27  pos: 156 43 size: 32 32");
		fwprintf(f,L"\n29 type: 0 cmndNum: 28  pos: 195 43 size: 32 32");
		fwprintf(f,L"\n30 type: 0 cmndNum: 29  pos: 234 43 size: 32 32");
		fwprintf(f,L"\n31 type: 0 cmndNum: 30  pos: 273 43 size: 32 32");
		fwprintf(f,L"\n32 type: 0 cmndNum: 31  pos: 312 43 size: 32 32");
		fwprintf(f,L"\n33 type: 0 cmndNum: 32  pos: 351 43 size: 32 32");
		fwprintf(f,L"\n34 type: 0 cmndNum: 33  pos: 390 43 size: 32 32");
		fwprintf(f,L"\n35 type: 0 cmndNum: 34  pos: 429 43 size: 32 32");
		fwprintf(f,L"\n36 type: 0 cmndNum: 35  pos: 468 43 size: 32 32");
		fwprintf(f,L"\n37 type: 0 cmndNum: 36  pos: 507 43 size: 32 32");
		fwprintf(f,L"\n38 type: 0 cmndNum: 37  pos: 546 43 size: 32 32");
		fwprintf(f,L"\n39 type: 0 cmndNum: 38  pos: 585 43 size: 32 32");
		fwprintf(f,L"\n40 type: 0 cmndNum: 39  pos: 624 43 size: 32 32");
		fwprintf(f,L"\n41 type: 0 cmndNum: 40  pos: 663 43 size: 32 32");
		fwprintf(f,L"\n42 type: 0 cmndNum: 41  pos: 702 43 size: 32 32");
		fwprintf(f,L"\n43 type: 0 cmndNum: 42  pos: 741 43 size: 32 32");
		fwprintf(f,L"\n44 type: 0 cmndNum: 43  pos: 780 43 size: 32 32");
		fwprintf(f,L"\n45 type: 0 cmndNum: 44  pos: 819 43 size: 32 32");
		fwprintf(f,L"\n46 type: 0 cmndNum: 45  pos: 858 43 size: 32 32");
		fclose(f);
}	}

BOOL CheckRegistryForSino()
{
HKEY k;
//#ifdef _WIN64
	if(ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{E773413F-6001-47E4-A408-500F57A540DC}",0,KEY_READ,&k))//KEY_SET_VALUE SOFTWARE\\Wow6432Node\\
//#else
//	if(ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{E773413F-6001-47E4-A408-500F57A540DC}",0,KEY_READ,&k))
//#endif
	{InsFrSetup:MessageBoxW(NULL,L"Please,first install program 'Sino'.",L"Err...",MB_OK);
		return FALSE;
	}
	/*if(ERROR_SUCCESS==RegQueryValueExW(k,L"UninstallString",NULL,&rType,(LPBYTE)&s[0],&sz))
	{	wchar_t *p,pth[2*MAX_PATH];GetModuleFileName(NULL,pth,MAX_PATH);
		p = wcsrchr(pth,'\\');if(p) *p=0;
		if(_wcsnicmp(s,pth,wcslen(pth)))//wcsstr(s,pth))
		{	MyStringCat(pth,2*MAX_PATH,L"\\Uninstall.exe /");
			MyStringCat(pth,2*MAX_PATH,s);
			sz=RegSetValueExW(k,L"UninstallString",NULL,rType,(BYTE*)&pth[0],2*wcslen(pth)+2);
	}	}*/
	RegCloseKey(k);

	if(ERROR_SUCCESS != RegOpenKeyExW(HKEY_CURRENT_USER,L"SOFTWARE\\SavarCo\\Sino",0,KEY_READ,&k))
		goto InsFrSetup;
	RegCloseKey(k);
	return TRUE;
}

BOOL CheckAdminPrivilegy()
{
//TOKEN_GROUPS *PTOKEN_GROUPS;
SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;
PSID psidAdmin;
TOKEN_GROUPS* ptg = NULL;
HANDLE htkThread;
DWORD cbTokenGroups,iGroup=(DWORD)-1;
BOOL res=FALSE;//bAdmin

  if(!OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,FALSE,&htkThread))
  {if(ERROR_NO_TOKEN==GetLastError())
   {if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&htkThread))
		return FALSE;
  }}
  if(GetTokenInformation(htkThread,TokenGroups,NULL,0,&cbTokenGroups))// get #of groups
	return FALSE;
  if(ERROR_INSUFFICIENT_BUFFER!=GetLastError())
	return FALSE;
  ptg = (TOKEN_GROUPS*)(HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,cbTokenGroups));
  if(NULL==ptg)
    return FALSE;
  if(GetTokenInformation(htkThread,TokenGroups,ptg,cbTokenGroups,&cbTokenGroups))// get groups
  {if(AllocateAndInitializeSid(	&SystemSidAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,
								DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&psidAdmin))
   {while(++iGroup < ptg->GroupCount)
    {if(EqualSid(ptg->Groups[iGroup].Sid,psidAdmin))
     {res = TRUE;
      break;
    }}
    FreeSid(psidAdmin);
  }}
  HeapFree(GetProcessHeap(),0,(LPVOID)ptg);
  return res;
}

LRESULT CALLBACK LogonDlgProc(HWND dlg, UINT message, WPARAM wParam, LPARAM lParam)
{	switch(message)
	{	case WM_INITDIALOG:wchar_t s[MAX_PATH];
			LoadString(hInst,IDS_STRINGSW_14,s,MAX_PATH);
			if(!wcscmp(s,L"Авторы ..."))
			{	SetWindowText(dlg,L"Не удалось войти в систему как администратор:");
				SetDlgItemText(dlg,IDC_STATIC1,L"Пользователь");
				SetDlgItemText(dlg,IDC_STATIC2,L"Парол");
				SetDlgItemText(dlg,IDC_STATIC3,L"Домен");
				SetDlgItemText(dlg,IDCANCEL,L"Выход");
				SetDlgItemText(dlg,IDOK,L"Пуск с текушими привилегиями");
				SetDlgItemText(dlg,IDOK3,L"Поменять привелегия и запускать");
				SetDlgItemText(dlg,IDC_EDIT1,L"Администратор");
			}
			else if(!wcscmp(s,L"Mualliflar ..."))
			{	SetWindowText(dlg,L"Tizimga administrator bo'lib kirishilmadi:");
				SetDlgItemText(dlg,IDC_STATIC1,L"Ishlovchi");
				SetDlgItemText(dlg,IDC_STATIC2,L"Parol");
				SetDlgItemText(dlg,IDC_STATIC3,L"Domen");
				SetDlgItemText(dlg,IDCANCEL,L"Chiqish");
				SetDlgItemText(dlg,IDOK,L"Joriy privilegiya bilan ishlatish");
				SetDlgItemText(dlg,IDOK3,L"Privelegiyani o'zgartirib kirish");
				SetDlgItemText(dlg,IDC_EDIT1,L"Administrator");
			}
			else if(!wcscmp(s,L"Муаллифлар ..."))
			{	SetWindowText(dlg,L"Тизимга администратор бўлиб киришилмади:");
				SetDlgItemText(dlg,IDC_STATIC1,L"Ишловчи");
				SetDlgItemText(dlg,IDC_STATIC2,L"Парол");
				SetDlgItemText(dlg,IDC_STATIC3,L"Домен");
				SetDlgItemText(dlg,IDCANCEL,L"Чиқиш");
				SetDlgItemText(dlg,IDOK,L"Жорий привилегия билан ишлатиш");
				SetDlgItemText(dlg,IDOK3,L"Привилегияни ўзгартириб кириш");
				SetDlgItemText(dlg,IDC_EDIT1,L"Администратор");
			}
			else//if(!wcscmp(s,L"About ...")) all others in engl version:
			{	SetWindowText(dlg,L"Logon to \"Sino\"as administrator failed:");
				SetDlgItemText(dlg,IDC_STATIC1,L"User");
				SetDlgItemText(dlg,IDC_STATIC2,L"Password");
				SetDlgItemText(dlg,IDC_STATIC3,L"Domain");
				SetDlgItemText(dlg,IDCANCEL,L"Exit");
				SetDlgItemText(dlg,IDOK,L"Run with current privilegy");
				SetDlgItemText(dlg,IDOK3,L"Change privelegy and run");
				SetDlgItemText(dlg,IDC_EDIT1,L"Administrator");
			}
			RECT rc;int w,h,sc,sh;
			GetWindowRect(dlg,&rc);
			w = rc.right-rc.left; sc = GetSystemMetrics(SM_CXSCREEN);
			h = rc.bottom-rc.top; sh = GetSystemMetrics(SM_CYSCREEN);
			MoveWindow(dlg,(sc-w)/2,(sh-h)/2,w,h,TRUE);
			DWORD dw;dw=MAX_PATH;
			if(GetUserName(s,&dw))
				SetDlgItemText(dlg,IDC_EDIT1,s);//L"Администратор");
			dw=MAX_PATH;
			if(GetComputerName(s,&dw))
				SetDlgItemText(dlg,IDC_EDIT2,s);//Erkin-PC
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{	case IDOK:					
					EndDialog(dlg,0);
					break;
				case IDOK3:
					int r;wchar_t usr[64],dmn[64],psw[64],modFlName[MAX_PATH];STARTUPINFO si;PROCESS_INFORMATION pi;
					GetDlgItemText(dlg,IDC_EDIT1,usr,64);
					GetDlgItemText(dlg,IDC_EDIT2,psw,64);
					GetDlgItemText(dlg,IDC_EDIT3,dmn,64);
					GetModuleFileName(NULL,modFlName,MAX_PATH);
					ZeroMemory(&si,sizeof(si));
					si.cb=sizeof(si);
					//MyStringCpy(si.lpDesktop,16,L"winsta0\Default");
					if(!CreateProcessWithLogonW(usr,
												dmn,//L"ЭРКИН-ПК",//"localhost",//".",//SOKAL",
												psw,
												LOGON_WITH_PROFILE,
												modFlName,
												NULL,
												0,//CREATE_DEFAULT_ERROR_MODE,
												NULL,
												NULL,
												&si,
												&pi))
					{
						r=GetLastError();
					}
					EndDialog(dlg,1);
					break;
				case IDCANCEL:
					EndDialog(dlg,2);
					break;
			}
			break;
	}
	return FALSE;
}

BOOL TrySetAdminPrivilegy()
{	int res=(int)DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_LOGON),NULL,(DLGPROC)LogonDlgProc);
	if(0!=res)return FALSE;
	return TRUE;
}

BOOL CheckOSBitRate()
{
	OSVERSIONINFOEX os;os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(GetVersionEx((LPOSVERSIONINFO)&os))
	{	if(os.dwMajorVersion>5 && os.dwMinorVersion>1)
		{	HKEY k;
			if(ERROR_SUCCESS ==RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
					0,KEY_QUERY_VALUE|KEY_SET_VALUE,&k))
			{	wchar_t buf[8]={0};DWORD sz=16;
				if(ERROR_SUCCESS==RegQueryValueEx(k,L"EnableLUA",NULL,NULL,(LPBYTE)buf,&sz))
				{	if(1==buf[0])
					{	buf[0]=0;
						int r=RegSetValueEx(k,L"EnableLUA",NULL,NULL,(LPBYTE)buf,2);
						if(ERROR_SUCCESS==r)
							Beep(0,0);
						MessageBox(NULL,L"HKLM\\\nSOFTWARE\\\nMicrosoft\\\nWindows\\\nCurrentVersion\\\nPolicies\\\nSystem\\\nEnableLUA\n\nFor the program correct working in administrator level,\nset this registry value to zero and restart PC.\nUse regedit utility.",
									L"Please,set following registry value(UACL) to 0.",MB_OK);
				}	}
				RegCloseKey(k);
	}	}	}

#define WIN32_WINNT
SYSTEM_INFO nsi;
	ZeroMemory(&nsi,sizeof(nsi));
	GetNativeSystemInfo(&nsi);
	if(0==nsi.dwPageSize)return TRUE;//function do not work;
#ifdef _WIN64
	if(PROCESSOR_ARCHITECTURE_AMD64==nsi.wProcessorArchitecture)
		return TRUE;
	MessageBox(NULL,L"Operating system bit rate checking...",L"This program do not work on x86 processor architecture.",MB_OK);
	return FALSE;
#else
	if(PROCESSOR_ARCHITECTURE_INTEL==nsi.wProcessorArchitecture)
		return TRUE;
	MessageBox(NULL,L"Operating system bit rate checking...",L"This program do not work on x64 processor architecture.",MB_OK);
	return FALSE;
#endif
#undef WIN32_WINNT
}

//http://msdn.microsoft.com/downloads/samples/internet/shellcc/CustomDraw_ListView/default.asp
//http://msdn.microsoft.com/downloads/samples/internet/shellcc/CustomDrawButtonDlg/default.asp