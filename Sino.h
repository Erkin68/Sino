#pragma once

#include "windows.h"
#include "shellapi.h"
#include "resource.h"
#include "Panel.h"
#include "Sinolib\WndEfctWithD3D9.h"

//#define RT_MANIFEST 24
//#define ISOLATIONAWARE_MANIFEST_RESOURCE_ID 2

#define BEGIN_TRY __try {
#define END_TRY }\
			__except(EXCEPTION_EXECUTE_HANDLER)

#define BEGIN_C_TRY try {
#define END_C_TRY }\
			catch(...)

#define MAX_LOADSTRING 100
#define MAX_ICONS	   2
#define DegToRad(x) x*3.141592653589793238f/180

//hozircha:
#define INFO_EDT_HEIGHT			30
#define INFO_EDT_HEIGHT_INSIDE	18
#define HOT_KEY_BAR_HEIGHT		16

typedef __int32 Int32;
typedef __int64 Int64;
typedef unsigned __int32 UInt32;
typedef unsigned __int64 UInt64;
typedef unsigned char Byte;
typedef int Bool;
#define True 1
#define False 0

extern HINSTANCE	hInst;
extern HWND			hWnd,btns[7],sortBtns[MAX_PANELS][3],sheetTab[MAX_PANELS],cmndsCB,cmndsCBEdt;
extern HFONT		fonts[4];
extern BOOL			bActivate;

extern TEfctThrTls	*efctTLSs[NUM_EFFECT_THREADS];
extern HANDLE		efctThrdHandle[NUM_EFFECT_THREADS];
extern TCHAR		szTitle[MAX_LOADSTRING];
extern TCHAR		szWindowClass[MAX_LOADSTRING];
extern RECT			rcClient;
extern DWORD		tickProced;
extern int			tick;

//******** FROM SINOLIB.LIB **********************************************
//******** FROM SINOLIB.LIB **********************************************
//******** FROM SINOLIB.LIB **********************************************
extern PBITMAPINFO	CreateBitmapInfoStruct(HWND, HBITMAP);
extern void			CreateBMPFile(HWND, LPTSTR, PBITMAPINFO, HBITMAP, HDC);
extern void			errhandler(LPWSTR, HWND);
extern HBITMAP		OpenBMPFile(LPTSTR, HDC, HDC);
extern void			StartAndWaitEffect(int, HBITMAP, HDC);
extern HRESULT		InitD3D(EfctThrTls*);
extern VOID			CleanupD3D(EfctThrTls*);
extern VOID			Render(EfctThrTls*);


//******* COMMONCONTROLS.CPP *********************************************
extern VOID			DeselectAllWithout(int);
extern int			FindMouseClickPanel(int, int);
extern DWORD		GetDllVersion(LPCTSTR);
extern int			panelHeight[MAX_PANELS],pathAndNamesEditY[MAX_PANELS],sortBtnsY[MAX_PANELS],
					selFilesInfoEditY,cmndsComboY;

//******* FileAssociations.cpp *******************************************
extern HICON		MyLoadIcon(char*, int);


//*****  StringOpForSearchFiltr.cpp ************************
//extern int		FindSubstrToAst(wchar_t*, wchar_t*);
extern wchar_t*	FindFirstSubstrTail(wchar_t*, wchar_t*, int);
extern wchar_t*	EmitAst(wchar_t*);
extern int		FindSubstrToAst(wchar_t*, wchar_t*);
extern wchar_t* FindFirstSubstrTail(wchar_t*, wchar_t*, int);
extern wchar_t* EmitAst(wchar_t*);
//extern int	MyStringCmprFltrA(char*, char*, int, int);
extern int		MyStringCmprFltr(wchar_t*, wchar_t*, int, int);


//********* SelectionViaPlus.cpp *************************************************
extern INT_PTR CALLBACK SelectViaPlusDlgProc(HWND,UINT,WPARAM,LPARAM);

//******** MyShell.cpp ***********************************************************
//LRESULT CALLBACK ShellProc(int, WPARAM, LPARAM);
//VOID SetShellHook(BOOL);
//VOID WaitForShellHookDestroyMessage(int,int,int);





// AccessNeed.cpp *****************
extern "C"
{

extern BOOL WINAPI MyCopyFileEx( __in	LPWSTR lpExistingFileName,
						  __in  LPWSTR lpNewFileName,
						  __in  LPPROGRESS_ROUTINE lpProgressRoutine,
						  __in  LPVOID lpData,
						  __in  LPBOOL pbCancel,
						  __in  DWORD dwCopyFlags);
extern BOOL WINAPI MyCopyRenameFileEx( __in  LPWSTR lpExistingFileName,
								__in  LPWSTR lpNewFileName,
								__in  LPPROGRESS_ROUTINE lpProgressRoutine,
								__in  LPVOID lpData,
								__in  LPBOOL pbCancel,
								__in  DWORD dwCopyFlags);
extern BOOL WINAPI MyCopyOverwriteOldestFileEx(__in  LPWSTR lpExistingFileName,
										__in  LPWSTR lpNewFileName,
										__in  LPPROGRESS_ROUTINE lpProgressRoutine,
										__in  LPVOID lpData,
										__in  LPBOOL pbCancel,
										__in  DWORD dwCopyFlags);
extern BOOL WINAPI MyCopyOverwriteLatestFileEx(__in  LPWSTR lpExistingFileName,
										__in  LPWSTR lpNewFileName,
										__in  LPPROGRESS_ROUTINE lpProgressRoutine,
										__in  LPVOID lpData,
										__in  LPBOOL pbCancel,
										__in  DWORD dwCopyFlags);
extern BOOL WINAPI MyCopyOverwriteBigestFileEx(__in  LPCWSTR lpExistingFileName,
										__in  LPCWSTR lpNewFileName,
										__in  LPPROGRESS_ROUTINE lpProgressRoutine,
										__in  LPVOID lpData,
										__in  LPBOOL pbCancel,
										__in  DWORD dwCopyFlags);
extern BOOL WINAPI MyCopyOverwriteLittlestFileEx(__in  LPWSTR lpExistingFileName,
										  __in  LPWSTR lpNewFileName,
										  __in  LPPROGRESS_ROUTINE lpProgressRoutine,
										  __in  LPVOID lpData,
										  __in  LPBOOL pbCancel,
										  __in  DWORD dwCopyFlags);
extern BOOL WINAPI MyCreateDirectory(__in LPWSTR lpPathName,
							  __in LPSECURITY_ATTRIBUTES lpSecurityAttributes);
extern BOOL WINAPI MyDeleteFile(__in LPCWSTR lpFileName);
extern BOOL WINAPI MyMoveFile(__in  LPCWSTR lpExistingFileName,
						__in  LPCWSTR lpNewFileName);
extern BOOL WINAPI MyMoveFileWithProgress(__in  LPCWSTR lpExistingFileName,
									__in  LPCWSTR lpNewFileName,
									__in  LPPROGRESS_ROUTINE lpProgressRoutine,
									__in  LPVOID lpData,
									__in  DWORD dwFlags);
extern BOOL WINAPI MyMoveRenameFileWithProgress(__in  LPCWSTR lpExistingFileName,
										  __in  LPCWSTR lpNewFileName,
										  __in  LPPROGRESS_ROUTINE lpProgressRoutine,
										  __in  LPVOID lpData,
										  __in  DWORD dwFlags);
extern BOOL WINAPI MyMoveOverwriteOldestFileWithProgress(
									__in  LPCWSTR lpExistingFileName,
									__in  LPCWSTR lpNewFileName,
									__in  LPPROGRESS_ROUTINE lpProgressRoutine,
									__in  LPVOID lpData,
									__in  DWORD dwFlags);
extern BOOL WINAPI MyMoveOverwriteLatestFileWithProgress(
									__in  LPCWSTR lpExistingFileName,
									__in  LPCWSTR lpNewFileName,
									__in  LPPROGRESS_ROUTINE lpProgressRoutine,
									__in  LPVOID lpData,
									__in  DWORD dwFlags);
extern BOOL WINAPI MyMoveOverwriteBigestFileWithProgress(
									__in  LPCWSTR lpExistingFileName,
									__in  LPCWSTR lpNewFileName,
									__in  LPPROGRESS_ROUTINE lpProgressRoutine,
									__in  LPVOID lpData,
									__in  DWORD dwFlags);
extern BOOL WINAPI MyMoveOverwriteLittlestFileWithProgress(
									__in  LPCWSTR lpExistingFileName,
									__in  LPCWSTR lpNewFileName,
									__in  LPPROGRESS_ROUTINE lpProgressRoutine,
									__in  LPVOID lpData,
									__in  DWORD dwFlags);
extern BOOL MySHFileOperation(UINT Func,LPWSTR From,LPCWSTR To,FILEOP_FLAGS Flags, BOOL bApndEscape=FALSE);
extern BOOL MyDeleteTryUnicode(HWND prnt,CpyStack* stck);
extern BOOL MySetCurrentDirectory(wchar_t* wst);






}
