#include "MyStrCmpNN_SSE.h"


/*namespace NMyStrCmpSSE
{

char* MyStrCmpN5_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<5; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*5);
		if(!((*p0) ^ (*p1)))
		{	if(!(dest[f*5+4] ^ src[4]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN6_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<6; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*6);
		if(!((*p0) ^ (*p1)))
		{	if(!(dest[f*6+4] ^ src[4]))
			if(!(dest[f*6+5] ^ src[5]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN7_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<7; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*7);
		if(!((*p0) ^ (*p1)))
		{	if(!(dest[f*7+4] ^ src[4]))
			if(!(dest[f*7+5] ^ src[5]))
			if(!(dest[f*7+6] ^ src[6]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN8_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<8; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*8);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN9_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<9; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*9);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		{	if(!(dest[f*9+8] ^ src[8]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN10_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<10; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*10);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		{	if(!(dest[f*10+9] ^ src[9]))
			if(!(dest[f*10+10] ^ src[10]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN11_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<11; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*11);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		{	if(!(dest[f*11+9] ^ src[9]))
			if(!(dest[f*11+10] ^ src[10]))
			if(!(dest[f*11+11] ^ src[11]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN12_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<12; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*12);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN13_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<13; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*13);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		{	if(!(dest[f*13+13] ^ src[13]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN14_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<13; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*14);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		{	if(!(dest[f*14+13] ^ src[13]))
			if(!(dest[f*14+14] ^ src[14]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN15_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<15; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*15);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		{	if(!(dest[f*15+13] ^ src[13]))
			if(!(dest[f*15+14] ^ src[14]))
			if(!(dest[f*15+15] ^ src[15]))
				return (char*)p0;
	}	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN16_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<16; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*16);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		if(!((*(p0+3)) ^ (*(p1+3))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN32_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<32; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*32);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		if(!((*(p0+3)) ^ (*(p1+3))))

		if(!((*(p0+5)) ^ (*(p1+5))))
		if(!((*(p0+6)) ^ (*(p1+6))))
		if(!((*(p0+7)) ^ (*(p1+7))))
		if(!((*(p0+8)) ^ (*(p1+8))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN48_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<48; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*48);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		if(!((*(p0+3)) ^ (*(p1+3))))

		if(!((*(p0+5)) ^ (*(p1+5))))
		if(!((*(p0+6)) ^ (*(p1+6))))
		if(!((*(p0+7)) ^ (*(p1+7))))
		if(!((*(p0+8)) ^ (*(p1+8))))

		if(!((*(p0+9)) ^ (*(p1+9))))
		if(!((*(p0+10)) ^ (*(p1+10))))
		if(!((*(p0+11)) ^ (*(p1+11))))
		if(!((*(p0+12)) ^ (*(p1+12))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN64_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<64; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*64);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		if(!((*(p0+3)) ^ (*(p1+3))))

		if(!((*(p0+5)) ^ (*(p1+5))))
		if(!((*(p0+6)) ^ (*(p1+6))))
		if(!((*(p0+7)) ^ (*(p1+7))))
		if(!((*(p0+8)) ^ (*(p1+8))))

		if(!((*(p0+9)) ^ (*(p1+9))))
		if(!((*(p0+10)) ^ (*(p1+10))))
		if(!((*(p0+11)) ^ (*(p1+11))))
		if(!((*(p0+12)) ^ (*(p1+12))))

		if(!((*(p0+13)) ^ (*(p1+13))))
		if(!((*(p0+14)) ^ (*(p1+14))))
		if(!((*(p0+15)) ^ (*(p1+15))))
		if(!((*(p0+16)) ^ (*(p1+16))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN80_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<80; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*80);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		if(!((*(p0+3)) ^ (*(p1+3))))

		if(!((*(p0+5)) ^ (*(p1+5))))
		if(!((*(p0+6)) ^ (*(p1+6))))
		if(!((*(p0+7)) ^ (*(p1+7))))
		if(!((*(p0+8)) ^ (*(p1+8))))

		if(!((*(p0+9)) ^ (*(p1+9))))
		if(!((*(p0+10)) ^ (*(p1+10))))
		if(!((*(p0+11)) ^ (*(p1+11))))
		if(!((*(p0+12)) ^ (*(p1+12))))

		if(!((*(p0+13)) ^ (*(p1+13))))
		if(!((*(p0+14)) ^ (*(p1+14))))
		if(!((*(p0+15)) ^ (*(p1+15))))
		if(!((*(p0+16)) ^ (*(p1+16))))

		if(!((*(p0+17)) ^ (*(p1+17))))
		if(!((*(p0+18)) ^ (*(p1+18))))
		if(!((*(p0+19)) ^ (*(p1+19))))
		if(!((*(p0+20)) ^ (*(p1+20))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN96_C(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<96; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = (unsigned __int32*)(dest+f*96);
		if(!((*p0) ^ (*p1)))
		if(!((*(p0+1)) ^ (*(p1+1))))
		if(!((*(p0+2)) ^ (*(p1+2))))
		if(!((*(p0+3)) ^ (*(p1+3))))

		if(!((*(p0+5)) ^ (*(p1+5))))
		if(!((*(p0+6)) ^ (*(p1+6))))
		if(!((*(p0+7)) ^ (*(p1+7))))
		if(!((*(p0+8)) ^ (*(p1+8))))

		if(!((*(p0+9)) ^ (*(p1+9))))
		if(!((*(p0+10)) ^ (*(p1+10))))
		if(!((*(p0+11)) ^ (*(p1+11))))
		if(!((*(p0+12)) ^ (*(p1+12))))

		if(!((*(p0+13)) ^ (*(p1+13))))
		if(!((*(p0+14)) ^ (*(p1+14))))
		if(!((*(p0+15)) ^ (*(p1+15))))
		if(!((*(p0+16)) ^ (*(p1+16))))

		if(!((*(p0+17)) ^ (*(p1+17))))
		if(!((*(p0+18)) ^ (*(p1+18))))
		if(!((*(p0+19)) ^ (*(p1+19))))
		if(!((*(p0+20)) ^ (*(p1+20))))

		if(!((*(p0+21)) ^ (*(p1+21))))
		if(!((*(p0+22)) ^ (*(p1+22))))
		if(!((*(p0+23)) ^ (*(p1+23))))
		if(!((*(p0+24)) ^ (*(p1+24))))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN1(char* dest,char* src,int cnt)
{
 for(int f=0; f<cnt; f++)
 {	if(!((*dest) ^ (*src)))
		return (char*)dest;
	dest++;
 }
 return 0;
}

char* MyStrCmpN2(char* dest,char* src,int cnt)
{
 unsigned __int16 *p1 = (unsigned __int16*)src;
 for(int s=0; s<2; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int16* p0 = ((unsigned __int16*)dest)+cnt;
		if(!((*p0) ^ (*p1)))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN3(char* dest,char* src,int cnt)
{
 unsigned __int16 *p1 = (unsigned __int16*)src;
 for(int s=0; s<3; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int16* p0 = (unsigned __int16*)(dest+f*3);
		if(!((*p0) ^ (*p1)))
		if(!(dest[f*3+3] ^ src[3]))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN4(char* dest,char* src,int cnt)
{
 unsigned __int32 *p1 = (unsigned __int32*)src;
 for(int s=0; s<4; s++)
 {	for(int f=0; f<cnt; f++)
	{	unsigned __int32* p0 = ((unsigned __int32*)dest)+cnt;
		if(!((*p0) ^ (*p1)))
			return (char*)p0;
	}
	dest++;
 }
 return 0;
}

char* MyStrCmpN17(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*17 - 16;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);//16
	if(!r)
		return r;
	if(r[16] == src[16])
		return r;
	r += 16;
	Cnt = (end - r) / 17;
 }
 return 0;
}

char* MyStrCmpN18(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*18 - 17;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);//16
	if(!r)
		return r;
	if(MyStrCmpN[1](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 18;
 }
 return 0;
}

char* MyStrCmpN19(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*19 - 18;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[2](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 19;
 }
 return 0;
}

char* MyStrCmpN20(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*20 - 19;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[3](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 20;
 }
 return 0;
}

char* MyStrCmpN21(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*21 - 20;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[5](r+16,src,1))//5
		return r;
	r += 16;
	Cnt = (end - r) / 21;
 }
 return 0;
}

char* MyStrCmpN22(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*22 - 21;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[6](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 22;
 }
 return 0;
}

char* MyStrCmpN23(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*23 - 22;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[7](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 23;
 }
 return 0;
}

char* MyStrCmpN24(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*24 - 23;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[8](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 24;
 }
 return 0;
}

char* MyStrCmpN25(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*25 - 24;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[9](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 25;
 }
 return 0;
}

char* MyStrCmpN26(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*26 - 25;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[10](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 26;
 }
 return 0;
}

char* MyStrCmpN27(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*27 - 26;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[11](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 27;
 }
 return 0;
}

char* MyStrCmpN28(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*28 - 27;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[12](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 28;
 }
 return 0;
}

char* MyStrCmpN29(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*29 - 28;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[13](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 29;
 }
 return 0;
}

char* MyStrCmpN30(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*30 - 29;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[14](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 30;
 }
 return 0;
}

char* MyStrCmpN31(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*31 - 30;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[15](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[15](r+16,src,1))
		return r;
	r += 16;
	Cnt = (end - r) / 31;
 }
 return 0;
}

char* MyStrCmpN33(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*33 - 32;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(r[32] == src[32])
		return r;
	r += 32;
	Cnt = (end - r) / 33;
 }
 return 0;
}

char* MyStrCmpN34(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*33 - 32;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[1](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 34;
 }
 return 0;
}

char* MyStrCmpN35(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*35 - 34;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[2](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 35;
 }
 return 0;
}

char* MyStrCmpN36(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*36 - 35;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[3](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 36;
 }
 return 0;
}

char* MyStrCmpN37(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*37 - 36;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[4](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 37;
 }
 return 0;
}

char* MyStrCmpN38(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*38 - 37;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[5](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 38;
 }
 return 0;
}

char* MyStrCmpN39(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*39 - 38;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[6](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 39;
 }
 return 0;
}

char* MyStrCmpN40(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*40 - 39;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[7](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 40;
 }
 return 0;
}

char* MyStrCmpN41(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*41 - 40;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[8](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 41;
 }
 return 0;
}

char* MyStrCmpN42(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*42 - 41;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[9](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 42;
 }
 return 0;
}

char* MyStrCmpN43(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*43 - 42;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[10](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 43;
 }
 return 0;
}

char* MyStrCmpN44(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*44 - 43;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[11](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 44;
 }
 return 0;
}

char* MyStrCmpN45(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*45 - 44;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[12](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 45;
 }
 return 0;
}

char* MyStrCmpN46(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*46 - 45;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[13](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 46;
 }
 return 0;
}

char* MyStrCmpN47(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*47 - 46;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[31](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[14](r+32,src,1))
		return r;
	r += 32;
	Cnt = (end - r) / 47;
 }
 return 0;
}

char* MyStrCmpN49(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*49 - 48;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(r[48] == src[48])
		return r;
	r += 48;
	Cnt = (end - r) / 49;
 }
 return 0;
}

char* MyStrCmpN50(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*50 - 49;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[1](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 50;
 }
 return 0;
}

char* MyStrCmpN51(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*51 - 50;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[2](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 51;
 }
 return 0;
}

char* MyStrCmpN52(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*52 - 51;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[3](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 52;
 }
 return 0;
}

char* MyStrCmpN53(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*53 - 52;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[4](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 53;
 }
 return 0;
}

char* MyStrCmpN54(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*54 - 53;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[5](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 54;
 }
 return 0;
}

char* MyStrCmpN55(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*55 - 54;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[6](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 55;
 }
 return 0;
}

char* MyStrCmpN56(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*56 - 55;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[7](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 56;
 }
 return 0;
}

char* MyStrCmpN57(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*57 - 56;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[8](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 57;
 }
 return 0;
}

char* MyStrCmpN58(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*58 - 57;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[9](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 58;
 }
 return 0;
}

char* MyStrCmpN59(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*59 - 58;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[10](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 59;
 }
 return 0;
}

char* MyStrCmpN60(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*60 - 59;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[11](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 60;
 }
 return 0;
}

char* MyStrCmpN61(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*61 - 60;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[12](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 61;
 }
 return 0;
}

char* MyStrCmpN62(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*62 - 61;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[13](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 62;
 }
 return 0;
}

char* MyStrCmpN63(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*63 - 62;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[47](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[14](r+48,src,1))
		return r;
	r += 48;
	Cnt = (end - r) / 63;
 }
 return 0;
}

char* MyStrCmpN65(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*65 - 64;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	r += 64;
	if(r[64] == src[64])
		return r;
	Cnt = (end - r) / 65;
 }
 return 0;
}

char* MyStrCmpN66(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*66 - 65;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[1](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 66;
 }
 return 0;
}

char* MyStrCmpN67(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*67 - 66;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[2](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 67;
 }
 return 0;
}

char* MyStrCmpN68(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*68 - 67;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[3](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 68;
 }
 return 0;
}

char* MyStrCmpN69(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*69 - 68;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[4](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 69;
 }
 return 0;
}

char* MyStrCmpN70(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*70 - 69;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[5](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 70;
 }
 return 0;
}

char* MyStrCmpN71(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*71 - 70;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[6](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 71;
 }
 return 0;
}

char* MyStrCmpN72(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*72 - 71;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[7](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 72;
 }
 return 0;
}

char* MyStrCmpN73(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*73 - 72;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[8](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 73;
 }
 return 0;
}

char* MyStrCmpN74(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*74 - 73;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[9](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 74;
 }
 return 0;
}

char* MyStrCmpN75(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*75 - 74;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[10](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 75;
 }
 return 0;
}

char* MyStrCmpN76(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*76 - 75;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[11](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 76;
 }
 return 0;
}

char* MyStrCmpN77(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*77 - 76;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[12](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 77;
 }
 return 0;
}

char* MyStrCmpN78(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*78 - 77;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[13](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 78;
 }
 return 0;
}

char* MyStrCmpN79(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*79 - 78;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[63](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[14](r+64,src,1))
		return r;
	r += 64;
	Cnt = (end - r) / 79;
 }
 return 0;
}

char* MyStrCmpN81(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*81 - 80;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(r[80] == src[80])
		return r;
	r += 80;
	Cnt = (end - r) / 81;
 }
 return 0;
}

char* MyStrCmpN82(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*82 - 81;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[1](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 82;
 }
 return 0;
}

char* MyStrCmpN83(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*83 - 82;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[2](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 83;
 }
 return 0;
}

char* MyStrCmpN84(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*84 - 83;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[3](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 84;
 }
 return 0;
}

char* MyStrCmpN85(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*85 - 84;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[4](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 85;
 }
 return 0;
}

char* MyStrCmpN86(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*86 - 85;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[5](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 86;
 }
 return 0;
}

char* MyStrCmpN87(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*87 - 86;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[6](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 87;
 }
 return 0;
}

char* MyStrCmpN88(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*88 - 86;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[7](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 88;
 }
 return 0;
}

char* MyStrCmpN89(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*89 - 88;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[8](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 89;
 }
 return 0;
}

char* MyStrCmpN90(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*90 - 89;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[9](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 90;
 }
 return 0;
}

char* MyStrCmpN91(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*91 - 90;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[10](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 91;
 }
 return 0;
}

char* MyStrCmpN92(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*92 - 91;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[11](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 92;
 }
 return 0;
}

char* MyStrCmpN93(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*93 - 92;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[12](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 92;
 }
 return 0;
}

char* MyStrCmpN94(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*94 - 93;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[13](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 94;
 }
 return 0;
}

char* MyStrCmpN95(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*95 - 94;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[79](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[14](r+80,src,1))
		return r;
	r += 80;
	Cnt = (end - r) / 95;
 }
 return 0;
}

char* MyStrCmpN97(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*97 - 96;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[95](r,src,Cnt);
	if(!r)
		return r;
	if(r[96] == src[96])
		return r;
	r += 96;
	Cnt = (end - r) / 97;
 }
 return 0;
}

char* MyStrCmpN98(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*98 - 97;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[95](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[1](r+96,src,1))
		return r;
	r += 96;
	Cnt = (end - r) / 98;
 }
 return 0;
}

char* MyStrCmpN99(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*99 - 98;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[95](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[2](r+96,src,1))
		return r;
	r += 96;
	Cnt = (end - r) / 99;
 }
 return 0;
}

char* MyStrCmpN100(char* dest,char* src,int cnt)
{
 char* r = 	dest;
 char* end = dest + cnt*100 - 99;
 int Cnt = cnt;

 while(r < end)
 {	r = MyStrCmpN[95](r,src,Cnt);
	if(!r)
		return r;
	if(MyStrCmpN[3](r+96,src,1))
		return r;
	r += 96;
	Cnt = (end - r) / 100;
 }
 return 0;
}

TMyStrCmpN MyStrCmpN[100] = {
MyStrCmpN1,MyStrCmpN2,MyStrCmpN3,MyStrCmpN4,MyStrCmpN5_C,
MyStrCmpN6_C,MyStrCmpN7_C,MyStrCmpN8_C,MyStrCmpN9_C,MyStrCmpN10_C,
MyStrCmpN11_C,MyStrCmpN12_C,MyStrCmpN13_C,MyStrCmpN14_C,MyStrCmpN15_C,
MyStrCmpN16_C,MyStrCmpN17,MyStrCmpN18,MyStrCmpN19,MyStrCmpN20,
MyStrCmpN21,MyStrCmpN22,MyStrCmpN23,MyStrCmpN24,MyStrCmpN25,
MyStrCmpN26,MyStrCmpN27,MyStrCmpN28,MyStrCmpN29,MyStrCmpN30,
MyStrCmpN31,MyStrCmpN32_C,MyStrCmpN33,MyStrCmpN34,MyStrCmpN35,
MyStrCmpN36,MyStrCmpN37,MyStrCmpN38,MyStrCmpN39,MyStrCmpN40,
MyStrCmpN41,MyStrCmpN42,MyStrCmpN43,MyStrCmpN44,MyStrCmpN45,
MyStrCmpN46,MyStrCmpN47,MyStrCmpN48_C,MyStrCmpN49,MyStrCmpN50,
MyStrCmpN51,MyStrCmpN52,MyStrCmpN53,MyStrCmpN54,MyStrCmpN55,
MyStrCmpN56,MyStrCmpN57,MyStrCmpN58,MyStrCmpN59,MyStrCmpN60,
MyStrCmpN61,MyStrCmpN62,MyStrCmpN63,MyStrCmpN64_C,MyStrCmpN65,
MyStrCmpN66,MyStrCmpN67,MyStrCmpN68,MyStrCmpN69,MyStrCmpN70,
MyStrCmpN71,MyStrCmpN72,MyStrCmpN73,MyStrCmpN74,MyStrCmpN75,
MyStrCmpN76,MyStrCmpN77,MyStrCmpN78,MyStrCmpN79,MyStrCmpN80_C,
MyStrCmpN81,MyStrCmpN82,MyStrCmpN83,MyStrCmpN84,MyStrCmpN85,
MyStrCmpN86,MyStrCmpN87,MyStrCmpN88,MyStrCmpN89,MyStrCmpN90,
MyStrCmpN91,MyStrCmpN92,MyStrCmpN93,MyStrCmpN94,MyStrCmpN95,
MyStrCmpN96_C,MyStrCmpN97,MyStrCmpN98,MyStrCmpN99,MyStrCmpN100
							};


}//namespace NMyStrCmpSSE*/


char* MyStrCmpNNA(char *str0, char *str1, int N, int n)
{
	register int dw_n = n >> 2;// / 4;
	register int rem_n  = n & 0x0003;// % 4;
	register int dwF = (N - n + 1) >> 2;// / 4;

	unsigned __int32 *p1 = (unsigned __int32*)str1;

	for(int s=0; s<4; s++)
	{	unsigned __int32 *p0 = (unsigned __int32*)str0;
		for(register int f=0; f<dwF; f++)
		{	for(register int fdw=0; fdw<dw_n; fdw++)
			{	if(p0[fdw] != p1[fdw])//(p0[dw] ^ p1[dw])
					goto Cont;//return FALSE;
			}
			for(register int i=n-rem_n; i<n; i++)
			{	if(str0[f*4+i] != str1[i])//(str0[i] ^ str1[i])
					goto Cont;//return FALSE;
			}
			return (char*)p0;
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
		return (char*)p0;
	 Cont:
		p0++;
	}
	str0++;
}*/
	return 0;
}

wchar_t* MyStrCmpNNW(wchar_t *str0, wchar_t *str1, int N, int n)
{
	register int dw_n = n >> 2;// / 4;
	register int rem_n  = n & 0x0003;// % 4;
	register int dwF = (N - n + 1) >> 2;// / 4;

	unsigned __int32 *p1 = (unsigned __int32*)str1;

	for(int s=0; s<4; s++)
	{	unsigned __int32 *p0 = (unsigned __int32*)str0;
		for(register int f=0; f<dwF; f++)
		{	for(register int fdw=0; fdw<dw_n; fdw++)
			{	if(p0[fdw] != p1[fdw])//(p0[dw] ^ p1[dw])
					goto Cont;//return FALSE;
			}
			for(register int i=n-rem_n; i<n; i++)
			{	if(str0[f*4+i] != str1[i])//(str0[i] ^ str1[i])
					goto Cont;//return FALSE;
			}
			return (wchar_t*)p0;
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
		return (char*)p0;
	 Cont:
		p0++;
	}
	str0++;
}*/
	return 0;
}