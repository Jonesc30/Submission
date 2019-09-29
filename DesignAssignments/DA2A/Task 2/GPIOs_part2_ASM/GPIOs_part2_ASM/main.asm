;
; GPIOs_part2_ASM.asm
;
; Created: 9/29/2019 1:57:41 PM
; Author : Cody Jones
;

.org 0

cbi	DDRC, 2		; set PORTC.2 as input
sbi	PORTC, 2	; enable pull up

sbi	DDRB, 2		; set PORTB.2 as output
sbi PORTB, 2	; set LED off

WHILE:
sbic PINC, 2	; skip next instr if PINC is low
jmp	SKIP		; jump when PINC is high
cbi PORTB, 2	; set LED on

; Delay 21 328 000 cycles
; 1s 333ms at 16.0 MHz

    ldi  r18, 109
    ldi  r19, 51
    ldi  r20, 106
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1

jmp WHILE
SKIP:
sbi	PORTB, 2	; set LED off

jmp WHILE