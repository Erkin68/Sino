#ifndef _ENTRY_STEP_H
#define _ENTRY_STEP_H

#include "windows.h"
#include "Operations\Temporary.h"


typedef enum TEntryType
{	unknown=0,
	directFolder=1,
	guidFolder=2,
	rndPathList=3,//result of seachVia7, for sample
	archElem=4,
	socketCl=5,
	virtualPanel=6
} EntryType;

class EntryStep
{
public:
			EntryStep();
	       ~EntryStep();

typedef struct TRecord
{	wchar_t		path[MAX_PATH];
	wchar_t		archPath[MAX_PATH];
	int			childEntr;
	LPVOID      VPPlgObj;
	int         iVPPlg;
	LPVOID      arjPlgObj;
	int         iArjPlg;
	CTempDir    *tmpDir;
	EntryType	type;
} Record;

	VOID		ChangeCrntRecPath(wchar_t*,int);
	VOID		Clear();
	VOID		CopyFrom(EntryStep*);
	VOID		CreateTemporaryDir(Panel*,int);
	wchar_t*	GetCrntRecArchPath();
	char*		GetCrntRecPathA();
	wchar_t*	GetCrntRecPath();
	EntryType	GetCrntRecType();
	EntryType	GetCrnt(wchar_t*,EntryType=unknown);
	int			GetCrntRecChild();
	wchar_t*	GetCrntTemporaryDir();
	CTempDir*   GetCrntTemporary();
	LPVOID		GetCrntRecVPPlgObj();
	int			GetCrntRecIVPPlg();
	LPVOID		GetCrntRecArjPlgObj();
	int			GetCrntRecArjPlgNum();

	//HWND		GetCrntRecRandomListBox();
	VOID		PushA(char*, EntryType, int);
	VOID		Push(wchar_t*, EntryType, int, wchar_t* guidName=NULL);
	VOID		PushFindType(wchar_t*, int, int);
	BOOL		PushToArch(wchar_t*, int, LPVOID,int);
	BOOL		PushToVirtualPanel(wchar_t*, int, LPVOID,int);
	int			PushToFolder(wchar_t*, int, int);
	EntryType	Pop();
	VOID		Reset();
	VOID		SetPushArchEntryInStartup(wchar_t*,wchar_t*);
	VOID		SetPushEntryInStartup(wchar_t*);
	VOID		SetPushGuidEntryInStartup(wchar_t*);
	VOID		ClearCrntTemporary();
	VOID		CheckTemporaryes();

	Record *recs;
	int maxItems;
	int numItems;
};

#endif
