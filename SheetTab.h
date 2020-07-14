#ifndef _SHEET_TAB_H_
#define _SHEET_TAB_H_

#include "Panel.h"

class SheetTab
{
public:

static BOOL Save(HWND,int,Panel*);
static BOOL Load(HWND,int,Panel*);

		 SheetTab();
		~SheetTab();
	VOID Destroy();
	BOOL LoadInArch(Panel*);
	BOOL SetPath(wchar_t*);
	BOOL SetAltPath(wchar_t*);

	__int32    *pSelects,*pSelectSts,archPlgNum;
	wchar_t		path[MAX_PATH],archPath[MAX_PATH],archFilePath[MAX_PATH],altPath[MAX_PATH];//When rename from context menu.
	int			pathLn,totSelects,totSelectsFiles,totSelectsFolders,iHot;
	EntryType	entrType;
	enum
	{	normal,
		fixed,
		fixedChangeDir
	} type;
	union
	{U32 viewMenuBitsU32;
	 struct
	 {	U32 bItemsViewType      : 2;//00-short,01-full,10-custom,...etc.
		U32 bItemsCustomName	: 1;//View->Custom b-sa, Name i
		U32 bItemsCustomExt		: 1;//View->Custom b-sa, Ext i
		U32 bItemsCustomCrTime	: 1;//View->Custom b-sa, ...
		U32 bItemsCustomWrTime	: 1;
		U32 bItemsCustomSz		: 1;
		U32 bItemsCustomTotSz	: 1;
		U32 bItemsCustomAtrb	: 1;
		U32 bItemsCustomState	: 1;
		U32 bItemsCustomNameLn	: 1;
		U32 bItemsCustomLstAccTime	: 1;
	 } viewMenuBits;
	};
};


#endif