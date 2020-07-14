#include "windows.h"
#include "Oleacc.h"
#include "sino.h"
/*
HWINEVENTHOOK hShellHook=NULL;


void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, 
                             LONG idObject, LONG idChild, 
                             DWORD dwEventThread, DWORD dwmsEventTime)
{
    IAccessible* pAcc = NULL;
    VARIANT varChild;
    HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);  
	if ((hr == S_OK) && (pAcc != NULL))
    {
        BSTR bstrName;
        pAcc->get_accName(varChild, &bstrName);
        if (event == EVENT_OBJECT_CREATE) 
        {
            //printf("Begin: ");
        }
        else if (event == EVENT_OBJECT_DESTROY)
        {
            //printf("End:   ");
        }
        //printf("%S\n", bstrName);
        SysFreeString(bstrName);
        pAcc->Release();
}	}

VOID SetShellHook(BOOL b)
{
	if(b)
	{	CoInitialize(NULL);
		hShellHook = SetWinEventHook(
        EVENT_MIN, EVENT_MAX,						// Range of events (4 to 5).
        NULL,                                       // Handle to DLL.
        HandleWinEvent,                             // The callback.
        0, 0,              // Process and thread IDs of interest (0 = all)
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.
	}
	else
	{	if(hShellHook)
			UnhookWinEvent(hShellHook);
		CoUninitialize();
		hShellHook  = NULL;
}	}*/


/*
HHOOK hShelHook = NULL;
int NCODE=0,wPARAM=0,lPARAM=0;

LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
		return CallNextHookEx(hShelHook, nCode, wParam, lParam);
	switch(nCode)
	{	case HSHELL_ACCESSIBILITYSTATE:
			break;
		case HSHELL_ACTIVATESHELLWINDOW:
			break;
		case HSHELL_APPCOMMAND:
			break;
		case HSHELL_GETMINRECT:
			break;
		case HSHELL_LANGUAGE:
			break;
		case HSHELL_REDRAW:
			break;
		case HSHELL_TASKMAN:
			break;
		case HSHELL_WINDOWACTIVATED:
			break;
		case HSHELL_WINDOWCREATED:
			break;
		case HSHELL_WINDOWDESTROYED:
			NCODE = nCode;
			wPARAM = wParam;
			lPARAM = lParam;
			break;
		case HSHELL_WINDOWREPLACED:
			break;			
	}
	CallNextHookEx(hShelHook, nCode, wParam, lParam);
	return 0;
}

VOID SetShellHook(BOOL b)
{
	if(b)
	{	if(!hShelHook)
			hShelHook = SetWindowsHookEx(	WH_SHELL,
											ShellProc,
											hInst,
											GetCurrentThreadId());
		if(!hShelHook)
			ErrMsg(-1,"SetWindowsHookEx(WH_SHELL...)");

		//RegisterShellHookWindow(hWnd);

		NCODE=0;
		wPARAM=0;
		lPARAM=0;
	}
	else
	{	//if(hShelHook)
		//	UnhookWindowsHookEx(hShelHook);
		//hShelHook = NULL;
		//DeregisterShellHookWindow(hWnd);
}	}

VOID WaitForShellHookDestroyMessage(int msg, int wParam, int lParam)
{
	if(hShelHook)
	{	for(;;)
		{	if(msg==NCODE)
			if(wPARAM==wParam)
			if(lPARAM==lParam)
				return;
			Sleep(25);
}	}	}*/