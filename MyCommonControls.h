#ifndef _COMMON_CONTROLS_H_
#define _COMMON_CONTROLS_H_

#include "windows.h"

namespace CmnCntrl
{
//extern int TabCtrl_GetRowCount(HWND);
extern BOOL CmndsCBMsgQueue(MSG*);
extern BOOL	CreateControls(HWND,BOOL);
extern BOOL CreateDragAndDrop();
extern BOOL CreatePanelSelFilesInfoEdits(HWND,int*);
extern BOOL CreatePatnAndNameEdit(HWND,int,int*);
extern BOOL	DestroyControls(HWND);
extern BOOL GetClientRect(HWND,RECT*);
extern BOOL GetWindowRect(HWND,RECT*);
extern VOID InsertToCmndLine(Panel*, int);
extern BOOL IsPointInsideCmndCBEdit(POINT*);
extern int  IsPointInsidePatnAndNameEditRect(POINT*);
extern VOID ReleaseDragAndDrop();
extern VOID ReleaseDragAndDrop();
extern BOOL	ResizeControls();
extern BOOL ResizeEachPanelControls(int,int*,int);
extern BOOL ResizePanelSelFilesInfoEdits(int*);
extern BOOL ResizePathAndNamesEdit(int, int*);
extern VOID DrawDskSpace(HDC,int numPanel=-1);

extern HPEN hPenBlue;
}

extern int panelsTop;


#endif
