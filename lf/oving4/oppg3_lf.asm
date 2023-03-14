;
; oppg3.asm
;
; Created: 14/02/2023 15:34:13
; Author : troch
;
#include<AVR128DB48def.inc>

; Replace with your application code

ldi r17, 0b00000100; PIN2_bm
ldi r18, 0b00001000; PIN3_bm / PINCTL_PULLUPEN_bm
ldi r19, 0xFF; 

sts PORTB_DIR, r18; PB3 output
sts 0x432, r18; PB2 PULLUPEN

start:

	lds r0, PORTB_IN; reed PB
	and r0, r17; Mask PB2

	cp r0, r17; compare 
	brne tgl

	rjmp start
nop

tgl:
	sts PORTB_OUTTGL, r18; toggle PB3
	rjmp loopStart
nop

loopStart:
	ldi r22, 15			
loop3:					; ~3928k clk
	ldi r21, 0xFF
loop2:					; ~262k clk
	ldi r20, 0xFF	
loop:					; 1024 clk
	dec r20		;1clk
	nop			;1clk
	brne loop	;2clk

	dec r21
	brne loop2

	dec r22
	brne loop3

	rjmp tgl
nop