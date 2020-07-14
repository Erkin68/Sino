#ifndef _MyStrCmpNN_SSE_H_
#define _MyStrCmpNN_SSE_H_

/*namespace NMyStrCmpSSE
{

typedef char* (*TMyStrCmpN)(char*,char*,int);
extern TMyStrCmpN MyStrCmpN[100];

extern char* MyStrCmpN1(char*,char*,int);
extern char* MyStrCmpN2(char*,char*,int);
extern char* MyStrCmpN3(char*,char*,int);
extern char* MyStrCmpN4(char*,char*,int);
extern char* MyStrCmpN17(char*,char*,int);
extern char* MyStrCmpN18(char*,char*,int);
extern char* MyStrCmpN19(char*,char*,int);
extern char* MyStrCmpN20(char*,char*,int);
extern char* MyStrCmpN21(char*,char*,int);
extern char* MyStrCmpN22(char*,char*,int);
extern char* MyStrCmpN23(char*,char*,int);
extern char* MyStrCmpN24(char*,char*,int);
extern char* MyStrCmpN25(char*,char*,int);
extern char* MyStrCmpN26(char*,char*,int);
extern char* MyStrCmpN27(char*,char*,int);
extern char* MyStrCmpN28(char*,char*,int);
extern char* MyStrCmpN29(char*,char*,int);
extern char* MyStrCmpN30(char*,char*,int);
extern char* MyStrCmpN31(char*,char*,int);
extern char* MyStrCmpN33(char*,char*,int);
extern char* MyStrCmpN34(char*,char*,int);
extern char* MyStrCmpN35(char*,char*,int);
extern char* MyStrCmpN36(char*,char*,int);
extern char* MyStrCmpN37(char*,char*,int);
extern char* MyStrCmpN38(char*,char*,int);
extern char* MyStrCmpN39(char*,char*,int);
extern char* MyStrCmpN40(char*,char*,int);
extern char* MyStrCmpN41(char*,char*,int);
extern char* MyStrCmpN42(char*,char*,int);
extern char* MyStrCmpN43(char*,char*,int);
extern char* MyStrCmpN44(char*,char*,int);
extern char* MyStrCmpN45(char*,char*,int);
extern char* MyStrCmpN46(char*,char*,int);
extern char* MyStrCmpN47(char*,char*,int);
extern char* MyStrCmpN49(char*,char*,int);
extern char* MyStrCmpN50(char*,char*,int);
extern char* MyStrCmpN51(char*,char*,int);
extern char* MyStrCmpN52(char*,char*,int);
extern char* MyStrCmpN53(char*,char*,int);
extern char* MyStrCmpN54(char*,char*,int);
extern char* MyStrCmpN55(char*,char*,int);
extern char* MyStrCmpN56(char*,char*,int);
extern char* MyStrCmpN57(char*,char*,int);
extern char* MyStrCmpN58(char*,char*,int);
extern char* MyStrCmpN59(char*,char*,int);
extern char* MyStrCmpN60(char*,char*,int);
extern char* MyStrCmpN61(char*,char*,int);
extern char* MyStrCmpN62(char*,char*,int);
extern char* MyStrCmpN63(char*,char*,int);
extern char* MyStrCmpN65(char*,char*,int);
extern char* MyStrCmpN66(char*,char*,int);
extern char* MyStrCmpN67(char*,char*,int);
extern char* MyStrCmpN68(char*,char*,int);
extern char* MyStrCmpN69(char*,char*,int);
extern char* MyStrCmpN70(char*,char*,int);
extern char* MyStrCmpN71(char*,char*,int);
extern char* MyStrCmpN72(char*,char*,int);
extern char* MyStrCmpN73(char*,char*,int);
extern char* MyStrCmpN74(char*,char*,int);
extern char* MyStrCmpN75(char*,char*,int);
extern char* MyStrCmpN76(char*,char*,int);
extern char* MyStrCmpN77(char*,char*,int);
extern char* MyStrCmpN78(char*,char*,int);
extern char* MyStrCmpN79(char*,char*,int);
extern char* MyStrCmpN81(char*,char*,int);
extern char* MyStrCmpN82(char*,char*,int);
extern char* MyStrCmpN83(char*,char*,int);
extern char* MyStrCmpN84(char*,char*,int);
extern char* MyStrCmpN85(char*,char*,int);
extern char* MyStrCmpN86(char*,char*,int);
extern char* MyStrCmpN87(char*,char*,int);
extern char* MyStrCmpN88(char*,char*,int);
extern char* MyStrCmpN89(char*,char*,int);
extern char* MyStrCmpN90(char*,char*,int);
extern char* MyStrCmpN91(char*,char*,int);
extern char* MyStrCmpN92(char*,char*,int);
extern char* MyStrCmpN93(char*,char*,int);
extern char* MyStrCmpN94(char*,char*,int);
extern char* MyStrCmpN95(char*,char*,int);
extern char* MyStrCmpN97(char*,char*,int);
extern char* MyStrCmpN98(char*,char*,int);
extern char* MyStrCmpN99(char*,char*,int);
extern char* MyStrCmpN100(char*,char*,int);



extern char* MyStrCmpN5_C(char*,char*,int);
extern char* MyStrCmpN6_C(char*,char*,int);
extern char* MyStrCmpN7_C(char*,char*,int);
extern char* MyStrCmpN8_C(char*,char*,int);
extern char* MyStrCmpN9_C(char*,char*,int);
extern char* MyStrCmpN10_C(char*,char*,int);
extern char* MyStrCmpN11_C(char*,char*,int);
extern char* MyStrCmpN12_C(char*,char*,int);
extern char* MyStrCmpN13_C(char*,char*,int);
extern char* MyStrCmpN14_C(char*,char*,int);
extern char* MyStrCmpN15_C(char*,char*,int);
extern char* MyStrCmpN16_C(char*,char*,int);
extern char* MyStrCmpN32_C(char*,char*,int);
extern char* MyStrCmpN48_C(char*,char*,int);
extern char* MyStrCmpN64_C(char*,char*,int);
extern char* MyStrCmpN80_C(char*,char*,int);
extern char* MyStrCmpN96_C(char*,char*,int);


extern "C"
{
 char* MyStrCmpN5_SSE(char*,char*,int);
 char* MyStrCmpN6_SSE(char*,char*,int);
 char* MyStrCmpN7_SSE(char*,char*,int);
 char* MyStrCmpN8_SSE(char*,char*,int);
 char* MyStrCmpN9_SSE(char*,char*,int);
 char* MyStrCmpN10_SSE(char*,char*,int);
 char* MyStrCmpN11_SSE(char*,char*,int);
 char* MyStrCmpN12_SSE(char*,char*,int);
 char* MyStrCmpN13_SSE(char*,char*,int);
 char* MyStrCmpN14_SSE(char*,char*,int);
 char* MyStrCmpN15_SSE(char*,char*,int);
 char* MyStrCmpN16_SSE(char*,char*,int);
 char* MyStrCmpN32_SSE(char*,char*,int);
 char* MyStrCmpN48_SSE(char*,char*,int);
 char* MyStrCmpN64_SSE(char*,char*,int);
 char* MyStrCmpN80_SSE(char*,char*,int);
 char* MyStrCmpN96_SSE(char*,char*,int);
}
}//namespace NMyStrCmpSSE;*/

//#define MyStrCmpNN(dest, src, N, n) NMyStrCmpSSE::MyStrCmpN##n(dest,src,N)
//#define MyStrCmpNN(dest, src, N, n) NMyStrCmpSSE::MyStrCmpN[n-1](dest,src,((int)(N))/n);
char* MyStrCmpNNA(char*,char*,int,int);
wchar_t* MyStrCmpNNW(wchar_t*,wchar_t*,int,int);

/*BOOL MyStrCmpNN(char *str0, char *str1, int N, int n)
{
	register int dw_n = n >> 2;// / 4;
	register int rem_n  = n & 0x0003;// % 4;
	register int dwF = (N - n + 1) >> 2;// / 4;

	DWORD *p1 = (DWORD*)str1;

for(int s=0; s<4; s++)
{	DWORD *p0 = (DWORD*)str0;
	for(register int f=0; f<dwF; f++)
	{	for(register int fdw=0; fdw<dw_n; fdw++)
		{	if(p0[fdw] ^ p1[fdw])//(p0[dw] != p1[dw])
				goto Cont;//return FALSE;
		}
		for(register int i=n-rem_n; i<n; i++)
		{	if(str0[f*4+i] ^ str1[i])//(str0[i] != str1[i])
				goto Cont;//return FALSE;
		}
		return TRUE;
	 Cont:
		p0++;// = (DWORD*)(++str0);
	}
	str0++;
}

//	in int64 size calculate, very small effect:
/*	register int qw_n = n >> 3;// / 8;
	register int rem_n  = n & 0x0007;// % 8;
	register int qwF = (N - n + 1) >> 3;// / 8;

	unsigned __int64 *p1 = (unsigned __int64*)str1;

for(int s=0; s<8; s++)
{	unsigned __int64 *p0 = (unsigned __int64 *)str0;
	for(register int f=0; f<qwF; f++)
	{	for(register int fqw=0; fqw<qw_n; fqw++)
		{	if(p0[fqw] ^ p1[fqw])
				goto Cont;
		}
		for(register int i=n-rem_n; i<n; i++)
		{	if(str0[f*8+i] ^ str1[i])
				goto Cont;
		}
		return TRUE;
	 Cont:
		p0++;
	}
	str0++;
}*/


/*	return FALSE;
}*/



#endif