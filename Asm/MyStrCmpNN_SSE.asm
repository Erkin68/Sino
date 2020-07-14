page .132
.686
.XMM
.model  flat, C

.data
ALIGN 16
SpaceFor5   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0,0,0,0
SpaceFor6   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0,0,0
SpaceFor7   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0,0
SpaceFor8   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0
SpaceFor9   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0
SpaceFor10  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0
SpaceFor11  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0
SpaceFor12  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0
SpaceFor13  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0
SpaceFor14  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0
SpaceFor15  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0

.code
ALIGN 16
ZeroBytes DB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
AndFor5   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0,0,0,0
AndFor6   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0,0,0
AndFor7   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0,0
AndFor8   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0,0
AndFor9   DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0,0
AndFor10  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0,0
AndFor11  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0,0
AndFor12  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0,0
AndFor13  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0,0
AndFor14  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0,0
AndFor15  DB 0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0ffh,0


MyStrCmpN5_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 5 count ent

		cld

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor5
        movaps	xmm4,	[eax]	;andfor5 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor5
        mov		ecx,	5
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor5
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 5 byte;
        xorps   xmm0,	xmm1
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	5
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	5
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN5_SSE endp








MyStrCmpN6_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 6 count ent
        
        cld

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor6
        movaps	xmm4,	[eax]	;andfor6 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor6
        mov		ecx,	6
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor6
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 6 byte;
        xorps   xmm0,	xmm1
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	6
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	6
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN6_SSE endp







MyStrCmpN7_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 7 count ent

		cld

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor7
        movaps	xmm4,	[eax]	;andfor7 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor7
        mov		ecx,	7
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor7
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 7 byte;
        xorps   xmm0,	xmm1
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	7
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	7
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN7_SSE endp








MyStrCmpN8_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 8 count ent

		cld

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor8
        movaps	xmm4,	[eax]	;andfor8 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor8
        mov		ecx,	8
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor8
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 8 byte;
        xorps   xmm0,	xmm1
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	8
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	8
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN8_SSE endp










MyStrCmpN9_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 9 count ent

		cld

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor9
        movaps	xmm4,	[eax]	;andfor9 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor9
        mov		ecx,	9
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor9
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 9 byte;
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
        ;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	9
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	9
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN9_SSE endp








MyStrCmpN10_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 10 count ent

		cld

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor10
        movaps	xmm4,	[eax]	;andfor10 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor10
        mov		ecx,	10
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor10
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 10 byte;
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
        ;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	10
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	10
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN10_SSE endp








MyStrCmpN11_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 11 count ent

		cld
		
		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor11
        movaps	xmm4,	[eax]	;andfor11 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor11
        mov		ecx,	11
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor11
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 11 byte;
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
        ;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	11
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	11
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN11_SSE endp






MyStrCmpN12_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 12 count ent

		cld
		
		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor12
        movaps	xmm4,	[eax]	;andfor12 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor12
        mov		ecx,	12
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor12
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 12 byte;
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	12
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	12
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN12_SSE endp






MyStrCmpN13_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 13 count ent

		cld
		
		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor13
        movaps	xmm4,	[eax]	;andfor13 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor13
        mov		ecx,	13
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor13
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 13 byte;
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	13
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	13
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN13_SSE endp








MyStrCmpN14_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 14 count ent

		cld
		
		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor14
        movaps	xmm4,	[eax]	;andfor14 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor14
        mov		ecx,	14
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor14
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 14 byte;
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	14
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	14
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN14_SSE endp





MyStrCmpN15_SSE proc \
		uses esi edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 15 count ent

		cld
		
		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
		lea		eax,	AndFor15
        movaps	xmm4,	[eax]	;andfor15 srazu v xmm4;
        
        mov     eax,	[dest]
        
        mov     esi,	[src ]
        lea     edi,	SpaceFor15
        mov		ecx,	15
        rep     movsb
        
        mov		ecx,	count
        
        lea     edi,	SpaceFor15
        movaps	xmm1,	es:[edi]
   		xor		edi,	edi

BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

		;mov		ebx,	[dest]
		;add		ebx,	0adfh
		;cmp		eax,	ebx
		;jnz		Check
		;or		eax,	eax
;Check:

        movups	xmm0,	[eax]
        andps	xmm0,	xmm4	;zero for 15 byte;
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
        ;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	15
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	15
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN15_SSE endp





MyStrCmpN16_SSE proc \
		uses edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 16 count ent

		xor		edi,	edi
		
		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
        
        mov     eax,	[dest]
        mov     ebx,	[src ]
        mov		ecx,	count
        
        movups	xmm1,	[ebx]
BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        xorps   xmm0,	xmm1
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	16
		loop	BegCmp
		
ToNextByte:		

		inc		edi
		cmp		edi,	16
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
        
ToFinish:		

		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN16_SSE endp




MyStrCmpN32_SSE proc \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 32 count ent

		xor		edi,	edi

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
        
        mov     eax,	[dest]
        mov     ebx,	[src ]
        mov		ecx,	count
        
        movups	xmm1,	[ebx]
        movups	xmm4,	[ebx+16]
BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        movups	xmm5,	[eax+16]
        xorps   xmm0,	xmm1
        xorps	xmm5,	xmm4
        addps	xmm0,	xmm5
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	32
		loop	BegCmp 
		       
ToNextByte:		
		
		inc		edi
		cmp		edi,	32
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
		
ToFinish:
		
		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN32_SSE endp



MyStrCmpN48_SSE proc \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 48 count ent

		xor		edi,	edi

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
        
        mov     eax,	[dest]
        mov     ebx,	[src ]
        mov		ecx,	count
        
        movups	xmm1,	[ebx]
        movups	xmm4,	[ebx+16]
        movups	xmm6,	[ebx+32]
BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        movups	xmm5,	[eax+16]
        movups	xmm7,	[eax+32]
        xorps   xmm0,	xmm1
        xorps	xmm5,	xmm4
        xorps	xmm7,	xmm6
        addps	xmm0,	xmm5
        addps	xmm0,	xmm7
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	48
		loop	BegCmp    
		
ToNextByte:		

		inc		edi
		cmp		edi,	48
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
		
ToFinish:
		    
		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN48_SSE endp




MyStrCmpN64_SSE proc \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 64 count ent

		xor		edi,	edi

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
        
        mov     eax,	[dest]
        mov     ebx,	[src ]
        mov		ecx,	count
        
        movups	xmm1,	[ebx]
        movups	xmm4,	[ebx+16]
        movups	xmm6,	[ebx+32]
BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        movups	xmm5,	[eax+16]
        movups	xmm7,	[eax+32]
        xorps   xmm0,	xmm1
        xorps	xmm5,	xmm4
        xorps	xmm7,	xmm6
        addps	xmm0,	xmm5
        addps	xmm0,	xmm7
        
        movups	xmm2,	[eax+48]
        movups	xmm5,	[ebx+48]
        xorps	xmm2,	xmm5
        addps	xmm0,	xmm2
        
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps  xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	64
		loop	BegCmp 
	      
ToNextByte:		
		
		inc		edi
		cmp		edi,	64
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
		
ToFinish:
		       
		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN64_SSE endp




MyStrCmpN80_SSE proc \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 80 count ent

		xor		edi,	edi
		
		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
        
        mov     eax,	[dest]
        mov     ebx,	[src ]
        mov		ecx,	count
        
        movups	xmm1,	[ebx]
        movups	xmm4,	[ebx+16]
        movups	xmm6,	[ebx+32]
BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        movups	xmm5,	[eax+16]
        movups	xmm7,	[eax+32]
        xorps   xmm0,	xmm1
        xorps	xmm5,	xmm4
        xorps	xmm7,	xmm6
        addps	xmm0,	xmm5
        addps	xmm0,	xmm7
        
        movups	xmm2,	[eax+48]
        movups	xmm5,	[ebx+48]
        xorps	xmm2,	xmm5
        addps	xmm0,	xmm2
        
        movups	xmm2,	[eax+64]
        movups	xmm5,	[ebx+64]
        xorps	xmm2,	xmm5
        addps	xmm0,	xmm2        
        
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	80
		loop	BegCmp   

ToNextByte:		
		
		inc		edi
		cmp		edi,	80
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
		
ToFinish:
		     
		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN80_SSE endp





MyStrCmpN96_SSE proc \
		uses edi \
        dest:ptr byte, \
        src:ptr byte, \
        count: dword			;in 96 count ent
        
        xor		edi,	edi		;counter for 96

		lea		eax,	ZeroBytes
        movaps	xmm3,	[eax]	;zero srazu v xmm3;
        
        mov     eax,	[dest]
        mov     ebx,	[src ]
        mov		ecx,	count
        
        movups	xmm1,	[ebx]
        movups	xmm4,	[ebx+16]
        movups	xmm6,	[ebx+32]
BegCmp:
		cmp		ecx,	1
		ja		NoLastCircle
		or		edi,	edi
		jnz		ToNextByte
		
NoLastCircle:

        movups	xmm0,	[eax]
        movups	xmm5,	[eax+16]
        movups	xmm7,	[eax+32]
        xorps   xmm0,	xmm1
        xorps	xmm5,	xmm4
        xorps	xmm7,	xmm6
        addps	xmm0,	xmm5
        addps	xmm0,	xmm7
        
        movups	xmm2,	[eax+48]
        movups	xmm5,	[ebx+48]
        xorps	xmm2,	xmm5
        addps	xmm0,	xmm2
        
        movups	xmm2,	[eax+64]
        movups	xmm5,	[ebx+64]
        xorps	xmm2,	xmm5
        addps	xmm0,	xmm2        
        
        movups	xmm2,	[eax+80]
        movups	xmm5,	[ebx+80]
        xorps	xmm2,	xmm5
        addps	xmm0,	xmm2        
        
        movhlps xmm2,	xmm0	;xmm1 ga xmm0 ning katta 2 ta F32 si
        addps	xmm0,	xmm2	;xmm0 ning 0 - va 1 i F32 si da turibdi, yig'indi;
        movlhps xmm2,	xmm0
        shufps	xmm2,	xmm2,	3
        addss	xmm0,   xmm2
        comiss	xmm0,	xmm3	;compare with zero
        jc		dalshe
		;jp		dalshe
        jnz		dalshe
		ret						;ret with address of equ str in eax
dalshe:
		add		eax,	96
		loop	BegCmp

ToNextByte:		

		inc		edi
		cmp		edi,	96
		ja		ToFinish
		
		mov		ecx,	count
        mov     eax,	[dest]
        add		eax,	edi
        jmp		BegCmp	
		
ToFinish:		
		xor		eax,	eax		
        ret						;ret with no found, i.e. zero

MyStrCmpN96_SSE endp

Math_ReplaceDW_SSE proc C \
		uses edi \
        s1: ptr byte,\
        s2: ptr byte,\
        cnt: dword
        
		mov			eax,		[s1]
		mov         ebx,		[s2]
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
		ret
		
Math_ReplaceDW_SSE endp

        end