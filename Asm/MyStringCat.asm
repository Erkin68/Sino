page .132
.686
.model  flat, C
.code

        public  MyStringCat
MyStringCat proc \
        uses edi esi, \
        dest:ptr byte, \
        count: dword, \
        src:ptr byte

        mov     edi,[dest]  ; edi = src
        xor     eax,eax     ; eax=null byte
        mov     ecx,-1      ; ecx = -1
repne   scasb               ; find the null & count bytes
        add     ecx,1       ; ecx=-byte count (with null)
        neg     ecx         ; ecx=+byte count (with null)
        mov		edx, ecx	;
        dec		edx			        

        mov     edi,[src]   ; edi = src
        ;xor    eax,eax     ; eax=null byte
        or      ecx,-1      ; ecx = -1
repne   scasb               ; find the null & count bytes
        add     ecx,1       ; ecx=-byte count (with null)
        neg     ecx         ; ecx=+byte count (with null)
        
        cmp     ecx, count
        jbe		CpyToNillChar
        mov     ecx, count 
       
CpyToNillChar:        
        mov		eax,ecx		; edx - length of src
        
		mov     esi,[src]
		mov     edi,[dest]
		add		edi, edx	
rep		movsb
		
        cld
        ret                 ; _cdecl return

MyStringCat endp
        end