#include "SheetTab.h"
#include "Sino.h"
#include "commctrl.h"
#include "shlGuid.h"
#include "Operations\VirtualPanel.h"
#include "Operations\MyShell\MyShell.h"



SheetTab::SheetTab():pSelects(NULL),pSelectSts(NULL),pathLn(0),totSelects(0),type(normal),
entrType(directFolder),totSelectsFolders(0),totSelectsFiles(0),iHot(0),archPlgNum(0),viewMenuBitsU32(0)
{
	path[0]=altPath[0]=archPath[0]=archFilePath[0]=0;
}

SheetTab::~SheetTab()
{
//	if(archPlgObj)
//	{	archive::plgns[archPlgNum].Close$4(archPlgObj);
//		archive::plgns[archPlgNum].bOpen=FALSE;
//	}
	Destroy();
}

VOID SheetTab::Destroy()
{
 __try
 {
	if(pSelects)
		HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,pSelects);
	if(pSelectSts)
		HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,pSelectSts);
	pSelects=NULL;
	pSelectSts=NULL;
 }

 __except(EXCEPTION_EXECUTE_HANDLER)
 {	MessageBox(NULL,L"SheetTab::Destroy:",L"Exception:",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
 }

}

BOOL SheetTab::Load(HWND htab,int id,Panel* p)
{
TCITEM item;item.mask=TCIF_PARAM;

	if(!TabCtrl_GetItem(htab,id,&item))return FALSE;
	SheetTab* sheet = (SheetTab*)item.lParam;
	if(archElem==sheet->entrType)
		return sheet->LoadInArch(p);

	p->FreeMem();
	p->GetEntry()->Reset();
	if(directFolder==sheet->entrType)
	{	p->SetPath(sheet->path,MAX_PATH);
		p->GetEntry()->SetPushEntryInStartup(sheet->path);
		p->fManager.FillDirectFolder();
		p->GetEntry()->Push(sheet->path,directFolder,0);
		MySetCurrentDirectory(sheet->path);
	}
	else if(guidFolder==sheet->entrType)
	{	p->SetPath(sheet->path,MAX_PATH,FALSE);
		p->GetEntry()->SetPushGuidEntryInStartup(sheet->path);
		p->fManager.FillGuidFolder();
		p->GetEntry()->Push(sheet->path,guidFolder,0);
	}
	//else if(virtualPanel==sheet->entrType)//sheetTab mumkin emas;
	//{	//Agar pathi b-sa directDir qo'yamiz:
	//}
	else if(rndPathList==sheet->entrType)
	{	
	}
	else if(socketCl==sheet->entrType)
	{	
	}
	
	p->totSelects = sheet->totSelects;
	p->totSelectsFiles = sheet->totSelectsFiles;
	p->totSelectsFolders = sheet->totSelectsFolders;
	p->viewMenuBitsU32 = sheet->viewMenuBitsU32;

	for(int i=0; i<p->GetTotItems(); i++)
		p->GetItem(i)->state = normal;
	for(int i=0; i<sheet->totSelects; i++)
		p->GetItem(sheet->pSelects[i])->state = sheet->pSelectSts[i];

	p->AdjustScrollity();
	p->ScrollItemToView(sheet->iHot);
	p->ClrScr();
	p->Render(NULL);
	p->SetFocus();
	p->ShowSelectedFilesInfo();
	return TRUE;
}

BOOL SheetTab::LoadInArch(Panel* p)
{
	p->FreeMem();
	p->GetEntry()->Reset();
	p->SetPath(path,MAX_PATH);
	p->GetEntry()->SetPushEntryInStartup(path);

BEGIN_TRY
	if(!(0==wcscmp(p->arch.name,archFilePath) && p->arch.plgObj))
	{	p->arch.plgObj = archive::plgns[archPlgNum].OpenForUnpacking$8(archFilePath,&p->arch);
		p->arch.plgNum = archPlgNum;
		p->arch.nameLn=MyStringCpy(p->arch.name,MAX_PATH-1,archFilePath);
	}
END_TRY
{	
}  
	p->GetEntry()->Reset();
	p->GetEntry()->SetPushArchEntryInStartup(archFilePath,archPath);
	p->fManager.FillArchItems(archPath);

	MySetCurrentDirectory(path);

	p->totSelects = totSelects;
	p->totSelectsFiles = totSelectsFiles;
	p->totSelectsFolders = totSelectsFolders;
	p->viewMenuBitsU32 = viewMenuBitsU32;

	for(int i=0; i<p->GetTotItems(); i++)
		p->GetItem(i)->state = normal;
	for(int i=0; i<totSelects; i++)
		p->GetItem(pSelects[i])->state = pSelectSts[i];

	p->AdjustScrollity();
	p->ScrollItemToView(iHot);
	p->ClrScr();
	p->Render(NULL);
	p->SetFocus();
	p->ShowSelectedFilesInfo();
	return TRUE;
}

BOOL SheetTab::Save(HWND htab,int id,Panel* p)
{
 TCITEM item;item.mask=TCIF_PARAM;
	if(!TabCtrl_GetItem(htab,id,&item))return FALSE;
	SheetTab* sheet = (SheetTab*)item.lParam;

	sheet->pathLn = MyStringCpy(sheet->path,MAX_PATH-1,p->GetPath());
	sheet->totSelects = p->GetTotSelects();
	sheet->totSelectsFiles = p->GetTotSelectsFiles();
	sheet->totSelectsFolders = p->GetTotSelectsFolders();
	if(sheet->totSelects)
	{	if(!sheet->pSelects)
		{	sheet->pSelects = (int*)HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,
							sheet->totSelects*sizeof(__int32));
			sheet->pSelectSts = (int*)HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,
							sheet->totSelects*sizeof(__int32));
		}
		else
		{  sheet->pSelects = (__int32*)HeapReAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,
							sheet->pSelects,sizeof(__int32)*sheet->totSelects);
		   sheet->pSelectSts = (__int32*)HeapReAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,
							sheet->pSelectSts,sizeof(__int32)*sheet->totSelects);
		}
		for(int i=0; i<sheet->totSelects; i++)
		{	sheet->pSelects[i] = p->GetSelectedItemNum(i);
			sheet->pSelectSts[i] = p->GetItem(sheet->pSelects[i])->state;
	}	}

	sheet->iHot = p->iHot;
	sheet->entrType = p->GetEntry()->GetCrntRecType();
	if(archElem==sheet->entrType)
	{	sheet->archPlgNum = p->arch.plgNum;
		MyStringCpy(sheet->archFilePath,MAX_PATH-1,p->arch.name);
		MyStringCpy(sheet->archPath,MAX_PATH-1,p->GetEntry()->GetCrntRecArchPath());
	}
	else if(virtualPanel==sheet->entrType)
	{	sheet->archPlgNum = p->iVPPlg;
		//vrtlPanels::plgns[p->iVPPlg].GetItemPathAndName$16(vrtlPanels::plgns[iVPPlg].plgObj,
		//													 sheet->archFilePath,MAX_PATH,
		//													 p->GetItem(1)->itemId);
		//wchar_t *p=wcsrchr(sheet->archFilePath,'\\');
		//if(p) *(p+1)=0;
	}
	sheet->viewMenuBitsU32 = p->viewMenuBitsU32;
	return TRUE;
}

BOOL SheetTab::SetPath(wchar_t* pthW)
{
	int l;
	pathLn = 0;
	if(pthW && pthW[0])
		pathLn = MyStringCpy(path,MAX_PATH-1,pthW);
	if(pthW && pthW[0])
	{	l=MyStringCpy(path,MAX_PATH-1,pthW);
		if(!pathLn)
			pathLn=l;
	}
	return TRUE;
}

BOOL SheetTab::SetAltPath(wchar_t* pth)
{
int altPathLn = 0;
	if(pth && pth[0])
	{	altPathLn = MyStringCpy(altPath,MAX_PATH-1,pth);
		return TRUE;
	}
	return FALSE;
}