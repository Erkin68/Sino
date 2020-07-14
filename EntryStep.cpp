#include "EntryStep.h"
#include "strsafe.h"
#include "registry.h"
#include "sino.h"
#include "WindowsManagmentInstrumentation.h"
#include "Operations\MyErrors.h"
#include "Operations\MyShell\MyShell.h"



EntryStep::EntryStep() : maxItems(32), numItems(0)
{
	recs = (Record*)malloc(32*sizeof(Record));
}

EntryStep::~EntryStep()
{
	free(recs);// recs = NULL;
}

VOID EntryStep::ChangeCrntRecPath(wchar_t* p,int l)
{
	if(numItems>0)
		MyStringCpy(recs[numItems-1].path,l,p);
}

VOID EntryStep::Clear()
{
	CheckTemporaryes();
	if(maxItems>32)
		recs = (Record*)realloc(recs,sizeof(Record)*32);
	maxItems = 32;
	numItems = 0;
}

VOID EntryStep::CopyFrom(EntryStep *e)
{
	recs = (Record*)realloc(recs,sizeof(Record)*(e->maxItems));
	memmove(recs,e->recs,e->numItems*sizeof(Record));
	maxItems = e->maxItems;
	numItems = e->numItems;
}

VOID EntryStep::CreateTemporaryDir(Panel* p,int id)
{
	recs[numItems-1].tmpDir = (CTempDir*)malloc(sizeof(CTempDir));
	recs[numItems-1].tmpDir->type=CTempDir::arjStack;
	recs[numItems-1].tmpDir->FindRandomDir();
	recs[numItems-1].tmpDir->SetArchFilePath(p->GetArcFilePathAndName());
	recs[numItems-1].tmpDir->SetArchPath(p->GetArcItPathAndName(id));
	recs[numItems-1].tmpDir->SetOldItem(id);
}

EntryType EntryStep::GetCrnt(wchar_t *bfW, EntryType oldType)
{
	if(numItems>0)
	{	MyStringCpy(bfW,MAX_PATH-1,recs[numItems-1].path);
		if(guidFolder==recs[numItems-1].type)
			MyStringCpy(&bfW[GUID_CHAR_POS],GUID_CHAR_LEN,&recs[numItems-1].path[GUID_CHAR_POS]);
		return recs[numItems-1].type;
	}//else, or to my comp, or to desktop;
	if(directFolder==oldType)//this means to MyComp;
	{	wchar_t* myComp = myWMI::GetMyCompClsid();
		if(myComp)
		{	Push(myComp,guidFolder,0,L"My Computer");
			MyStringCpy(bfW,MAX_PATH/2,myComp);
			MyStringCpy(&bfW[GUID_CHAR_POS],GUID_CHAR_LEN,L"My Computer");
			return guidFolder;
	}	}//else this means to desktop;
	else
	{	if(myWMI::CmpCrntClsidWithMyCompClsid(recs[numItems].path))
		{//this means up to desktop:
			Push(NULL,guidFolder,0,L"Desktop");
			MyStringCpy(bfW,MAX_PATH/2,L"");
			MyStringCpy(&bfW[GUID_CHAR_POS],GUID_CHAR_LEN,L"Desktop");
			return guidFolder;
	}	}
	//Push((char*)clsidDesktop,guidFolder,0);
	return unknown;
}

wchar_t* EntryStep::GetCrntRecArchPath()
{
	if(numItems>0)
		return recs[numItems-1].archPath;
	return NULL;
}

char* EntryStep::GetCrntRecPathA()
{
static char s[MAX_PATH]={0};
	if(numItems<1)return NULL;
	if(WideCharToMultiByte(AreFileApisANSI()?CP_ACP:CP_OEMCP,WC_COMPOSITECHECK,GetCrntRecPath(),-1,s,MAX_PATH-1,NULL,NULL))
		return s;
	return NULL;
}

wchar_t* EntryStep::GetCrntRecPath()
{
	if(numItems>0)
		return recs[numItems-1].path;
	return NULL;
}

EntryType EntryStep::GetCrntRecType()
{
	if(numItems>0)
		return recs[numItems-1].type;
	return unknown;
}

int EntryStep::GetCrntRecChild()
{
	if(numItems>0)
		return recs[numItems-1].childEntr;
	return 0;
}

wchar_t* EntryStep::GetCrntTemporaryDir()
{
	if(numItems>0)
		return recs[numItems-1].tmpDir->GetPath();
	return 0;
}

CTempDir* EntryStep::GetCrntTemporary()
{
	if(numItems>0)
		return recs[numItems-1].tmpDir;
	return 0;
}

VOID EntryStep::ClearCrntTemporary()
{
	if(numItems>0)
		recs[numItems-1].tmpDir=NULL;
}

LPVOID EntryStep::GetCrntRecVPPlgObj()
{
	if(numItems>0)
		return recs[numItems-1].VPPlgObj;
	return NULL;
}

int EntryStep::GetCrntRecIVPPlg()
{
	if(numItems>0)
		return recs[numItems-1].iVPPlg;
	return -1;
}

LPVOID EntryStep::GetCrntRecArjPlgObj()
{
	if(numItems>0)
		return recs[numItems-1].arjPlgObj;
	return NULL;
}

int EntryStep::GetCrntRecArjPlgNum()
{
	if(numItems>0)
		return recs[numItems-1].iArjPlg;
	return NULL;
}

/*HWND EntryStep::GetCrntRecRandomListBox()
{
	if(numItems>0)
	{	HWND *wnd = (HWND*)&recs[numItems-1].path[0];
		return *wnd;
	}
	return NULL;
}*/

VOID EntryStep::PushA(char* p, EntryType t, int chld)
{
wchar_t ws[MAX_PATH];
	MultiByteToWideChar(AreFileApisANSI()?CP_ACP:CP_OEMCP,MB_PRECOMPOSED,p,MAX_PATH-1,ws,MAX_PATH);
	Push(ws,t,chld,0);
}

VOID EntryStep::Push(wchar_t* path, EntryType t, int chld, wchar_t* guidName)
{
	if(numItems>maxItems-2)
	{	recs = (Record*)realloc(recs,sizeof(Record)*(maxItems+32));
		maxItems += 32;
	}

	//if(numItems<maxItems-2)
	{	if(numItems>0)
			recs[numItems-1].childEntr = chld;
		else
			recs[numItems].childEntr = chld;
		recs[numItems].type = t;
		recs[numItems].tmpDir = 0;
		recs[numItems].arjPlgObj = 0;
		switch(t)
		{	case directFolder:
				MyStringCpy(&recs[numItems].path[0],MAX_PATH-1,path);
				break;
			case guidFolder:
				if(path)
					MyStringCpy(recs[numItems].path,MAX_PATH-1,(wchar_t*)path);
				else
					recs[numItems].path[0] = 0;
				if(guidName)
					MyStringCpy(&recs[numItems].path[GUID_CHAR_POS],GUID_CHAR_LEN,guidName);
				break;
			case rndPathList:
				memcpy(&recs[numItems].path[0],&path,sizeof(HWND));
				recs[numItems].path[0]=0;
				break;
			case archElem:
//			case virtualPanel:
				if(archElem!=recs[numItems-1].type)// || virtualPanel!=recs[numItems-1].type)
					 break;
				MyStringCpy(&recs[numItems].path[0],MAX_PATH-1,&recs[numItems-1].path[0]);
				MyStringCpy(&recs[numItems].archPath[0],MAX_PATH-1,&recs[numItems-1].archPath[0]);
				MyStringCat(&recs[numItems].archPath[0],MAX_PATH-1,path);
				MyStringCat(&recs[numItems].archPath[0],MAX_PATH-1,L"\\");
				recs[numItems].arjPlgObj = recs[numItems-1].arjPlgObj;
				recs[numItems].VPPlgObj = recs[numItems-1].VPPlgObj;
				recs[numItems].iArjPlg = recs[numItems-1].iArjPlg;
				recs[numItems].iVPPlg = recs[numItems-1].iVPPlg;
				break;
			case socketCl:
				MyStringCpy(&recs[numItems].path[0],MAX_PATH-1,path);
				recs[numItems].path[0]=0;
				break;
		}
		++numItems;
}	}

BOOL EntryStep::PushToArch(wchar_t* archFilePathAndName, int chld, LPVOID plgObj, int iPlg)
{
	if(numItems>maxItems-2)
	{	recs = (Record*)realloc(recs,sizeof(Record)*(maxItems+32));
		maxItems += 32;
	}

	if(numItems>0)
		recs[numItems-1].childEntr = chld;
	else
		recs[numItems].childEntr = chld;
	recs[numItems].type = archElem;
	recs[numItems].tmpDir = 0;
	recs[numItems].archPath[0] = 0;//""
	recs[numItems].arjPlgObj = plgObj;
	recs[numItems].iArjPlg = iPlg;
	MyStringCpy(&recs[numItems].path[0],MAX_PATH-1,archFilePathAndName);
	++numItems;
	return TRUE;
}	

BOOL EntryStep::PushToVirtualPanel(wchar_t* archFilePathAndName, int chld, LPVOID VPPlgObj, int iVPPlg)
{
	if(numItems>maxItems-2)
	{	recs = (Record*)realloc(recs,sizeof(Record)*(maxItems+32));
		maxItems += 32;
	}

	if(numItems>0)
		recs[numItems-1].childEntr = chld;
	else
		recs[numItems].childEntr = chld;
	recs[numItems].type = virtualPanel;
	recs[numItems].tmpDir = 0;
	recs[numItems].archPath[0] = 0;//""
	recs[numItems].VPPlgObj = VPPlgObj;
	recs[numItems].iVPPlg = iVPPlg;
	MyStringCpy(&recs[numItems].path[0],MAX_PATH-1,archFilePathAndName);
	++numItems;
	return TRUE;
}	

VOID EntryStep::PushFindType(wchar_t* name, int chld, int nameLn)
{
//char st[MAX_PATH];
WIN32_FIND_DATAW ff;
HANDLE			hf = INVALID_HANDLE_VALUE;

	/*if(!registry.IsCLSIDExists((wchar_t*)name))
	{	EntryType t = GetCrnt(st);
		if(directFolder==t)
		{	*(strrchr(st, '\\')) = 0;//* ni olib otaramiz:
			StringCbCatN(st,MAX_PATH-1,"\\", 2*sizeof(wchar_t));
			StringCbCatN(st,MAX_PATH-1,name,sizeof(wchar_t)*nameLn);
			StringCbCatN(st,MAX_PATH-1,"\\*", 2*sizeof(wchar_t));
			hf = FindFirstFile(st,&ff);
			if(INVALID_HANDLE_VALUE!=hf)
			{	FindClose(hf);
				if((FILE_ATTRIBUTE_DIRECTORY & ff.dwFileAttributes) ||
				   (FILE_ATTRIBUTE_SYSTEM    & ff.dwFileAttributes))
				{	Push(st, directFolder, chld);
					return;
		}	}	}
		else
		{	StringCchPrintf(st,MAX_PATH-1,name);
			if(myWMI::CmpCrntClsidWithMyCompClsid(recs[numItems-1].path))
			{	Pop();//0 ta qilib qo'yamiz;
				MyStringCat(st,MAX_PATH-1,"*");
			}
			Push(st, directFolder, chld);
	}	}
	else//parent is clsid:*/
	{	hf = FindFirstFile(name,&ff);
		if(INVALID_HANDLE_VALUE!=hf)
		{	FindClose(hf);
			Push(name, directFolder, chld, 0);
		}
		else if(registry::IsCLSIDExists(name))
		{
			Push(name, guidFolder, chld, 0);
}	}	}

int EntryStep::PushToFolder(wchar_t* nameW, int chld, int nameLn)
{
wchar_t ws[MAX_PATH]={0};
int ln;
	switch(recs[numItems-1].type)
	{	case directFolder:
			ln = MyStringCpy(ws,MAX_PATH-1,recs[numItems-1].path);
			if(ln>0)
			{	if('*'==ws[ln-1] && '\\'==ws[ln-2])
					ws[--ln] = 0;
				else if('\\'!=ws[ln-1])
					ws[ln++]='\\';
				ln += MyStringCpy(&ws[ln],nameLn,nameW);
				if('\\'==ws[ln-1])
					{ws[ln++] = '*'; ws[ln] = 0;}
				else if('\\'!=ws[ln-1])
					{ws[ln++]='\\'; ws[ln++]='*';ws[ln]=0;}
			}
			break;
		case guidFolder:
			ln = MyStringCpy(ws,MAX_PATH-1,nameW);
			if('*'!=ws[ln-1] && '\\'!=ws[ln-2])
			{	if('\\'==ws[ln-1])
					{ws[ln++]='*';ws[ln]=0;}
				else
					{ws[ln++]='\\';ws[ln++]='*';ws[ln]=0;}
			}
			//if(IsDirExist(ws))
			//	StringCbCatN(ws,MAX_PATH-1,L"\\*", 2*sizeof(wchar_t));
			//else
			//	StringCbCatN(ws,MAX_PATH-1,L"*", sizeof(wchar_t));
			break;
		case rndPathList:
			ln = MyStringCpy(ws,MAX_PATH-1,nameW);
			if('*'!=ws[ln-1] && '\\'!=ws[ln-2])
			{	if('\\'==ws[ln-1])
					{ws[ln++]='*';ws[ln]=0;}
				else
					{ws[ln++]='\\';ws[ln++]='*';ws[ln]=0;}
			}
			break;
	}
	BOOL bAccessDenied=FALSE;
TryAccessCorrect:
	if(IsDirExist(ws))
	{	Push(ws, directFolder, chld);
		return bAccessDenied?1:2;
	}
	else if(registry::IsCLSIDExists(ws))
	{
		Push(nameW, guidFolder, chld);
	}
	else
	{	DWORD r =GetLastError();
		if(ERROR_ACCESS_DENIED==r)
		{	if(!bAccessDenied)
			{	ChangePrivilege(0,17,TRUE);//SE_BACKUP_PRIVILEGE,Set
				bAccessDenied=TRUE;
				goto TryAccessCorrect;
			}
			return 0;
		}
		else
		{	Err::msg1(hWnd,r,L"FindFirstFile",nameW);
			return 0;
	}	}
	return 0;
}

EntryType EntryStep::Pop()//return preposition type;
{
	EntryType t = directFolder;
	if(numItems>0)//rndPathList larni chiqarib tashlaymiz;
	{	--numItems;
		t = recs[numItems].type;
		while(rndPathList==recs[numItems-1].type && numItems>0)
		{	--numItems;
			t = recs[numItems].type;
	}	}
	if(maxItems>32)
	if(numItems<maxItems-32)
	{	recs = (Record*)realloc(recs,sizeof(Record)*(maxItems-32));
		maxItems -= 32;
	}
	return t;
}

VOID EntryStep::Reset()
{
	CheckTemporaryes();
	maxItems = 32;
	numItems = 0;
	recs = (Record*)malloc(32*sizeof(Record));
}

VOID EntryStep::SetPushArchEntryInStartup(wchar_t* archFilePath,wchar_t *archPath)
{
	SetPushEntryInStartup(archFilePath);
	Push(L"",archElem,0);//root dir:
	MyStringCpy(&recs[numItems-1].path[0],MAX_PATH-1,archFilePath);
	recs[numItems-1].archPath[0]=0;
	
wchar_t s[MAX_PATH];
	int l=MyStringLength(archPath,MAX_PATH);
	for(int i=0; i<l; i++)
	{	if(archPath[i]=='\\')
		{	MyStringCpy(s,i+1,archPath);
			s[i]='\\';
			s[i+1]=0;
			Push(s,archElem,0);
			MyStringCpy(&recs[numItems-1].path[0],MAX_PATH-1,archFilePath);
			MyStringCpy(&recs[numItems-1].archPath[0],MAX_PATH-1,s);
}	}	}

//Agar conf::panel_path larda o'rnatilgan bo'lsa chiqsin uchun entry sini to'g'rilash:
VOID EntryStep::SetPushEntryInStartup(wchar_t* pthW)
{
wchar_t ws[MAX_PATH];int l;
	if(pthW && pthW[0])
	{	l=MyStringLength(pthW,MAX_PATH);
		for(int i=0; i<l-2; i++)
		{	if(pthW[i]=='\\')
			{	MyStringCpy(ws,i+1,pthW);
				ws[i]='\\';
				ws[i+1]='*';
				ws[i+2]=0;
				if(IsDirExist(ws))
					Push(ws,directFolder,0);
}	}	}	}

VOID EntryStep::SetPushGuidEntryInStartup(wchar_t* guidPath)
{
	if(0==guidPath[0])
		return;//we are already in desktop folder;
	else if(!wcscmp(guidPath,L"::{20D04FE0-3AEA-1069-A2D8-08002B30309D}"))
		Push(NULL,guidFolder,0,L"My Computer");
	else if(!wcscmp(guidPath,L"::{26EE0668-A00A-44D7-9371-BEB064C98683}"))
		Push(NULL,guidFolder,0,L"Desktop");
	else if(!wcscmp(guidPath,L"::{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}"))
		Push(NULL,guidFolder,0,L"Network");
	else if(!wcscmp(guidPath,L"::{645FF040-5081-101B-9F08-00AA002F954E}"))
		Push(NULL,guidFolder,0,L"Recycle");
	else if(!wcscmp(guidPath,L"::{2227A280-3AEA-1069-A2DE-08002B30309D}"))
	{	Push(L"::{26EE0668-A00A-44D7-9371-BEB064C98683}",guidFolder,0,L"Control Panel");//control panel;
		Push(L"",guidFolder,0,L"Desktop");//Desktop folder;
	}
	//My Network Places  {208D2C60-3AEA-1069-A2D7-08002B30309D} 
	//Internet Explorer {871C5380-42A0-1069-A2EA-08002B30309D} 
	//Connect to a network wizard "::{20d04fe0-3aea-1069-a2d8-08002b30309d}\\::{21ec2020-3aea-1069-a2dd-08002b30309d}\\::{38A98528-6CBF-4CA9-8DC0-B1E1D10F7B1B}",
	//Manage Wireless Networks item ::{21EC2020-3AEA-1069-A2DD-08002B30309D}\\3\\::{1fa9085f-25a2-489b-85d4-86326eedcd87}
	//Connections.{241D7C96-F8BF-4F85-B01F-E2B043341A4B}
/*Настройки месторасположения
Myplace.{00C6D95F-329C-409a-81D7-C46C66EA7F33}

Биометрические устройства
Biometric.{0142e4d0-fb7a-11dc-ba4a-000ffe7ab428}

Управление электропитанием
Power.{025A5937-A6BE-4686-A844-36FE4BEC8B6D}

Управление значками в области уведомлений
SysTrayIcon.{05d7b0f4-2121-4eff-bf6b-ed3f69b894d9}

Хранилище Windows
Backup.{1206F5F1-0569-412C-8FEC-3204630DFB70}

Установка программ из сети
NetInstall.{15eae92e-f17a-4431-9f28-805e482dafd4}

Выбор программ по умолчанию
AppDef.{17cd9488-1228-4b2f-88ce-4298e93e0966}

Управление сертификатами
Sert.{1D2680C9-0E2A-469d-B787-065558BC7D43}

Управление беспроводными устройствами
Radio.{1FA9085F-25A2-489B-85D4-86326EEDCD87}

Сеть (Рабочая группа)
Workgroup.{208D2C60-3AEA-1069-A2D7-08002B30309D}

Мой компьютер
myComp.{20D04FE0-3AEA-1069-A2D8-08002B30309D}

Принтеры и факсы
Printers.{2227A280-3AEA-1069-A2DE-08002B30309D}

Подключение к компьютерам и программам
Connect.{241D7C96-F8BF-4F85-B01F-E2B043341A4B}

Управление брандмауэром Windows
Brandmauer.{4026492F-2F69-46B8-B9BF-5654FC07E423}

Оценка производительности компьютера
{78F3955E-3B90-4184-BD14-5397C15F1EFC}*/
}

VOID EntryStep::CheckTemporaryes()
{
	for(int e=0; e<numItems; e++)
	{	if(recs[e].tmpDir)
		{	if(!recs[e].tmpDir->TryRestore())
				Err::msg1(hWnd,0,L"Archive was unclosed...",recs[e].tmpDir->GetItem(0));
}	}	}
