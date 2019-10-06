;
; GPIO_IRQ_ASM.asm
;
; Created: 10/5/2019 7:45:58 PM
; Author : Cody Jones
;

.org 0			; location for reset
jmp MAIN
.org 0x02		; location for external interrupt INT0
jmp EX0_ISR

MAIN:
ldi R20, HIGH(RAMEND)	; initialize stack
out SPH, R20
ldi R20, LOW(RAMEND)
out SPL, R20

ldi R20, 2		; make INT0 falling edge triggered
sts EICRA, R20

sbi DDRB, 3		; set PORTB.3 as output
sbi PORTB, 3	; set LED off
sbi PORTD, 3	; turn on pull-up
cbi DDRC, 3		; set PORTC.3 as input
sbi PORTC, 3	; enable pull up

ldi R20, 1<<INT0	; enable INT0
out EIMSK, R20
sei				; enable interrupts


WHILE:

cbi PORTB, 3	; set LED on

; Delay 4 000 000 cycles
; 250ms at 16.0 MHz

    ldi  r18, 21
    ldi  r19, 75
    ldi  r20, 191
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1
    nop

sbi PORTB, 3	; set LED off

; Delay 6 000 000 cycles
; 375ms at 16.0 MHz

    ldi  r18, 31
    ldi  r19, 113
    ldi  r20, 31
L2: dec  r20
    brne L2
    dec  r19
    brne L2
    dec  r18
    brne L2
    nop

jmp	WHILE		; wait for interrupt


EX0_ISR:

cbi PORTB, 3

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


reti			; return from interrupt