;
; AdvancedASM.asm
;
; Created: 9/21/2019 10:26:04 PM
; Author : Cody Jones
; Student #: 5002863328
; Email: jonesc30@unlv.nevada.edu


; --- STORING VALUES ---
.equ STARTADDS = 0x0200 ; set starting address
.equ FIVEADDS = 0x0300	; set address for #s divisible by 5
.equ OTHERADDS = 0x0500 ; set address for all other #s
ldi	ZL, LOW(STARTADDS)  ; ZL=0x00
ldi ZH, HIGH(STARTADDS) ; ZH=0x02
ldi R20, 0x0A			; load R20 with starting value to store (10)
ldi R21, 0x64			; load R21 with counter value (100)
POPULATE_LOOP1:
st	Z+, R20				; Z=R20 then increment pointer
inc R20					; increment value to be stored
dec	R21					; R21 -= 1
brne POPULATE_LOOP1		; loop until R21=0

ldi R20, 0x0A			; load R20 with starting value to store (10)
ldi R21, 0x95			; load R21 with counter value (149)
POPULATE_LOOP2:
st	Z+, R20				; Z=R20 then increment pointer
inc R20					; increment value to be stored
dec	R21					; R21 -= 1
brne POPULATE_LOOP2		; loop until R21=0


; --- SEPARATING AND ADDING VALUES ---
ldi	ZL, LOW(STARTADDS)  ; ZL=0x00
ldi ZH, HIGH(STARTADDS) ; ZH=0x02
ldi YL, LOW(FIVEADDS)	; YL=0x00
ldi YH, HIGH(FIVEADDS)	; YH=0x03
ldi XL, LOW(OTHERADDS)	; XL=0x00
ldi XH, HIGH(OTHERADDS) ; XH=0x05
ldi R16, 0				; initialize sum register to zero
ldi R17, 0				; initialize sum register to zero
ldi R18, 0				; initialize sum register to zero
ldi R19, 0				; initialize sum register to zero
ldi R20, 249			; load R0 with counter value
ldi R23, 0				; used to add carry

PARSING_LOOP:
ld	R21, Z+				; R21=Z then inc Z
mov	R22, R21			; R22=R21

DIV_BY_FIVE:
subi R21, 0x05			; R21 -= 5 to check for divisibility
breq DIVISIBLE			; branch if R21 = 0 (divisible by 5)
brsh DIV_BY_FIVE		; branch if R21 >= 5, continue parsing

st	X+, R22				; store value not divisible by 5
add	R18, R22			; add value to running sum
adc	R19, R23			; add carry

rjmp CONT_PARSE			; jump to continue parsing

DIVISIBLE:
st	Y+, R22				; store value divisible by 5
add	R16, R22			; add value to running sum
adc	R17, R23			; add carry

CONT_PARSE:
dec	R20					; R20 -= 1
brne PARSING_LOOP		; loop until R20=0

END: jmp END			; end of program

