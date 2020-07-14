// AfterSetup.cpp : Defines the entry point for the console application.
//

#pragma comment(lib,"Msi.lib")

#ifndef UNICODE
#define UNICODE
#endif //UNICODE

#ifndef _WIN32_MSI
#define _WIN32_MSI 200
#endif //_WIN32_MSI

#include <stdio.h>
#include <windows.h>
#include <msi.h>

const int cchGUID = 38;



BOOL GetMSIInfoFrReg(wchar_t*);
BOOL GetMSIInfo(wchar_t*,wchar_t*);
BOOL GetSerNumFrMsi(wchar_t*,wchar_t*);
DWORD GetSerNumOffsInPE(wchar_t*);
BOOL ChngeSerNumInPE(wchar_t*,char*);
BOOL SelectLastMsi(wchar_t*);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)//int main()
{
int numArgs;
LPWSTR *args = CommandLineToArgvW(GetCommandLine(),&numArgs);
	//MessageBoxA(NULL,"Command Line is ...",lpCmdLine,MB_OK);
	//MessageBox(NULL,args[0],args[1],MB_OK);
wchar_t instSrc[MAX_PATH]=L"",instLoc[MAX_PATH];
wcscpy(instLoc,args[0]);
#ifdef _WIN64
wcscpy(instSrc,args[1]);
#else
int l=MultiByteToWideChar(CP_ACP,0,lpCmdLine,-1,instSrc,MAX_PATH);
if(l){instSrc[l++]='\\';instSrc[l]=0;}
#endif
wchar_t *p=wcsrchr(instLoc,'\\');
if(p)*(p+1)=0;

#ifdef _WIN64
wcscat(instSrc,L"Sino64.msi");
wcscat(instLoc,L"Sino64.exe");
#else
wcscat(instSrc,L"Sino.msi");
wcscat(instLoc,L"Sino.exe");
#endif

  //MessageBox(NULL,instSrc,L"instSrc",MB_OK);
  //MessageBox(NULL,instLoc,L"instLoc",MB_OK);
   
  wchar_t serNum[32];
  if(GetSerNumFrMsi(instSrc,serNum))
   ChngeSerNumInPE(instLoc,(char*)serNum);

wcscpy(instSrc,instLoc);
p=wcsrchr(instLoc,'\\');
if(p)*p=0;
  //MessageBox(NULL,L"instSrc",instSrc,MB_OK);
  //MessageBox(NULL,L"instLoc",instLoc,MB_OK);
//ShellExecuteW(GetDesktopWindow(),L"explore",instSrc,NULL,instLoc,SW_SHOWNORMAL);


SHELLEXECUTEINFO she;ZeroMemory(&she,sizeof(she));she.cbSize=sizeof(she);she.nShow=SW_MAXIMIZE;she.lpVerb=L"open";
she.lpFile=instLoc;
int err=ShellExecuteEx(&she);
//err=GetLastError();

	
return 0;
}

BOOL GetMSIInfoFrReg(wchar_t *dst)
{
HKEY kPrdcts;DWORD sz,ind=0;wchar_t s[MAX_PATH];BOOL bNotFnd=TRUE;
HRESULT r = RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Classes\\Installer\\Products",0,
						 KEY_ENUMERATE_SUB_KEYS,&kPrdcts);
	if(ERROR_SUCCESS!=r) return FALSE;
	r = ERROR_SUCCESS;dst[0]=0;
	while((ERROR_SUCCESS==r) && ERROR_NO_MORE_ITEMS!=r && bNotFnd)
	{	sz=MAX_PATH;r = RegEnumKeyEx(kPrdcts,ind++,s,&sz,NULL,NULL,NULL,NULL);
		if(ERROR_SUCCESS==r || ERROR_NO_MORE_ITEMS==r)
		{	HKEY k;HRESULT r1=RegOpenKeyEx(kPrdcts,s,0,KEY_QUERY_VALUE,&k);
			if(ERROR_SUCCESS==r1)
			{	wchar_t sn[MAX_PATH];DWORD snSz=MAX_PATH;
				r1=RegQueryValueEx(k,L"ProductName",NULL,NULL,(LPBYTE)&sn[0],&snSz);
				if(ERROR_SUCCESS==r1)
				{	if(0==wcscmp(sn,L"Sino"))
					{	snSz=MAX_PATH;r1=RegQueryValueEx(k,L"PackageCode",NULL,NULL,(LPBYTE)&sn[0],&snSz);
						if(ERROR_SUCCESS==r1)
							wcscpy(dst,sn);
						bNotFnd=FALSE;
				}	}
				RegCloseKey(k);
	}	}	}
	RegCloseKey(kPrdcts);
	return (0!=dst[0]) && (!bNotFnd);
}

BOOL GetMSIInfo(wchar_t *instSrc,wchar_t *instLoc)
{UINT r=0;DWORD i=0;wchar_t s[40];instSrc[0]=instLoc[0]=0;
	while(ERROR_SUCCESS==r && ERROR_NO_MORE_ITEMS!=r)
	{	r=MsiEnumProducts(i++,s);
		if(ERROR_SUCCESS==r && ERROR_NO_MORE_ITEMS!=r)
		{	wchar_t st[MAX_PATH]=L"";DWORD sz=MAX_PATH;
			UINT u=MsiGetProductInfo(s,INSTALLPROPERTY_PRODUCTNAME,st,&sz);
			if(0==wcscmp(st,L"Sino"))
			{	sz=MAX_PATH;u=MsiGetProductInfo(s,INSTALLPROPERTY_INSTALLSOURCE,st,&sz);
				if(ERROR_SUCCESS==u)
					wcscpy(instSrc,st);
				sz=MAX_PATH;u=MsiGetProductInfo(s,INSTALLPROPERTY_INSTALLLOCATION,st,&sz);
				//if(ERROR_SUCCESS==u)
				//	wcscpy(instLoc,st);
				MessageBox(NULL,L"2.GetMSIInfo finished successfully",instSrc,MB_OK);
				return TRUE;
	}	}	}
	//MessageBox(NULL,L"2.GetMSIInfo finished",L"unsuccessfully",MB_OK);
	return FALSE;
}

BOOL GetSerNumFrMsi(wchar_t *instSrc,wchar_t *serNum)
{FILE *f=_wfopen(instSrc,L"rb");
 if(!f)return FALSE;
#ifdef _WIN64
 fseek(f,0x16d50,SEEK_SET);
#else
 fseek(f,0x18930,SEEK_SET);
#endif
 char buf[4096+16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 for(int i=0; i<0x15; i++)//0x15000=0x15*0x1000(4096)
 {	int r=(int)fread(&buf[15],4,1024,f);
	for(int k=0; k<15+r*4; k++)
	{	if(0==strncmp(&buf[k],"Product serial number: ",23))
		{	if(buf[k+27]!='-' && buf[k+32]!='-' && buf[k+37]!='-')
				continue;
			strncpy((char*)serNum,&buf[k+23],19);
			fclose(f);
			//serNum[19]=0;
			//MessageBoxA(NULL,"4.GetSerNumFrMsi finished successfully",(char*)serNum,MB_OK);
			return TRUE;	
	}	}
	if(r>15)
		memcpy(buf,&buf[r-15],15);
 }
 fclose(f);
 return FALSE;
}

DWORD GetSerNumOffsInPE(wchar_t *PEName)
{//MessageBox(NULL,PEName,L"6.Starting GetSerNumOffsInPE ...",MB_OK);
 FILE *f=_wfopen(PEName,L"rb");
 if(!f)
 {//MessageBox(NULL,PEName,L"6.Error opening ...",MB_OK);
  return 0xFFFFFFFF;
 }
 fseek(f,0x110000,SEEK_SET);
 char buf[4096+16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 for(int i=0; i<0x15; i++)//0x15000=0x15*0x1000(4096)
 {	int r=(int)fread(&buf[15],4,1024,f);
    //MessageBoxA(NULL,"6.Reading to buffer 4*1024",&buf[15],MB_OK);
	for(int k=0; k<15+r*4; k++)
	{	if(0==wcsncmp((wchar_t*)&buf[k],L"SINOSTRING",10))
		{	if(0==wcsncmp((wchar_t*)&buf[k+22],L"SERIAL_NUMBER",13))
			{	fclose(f);
				//wchar_t s[MAX_PATH];wsprintf(s,L"0x%x",0x110000+i*4*1024+k+35);
				//MessageBox(NULL,L"6.GetSerNumOffsInPE finished successfully",s,MB_OK);
				return 0x110000+i*4*1024+k+35;	
	}	}	}
	if(r>15)
		memcpy(buf,&buf[r-15],15);//dumini oldinga suramiz;
 }
 fclose(f);
 return 0xFFFFFFFF;
}

BOOL ChngeSerNumInPE(wchar_t *PEName,char *serNum)
{
//MessageBox(NULL,PEName,L"5.Starting ChngeSerNumInPE ...",MB_OK);
HANDLE hUpdRes = BeginUpdateResource(PEName,FALSE);
if(hUpdRes)
{char newSer[20];for(int i=0; i<19; i++)newSer[i]=serNum[i];serNum[19]=0;
 if(UpdateResource(hUpdRes,L"SINOSTRING",L"SERIAL_NUMBER",MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),newSer,19))
 {if(EndUpdateResource(hUpdRes,FALSE))
   return TRUE;
}}
//else MessageBox(NULL,PEName,L"5.Error BeginUpdateResource ...",MB_OK);
DWORD offsInPE=GetSerNumOffsInPE(PEName);
if(0xFFFFFFFF==offsInPE)return FALSE;
FILE *f=_wfopen(PEName,L"r+b");
if(!f)return FALSE;
fseek(f,offsInPE,SEEK_SET);
char ch[19];
for(int i=0; i<19; i++)
 ch[i]=serNum[i];	
fwrite(ch,1,19,f);
fclose(f);
//MessageBox(NULL,L"5.ChngeSerNumInPE finished successfully",L"",MB_OK);
return TRUE;
}

/*BOOL SelectLastMsi(wchar_t *FstMsiPath)
{
int l=wcslen(FstMsiPath);
wcscpy(&FstMsiPath[l],L"*.msi");
WIN32_FIND_DATA fd;
HANDLE h=FindFirstFile(FstMsiPath,&fd);
if(INVALID_HANDLE_VALUE==h)return FALSE;
wchar_t fn[MAX_PATH]=L"Sino.msi";
FILETIME wrtm={0,0};

if(fd.ftLastWriteTime.dwLowDateTime>wrtm.dwLowDateTime)
{if(wcsstr(fd.cFileName,L"Sino"))
 {wrtm=fd.ftLastWriteTime;
  wcscpy(fn,fd.cFileName);
}}

BOOL b=TRUE;while(b)
{b=FindNextFile(h,&fd);
 if(b && fd.ftLastWriteTime.dwLowDateTime>wrtm.dwLowDateTime)
 {if(wcsstr(fd.cFileName,L"Sino"))
  {wrtm=fd.ftLastWriteTime;
   wcscpy(fn,fd.cFileName);
}}}
FindClose(h);
wcscpy(&FstMsiPath[l],fn);
//MessageBox(NULL,L"3.SelectLastMsi finished successfully",FstMsiPath,MB_OK);
return TRUE;
}*/
