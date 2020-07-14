// Uninstall.cpp : Defines the entry point for the application.
//

#include "windows.h"
#include "resource.h"
#include "commctrl.h"


HINSTANCE hInst;
HWND hWnd;
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
	if(IDYES!=MessageBoxW(hWnd,L"Are You shure to uninstall Sino auxilary package?",L"Warn!!!",MB_YESNO))
	{	SendMessage(hWnd,WM_USER,0,2536);
		return 0;
	}

DWORD res = 0,rType;
SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!scmHandle)
	{	SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Error opening service manager ...");
		SendMessage(hWnd,WM_USER,res,2536);
		return 1;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,10,0);
	SC_HANDLE schService = OpenServiceA(scmHandle,"NPF",DELETE|4|0x20);//4=SERIVCE_QUERY_STATUS,20-SERVICE_STOP);
	if(!schService)
	{	SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Error opening NPF adapter capture device ...");
		res = 2;
		goto End1;
	}






	DWORD dwTimeout=2500,dwStartTime = GetTickCount();
	SERVICE_STATUS_PROCESS ssp;DWORD dwBytesNeeded;
    if(!QueryServiceStatusEx(schService,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp, 
							 sizeof(SERVICE_STATUS_PROCESS),&dwBytesNeeded))
    {   SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Error QueryServiceStatusEx ...");
        goto TryDalshe;
    }
    if(ssp.dwCurrentState == SERVICE_STOPPED)
        goto TryDalshe;

    while(ssp.dwCurrentState == SERVICE_STOP_PENDING)
    {	SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Service stop pending...");
        Sleep( ssp.dwWaitHint );
        if(!QueryServiceStatusEx(schService,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp, 
							sizeof(SERVICE_STATUS_PROCESS),&dwBytesNeeded))
		{   SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Error QueryServiceStatusEx ...");
			goto TryDalshe;
		}
        if(ssp.dwCurrentState == SERVICE_STOPPED)
			goto TryDalshe;
        if(GetTickCount() - dwStartTime > dwTimeout)
		{   SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Service stop timed out ...");
            goto TryDalshe;
    }	}
    StopDependentServices(scmHandle,schService);
    if(!ControlService(schService,SERVICE_CONTROL_STOP,(LPSERVICE_STATUS)&ssp))
	{   SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"ControlService failed ...");
        goto TryDalshe;
    }
    // Wait for the service to stop.
    while(ssp.dwCurrentState != SERVICE_STOPPED)
    {   Sleep(ssp.dwWaitHint);
        if(!QueryServiceStatusEx(schService,SC_STATUS_PROCESS_INFO,(LPBYTE)&ssp,
                sizeof(SERVICE_STATUS_PROCESS),&dwBytesNeeded))
		{   SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"QueryServiceStatusEx failed ...");
            goto TryDalshe;
        }
        if(ssp.dwCurrentState == SERVICE_STOPPED)
            break;
        if(GetTickCount() - dwStartTime > dwTimeout)
		{   SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Wait timed out ...");
            goto TryDalshe;
    }	}
	SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Service stopped successfully ...");


















TryDalshe:
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,20,0);
	if(!DeleteService(schService) ) 
	{	SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Error deleting NPF adapter capture device ...");
		res = 3;
    }
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,30,0);
	if(!CloseServiceHandle(schService))
	{	SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Error closing service handle ...");
		res = 4;
	}
End1:
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,40,0);
	if(!CloseServiceHandle(scmHandle))
	{	SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Error closing service handle ...");
		SendMessage(hWnd,WM_USER,res,2536);
		res = 5;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,50,0);
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
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,60,0);
	if(ERROR_SUCCESS==RegQueryValueExW(k,L"UninstallString",NULL,&rType,(LPBYTE)&s[0],&sz))
	{	wchar_t pth[2*MAX_PATH];GetModuleFileName(NULL,pth,MAX_PATH);
		ln = wcslen(pth);
		if(_wcsnicmp(s,pth,wcslen(pth)))//wcsstr(s,pth))
		{	SetDlgItemText(hWnd,IDC_EDIT_STATE,L"Error, uncorrect uninstall registry record for Sino ...");
			res = 6;
			goto End2;
		}
		SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,70,0);
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
End2:
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,80,0);
	//RegCloseKey(k);
	if(res)
	{	SendMessage(hWnd,WM_USER,res,2536);
		return res;
	}
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,90,0);

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
	SendMessage(GetDlgItem(hWnd,IDC_PROGRESS_PROCESS),PBM_SETPOS,100,0);
	if(!res)
		SetDlgItemTextW(hWnd,IDC_EDIT_STATE,L"Finish uninstalling succesfully ...");
	SendMessage(hWnd,WM_USER,res,2536);
	return res;
}

INT_PTR CALLBACK UninstallDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
int width,height;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:RECT rc,rcPrnt;
		hWnd = hDlg;
		GetWindowRect(hDlg, &rc);
		GetWindowRect(GetDesktopWindow(), &rcPrnt);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		MoveWindow(hDlg,(rcPrnt.right-width)/2,(rcPrnt.bottom-height)/2,width,height, TRUE);
		SetDlgItemTextW(hDlg,IDC_EDIT_STATE,L"Uninstalling WinPCap kernel driver from the system ....");
		CreateThread(NULL,0,UninstlThrd,NULL,0,&thrId);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{	EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_USER://wParam da resultat;
		if(2536==lParam)
		{	Sleep(2500);//ozgina kutsun:
			EndDialog(hDlg,wParam);
		}
		return FALSE;
	case WM_SETFONT:
		return FALSE;
	}
	return (INT_PTR)FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	//return DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_UNINSTALL),GetDesktopWindow(),UninstallDlgProc);
	HWND dlg=CreateWindowEx(WS_EX_APPWINDOW,L"#32770",L"Uninstall",WS_CHILD,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,
		(HMENU)IDD_DIALOG_UNINSTALL,hInst,NULL);
	int r=GetLastError();
	return r;
}