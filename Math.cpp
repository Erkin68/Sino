#include "Math.h"
#include "Asm\MyStrCmpNN_SSE.h"


BOOL IsSSESupport()
{
	return IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE)>0;
/*	__try
	{ 	__asm	
		{ 
			xorps xmm0, xmm0 ; executing SSE instruction 
	}	} 
	__except (EXCEPTION_EXECUTE_HANDLER) 
	{ 	if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) 
		{ 
			return FALSE;
	}	} 
	return TRUE;*/
}

VOID __cdecl SetSSELib()
{
/*	Math_ReplaceDW = Math_ReplaceDW_SSE;
/*	NMyStrCmpSSE::MyStrCmpN[4]=NMyStrCmpSSE::MyStrCmpN5_SSE;
	NMyStrCmpSSE::MyStrCmpN[5]=NMyStrCmpSSE::MyStrCmpN6_SSE;
	NMyStrCmpSSE::MyStrCmpN[6]=NMyStrCmpSSE::MyStrCmpN7_SSE;
	NMyStrCmpSSE::MyStrCmpN[7]=NMyStrCmpSSE::MyStrCmpN8_SSE;
	NMyStrCmpSSE::MyStrCmpN[8]=NMyStrCmpSSE::MyStrCmpN9_SSE;
	NMyStrCmpSSE::MyStrCmpN[9]=NMyStrCmpSSE::MyStrCmpN10_SSE;
	NMyStrCmpSSE::MyStrCmpN[10]=NMyStrCmpSSE::MyStrCmpN11_SSE;
	NMyStrCmpSSE::MyStrCmpN[11]=NMyStrCmpSSE::MyStrCmpN12_SSE;
	NMyStrCmpSSE::MyStrCmpN[12]=NMyStrCmpSSE::MyStrCmpN13_SSE;
	NMyStrCmpSSE::MyStrCmpN[13]=NMyStrCmpSSE::MyStrCmpN14_SSE;
	NMyStrCmpSSE::MyStrCmpN[14]=NMyStrCmpSSE::MyStrCmpN15_SSE;
	NMyStrCmpSSE::MyStrCmpN[15]=NMyStrCmpSSE::MyStrCmpN16_SSE;
	NMyStrCmpSSE::MyStrCmpN[31]=NMyStrCmpSSE::MyStrCmpN32_SSE;
	NMyStrCmpSSE::MyStrCmpN[47]=NMyStrCmpSSE::MyStrCmpN48_SSE;
	NMyStrCmpSSE::MyStrCmpN[63]=NMyStrCmpSSE::MyStrCmpN64_SSE;
	NMyStrCmpSSE::MyStrCmpN[79]=NMyStrCmpSSE::MyStrCmpN80_SSE;
	NMyStrCmpSSE::MyStrCmpN[95]=NMyStrCmpSSE::MyStrCmpN96_SSE;*/
}

VOID Math_ReplaceDW_C(DWORD* s1, DWORD* s2, int cnt)
{
	for(register int d=0; d<cnt; d++)
	{	register DWORD r;
		r = s1[d];
		s1[d] = s2[d];
		s2[d] = r;
}	}
//VOID (*Math_ReplaceDW)(DWORD*, DWORD*, int) = Math_ReplaceDW_C;

/*VOID Math_ReplaceDW_SSE(DWORD* s1, DWORD* s2, int cnt)
{
	__asm
	{	mov			eax,		s1
		mov         ebx,		s2
		mov			ecx,		cnt
		shr			ecx,		2
L1:
		movups      xmm0,		[eax]
		movups      xmm1,		[ebx]
		movups      [eax],		xmm1
		movups      [ebx],		xmm0
		add			eax	 ,		16
		add			ebx  ,		16
		loop		L1
}	}*/