
	.code

_Sound		proc
	PUSH	BP
	MOV	BP,SP
	MOV	BX,[BP+04]
	MOV	AX,34DD
	MOV	DX,0012
	CMP	DX,BX
	JNB	_Sound_Exit
	DIV	BX
	MOV	BX,AX
	IN	AL,61
	TEST	AL,03
	JNZ	_Sound_Hit
	OR	AL,03
	OUT	61,AL
	MOV	AL,B6
	OUT	43,AL
_Sound_Hit:
	MOV	AL,BL
	OUT	42,AL
	MOV	AL,BH
	OUT	42,AL
_Sound_Exit:
	POP	BP
	RET
