.686                                ; create 32 bit code
.model flat, C                      ; 32 bit memory model
 option casemap:none                ; case sensitive

 .data
 g dword 4

.code

;
; T1.asm
;
;

public      min_IA32               	

min_IA32:	push ebp					; a = ebp + 8
			mov ebp, esp					; b = ebp + 12
			sub esp, 4						; c = ebp + 16
											; v = ebp - 4
			mov eax, [ebp + 8]
			mov [ebp - 4], eax
		
			mov eax, [ebp -4]
			cmp [ebp + 12], eax
			jge minSkip1
			mov eax, [ebp + 12]
			mov [ebp - 4], eax
		
minSkip1:	mov eax, [ebp + 16]
			cmp eax, [ebp - 4]
			jge finishMin
			mov [ebp - 4], eax
		
finishMin:	mov eax, [ebp - 4]
			mov esp, ebp
			pop ebp
			ret	0
    
public		p_IA32					

p_IA32:		push	ebp					; i = [ebp + 8]
			mov		ebp, esp			; j = [ebp + 12]
										; k = [ebp + 16]
			push	[ebp + 12]			; l = [ebp + 20]
			push	[ebp + 8]
			mov		eax, g
			push	eax
			call	min_IA32
			add		esp, 12

			push	[ebp + 20]
			push	[ebp + 16]
			push	eax
			call	min_IA32
			add		esp, 12

			mov		esp, ebp
			pop		ebp
			ret		0

public		gcd_IA32

gcd_IA32:	push	ebp					; a = [ebp + 8]
			mov		ebp, esp			; b = [ebp + 12]
			push	ebx

			mov		eax, [ebp + 12]
			cmp		eax, 0
			jne		skip
			mov		eax, [ebp + 8]
			jmp		finished

skip:
			mov		eax, [ebp + 8]
			mov		ebx, [ebp + 12]
			cdq
			idiv	ebx
			push	edx
			mov		eax, [ebp + 12]
			push	eax
			call	gcd_IA32
			add		esp, 8

finished:
			pop		ebx
			mov		esp, ebp
			pop		ebp
			ret		0

end