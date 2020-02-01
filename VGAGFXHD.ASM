;;
; vgagfxhd.asm : Created ??-??-1994 v1.0a  - Matthew Jacob Edwards
;              : Updated 11-03-1995 v1.1   - Matthew Jacob Edwards
;;
;;;
;;;;
;;;;;
;
;
; gd<name> - Global Define
; gc<name> - Global Constant
;
;

;                    ;
;; Screen Specifics ;;
;                    ;
gcScreenAddress		=	0b800h
gcScreenWidth		=	320
gcScreenHeight		=	200

;                  ;
;; Font specifics ;;
;                  ;
gcMinFontChar		=	32
gcMaxFontChar		=	128								;** really = 127
gcCntFontChar		=	gcMaxFontChar - gcMinFontChar
gcFontWidth			=	8
gcFontHeight		=	8
gcMaxCharSize		=	gcFontHeight * gcFontWidth

;
;; Tile Specifics
;
gcTileWidth			=	8
gcTileHeight		=	8
gcTileDepth			=	8   ; 256 cols max.

;                  ;
;; Matt's Macro's ;;
;                  ;

UBYTE	macro	list
list	db		?
	endm

;BYTE	macro	list
;list	db		?
;	endm

UWORD	macro	list
list	dw		?
	endm

;WORD	macro	list
;list	dw		?
;	endm

ULONG	macro	list
list	dd		?
	endm

LONG	macro	list
list	dd		?
	endm

APTR	macro	list
list	dd		?
	endm

pushr	macro list
		irp    n,list           ;repeat for macro list
			push   &n               ;with the push command
		endm
	endm


popr	macro list
		irp    n,list           ;repeat for macro list
			pop    &n               ;for the pop command
		endm
	endm

;                    ;
;; Register Summary ;;
;                    ;

;
; ax  [ah,al]
; bx  [bh,bl]
; cx  [ch,cl]
; dx  [dh,dl]
; ss  stack segment
; cs  code segment
; es  extra segment
; ds  data segment
; si  source indicator
; di  destination indicator
;
; [ES:DI] extra source
; [DS:SI] data destination
;

.model large
.data

.code

public	_Draw_Screen			;
public	_Draw_Screen_2			;
public	_Draw_Screen_3			;
public	_Wait_VertRetr			; ()
public	_Install_Game_Key		; ()
public	_Uninstall_Game_Key		; ()
public	_Game_Key				; ()
public	Translate_In_Out_Key	;
public	_WriteTile_Char			; (int x,int y,byte block,byte forecol,byte backcol)
public	_Fill_Rectangle			; (x1,y1,x2,y2)
public	_Setup_ASM_Vars			; (Screenptr,screenmem,tiles,map,craft,font)
public	_Get_LineOffsets		; ()
public	_Get_CharOffsets		; ()

gdTheScreen			dd		0		; Screen pointer ($B800:0000)
gdScreenData		dd		0		; Screen Memory from an allocation of mem
gdTileData			dd		0		; Tile Mem
gdMapData			dd		0		; Map Mem
gdCraftData			dd		0		; Craft Mem
gdFontData			dd		0		; 8x8 Character font Mem (8*8*96 = 6144 (6k) )
gdLineOffsets		dw		gcScreenHeight dup (?)	; WriteTile_Char Line Offsets
gdCharOffsets		dw		gcCntFontChar dup (?)	; WriteTile_Char Character Offsets

;********************************************************
;  Setup_ASM_Vars :
;********************************************************

sav_parms      struc
					dw      ?       ; ????
					dw      ?       ; pushed bp
					dw      ?       ; return address
sav_ScreenPtr		dd		?
sav_ScreenBuf		dd		?
sav_Tiles			dd		?
sav_Map				dd		?
sav_Craft			dd		?
sav_Font			dd		?
sav_parms      ends

_Setup_ASM_Vars proc
	cld
	push	bp
	mov		bp,sp
	pushr	<es,di>
	les		di,[bp+sav_ScreenPtr]   ; get parameter of screen
	mov     word ptr cs:gdTheScreen,di
	mov     word ptr cs:gdTheScreen+2,es

	les		di,[bp+sav_ScreenBuf]   ; get parameter of screen
	mov     word ptr cs:gdScreenData,di
	mov     word ptr cs:gdScreenData+2,es

	les		di,[bp+sav_Tiles]   ; get parameter of tiles
	mov     word ptr cs:gdTileData,di
	mov     word ptr cs:gdTileData+2,es

	les		di,[bp+sav_Map]   ; get parameter of map
	mov     word ptr cs:gdMapData,di
	mov     word ptr cs:gdMapData+2,es

	les		di,[bp+sav_Craft]   ; get parameter of craft
	mov     word ptr cs:gdCraftData,di
	mov     word ptr cs:gdCraftData+2,es

	les		di,[bp+sav_Font]   ; get parameter of font
	mov     word ptr cs:gdFontData,di
	mov     word ptr cs:gdFontData+2,es

;
;; Setup for the WriteTile_Char proc, the line offsets.
;

	lea		di,gdLineOffsets		; effective address of line array
	mov		ax,0					; start of line 0 pixel 0
	mov		cx,gcScreenHeight		; the number of lines to create
sav_ScrnNext:
	mov		word ptr cs:[di],ax
	add		ax,gcScreenWidth		; add bytes per screen line width to get to next line
	add		di,2
	loop	sav_ScrnNext			; loop for CX times

	lea		di,gdCharOffsets		; effective address of char array
	mov		ax,0					; start of line 0 pixel 0
	mov		cx,gcCntFontChar		; the number of offsets to create
sav_FontNext:
	mov		word ptr cs:[di],ax
	add		ax,gcMaxCharSize		; add bytes per char size
	add		di,2
	loop	sav_FontNext			; loop for CX times

; code here

	popr	<di,es>
	mov		sp,bp
	pop		bp
	ret
_Setup_ASM_Vars endp

_Get_LineOffsets	proc
	push	di
	push	es
	lea		di,gdLineOffsets
	mov		ax,di
	mov		dx,cs
	pop		es
	pop		di
	ret
_Get_LineOffsets	endp

_Get_CharOffsets	proc
	push	di
	push	es
	lea		di,gdCharOffsets
	mov		ax,di
	mov		dx,cs
	pop		es
	pop		di
	ret
_Get_CharOffsets	endp

;********************************************************
;  Draw Screen :
;********************************************************

DrawScreen1_Params	struc
			dw      ?       ; ????
			dw      ?       ; pushed bp
			dw      ?       ; return address
	APTR	ds1_ScreenBuf		dd      ?       ; address of screen buffer
	APTR	ds1_ScreenPtr		dd      ?       ; address of screen
	UWORD	ds1_Win_Width		dw      ?
	UWORD	ds1_Win_Height		dw      ?
	UWORD	ds1_Pos_X			dw      ?
	UWORD	ds1_Pos_Y			dw      ?
	UWORD	ds1_MSW				dw      ?
	UWORD	ds1_MSH				dw      ?

DrawScreen1_Params	ends

a0		dw ?
b0		dw ?
c0		dw ?
d0		dw ?
a1		dw ?
b1		dw ?
Offset2	dw ?


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
	mov     ax,[bp+ds1_Pos_Y]           ; y
	add     ax,bx                   ; i+y
	mov     bx,[bp+ds1_MSH]
;       mov     bx,104
	xor     dx,dx
	div     bl
	mov     al,0
	xchg    al,ah
	mov     bx,[bp+ds1_MSW]
;       mov     bx,160
	mul     bl
	mov     dx,ax
	mov     cx,0                    ; j
J_Loop:
	push    cx

	mov     bx,cx                   ; j
	mov     ax,[bp+ds1_Pos_X]           ; x
	add     ax,bx                   ; j+x
	mov     bx,[bp+ds1_MSW]
;       mov     bx,160
	div     bl
	mov     al,0
	xchg    al,ah

;       mov     bx,dx
	add     ax,dx

;       mov     bx,[bp+S_Offset]
;       add     bx,cx

	les     si,[bp+ds1_ScreenBuf]
	add     si,ax
	mov     al,es:[si]

	les     di,[bp+ds1_ScreenPtr]
	add     di,bx
	mov     es:[di],al

	pop     cx
	inc     cx
	cmp     cx,[bp+ds1_Win_Width]
	jb      J_Loop
	mov     ax,320
;       add     [bp+S_Offset],ax

	pop     dx
	inc     dx
	cmp     dx,[bp+ds1_Win_Height]
	jb      I_Loop
	pop     si
	pop     di
	pop     bp
	ret
_Draw_Screen    endp

;********************************************************
;  Draw Screen 2 :
;********************************************************

_Draw_Screen_2  proc
	cld
	push    bp
	mov     bp,sp
	pushr   <es,di,ds,si>
	pushr   <ax,bx,cx,dx>

	mov     ax,[bp+ds1_Pos_X]           ; x
	mov     bx,[bp+ds1_MSW]             ; MY_SCREEN_WIDTH
;       xor     dx,dx
	div     bl                              ; a=x%MY_SCREEN_WIDTH
	mov     al,0
	xchg    al,ah
	mov     word ptr cs:a0,ax
	sub     bx,ax                                   ; b=MY_SCREEN_WIDTH-a
	mov     word ptr cs:b0,bx

	mov     ax,[bp+ds1_Pos_Y]           ; y
	mov     bx,[bp+ds1_MSH]             ; MY_SCREEN_HEIGHT
;       xor     dx,dx
	div     bl                              ; c=y%MY_SCREEN_HEIGHT
	mov     al,0
	xchg    al,ah
	mov     word ptr cs:c0,ax
	sub     bx,ax                                   ; d=MY_SCREEN_HEIGHT-c
	mov     word ptr cs:d0,bx

	mov     ax,word ptr cs:c0               ; Offset2=c*MY_SCREEN_WIDTH
	mul     [bp+ds1_MSW]
	mov     word ptr cs:Offset2,ax

	mov     cx,word ptr cs:b0       ; for (i=0;i<b;i++) SCREENPTR[i]=SCREENBUF[a+i]
	cmp     cx,[bp+ds1_Win_Width]
	jb      ?22
	mov     cx,[bp+ds1_Win_Width]
?22:
	mov     word ptr cs:b1,cx

;;       mov     cx,word ptr cs:a0
;;       cmp     cx,8
;;       ja      ?23
;;       sub     cx,8
;;       ja      ?23
;;       mov     cx,0
;;?23:
;;       mov     word ptr cs:a0,cx



	les     di,[bp+ds1_ScreenPtr]
;       add     di,[bp+ds1_S_Offset]
	lds     si,[bp+ds1_ScreenBuf]
	add     si,word ptr cs:Offset2

	mov     cx,word ptr cs:d0       ; if (j<(wh<<3))
	cmp     cx,[bp+ds1_Win_Height]
	jb      ?01
	mov     cx,[bp+ds1_Win_Height]
?01:
	push    cx
	pushr   <ds,si,di,es>
	add     si,word ptr cs:a0

	mov     cx,word ptr cs:b1
	rep     movsb

;       push    bp
;       mov     bp,sp
;       mov     es,[bp+2]
;       mov     di,[bp+4]
;       mov     si,[bp+6]
;       mov     ds,[bp+8]
;       pop     bp
	popr    <es,di,si,ds>
	pushr   <ds,si,di,es>

	add     di,word ptr cs:b0

	mov     cx,word ptr cs:a0
	cmp     cx,32
	jb      ?03
	sub     cx,32
	jz      ?03
	repnz   movsb
?03:

	popr    <es,di,si,ds>
	pop     cx

	add     di,320                  ; Offset  += SCREEN_WIDTH
	add     si,[bp+ds1_MSW]             ; offset2 += MY_SCREEN_WIDTH

	dec     cx
	jnz     ?01
?05:

	mov     word ptr cs:Offset2,0
;      les     di,[bp+ds1_ScreenPtr]
;      add     di,[bp+ds1_S_Offset]
	lds     si,[bp+ds1_ScreenBuf]
;      add     si,word ptr cs:Offset2

	mov     cx,word ptr cs:c0
	cmp     cx,24
	jb      ?15
	sub     cx,24
	jz      ?15
?11:
	push    cx

	pushr   <ds,si,di,es>
	add     si,word ptr cs:a0

	mov     cx,word ptr cs:b1
	repnz   movsb

;       push    bp
;       mov     bp,sp
;       mov     es,[bp+2]
;       mov     di,[bp+4]
;       mov     si,[bp+6]
;       mov     ds,[bp+8]
;       pop     bp
	popr    <es,di,si,ds>
	pushr   <ds,si,di,es>
	add     di,word ptr cs:b0

	mov     cx,word ptr cs:a0
	cmp     cx,32
	jb      ?13
	sub     cx,32
	jz      ?13
	repnz   movsb
?13:

	popr    <es,di,si,ds>


	pop     cx
	add     di,320
	add     si,[bp+ds1_MSW]
	dec     cx
	jnz     ?11
?15:

	popr    <dx,cx,bx,ax>
	popr    <si,ds,di,es>
	pop     bp
	ret

_Draw_Screen_2  endp

;********************************************************
;  Draw Screen 3 :
;********************************************************

_Draw_Screen_3  proc
_DS3:
	cld
	push    bp
	mov     bp,sp
	pushr   <es,di,ds,si>
	pushr   <ax,bx,cx,dx>

	mov     ax,[bp+ds1_Pos_X]           ; x
	mov     bx,[bp+ds1_MSW]             ; MY_SCREEN_WIDTH
;       xor     dx,dx
	div     bl                              ; a=x%MY_SCREEN_WIDTH
	mov     al,0
	xchg    al,ah
	mov     word ptr cs:a0,ax
	sub     bx,ax                                   ; b=MY_SCREEN_WIDTH-a
	mov     word ptr cs:b0,bx

	mov     ax,[bp+ds1_Pos_Y]           ; y
	mov     bx,[bp+ds1_MSH]             ; MY_SCREEN_HEIGHT
;       xor     dx,dx
	div     bl                              ; c=y%MY_SCREEN_HEIGHT
	mov     al,0
	xchg    al,ah
	mov     word ptr cs:c0,ax
	sub     bx,ax                                   ; d=MY_SCREEN_HEIGHT-c
	mov     word ptr cs:d0,bx

	mov     ax,word ptr cs:c0               ; Offset2=c*MY_SCREEN_WIDTH
	mul     [bp+ds1_MSW]
	mov     word ptr cs:Offset2,ax

;       mov     cx,word ptr cs:b0       ; for (i=0;i<b;i++) SCREENPTR[i]=SCREENBUF[a+i]
;       cmp     cx,[bp+ds1_Win_Width]
;       jb      ?022
;       mov     cx,[bp+ds1_Win_Width]
;?022:
;       mov     word ptr cs:b1,cx

;;       mov     cx,word ptr cs:a0
;;       cmp     cx,8
;;       ja      ?023
;;       sub     cx,8
;;       ja      ?023
;;       mov     cx,0
;;?023:
;;       mov     word ptr cs:a0,cx



	les     di,[bp+ds1_ScreenPtr]
;       add     di,[bp+S_Offset]
	lds     si,[bp+ds1_ScreenBuf]
	add     si,word ptr cs:Offset2

	mov     cx,word ptr cs:d0       ; if (j<(wh<<3))
;       cmp     cx,[bp+ds1_Win_Height]
;       jb      ?001
;       mov     cx,[bp+ds1_Win_Height]
?001:
	push    cx
	pushr   <ds,si,di,es>
	add     si,word ptr cs:a0

	mov     cx,word ptr cs:b0
	rep     movsb

;       push    bp
;       mov     bp,sp
;       mov     es,[bp+2]
;       mov     di,[bp+4]
;       mov     si,[bp+6]
;       mov     ds,[bp+8]
;       pop     bp
	popr    <es,di,si,ds>
	pushr   <ds,si,di,es>

	add     di,word ptr cs:b0

	mov     cx,word ptr cs:a0
;       cmp     cx,32
;       jb      ?003
;       sub     cx,32
;       jz      ?003
	repnz   movsb
?003:

	popr    <es,di,si,ds>
	pop     cx

	add     di,320                  ; Offset  += SCREEN_WIDTH
	add     si,[bp+ds1_MSW]             ; offset2 += MY_SCREEN_WIDTH

	dec     cx
	jnz     ?001
?005:

	mov     word ptr cs:Offset2,0
;      les     di,[bp+ScreenPtr]
;      add     di,[bp+S_Offset]
	lds     si,[bp+ds1_ScreenBuf]
;      add     si,word ptr cs:Offset2

	mov     cx,word ptr cs:c0
;       cmp     cx,24
;       jb      ?015
;       sub     cx,24
;       jz      ?015
?011:
	push    cx

	pushr   <ds,si,di,es>
	add     si,word ptr cs:a0

	mov     cx,word ptr cs:b0
	repnz   movsb

;       push    bp
;       mov     bp,sp
;       mov     es,[bp+2]
;       mov     di,[bp+4]
;       mov     si,[bp+6]
;       mov     ds,[bp+8]
;       pop     bp
	popr    <es,di,si,ds>
	pushr   <ds,si,di,es>
	add     di,word ptr cs:b0

	mov     cx,word ptr cs:a0
;       cmp     cx,32
;       jb      ?013
;       sub     cx,32
;       jz      ?013
	repnz   movsb
?013:

	popr    <es,di,si,ds>


	pop     cx
	add     di,320
	add     si,[bp+ds1_MSW]
	dec     cx
	jnz     ?011
?015:

	popr    <dx,cx,bx,ax>
	popr    <si,ds,di,es>
	pop     bp
	ret


_Draw_Screen_3  endp

;********************************************************
;  Wait_VertRetr : Wait for a vertical retrace to complete
;********************************************************

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


;********************************************************
;  Fill Rectangle : fills a rectangle on screen
;********************************************************

Rect1  struc
				dw      ?       ; pushed bp
				dw      ?       ; ???? Code Segment for large model
				dw      ?       ; return address
X1				dw      ?
Y1				dw      ?
X2				dw      ?
Y2				dw      ?
COL				dw      ?
SCREENP			dd      ?

Rect1  ends

Rect_W          dw      ?
Rect_H          dw      ?

_Fill_Rectangle proc
	cld
	push    bp
	mov     bp,sp
	pushr   <es,di,ds,si>
	pushr   <ax,bx,cx>

	mov     ax,[bp+X2]
	sub     ax,[bp+X1]
	mov     word ptr cs:Rect_W,ax
	mov     ax,[bp+Y2]
	sub     ax,[bp+Y1]
	mov     word ptr cs:Rect_H,ax

	les     di,[bp+SCREENP]
	mov     ax,[bp+Y1]
	mov     cl,8
	shl     ax,cl
	add     di,ax
	mov     ax,[bp+Y1]
	mov     cl,6
	shl     ax,cl
	add     di,ax
	mov     ax,[bp+X1]
	add     di,ax

	mov     cx,word ptr cs:Rect_H
_Rect_Height:
	push    cx
	mov     ax,[bp+COL]
	mov     cx,word ptr cs:Rect_W
	rep     stosb
	mov     bx,320
	sub     bx,word ptr cs:Rect_W
	add     di,bx
	pop     cx

	loop    _Rect_Height

	popr    <cx,bx,ax>
	popr    <si,ds,di,es>
	pop     bp
	ret
_Fill_Rectangle endp

;;;;;;;;;
;;;;;;;;
;;;;;;;
;;;;;;
;;;;;
;;;;
;;;
;;
; KEYA.ASM:  Originally Written By Phillip Whillier 1993.
;;           NewInt09 ReWritten by Matthew Edwards 1993.
;;;          The Keyboard Will Be Disabled To Avoid
;;;;         keyboard overflow! So Don't Forget To
;;;;;        Uninstall The Game Key!
;;;;;;
;;;;;;;      Link This Module With Your C Games!
;;;;;;;;
;;;;;;;;;

old09   dd ?
old1c   dd ?
Gkey    dw ?

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
	mov     word ptr cs:Gkey,0              ;clear game key!

	mov     ah,35h                          ;store old int 1c vector
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
	pushr   <ds,dx,ax>                      ;restore int 1c
	mov     byte ptr cs:Gkey,0              ;clear game key!
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

Translate_In_Out_Key proc
;       mov      ah,0
       mov      bx,0
       cmp      al,48h          ; crsr up
       jnz      c1
       mov      bx,0000000000000001b
       jmp      cz
c1:
       cmp      al,50h          ; crsr down
       jnz      c2
       mov      bx,0000000000000010b
       jmp      cz
c2:
       cmp      al,4bh          ; crsr left
       jnz      c3
       mov      bx,0000000000000100b
       jmp      cz
c3:
       cmp      al,4dh          ; crsr right
       jnz      c4
       mov      bx,0000000000001000b
       jmp      cz
c4:
       cmp      al,39h          ; space
       jnz      c5
       mov      bx,0000000000010000b
       jmp      cz
c5:
       cmp      al,1ch          ; enter
       jnz      c6
       mov      bx,0000000000100000b
       jmp      cz
c6:
       cmp      al,0Fh          ; tab
       jnz      c7
       mov      bx,0000000001000000b
       jmp      cz
c7:
;       cmp      al,00h          ; Key_undef7
;       jnz      c8
;       mov      bx,0000000010000000b
;       jmp      cz
;c8:
;       cmp      al,00h      ; Key_undef8
;       jnz      c9
;       mov      bx,0000000100000000b
;       jmp      cz
;c9:
;       cmp      al,00h      ; Key_undef9
;       jnz      cA
;       mov      bx,0000001000000000b
;       jmp      cz
;cA:
;       cmp      al,00h          ; Key_undefA
;       jnz      cB
;       mov      bx,0000010000000000b
;       jmp      cz
;cB:
;       cmp      al,00h          ; Key_undefB
;       jnz      cC
;       mov      bx,0000100000000000b
;       jmp      cz
cC:
       cmp      al,19h          ; p
       jnz      cD
       mov      bx,0001000000000000b
       jmp      cz
cD:
       cmp      al,3bh          ; F1
       jnz      cE
       mov      bx,0010000000000000b
       jmp      cz
cE:
       cmp      al,3ch          ; F2
       jnz      cF
       mov      bx,0100000000000000b
       jmp      cz
cF:
       cmp      al,01h          ; escape
       jnz      cz
       mov      bx,1000000000000000b
       ret
cz:
       or       bx,0000100000000000b

       ret

Translate_In_Out_Key endp

NewInt09     proc
	cli
	pushr   <ax,bx>
	xor     ax,ax
	xor     bx,bx
	in      al,60h
	test    al,80h
	jnz     Key_Hit_Up
Key_Hit_Down:
	call    Translate_In_Out_Key
	or      word ptr cs:Gkey,bx
	jmp     Key_Hit_OK
Key_Hit_Up:
	and     al,7Fh
	call    Translate_In_Out_Key
	not     bx
	and     word ptr cs:Gkey,bx
Key_Hit_OK:
	popr    <bx,ax>
	jmp     dword ptr cs:old09
	ret
NewInt09 endp

;void writetile_ch(word x, word y, byte block,byte col1,byte col2)
;{
;;  register char i,j;
;;  register word blk=(int)(block*(tw*th));
;;  register word scrn=x+(y*SCREEN_WIDTH);
;	for(j=0;j<th;j++) {
;	  for(i=0;i<tw;i++) {
;
;		if(TILES[blk])
;			if ( col1 > 0 ) SCREENPTR[scrn]=col1;
;			else SCREENPTR[scrn]=j+24; /* back colour */
;		else
;			if ( col2 > 0 ) SCREENPTR[scrn]=col2;
;	blk++;
;	scrn++;
;	  }
;	  scrn+=SCREEN_WIDTH-tw;
;	}
;}

;***********************************************************
;  WriteTile_Char : copies a tile from TILE to screen (B800)
;***********************************************************

WriteTile_Params	struc
	UWORD	wt_BP			; pushed bp
	UWORD	wt_CS			; Code Segment for large model
	UWORD	wt_RET			; return address

	UWORD	wt_XPos			; x position on screen
	UWORD	wt_YPos			; y position on screen
	UBYTE	wt_Block		; the tile to display
	UBYTE	wt_FgCol		; forground colour
	UBYTE	wt_BkCol		; background colour

WriteTile_Params	ends

_WriteTile_Char	proc
	cld                             ; clear direction flag
	push    bp                      ; save off bp
	mov     bp,sp                   ; use bp for params base
	pushr   <es,di,ds,si>           ; save used registers
	pushr   <ax,bx,cx>

	les		di,gdTheScreen

	lea		di,gdLineOffsets        ; effective address of line array
	mov		ax,[bp+wt_YPos]         ; get y coord
	add		ax,ax                   ; * 2 for word align
	add		di,ax                   ; add this to base address
	mov		ax,cs:[di]					; get y offset from array
	add		ax,[bp+wt_XPos]         ; add to ax the x offset
;
;; ax is now the offset on the screen that the char will end up
;

	lea		di,gdCharOffsets		; effective address of char array
	xor		bx,bx
	mov		bl,[bp+wt_Block]		; get block
	add		bx,bx                   ; * 2 for word align
	add		di,bx                   ; add this to base address
	mov		bx,cs:[di]					; get offset in array of block offset
;
;; bx is now the offset of the block to display
;

	les		di,gdTheScreen
	add		di,ax
	lds		si,gdFontData
	add		si,bx

	mov		cx,gcTileHeight
wt_NextHeight:						; for h = 0 to tileheight-1
		push	cx
		mov		cx,gcTileWidth
wt_NextWidth:						; for w = 0 to tilewidth-1
wt_1_If:
			mov		al,ds:[si]          ; get pixel from font
			or		al,al               ; test for 0
			je		wt_1_Else			; skip if 0
wt_2_If:
				mov		dl,[bp+wt_BkCol]
				or		dl,dl
				je		wt_2_Else
					mov		es:[di],dl	; scrn=fgcol
				jmp		wt_2_EndIf
wt_2_Else:
					pop		dx
					push	dx
					mov		ax,gcTileHeight
					sub		ax,dx
					add		ax,24
					mov		es:[di],al	; scrn=h+24
wt_2_EndIf:

			jmp     wt_1_EndIf
wt_1_Else:
wt_3_If:
				mov	dl,[bp+wt_FgCol]
				or	dl,dl
				je	wt_3_EndIf
					mov		es:[di],dl	; scrn=bgcol
wt_3_EndIf:
wt_1_EndIf:
		add		si,1                ; font++
		add		di,1				; scrn++
		loop	wt_NextWidth
		add		di,gcScreenWidth-gcTileWidth
		pop		cx
	loop	wt_NextHeight

	popr    <cx,bx,ax>
	popr    <si,ds,di,es>
	pop     bp
	ret
_WriteTile_Char	endp

end