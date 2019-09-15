;
; SimpleASM.asm
;
; Created: 9/14/2019 6:50:32 PM
; Author : Cody Jones
;


.org 0			;burn into ROM starting at 0
ldi R25, 0x30	;load upper 8 bits of multiplicand
ldi R24, 0x30	;load lower 8 bits of multiplicand
ldi R23, 0x30	;load upper 8 bits of multiplier
ldi R22, 0x30	;load lower 8 bits of multiplier
ldi R18, 0		;initialize to zero
ldi R19, 0		;initialize to zero
ldi R20, 0		;initialize to zero
ldi R21, 0		;used to carry into R20

L1:
add R17, R24	;add lower 8 bits
adc R18, R25	;add upper 9 bits
adc R20, R21	;add carry
dec R23			; decrement counter of upper 8 bits of multiplier
brne L1			; loops while counter is greater than 0

L2:
add R17, R24	;add lower 8 bits
adc R18, R25	;add upper 9 bits
adc R20, R21	;add carry
dec R22			; decrement counter of lower 8 bits of multiplier
brne L1			; loops while counter is greater than 0

END:
jmp END			;ends program