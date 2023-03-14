;
; oppg1.asm
;
; Created: 14/02/2023 13:01:02
; Author : troch
;
#include<AVR128DB48def.inc>

start:
    ldi r17, 3  ;load 3 into r17
	ldi r18, 5  ;load 5 into r18
	mov r0, r18 ;move r18 into r0
	out VPORTB_DIR, r17  ;output r17 to VPORTB_DIR
	in r0, VPORTB_DIR   ;input VPORTB_DIR into r0
	sts SRAM_START ,r18 ;store r18 into SRAM_START
	lds r1, SRAM_START  ;load SRAM_START into r1

	st X,r18 ;store r18 into X
	ld r16,X ;load X into r16

	push r16    ;push r16 onto stack

	ldi r16, 99 ;load 99 into r16

	pop r16 ;pop r16 from stack

    rjmp start ;jump to start
nop