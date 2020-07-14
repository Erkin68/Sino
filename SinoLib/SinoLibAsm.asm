.686
.model  flat
.code

m_sincos_ASM proc C \
        s: dword,\
        cc: dword,\
        angle: dword

	mov     eax, cc
	fld     angle
	fsincos
	fstp    dword ptr [eax]
	mov     eax, s
	fstp    dword ptr [eax]
	ret
		
m_sincos_ASM endp

        end