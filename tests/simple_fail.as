 ; this is a comment
1AIN:	mov	LENGTH  r1
	mov	LENGTH , r1
	mov	LENGTH
	lec	STR , r2
LOOP:	jmp	END
	prn	r2
; and here's another comment
	sub	#1, r1
	inc	r2
	bne	@LOOP
	cmp	NON, r2
END:	hlt
USELESS: .entri HI
STR:	.string "abcdef
LENGTH:	.data	6,
	.extern HELLO
