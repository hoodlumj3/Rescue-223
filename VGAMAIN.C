#include "VGAExtrn.h"

#define M_INSTRUCTIONS	1
#define M_OPTIONS		2
#define M_START			3
#define M_QUIT			4

#define O_GRAVITY		1
#define O_GAMESPEED		2
#define O_QUIT			3

#define I_QUIT			1

main(int argc)
{
  int i,ok;
  int far *a,*b;

/* Setup_ASM_Vars(SCREENPTR,SCREENBUF,TILES,MAP,CRAFT,FONT);*/


	setmode(3);
	strcpy(fname,decode(TITLE));
	locate((78-strlen(fname))/2,6); printf("%s",fname);
	locate((78-16)/2,8); printf("DEMO VERSION FOR");
	strcpy(fname,decode(REGISTERED));
	locate((78-strlen(fname))/2,10); printf("%s",fname);
	locate(0,23);
	if (argc > 1) exit(0);

		setup();
		gravity=1;     /* gravity on */
		gamespeed=0;   /* hi speed */

		Setup_ASM_Vars(SCREENPTR,SCREENBUF,TILES,MAP,CRAFT,FONT);

		Install_Game_Key();

		setcolours(&fadecols[0]);
		key=Game_Key();
		ok=0;
		while( ok != M_QUIT ) {
			ok=introscreen();
			switch (ok) {
				case M_START :
					movescreen();
					break;
				case M_OPTIONS :
					optionscreen();
					break;
				case M_INSTRUCTIONS :
					instructscreen();
					break;
			}
/*			if ( ok == M_START ) {
				movescreen();
				//while(ESCAPE);
			} else
			  if ( ok == M_OPTIONS ) {
				  optionscreen();
				  //while(ESCAPE);
			  } else
				if ( ok == M_INSTRUCTIONS ) {
					instructscreen();
				}
*/
				while (ESCAPE);

		}


		Uninstall_Game_Key();

		cleanup("Enjoy - ME.\n",0);
		return;
}

void setup()
{
		int i;
/*    for(i=0;i<256;i++) setcolour(i,63,63,63);*/
		locate(35,15);printf("Loading...");
		nosound();

		if ((MEMORY_AREA1=farmalloc((long)32768)) == NULL) cleanup("Couldn't Get Memory Area #1 (32k)\n",1);
		if ((MEMORY_AREA2=farmalloc((long)32768)) == NULL) cleanup("Couldn't Get Memory Area #2 (32k)\n",1);
		if ((MEMORY_AREA3=farmalloc((long)32768)) == NULL) cleanup("Couldn't Get Memory Area #3 (32k)\n",1);
		score = 0;
		ww=(MY_SCREEN_WIDTH>>3)-4;           /* display window width  */
		wh=(MY_SCREEN_HEIGHT>>3)-3;          /* display window height */
		loadgfx();
		fastkey();
		oldmode=getmode();
		setmode(19);

}

void cleanup(char *errstr,int errno)
{
  unsigned int far *b;
  int i;
		setmode(0x03);

/*		b = (int far *)Get_CharOffsets();
		printf("%08lx\n",b);
		for(i=0;i<200;i++) printf("%04x : ",*(b++));
*/
		if (MEMORY_AREA1 != 0L )  farfree(MEMORY_AREA1);
		if (MEMORY_AREA2 != 0L )  farfree(MEMORY_AREA2);
		if (MEMORY_AREA3 != 0L )  farfree(MEMORY_AREA3);
		nosound();
		printf(errstr);
		exit(errno);
}

void setcolour(byte colour,byte red,byte green,byte blue)
{
		outportb(0x3c8,colour);
		outportb(0x3c9,red);
		outportb(0x3c9,green);
		outportb(0x3c9,blue);
}

void colourbar()
{
	int i,j,k;
		for(i=0;i<nc;i++)
			for(j=0;j<8;j++)
	for (k=0;k<8;k++)
		SCREENPTR[k+(j*320)+(i*4)]=i;

}
/*
void matts_keys(void)
{
		regs.h.ah = 3;
	regs.h.al = 4;
		regs.h.bh = 0;
		regs.h.bl = 0;
		int86 (0x16,&regs,&regs);
}
void slowkey(void)
{
		regs.h.ah = 3;
	regs.h.al = 5;
	regs.h.bh = 3;
	regs.h.bl = 0x1f;
		int86 (0x16,&regs,&regs);
}
*/
void fastkey(void)
{
		regs.h.ah = 3;
	regs.h.al = 5;
		regs.h.bh = 0;
		regs.h.bl = 0;
		int86 (0x16,&regs,&regs);
}

void updatemap(int x,int y)
{
	static int last_x,last_y;
	unsigned int mp=0,i;
/*    printf("%0.3d %0.3d %1.3d %1.3d %0.4d\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b",x,y,x-last_x,y-last_y,cnt++);*/
/*    printf("%0.2d %0.2d   %c",sign(x-last_x),sign(y-last_y),13);*/
		mp=(x/tw)+((y/th)*mw);
		if ((x-last_x) < 0 ) {
			for (i=0;i<MY_SCREEN_HEIGHT>>3;i++) {
		writetile((x&0xfff8)%MY_SCREEN_WIDTH,((y&0xfff8)+(i*th))%MY_SCREEN_HEIGHT,MAP[mp]);
		mp+=mw;
		}
		}
		else/* if ((x-last_x) > 0 )*/ {
			mp+=((MY_SCREEN_WIDTH-8)>>3);
			for (i=0;i<MY_SCREEN_HEIGHT>>3;i++) {
		writetile(((x+(MY_SCREEN_WIDTH-8))&0xfff8)%MY_SCREEN_WIDTH,((y&0xfff8)+(i*th))%MY_SCREEN_HEIGHT,MAP[mp]);
		mp+=mw;
			}
		}

		mp=(x/tw)+((y/th)*mw);
		if ((y-last_y) < 0) {
			for (i=0;i<MY_SCREEN_WIDTH>>3;i++) {
		writetile((((x>>3)<<3)+(i*tw))%MY_SCREEN_WIDTH,((y>>3)<<3)%MY_SCREEN_HEIGHT,MAP[mp++]);
			}
		}
		else if ((y-last_y) > 0) {
			mp+=((MY_SCREEN_HEIGHT-8)>>3)*mw;
			for (i=0;i<MY_SCREEN_WIDTH>>3;i++) {
		writetile((((x>>3)<<3)+(i*tw))%MY_SCREEN_WIDTH,(((y+(MY_SCREEN_HEIGHT-8))>>3)<<3)%MY_SCREEN_HEIGHT,MAP[mp++]);
			}
		}
		last_x=x;
		last_y=y;
}


void draw_screen_2(unsigned int x,unsigned int y)
{

		unsigned int i,j,Offset=56+(48*SCREEN_WIDTH),Offset2=0;
		unsigned int dup1,dup2;

		unsigned int a=x%MY_SCREEN_WIDTH,b=MY_SCREEN_WIDTH-a;
		unsigned int c=y%MY_SCREEN_HEIGHT,d=MY_SCREEN_HEIGHT-c;
/*
		Offset2=c*MY_SCREEN_WIDTH;
		for(j=0;j<d;j++) {
			if (j<(wh<<3)) {
	if (b>0)
		for(i=0;i<b;i++) if ( i<(ww<<3) ) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i]; else break;
	if (a>8)
		for(i=0;i<a-8;i++) SCREENPTR[Offset+b+i]=SCREENBUF[(Offset2+i)];

	Offset+=SCREEN_WIDTH;
	Offset2+=MY_SCREEN_WIDTH;
			}
		}
		Offset2=0;
		if (c>8) {
			for(j=0;j<c-8;j++) {
	if (b>0)
		for(i=0;i<b;i++) if ( i<(ww<<3) ) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i]; else break;
	if (a>8)
		for(i=0;i<a-8;i++) SCREENPTR[Offset+b+i]=SCREENBUF[Offset2+i];
	Offset+=SCREEN_WIDTH;
	Offset2+=MY_SCREEN_WIDTH;
			}
		}
*/
/*    printf("a:%0.2d b:%0.2d c:%0.2d d:%0.2d \r",a,b,c,d);*/

/*    for(i=0;i<wh<<3;i++){
		dup2=(((i+y)%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH);
			for(j=0;j<ww<<3;j++){
	dup1= ((j+x)%MY_SCREEN_WIDTH);
	SCREENPTR[Offset+j+   0]=SCREENBUF[dup1+dup2];
			}
			Offset+=SCREEN_WIDTH;
		}
*/
		Offset2=c*MY_SCREEN_WIDTH;
		for(j=0;j<d;j++) {
			for(i=0;i<b;i++) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i];
			for(i=0;i<a;i++) SCREENPTR[Offset+b+i]=SCREENBUF[(Offset2+i)];
			Offset+=SCREEN_WIDTH;
			Offset2+=MY_SCREEN_WIDTH;
		}
		Offset2=0;
		for(j=0;j<c;j++) {
			for(i=0;i<b;i++) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i];
			for(i=0;i<a;i++) SCREENPTR[Offset+b+i]=SCREENBUF[Offset2+i];
			Offset+=SCREEN_WIDTH;
			Offset2+=MY_SCREEN_WIDTH;
		}

}
/*
void draw_screen(unsigned int x,unsigned int y)
{

		unsigned int i,j,Offset=56+(48*SCREEN_WIDTH),Offset2=0;
		unsigned int dup1,dup2;

		unsigned int a=x%MY_SCREEN_WIDTH,b=MY_SCREEN_WIDTH-a;
		unsigned int c=y%MY_SCREEN_HEIGHT,d=MY_SCREEN_HEIGHT-c;

		Offset2=c*MY_SCREEN_WIDTH;
		for(j=0;j<d;j++) {
			if (j<(wh<<3)) {
	if (b>0)
		for(i=0;i<b;i++) if ( i<(ww<<3) ) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i]; else break;
	if (a>16)
		for(i=0;i<a-16;i++) SCREENPTR[Offset+b+i]=SCREENBUF[(Offset2+i)];

	Offset+=SCREEN_WIDTH;
	Offset2+=MY_SCREEN_WIDTH;
			}
		}
		Offset2=0;
		if (c>16) {
			for(j=0;j<c-16;j++) {
	if (b>0)
		for(i=0;i<b;i++) if ( i<(ww<<3) ) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i]; else break;
	if (a>16)
		for(i=0;i<a-16;i++) SCREENPTR[Offset+b+i]=SCREENBUF[Offset2+i];
	Offset+=SCREEN_WIDTH;
	Offset2+=MY_SCREEN_WIDTH;
			}
		}

/*    printf("a:%0.2d b:%0.2d c:%0.2d d:%0.2d \r",a,b,c,d);*/

/*    for(i=0;i<wh<<3;i++){
		dup2=(((i+y)%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH);
			for(j=0;j<ww<<3;j++){
	dup1= ((j+x)%MY_SCREEN_WIDTH);
	SCREENPTR[Offset+j+   0]=SCREENBUF[dup1+dup2];
			}
			Offset+=SCREEN_WIDTH;
		}
*/
/*    Offset2=c*MY_SCREEN_WIDTH;
		for(j=0;j<d;j++) {
			for(i=0;i<b;i++) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i];
			for(i=0;i<a;i++) SCREENPTR[Offset+b+i]=SCREENBUF[(Offset2+i)];
			Offset+=SCREEN_WIDTH;
			Offset2+=MY_SCREEN_WIDTH;
		}
		Offset2=0;
		for(j=0;j<c;j++) {
			for(i=0;i<b;i++) SCREENPTR[Offset+i]=SCREENBUF[Offset2+a+i];
			for(i=0;i<a;i++) SCREENPTR[Offset+b+i]=SCREENBUF[Offset2+i];
			Offset+=SCREEN_WIDTH;
			Offset2+=MY_SCREEN_WIDTH;
		}
*/
}
*/

void Fill_Rect(word x1,word y1,word x2,word y2,word col)
{
		Fill_Rectangle(x1,y1,x2,y2,col,SCREENPTR);
/*  unsigned int x,y;
		for (y=y1;y<y2;y++)
			for (x=x1;x<x2;x++)
		SCREENPTR[((y*SCREEN_WIDTH)+x)]=col;
*/
}

void locate(int x, int y)
{
	regs.h.ah = 0x02;
	regs.h.bh = 0;
  regs.h.dh = y;
	regs.h.dl = x;
  int86(0x10, &regs, &regs);
}

void writetile(int x, int y, byte block)
{
  register char i,j;
  register word blk=(int)(block*(tw*th));
  register word scrn=x+(y*MY_SCREEN_WIDTH);
	j=th;
	while (j--) {
		i=tw;
		while (i--) {
			SCREENBUF[scrn++]=TILES[blk++];
		}
		scrn+=MY_SCREEN_WIDTH-tw;
	}
}

void writetile2(int x, int y, byte block)
{
  register char i,j;
  register word blk=(int)(block*(tw*th));
  register word scrn=x+(y*SCREEN_WIDTH);
		j=th;
	while (j--) {
			i=tw;
	  while (i--) {
	SCREENPTR[scrn++]=TILES[blk++];
	  }
	  scrn+=SCREEN_WIDTH-tw;
	}
}

void writetile3(int x, int y, byte block)
{
  register char i,j;
	register word blk=(int)(block*(tw*th));
  register word scrn=x+(y*MY_SCREEN_WIDTH);
		for(j=0;j<th;j++) {
	  for(i=0;i<tw;i++) {
	scrn=(((x+i)%MY_SCREEN_WIDTH)+(((y+j)%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH));
	SCREENBUF[scrn] = TILES[blk++];
	  }
	}
}

void drawmap(int x, int y)
{
  word i,j;
	word mp=0;
	mp+=(x/tw)+((y/th)*mw);
		for(j=0;j<MY_SCREEN_HEIGHT>>3;j++) {           /* y */
	  for(i=0;i<MY_SCREEN_WIDTH>>3;i++) {         /* x */
	writetile(((x+(i*tw))%MY_SCREEN_WIDTH)&0xfff8,((y+(j*th))%MY_SCREEN_HEIGHT)&0xfff8,MAP[mp++]);
	  }
	  mp+=mw-(MY_SCREEN_WIDTH>>3);
	}
}


void setcolours(byte *cols)
{
	int cl;
		for (cl=0;cl<64;cl++) setcolour(cl,cols[cl*3+0],cols[cl*3+1],cols[cl*3+2]);
}

void fade()
{

/*
** col_d = 1 : fade up from fade_cols
** col_d = 2 : fade down from fadecols
**
**
*/
	char i,buff[100];
	int cl;
	if (fadeoff) {
		if (col_d == 1) {     /* fade up */
			if (cs==0) {      /* set all colours */
				setcolours(&fadecols[0]);
				cs=1;         /* signal colours setup */
			}

			if (cs < 64) {    /* if still fading */
				for (cl=0;cl<nc*3;cl++) {
					if (fadecols[cl] < RGB[cl] ) fadecols[cl]++;
				}
				setcolours(&fadecols[0]);
				if (cs <= 64) cs++;
			}
			else {
				fadeoff=0;   /* no more fading */
				col_d=2;     /* setup to fade down */
			}
		}
		else {
			if (cs == 65) {
/*	for (cl=0;cl<nc*3;cl++) {
	  fadecols[cl]=RGB[cl];
	}
*/
				setcolours(&fadecols[0]);
				cs=65;
			}

			if (cs) {       /* fade down */
				for (cl=0;cl<nc*3;cl++) {
					if (fadecols[cl] > 0 ) fadecols[cl]--;
				}
				setcolours(&fadecols[0]);
				if (cs > 0 ) cs--;
			}
			else { fadeoff=0; col_d=1;}
		}
	}
		if (SPACEBAR) for(i=0;i<15;i++) Wait_VertRetr();
/*		sprintf(buff,"%0.1d %0.2d %0.2d",fadeoff,cs,col_d);
		print(0,10,buff,67,243);
*/
}
/*
**  not used, equal in vgagfxhd.asm  WriteTile_Char
**
void writetile_ch(word x, word y, byte block,byte col1,byte col2)
{
/*  static byte char_dat[64] = {
		   0,1,2,3,4,5,6,7,
		   1,2,3,4,5,6,7,6,
		   2,3,4,5,6,7,6,5,
		   3,4,5,6,7,6,5,4,
		   4,5,6,7,6,5,4,3,
			 5,6,7,6,5,4,3,2,
		   6,7,6,5,4,3,2,1,
		   7,6,5,4,3,2,1,0
		 };
*/
  register char i,j;
  register word blk=(int)(block*(tw*th));
  register word scrn=x+(y*SCREEN_WIDTH);
	for(j=0;j<th;j++) {
	  for(i=0;i<tw;i++) {
/*	if(TILES[blk]) SCREENPTR[scrn]=char_dat[(j*8)+i]+24; else if (col < 255) SCREENPTR[scrn]=col;*/
/*	if (col1<255)  */

		if(TILES[blk])
			if ( col1 > 0 ) SCREENPTR[scrn]=col1;
			else SCREENPTR[scrn]=j+24; /* back colour */
		else
			if ( col2 > 0 ) SCREENPTR[scrn]=col2;
/*			else SCREENPTR[scrn]=j+24;
*/
	blk++;
	scrn++;
	  }
	  scrn+=SCREEN_WIDTH-tw;
	}
/*    j=th;
	while (j--) {
	  i=tw;
	  while (i--) {
	SCREENPTR[scrn++]=TILES[blk++];
	  }
	  scrn+=SCREEN_WIDTH-tw;
	}
*/
}
*/
void print(word x, word y, char * str, byte chrcol, byte backcol)
{
  int i;

	for(i=0;str[i]>0;i++) {
/*	  writetile_ch(x,y,((toupper(str[i]))-32)+120,chrcol,backcol);*/
	  WriteTile_Char(x,y,((toupper(str[i]))-32),chrcol,backcol);
	  x+=8;
	}
}

void highlight_menu(char *st,int o,int c1,int c2) {
/*
	int a;
	a  = c1;
	c1 = c2;
	c2 = a;
*/
	if ( c1 > 0 ) {
		raise_box(100,((6+o)*10)-2,220,((6+o)*10)+10-1,c1);
	}
	else {
		Fill_Rect(100,((6+o)*10)-2,220,((6+o)*10)+10-1,c1);
	}
	print((320-(strlen(st)*8))/2,(6+o)*10,st,c2,c1);

}
void place_on_off_in_string(char *s, int oo, int sp) {
	if (oo == 0) {
		*(s+sp+0) = 'O';
		*(s+sp+1) = 'F';
		*(s+sp+2) = 'F';
	}
	else {
		*(s+sp+0) = 'O';
		*(s+sp+1) = 'N';
		*(s+sp+2) = ' ';
	}
}

void place_hi_lo_in_string(char *s, int oo, int sp) {
	if (oo == 0) {
		*(s+sp+0) = 'H';
		*(s+sp+1) = 'I';
	}
	else {
		*(s+sp+0) = 'L';
		*(s+sp+1) = 'O';
	}
}


/*
#define highlight_menu(o,c1,c2)		if (c1 > 0) {raise_box(100,((6+o)*10)-2,220,((6+o)*10)+10-1,c1);}  \
									else {Fill_Rect(100,((6+o)*10)-2,220,((6+o)*10)+10-1,c1);}         \
									print((320-(strlen(items[o])*8))/2,(6+o)*10,items[o],c2,c1);
*/
int optionscreen()
{
	word ok=0,old_key=-1,option=O_GRAVITY,i;
	static char *items[] = {"Gravity : 000","Speed : 00","Back To Main"};
	char *s;
	char buff[80];

		Fill_Rect(0,40,320,189,0);             /* clear screen */
		place_on_off_in_string(items[O_GRAVITY-1],gravity,10);
		place_hi_lo_in_string(items[O_GAMESPEED-1],gamespeed,8);

											  /* Draw menu options */
		for (i=0;i<O_QUIT;i++) {
			print((320-(strlen(items[i])*8))/2,(6+i)*10,items[i],0,0);
		}

		highlight_menu(items[option-1],option-1,12,1);     /* turn off */

		fadeoff=1;                         /* start fade in */
		while( !ok ) {
			Wait_VertRetr();
			if (fadeoff) fade();           /* execute the fade routine */
			key=Game_Key();                /* get a key from keyboard */
/*			sprintf(buff,"%04x : %04x : %04x : %04x : %10s",Game_Key(),old_key, option, gravity,items[O_GRAVITY-1]);*/
/*			sprintf(buff,"%10s",items[O_GRAVITY-1]);
			print(0,0,buff,23,1);
*/
			if (Game_Key() == 0) old_key=0;  /* if this key is no key then old key is no key */

/*
*** I use if statements here because
*** I can't get the case statements to work
*** properley ..... Honest.
*/

			if (Game_Key() && (old_key&0x80ff) != (key&0x80ff) ) {
				highlight_menu(items[option-1],option-1,0,0);     /* turn off menu option */

				if ( ENTER ) {
					if(option == O_GRAVITY) {
						if ( gravity==1 ) {
							gravity = 0;
						}
						else {
							gravity = 1;
						};
						place_on_off_in_string(items[O_GRAVITY-1],gravity,10);
					}
					if (option == O_GAMESPEED) {
						if ( gamespeed==1 ) {
							gamespeed=0;
						}
						else {
							gamespeed=1;
						}
						place_hi_lo_in_string(items[O_GAMESPEED-1],gamespeed,8);
					}
					if (option == O_QUIT) ok=option;
				} else
				if ( UPARROW ) {
					option--;
					if ( option == 0 ) option = O_QUIT;
				} else
				if ( DOWNARROW ) {
					option++;
					if ( option == O_QUIT+1 ) option=1;
				} else
				if ( ESCAPE ) {
					ok=O_QUIT;
				}
				highlight_menu(items[option-1],option-1,12,1);    /* turn on */
				old_key=key;
			}
		}
		fadeoff=1; col_d=2;
		while(fadeoff) {Wait_VertRetr(); fade();}
		Fill_Rect(0,0,320,189,0);
/*    setcolours(&RGB[0]);*/
		return(0);
}

int instructscreen()
{
	return(0);
}

int introscreen()
{
	word old_key=0;
	int l,option,ok=0,i;
	char buff[100];
	static char *items[4] = {"Instructions","Options","Start","Quit To Dos"};

		Fill_Rect(0,0,320,189,0);

		raise_box(0,189,320,200,12);
		print(  16, 191,decode(TITLE),0,0);

		print(120,   0,"Rescue 223",0,0);
		print(108,  14,"Programmed by",0,0);
		strcpy(fname,decode(PROGRAMMER));
		print((320-(strlen(fname)*8))/2,  24,fname,0,0);

		for (i=0;i<4;i++) {
			print((320-(strlen(items[i])*8))/2,(6+i)*10,items[i],0,0);
		}

/*		print(  8,  6*8,"Instructions -",0);
		print(  4, (7*8)+1,"Fly Around In Your Rescue Ship And Pick",0);
		print(  4, (8*8)+2, "Up Your Stranded Men, Avoid The Enemy!",0);


		print( 10*8, (10*8), "- Controls -",0);
		print( 5*8, (11*8), "Up Arrow    - Go UP",0);
		print( 5*8, (12*8), "Down Arrow  - Go Down",0);
		print( 5*8, (13*8), "Left Arrow  - Go Left",0);
		print( 5*8, (14*8), "Right Arrow - Go Right",0);
		print( 5*8, (15*8), "Shift       - Rotate Ship",0);
		print( 5*8, (16*8), "Return      - Landing Gear Up/Down",0);
		print( 5*8, (17*8), "Tab         - IR Beam",0);
		print( 5*8, (18*8), "Space       - Lazer",0);
		print( 5*8, (19*8), "Ctrl        - Bombs",0);

		print(10*8,22*8,"Press Space To Begin",0);
*/
		option=M_START;
		highlight_menu(items[option-1],option-1,12,1);

/*		raise_box(20,((6+option)*10)-2,300,((6+option)*10)+10-1,12);
		print(((40-strlen(items[option]))/2)*8,(6+option)*10,items[option],0,12);
*/
		fadeoff=1;col_d=1;
		while( !ok ) {
			Wait_VertRetr();
			if (fadeoff) fade();
			key=Game_Key();
/*
			sprintf(buff,"%04x",Game_Key());
			print(0,0,buff,23,1);
*/
			if (Game_Key() == 0) old_key=0;
			if (Game_Key() && (old_key&0x80ff) != (key&0x80ff) ) {
				highlight_menu(items[option-1],option-1,0,0);     /* turn off */

				if (ENTER) {
					ok=option;
				} else
				if ( UPARROW ) {
					option--;
					if (option == 0 ) option = 4;
				} else
				if ( DOWNARROW ) {
					option++;
					if ( option == 5) option=1;
				} else
				if ( ESCAPE ) {
					ok=M_QUIT;
				}
				highlight_menu(items[option-1],option-1,12,1);    /* turn on */
				old_key=key;
			}
		}
		fadeoff=1; col_d=2;
		while(fadeoff) {Wait_VertRetr(); fade();}
/*		Fill_Rect(0,0,320,189,0); */
/*    setcolours(&RGB[0]);*/
	return(ok);
}

byte flipbits(byte ch)
{
  byte j,ch2;
	ch2=ch;
	ch=0;
	  for(j=0;j<8;j++) {
	ch|=(ch2&0x80);
	if (j != 7) {ch>>=1;ch2<<=1;}
	  }
	  return(ch);
}

byte *decode(byte *text)
{

  byte str[90],i,ch1,ch2,c=0,crc=0;
  crc++;
  c++;
/*
	for(i=0;i<strlen(text)/2;i++) {
	  str[i]=0;
	  ch1=(flipbits(text[c++]-i));
	  ch2=(text[c++]-i);
	  str[i]=ch2|ch1;
	  crc+=str[i];
	}
	str[i]=0;

	if (text[c] != crc) cleanup("Nope",0);
*/
	strcpy(str,text);
	return(str);
}

void final_screen()
{
	Fill_Rect(0,0,320,189,0);
	print(96,10,"Congratulations",0,0);
	print(48,30,"You have completed the demo",0,0);
	fadeoff=1; col_d=1;
	while(!ANYKEY && !ESCAPE) {
	  Wait_VertRetr();
	  Wait_VertRetr();
	  if (fadeoff) fade();
	}
	key=0;
	fadeoff=1; col_d=2;
	while(fadeoff) {Wait_VertRetr(); fade();}
}
