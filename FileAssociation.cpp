#include "FileAssociation.h"
#include "registry.h"
#include "strsafe.h"
#include "config.h"
#include "Sino.h"
#include "Operations\MyShell\Myshell.h"
//#include <gdiplus.h>

//using namespace Gdiplus;


#define ASS_HEAP_INC_ITEM_NUMBER 512


extern HINSTANCE hInst;
extern HWND		 hWnd;

//static LPTSTR IconEnumName;
//static char IconNameChar[128];
static HICON EnumFindIcon;
BOOL CALLBACK IconGrEnumProc(HMODULE hm,LPCTSTR lpszType,LPTSTR lpszName,LONG_PTR lParam)
{
	HRSRC hResource = FindResource(hm, lpszName, RT_GROUP_ICON);
	if(!hResource) return TRUE;
	HGLOBAL hMem = LoadResource(hm, hResource);
	if(!hMem) return TRUE;
	LPVOID lpResource = LockResource(hMem); 
	if(!lpResource) return TRUE;
	int nID = LookupIconIdFromDirectoryEx((PBYTE)lpResource,TRUE,conf::Dlg.iIconSize[0],conf::Dlg.iIconSize[0],LR_DEFAULTCOLOR);
	if(!nID) return TRUE;
	hResource = FindResource(hm, MAKEINTRESOURCE(nID), MAKEINTRESOURCE(RT_ICON));
	if(!hResource) return TRUE;
	hMem = LoadResource(hm, hResource);
	if(!hMem) return TRUE;
	lpResource = LockResource(hMem);
	if(!lpResource) return TRUE;
	EnumFindIcon = CreateIconFromResourceEx((PBYTE)lpResource, SizeofResource(hm,hResource),
			TRUE, 0x00030000, conf::Dlg.iIconSize[0], conf::Dlg.iIconSize[0], LR_DEFAULTCOLOR);
	//if(IS_INTRESOURCE(lpszName)) IconEnumName = lpszName;
	//else
	//{	MyStringCpy(IconNameChar,128,lpszName);
	//	IconEnumName = &IconNameChar[0];
	//}
	if(!EnumFindIcon) return TRUE;
	return FALSE;
}
BOOL CALLBACK IconEnumProc(HMODULE hm,LPCTSTR lpszType,LPTSTR lpszName,LONG_PTR lParam)
{
	HRSRC hResource = FindResource(hm, lpszName, RT_ICON);
	if(!hResource) return TRUE;
	HGLOBAL hMem = LoadResource(hm, hResource);
	if(!hMem) return TRUE;
	LPVOID lpResource = LockResource(hMem); 
	if(!lpResource) return TRUE;
	EnumFindIcon= CreateIconFromResourceEx((PBYTE)lpResource, SizeofResource(hm,hResource),
			TRUE, 0x00030000, 0, 0, LR_DEFAULTCOLOR);
	return FALSE;
}
HICON MyLoadIcon(wchar_t* path, int num)
{	HGLOBAL hMem; LPVOID lpResource; HICON h = NULL;
	HMODULE hm = LoadLibraryEx(path,NULL,LOAD_LIBRARY_AS_DATAFILE);//LOAD_LIBRARY_AS_IMAGE_RESOURCE); exception beradi;
	if(!hm) return NULL;
	HRSRC hResource = FindResource(hm, MAKEINTRESOURCE(num),RT_GROUP_ICON);
	if(!hResource) goto End;
	hMem = LoadResource(hm, hResource);
	if(!hMem) goto End;
	lpResource = LockResource(hMem); 
	if(!lpResource) goto End;
	int nID = LookupIconIdFromDirectoryEx((PBYTE)lpResource,TRUE,
					conf::Dlg.iIconSize[0],
					conf::Dlg.iIconSize[0],
					LR_DEFAULTCOLOR);
	if(!nID)
		goto End;
/*	int sz=48;
	int nID = LookupIconIdFromDirectoryEx((PBYTE)lpResource,TRUE,
					sz,//conf::Dlg.iIconSize[0],
					sz,//conf::Dlg.iIconSize[0],
					LR_DEFAULTCOLOR);
	if(!nID)
	{	sz=32;
		nID = LookupIconIdFromDirectoryEx((PBYTE)lpResource,TRUE,
					sz,//conf::Dlg.iIconSize[0],
					sz,//conf::Dlg.iIconSize[0],
					LR_DEFAULTCOLOR);
		if(!nID)
		{	sz=16;
			nID = LookupIconIdFromDirectoryEx((PBYTE)lpResource,TRUE,
						sz,//conf::Dlg.iIconSize[0],
						sz,//conf::Dlg.iIconSize[0],
						LR_DEFAULTCOLOR);
			if(!nID)
				goto End;
	}	}*/
	hResource = FindResource(hm, MAKEINTRESOURCE(nID), MAKEINTRESOURCE(RT_ICON));
	if(!hResource) goto End;
	hMem = LoadResource(hm, hResource);
	if(!hMem) goto End;
	lpResource = LockResource(hMem);
	if(!lpResource) goto End;
	h = CreateIconFromResourceEx((PBYTE)lpResource, SizeofResource(hm,hResource),
		TRUE, 0x00030000,
		conf::Dlg.iIconSize[0],//sz
		conf::Dlg.iIconSize[0],//sz
		LR_DEFAULTCOLOR);
End:
	FreeLibrary(hm);
	//if(!h) h = ExtractIcon(hInst,path,num);
	return h;
}
static HICON MyLoadMasterIcon(wchar_t* path)
{	//HMODULE hm = LoadLibraryEx(path,NULL,LOAD_LIBRARY_AS_DATAFILE);//LOAD_LIBRARY_AS_IMAGE_RESOURCE); exception beradi;
	//EnumFindIcon = NULL;
	//if(!hm) goto End;
	//EnumResourceNames(hm,RT_GROUP_ICON,IconGrEnumProc,conf::iIconSize);
	//if(!EnumFindIcon) EnumResourceNames(hm,RT_ICON,IconEnumProc,conf::iIconSize);
	//FreeLibrary(hm);
//End:
	//if(!EnumFindIcon) EnumFindIcon = ExtractIcon(hInst,path,0);

	//__int64 d[5];
	//QueryPerformanceCounter((LARGE_INTEGER*)&d[0]);
	//UINT ni = 
		ExtractIconEx(path,0,&EnumFindIcon,NULL,1);//large icon, onli small icon
	//QueryPerformanceCounter((LARGE_INTEGER*)&d[1]);

	//SHFILEINFO si;
	//SHGetFileInfo(path,0,&si,sizeof(SHFILEINFO),SHGFI_ICON|SHGFI_SYSICONINDEX|SHGFI_OPENICON);
	//QueryPerformanceCounter((LARGE_INTEGER*)&d[2]);

	//d[3] = d[1]-d[0];
	//d[4] = d[2]-d[1];


	return EnumFindIcon;
	//HIMAGELIST himlIcons = si.hIcon;
	//return ImageList_GetIcon((HIMAGELIST)si.hIcon,si.iIcon,ILD_NORMAL);
}

VOID FileAssociation::CreateBMP(HICON hi, HBITMAP *bm, HDC *DC,int *width,int *height)
{
//GpBitmap *gdiBMP;
//	GpStatus st = GdipCreateBitmapFromHICON(hi,&gdiBMP);


	*bm = NULL; *DC = NULL;
	ICONINFO ii;
	//if(*width==0 || *height==0)
	{	if(!GetIconInfo(hi,&ii)) return;
		if(ii.xHotspot>0) (*width)  = ii.xHotspot*2; else (*width)  = 32;
		if(ii.yHotspot>0) (*height) = ii.yHotspot*2; else (*height) = 32;
	}
	HDC dc = GetDC(hWnd);
	*DC = CreateCompatibleDC(dc);
	*bm = CreateCompatibleBitmap(dc,(*width),(*height));
    SelectObject(*DC, *bm);
	DrawIcon(*DC,0,0,hi);

	//SetStretchBltMode(*DC,COLORONCOLOR);
//	if(48==width && 48==height)
//		StretchBlt(*DC,0,0,20,20,memDC,0,0,width-16,height-16,SRCCOPY);
//	else if(16==width && 16==height)
//		BitBlt(*DC,0,0,(*width),(*height),memDC,0,0,SRCCOPY);
//	else
//		StretchBlt(*DC,0,0,20,20,memDC,0,0,width,height,SRCCOPY);
/*
	int bPt[64][64];
	for(int w=0; w<(*width); w++)for(int h=0; h<(*height); h++)bPt[w][h] = 0;

	for(int w=0; w<(*width); w++)
	{	for(int h=0; h<(*height); h++)
		{	COLORREF c = GetPixel(*DC,w,h);
			if(0!=c)
			{	bPt[w][h] = 2;//Icon crnt color;
				if(w>0)
				{	COLORREF cl = GetPixel(*DC,w-1,h);
					if(0==cl)
					if(2!=bPt[w-1][h])
						bPt[w-1][h] = 1;
				}
				if(w<(*width)-1)
				{	COLORREF cl = GetPixel(*DC,w+1,h);
					if(0==cl)
					if(2!=bPt[w+1][h])
						bPt[w+1][h] = 1;
				}
				if(h>0)
				{	COLORREF cl = GetPixel(*DC,w,h-1);
					if(0==cl)
					if(2!=bPt[w][h-1])
						bPt[w][h-1] = 1;
				}
				if(h<(*height)-1)
				{	COLORREF cl = GetPixel(*DC,w,h+1);
					if(0==cl)
					if(2!=bPt[w][h+1])
						bPt[w][h+1] = 1;
	}	}	}	}

	for(int w=0; w<(*width); w++)
	{	for(int h=0; h<(*height); h++)
		{	if(1==bPt[w][h])
				SetPixel(*DC,w,h,RGB(0,0,180));
			else if(0==bPt[w][h])
				SetPixel(*DC,w,h,RGB(255,255,255));
	}	}
*/
	ReleaseDC(hWnd,dc);
//	DeleteObject(memBM);
	return;
}

FileAssociation::FileAssociation() :
	pBuf(NULL),
	tot(0),
	totMax(0)
{
}

FileAssociation::~FileAssociation()
{
	FreeMem();
}

/*int FileAssociation::Add(wchar_t *EXT)
{
char ext[32];
	int r = Add(ext);
	for(int i=0; i<=r; i++)
		EXT[i]=ext[i];
	return r;
}*/

int FileAssociation::Add(wchar_t *EXT)//,wchar_t *Name) fullpathand nameb-n SHGetFileInfo ni ishlatib
{
//unsigned __int64 d[4];
	//QueryPerformanceCounter((LARGE_INTEGER*)&d[0]);


	//if(strstr(ext,"ax"))
	//	int i=0;
	wchar_t* ext = ConvertToLittleR(EXT,0);
int r = IsRecExist(ext);
	if(r>-1)
	{	int oi = IsOwnIcoOrCurExt(ext);
		if(oi<0) return oi;
		return r;
	}
	if(!MemCheck()) return -100;

	wchar_t** fAss = registry::GetFileOpenWithAssociation(ext);
	if(!fAss) return -100;

	pBuf[tot].extLn = MyStringCpy(pBuf[tot].ext,MAX_EXT-1,ext);
	
	if(fAss[0] && '\0'!=fAss[0])
		pBuf[tot].descLn = MyStringCpy(pBuf[tot].desc,MAX_EXT-1,fAss[0]);
	else
	{	pBuf[tot].desc[0] = '\0';
		pBuf[tot].descLn = 0;
	}
	
	if(fAss[1] && '\0'!=fAss[1])
	{	//ExpandEnvironmentStrings(fAss[1],pBuf[tot].iconPath,MAX_PATH-1);
		//pBuf[tot].iconPathLn = MyStringCpy(pBuf[tot].iconPath,MAX_PATH-1,pBuf[tot].iconPath);//fAss[1]);
		//wchar_t* pComma = wcsrchr(pBuf[tot].iconPath,',');
		wchar_t* pComma = wcsrchr(fAss[1],',');
		if(pComma)
		{	pBuf[tot].iconNum = _wtoi(pComma+1);
			if(pBuf[tot].iconNum < 0) pBuf[tot].iconNum = - pBuf[tot].iconNum;
			//*pComma = '\0';
			//pBuf[tot].iconPathLn = MyStringCpy(pBuf[tot].iconPath,MAX_PATH-1,pBuf[tot].iconPath);
			pBuf[tot].iconPathLn = MyStringCpy(pBuf[tot].iconPath,(int)(pComma-fAss[1]),fAss[1]);
			pBuf[tot].icon = MyLoadIcon(pBuf[tot].iconPath,pBuf[tot].iconNum);
			if(!pBuf[tot].icon) pBuf[tot].icon = MyLoadMasterIcon(pBuf[tot].iconPath);
			if(pBuf[tot].icon) CreateBMP(pBuf[tot].icon,&pBuf[tot].iconBM,&pBuf[tot].iconDC,&pBuf[tot].width,&pBuf[tot].height);
		} else {pBuf[tot].iconNum = -1; pBuf[tot].icon = NULL;}
	} else
	{	pBuf[tot].iconPath[0] = '\0';
		pBuf[tot].iconPathLn = 0;
		pBuf[tot].icon = NULL;
	}

	if(fAss[2] && '\0'!=fAss[2])
	{	if('%'== *fAss[2])
		{	pBuf[tot].appPathLn = (char)ExpandEnvironmentStrings(fAss[2],pBuf[tot].appPath,MAX_PATH-1)-1;
			wchar_t *p=wcsrchr(pBuf[tot].appPath,' ');
			if(p)
			{	*p=0;
				pBuf[tot].appPathLn = (char)(p-&pBuf[tot].appPath[0]);	
			}
			//pBuf[tot].appPathLn=MyStringCpy(pBuf[tot].appPath,p-fAss[2],fAss[2]);
		}
		else
		{	if('"'!=*fAss[2])
			{
				pBuf[tot].appPathLn = MyStringCpy(pBuf[tot].appPath,MAX_PATH,fAss[2]);
				wchar_t *p=wcsrchr(pBuf[tot].appPath,' ');
				if(p)
				{	*p=0;
					pBuf[tot].appPathLn = (char)(p-&pBuf[tot].appPath[0]);	
	}	}	}	}
	else
	{	pBuf[tot].appPath[0] = '\0';
		pBuf[tot].appPathLn = 0;
	}

	if(fAss[3] && '\0'!=fAss[3])
		pBuf[tot].appDescLn = MyStringCpy(pBuf[tot].appDesc,MAX_PATH-1,fAss[3]);
	else
	{	pBuf[tot].appDesc[0] = '\0';
		pBuf[tot].appDescLn = 0;
	}

	++tot;
	{	int oi = IsOwnIcoOrCurExt(ext);
		if(oi<0) return oi;
	}
	//QueryPerformanceCounter((LARGE_INTEGER*)&d[1]);
	//d[2]=d[1]-d[0];
	return tot-1;
}

BOOL FileAssociation::AddMasterIcon(PanelItem* it, wchar_t *path)//If cur or ico file;
{
static wchar_t fullName[MAX_PATH];
	switch(it->assocNum)//Lovushka uchun har 1ini alohida yozduk:
	{	case -2://ico:
			MyStringCpy(fullName,MAX_PATH-1,path);
			MyStringEraseEndAndCat(fullName,MAX_PATH-1,it->Name);
			it->hMasterIcon = MyLoadMasterIcon(fullName);
			if(it->hMasterIcon)
				return TRUE;
			return FALSE;
		case -3://cur:
			MyStringCpy(fullName,MAX_PATH-1,path);
			MyStringEraseEndAndCat(fullName,MAX_PATH-1,it->Name);
			it->hMasterIcon = MyLoadMasterIcon(fullName);
			if(it->hMasterIcon)
				return TRUE;
			return FALSE;
		case -4://exe:
			MyStringCpy(fullName,MAX_PATH-1,path);
			MyStringEraseEndAndCat(fullName,MAX_PATH-1,it->Name);
			it->hMasterIcon = MyLoadMasterIcon(fullName);
			if(it->hMasterIcon)
				return TRUE;
			return FALSE;
		case -5://dll:
			MyStringCpy(fullName,MAX_PATH-1,path);
			MyStringEraseEndAndCat(fullName,MAX_PATH-1,it->Name);
			it->hMasterIcon = MyLoadMasterIcon(fullName);
			if(it->hMasterIcon)
				return TRUE;
			return FALSE;
		case -6://sys:
			MyStringCpy(fullName,MAX_PATH-1,path);
			MyStringEraseEndAndCat(fullName,MAX_PATH-1,it->Name);
			it->hMasterIcon = MyLoadMasterIcon(fullName);
			if(it->hMasterIcon)
				return TRUE;
			return FALSE;
	}
	return FALSE;
}

VOID FileAssociation::FreeMem()//in panel-folderUp, for example;
{
 __try
 {
	if(pBuf)
	{	for(int i=0; i<tot; i++)
		{	if(pBuf[i].icon)  DestroyIcon(pBuf[i].icon);
			pBuf[i].icon = NULL;
			if(pBuf[i].iconDC)DeleteObject(pBuf[i].iconDC);
			pBuf[i].iconDC = NULL;
			if(pBuf[i].iconBM)DeleteObject(pBuf[i].iconBM);
			pBuf[i].iconBM = NULL;
		}
		free(pBuf);
		pBuf = NULL;
	}
 }

 __except(EXCEPTION_EXECUTE_HANDLER)
 {	MessageBox(NULL,L"FileAssociation::FreeMem:",L"Exception:",MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
 }

	pBuf = NULL;
	totMax = 0;
	tot = 0;
}

HICON* FileAssociation::GetIcon(int n)
{
	if(n<0)return NULL;
	if(n>=tot)return NULL;
	if(pBuf[n].icon)
		return &pBuf[n].icon;
	return NULL;
}

HBITMAP* FileAssociation::GetIconBM(int n)
{
	if(n<0)return NULL;
	if(n>=tot)return NULL;
	if(pBuf[n].iconBM)
		return &pBuf[n].iconBM;
	return NULL;
}

int FileAssociation::GetIconWidth(int n)
{
	if(n<0)return NULL;
	if(n>=tot)return NULL;
	if(pBuf[n].icon)
		return pBuf[n].width;
	return 0;
}

int FileAssociation::GetIconHeight(int n)
{
	if(n<0)return NULL;
	if(n>=tot)return NULL;
	if(pBuf[n].icon)
		return pBuf[n].height;
	return 0;
}

HDC* FileAssociation::GetIconDC(int n)
{
	if(n<0)return NULL;
	if(n>=tot)return NULL;
	if(!pBuf[n].icon) return NULL;
	return &pBuf[n].iconDC;
}

int FileAssociation::IsOwnIcoOrCurExt(wchar_t *ext)
{
	if('i'==ext[0])
	if('c'==ext[1])
	if('o'==ext[2])
		return -2;
	if('I'==ext[0])
	if('C'==ext[1])
	if('O'==ext[2])
		return -2;

	if('c'==ext[0])
	if('u'==ext[1])
	if('r'==ext[2])
		return -3;
	if('C'==ext[0])
	if('U'==ext[1])
	if('R'==ext[2])
		return -3;

	if('e'==ext[0])
	if('x'==ext[1])
	if('e'==ext[2])
		return -4;
	if('E'==ext[0])
	if('X'==ext[1])
	if('E'==ext[2])
		return -4;

	if('d'==ext[0])
	if('l'==ext[1])
	if('l'==ext[2])
		return -5;
	if('D'==ext[0])
	if('L'==ext[1])
	if('L'==ext[2])
		return -5;

	if('s'==ext[0])
	if('y'==ext[1])
	if('s'==ext[2])
		return -6;
	if('S'==ext[0])
	if('Y'==ext[1])
	if('S'==ext[2])
		return -6;
	return 0;
}

int FileAssociation::IsRecExist(wchar_t *ext)
{
	for(int c=0; c<tot; c++)
	{	BOOL bExist = TRUE; int l;
		for(l=0; l<pBuf[c].extLn; l++)
		{	if(ext[l] != pBuf[c].ext[l])
				{bExist = FALSE; break;}
		} if(bExist && '\0'==ext[l])
			return c;
	}
	return -1;
}

BOOL FileAssociation::MemCheck()
{
	if(!totMax)
	{	pBuf = (TCell*)malloc(ASS_HEAP_INC_ITEM_NUMBER*sizeof(TCell));
		if(!pBuf) return FALSE;
		totMax = ASS_HEAP_INC_ITEM_NUMBER;
		return TRUE;
	} else if(tot < totMax-2) return TRUE;
	//else
	pBuf = (TCell*)realloc(pBuf,(totMax+ASS_HEAP_INC_ITEM_NUMBER)*sizeof(TCell));
	if(!pBuf) return FALSE;
	totMax += ASS_HEAP_INC_ITEM_NUMBER;
	return TRUE;
}

#undef ASS_HEAP_INC_ITEM_NUMBER
#undef MAX_EXT


/* *.lnk file taxlash:
#define LD_USEDESC     0x00000001
#define LD_USEARGS     0x00000002
#define LD_USEICON     0x00000004
#define LD_USEWORKDIR  0x00000008
#define LD_USEHOTKEY   0x00000010
#define LD_USESHOWCMD  0x00000020
typedef struct {  // Mandatory members
	LPTSTR pszPathname; // Pathname of original object  
	DWORD fdwFlags;     // LD_* flags ORed together for optional members  
						// Optional members  
	LPTSTR pszDesc;     // Description of link file (its filename)  
	LPTSTR pszArgs;     // command-line arguments  
	LPTSTR pszIconPath; // Pathname of file containing the icon  
	int  nIconIndex;    // Index of icon in pszIconPath  
	LPTSTR pszWorkingDir;// Working directory when process starts  
	int  nShowCmd;      // How to show the initial window  
	WORD  wHotkey;      // Hot key for the link
} LINKDATA, *PLINKDATA;
HRESULT WINAPI Shell_CreateLink (LPCTSTR,PLINKDATA);


int _tmain(int argc, _TCHAR* argv[])
{
LINKDATA ld;
	Shell_CreateLink((LPCTSTR)"C:\\Program Files\\Microsoft Games\\Mahjong\\MahjongMCE.lnk",&ld);
}

HRESULT WINAPI Shell_CreateLink (LPCTSTR pszLinkFilePathname, PLINKDATA pld)
{
HRESULT hres;
IShellLink* psl;
IPersistFile* ppf;  hres = CoInitialize(NULL);//Create a shell link object
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &psl);
	if (SUCCEEDED(hres))
	{//Initialize the shell link object   
		psl->SetPath(pld->pszPathname);
		if (pld->fdwFlags & LD_USEARGS)
			psl->SetArguments(pld->pszArgs);
		if (pld->fdwFlags & LD_USEDESC)      
			psl->SetDescription(pld->pszDesc);
		if (pld->fdwFlags & LD_USEICON)
			psl->SetIconLocation(pld->pszIconPath, pld->nIconIndex);
		if (pld->fdwFlags & LD_USEWORKDIR)
			psl->SetWorkingDirectory(pld->pszWorkingDir);
		if (pld->fdwFlags & LD_USESHOWCMD)
			psl->SetShowCmd(pld->nShowCmd);
		if (pld->fdwFlags & LD_USEHOTKEY)
			psl->SetHotkey(pld->wHotkey);// Save the shell link object on the disk
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if (SUCCEEDED(hres))
		{
#ifndef UNICODE // Convert the ANSI path to a Unicode path     
			WCHAR szPath[_MAX_PATH] = { 0 };
			MultiByteToWideChar(CP_ACP, 0, pszLinkFilePathname, strlen(pszLinkFilePathname), szPath, adgARRAY_SIZE(szPath));
			hres = ppf->Save(szPath, TRUE);
#else     
			hres = ppf->Save(pszLinkFilePathname, TRUE);
#endif     
			ppf->Release();
		}   
		psl->Release();
	}
	CoUninitialize();
	return(hres);
}
*/