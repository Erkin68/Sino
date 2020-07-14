#ifndef _FILE_ASSOCIATION_H_
#define _FILE_ASSOCIATION_H_

#include "windows.h"



#define MAX_EXT 64

class PanelItem;
class FileAssociation
{
public:
	static VOID CreateBMP(HICON,HBITMAP*,HDC*,int*,int*);

				FileAssociation();
			   ~FileAssociation();

	int			Add(wchar_t*);
	BOOL		AddMasterIcon(PanelItem*,wchar_t*);
	VOID		FreeMem();
	HICON*		GetIcon(int);
	HBITMAP*	GetIconBM(int);
	int			GetIconWidth(int);
	int			GetIconHeight(int);
	HDC*		GetIconDC(int);
	int			IsRecExist(wchar_t*);

typedef struct TCell
{
	wchar_t	ext[MAX_EXT];
	__int8  extLn;

	wchar_t	desc[MAX_EXT];
	__int8  descLn;

	wchar_t	iconPath[MAX_PATH];
	__int8  iconPathLn;
	__int16 iconNum;
	HICON	icon;
	HBITMAP iconBM;
	HDC	  iconDC;
	int   width,height;//original

	wchar_t	appPath[MAX_PATH];
	__int8  appPathLn;

	wchar_t	appDesc[MAX_PATH];
	__int8  appDescLn;
} Cell;

protected:
	Cell	*pBuf;
	int     tot;
	int     totMax;

	BOOL	MemCheck();
	int		IsOwnIcoOrCurExt(wchar_t*);
};

#endif
