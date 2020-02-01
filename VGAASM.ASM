
/* vgagfxhd.asm */

pushr    macro list
	 irp    n,list           ;repeat for macro list
	 push   &n               ;with the push command
	 endm
endm


popr     macro list
	 irp    n,list           ;repeat for macro list
	 pop    &n               ;for the pop command
	 endm
endm


.model large
.data

.code

old09   dd ?
old1c   dd ?
Gkey    dw ?

	public  _Draw_Screen
	public  _Draw_Screen_2
	public  _Wait_VertRetr
	public  _Install_Game_Key
	public  _Uninstall_Game_Key
	public  _Game_Key


parms1  struc
		dw      ?       ; pushed bp
		dw      ?       ; return address
ScreenBuf	dd      ?       ; address of screen buffer
ScreenPtr	dd      ?       ; address of screen
Win_Width	dw      ?
Win_Height	dw      ?
Pos_X		dw      ?
Pos_Y		dw      ?
S_Offset	dw      ?
MSW		dw      ?
MSH		dw      ?

parms1  ends

a0		dw      0
b0		dw      0
c0		dw      0
d0		dw      0
a1		dw      0
b1		dw      0
Offset2		dw      0

_Draw_Screen_2  proc
	cld
	push    bp
	mov     bp,sp
	pushr   <es,di,ds,si>


	mov     ax,[bp+Pos_X]           ; x
	mov     bx,[bp+MSW]             ; MY_SCREEN_WIDTH
;       xor     dx,dx
	div     bl                              ; a=x%MY_SCREEN_WIDTH
	mov     al,0
	xchg    al,ah
	mov     word ptr cs:a0,ax
	sub     bx,ax                                   ; b=MY_SCREEN_WIDTH-a
	mov     word ptr cs:b0,bx

	mov     ax,[bp+Pos_Y]           ; y
	mov     bx,[bp+MSH]             ; MY_SCREEN_HEIGHT
;       xor     dx,dx
	div     bl                              ; c=y%MY_SCREEN_HEIGHT
	mov     al,0
	xchg    al,ah
	mov     word ptr cs:c0,ax
	sub     bx,ax                                   ; d=MY_SCREEN_HEIGHT-c
	mov     word ptr cs:d0,bx

	mov     ax,word ptr cs:c0               ; Offset2=c*MY_SCREEN_WIDTH
	mul     [bp+MSW]
	mov     word ptr cs:Offset2,ax

	mov     cx,word ptr cs:b0       ; for (i=0;i<b;i++) SCREENPTR[i]=SCREENBUF[a+i]
	cmp     cx,[bp+Win_Width]
	jb      ?22
	mov     cx,[bp+Win_Width]
?22:
	mov     word ptr cs:b1,cx

;       mov     cx,word ptr cs:a0
;       cmp     cx,8
;       ja      ?23
;       sub     cx,8
;       ja      ?23
;       mov     cx,0
;?23:
;       mov     word ptr cs:a0,cx



	les     di,[bp+ScreenPtr]
	add     di,[bp+S_Offset]
	lds     si,[bp+ScreenBuf]
	add     si,word ptr cs:Offset2


	mov     cx,word ptr cs:d0       ; if (j<(wh<<3))
	cmp     cx,[bp+Win_Height]
	jb      ?01
	mov     cx,[bp+Win_Height]
?01:
	push    cx
	pushr   <ds,si,di,es>
	add     si,word ptr cs:a0

	mov     cx,word ptr cs:b1
	repnz   movsb

	push    bp
	mov     bp,sp
	mov     es,[bp+2]
	mov     di,[bp+4]
	mov     si,[bp+6]
	mov     ds,[bp+8]
	pop     bp
	add     di,word ptr cs:b0

	mov     cx,word ptr cs:a0
	cmp     cx,16
	jb      ?03
	sub     cx,16
	jz      ?03
	repnz   movsb
?03:

	popr    <es,di,si,ds>
	pop     cx
	add     di,320                  ; Offset  += SCREEN_WIDTH
	add     si,[bp+MSW]             ; offset2 += MY_SCREEN_WIDTH
	dec     cx
	jnz     ?01
?05:

	mov     word ptr cs:Offset2,0
;;      les     di,[bp+ScreenPtr]
;;      add     di,[bp+S_Offset]
	lds     si,[bp+ScreenBuf]
;;      add     si,word ptr cs:Offset2

	mov     cx,word ptr cs:c0
	cmp     cx,16
	jb      ?15
	sub     cx,16
	jz      ?15
?11:
	push    cx

	pushr   <ds,si,di,es>
	add     si,word ptr cs:a0

	mov     cx,word ptr cs:b1
	repnz   movsb

	push    bp
	mov     bp,sp
	mov     es,[bp+2]
	mov     di,[bp+4]
	mov     si,[bp+6]
	mov     ds,[bp+8]
	pop     bp
	add     di,word ptr cs:b0

	mov     cx,word ptr cs:a0
	cmp     cx,16
	jb      ?13
	sub     cx,16
	jz      ?13
	repnz   movsb
?13:

	popr    <es,di,si,ds>


	pop     cx
	add     di,320
	add     si,[bp+MSW]
	dec     cx
	jnz     ?11
?15:

	popr    <si,ds,di,es>
	pop     bp
	ret
_Draw_Screen_2  endp

_Draw_Screen    proc
	cld
	push    bp
	mov     bp,sp
	push    di
	push    si
	mov     dx,0                    ; i
I_Loop:
	push    dx

	mov     bx,dx                   ; i
	mov     ax,[bp+Pos_Y]           ; y
	add     ax,bx                   ; i+y
	mov     bx,[bp+MSH]
;       mov     bx,104
	xor     dx,dx
	div     bl
	mov     al,0
	xchg    al,ah
	mov     bx,[bp+MSW]
;       mov     bx,160
	mul     bl
	mov     dx,ax
	mov     cx,0                    ; j
J_Loop:
	push    cx

	mov     bx,cx                   ; j
	mov     ax,[bp+Pos_X]           ; x
	add     ax,bx                   ; j+x
	mov     bx,[bp+MSW]
;       mov     bx,160
	div     bl
	mov     al,0
	xchg    al,ah

;       mov     bx,dx
	add     ax,dx

	mov     bx,[bp+S_Offset]
	add     bx,cx

	les     si,[bp+ScreenBuf]
	add     si,ax
	mov     al,es:[si]

	les     di,[bp+ScreenPtr]
	add     di,bx
	mov     es:[di],al

	pop     cx
	inc     cx
	cmp     cx,[bp+Win_Width]
	jb      J_Loop
	mov     ax,320
	add     [bp+S_Offset],ax

	pop     dx
	inc     dx
	cmp     dx,[bp+Win_Height]
	jb      I_Loop
	pop     si
	pop     di
	pop     bp
	ret
_Draw_Screen    endp


_Wait_VertRetr  proc
	push    bp
	push    dx
	push    ax
	mov     dx,03dah
_Wait_Again1:
	in      al,dx
	test    al,08h
	jz      _Wait_Again1
_Wait_Again2:
	in      al,dx
	test    al,08h
	jnz     _Wait_Again2
	pop     ax
	pop     dx
	pop     bp
	ret
_Wait_VertRetr  endp

;;;;;;;;;
;;;;;;;;
;;;;;;;
;;;;;;
;;;;;
;;;;
;;;
;;
; KEYB.ASM:  Originally Written By Phillip Whillier 1993.
;;           NewInt09 ReWritten by Matthew Edwards 1993.
;;;          The Keyboard Will Be Disabled To Avoid
;;;;         keyboard overflow! So Don't Forget To
;;;;;        Uninstall The Game Key!
;;;;;;
;;;;;;;      Link This Module With Your C Games!
;;;;;;;;
;;;;;;;;;

rom_bios_data segment at 40h

	org     1ah
	head    dw ?
	tail    dw ?

rom_bios_data ends

Disable_Keyboard proc

	pushf
	cli
	call    dword ptr cs:old1c
	sti

	cli
	pushr   <ax,es>
	mov     ax,seg rom_bios_data       ;Clear keyboard buffer
	mov     es,ax
	mov     ax,es:tail
	mov     es:head,ax
	popr    <es,ax>
exdk:
	sti
	iret

Disable_Keyboard endp


_Install_Game_Key proc
	cli
	pushr   <ds,es,ax,bx,dx>
	mov     word ptr cs:Gkey,0		;clear game key!

	mov     ah,35h				;store old int 1c vector
	mov     al,1ch
	int     21h
	mov     word ptr cs:old1c,bx
	mov     word ptr cs:old1c+2,es
										;capture int 1c
	push    cs
	pop     ds
	mov     dx,offset Disable_Keyboard
	mov     ah,25h                                  ; set int 1c
	mov     al,1ch
	int     21h
	push    es
	mov     ah,35h                                  ;store old int 09 vector
	mov     al,09h
	int     21h
	mov     word ptr cs:old09,bx
	mov     word ptr cs:old09+2,es

	push    ds                                              ;capture int 09
	push    cs
	pop     ds
	mov     dx,offset NewInt09
	mov     ah,25h                                  ; set int 09
	mov     al,09h
	int     21h
	pop     ds
	pop     es
	popr    <dx,bx,ax,es,ds>
	sti
	ret
_Install_Game_Key endp

_Uninstall_Game_Key proc
	cli
	pushr   <ds,dx,ax>			;restore int 1c
;	mov     byte ptr cs:Gkey,0		;clear game key!
	mov     dx,word ptr cs:old1c            ;Timer
	mov     ax,word ptr cs:old1c+2
	mov     ds,ax
	mov     ah,25h
	mov     al,1ch
	int     21h

	push    ds                              ;restore int 09
	mov     dx,word ptr cs:old09            ;Timer
	mov     ax,word ptr cs:old09+2
	mov     ds,ax
	mov     ah,25h
	mov     al,09h
	int     21h
	pop     ds
	popr    <ax,dx,ds>
	sti
	ret

_Uninstall_Game_Key endp

_Game_Key proc
	mov     ax,word ptr cs:Gkey
	ret
_Game_Key endp

NewInt09     proc
	cli
	pushr   <ax,bx,es>
	in      al,60h
	test    al,80h
	jnz             Key_Hit_Up
Key_Hit_Down:
	call    Translate_In_Out_Key
	or              word ptr cs:Gkey,bx
	jmp             Key_Hit_OK
Key_Hit_Up:
	and             al,7Fh
	call    Translate_In_Out_Key
	not             bx
	and             word ptr cs:Gkey,bx
Key_Hit_OK:
	popr    <es,bx,ax>
	jmp     dword ptr cs:old09

Translate_In_Out_Key:
	mov     ah,0
	mov     bx,0
	cmp     al,48h          ; crsr up
	jnz     c1
	mov     bx,0000000000000001b
	jmp     cz
c1:
	cmp     al,50h          ; crsr down
	jnz     c2
	mov     bx,0000000000000010b
	jmp     cz
c2:
	cmp     al,4bh          ; crsr left
	jnz     c3
	mov     bx,0000000000000100b
	jmp     cz
c3:
	cmp     al,4dh          ; crsr right
	jnz     c4
	mov     bx,0000000000001000b
	jmp     cz
c4:
	cmp     al,39h          ; space
	jnz     c5
	mov     bx,0000000000010000b
	jmp     cz
c5:
	cmp     al,1ch          ; enter
	jnz     c6
	mov     bx,0000000000100000b
	jmp     cz
c6:
	cmp     al,0Fh          ; tab
	jnz     c7
	mov     bx,0000000001000000b
	jmp     cz
c7:
	cmp     al,00h          ; Key_undef7
	jnz     c8
	mov     bx,0000000010000000b
	jmp     cz
c8:
	cmp     al,00h      ; Key_undef8
	jnz     c9
	mov     bx,0000000100000000b
	jmp     cz
c9:
	cmp     al,00h      ; Key_undef9
	jnz     cA
	mov     bx,0000001000000000b
	jmp     cz
cA:
	cmp     al,00h          ; Key_undefA
	jnz     cB
	mov     bx,0000010000000000b
	jmp     cz
cB:
	cmp     al,00h          ; Key_undefB
	jnz     cC
	mov     bx,0000100000000000b
	jmp     cz
cC:
	cmp     al,19h          ; p
	jnz     cD
	mov     bx,0001000000000000b
	jmp     cz
cD:
	cmp     al,3bh          ; F1
	jnz     cE
	mov     bx,0010000000000000b
	jmp     cz
cE:
	cmp     al,3ch          ; F2
	jnz     cF
	mov     bx,0100000000000000b
	jmp     cz
cF:
	cmp     al,01h          ; escape
	jnz     cz
	mov     bx,1000000000000000b
cz:
	ret

NewInt09 endp

end
