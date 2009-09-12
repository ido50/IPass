; this is a comment
MAIN:	mov	LENGTH, r1
	lea	STR, r2
LOOP:	jmp	END
	prn	r2
; and here's another comment
	sub	#1, r1
	inc	r2
	bne	@LOOP
END:	hlt
USELESS: .entry HI
STR:	.string "abcdef"
LENGTH:	.data	6
	.extern HELLO
