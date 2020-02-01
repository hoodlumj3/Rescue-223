#include "VGAExtrn.h"

int sign(int var)
{
  if (var < 0 ) return(-1); else if (var == 0) return(0); else return(1);
}

/*
**                     **********************************
**
**                     The WORK-HORSE of this little game
**
**                     **********************************
*/

void movescreen()
{
	int i,z,lev,final;
	int alls_fine_1,alls_fine_2;
	int flash;
	int delay;
		printf("\n");

		initialize();
		shifton=shift&0x03;
		score=0;
		final=0;
		alls_fine_1=1;

		while(alls_fine_1) {
			alls_fine_2=1;
			init_level();
			flash=0;
			lev=level;
			fadeoff=1;col_d=1;  /* set for fade up */

			if (level > 255) {
				fadeoff=1;col_d=2;  /* set for fade up */
				final=1;
				alls_fine_2=0;
			}
			else {
				level=loadlevel(lev);
				last_score=-1;
				lnc=-1;
				lml=-1;
				complete=0;
				crash=0;
				mode=Level.start_mode;
				sx=Level.start_x;
				sy=Level.start_y;
				ssx=sx<<3;
				ssy=sy<<3;

				incx=0;
				incy=0;

				setup_display();
				colourbar();
				drawmap(sx,sy);
				landing=1;
				landed=1;
			}

			while (alls_fine_2) {
				if(ESCAPE) {fadeoff=1;col_d=2;alls_fine_1=0; alls_fine_2=0;}
/*!ESCAPE && !(crash > 32) && !complete && !final) {*/
/*      locate(0,0); for(z=0;z<Level.num_persons;z++) printf("%0.1d",Pers[z].active);*/
/*	if (KEY_P) if (pause==1) pause=0; else pause=1;*/

				for (delay=0;delay<(gamespeed*3);delay++) Wait_VertRetr();

				if (fadeoff) {fade();}
				if (fadeoff) {fade();}
				if (fadeoff) {fade();}
				if (fadeoff) {fade();}
				if (!pause) {
				if (crash==0) {
				if (!landed && shift&0x0003 && shifton==0) {
					if (mode==0) {mode=1; lastmode=0;
				}
				else
					if (mode==2) {mode=1; lastmode=2;}
					else
						if (mode==1 && lastmode==2) mode=0;
						else
							if (mode==1 && lastmode==0) mode=2;
					shifton=1;
				}
				else if (!(shift&0x03)) shifton=0;

				if (SPACEBAR && mode != 1) { for(z=0;z<laser_count;z++) if (Laser[z].active == 0) {Laser[z].active=1;break;}}
				if (shift&0x04) {for(z=0;z<bomb_count;z++) if (Bomb[z].active == 0) {Bomb[z].active=1;break;}}

				if (ENTER) {
					enter++;
					if (enter == 1)
						if (landing == 0)
							landing = 1;
						else
							landing = 0;
					}
				else {
					enter = 0;
				}

				if (F1 && ENTER) {cheat=1; print(17*8,0*8,"Cheat On",12,0);}

				if (F2 && cheat) {alls_fine_2=0; level+=1; complete=1; col_d=2; fadeoff=1;}

				switch (mode) {
					case 0 :	/*
								** mode 0 = flying to the left
								*/
						if (!landed) {
							switch(landing) {
								case 0:
									if (LEFTARROW && incx >= -64 ) {incx-=4; if(incx <  -64) incx=-64; }
									else if (incx < 0) incx+=1;
									if (RIGHTARROW && incx <= 32 ) {incx+=4; if(incx >  32) incx=32; }
									else if (incx > 0) incx-=1;
								break;
							case 1:
									if (LEFTARROW && incx >= -8 ) {incx-=4; if(incx <  -64) incx=-64; }
									else if (incx < 0) incx+=1;
									if (RIGHTARROW && incx <=  4 ) {incx+=4; if(incx >  32) incx=32; }
									else if (incx > 0) incx-=1;
								break;
							}
						}
	/*		  if (!landing && LEFTARROW && incx >= -64 ) {incx-=4; if(incx <  -64) incx=-64; }
				else if (incx < 0) incx+=1;
				if (!landing && RIGHTARROW && incx <= 32 ) {incx+=4; if(incx >  32) incx=32; }
				else if (incx > 0) incx-=1;
	*/
						if (DOWNARROW && !landed  && incy < 32 ) incy+=2;
						if (!cheat && !UPARROW && !landed && incy < 8*gravity) incy+=1;
						if (UPARROW  && incy > -24 ) incy-=4;
						else if (!UPARROW && incy < 0) incy+=1;
						if (TAB) {if (beam > 0 && beam < 21 && beam_hit==4) beam++; else beam=1;} else beam=0;
	/*		if (SPACE) fire=1; else fire =0;*/
						ssx+=incx;
						ssy+=incy;

					break;
					case 1 :	/*
								** mode 1 = facing you but can drift
								** left/right and drop bombs
								*/
						if (!landed) {
							switch (landing) {
								case 0:
									if (LEFTARROW && incx >= -32 ) {incx-=4; if(incx <  -32) incx=-32; }
									else if (incx < 0) incx+=1;
									if (RIGHTARROW && incx <= 32 ) {incx+=4; if(incx >  32) incx=32; }
									else if (incx > 0) incx-=1;
								break;
								case 1:
									if (LEFTARROW && incx >= -4 ) {incx-=4; if(incx <  -32) incx=-32; }
									else if (incx < 0) incx+=1;
									if (RIGHTARROW && incx <= 4 ) {incx+=4; if(incx >  32) incx=32; }
									else if (incx > 0) incx-=1;
								break;
							}
						}
	/*
				if (!landing && LEFTARROW && incx >= -32 ) {incx-=4; if(incx <  -32) incx=-32; }
				else if (incx < 0) incx+=1;
				if (!landing && RIGHTARROW && incx <= 32 ) {incx+=4; if(incx >  32) incx=32; }
				else if (incx > 0) incx-=1;
	*/
						if (DOWNARROW && !landed && incy < 32 ) incy+=2;
						if (!cheat && !UPARROW && !landed && incy < 8*gravity) incy+=1;
						if (UPARROW  && incy > -24 ) incy-=4;
						else if (!UPARROW && incy < 0) incy+=1;
						ssx+=incx;
						ssy+=incy;

					break;
					case 2 :	/*
								** mode 2 = facing to the right
								*/
						if (!landed) {
							switch (landing) {
								case 0:
									if (RIGHTARROW && incx <= 64 ) {incx+=4; if(incx >  64) incx=64; }
									else if (incx > 0) incx-=1;
									if (LEFTARROW && incx >= -32 ) {incx-=4; if(incx <  -32) incx=-32; }
									else if (incx < 0) incx+=1;
									break;
								case 1:
									if (RIGHTARROW && incx <= 8 ) {incx+=4; if(incx >  64) incx=64; }
									else if (incx > 0) incx-=1;
									if (LEFTARROW && incx >= -4 ) {incx-=4; if(incx <  -32) incx=-32; }
									else if (incx < 0) incx+=1;
									break;
							}
						}
	/*
				if (!landing && RIGHTARROW && incx <= 64 ) {incx+=4; if(incx >  64) incx=64; }
				else if (incx > 0) incx-=1;
				if (!landing && LEFTARROW && incx >= -32 ) {incx-=4; if(incx <  -32) incx=-32; }
				else if (incx < 0) incx+=1;
	*/
						if (DOWNARROW && !landed && incy < 32 ) incy+=2;
						if (!cheat && !UPARROW && !landed && incy < 8*gravity) incy+=1;
						if (UPARROW  && incy > -24 ) incy-=4;
						else if (!UPARROW && incy < 0) incy+=1;
						if (TAB) {if (beam > 0 && beam < 21 && beam_hit==4) beam++; else beam=1;} else beam=0;
						ssx+=incx;
						ssy+=incy;
					break;
				}
			}
			else {  /* crash */
				if (crash == 3 ) {fadeoff=1;col_d=2;}
				ssx+=(incx+=(sign(incx)))/4;
				ssy+=8+(incy);
				for(z=0;z<2;z++) {
					add_explosion(sx+(rand()>>10)+(104-12),sy+(rand()>>12)+(56-6));
				}
				crash++;
				if (num_crafts == 1) print(124,104,"Game Over",0,0);
				if (crash > 30) alls_fine_2=0;
			}

			/*
			**  check if landed and all men collected
			*/

			if (landed) {
				if ( (men_left=check_all_persons()) == 0) {
					if (((int)sx-16) < Level.finish_x && ((int)sx+16) > Level.finish_x && Level.finish_y == sy ) { complete=1; level+=1; alls_fine_2=0;}
				}
			}

			if (ssx>>3 >0 && ssx>>3 < (mw*8)) sx=ssx>>3;
			if (ssy>>3 >0 && ssy>>3 < (mh*8)) sy=ssy>>3;
			if (fadeoff == 0 && crash == 0) {
				flash-=3;
				flash=(flash%64);
				setcolour(1,flash,flash,flash);
			}
			updatemap(sx,sy);
			draw_objects();
/*			Wait_VertRetr();*/

			Draw_Screen_2(SCREENBUF,SCREENPTR+56+(48*SCREEN_WIDTH),ww<<3,wh<<3,sx,sy,MY_SCREEN_WIDTH,MY_SCREEN_HEIGHT);
/*		  Draw_Screen(SCREENBUF,SCREENPTR+56+(48*SCREEN_WIDTH),ww<<3,wh<<3,sx,sy,MY_SCREEN_WIDTH,MY_SCREEN_HEIGHT);*/
/*		  draw_screen(sx,sy);*/
			undraw_objects();
			shift=SHIFT[0];
			draw_display();
		} /* pause */

			} /* alls_fine_2 */
		fadeoff=1;
		col_d=2;
		while(fadeoff) {Wait_VertRetr(); fade();}
		if(!complete) --num_crafts;
		if (num_crafts == 0) alls_fine_1=0;
		if (final) {final_screen(); alls_fine_1=0;}

	} /* alls_fine_1 */
}

void setup_display()
{
	word i;
		raise_box(0,0,320,189,12);
/*
		Fill_Rect(56-2,48-2,264+1,160+1,15);
		Fill_Rect(56-1,48-1,264+2,160+2,4);
		Fill_Rect(56-1,48-1,264+1,160+1,0);
*/
		raise_box(56-2,48-2,264+2,160+2,0);

		raise_box(2,30,54,41,12);
		print(4,32,"Damage",23,0);
		lower_box(17-2,48-2,37+2,126+2,12);
		for(i=0;i<10;i++) {
				raise_box(17,(i*8)+48,37,(i*8)+54,12);
		}
		print(100, 8,"SCORE : ",0,0);
		print(100,18,"CRAFT : ",0,0);
		print( 76,28,"MEN LEFT : ",0,0);
		if (cheat==1) {print(17*8,0*8,"Cheat On",0,0);}
}

void raise_box(word x1,word y1,word x2,word y2,word col)
{
		Fill_Rect(x1+0,y1+0,x2-0,y2-0,col); /* med gray */
		Fill_Rect(x1+0,y1+0,x2-1,y1+1,14);	/* white t */
		Fill_Rect(x1+0,y1+0,x1+1,y2-1,14);	/* white l*/
		Fill_Rect(x1+1,y2-1,x2+0,y2+0,10);  /* dk gray b*/
		Fill_Rect(x2-1,y1+1,x2+0,y2+0,10);  /* dk gray r*/

}
void lower_box(word x1,word y1,word x2,word y2,word col)
{
		Fill_Rect(x1+0,y1+0,x2-1,y2-1,10);
		Fill_Rect(x1+1,y1+1,x2+0,y2+0,14);
		Fill_Rect(x1+1,y1+1,x2-1,y2-1,col);
}

void draw_display()
{
	static word x,y;
		if (score != last_score) {
			sprintf(fname,"%0.8d",score);
			print(20*8,8,fname,0,0);
			last_score=score;
		}
		if (num_crafts != lnc) {
			sprintf(fname,"%0.1d",num_crafts);
			print(20*8,18,fname,0,0);
			lnc=num_crafts;
		}
		if ((men_left=check_all_persons()) != lml) {
			sprintf(fname,"%0.2d",men_left);
			print(20*8,28,fname,0,0);
			lml=men_left;
		}
		if (hit_count != lhc) {
			hit_count=lhc+1;
			if (hit_count >= 10) crash|=1;
			if (hit_count <= 10) {
				x=17;y=48+(10*8)+((hit_count-11)*8);
				lower_box(x,y,x+20,y+6,21);
			}
			lhc=hit_count;
		}

}


void open_door_2()
{
  word z,i,lx,ly1,ly2;
  static byte door_bits[16] = {0,0,35,75,75,115,0,0,0,0,30,70,31,71};
  byte d;

	for (z=0;z<Level.num_doors;z++) {

	  if (Door[z].active > 0 && (int)sx > (int)((Door[z].x*8)-232) && sx < (Door[z].x*8)+8 && (int)sy > (int)(Door[z].y*8)-144 && (int)sy < (int)(Door[z].y*8)+136) {

	  if (Door[z].active > 0 && Door[z].active < 3 ) d=0; else d=1;

	  lx=(Door[z].x)*8;
	  ly1=(Door[z].y-1)*8;
	  for(i=0;i<2;i++) {
	    if ( Door[z].type&2 && (ly1+8) > sy && ly1 < (sy+108) &&  lx    > sx && lx < (sx+232)) writetile3((lx-8)%MY_SCREEN_WIDTH,(ly1+(i*8))%MY_SCREEN_HEIGHT,door_bits[i+6+0+(d*4)]);
	    if ( Door[z].type&1 && (ly1+8) > sy && ly1 < (sy+108) && (lx+8) > sx && lx < (sx+208)) writetile3((lx+8)%MY_SCREEN_WIDTH,(ly1+(i*8))%MY_SCREEN_HEIGHT,door_bits[i+6+2+(d*4)]);
	  }

	lx=(Door[z].x*8)%MY_SCREEN_WIDTH;

	for(i=0;i<7;i++) {
	  ly1=(Door[z].y*8-(Door[z].pos>>1)-(i*8)+8);
	  ly2=(Door[z].y*8+(Door[z].pos>>1)+(i*8)-16);
	  if ((ly1+8) > sy && ly1 < (sy+108)) writetile3(lx,ly1%MY_SCREEN_HEIGHT,door_bits[7-i]);
	  if ((ly2+8) > sy && ly2 < (sy+108)) writetile3(lx,ly2%MY_SCREEN_HEIGHT,door_bits[i]);
	}
      }
      if (Door[z].active == 3) {
	Door[z].pos=0; Door[z].timer=0; Door[z].active=0;
      }
      else if (Door[z].active == 2) {
	     if (Door[z].pos > 0 )
	       Door[z].pos-=Door[z].speedc;
	     else
	       Door[z].active=3;
	   } else if (Door[z].active == 1) {
		    if (Door[z].timer < (Door[z].tcount<<1))
		      Door[z].timer++;
		    else
		      Door[z].active=2;
		    if (Door[z].pos < 32) Door[z].pos+=Door[z].speedo;
		  }

/*      if (Door[z].active == 1) Door[z].active=2;*/

    }
}

byte draw_object(char far *Object, struct BUFF *Buffer, word x, word y, word width, word height, word rw)
{
  register char i,j;
  register word blka=0,blkb=0;
  register word scrn;
  int colis=0;

    Buffer->x= x;
    Buffer->y= y;
    Buffer->w= width;
    Buffer->h= height;

    for(j=0;j<height;j++) {
      for(i=0;i<width;i++) {
	scrn=(((x+i)%MY_SCREEN_WIDTH)+(((y+j)%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH));
	Buffer->buff[blka]=SCREENBUF[scrn]; /* save off background to buffer */

/*
** write a quick mask of object to buffer
*/

	if (Object[blkb] != 0) {
	  if (SCREENBUF[scrn] == 0 ) {
	    SCREENBUF[scrn] = Object[blkb];
	  }
	  else {
	    if (!colis) {
	      colis=1;
	      globx=x+i;
	      globy=y+j;
/*	    colis=death_table[MAP[((x+i)/th)+(((y+j)/th)*mw)]];
	    locate(1,1); printf("%0.1d %0.6d ",colis,cnt++);
*/
	    }
	  }
	}

/*      if (Object[blkb] != 0)
	  SCREENBUF[scrn] = Object[blkb];
	else if (SCREENBUF[scrn] == 0 )
	       collision(x+i,y+j);
*/
	blkb++;
	blka++;
      }
      blkb+=rw-width;
    }
   return(colis);
/*   return(0);*/
}

byte draw_object_2(char far *Object, struct BUFF *Buffer, word x, word y, word width, word height, word rw)
{
  register char i,j;
  register word blka=0,blkb=0;
  register word scrn;
  int colis=0;

    Buffer->x= x;
    Buffer->y= y;
    Buffer->w= width;
    Buffer->h= height;

    for(j=0;j<height;j++) {
      for(i=0;i<width;i++) {
	scrn=(((x+i)%MY_SCREEN_WIDTH)+(((y+j)%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH));
	Buffer->buff[blka]=SCREENBUF[scrn]; /* save off background to buffer */

	if (Object[blkb] != 0)
	    SCREENBUF[scrn] = Object[blkb];
	blkb++;
	blka++;
      }
      blkb+=rw-width;
    }
   return(colis);
}

void undraw_object(struct BUFF *Buffer)
{

  register char i,j;
  register word blka=0;
  register word scrn;
  word x,y,width,height;
    x=Buffer->x;
    y=Buffer->y;
    width=Buffer->w;
    height=Buffer->h;

    for(j=0;j<height;j++) {
      for(i=0;i<width;i++) {
	scrn=(((x+i)%MY_SCREEN_WIDTH)+(((y+j)%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH));
	SCREENBUF[scrn]=Buffer->buff[blka];
	blka++;
      }
    }

}

void undraw_object_2(struct BUFF *Buffer)
{

  register char i,j;
  register word blka=0;
  register word scrn;
  word x,y,width,height;
    x=5; /*Buffer->x;*/
    y=70; /*Buffer->y;*/
    width=Buffer->w;
    height=Buffer->h;


    locate(0,4);
    printf("x:%0.2d y:%0.2d w:%0.2d h:%0.2d %0.8lx %0.8lx\r ",x,y,width,height,Buffer,Buffer->buff);

    for(j=0;j<height;j++) {
      for(i=0;i<width;i++) {
	scrn=(((x+i)%SCREEN_WIDTH)+(((y+j)%SCREEN_HEIGHT)*SCREEN_WIDTH));
	SCREENPTR[scrn]=Buffer->buff[blka];
	blka++;
      }
    }


}


void draw_objects()
{
    if (beam) draw_beam(sx,sy,mode);
    open_door_2();

    draw_enemy();
    draw_persons();
    draw_laser();
    draw_bomb();
    draw_craft(sx,sy,incx,incy,mode);
    draw_bullet();
    draw_explosion();
}


void undraw_objects()
{
    undraw_explosion();
    undraw_bullet();
    undraw_craft();
    undraw_bomb();
    undraw_laser();
    undraw_persons();
    undraw_enemy();
}
