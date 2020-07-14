// UninstallAux1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "UninstallAux1.h"
#include "commctrl.h"
#include "Shellapi.h"


HWND hWnd,edt,prgs,btnOK,btnCANCEL;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_UNINSTALLAUX1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UNINSTALLAUX1));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UNINSTALLAUX1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(GRAY_BRUSH);//(COLOR_WINDOW+2);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_UNINSTALLAUX1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

DWORD thrId;

BOOL StopDependentServices(SC_HANDLE schSCManager,SC_HANDLE schService)
{
DWORD i;
DWORD dwBytesNeeded;
DWORD dwCount;

LPENUM_SERVICE_STATUS   lpDependencies = NULL;
ENUM_SERVICE_STATUS     ess;
SC_HANDLE               hDepService;
SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if ( EnumDependentServices( schService, SERVICE_ACTIVE, 
         lpDependencies, 0, &dwBytesNeeded, &dwCount ) ) 
    {
         // If the Enum call succeeds, then there are no dependent
         // services, so do nothing.
         return TRUE;
    } 
    else 
    {
        if ( GetLastError() != ERROR_MORE_DATA )
            return FALSE; // Unexpected error

        // Allocate a buffer for the dependencies.
        lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc( 
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded );
  
        if ( !lpDependencies )
            return FALSE;

        __try {
            // Enumerate the dependencies.
            if ( !EnumDependentServices( schService, SERVICE_ACTIVE, 
                lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                &dwCount ) )
            return FALSE;

            for ( i = 0; i < dwCount; i++ ) 
            {
                ess = *(lpDependencies + i);
                // Open the service.
                hDepService = OpenService( schSCManager, 
                   ess.lpServiceName, 
                   SERVICE_STOP | SERVICE_QUERY_STATUS );

                if ( !hDepService )
                   return FALSE;

                __try {
                    // Send a stop code.
                    if ( !ControlService( hDepService, 
                            SERVICE_CONTROL_STOP,
                            (LPSERVICE_STATUS) &ssp ) )
                    return FALSE;

                    // Wait for the service to stop.
                    while ( ssp.dwCurrentState != SERVICE_STOPPED ) 
                    {
                        Sleep( ssp.dwWaitHint );
                        if ( !QueryServiceStatusEx( 
                                hDepService, 
                                SC_STATUS_PROCESS_INFO,
                                (LPBYTE)&ssp, 
                                sizeof(SERVICE_STATUS_PROCESS),
                                &dwBytesNeeded ) )
                        return FALSE;

                        if ( ssp.dwCurrentState == SERVICE_STOPPED )
                            break;

                        if ( GetTickCount() - dwStartTime > dwTimeout )
                            return FALSE;
                    }
                } 
                __finally 
                {
                    // Always release the service handle.
                    CloseServiceHandle( hDepService );
                }
            }
        } 
        __finally 
        {
            // Always free the enumeration buffer.
            HeapFree( GetProcessHeap(), 0, lpDependencies );
        }
    } 
    return TRUE;
}

DWORD WINAPI UninstlThrd(LPVOID lpParameter)
{
int NumArgs;
LPWSTR *cmdlnstrs = CommandLineToArgvW(GetCommandLine(),&NumArgs);
	if(NumArgs==2)
	{	if(0==wcscmp(cmdlnstrs[1],L"emtyCmndLine"))
		{	if(IDYES!=MessageBoxW(hWnd,L"Are You shure to uninstall Sino auxilary package?",L"Warn!!!",MB_YESNO))
			{	SendMessage(hWnd,WM_USER,0,2536);
				return 0;
	}	}	}

	//MessageBox(hWnd,L"ncvnvn",L"dfjhfhfd",MB_OK);

DWORD res = 0;//,rType;
SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!scmHandle)
	{	SetWindowText(edt,L"Error opening service manager ...");
		SendMessage(hWnd,WM_USER,res,2536);
		return 1;
	}
	SendMessage(prgs,PBM_SETPOS,10,0);
#ifdef _WIN64
	SC_HANDLE schService = OpenServiceA(scmHandle,"NPF",DELETE|4|0x20);//4=SERIVCE_QUERY_STATUS,20-SERVICE_STOP);
	if(!schService)
	{	SetWindowText(edt,L"Error opening NPF adapter capture device ...");
		res = 2;
		goto End1;
	}
#else
	SC_HANDLE schService = OpenServiceA(scmHandle,"NPFSino",DELETE|4|0x20);//4=SERIVCE_QUERY_STATUS,20-SERVICE_STOP);
	if(!schService)
	{	SetWindowText(edt,L"Error opening NPFSino adapter capture device ...");
		res = 2;
		goto End1;
	}
#endif


	DWORD dwTimeout=2500,dwStartTime = GetTickCount();
	SERVICE_STATUS_PROCESS ssp;DWORD dwBytesNeeded;
    if(!QueryServiceStatusEx(schService,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp, 
							 sizeof(SERVICE_STATUS_PROCESS),&dwBytesNeeded))
    {   SetWindowText(edt,L"Error QueryServiceStatusEx ...");
        goto TryDalshe;
    }
    if(ssp.dwCurrentState == SERVICE_STOPPED)
        goto TryDalshe;

    while(ssp.dwCurrentState == SERVICE_STOP_PENDING)
    {	SetWindowText(edt,L"Service stop pending...");
        Sleep( ssp.dwWaitHint );
        if(!QueryServiceStatusEx(schService,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp, 
							sizeof(SERVICE_STATUS_PROCESS),&dwBytesNeeded))
		{   SetWindowText(edt,L"Error QueryServiceStatusEx ...");
			goto TryDalshe;
		}
        if(ssp.dwCurrentState == SERVICE_STOPPED)
			goto TryDalshe;
        if(GetTickCount() - dwStartTime > dwTimeout)
		{   SetWindowText(edt,L"Service stop timed out ...");
            goto TryDalshe;
    }	}
    StopDependentServices(scmHandle,schService);
    if(!ControlService(schService,SERVICE_CONTROL_STOP,(LPSERVICE_STATUS)&ssp))
	{   SetWindowText(edt,L"ControlService failed ...");
        goto TryDalshe;
    }
    // Wait for the service to stop.
    while(ssp.dwCurrentState != SERVICE_STOPPED)
    {   Sleep(ssp.dwWaitHint);
        if(!QueryServiceStatusEx(schService,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp,
                sizeof(SERVICE_STATUS_PROCESS),&dwBytesNeeded))
		{   SetWindowText(edt,L"QueryServiceStatusEx failed ...");
            goto TryDalshe;
        }
        if(ssp.dwCurrentState == SERVICE_STOPPED)
            break;
        if(GetTickCount() - dwStartTime > dwTimeout)
		{   SetWindowText(edt,L"Wait timed out ...");
            goto TryDalshe;
    }	}
	SetWindowText(edt,L"Service stopped successfully ...");


















TryDalshe:
	SendMessage(prgs,PBM_SETPOS,20,0);
	if(!DeleteService(schService) ) 
#ifdef _WIN64
	{	SetWindowText(edt,L"Error deleting NPF adapter capture device ...");
#else
	{	SetWindowText(edt,L"Error deleting NPFSino adapter capture device ...");
#endif
		res = 3;
    }
	SendMessage(prgs,PBM_SETPOS,30,0);
	if(!CloseServiceHandle(schService))
	{	SetWindowText(edt,L"Error closing service handle ...");
		res = 4;
	}
End1:
	SendMessage(prgs,PBM_SETPOS,40,0);
	if(!CloseServiceHandle(scmHandle))
	{	SetWindowText(edt,L"Error closing service handle ...");
		SendMessage(hWnd,WM_USER,res,2536);
		res = 5;
	}
	SendMessage(prgs,PBM_SETPOS,50,0);
	if(res && 2!=res)//Service ni o'zi yo'q;
	{	SendMessage(hWnd,WM_USER,res,2536);
		return res;//Dalneyshiy becpolezno;
	}



	//Registrni qaytib MsiExe ga to'g'rilaymiz:
/*	wchar_t s[2*MAX_PATH];DWORD sz=2*MAX_PATH;HKEY k;int ln;
	if(ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{E773413F-6001-47E4-A408-500F57A540DC}",0,KEY_SET_VALUE|KEY_READ,&k))
	{	SendMessage(hWnd,WM_USER,res,2536);
		return 0;
	}
	SendMessage(prgs,PBM_SETPOS,60,0);
	if(ERROR_SUCCESS==RegQueryValueExW(k,L"UninstallString",NULL,&rType,(LPBYTE)&s[0],&sz))
	{	wchar_t pth[2*MAX_PATH];GetModuleFileName(NULL,pth,MAX_PATH);
		ln = wcslen(pth);
		if(_wcsnicmp(s,pth,wcslen(pth)))//wcsstr(s,pth))
		{	SetDlgItemText(hWnd,IDC_EDIT_STATE,L"Error, uncorrect uninstall registry record for Sino ...");
			res = 6;
			goto End2;
		}
		SendMessage(prgs,PBM_SETPOS,70,0);
		if(ERROR_SUCCESS!=RegSetValueExW(k,L"UninstallString",NULL,rType,(BYTE*)&s[ln+2],2*wcslen(&s[ln+2])+2))
		{	SetDlgItemText(hWnd,IDC_EDIT_STATE,L"Error, changing uninstall registry record for Sino ...");
			res = 7;
			goto End2;
		}//else hech narsa qilmasun, kerak emas;
	}
	else
	{	SetDlgItemText(hWnd,IDC_EDIT_STATE,L"Error opening uninstall registry record for Sino ...");
		res = 7;
	}*/
//End2:
	SendMessage(prgs,PBM_SETPOS,80,0);
	//RegCloseKey(k);
	if(res)
	{	SendMessage(hWnd,WM_USER,res,2536);
		return res;
	}
	SendMessage(prgs,PBM_SETPOS,90,0);

	//Endi setupApp ni chaqiramiz:
	//STARTUPINFO si;ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);
	//PROCESS_INFORMATION pi;
	//if(!CreateProcessW(&s[ln+2],NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
/*	SHELLEXECUTEINFO shi;ZeroMemory(&shi,sizeof(shi));
	shi.cbSize = sizeof(shi);
	shi.lpVerb = L"open";
	shi.lpFile = &s[ln+2];
	wchar_t *p = wcschr(&s[ln+2],' ');
	if(p)
	{	*p = 0;
		shi.lpParameters = p+1;
	}
	shi.nShow = SW_SHOW;
	ShellExecuteEx(&shi);
	{	SetDlgItemText(hWnd,IDC_EDIT_STATE,L"Error uninstallling via system utility,please try from system utility ...");
		SendMessage(hWnd,WM_USER,res,2536);
		return 8;
	}*/
	SendMessage(prgs,PBM_SETPOS,100,0);
	if(!res)
		SetWindowText(edt,L"Finish uninstalling succesfully ...");
	SendMessage(hWnd,WM_USER,res,2536);
	return res;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;
static BOOL bExit=FALSE;
	switch (message)
	{
	case WM_CREATE:RECT rc,rcPrnt;int width,height;
		hWnd = wnd;
		GetWindowRect(hWnd, &rc);
		GetWindowRect(GetDesktopWindow(), &rcPrnt);
		width = 406;//rc.right - rc.left;
		height = 114;//rc.bottom - rc.top;
		MoveWindow(hWnd,(rcPrnt.right-width)/2,(rcPrnt.bottom-height)/2,width,height, TRUE);
		edt = CreateWindow(L"EDIT",L"#101",WS_CHILD,6,6,378,14,wnd,NULL,NULL,NULL);
		prgs = CreateWindowEx(0, PROGRESS_CLASS, (LPCWSTR)NULL, WS_CHILD|WS_VISIBLE, 6, 26, 378, 18, 
							  wnd, (HMENU) 0, hInst, NULL); 
		btnOK = CreateWindow(L"BUTTON",L"OK",WS_CHILD,240,48,60,25,wnd,NULL,NULL,NULL);
		btnCANCEL = CreateWindow(L"BUTTON",L"Cancel",WS_CHILD,306,48,80,25,wnd,NULL,NULL,NULL);
		SetWindowText(edt,L"Uninstalling WinPCap kernel driver from the system ....");
		ShowWindow(edt,SW_SHOW);
		ShowWindow(prgs,SW_SHOW);
		ShowWindow(btnOK,SW_SHOW);
		ShowWindow(btnCANCEL,SW_SHOW);
		CreateThread(NULL,0,UninstlThrd,NULL,0,&thrId);
		return 0;
	case WM_COMMAND:
		if(bExit)
		{	if(LOWORD(wParam)==IDM_EXIT || LOWORD(wParam)==IDM_ENTER)
			{	DestroyWindow(wnd);
				return (INT_PTR)TRUE;
			}
			else if(lParam == (LPARAM)btnOK || lParam == (LPARAM)btnCANCEL)
			{	DestroyWindow(wnd);
				return (INT_PTR)TRUE;
		}	}
		break;
	case WM_USER://wParam da resultat;
		if(2536==lParam)
		{	bExit = TRUE;
			SetTimer(wnd,NULL,2000,NULL);//Sleep(1500);//ozgina kutsun:
			//DestroyWindow(wnd);
		}
		return FALSE;
	case WM_TIMER:
		DestroyWindow(wnd);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(wnd, message, wParam, lParam);
	}
	return 0;
}