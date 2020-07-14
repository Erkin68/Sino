#include "Registry.h"
#include "strsafe.h"
#include <malloc.h>
#include "Sino.h"



namespace registry
{

//4 ta char* ning 1-si adresini qaytaradi, oxirgi 3-si NULL b-shiyam mumkin:
//1-ext faylining deskriptori;
//2-fayl iconi exe(dll) si, icon raqami, stringda;
//3-faylni ochadigan app exe gi yo'li;
//4- 3 ning deckriptori;
//Yo'llar %System
/*wchar_t** GetFileOpenWithAssociation(wchar_t* Ext)
{
static wchar_t ext[128];BYTE buf[520];
HKEY k,kDefIcon=NULL,kAppl=NULL,kCmnd=NULL; LONG r; DWORD sz;

	//Avval qaysi app egallaganini topamiz, aniqrog'i registrdagi ko'rsatkichini:
	if('.'!=Ext[0])
	{	ext[0] = '.';//ext[1] = 0;
		MyStringCpy(&ext[1],127,Ext);//MyStringCat(ext, 128, Ext);
	} else MyStringCpy(ext,127,Ext);//MyStringCat(ext, 128, Ext);
	r = RegOpenKeyEx(HKEY_CLASSES_ROOT,ext,0,KEY_QUERY_VALUE,&k);
	if(ERROR_SUCCESS!=r) return NULL;

	//NULL-default value:
	//r = RegQueryValueEx(k,NULL,NULL,NULL,NULL,&sz); 520 taga sig'adu, deb faraz qilamiz;
	//if(ERROR_SUCCESS!=r)
	//{	RegCloseKey(k);
	//	return NULL;
	//}
	sz = 520;
	r = RegQueryValueEx(k,NULL,NULL,NULL,buf,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}
	RegCloseKey(k);

	//Registrdagi app ko'rsatkichidan qaysi app ligini aniqlaymiz:
	r = RegOpenKeyEx(HKEY_CLASSES_ROOT,(LPWSTR)buf,0,KEY_QUERY_VALUE,&k);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}

static wchar_t *pRets[4];
	pRets[0]='\0';	pRets[1]='\0';	pRets[2]='\0'; pRets[3]='\0';
static wchar_t FileDescription[MAX_PATH];
	r = RegQueryValueEx(k,NULL,NULL,NULL,NULL,&sz);
	if(ERROR_SUCCESS!=r || sz>=MAX_PATH)
	{	RegCloseKey(k);
		return NULL;
	}
	r = RegQueryValueEx(k,NULL,NULL,NULL,(LPBYTE)FileDescription,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}
	pRets[0] = &FileDescription[0];

	r = RegOpenKeyEx(k,L"DefaultIcon",0,KEY_QUERY_VALUE,&kDefIcon);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}

static wchar_t DefaultIconDescription[MAX_PATH];
	//r = RegQueryValueEx(kDefIcon,NULL,NULL,NULL,NULL,&sz);
	//if(ERROR_SUCCESS!=r || sz>=MAX_PATH)
	//{	RegCloseKey(k);
	//	RegCloseKey(kDefIcon);
	//	return NULL;
	//}
	sz = MAX_PATH*sizeof(wchar_t);
	r = RegQueryValueEx(kDefIcon,NULL,NULL,NULL,(LPBYTE)DefaultIconDescription,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		return NULL;
	}
	pRets[1] = &DefaultIconDescription[0];

	r = RegOpenKeyEx(k,L"Shell\\Open\\Command",0,KEY_QUERY_VALUE,&kCmnd);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		return &pRets[0];
	}

static wchar_t ApplicationPath[MAX_PATH];
	//r = RegQueryValueEx(kCmnd,NULL,NULL,NULL,NULL,&sz);
	//if(ERROR_SUCCESS!=r || sz>=MAX_PATH)
	//{	RegCloseKey(k);
	//	RegCloseKey(kDefIcon);
	//	RegCloseKey(kCmnd);
	//	return &pRets[0];
	//}
	sz=MAX_PATH*sizeof(wchar_t);
	r = RegQueryValueEx(kCmnd,NULL,NULL,NULL,(LPBYTE)ApplicationPath,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		RegCloseKey(kCmnd);
		return &pRets[0];
	}
	pRets[2] = &ApplicationPath[0];

	r = RegOpenKeyEx(k,L"Shell\\Open\\ddeexec\\Application",0,KEY_QUERY_VALUE,&kAppl);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		RegCloseKey(kCmnd);
		return &pRets[0];
	}

static wchar_t ApplicationDescription[MAX_PATH];
	//r = RegQueryValueEx(kAppl,NULL,NULL,NULL,NULL,&sz);
	//if(ERROR_SUCCESS!=r || sz>=MAX_PATH)
	//{	RegCloseKey(k);
	//	RegCloseKey(kDefIcon);
	//	RegCloseKey(kCmnd);
	//	RegCloseKey(kAppl);
	//	return &pRets[0];
	//}
	sz=MAX_PATH*sizeof(wchar_t);
	r = RegQueryValueEx(kAppl,NULL,NULL,NULL,(LPBYTE)ApplicationDescription,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		RegCloseKey(kCmnd);
		RegCloseKey(kAppl);
		return &pRets[0];
	}
	pRets[3] = &ApplicationDescription[0];

	RegCloseKey(kAppl);
	RegCloseKey(kCmnd);
	RegCloseKey(kDefIcon);
	RegCloseKey(k);
	return &pRets[0];
}*/

/*HANDLE GetPrcssTkn()
{
	HANDLE hToken;
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
		return FALSE;
	return hToken;
}
HANDLE hPrcssTkn = GetPrcssTkn();*/

wchar_t** GetFileOpenWithAssociation(wchar_t* Ext)
{
static wchar_t ext[128];BYTE buf[520];
HKEY k,kDefIcon=NULL,kAppl=NULL,kCmnd=NULL; LONG r; DWORD sz;

	//Avval qaysi app egallaganini topamiz, aniqrog'i registrdagi ko'rsatkichini:
	if('.'!=Ext[0])
	{	ext[0] = '.';//ext[1] = 0;
		MyStringCpy(&ext[1],127,Ext);//MyStringCat(ext, 128, Ext);
	} else MyStringCpy(ext,127,Ext); //MyStringCat(ext, 128, Ext);
	r = RegOpenKeyEx(HKEY_CLASSES_ROOT,ext,0,KEY_QUERY_VALUE,&k);
	if(ERROR_SUCCESS!=r) return NULL;

	sz = 520;
	r = RegQueryValueEx(k,NULL,NULL,NULL,buf,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}
	RegCloseKey(k);

	//Registrdagi app ko'rsatkichidan qaysi app ligini aniqlaymiz:
	r = RegOpenKeyEx(HKEY_CLASSES_ROOT,(LPWSTR)buf,0,KEY_QUERY_VALUE,&k);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}

static wchar_t *pRets[4];
	pRets[0]='\0';	pRets[1]='\0';	pRets[2]='\0'; pRets[3]='\0';
static wchar_t FileDescription[MAX_PATH];
	r = RegQueryValueEx(k,NULL,NULL,NULL,NULL,&sz);
	if(ERROR_SUCCESS!=r || sz>=MAX_PATH)
	{	RegCloseKey(k);
		return NULL;
	}
	r = RegQueryValueEx(k,NULL,NULL,NULL,(LPBYTE)FileDescription,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}
	pRets[0] = &FileDescription[0];

	r = RegOpenKeyEx(k,L"DefaultIcon",0,KEY_QUERY_VALUE,&kDefIcon);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		return NULL;
	}

static wchar_t DefaultIconDescription[MAX_PATH];
	sz = MAX_PATH*sizeof(wchar_t);
	r = RegQueryValueEx(kDefIcon,NULL,NULL,NULL,(LPBYTE)DefaultIconDescription,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		return NULL;
	}
	pRets[1] = &DefaultIconDescription[0];


/*static VALENT v[2]={L"Shell\\Open\\Command",0,0,0,L"Shell\\Open\\ddeexec\\Application"};
DWORD szs[2]={MAX_PATH,MAX_PATH};
static wchar_t ApplicationPath[MAX_PATH];
	r=RegQueryMultipleValues(k,v,2,ApplicationPathAndDescrptn,szs);
	if(ERROR_SUCCESS==r)
	{	pRets[3] = ApplicationPathAndDescrptn;
		pRets[3] = &ApplicationPathAndDescrptn[MAX_PATH];
	}*/


/* Exe command b-n ochamiz:

static wchar_t ApplicationPath[2*MAX_PATH];
	r = RegOpenKeyEx(k,L"Shell\\Open\\Command",0,KEY_QUERY_VALUE,&kCmnd);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		return &pRets[0];
	}

	sz=MAX_PATH*sizeof(wchar_t);
	r = RegQueryValueEx(kCmnd,NULL,NULL,NULL,(LPBYTE)ApplicationPath,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		RegCloseKey(kCmnd);
		return &pRets[0];
	}
	pRets[2] = &ApplicationPath[0];

	r = RegOpenKeyEx(k,L"Shell\\Open\\ddeexec\\Application",0,KEY_QUERY_VALUE,&kAppl);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		RegCloseKey(kCmnd);
		return &pRets[0];
	}

static wchar_t ApplicationDescription[MAX_PATH];
	sz=MAX_PATH*sizeof(wchar_t);
	r = RegQueryValueEx(kAppl,NULL,NULL,NULL,(LPBYTE)ApplicationDescription,&sz);
	if(ERROR_SUCCESS!=r)
	{	RegCloseKey(k);
		RegCloseKey(kDefIcon);
		RegCloseKey(kCmnd);
		RegCloseKey(kAppl);
		return &pRets[0];
	}
	pRets[3] = &ApplicationDescription[0];
*/
	//RegCloseKey(kAppl);
	//RegCloseKey(kCmnd);
	RegCloseKey(kDefIcon);
	RegCloseKey(k);
	return &pRets[0];
}

BOOL IsCLSIDExistsA(TCHAR* clsid)
{
HKEY k,k1;
	if(ERROR_SUCCESS!=RegOpenKeyEx(HKEY_CLASSES_ROOT,L"CLSID",0,KEY_QUERY_VALUE,&k))
		return FALSE;
	if(ERROR_SUCCESS!=RegOpenKeyEx(k,clsid,0,KEY_QUERY_VALUE,&k1))
	{	//char *padd = strchr(clsid,'\\');
		//if(padd)
		//{	*padd = 0;
		//	if(ERROR_SUCCESS!=RegOpenKeyEx(k,clsid,0,KEY_QUERY_VALUE,&k1))
		//		{RegCloseKey(k); *padd = '\\'; return FALSE;}
		//	*padd = '\\';
		//}else{
		RegCloseKey(k); return FALSE;//}
	}
	RegCloseKey(k);
	RegCloseKey(k1);
	return TRUE;
}

BOOL IsCLSIDExists(wchar_t* clsid)
{
HKEY k,k1;
	if(ERROR_SUCCESS!=RegOpenKeyEx(HKEY_CLASSES_ROOT,L"CLSID",0,KEY_QUERY_VALUE,&k))
		return FALSE;
	if(ERROR_SUCCESS!=RegOpenKeyEx(k,clsid,0,KEY_QUERY_VALUE,&k1))
		{RegCloseKey(k); return FALSE;}
	RegCloseKey(k);
	RegCloseKey(k1);
	return TRUE;
}

}//end of namespace