#ifndef _MATH_H
#define _MATH_H


#include "windows.h"


extern VOID SetSSELib();
extern BOOL IsSSESupport();

extern VOID (*Math_ReplaceDW) (DWORD*, DWORD*, int);
extern VOID Math_ReplaceDW_C  (DWORD*, DWORD*, int);
extern "C"{
VOID Math_ReplaceDW_SSE(DWORD*, DWORD*, int);
}



#endif