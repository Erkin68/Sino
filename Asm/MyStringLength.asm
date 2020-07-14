page .132
.686
.model  flat, C
.code

        public  MyStringLength
MyStringLength proc \
        uses edi, \
        src:ptr byte, \
        count: dword

        mov     edi,[src]   ; edi = src
        xor     eax,eax     ; eax=null byte
        mov     ecx,count   ; ecx = count
repne   scasb               ; find the null & count bytes

		or      ecx, ecx
		jz		ToCount
		sub		ecx, count
        neg     ecx         ; ecx=+byte count (with null)        
ToCount:
		mov		eax, ecx
		dec		eax
		        
        cld
        ret                 ; _cdecl return

MyStringLength endp
        end