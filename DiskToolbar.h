#include "windows.h"


namespace DskToolBar
{
extern int   totLogDsks;
extern HWND  bar[4];
extern DWORD DskDrivesBitMask;
extern wchar_t logDiskNames[32][2];
extern int	   iSameDskPushd[32];
extern wchar_t oldPathsOnDiskPanels[32][MAX_PATH];

extern BOOL  Create(HWND,int*);
extern int	 FindDiskFromPath(wchar_t*);
extern int   GetBtn(POINT*);
extern VOID  Destroy();
extern int   IsPointInsideRect(POINT*);
extern BOOL  Resize(int*);

}