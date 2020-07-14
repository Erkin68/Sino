#include "WindowsManagmentInstrumentation.h"
#include "Operations\KernelInUserSpace.h"
#include "Operations\VirtualPanel.h"
#include "Operations\LinkSocket.h"
#include "Operations\Temporary.h"
#include "Operations\Execution.h"
#include "Operations\MyErrors.h"
#include "Operations\MyShell\MyShell.h"
#include "DiskToolbar.h"
#include "FillManager.h"
#include "Profiler.h"
#include "registry.h"
#include "strsafe.h"
#include "Config.h"
#include "Panel.h"
#include "Sino.h"


FillManager::FillManager():hFstChngNot(0)// : hFolderInThread(NULL)
{	
	pathForWaitThr[0]='\\';
	pathForWaitThr[1]='\\';
	pathForWaitThr[2]='?';
	pathForWaitThr[3]='\\';
}

FillManager::~FillManager()
{
	DestroyChangeNotify();
}

BOOL FillManager::Fill()//occured only in startup
{	
BOOL r = FALSE;
	switch(p->GetEntry()->GetCrntRecType())
	{	default:case unknown:
			break;
		case directFolder:
			r = FillDirectFolder();
			break;
		case guidFolder:
			r = FillGuidFolder();
			break;
		case rndPathList:
			//r = FillRandomPathList();
			break;
	}
	return r;
}

BOOL FillManager::FillArchItems(wchar_t *path)
{
	if(!p->arch.plgObj)return FALSE;
	p->SetTotItems(0);
	p->SetIdForTooltip(0);
	p->SetNColumns(0);
	p->SetNRows(0);
	p->SetItemMaxWidth(0);
	p->AddItem(L"..",header);
BEGIN_TRY
	archive::plgns[p->arch.plgNum].EnumDirectory$8(p->arch.plgObj,path);
END_TRY
{
}
	p->Sort();

	p->ShowPathAndName(0);
	return TRUE;
}

BOOL FillManager::FillDirectFolder(BOOL bAccessDenied)
{
WIN32_FIND_DATAW ffW;
HANDLE hFind = INVALID_HANDLE_VALUE;

	//SetWaitForPathChangeNotify(NULL,TRUE); Panel::FreeMem ga ko'chdi;

	p->SetTotItems(0);
	p->SetIdForTooltip(0);
	p->SetNColumns(0);
	p->SetNRows(0);
	p->SetItemMaxWidth(0);
	p->AddItem(L"..",header);

	if((!bAccessDenied) && NSKERNEL::pZwCreateFile && (1==conf::Dlg.iEnumMethod))
	{
		NSKERNEL::EnumPanel(p);
	}
	else
	{	/*hFind = MyFindFirstFileEx(p->GetPath(),FindExInfoStandard,&ff,FindExSearchLimitToDirectories,NULL,0);
		do
		{	if(INVALID_HANDLE_VALUE==hFind) return FALSE;
			if(FILE_ATTRIBUTE_DIRECTORY & ff.dwFileAttributes)//if(IsDirectory(p->path, &ff, FALSE))
			{	if(IsCrntOrPrntDirAttrb(ff.cFileName))
				{	p->AddItem(ff.cFileName,folder,0,&ff);
			}	}
			else
			{	p->AddItem(ff.cFileName,file,0,&ff);
		}	}
		while(MyFindNextFile(hFind, &ff));
		MyFindClose(hFind);*/

		//My unicode version changing:
		hFind = MyFindFirstFileEx(p->GetPath(),FindExInfoStandard,&ffW,FindExSearchLimitToDirectories,NULL,0);
		do
		{	if(INVALID_HANDLE_VALUE==hFind) return FALSE;
			if(FILE_ATTRIBUTE_DIRECTORY & ffW.dwFileAttributes)//if(IsDirectory(p->path, &ff, FALSE))
			{	if(IsCrntOrPrntDirAttrb(ffW.cFileName))
					p->AddItem(ffW.cFileName,folder,0,(WIN32_FIND_DATA*)&ffW);
			}
			else
				p->AddItem(ffW.cFileName,file,0,(WIN32_FIND_DATA*)&ffW);
		}
		while(FindNextFile(hFind, &ffW));
		FindClose(hFind);		
	}

	p->Sort();

	for(int i=0; i<conf::Dlg.iTotPanels; i++)
	{	if(p==&panel[i])
		{	for(int t=0; t<DskToolBar::totLogDsks; t++)
			{	if(p->GetPath()[0]==DskToolBar::logDiskNames[t][0] || p->GetPath()[0]==DskToolBar::logDiskNames[t][0]-32)
					SendMessage(DskToolBar::bar[i],TB_CHECKBUTTON,IDB_DSK_BTN0+t,(LPARAM)MAKELONG(TRUE,1));
				else
					SendMessage(DskToolBar::bar[i],TB_CHECKBUTTON,IDB_DSK_BTN0+t,(LPARAM)MAKELONG(FALSE,0));
	}	}	}
	p->ShowPathAndName(0);
	SetChangeNotifyPath(p->GetPath());//Begin waiting;
	return TRUE;
}

BOOL FillManager::FillGuidFolder()
{
	SetChangeNotifyPath(0);//Stop waiting;
	p->SetTotItems(0);
	p->SetIdForTooltip(0);
	p->SetNColumns(0);
	p->SetNRows(0);
	p->SetItemMaxWidth(0);
	//if(p->path[0]>0)//Desktopda NULL seliy istoriya
		p->AddItem(L"..",header);
		myWMI::BuildGuidUseShelAPI(p->GetPath(),p);
	p->Sort();
	p->ShowPathAndName(0);
	return TRUE;
}

BOOL FillManager::FillRandomPathList(LPVOID buf)
{
	//HWND LB = p->GetEntry()->GetCrntRecRandomListBox();
	//int cntLB = SendMessage(fSearchViaF7::hResultsLB,LB_GETCOUNT,0,0);
	//if(LB_ERR == cntLB) return FALSE;

	SetChangeNotifyPath(0);//stop waiting;

	p->SetTotItems(0);
	p->SetIdForTooltip(0);
	p->SetNColumns(0);
	p->SetNRows(0);
	p->SetItemMaxWidth(0);
	p->AddItem(L"..",header);

	/*for(int i=0; i<cntLB; i++)
	{	char s[MAX_PATH];int ln = SendMessage(fSearchViaF7::hResultsLB,LB_GETTEXT,i,(LPARAM)s);
		if(LB_ERR!=ln)
		{	if('<'==s[0])
			{	s[ln-1] = 0;
				p->AddItemFrRandomList(&s[1],folder);
			}
			else
				p->AddItemFrRandomList(s,file);
	}	}*/
	int cnt = *((int*)buf);
	wchar_t *pbuf = (wchar_t*)((char*)buf + sizeof(int));
	for(int i=0; i<cnt; i++)//while(*pbuf)
	{	int ln=MyStringLength(pbuf,MAX_PATH);
		if('<'==pbuf[0])
		{	pbuf[ln-1]=0;
			p->AddItemFrRandomList(&pbuf[1],folder);
			pbuf[ln-1]='>';			
		}
		else p->AddItemFrRandomList(pbuf,file);
		pbuf += ln+1;
		if(!(*pbuf)) break;
	}
	p->Sort();
	p->ShowPathAndName(0);

	return TRUE;
}

BOOL FillManager::FillRandomPathListFrArj(LPVOID buf)
{
typedef struct _MY_WIN32_FIND_DATA {
  DWORD    dwFileAttributes; 
  FILETIME ftCreationTime; 
  FILETIME ftLastAccessTime; 
  FILETIME ftLastWriteTime; 
  DWORD    nFileSizeHigh; 
  DWORD    nFileSizeLow; 
  DWORD    dwReserved0; 
  DWORD    dwReserved1; 
} MY_WIN32_FIND_DATA;


	//HWND LB = p->GetEntry()->GetCrntRecRandomListBox();
	//int cntLB = SendMessage(fSearchViaF7::hResultsLB,LB_GETCOUNT,0,0);
	//if(LB_ERR == cntLB) return FALSE;

	SetChangeNotifyPath(0);//stop waiting;

	p->SetTotItems(0);
	p->SetIdForTooltip(0);
	p->SetNColumns(0);
	p->SetNRows(0);
	p->SetItemMaxWidth(0);
	p->AddItem(L"..",header);

	/*for(int i=0; i<cntLB; i++)
	{	char s[MAX_PATH];int ln = SendMessage(fSearchViaF7::hResultsLB,LB_GETTEXT,i,(LPARAM)s);
		if(LB_ERR!=ln)
		{	if('<'==s[0])
			{	s[ln-1] = 0;
				p->AddItemFrRandomList(&s[1],folder);
			}
			else
				p->AddItemFrRandomList(s,file);
	}	}*/
	int cnt = *((int*)buf);
	wchar_t *pbuf = (wchar_t*)((char*)buf + sizeof(int));
	for(int i=0; i<cnt; i++)//while(*pbuf)
	{	int ln=MyStringLength(pbuf,MAX_PATH);
		if('<'==pbuf[0])
		{	pbuf[ln-1]=0;
			p->AddItemFrRandomListArj(&pbuf[1],folder,ln);
			pbuf[ln-1]='>';			
		}
		else p->AddItemFrRandomListArj(pbuf,file,ln);
		pbuf += ln+1 + sizeof(MY_WIN32_FIND_DATA);
		if(!(*pbuf)) break;
	}
	p->Sort();
	p->ShowPathAndName(0);

	return TRUE;
}

BOOL FillManager::FillRandomPathListIP4(LPVOID buf)
{
wchar_t s[MAX_PATH]=L"\\\\";
	//HWND LB = p->GetEntry()->GetCrntRecRandomListBox();
	//int cntLB = SendMessage(fSearchViaF7::hResultsLB,LB_GETCOUNT,0,0);
	//if(LB_ERR == cntLB) return FALSE;

	SetChangeNotifyPath(0);//stop waiting;

	p->SetTotItems(0);
	p->SetIdForTooltip(0);
	p->SetNColumns(0);
	p->SetNRows(0);
	p->SetItemMaxWidth(0);
	p->AddItem(L"..",header);

	int cnt = *((int*)buf);
	wchar_t *pbuf = (wchar_t*)((char*)buf + sizeof(int));
	for(int i=0; i<cnt; i++)//while(*pbuf)
	{	int ln=MyStringLength(pbuf,MAX_PATH);
		if('<'==pbuf[0])
		{	pbuf[ln-1]=0;
			MyStringCpy(&s[2],MAX_PATH-2,&pbuf[1]);
			p->AddItemFrRandomList(s,folder);
			pbuf[ln-1]='>';			
		}
		else
		{	MyStringCpy(&s[2],MAX_PATH-2,pbuf);
			p->AddItemFrRandomList(s,file);
		}
		pbuf += ln+1;
	}//while(*pbuf);
	p->Sort();
	p->ShowPathAndName(0);

	return TRUE;
}

BOOL FillManager::FolderIn(int id)
{
	if(file==p->pItems[id].attribute)
		if(Execution::tryToExec(p,id))
			return TRUE;
	if(socketCl==p->GetEntry()->GetCrntRecType())
		return linkSock::FolderInClient(p,id);
	else if(archElem==p->GetEntry()->GetCrntRecType())
		return FolderInArch(id);
	else if(virtualPanel==p->GetEntry()->GetCrntRecType())
		return FolderInVirtualPanel(id);

	wchar_t stW[MAX_GUID_CHAR];
int chld = id;
	if(rndPathList==p->GetEntry()->GetCrntRecType())
		chld = 0;

int bAccessDenied=0;
	if(guidElement==p->GetItem(id)->attribute)
	{	
		p->GetEntry()->Push(&p->GetItem(id)->Name[GUID_CHAR_POS],guidFolder,id,p->GetItem(id)->Name);
	}
	else if(folder==p->GetItem(id)->attribute)
	{	bAccessDenied=p->GetEntry()->PushToFolder(p->GetItem(id)->Name,
										  chld,
										  p->GetItem(id)->NameCharLen);
		if(0==bAccessDenied)
			return FALSE;
	}
	EntryType t = p->GetEntry()->GetCrnt(stW,unknown);
	
	//Clear all panel items:
	p->FreeMem();//p->FreeSelection();shartmas

	if(directFolder==t)
	{	p->SetPath(stW,MAX_PATH);
		FillDirectFolder(1==bAccessDenied);
		MySetCurrentDirectory(stW);
		if(bAccessDenied)
			ChangePrivilege(0,17,FALSE);
	}
	else if(guidFolder==t)
	{	p->SetPath(stW,MAX_PATH,FALSE);
		FillGuidFolder();
	}

	//p->Sort(); Fillarda turibdi:
	p->ChangeSheetTabPath();
	p->AdjustScrollity();
	p->SetHot(0);
	p->ClrScr();
	p->Render(NULL);
	return TRUE;
}

BOOL FillManager::FolderInArch(int id)
{
	//SetWaitForPathChangeNotify(NULL,TRUE);//Stop waiting;
	if(archElem!=p->GetEntry()->GetCrntRecType())
		return FALSE;

	p->GetEntry()->Push(p->GetItem(id)->Name,archElem,id);

	//Clear all panel items:
	p->FreeMem();
	wchar_t st[MAX_PATH];int ln = MyStringCpy(st,MAX_PATH-1,p->GetEntry()->GetCrntRecPath());
	st[ln] = '\\';
	st[ln+1] = 0;// MyStringCpy(&st[ln+1],MAX_PATH-1,p->GetEntry()->GetCrntRecArchPath());
	p->SetPath(st,MAX_PATH);
	FillArchItems(p->GetEntry()->GetCrntRecArchPath());
	MySetCurrentDirectory(st);

	//p->Sort(); Fillarda turibdi:
	p->ChangeSheetTabPath();
	p->AdjustScrollity();
	p->SetHot(0);
	p->ClrScr();
	p->Render(NULL);
	return TRUE;
}

BOOL FillManager::FolderInVirtualPanel(int id)
{
	//SetWaitForPathChangeNotify(NULL,TRUE);//Stop waiting;
	if(virtualPanel!=p->GetEntry()->GetCrntRecType())
		return FALSE;

	if(vrtlPanels::plgns[p->iVPPlg].EnumDir$12)//1 xil tip virtual panel,WinObj misol)
	{	wchar_t st[MAX_PATH];
		if(!vrtlPanels::plgns[p->iVPPlg].GetItemPathAndName$16(
							p->VPPlgObj,
							st,MAX_PATH,
							p->GetItem(id)->itemId))
			p->GetFullPathAndName(id,st,MAX_PATH);//goto Ev;//return FALSE;
		if(vrtlPanels::plgns[p->iVPPlg].GetReadyForEnumDir$12(p->GetHWND(),p->VPPlgObj,st))
		{	p->FreeMem();
			p->GetEntry()->PushToVirtualPanel(st,id,p->VPPlgObj,p->iVPPlg);
			p->AddItem(L"..",header);
			vrtlPanels::plgns[p->iVPPlg].EnumDir$12(p->GetHWND(),p->VPPlgObj,st);

			p->Sort();
			p->ChangeSheetTabPath();
			p->AdjustScrollity();
			p->SetHot(0);
			p->ClrScr();
			p->Render(NULL);
		}
		else MessageBox(p->GetHWND(),L"EnumDir$12 not allowed for path:",st,MB_OK);
	}//IP4Scan misol 2- xil virtual panellar bo'lsa:
	//else// if(vrtlPanels::plgns[p->iVPPlg].EventPanelItemClick$16)
	//{	//vrtlPanels::plgns[p->iVPPlg].EventPanelItemClick$16(p->GetHWND(),
		//														p->VPPlgObj,
		//														/*id,*/p->GetItem(id)->assocNum,
		//														p->GetItem(id)->Name);
		//vrtlPanels::EventPanelItemClick$16(p->GetHWND(),p->GetArch()->plgNum,
		//														p->VPPlgObj,
		//														/*id,*/p->GetItem(id)->assocNum,
		//														p->GetItem(id)->Name);
	//}
	return TRUE;
}

BOOL FillManager::FolderUpVirtualPanel(int id,BOOL bCloseVirtual)
{
	//SetWaitForPathChangeNotify(NULL,TRUE);//Stop waiting;
	if(virtualPanel!=p->GetEntry()->GetCrntRecType())
		return FALSE;

if(bCloseVirtual || vrtlPanels::plgns[p->iVPPlg].EnumDir$12)//1 xil tip virtual panel,WinObj misol)
{	int ln=0;wchar_t *pp,st[MAX_PATH],stvp[MAX_PATH];
	if(!vrtlPanels::plgns[p->iVPPlg].GetItemPathAndName$16(
							p->VPPlgObj,
							stvp,MAX_PATH,
							id ? p->GetItem(id)->itemId : 0xffffffff))
		ln = MyStringCpy(stvp,MAX_PATH,p->GetPath());//p->GetFullPathAndName(id,stvp,MAX_PATH);

	EntryStep *e = p->GetEntry();//o'shaning ustiga hot qilish uchun;
	pp = wcsrchr(stvp,'\\');
	if(pp)
	{	if(pp)
		{	*pp = 0;
			pp = wcsrchr(stvp,'\\');
			if(pp)
			{	ln=MyStringCpy(p->findItemName,MAX_PATH-1,pp+1);
				*pp=0;
			}
			else p->findItemName[0]=0;
	}	}

	int iPlg = p->GetEntry()->GetCrntRecIVPPlg();
	LPVOID VPPlgObj = p->GetEntry()->GetCrntRecVPPlgObj();
		
	EntryType oldT;EntryType t;
	do
	{	oldT = e->Pop();
		t = p->GetEntry()->GetCrnt(st,oldT);
	}
	while(bCloseVirtual && virtualPanel==t);

	if(unknown==t)return FALSE;

	else if(virtualPanel==t && (!vrtlPanels::plgns[p->iVPPlg].GetReadyForEnumDir$12(p->GetHWND(),p->VPPlgObj,stvp)))
	{	MessageBox(p->GetHWND(),L"EnumDir$12 not allowed for path:",stvp,MB_OK);
		return FALSE;
	}

	p->FreeMem();//p->FreeSelection(); shartmas
	if(directFolder==t)
	{	p->arch.Close$4();
		p->SetPath(st,MAX_PATH);
		FillDirectFolder();
		MySetCurrentDirectory(st);
	}
	else if(guidFolder==t)
	{	p->arch.Close$4();
		p->SetPath(st,MAX_PATH,FALSE);
		FillGuidFolder();
	}
	else if(archElem==t)
	{	wchar_t *pp=wcsrchr(st,'.');
		if(pp)
		{	int pn = archive::GetUnpackPlgnNum(pp+1);
			if(pn>-1)
			{	if(p->arch.plgNum != pn)
				{	archive::plgns[p->arch.plgNum].Close$4(p->arch.plgObj);
					if(!p->arch.OpenForUnpacking$8(e->GetCrntRecPath(),pn))return FALSE;
					//p->arch.plgNum = pn;
		}	}	}
		FillArchItems(e->GetCrntRecArchPath());
		//MyStringCpy(p->findItemName,MAX_PATH-1,archNameForFindItem); kerakmas,archUp da turibdi;
	}
	else if(virtualPanel==t)
	{	p->AddItem(L"..",header);
		vrtlPanels::plgns[p->iVPPlg].EnumDir$12(p->GetHWND(),p->VPPlgObj,stvp);
	}
	if(virtualPanel!=t)
	{	p->VPPlgObj=0;//p->iVPPlg=0;
		vrtlPanels::plgns[iPlg].DetachPanel$8(VPPlgObj,p);//FreePlugin();
		vrtlPanels::plgns[iPlg].FreePlugin();
	}

	p->Sort();
	p->ChangeSheetTabPath();
	p->AdjustScrollity();

	if(p->findItemName[0])
	{	if(ln>4)
		{	pp=wcsrchr(p->findItemName,'\\');
			if(pp)
				MyStringCpy(p->findItemName,ln,pp+1);//strcpy_s(p->findItemName,ln,pp+1);
			p->SetHot(p->FindItem(p->findItemName/*pp+1*/),FALSE);
			p->findItemName[0]=0;
		}
		else
		{	int fi = p->FindItem(p->findItemName);
			if(fi>-1)
				p->SetHot(fi,FALSE);
			p->findItemName[0]=0;//ShowPath to'g'ri ishlashi uchun;
	}	}
	p->ClrScr();
	p->Render(NULL);
	p->ShowSelectedFilesInfo();

}//IP4Scan misol 2- xil virtual panellar bo'lsa:
//else// if(vrtlPanels::plgns[p->iVPPlg].EventPanelItemClick$16)
//{	//vrtlPanels::plgns[p->iVPPlg].EventPanelItemClick$16(p->GetHWND(),p->VPPlgObj,0,NULL);
//	vrtlPanels::EventPanelItemClick$16(p->GetHWND(),p->iVPPlg,p->arch.VPPlgObj,0,NULL);
//}

	return TRUE;
}

/*BOOL FillManager::FolderEnumInVirtualPanel(wchar_t *path)
{
	//SetWaitForPathChangeNotify(NULL,TRUE);//Stop waiting;
	if(virtualPanel!=p->GetEntry()->GetCrntRecType())
		return FALSE;

	if(!vrtlPanels::plgns[p->iVPPlg].GetReadyForEnumDir$12(p->GetHWND(),p->VPPlgObj,path)
	{	MessageBox(p->GetHWND(),L"EnumDir$12 not allowed for path:",stvp,MB_OK);
		return FALSE;
	}

	p->FreeMem();//p->FreeSelection(); shartmas
	p->AddItem(L"..",header);
	if(vrtlPanels::plgns[p->iVPPlg].EnumDir$12)
		vrtlPanels::plgns[p->iVPPlg].EnumDir$12(p->GetHWND(),p->VPPlgObj,path);

	p->Sort();
	p->AdjustScrollity();

	if(p->findItemName[0])
	{	p->SetHot(p->FindItem(p->findItemName),FALSE);
/*		p->findItemName[0]=0;
	}
	else
	{	int fi = p->FindItem(p->findItemName);
		if(fi>-1)
			p->SetHot(fi,FALSE);
		p->findItemName[0]=0;//ShowPath to'g'ri ishlashi uchun;
	}
	p->ClrScr();
	p->Render(NULL);
	p->ShowSelectedFilesInfo();

	return TRUE;
}*/

BOOL FillManager::FolderInToArch(int archPlgnNum,int id)
{
	//SetWaitForPathChangeNotify(NULL,TRUE);//Stop waiting;
	if(archElem==p->GetEntry()->GetCrntRecType())
		return FolderInArchToArch(archPlgnNum,id);
	if(directFolder!=p->GetEntry()->GetCrntRecType())
	{	if(virtualPanel!=p->GetEntry()->GetCrntRecType())
			return FALSE;
		else
		{	if(wcscmp(vrtlPanels::plgns[p->iVPPlg].name,L"AdptrIP4Scan"))
				return FALSE;
			//p->GetFullPathAndName(id,p->findItemName,260);//Chiqish uchun;
	}	}

/*	int id;
	if(p->GetTotSelects()<=1 || p->GetTotSelects()>p->GetTotItems()-1)
		id = p->GetHot();
	else//get from selection:
		id=p->GetSelectedItemNum(0);*/

	if(!p->arch.OpenForUnpacking$8(p->GetFullPathAndName(id),archPlgnNum))
		return FALSE;

	if(!p->GetEntry()->PushToArch(p->GetFullPathAndName(id),id,p->arch.plgObj,p->arch.plgNum))
	{	
	  BEGIN_TRY
		archive::plgns[p->arch.plgNum].Close$4(p->arch.plgObj);
	  END_TRY
	  {
	  }
		return FALSE;
	}
	
	//Clear all panel items:
	p->FreeMem();//p->FreeSelection();shartmas

	//p->SetPath(st,MAX_PATH);
	FillArchItems(L"");
	//MySetCurrentDirectory(st);
	
	//p->Sort(); Fillarda turibdi:
	p->ChangeSheetTabPath();
	p->AdjustScrollity();
	p->SetHot(0);
	p->ClrScr();
	p->Render(NULL);
	//archive::plgns[p->arch.plgNum].Close$4(p->arch.plgObj);
	//archive::plgns[p->arch.plgNum].bOpen=FALSE;
	return TRUE;
}

BOOL FillManager::FolderInArchToArch(int archPlgnNum,int id)
{
BOOL bCancel=FALSE;__int32 bools;//,l;wchar_t s[MAX_PATH];
	if(archElem!=p->GetEntry()->GetCrntRecType()) return FALSE;
	p->GetEntry()->CreateTemporaryDir(p,id);
	p->GetEntry()->GetCrntTemporary()->AddSlashToPath();
	p->GetEntry()->GetCrntTemporary()->SetPlgNum(p->arch.plgNum);
	//l=MyStringCpy(s,MAX_PATH,p->GetEntry()->GetCrntTemporaryDir());
	//MyStringCpy(&s[l],MAX_PATH-1,p->GetItem(id)->Name);

int pn=-1;
 BEGIN_TRY
	if(!archive::plgns[p->arch.plgNum].Unpack$28(
							p->GetHWND(),
							p->arch.GetPlgObj(),
							p->GetEntry()->GetCrntTemporaryDir(),//s
							p->GetArcItPathAndName(id),
							&bCancel,
							0,//(archive::UnpackProgressRoutine_t)UnpackCpyProgressRoutine,
							&bools))
	{	p->GetEntry()->GetCrntTemporary()->Close();
		return FALSE;
	}
	archive::plgns[p->arch.plgNum].Close$4(p->arch.plgObj);
	pn = archive::GetUnpackPlgnNum(wcsrchr(p->GetArcItPathAndName(id),'.')+1);
	if(pn<0)return FALSE;
	if(!p->arch.OpenForUnpacking$8(p->GetEntry()->GetCrntTemporary()->GetItem(0),pn))return FALSE;
  END_TRY
  {	return FALSE;
  }  
	if(!p->GetEntry()->PushToArch(p->GetEntry()->GetCrntTemporary()->GetItem(0),id,p->arch.GetPlgObj(),pn))
		return FALSE;
	p->arch.plgNum = pn;

	//Clear all panel items:
	p->FreeMem();//p->FreeSelection();shartmas

	//p->SetPath(st,MAX_PATH);
	FillArchItems(L"");
	//MySetCurrentDirectory(st);
	
	//p->Sort(); Fillarda turibdi:
	p->ChangeSheetTabPath();
	p->AdjustScrollity();
	p->SetHot(0);
	p->ClrScr();
	p->Render(NULL);
	//archive::plgns[p->arch.plgNum].Close$4(p->arch.plgObj);
	//archive::plgns[p->arch.plgNum].bOpen=FALSE;
	return TRUE;
}

BOOL FillManager::FolderUp(int id)
{	if(linkSock::sockDlg[p->iThis])//Destroy bo'lgan bo'lsa pastga
	if(socketCl==p->GetEntry()->GetCrntRecType())//o'tishi shart;
		return linkSock::FolderUpClient(p,id);

	if(archElem==p->GetEntry()->GetCrntRecType())//o'tishi shart;
		return FolderUpInArch(id);

	if(virtualPanel==p->GetEntry()->GetCrntRecType())//o'tishi shart;
		return FolderUpVirtualPanel(id,FALSE);

	if(rndPathList!=p->GetEntry()->GetCrntRecType())//o'tishi shart;
	if(!p->GetPath()[0])//This means desktop;
		return FALSE;//Top of entry, bolshe nekuda;

	//Chiqqanda qaysi papkadan chiqqanligini save qilish,
	EntryStep *e = p->GetEntry();//o'shaning ustiga hot qilish uchun;
	int ln=0;
	if(directFolder==e->GetCrntRecType())
	{	ln=MyStringCpy(p->findItemName,MAX_PATH-1,e->GetCrntRecPath());
		if(ln>4)
			p->findItemName[ln-2]=0;
		else
			p->findItemName[ln-1]=0;
	}

wchar_t st[MAX_GUID_CHAR];
	EntryType oldT = e->Pop();
	EntryType t = p->GetEntry()->GetCrnt(st,oldT);
	if(unknown==t)return FALSE;

	//Clear all panel items:
	p->FreeMem();//p->FreeSelection(); shartmas

	if(directFolder==t)
	{	p->SetPath(st,MAX_PATH);
		FillDirectFolder();
		MySetCurrentDirectory(st);
	}
	else if(guidFolder==t)
	{	p->SetPath(st,MAX_PATH,FALSE);
		FillGuidFolder();
	}
	else if(archElem==t)//random path arj dan chiqqan b-sa:
	{	wchar_t st[MAX_PATH];int ln = MyStringCpy(st,MAX_PATH-1,p->GetEntry()->GetCrntRecPath());
		st[ln] = '\\';
		st[ln+1] = 0;// MyStringCpy(&st[ln+1],MAX_PATH-1,p->GetEntry()->GetCrntRecArchPath());
		p->SetPath(st,MAX_PATH);
		FillArchItems(p->GetEntry()->GetCrntRecArchPath());
	}

	//p->Sort(); Fillarda;
	p->ChangeSheetTabPath();
	p->AdjustScrollity();

	if(p->findItemName[0])
	{	if(ln>4)
		{	wchar_t *ps=wcsrchr(p->findItemName,'\\');
			if(ps)
				MyStringCpy(p->findItemName,ln,ps+1);//strcpy_s(p->findItemName,ln,ps+1);
			p->SetHot(p->FindItem(p->findItemName/*ps+1*/),FALSE);				
		}
		else p->SetHot(p->FindItem(p->findItemName),FALSE);
	}
	p->ScrollItemToView(p->GetHot());
	p->ClrScr();
	p->Render(NULL);
	return TRUE;
}

BOOL FillManager::FolderUpInArch(int id)
{
	if(archElem!=p->GetEntry()->GetCrntRecType())//o'tishi shart;
		return FALSE;

	//Chiqqanda qaysi papkadan chiqqanligini save qilish,
	EntryStep *e = p->GetEntry();//o'shaning ustiga hot qilish uchun;
	int ln=MyStringCpy(p->findItemName,MAX_PATH-1,e->GetCrntRecArchPath());
	p->findItemName[ln-1]=0;

wchar_t archNameForFindItem[MAX_PATH];wchar_t *pp=wcsrchr(e->GetCrntRecPath(),'\\');
	if(pp) MyStringCpy(archNameForFindItem,MAX_PATH-1,pp+1);

	wchar_t st[MAX_GUID_CHAR];
	EntryType oldT = e->Pop();
	EntryType t = p->GetEntry()->GetCrnt(st,oldT);
	if(unknown==t)return FALSE;

	if(p->GetEntry()->GetCrntTemporary())
	{	p->GetEntry()->GetCrntTemporary()->Restore(p);
		p->GetEntry()->ClearCrntTemporary();
	}

	//Clear all panel items:
	p->FreeMem();//p->FreeSelection(); shartmas

	if(directFolder==t)
	{	p->arch.Close$4();
		p->SetPath(st,MAX_PATH);
		FillDirectFolder();
		MySetCurrentDirectory(st);
		MyStringCpy(p->findItemName,MAX_PATH-1,archNameForFindItem);
	}
	else if(guidFolder==t)
	{	p->arch.Close$4();
		p->SetPath(st,MAX_PATH,FALSE);
		FillGuidFolder();
	}
	else if(archElem==t)
	{	wchar_t *pp=wcsrchr(st,'.');
		if(pp)
		{	int pn = archive::GetUnpackPlgnNum(pp+1);
			if(pn>-1)
			{	if(p->arch.plgNum != pn)
				{	archive::plgns[p->arch.plgNum].Close$4(p->arch.plgObj);
					if(!p->arch.OpenForUnpacking$8(e->GetCrntRecPath(),pn))return FALSE;
					//p->arch.plgNum = pn;
		}	}	}
		FillArchItems(e->GetCrntRecArchPath());
		//MyStringCpy(p->findItemName,MAX_PATH-1,archNameForFindItem); kerakmas,archUp da turibdi;
	}
	else if(virtualPanel==t)
	{	p->arch.Close$4();
		MyStringCpy(p->findItemName,MAX_PATH-1,archNameForFindItem);
		//if(vrtlPanels::plgns[p->iVPPlg].EnumDir$12)
		//	return FolderEnumInVirtualPanel(p->path);
		//else 
		//	return vrtlPanels::EventPanelItemClick$16(	p->GetHWND(),
		//												p->GetEntry()->GetCrntRecIPlg(),
		//												p->GetEntry()->GetCrntRecPlgObj(),
		//												0,NULL);
	}

	//p->Sort(); Fillarda;
	p->ChangeSheetTabPath();
	p->AdjustScrollity();

	if(p->findItemName[0])
	{	if(ln>4)
		{	wchar_t *ps=wcsrchr(p->findItemName,'\\');
			if(ps)
				MyStringCpy(p->findItemName,ln,ps+1);//strcpy_s(p->findItemName,ln,ps+1);
			p->SetHot(p->FindItem(p->findItemName/*ps+1*/),FALSE);
			p->findItemName[0]=0;
		}
		else
		{	int fi = p->FindItem(p->findItemName);
			if(fi>-1)
				p->SetHot(fi,FALSE);
			p->findItemName[0]=0;//ShowPath to'g'ri ishlashi uchun;
	}	}
	p->ScrollItemToView(p->GetHot());
	p->ClrScr();
	p->Render(NULL);
	if(directFolder==t)
		p->ShowSelectedFilesInfo();
	return TRUE;
}

VOID FillManager::SetNillPaths()
{
	wchar_t pt[MAX_PATH] = L"\\\\.\\C:";HANDLE hd = NULL;
	for(int i=0; i<4; i++)
	{	hd = CreateFile(pt,0,FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,OPEN_EXISTING,0,NULL);
		if(hd == INVALID_HANDLE_VALUE){pt[4]++;continue;}//keyingi disk;
		int l=MyStringCpy(conf::panel_path[0],MAX_PATH-4,&pt[4]);//StringCchPrintf(conf::panel_path[0],MAX_PATH-4,&pt[4]);
		conf::panel_path[0][l++]='\\';//MyStringCat(conf::panel_path[0],MAX_PATH-1,"\\*");
		conf::panel_path[0][l++]='*';
		conf::panel_path[0][l]=0;
		CloseHandle(hd); break;
	}
	if(INVALID_HANDLE_VALUE == hd)Err::fatalExit(L"Can't open 0 - disk.",hWnd);
	MyStringCpy(&conf::panel_path[1][0],MAX_PATH-8,&conf::panel_path[0][0]);//StringCchPrintf
	MyStringCpy(&conf::panel_path[2][0],MAX_PATH-8,&conf::panel_path[0][0]);
	MyStringCpy(&conf::panel_path[3][0],MAX_PATH-8,&conf::panel_path[0][0]);
}

VOID FillManager::SetParent(Panel* prnt)
{
	p = prnt;
}

BOOL FillManager::AnalyzeDirChangeNotify()
{
	if(!hFstChngNot) return FALSE;
	if(WAIT_OBJECT_0!=WaitForSingleObject(hFstChngNot,0))return FALSE;
	if(!wcsncmp(p->GetPath(),&pathForWaitThr[4],p->GetPathLn()-1))
		p->OnDirectoryChangeNotify();
	if(!FindNextChangeNotification(hFstChngNot))
	{	hFstChngNot = FindFirstChangeNotification(pathForWaitThr,FALSE,
												FILE_NOTIFY_CHANGE_FILE_NAME |
												FILE_NOTIFY_CHANGE_DIR_NAME  |
												FILE_NOTIFY_CHANGE_ATTRIBUTES|
												FILE_NOTIFY_CHANGE_SIZE	     |
												FILE_NOTIFY_CHANGE_LAST_WRITE|
												FILE_NOTIFY_CHANGE_SECURITY  );
		if(INVALID_HANDLE_VALUE==hFstChngNot)
		{	Err::msg(p->GetHWND(),-1,L"FindNextChangeNotification error.");
			return FALSE;
	}	}
	return TRUE;
}

BOOL FillManager::CreateChangeNotify(wchar_t* path)
{
	int l=MyStringCpy(&pathForWaitThr[4],MAX_PATH-5,path);
	pathForWaitThr[0]='\\';
	pathForWaitThr[1]='\\';
	pathForWaitThr[2]='?';
	pathForWaitThr[3]='\\';
	pathForWaitThr[3+l]=0;
	hFstChngNot = FindFirstChangeNotification(pathForWaitThr,FALSE,
											FILE_NOTIFY_CHANGE_FILE_NAME |
											FILE_NOTIFY_CHANGE_DIR_NAME  |
											FILE_NOTIFY_CHANGE_ATTRIBUTES|
											FILE_NOTIFY_CHANGE_SIZE	     |
											FILE_NOTIFY_CHANGE_LAST_WRITE|
											FILE_NOTIFY_CHANGE_SECURITY  );
	if(INVALID_HANDLE_VALUE==hFstChngNot)
	{	Err::msg(p->GetHWND(),-1,L"FindFirstChangeNotification");
		return FALSE;
	}
	return TRUE;
}

VOID FillManager::DestroyChangeNotify()
{
	if(hFstChngNot)
	{	FindCloseChangeNotification(hFstChngNot);
		hFstChngNot=NULL;
		pathForWaitThr[4]=0;
}	}

BOOL FillManager::SetChangeNotifyPath(wchar_t *path)
{
	if(hFstChngNot)
	{	FindCloseChangeNotification(hFstChngNot);
		hFstChngNot=NULL;
	}
	if(!path)
	{	hFstChngNot=NULL;
		//pathForWaitThr[0]=0;
		return TRUE;
	}
	//else:
	int l = MyStringCpy(&pathForWaitThr[4],MAX_PATH-5,p->path);
	pathForWaitThr[0]='\\';
	pathForWaitThr[1]='\\';
	pathForWaitThr[2]='?';
	pathForWaitThr[3]='\\';
	pathForWaitThr[3+l]=0;
	hFstChngNot = FindFirstChangeNotification(pathForWaitThr,FALSE,
											FILE_NOTIFY_CHANGE_FILE_NAME |
											FILE_NOTIFY_CHANGE_DIR_NAME  |
											FILE_NOTIFY_CHANGE_ATTRIBUTES|
											FILE_NOTIFY_CHANGE_SIZE	     |
											FILE_NOTIFY_CHANGE_LAST_WRITE|
											FILE_NOTIFY_CHANGE_SECURITY  );
	if(INVALID_HANDLE_VALUE==hFstChngNot)
	{	Err::msg(p->GetHWND(),-1,L"FindFirstChangeNotification");
		return FALSE;
	}
	return TRUE;
}