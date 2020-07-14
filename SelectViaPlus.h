#ifndef _SELECT_VIA_PLUS_H_
#define _SELECT_VIA_PLUS_H_

#include "windows.h"


#define MAX_SEL_PATH   128
#define MAX_SEL_SUBSTR 32

class Panel;


class SelectViaPlus
{
friend INT_PTR CALLBACK SelectViaPlusDlgProc(HWND,UINT,WPARAM,LPARAM);

public:

	typedef struct TSelStrs
	{
		char c[MAX_SEL_PATH];
	}SelStrs;

			SelectViaPlus();
		   ~SelectViaPlus();
	VOID	Select(Panel*);

protected:
	SelStrs *slctnStrs;
	int		iSlctns;//nechta selStr chalar;

	VOID	AddSelection(HWND, int, int);
	VOID	FindSelectsItems(Panel*,char*,int,int);
	VOID	ReadCrntSeln(HWND,int);
	VOID	ReadSelections(HWND,int);
	VOID	RemoveSelections(int);
	VOID	WriteCrntSeln(HWND,int);
};

extern SelectViaPlus fSelViaPlus;
#endif
