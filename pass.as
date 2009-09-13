; this is a comment
	lea	STR, r2
LOOP:	jmp	END
	sub	#1, r1
	mov	HI, r5
	inc	r2
	bne	@LOOP
END:	hlt
