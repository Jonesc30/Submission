;
; GPIOs_part1.asm
;
; Created: 9/29/2019 12:32:00 PM
; Author : Cody Jones
; Student ID: 5002863328
; Email: Jonesc30@unlv.nevada.edu

.org 0

sbi DDRB, 2		; set PORTB.2 as output
sbi PORTB, 2	; set LED off

; generate waveform with 40% DC and .625 sec period
BEGIN:
cbi PORTB, 2	; set LED on

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


sbi PORTB, 2	; set LED off

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


jmp BEGIN