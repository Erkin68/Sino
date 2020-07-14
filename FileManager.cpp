#include "WindowsManagmentInstrumentation.h"
#include "FileManager.h"
#include "Profiler.h"
#include "registry.h"
#include "strsafe.h"
#include "Config.h"
#include "Panel.h"
#include "Sino.h"


FileManager::FileManager():hFstChngNot(0),waitForChngeNotThr(0)// : hFolderInThread(NULL)
{
	pathForWaitThr[0]=0;
}

FileManager::~FileManager()
{
}

BOOL FileManager::Fill()
{	
BOOL r = FALSE;
	switch(p->entry.GetCrntRecType())
	{	default:case unknown:
			break;
		case directFolder:
			r = FillDirectFolder();
			break;
		case guidFolder:
			r = FillGuidFolder();
			break;
	}
	return r;
}

BOOL FileManager::FillDirectFolder()
{
WIN32_FIND_DATA ff;
HANDLE			hFind = INVALID_HANDLE_VALUE;

	p->totItems = p->idForTooltip = p->nColumns = 0; p->nRows = 0; p->itemMaxWidth = 0;
	p->iFirstCtrlPressedId = p->iFirstShftPressedId = p->oldSelectId = -1;
	p->AddItem("..",header);

	hFind = FindFirstFile(p->path, &ff);
	if(INVALID_HANDLE_VALUE==hFind) return FALSE;

	if(IsDirectory(p->path, &ff, TRUE))
	{	if(IsCrntOrPrntDirAttrb(&ff))
			p->AddItem(ff.cFileName,folder,0,&ff);
	}
	else
		p->AddItem(ff.cFileName,file,0,&ff);

	while(FindNextFile(hFind, &ff))
	{	if(IsDirectory(p->path, &ff, FALSE))
		{	if(IsCrntOrPrntDirAttrb(&ff))
				p->AddItem(ff.cFileName,folder,0,&ff);
		}
		else
			p->AddItem(ff.cFileName,file,0,&ff);
	}
    FindClose(hFind);
	p->Sort();

	for(int i=0; i<conf.iTotPanels; i++)
	{	if(p==&panel[i])
		{	for(int t=0; t<totLogDsks; t++)
			{	if(p->path[0]==logDiskNames[t][0] || p->path[0]==logDiskNames[t][0]-32)
					SendMessage(dskToolBar[i],TB_CHECKBUTTON,IDB_DSK_BTN0+t,(LPARAM)MAKELONG(TRUE,1));
				else
					SendMessage(dskToolBar[i],TB_CHECKBUTTON,IDB_DSK_BTN0+t,(LPARAM)MAKELONG(FALSE,0));
	}	}	}

	//SetWaitForPathChangeNotify();
	return TRUE;
}

BOOL FileManager::FillGuidFolder()
{
	p->totItems = p->idForTooltip = p->nColumns = 0; p->nRows = 0; p->itemMaxWidth = 0;
	p->iFirstCtrlPressedId = p->iFirstShftPressedId = p->oldSelectId = -1;
	//if(p->path[0]>0)//Desktopda NULL seliy istoriya
		p->AddItem("..",header);
	myWMI.BuildGuidUseShelAPI((WCHAR*)p->path,p);
	p->Sort();
	return TRUE;
}

BOOL FileManager::FolderIn(int id)
{
char st[MAX_PATH];

	if(guidElement==p->pItems[id].attribute)
	{	
		p->entry.Push((char*)p->pItems[id].Guid,guidFolder,id);
	}
	else if(folder==p->pItems[id].attribute)
	{
		p->entry.PushToFolder(p->pItems[id].Name,id,p->pItems[id].NameCharLen);
	}
	EntryType t = p->entry.GetCrnt(st);
	
	//Clear all panel items:
	p->DeselectAllWithout(-1);
	p->FreeMem();

	if(directFolder==t)
	{	StringCchPrintf(p->path,MAX_PATH,"%s",st);
		FillDirectFolder();
		SetCurrentDirectory(st);
	}
	else if(guidFolder==t)
	{	StringCchPrintfW((WCHAR*)p->path,MAX_PATH,L"%s",(WCHAR*)st);
		FillGuidFolder();
	}

	//p->Sort(); Fillarda turibdi:
	p->AdjustHorScrollity();
	p->AddToSelection(0,3,FALSE);
	p->ClrScr();
	p->Render(NULL);
	p->oldSelectId = 0;
	return TRUE;
}

BOOL FileManager::FolderUp(int id)
{
char st[MAX_PATH];
	EntryType oldT = p->entry.Pop();
	EntryType t = p->entry.GetCrnt(st,oldT);

	//Clear all panel items:
	p->DeselectAllWithout(-1);
	p->FreeMem();

	if(directFolder==t)
	{	StringCchPrintf(p->path,MAX_PATH,"%s",st);
		FillDirectFolder();
		SetCurrentDirectory(st);
	}
	else if(guidFolder==t)
	{	StringCchPrintfW((STRSAFE_LPWSTR)p->path,MAX_PATH/2,L"%s",st);
		FillGuidFolder();
	}

	//p->Sort(); Fillarda;
	p->AdjustHorScrollity();
	p->oldSelectId = p->entry.GetCrntRecChild();
	p->AddToSelection(p->oldSelectId,3,FALSE);
	p->ScrollToSelectItemView(p->oldSelectId);
	p->ClrScr();
	p->Render(NULL);
	return TRUE;
}

VOID FileManager::SetNillPaths(Conf* conf)
{
	char pt[MAX_PATH] = "\\\\.\\C:";HANDLE hd = NULL;
	for(int i=0; i<4; i++)
	{	hd = CreateFile(pt,0,FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,OPEN_EXISTING,0,NULL);
		if(hd == INVALID_HANDLE_VALUE){pt[4]++;continue;}//keyingi disk;
		StringCchPrintf(conf->panel_path[0],MAX_PATH-4,&pt[4]);
		StringCchCat(conf->panel_path[0],MAX_PATH,"\\*");
		CloseHandle(hd); break;
	}
	if(INVALID_HANDLE_VALUE == hd) fatalExit("Can't open 0 - disk.");
	StringCchPrintf(&conf->panel_path[1][0],MAX_PATH-8,&conf->panel_path[0][0]);
	StringCchPrintf(&conf->panel_path[2][0],MAX_PATH-8,&conf->panel_path[0][0]);
	StringCchPrintf(&conf->panel_path[3][0],MAX_PATH-8,&conf->panel_path[0][0]);
}

VOID FileManager::SetParent(Panel* prnt)
{
	p = prnt;
}

VOID WaitForChangeDirNotify(FileManager *f)
{
	WaitForSingleObject(f->hFstChngNot,INFINITE);
	if(strstr(f->p->path,&f->pathForWaitThr[4]))
	{	f->p->DeselectAllWithout(-1);
		f->p->FreeMem();
		StringCchPrintf(f->p->path,MAX_PATH,"%s",&f->pathForWaitThr[4]);
		f->FillDirectFolder();
		//SetCurrentDirectory(st);
		//p->Sort(); Fillarda turibdi:
		f->p->AdjustHorScrollity();
		f->p->AddToSelection(0,3,FALSE);
		f->p->ClrScr();
		f->p->Render(NULL);
		f->p->oldSelectId = 0;
	}
	f->waitForChngeNotThr = NULL;
	ExitThread(0);
}

VOID FileManager::SetWaitForPathChangeNotify()
{return;
	if(waitForChngeNotThr)
	{	ResumeThread(waitForChngeNotThr);
		CloseHandle(waitForChngeNotThr);
		waitForChngeNotThr = NULL;
	}

	//Set notify event log, if path will be change:	
	int l = sprintf_s(pathForWaitThr,MAX_PATH,"%s%s","\\\\?\\",p->path);
	pathForWaitThr[l-1]=0;//  \* uchun
	hFstChngNot = FindFirstChangeNotificationA(pathForWaitThr,FALSE,
											FILE_NOTIFY_CHANGE_FILE_NAME |
											FILE_NOTIFY_CHANGE_DIR_NAME  |
											FILE_NOTIFY_CHANGE_ATTRIBUTES|
											FILE_NOTIFY_CHANGE_SIZE	     |
											FILE_NOTIFY_CHANGE_LAST_WRITE);
	if(INVALID_HANDLE_VALUE!=hFstChngNot)
	waitForChngeNotThr = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)WaitForChangeDirNotify,this,0,NULL);
}

BOOL IsDirectory(char *path, WIN32_FIND_DATA *ff, BOOL bFirstEntr)
{
static char st[MAX_PATH]; static char *pend;
	if(bFirstEntr)
	{	strcpy_s(st,path);
		pend = strrchr(st,'*');
		if(pend) *pend = 0;
		else pend = st;
	}

	if(ff->nFileSizeHigh > 0) return FALSE;
	if(ff->nFileSizeLow  > 0) return FALSE;
	if(ff->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return TRUE;

#pragma warning( disable : 4995)
#pragma warning( disable : 4996)
	strcpy(pend,ff->cFileName);//strcat_s(st,"\\.");
#pragma warning( default : 4995)
#pragma warning( default : 4996)

	HANDLE h = CreateFile(	st,
							FILE_READ_DATA,
							FILE_SHARE_READ,
							NULL,
							OPEN_EXISTING,  
							FILE_ATTRIBUTE_READONLY,
							NULL);
    if(h == INVALID_HANDLE_VALUE) return TRUE;
	CloseHandle(h);
	return FALSE;
}