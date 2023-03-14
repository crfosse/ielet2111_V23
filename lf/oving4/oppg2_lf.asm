;
; oppg1.asm
;
; Created: 14/02/2023 13:01:02
; Author : troch
;
#include<AVR128DB48def.inc>

; Replace with your application code
start:
	ldi r28, low(1000)	
	ldi r29, high(1000)

	ldi r30, low(500)
	ldi r31, high(500)

	add r28, r30	;add without carry
	adc r29, r31	;add with carry

	adiw r29:r28, 50

	rjmp start
nop