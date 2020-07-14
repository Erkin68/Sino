#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include "windows.h"




namespace registry
{
	extern wchar_t** GetFileOpenWithAssociation(wchar_t*);
	extern BOOL	IsCLSIDExistsA(TCHAR*);
	extern BOOL	IsCLSIDExists(wchar_t*);
}

#endif
