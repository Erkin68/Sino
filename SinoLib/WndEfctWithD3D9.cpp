#include "WndEfctWithD3D9.h"
#include "strsafe.h"
#include "..\config.h"
#include "..\Sino.h"

BOOL		bFinEfct;
int			iEffectWndInstCnt = 0;
HANDLE		efctThrdHandle[NUM_EFFECT_THREADS]={0,0,0,0};
DWORD		efctThrdID[NUM_EFFECT_THREADS];
TEfctThrTls	*efctTLSs[NUM_EFFECT_THREADS]={0,0,0,0};

ATOM				RegisterEffectWindowClass(HINSTANCE, EfctThrTls*);
BOOL				InitEffectWindowInstance(HINSTANCE, EfctThrTls*);
LRESULT CALLBACK	EffectWndProc(HWND, UINT, WPARAM, LPARAM);
int					EffectThreadWithoutD3D9(TEfctThrTls*);

void StartAndWaitEffect(int typeOfEfct, HBITMAP bm, HDC dc)
{
	bFinEfct=FALSE;
TEfctThrTls	tls; 
 switch(typeOfEfct)
 {case 0:case 1: default://InStart window eefect:
	tls.left = conf::wndLeft; tls.top  = conf::wndTop; tls.width = conf::wndWidth;
	tls.height = conf::wndHeight; tls.prcssSetTime = START_EFFECT_TIME;
	tls.bIncrOpasity = TRUE; tls.efctType = 0;
	tls.bmp = bm; tls.bmpDC = dc;
	efctTLSs[0] = NULL; tls.tlsOut = (void**)&efctTLSs[0];
	efctThrdHandle[0]=CreateThread(	NULL,0,
									(LPTHREAD_START_ROUTINE)EffectThread,
									&tls,0,&efctThrdID[0]);
	int rp=0;
	while(!efctTLSs[0])
	{	Sleep(50);
		if(rp++>1000) errhandler(L"CreateEffect",0);
	}
	WaitForSingleObject(efctTLSs[0]->finCircle,INFINITE);
	//efctThrdHandle[0]=NULL;
  break;
 }
 bFinEfct=TRUE;
}

//int EffectThread(DWORD dwTlsIndex,int x,int y,int w,int h,DWORD d,
//				   BOOL bOp, int EfctType, HBITMAP bm, TEfctThrTls **tlsOut)
int EffectThread(TEfctThrTls *TLS)
{
MSG			msg;
EfctThrTls	*tls;

	tls = (EfctThrTls*)LocalAlloc(LPTR, sizeof(EfctThrTls));
	if(!TlsSetValue(dwTlsIndex, tls)) return FALSE;
	
	tls = (EfctThrTls*)TlsGetValue(dwTlsIndex);
	tls->left = TLS->left; tls->top = TLS->top; tls->width = TLS->width; tls->height = TLS->height;
	tls->prcssSetTime = TLS->prcssSetTime; tls->bIncrOpasity = TLS->bIncrOpasity;
	tls->efctType = TLS->efctType; tls->bmp = TLS->bmp; tls->bmpDC = TLS->bmpDC;

	tls->finCircle = CreateEvent(NULL,FALSE,FALSE,NULL);//not auto reset, each Wait func do not
	tls->waitForShowParentEvnt = CreateEvent(NULL,FALSE,FALSE,NULL);//not auto reset, each Wait func do not
	
	if(!tls->finCircle) goto Fin;
	*TLS->tlsOut = tls;//Chaqirgan proc shungacha kutib turishi kerak.

	for(int i=0; i<3; i++)
	{	tls->evnts[i] = CreateEvent(NULL,FALSE,FALSE,NULL);//not auto reset, each Wait func do not
		if(!tls->evnts[i]) goto Fin;
	}
	StringCchPrintf(tls->szEffectWindowTitle,MAX_LOADSTRING,L"Effect window");
	StringCchPrintf(tls->szEffectWindowClass,MAX_LOADSTRING,L"Effect window class");
	TCHAR iInstCntStr[8]; StringCchPrintf(iInstCntStr,8,L"%d",iEffectWndInstCnt++);
	StringCchCat(tls->szEffectWindowClass,MAX_LOADSTRING,iInstCntStr);

	RegisterEffectWindowClass(hInst, tls);

	if(!InitEffectWindowInstance(hInst, tls)) goto Fin;

	if(FAILED(InitD3D(tls)))//goto End;
		errhandler(L"Please, install DirectX9 packet.",0);

	ZeroMemory( &msg, sizeof(msg) );

ProcessBegin:

	DWORD prcssBeginTick = GetTickCount();

    while(msg.message!=WM_QUIT)
    {	
		//if close event sended, close immediately:
		if(WAIT_OBJECT_0==WaitForSingleObject(tls->evnts[0],0))
			PostQuitMessage(0);
		if(WAIT_OBJECT_0==WaitForSingleObject(tls->evnts[2],0))
			goto ProcessBegin;

		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
     	{	TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} else
		{	//Render process;
			Render(tls);

			//if(tls->tick >= (DWORD)(0.75f*tls->prcssSetTime))
			//	SetEvent(tls->finCircle);
				

			//time process;
			tls->tick = GetTickCount() - prcssBeginTick;
			tls->interpTime = (float)tls->tick / tls->prcssSetTime; if(tls->interpTime > 1.0f) tls->interpTime = 1.0f;
			BYTE alpha = (int)(tls->interpTime * 255.0f); if(!tls->bIncrOpasity) alpha = 0xff - alpha;
			if(alpha > 255)
				alpha = 255;//else if(alpha < 55) alpha = 55;

			//End one full circle:
			if(tls->tick >= tls->prcssSetTime)
			{	//Signal to host:
				SetEvent(tls->finCircle);
				
				//Wait for parent SetPos to Z-top:
				WaitForSingleObject(tls->waitForShowParentEvnt,INFINITE);

				//Wait for show window, if need:
				ShowWindow(tls->hEfctWnd, SW_HIDE);

				//if close main window effect, fin all:
				if(tls->efctType==efctInCloseWindow)
					{ PostQuitMessage(0); continue; }
				
				//Wait for next commands:
				//if(WAIT_OBJECT_0==WaitForMultipleObjects(3,tls->evnts,FALSE,INFINITE))//Sleep(INFINITE);//Wait for next calls;
				//	break;
				
				//Begin with show and continue with effect:
				//ShowWindow(tls->hEfctWnd, SW_SHOW);
				
				goto ProcessBegin;
			} else
			{	tls->animLeft = -45+(int)(tls->interpTime*tls->left)-(int)(tls->width*0.5f*(1.0f-tls->interpTime));
				tls->animTop = (int)(tls->interpTime*tls->top)+
				(int)((1.0f-tls->interpTime)*(GetSystemMetrics(SM_CYFULLSCREEN)-tls->height/2));
				MoveWindow(	tls->hEfctWnd,
							tls->animLeft,
							tls->animTop,
							tls->width,
							tls->height,
							TRUE);
	}	}	}
	CleanupD3D(tls);
Fin:
	if(tls->finCircle)CloseHandle(tls->finCircle);
	if(tls->waitForShowParentEvnt)CloseHandle(tls->waitForShowParentEvnt);
	for(int i=0; i<3; i++) if(tls->evnts[i]){ CloseHandle(tls->evnts[i]);tls->evnts[i]=0;}
	if(tls->bmp) DeleteObject(tls->bmp);
	tls = (EfctThrTls*)TlsGetValue(dwTlsIndex);
	if(tls != 0){LocalFree((HLOCAL)tls);tls=0;}
	ExitThread(0);
	return (int)msg.wParam;
}

ATOM RegisterEffectWindowClass(HINSTANCE hInstance, EfctThrTls* tls)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= EffectWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= CreateSolidBrush(RGB(143,200,255));//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= tls->szEffectWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitEffectWindowInstance(HINSTANCE hInstance, EfctThrTls *tls)
{	int x = -tls->width/2;
	int y = GetSystemMetrics(SM_CYFULLSCREEN)-tls->height/2;
	tls->hEfctWnd = CreateWindowEx(	WS_EX_TOOLWINDOW,
									tls->szEffectWindowClass,
									tls->szEffectWindowTitle,
									WS_POPUP,
									x,
									y,
									tls->width,
									tls->height,
									NULL,
									NULL,
									hInstance,
									NULL);
	if (!tls->hEfctWnd) return FALSE;

	//Toki D3D render qilguncha kicjkina bo'lib tursun;
	HRGN rgn = CreateRectRgn(x+tls->width/2-10,y+tls->height/2-10,
							 x+tls->width/2+10,y+tls->height/2+10);
	SetWindowRgn(tls->hEfctWnd,rgn,TRUE);
	DeleteObject(rgn);
	EnableWindow(tls->hEfctWnd,FALSE);
	ShowWindow(tls->hEfctWnd, SW_SHOW);
	UpdateWindow(tls->hEfctWnd);
	return TRUE;
}

LRESULT CALLBACK EffectWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_ERASEBKGND:
			return 1;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
