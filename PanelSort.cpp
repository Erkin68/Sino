#include "Panel.h"
#include "Sino.h"
#include "Config.h"
#include "Operations\MyErrors.h"


namespace Sort
{


#define p1 ((PanelItem*)I1)
#define p2 ((PanelItem*)I2)


extern "C" int __cdecl CmpNumberStrs(wchar_t* dst,wchar_t* src,int iDst,int iSrc)
{
/*int d,s;                     pastdagi fn.ga o'zgartirish kiritib, birdaniga
	for(d=0; d<iDst; d++)	   hisoblaydigan qildim;
	{	if((*dst)!='0')
			break;
		++dst;
	}
	for(s=0; s<iSrc; s++)
	{	if((*src)!='0')
			break;
		++src;
	}
	int id=iDst-d;int is=iSrc-s;
	if(0==is)if(0!=id)return -1;
	if(0==id)if(0!=is)return  1;
	if(id!=is) return id>is?1:-1;*/
	if(0==iSrc)if(0!=iDst)return -1;
	if(0==iDst)if(0!=iSrc)return  1;
	if(iDst!=iSrc) return iDst>iSrc?1:-1;
	
	for(int i=0; i<iDst; i++)
	{	if((*dst) != (*src))
			return (*dst)>(*src)?1:-1;
		if((*src)==0) return -1;
		++dst;
		++src;
	}
	return 0;
}

extern "C" int __cdecl my_strnicmp(wchar_t * dst,wchar_t * src,int n1,int n2)
{
	for(;;)
	{	wchar_t *pDst=dst;	wchar_t *pSrc=src;
		BOOL bDstZero=TRUE,bSrcZero=TRUE;//Agar tepadagi olingan bo'lsa
		wchar_t *pDstNoZero=pDst,*pSrcNoZero=pSrc;//1 zaxodga birdaniga 0siz boshini
		for(;;)									//topish;
		{	if((wchar_t)(*pDst) < '0') break;	
			if((wchar_t)(*pDst) > '9') break;
			if((*pDst)!='0')
				bDstZero = FALSE;
			else if(bDstZero) pDstNoZero=pDst;
			pDst++;
		}
		for(;;)
		{	if((wchar_t)(*pSrc) < '0') break;	
			if((wchar_t)(*pSrc) > '9') break;
			if((*pSrc)!='0')
				bSrcZero = FALSE;
			else if(bSrcZero) pSrcNoZero=pSrc;
			pSrc++;
		}
		//if((*pDst)==0 || (*pSrc)==0)//2-sidan biri tugal bo'lsa,yo 2-siyam;
		//{	int r = CmpNumberStrs(dst,src,pDst-dst,pSrc-src);
		//	if(r!=0) return r;
		//}
		//int r = CmpNumberStrs(dst,src,pDst-dst,pSrc-src);
		int r = CmpNumberStrs(pDstNoZero,pSrcNoZero,(int)(pDst-pDstNoZero),(int)(pSrc-pSrcNoZero));
		if(r!=0) return r;

		//2-kisi teng va dalshe alfavit simvoli davomi bor;
		dst = pDst;	src = pSrc;
		for(;;)
		{	if((wchar_t)(*pDst) >= '0' && (wchar_t)(*pDst) <= '9') break;
			if((wchar_t)(*pDst) == 0) break;
			pDst++;
		}
		for(;;)
		{	if((wchar_t)(*pSrc) >= '0' && (wchar_t)(*pSrc) <= '9') break;
			if((wchar_t)(*pSrc) == 0) break;
			pSrc++;
		}
		//r = _wcsnicmp(dst,src,pDst-dst<=pSrc-src?pDst-dst:pSrc-src);
		r = _wcsnicmp(dst,src,n1<n2?n1:n2);
		if(0!=r)return r;
		if((*pDst)==0)
		{	if((*pSrc)!=0) return 1;
			return 0;
		} else if((*pSrc)==0)
			return -1;
		dst = pDst; src = pSrc;
	}
    return 0;
}



//Sorting funcs, all folders move to up:
//Sekin ishlagani uchun boshqacha metod qo'llaymiz, ya'ni avval raqamlab chiqamiz:
int compareUpFldrAlphabUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name,p1->NameCharLen,p2->NameCharLen);
}

int compareUpFldrAlphabUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrAlphabDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}// else
	if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return r;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabUpFileTypeDwn(const void *I1, const void *I2)
{
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return r;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabDwnFileTypeUp(const void *I1, const void *I2)
{
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrAlphabDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrEnumFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrEnumFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return 0;
}

int	compareUpFldrTimeUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareUpFldrTimeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareUpFldrTimeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareUpFldrTimeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareUpFldrTimeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareUpFldrTimeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareUpFldrTimeUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	} else
	if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareUpFldrTimeUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareUpFldrTimeDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareUpFldrTimeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareUpFldrTimeUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareUpFldrTimeUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareUpFldrTimeDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareUpFldrTimeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareUpFldrTimeUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareUpFldrTimeUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareUpFldrTimeDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareUpFldrTimeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareUpFldrSizeUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrSizeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r =-my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareUpFldrTypeUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareUpFldrTypeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrTypeUpFileTypeUp(const void *I1, const void *I2)
{
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrTypeUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrTypeDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareUpFldrTypeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return 1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return -1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrAlphabUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrAlphabUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrAlphabDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrAlphabDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrAlphabUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrAlphabDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrAlphabDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrEnumFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrEnumFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return 0;
}

int	compareDwnFldrTimeUpFileAlphabUp(const void *I1, const void *I2)
{
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareDwnFldrTimeUpFileAlphabDwn(const void *I1, const void *I2)
{
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareDwnFldrTimeDwnFileAlphabUp(const void *I1, const void *I2)
{
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareDwnFldrTimeDwnFileAlphabDwn(const void *I1, const void *I2)
{
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareDwnFldrTimeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareDwnFldrTimeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareDwnFldrTimeUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareDwnFldrTimeUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareDwnFldrTimeDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareDwnFldrTimeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareDwnFldrTimeUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareDwnFldrTimeUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareDwnFldrTimeDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int compareDwnFldrTimeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareDwnFldrTimeUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareDwnFldrTimeUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareDwnFldrTimeDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareDwnFldrTimeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareDwnFldrSizeUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size > p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrSizeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	if(p2->size == p1->size) return 0;
	return (p2->size < p1->size) ? 1 : -1;
}

int	compareDwnFldrTypeUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		return (t2>t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareDwnFldrTypeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		return (p2->size > p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareDwnFldrTypeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute != folder)
	{	if(p2->attribute == folder)
			return -1;
		if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	else if(p2->attribute != folder)
		return 1;
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareMixFldrAlphabUpFileAlphabUp(const void *I1, const void *I2)
{	
	return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int compareMixFldrAlphabUpFileAlphabDwn(const void *I1, const void *I2)
{	
	return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
}

int	compareMixFldrAlphabUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrAlphabDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrAlphabUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrAlphabUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrAlphabDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrAlphabDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrAlphabUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return (p2->size > p1->size) ? 1 : -1;
	return 0;
}

int	compareMixFldrAlphabUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrAlphabDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrAlphabDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrAlphabUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrAlphabUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrAlphabDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrAlphabDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrEnumFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return 0;
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrEnumFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return 0;
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrEnumFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrEnumFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrEnumFileEnum(const void *I1, const void *I2)
{	
	return 0;
}

int	compareMixFldrEnumFileTimeUp(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return 0;
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrEnumFileTimeDwn(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return 0;
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrEnumFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return 0;
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrEnumFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
		return 0;
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int compareMixFldrTimeUpFileAlphabUp(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int compareMixFldrTimeUpFileAlphabDwn(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int compareMixFldrTimeDwnFileAlphabUp(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int compareMixFldrTimeDwnFileAlphabDwn(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int compareMixFldrTimeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int compareMixFldrTimeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int compareMixFldrTimeUpFileTimeUp(const void *I1, const void *I2)
{	
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int compareMixFldrTimeUpFileTimeDwn(const void *I1, const void *I2)
{	
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareMixFldrTimeUpFileSizeUp(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrTimeUpFileSizeDwn(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrTimeDwnFileSizeUp(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2==t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrTimeDwnFileSizeDwn(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrTimeUpFileTypeUp(const void *I1, const void *I2)
{
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrTimeUpFileTypeDwn(const void *I1, const void *I2)
{
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrTimeDwnFileTypeUp(const void *I1, const void *I2)
{
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrTimeDwnFileTypeDwn(const void *I1, const void *I2)
{
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrSizeUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrSizeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrSizeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrSizeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrSizeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrSizeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int	compareMixFldrSizeUpFileTimeUp(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrSizeUpFileTimeDwn(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrSizeDwnFileTimeUp(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrSizeDwnFileTimeDwn(const void *I1, const void *I2)
{
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int	compareMixFldrSizeUpFileSizeUp(const void *I1, const void *I2)
{	
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2>t1) ? 1 : (-1);
}

int	compareMixFldrSizeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
	unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
	if(t2 == t1) return 0;
	return (t2<t1) ? 1 : (-1);
}

int	compareMixFldrSizeUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrSizeUpFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrSizeDwnFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrSizeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
		if(!r)
			return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
		else return 0;
	}
	return 0;
}

int	compareMixFldrTypeUpFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrTypeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrTypeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrTypeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return 0;
}

int	compareMixFldrTypeUpFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	return 0;
}

int	compareMixFldrTypeDwnFileEnum(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	return 0;
}

int compareMixFldrTypeUpFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int compareMixFldrTypeUpFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int compareMixFldrTypeDwnFileTimeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2>t1) ? 1 : (-1);
	}
	return 0;
}

int compareMixFldrTypeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	unsigned __int64 t1 = ((unsigned __int64)p1->ftWrite.dwHighDateTime << 32) & p1->ftWrite.dwLowDateTime;
		unsigned __int64 t2 = ((unsigned __int64)p2->ftWrite.dwHighDateTime << 32) & p2->ftWrite.dwLowDateTime;
		if(t2 == t1) return 0;
		return (t2<t1) ? 1 : (-1);
	}
	return 0;
}

int compareMixFldrTypeUpFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	return 0;
}

int compareMixFldrTypeUpFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int compareMixFldrTypeDwnFileSizeUp(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size > p1->size) ? 1 : -1;
	}
	return 0;
}

int compareMixFldrTypeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	if(p1->attribute == folder)
	if(p2->attribute == folder)
	{	if(!p1->ExtCharLen) return  1;
		if(!p2->ExtCharLen) return -1;
		return -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	}
	if(p1->attribute != folder)
	if(p2->attribute != folder)
	{	if(p2->size == p1->size) return 0;
		return (p2->size < p1->size) ? 1 : -1;
	}
	return 0;
}

int compareMixFldrTypeUpFileTypeUp(const void *I1, const void *I2)
{	
int r;
	if(!p1->ExtCharLen) return  1;
	if(!p2->ExtCharLen) return -1;
	r = my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	if(!r)
		return my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	else return 0;
}

int compareMixFldrTypeDwnFileTypeDwn(const void *I1, const void *I2)
{	
int r;
	if(!p1->ExtCharLen) return  1;
	if(!p2->ExtCharLen) return -1;
	r = -my_strnicmp(p1->GetExt(), p2->GetExt(), p1->ExtCharLen, p2->ExtCharLen);
	if(!r)
		return -my_strnicmp(p1->Name, p2->Name, p1->NameCharLen, p2->NameCharLen);
	return r;
}
#undef p1
#undef p2
int iPanCstmSrt=0;
} //end of namespace


using namespace Sort;
//swapda oxirgi 2 DWORD ko'chirilmaydi;
void myQsort32_2(void*,size_t,size_t,int(*)(const void*, const void*));
VOID Panel::Sort()
{	
	if(conf::Dlg.iCustPanItemDefs[iThis])
		return SortWithCustomItems();
	if(totItems<1) return;
	switch(folderAndFileMixingSortType)
	{	case foldersUp:
			switch(folderSortType)
			{	case alphabeticalUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabUpFileTypeDwn);
							break;
					}
					break;
				case alphabeticalDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrAlphabDwnFileTypeDwn);
							break;
					}
					break;
				case forEnums:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrEnumFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeUpFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTimeDwnFileTypeDwn);
							break;
					}
					break;
				case forSizeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forSizeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forTypeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeUpFileTypeDwn);
							break;
					}
					break;
				case forTypeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareUpFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
			}
			break;
		case filesUp:
			switch(folderSortType)
			{	case alphabeticalUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabUpFileTypeDwn);
							break;
					}
					break;
				case alphabeticalDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrAlphabDwnFileTypeDwn);
							break;
					}
					break;
				case forEnums:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrEnumFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeUpFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),compareDwnFldrTimeDwnFileTypeDwn);
							break;
					}
					break;
				case forSizeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forSizeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrSizeDwnFileTypeDwn);
							break;
					}
					break;
				case forTypeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),compareDwnFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),compareDwnFldrTypeUpFileTypeDwn);
							break;
					}
					break;
				case forTypeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareDwnFldrTypeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),compareDwnFldrTypeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),compareDwnFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
			}
			break;
		case mixing:
			switch(folderSortType)
			{	case alphabeticalUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileTypeDwn);
							break;
					}
					break;
				case alphabeticalDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrAlphabDwnFileTypeDwn);
							break;
					}
					break;
				case forEnums:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileAlphabDwn);
							break;
						case forEnums://kerakmas;
						//myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrEnumFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTimeDwnFileTypeDwn);
							break;
					}
					break;
				case forSizeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forSizeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrSizeDwnFileTypeDwn);
							break;
					}
					break;
				case forTypeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
				case forTypeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),compareMixFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
			}
			break;
}	}


namespace cstmSort
{

#define p1 ((PanelItem*)I1)
#define p2 ((PanelItem*)I2)

int cmpCustm(const void *I1, const void *I2)
{
	if(0xff!=p1->iCstmItem)
	{	if(0xff==p2->iCstmItem)
		{	switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p1->iCstmItem].sortMthd)
			{	case 0:	return 0;
				case 1:	return -1;
				case 2:	return 1;
		}	}
		else//ikkalasiyam custom;
		{	switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p1->iCstmItem].sortMthd)
			{	case 0:
					switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p2->iCstmItem].sortMthd)
					{	case 0: return 0;
						case 1: return 1;
						case 2: return -1;
					}
				case 1:
					switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p2->iCstmItem].sortMthd)
					{	case 0: return -1;
						case 1: return 0;
						case 2: return 1;
					}
				case 2:
					switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p2->iCstmItem].sortMthd)
					{	case 0: return 1;
						case 1: return -1;
						case 2: return 0;
					}
	}	}	}
	if(0xff!=p2->iCstmItem)
	{	if(0xff==p1->iCstmItem)
		{	switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p2->iCstmItem].sortMthd)
			{	case 0:	return 0;
				case 1:	return 1;
				case 2:	return -1;
		}	}
		else//ikkalasiyam custom;
		{	switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p2->iCstmItem].sortMthd)
			{	case 0:
					switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p1->iCstmItem].sortMthd)
					{	case 0: return 0;
						case 1: return -1;
						case 2: return 1;
					}
				case 1:
					switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p1->iCstmItem].sortMthd)
					{	case 0: return 1;
						case 1: return 0;
						case 2: return -1;
					}
				case 2:
					switch(conf::Dlg.cstmItms[Sort::iPanCstmSrt][p1->iCstmItem].sortMthd)
					{	case 0: return -1;
						case 1: return 1;
						case 2: return 0;
					}
	}	}	}
	return 0;
}

//Sorting funcs, all folders move to up:
//Sekin ishlagani uchun boshqacha metod qo'llaymiz, ya'ni avval raqamlab chiqamiz:
int cstmCompareUpFldrAlphabUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileAlphabUp(I1, I2);
}

int cstmCompareUpFldrAlphabUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileAlphabDwn(I1, I2);
}

int cstmCompareUpFldrAlphabDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileAlphabUp(I1, I2);
}

int cstmCompareUpFldrAlphabDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileAlphabDwn(I1, I2);
}

int cstmCompareUpFldrAlphabUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileEnum(I1, I2);
}

int cstmCompareUpFldrAlphabDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileEnum(I1, I2);
}

int cstmCompareUpFldrAlphabUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileTimeUp(I1, I2);
}

int cstmCompareUpFldrAlphabUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileTimeDwn(I1, I2);
}

int cstmCompareUpFldrAlphabDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileTimeUp(I1, I2);
}

int cstmCompareUpFldrAlphabDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileTimeDwn(I1, I2);
}

int	cstmCompareUpFldrAlphabUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileSizeUp(I1, I2);
}

int	cstmCompareUpFldrAlphabUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileSizeDwn(I1, I2);
}

int	cstmCompareUpFldrAlphabDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileSizeUp(I1, I2);
}

int	cstmCompareUpFldrAlphabDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileSizeDwn(I1, I2);
}

int	cstmCompareUpFldrAlphabUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileTypeUp(I1, I2);
}

int	cstmCompareUpFldrAlphabUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabUpFileTypeDwn(I1, I2);
}

int	cstmCompareUpFldrAlphabDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileTypeUp(I1, I2);
}

int	cstmCompareUpFldrAlphabDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrAlphabDwnFileTypeDwn(I1, I2);
}

int	cstmCompareUpFldrEnumFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileAlphabUp(I1, I2);
}

int	cstmCompareUpFldrEnumFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileAlphabDwn(I1, I2);
}

int	cstmCompareUpFldrEnumFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileEnum(I1, I2);
}

int	cstmCompareUpFldrEnumFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileTimeUp(I1, I2);
}

int	cstmCompareUpFldrEnumFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileTimeDwn(I1, I2);
}

int	cstmCompareUpFldrEnumFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileSizeUp(I1, I2);
}

int	cstmCompareUpFldrEnumFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileSizeDwn(I1, I2);
}

int	cstmCompareUpFldrEnumFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileTypeUp(I1, I2);
}

int	cstmCompareUpFldrEnumFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrEnumFileTypeDwn(I1, I2);
}

int	cstmCompareUpFldrTimeUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileAlphabUp(I1, I2);
}

int	cstmCompareUpFldrTimeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareUpFldrTimeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareUpFldrTimeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileAlphabDwn(I1, I2);
}

int	cstmCompareUpFldrTimeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileEnum(I1, I2);
}

int	cstmCompareUpFldrTimeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileEnum(I1, I2);
}

int cstmCompareUpFldrTimeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileTimeUp(I1, I2);
}

int cstmCompareUpFldrTimeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileTimeDwn(I1, I2);
}

int cstmCompareUpFldrTimeDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileTimeUp(I1, I2);
}

int cstmCompareUpFldrTimeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileTimeDwn(I1, I2);
}

int cstmCompareUpFldrTimeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileSizeUp(I1, I2);
}

int cstmCompareUpFldrTimeUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileSizeDwn(I1, I2);
}

int cstmCompareUpFldrTimeDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileSizeUp(I1, I2);
}

int cstmCompareUpFldrTimeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileSizeDwn(I1, I2);
}

int cstmCompareUpFldrTimeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileTypeUp(I1, I2);
}

int cstmCompareUpFldrTimeUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeUpFileTypeDwn(I1, I2);
}

int cstmCompareUpFldrTimeDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileTypeUp(I1, I2);
}

int cstmCompareUpFldrTimeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTimeDwnFileTypeDwn(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileAlphabUp(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileAlphabDwn(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileEnum(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileEnum(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileTimeUp(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileTimeDwn(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileTimeUp(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileTimeDwn(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileSizeUp(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileSizeDwn(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileSizeUp(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileSizeDwn(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileTypeUp(I1, I2);
}

int	cstmCompareUpFldrSizeUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeUpFileTypeDwn(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileTypeUp(I1, I2);
}

int	cstmCompareUpFldrSizeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrSizeDwnFileTypeDwn(I1, I2);
}

int	cstmCompareUpFldrTypeUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileAlphabUp(I1, I2);
}

int	cstmCompareUpFldrTypeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareUpFldrTypeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareUpFldrTypeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileAlphabDwn(I1, I2);
}

int	cstmCompareUpFldrTypeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileEnum(I1, I2);
}

int	cstmCompareUpFldrTypeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileEnum(I1, I2);
}

int	cstmCompareUpFldrTypeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileTimeUp(I1, I2);
}

int	cstmCompareUpFldrTypeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileTimeDwn(I1, I2);
}

int	cstmCompareUpFldrTypeDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileTimeUp(I1, I2);
}

int	cstmCompareUpFldrTypeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileTimeDwn(I1, I2);
}

int	cstmCompareUpFldrTypeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileSizeUp(I1, I2);
}

int	cstmCompareUpFldrTypeUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileSizeDwn(I1, I2);
}

int	cstmCompareUpFldrTypeDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileSizeUp(I1, I2);
}

int	cstmCompareUpFldrTypeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileSizeDwn(I1, I2);
}

int cstmCompareUpFldrTypeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileTypeUp(I1, I2);
}

int cstmCompareUpFldrTypeUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeUpFileTypeDwn(I1, I2);
}

int cstmCompareUpFldrTypeDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileTypeUp(I1, I2);
}

int cstmCompareUpFldrTypeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareUpFldrTypeDwnFileTypeDwn(I1, I2);
}

int cstmCompareDwnFldrAlphabUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileAlphabUp(I1, I2);
}

int cstmCompareDwnFldrAlphabUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileAlphabDwn(I1, I2);
}

int cstmCompareDwnFldrAlphabDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileAlphabUp(I1, I2);
}

int cstmCompareDwnFldrAlphabDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileAlphabDwn(I1, I2);
}

int cstmCompareDwnFldrAlphabUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileEnum(I1, I2);
}

int cstmCompareDwnFldrAlphabDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileEnum(I1, I2);
}

int	cstmCompareDwnFldrAlphabUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileTimeUp(I1, I2);
}

int	cstmCompareDwnFldrAlphabUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileTimeDwn(I1, I2);
}

int	cstmCompareDwnFldrAlphabDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileTimeUp(I1, I2);
}

int	cstmCompareDwnFldrAlphabDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileTimeDwn(I1, I2);
}

int	cstmCompareDwnFldrAlphabUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileSizeUp(I1, I2);
}

int	cstmCompareDwnFldrAlphabUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileSizeDwn(I1, I2);
}

int	cstmCompareDwnFldrAlphabDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileSizeUp(I1, I2);
}

int	cstmCompareDwnFldrAlphabDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileSizeDwn(I1, I2);
}

int	cstmCompareDwnFldrAlphabUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileTypeUp(I1, I2);
}

int	cstmCompareDwnFldrAlphabUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabUpFileTypeDwn(I1, I2);
}

int	cstmCompareDwnFldrAlphabDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileTypeUp(I1, I2);
}

int	cstmCompareDwnFldrAlphabDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrAlphabDwnFileTypeDwn(I1, I2);
}

int	cstmCompareDwnFldrEnumFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileAlphabUp(I1, I2);
}

int	cstmCompareDwnFldrEnumFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileAlphabDwn(I1, I2);
}

int	cstmCompareDwnFldrEnumFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileEnum(I1, I2);
}

int	cstmCompareDwnFldrEnumFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileTimeUp(I1, I2);
}

int	cstmCompareDwnFldrEnumFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileTimeDwn(I1, I2);
}

int	cstmCompareDwnFldrEnumFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileSizeUp(I1, I2);
}

int	cstmCompareDwnFldrEnumFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileSizeDwn(I1, I2);
}

int	cstmCompareDwnFldrEnumFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileTypeUp(I1, I2);
}

int	cstmCompareDwnFldrEnumFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrEnumFileTypeDwn(I1, I2);
}

int	cstmCompareDwnFldrTimeUpFileAlphabUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileAlphabUp(I1, I2);
}

int	cstmCompareDwnFldrTimeUpFileAlphabDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareDwnFldrTimeDwnFileAlphabUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareDwnFldrTimeDwnFileAlphabDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileAlphabDwn(I1, I2);
}

int cstmCompareDwnFldrTimeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileEnum(I1, I2);
}

int cstmCompareDwnFldrTimeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileEnum(I1, I2);
}

int cstmCompareDwnFldrTimeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileTimeUp(I1, I2);
}

int cstmCompareDwnFldrTimeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileTimeDwn(I1, I2);
}

int cstmCompareDwnFldrTimeDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileTimeUp(I1, I2);
}

int cstmCompareDwnFldrTimeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileTimeDwn(I1, I2);
}

int cstmCompareDwnFldrTimeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileSizeUp(I1, I2);
}

int cstmCompareDwnFldrTimeUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileSizeDwn(I1, I2);
}

int cstmCompareDwnFldrTimeDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileSizeUp(I1, I2);
}

int cstmCompareDwnFldrTimeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileSizeDwn(I1, I2);
}

int	cstmCompareDwnFldrTimeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileTypeUp(I1, I2);
}

int	cstmCompareDwnFldrTimeUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeUpFileTypeDwn(I1, I2);
}

int	cstmCompareDwnFldrTimeDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileTypeUp(I1, I2);
}

int	cstmCompareDwnFldrTimeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTimeDwnFileTypeDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileAlphabUp(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileAlphabDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileEnum(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileEnum(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileTimeUp(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileTimeDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileTimeUp(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileTimeDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileSizeUp(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileSizeDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileSizeUp(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileSizeDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileTypeUp(I1, I2);
}

int	cstmCompareDwnFldrSizeUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeUpFileTypeDwn(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileTypeUp(I1, I2);
}

int	cstmCompareDwnFldrSizeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrSizeDwnFileTypeDwn(I1, I2);
}

int	cstmCompareDwnFldrTypeUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileAlphabUp(I1, I2);
}

int	cstmCompareDwnFldrTypeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareDwnFldrTypeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareDwnFldrTypeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileAlphabDwn(I1, I2);
}

int	cstmCompareDwnFldrTypeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileEnum(I1, I2);
}

int	cstmCompareDwnFldrTypeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileEnum(I1, I2);
}

int	cstmCompareDwnFldrTypeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileTimeUp(I1, I2);
}

int	cstmCompareDwnFldrTypeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileTimeDwn(I1, I2);
}

int	cstmCompareDwnFldrTypeDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileTimeUp(I1, I2);
}

int	cstmCompareDwnFldrTypeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileTimeDwn(I1, I2);
}

int cstmCompareDwnFldrTypeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileSizeUp(I1, I2);
}

int cstmCompareDwnFldrTypeUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileSizeDwn(I1, I2);
}

int cstmCompareDwnFldrTypeDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileSizeUp(I1, I2);
}

int cstmCompareDwnFldrTypeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileSizeDwn(I1, I2);
}

int cstmCompareDwnFldrTypeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileTypeUp(I1, I2);
}

int cstmCompareDwnFldrTypeUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeUpFileTypeDwn(I1, I2);
}

int cstmCompareDwnFldrTypeDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileTypeUp(I1, I2);
}

int cstmCompareDwnFldrTypeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareDwnFldrTypeDwnFileTypeDwn(I1, I2);
}

int cstmCompareMixFldrAlphabUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileAlphabUp(I1, I2);
}

int cstmCompareMixFldrAlphabUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileAlphabDwn(I1, I2);
}

int	cstmCompareMixFldrAlphabUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileEnum(I1, I2);
}

int	cstmCompareMixFldrAlphabDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabDwnFileEnum(I1, I2);
}

int	cstmCompareMixFldrAlphabUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileTimeUp(I1, I2);
}

int	cstmCompareMixFldrAlphabUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileTimeDwn(I1, I2);
}

int	cstmCompareMixFldrAlphabDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabDwnFileTimeUp(I1, I2);
}

int	cstmCompareMixFldrAlphabDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabDwnFileTimeDwn(I1, I2);
}

int	cstmCompareMixFldrAlphabUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileSizeUp(I1, I2);
}

int	cstmCompareMixFldrAlphabUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileSizeDwn(I1, I2);
}

int	cstmCompareMixFldrAlphabDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabDwnFileSizeUp(I1, I2);
}

int	cstmCompareMixFldrAlphabDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabDwnFileSizeDwn(I1, I2);
}

int	cstmCompareMixFldrAlphabUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileTypeUp(I1, I2);
}

int	cstmCompareMixFldrAlphabUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabUpFileTypeDwn(I1, I2);
}

int	cstmCompareMixFldrAlphabDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabDwnFileTypeUp(I1, I2);
}

int	cstmCompareMixFldrAlphabDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrAlphabDwnFileTypeDwn(I1, I2);
}

int	cstmCompareMixFldrEnumFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileTypeUp(I1, I2);
}

int	cstmCompareMixFldrEnumFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileTypeDwn(I1, I2);
}

int	cstmCompareMixFldrEnumFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileAlphabUp(I1, I2);
}

int	cstmCompareMixFldrEnumFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileAlphabDwn(I1, I2);
}

int	cstmCompareMixFldrEnumFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileEnum(I1, I2);
}

int	cstmCompareMixFldrEnumFileTimeUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileTimeUp(I1, I2);
}

int	cstmCompareMixFldrEnumFileTimeDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileTimeDwn(I1, I2);
}

int	cstmCompareMixFldrEnumFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileSizeUp(I1, I2);
}

int	cstmCompareMixFldrEnumFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrEnumFileSizeDwn(I1, I2);
}

int cstmCompareMixFldrTimeUpFileAlphabUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileAlphabUp(I1, I2);
}

int cstmCompareMixFldrTimeUpFileAlphabDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileAlphabDwn(I1, I2);
}

int cstmCompareMixFldrTimeDwnFileAlphabUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeDwnFileAlphabUp(I1, I2);
}

int cstmCompareMixFldrTimeDwnFileAlphabDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeDwnFileAlphabDwn(I1, I2);
}

int cstmCompareMixFldrTimeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileEnum(I1, I2);
}

int cstmCompareMixFldrTimeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeDwnFileEnum(I1, I2);
}

int cstmCompareMixFldrTimeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileTimeUp(I1, I2);
}

int cstmCompareMixFldrTimeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileTimeDwn(I1, I2);
}

int	cstmCompareMixFldrTimeUpFileSizeUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileSizeUp(I1, I2);
}

int	cstmCompareMixFldrTimeUpFileSizeDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileSizeDwn(I1, I2);
}

int	cstmCompareMixFldrTimeDwnFileSizeUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeDwnFileSizeUp(I1, I2);
}

int	cstmCompareMixFldrTimeDwnFileSizeDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeDwnFileSizeDwn(I1, I2);
}

int	cstmCompareMixFldrTimeUpFileTypeUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileTypeUp(I1, I2);
}

int	cstmCompareMixFldrTimeUpFileTypeDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeUpFileTypeDwn(I1, I2);
}

int	cstmCompareMixFldrTimeDwnFileTypeUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeDwnFileTypeUp(I1, I2);
}

int	cstmCompareMixFldrTimeDwnFileTypeDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTimeDwnFileTypeDwn(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileAlphabUp(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileAlphabDwn(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileEnum(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileEnum(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileTimeUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileTimeUp(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileTimeDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileTimeDwn(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileTimeUp(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileTimeUp(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileTimeDwn(const void *I1, const void *I2)
{
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileTimeDwn(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileSizeUp(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileSizeDwn(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileTypeUp(I1, I2);
}

int	cstmCompareMixFldrSizeUpFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeUpFileTypeDwn(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileTypeUp(I1, I2);
}

int	cstmCompareMixFldrSizeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrSizeDwnFileTypeDwn(I1, I2);
}

int	cstmCompareMixFldrTypeUpFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileAlphabUp(I1, I2);
}

int	cstmCompareMixFldrTypeUpFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileAlphabDwn(I1, I2);
}

int	cstmCompareMixFldrTypeDwnFileAlphabUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileAlphabUp(I1, I2);
}

int	cstmCompareMixFldrTypeDwnFileAlphabDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileAlphabDwn(I1, I2);
}

int	cstmCompareMixFldrTypeUpFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileEnum(I1, I2);
}

int	cstmCompareMixFldrTypeDwnFileEnum(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileEnum(I1, I2);
}

int cstmCompareMixFldrTypeUpFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileTimeUp(I1, I2);
}

int cstmCompareMixFldrTypeUpFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileTimeDwn(I1, I2);
}

int cstmCompareMixFldrTypeDwnFileTimeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileTimeUp(I1, I2);
}

int cstmCompareMixFldrTypeDwnFileTimeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileTimeDwn(I1, I2);
}

int cstmCompareMixFldrTypeUpFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileSizeUp(I1, I2);
}

int cstmCompareMixFldrTypeUpFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileSizeDwn(I1, I2);
}

int cstmCompareMixFldrTypeDwnFileSizeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileSizeUp(I1, I2);
}

int cstmCompareMixFldrTypeDwnFileSizeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileSizeDwn(I1, I2);
}

int cstmCompareMixFldrTypeUpFileTypeUp(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeUpFileTypeUp(I1, I2);
}

int cstmCompareMixFldrTypeDwnFileTypeDwn(const void *I1, const void *I2)
{	
	int c = cmpCustm(I1,I2);
	if(c) return c;
	return Sort::compareMixFldrTypeDwnFileTypeDwn(I1, I2);
}

#undef p1
#undef p2

} //end of namespace

using namespace cstmSort;
VOID Panel::SortWithCustomItems()
{	if(totItems<1) return;
	switch(folderAndFileMixingSortType)
	{	case foldersUp:
			switch(folderSortType)
			{	case alphabeticalUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabUpFileTypeDwn);
							break;
					}
					break;
				case alphabeticalDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrAlphabDwnFileTypeDwn);
							break;
					}
					break;
				case forEnums:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrEnumFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeUpFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTimeDwnFileTypeDwn);
							break;
					}
					break;
				case forSizeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forSizeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forTypeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeUpFileTypeDwn);
							break;
					}
					break;
				case forTypeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareUpFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
			}
			break;
		case filesUp:
			switch(folderSortType)
			{	case alphabeticalUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabUpFileTypeDwn);
							break;
					}
					break;
				case alphabeticalDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrAlphabDwnFileTypeDwn);
							break;
					}
					break;
				case forEnums:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrEnumFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeUpFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1], totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTimeDwnFileTypeDwn);
							break;
					}
					break;
				case forSizeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forSizeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrSizeDwnFileTypeDwn);
							break;
					}
					break;
				case forTypeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),cstmCompareDwnFldrTypeUpFileTypeDwn);
							break;
					}
					break;
				case forTypeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1, sizeof(PanelItem),cstmCompareDwnFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
			}
			break;
		case mixing:
			switch(folderSortType)
			{	case alphabeticalUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileTypeDwn);
							break;
					}
					break;
				case alphabeticalDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrAlphabDwnFileTypeDwn);
							break;
					}
					break;
				case forEnums:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileAlphabDwn);
							break;
						case forEnums://kerakmas;
						//myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrEnumFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileTypeDwn);
							break;
					}
					break;
				case forCreateTimeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTimeDwnFileTypeDwn);
							break;
					}
					break;
				case forSizeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileTypeDwn);
							break;
					}
					break;
				case forSizeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrSizeDwnFileTypeDwn);
							break;
					}
					break;
				case forTypeUp:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
				case forTypeDwn:
					switch(fileSortType)
					{	case alphabeticalUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileAlphabUp);
							break;
						case alphabeticalDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileAlphabDwn);
							break;
						case forEnums:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileEnum);
							break;
						case forCreateTimeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileTimeUp);
							break;
						case forCreateTimeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileTimeDwn);
							break;
						case forSizeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileSizeUp);
							break;
						case forSizeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileSizeDwn);
							break;
						case forTypeUp:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeUpFileTypeUp);
							break;
						case forTypeDwn:
						myQsort32_2(&pItems[1],totItems-1,sizeof(PanelItem),cstmCompareMixFldrTypeDwnFileTypeDwn);
							break;
					}
					break;
			}
			break;
}	}

#include <errno.h>
#define CUTOFF 8            /* testing shows that this is good value */
#define STKSIZ (8*sizeof(void*) - 2)
#define __COMPARE(context, p1, p2) comp(p1, p2)
#define __SHORTSORT(lo, hi, width, comp, context) shortsort(lo, hi, width, comp);
#define _VALIDATE_RETURN_VOID( expr, errorcode )                               \
    {                                                                          \
        int _Expr_val=!!(expr);                                                \
		Err::AssertFatal( ( _Expr_val ), (LPWSTR)(#expr) );                    \
        if ( !( _Expr_val ) )                                                  \
        {                                                                      \
            errno = errorcode;                                                 \
			Err::fatalExit((LPWSTR)(#expr),hWnd);							   \
            return;                                                            \
        }                                                                      \
    }

//Oxiridan 2ta S32->8 byte qoldiramiz, nRow va nColumnlar uchun:
#pragma message ("Qara, myQsort32_2 dagi swap 32 talik, PanelIteming 4 byte chegarasida bo'lishi shart!!!")
static void swap (char *a,char *b,size_t width)
{
    DWORD tmp;//char tmp;
	DWORD *da = (DWORD*)a;
	DWORD *db = (DWORD*)b;
	size_t dwidth = (width/4) - 2;

    if ( da != db)//a != b )
        /* Do the swap one character at a time to avoid potential alignment
           problems. */
        while ( dwidth-- )//width-8 )
		{	//width--;
            tmp = *da;//a;
            *da++ = *db;//*a++ = *b;
            *db++ = tmp;//*b++ = tmp;
}		}

static void shortsort(char *lo,char *hi,size_t width,int (*comp)(const void*, const void*))
{
    char *p, *max;

    /* Note: in assertions below, i and j are alway inside original bound of
       array to sort. */

    while (hi > lo) {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            /* A[i] <= A[max] for lo <= i < p */
            if (__COMPARE(context, p, max) > 0) {
                max = p;
            }
            /* A[i] <= A[max] for lo <= i <= p */
        }

        /* A[i] <= A[max] for lo <= i <= hi */

        swap(max, hi, width);

        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

        hi -= width;

        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }
    /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
       so array is sorted */
}

void myQsort32_2(void *base,size_t num,size_t width, int(*comp)(const void*, const void*))
{
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    size_t size;                /* size of the sub-array */
    char *lostk[STKSIZ], *histk[STKSIZ];
    int stkptr;                 /* stack for saving sub-array to be processed */

    /* validation section */
    _VALIDATE_RETURN_VOID(base != NULL || num == 0,EINVAL);
    _VALIDATE_RETURN_VOID(width > 0, EINVAL);
    _VALIDATE_RETURN_VOID(comp != NULL, EINVAL);

    if (num < 2)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char *)base;
    hi = (char *)base + width * (num-1);        /* initialize limits */

    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       preserved, locals aren't, so we preserve stuff on the stack */
recurse:

    size = (hi - lo) / width + 1;        /* number of el's to sort */

    /* below a certain size, it is faster to use a O(n^2) sorting method */
    if (size <= CUTOFF) {
        __SHORTSORT(lo, hi, width, comp, context);
    }
    else {
        /* First we pick a partitioning element.  The efficiency of the
           algorithm demands that we find one that is approximately the median
           of the values, but also that we select one fast.  We choose the
           median of the first, middle, and last elements, to avoid bad
           performance in the face of already sorted data, or data that is made
           up of multiple sorted runs appended together.  Testing shows that a
           median-of-three algorithm provides better performance than simply
           picking the middle element for the latter case. */

        mid = lo + (size / 2) * width;      /* find middle element */

        /* Sort the first, middle, last elements into order */
        if (__COMPARE(context, lo, mid) > 0) {
            swap(lo, mid, width);
        }
        if (__COMPARE(context, lo, hi) > 0) {
            swap(lo, hi, width);
        }
        if (__COMPARE(context, mid, hi) > 0) {
            swap(mid, hi, width);
        }

        /* We now wish to partition the array into three pieces, one consisting
           of elements <= partition element, one of elements equal to the
           partition element, and one of elements > than it.  This is done
           below; comments indicate conditions established at every step. */

        loguy = lo;
        higuy = hi;

        /* Note that higuy decreases and loguy increases on every iteration,
           so loop must terminate. */
        for (;;) {
            /* lo <= loguy < hi, lo < higuy <= hi,
               A[i] <= A[mid] for lo <= i <= loguy,
               A[i] > A[mid] for higuy <= i < hi,
               A[hi] >= A[mid] */

            /* The doubled loop is to avoid calling comp(mid,mid), since some
               existing comparison funcs don't work when passed the same
               value for both pointers. */

            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && __COMPARE(context, loguy, mid) <= 0);
            }
            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && __COMPARE(context, loguy, mid) <= 0);
            }

            /* lo < loguy <= hi+1, A[i] <= A[mid] for lo <= i < loguy,
               either loguy > hi or A[loguy] > A[mid] */

            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid) > 0);

            /* lo <= higuy < hi, A[i] > A[mid] for higuy < i < hi,
               either higuy == lo or A[higuy] <= A[mid] */

            if (higuy < loguy)
                break;

            /* if loguy > hi or higuy == lo, then we would have exited, so
               A[loguy] > A[mid], A[higuy] <= A[mid],
               loguy <= hi, higuy > lo */

            swap(loguy, higuy, width);

            /* If the partition element was moved, follow it.  Only need
               to check for mid == higuy, since before the swap,
               A[loguy] > A[mid] implies loguy != mid. */

            if (mid == higuy)
                mid = loguy;

            /* A[loguy] <= A[mid], A[higuy] > A[mid]; so condition at top
               of loop is re-established */
        }

        /*     A[i] <= A[mid] for lo <= i < loguy,
               A[i] > A[mid] for higuy < i < hi,
               A[hi] >= A[mid]
               higuy < loguy
           implying:
               higuy == loguy-1
               or higuy == hi - 1, loguy == hi + 1, A[hi] == A[mid] */

        /* Find adjacent elements equal to the partition element.  The
           doubled loop is to avoid calling comp(mid,mid), since some
           existing comparison funcs don't work when passed the same value
           for both pointers. */

        higuy += width;
        if (mid < higuy) {
            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && __COMPARE(context, higuy, mid) == 0);
        }

        /* OK, now we have the following:
              higuy < loguy
              lo <= higuy <= hi
              A[i]  <= A[mid] for lo <= i <= higuy
              A[i]  == A[mid] for higuy < i < loguy
              A[i]  >  A[mid] for loguy <= i < hi
              A[hi] >= A[mid] */

        /* We've finished the partition, now we want to sort the subarrays
           [lo, higuy] and [loguy, hi].
           We do the smaller one first to minimize stack usage.
           We only sort arrays of length 2 or more.*/

        if ( higuy - lo >= hi - loguy ) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo < higuy) {
                hi = higuy;
                goto recurse;           /* do small recursion */
            }
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return;                 /* all subarrays done */
}

#undef __fileDECL
#undef __COMPARE
#undef __SHORTSORT
#undef swap
