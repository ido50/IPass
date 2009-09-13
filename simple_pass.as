; this is a comment
MAIN:	mov	LENGTH, r1
	lea	STR, r2
LOOP:	jmp	END
	prn	r2
	mov	HELLO, r4
; and here's another comment
	sub	#1, r1
	mov	HI, r5
	inc	r2
	mov	HELLO, r6
	bne	@LOOP
END:	hlt
	.entry	END
USELESS: .entry STR
STR:	.string "abcdef"
LENGTH:	.data	6
	.extern HI
	.extern HELLO
	.entry LOOP
