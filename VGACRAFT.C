#include "VGAExtrn.h"

void initialize()
{
    cheat=0;
    randomize();
    level=1;
    num_crafts=3;
    init_level();
}
void init_level()
{
  int i;
    hit_count=0;
    lhc=0;
	bomb_count=5;
	laser_count=5;
    expl_count=25;
    bullet_count=25;

    Craft_Buff.buff = craft_buff;

    for(i=0;i<bomb_count;i++) {
      Bomb[i].active=0;
      Bomb_Buff[i].buff = &bomb_buff[i*15];
    }
    for(i=0;i<laser_count;i++) {
      Laser[i].active=0;
      Laser_Buff[i].buff = &laser_buff[i*48];
    }
    for(i=0;i<expl_count;i++) {
      Expl[i].active=0;
      Expl_Buff[i].buff = &expl_buff[i*99];
      Expl[i].type=(i&0x01);
    }
    for(i=0;i<Level.num_enemys;i++) {
      Enemy[i].active=1;
      Enemy_Buff[i].buff = &enemy_buff[i*64];
    }
    for(i=0;i<bullet_count;i++) {
      Bullet[i].active=0;
      Bullet_Buff[i].buff = &bullet_buff[i*1];
    }
}

word check_craft(word x,word y)
{
	if (x > (sx+104-18) && x < (sx+104+18) && y > (sy+56-10) && y < (sy+56+10)) {
	  hit_count+=1;
	  return(1);
	}
	else {
	  return(0);
	}
}

void draw_craft(word x, word y,int ix,int iy,word mode)
{
  static byte mode0[6*5] = { 0, 90,31,16,15, 8,
			     0,106,32,13,16, 6,
			     0,119,33,10,16, 5,
			     0,129,34, 8,17, 3,
			     0,184,34,12,17, 5 };
  static byte mode1[6*9] = { 0,  0,29,13,14, 6,
			     0, 13,29,11,14, 5,
			     0, 24,29,10,14, 5,
			     0, 34,29,10,14, 5,
			     0, 34,29,10,14, 5,
			     0, 44,29,10,14, 5,
			     0, 54,29,11,14, 5,
			     0, 65,29,13,14, 6,
			     0, 78,29,12,14, 5 };
  static byte mode2[6*5] = { 0,137,31,16,15, 8,
			     0,153,32,13,16, 6,
			     0,166,33,10,16, 5,
			     0,176,34, 8,17, 3,
			     0,196,34,12,16, 5 };

  word offset;
  byte colis=0,ok=0;

    switch (mode) {
      case 0:
	  if (landing) { offset=4*6; flypos=2;}
	  else {
	    if (ix >=-64 && ix <-30) flypos=0;
	    if (ix >=-30 && ix <-10) flypos=1;
	    if (ix >=-10 && ix < 10) flypos=2;
	    if (ix >= 10 && ix <=64) flypos=3;
	    offset=flypos*6;
	  }
	  check_under_craft(x+104-mode0[offset+4],y+56-mode0[offset+5],mode0[offset+2],mode0[offset+3],4,19);
	  colis=draw_object(CRAFT+(mode0[offset]+(mode0[offset+1]*cw)),&Craft_Buff,x+104-mode0[offset+4],y+56-mode0[offset+5],mode0[offset+2],mode0[offset+3],cw);

	break;
      case 1:
	  if (ix>31) ix=31;
	  if (ix<-31) ix=-31;
	  if (landing) offset=8*6; else offset=((ix+32)/8)*6;
	  check_under_craft(x+104-mode1[offset+4],y+56-mode1[offset+5],mode1[offset+2],mode1[offset+3],8,20);
	  colis=draw_object(CRAFT+(mode1[offset]+(mode1[offset+1]*cw)),&Craft_Buff,x+104-mode1[offset+4],y+56-mode1[offset+5],mode1[offset+2],mode1[offset+3],cw);
	break;
      case 2:
	  if (landing) {offset=4*6;flypos=2;}
	  else {
	    if (ix >=-64 && ix <-10) flypos=3;
	    if (ix >=-10 && ix < 10) flypos=2;
	    if (ix >= 10 && ix < 30) flypos=1;
	    if (ix >= 30 && ix <=64) flypos=0;
	    offset=flypos*6;
	  }
	  check_under_craft(x+104-mode2[offset+4],y+56-mode2[offset+5],mode2[offset+2],mode2[offset+3],14,29);
	  colis=draw_object(CRAFT+(mode2[offset]+(mode2[offset+1]*cw)),&Craft_Buff,x+104-mode2[offset+4],y+56-mode2[offset+5],mode2[offset+2],mode2[offset+3],cw);
	break;
      }
    if ( colis == 1 ) {
      ok=check_enemy(globx,globy);
      if (ok) crash |=1;
      colis=death_table[MAP[(globx/th)+((globy/th)*mw)]];

      if (!cheat) if (colis >=1 && colis <= 2 || colis >= 4)  crash |=1;
/*      locate(1,3); printf("%0.2d\r",colis);*/

    }

    iy=iy+iy-iy;
}

void undraw_craft()
{
  undraw_object(&Craft_Buff);
}

void check_under_craft(int x,int y,int w,int h,int pos1,int pos2)
{
    w=w+w-w;
/*    SCREENPTR[(x+56-sx+pos1)  +((y+48+h-sy)*320)]=0x0F;
    SCREENPTR[(x+56-sx+pos2)+((y+48+h-sy)*320)]=0x0F;
*/
    if(death_table[MAP[((x+pos1)/th)+(((y+h)/th)*mw)]] == 2 && death_table[MAP[((x+pos2)/th)+(((y+h)/th)*mw)]] == 2 )
    {
    if (landing) landed=1; else crash |= 1;
      incy=0; incx=0;
    }
    else {
      landed = 0;
    }

}

byte draw_beam(word x, word y, byte mode)
{
  static char beam_dat[4*3] = { -1,1,7,-1,1,6,0,0,4,0,0,1 };
  word i=0,ok=0,z,j,k,bx,by,dx,dy;
  word scrn;

    while ( ok==0 ) {
      switch (mode) {
	case 0:
	    dx=x+104-17-i-beam_dat[flypos*3+0];
	    dy=y+56+beam_dat[flypos*3+2];
	    scrn=((dx%MY_SCREEN_WIDTH)+((dy%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH));
	    SCREENPTR[(dx-sx+56)  +((dy-sy+48)*320)]=rand()*255;
	    if(SCREENBUF[scrn] !=0) {
	      for(z=0;z<Level.num_doors;z++) {
		bx=Door[z].x;
		by=Door[z].y;
		beam_hit=death_table[MAP[(dx/th)+((dy/th)*mw)]];
		if( bx >= (dx>>3)-1 && bx <= (dx>>3)+1 && by >= (dy>>3)-1 && by <= (dy>>3)+1 && beam >= 1 && beam_hit==4 && Door[z].active==0) Door[z].active=1;
	      }
	    return(death_table[MAP[(dx/th)+((dy/th)*mw)]]);

	    }
	  break;
	case 2:
	    dx=x+104+17+i-beam_dat[flypos*3+1];
	    dy=y+56+beam_dat[flypos*3+2];
	    scrn=((dx%MY_SCREEN_WIDTH)+((dy%MY_SCREEN_HEIGHT)*MY_SCREEN_WIDTH));
	    SCREENPTR[(dx-sx+56)  +((dy-sy+48)*320)]=rand()*255;
	    if(SCREENBUF[scrn] !=0) {
	      for(z=0;z<Level.num_doors;z++) {
		bx=Door[z].x;
		by=Door[z].y;
		beam_hit=death_table[MAP[(dx/th)+((dy/th)*mw)]];
		if( bx >= (dx>>3)-1 && bx <= (dx>>3)+1 && by >= (dy>>3)-1 && by <= (dy>>3)+1 && beam >= 1 && beam_hit==4) Door[z].active=1;
	      }
	    return(death_table[MAP[(dx/th)+((dy/th)*mw)]]);

	    }
	  break;
      }
      i++;
      if (i > 30) ok=1;
    }

    return(0);
}

void draw_bomb()
{
  static byte bomb_dat[6*3] = { 30, 1, 3, 2, 1, 0,
				30, 2, 3, 4, 1, 0,
				30, 5, 3, 5, 1, 0
			      };
  byte colis=0,ok=0;
  word z,offset;

    for (z=0;z<bomb_count;z++) {
      if (Bomb[z].active >= 1) {
	if (Bomb[z].active == 1) {
/*	  locate(1,1);
	  printf("Bomb[%d] Setup",z);
*/
	  Bomb[z].active=2;
	  Bomb[z].x =sx+104;
	  Bomb[z].y =sy+61;
	  Bomb[z].ix=0; /*(-1*(incx/8));*/
	  Bomb[z].iy=1; /*(incy/8);*/
	  Bomb[z].frame=0;
	}
	if (Bomb[z].active == 2) {
	  if(Bomb[z].x > sx+2 && Bomb[z].x < (sx+208) && Bomb[z].y > sy && Bomb[z].y < (sy+112+12)) {
	    offset=Bomb[z].frame*6;
/*	    locate(1,1); printf("x:%d y:%d\r",Bomb[z].x,Bomb[z].y);*/
	    colis=draw_object(CRAFT+(bomb_dat[offset]+(bomb_dat[offset+1]*cw)),&Bomb_Buff[z],Bomb[z].x-bomb_dat[offset+4],Bomb[z].y-bomb_dat[offset+5],bomb_dat[offset+2],bomb_dat[offset+3],cw);
	    if (!colis) {
	      Bomb[z].x+=Bomb[z].ix;
	      Bomb[z].y+=Bomb[z].iy;
	      if (Bomb[z].ix != 0) Bomb[z].ix-=(1*sign(Bomb[z].ix));
	      if (Bomb[z].iy < 4) Bomb[z].iy++;
	      if (Bomb[z].frame < 2) Bomb[z].frame++;
	    }
	    else { /* when bomb hits something on screen */
/*	      locate(1,1);
	      printf("Bomb[%d] x:%0.4d y:%0.4d hit something",z,Bomb[z].x,Bomb[z].y);
*/
	      Bomb[z].frame=0;
/*	      Bomb[z].active=3;*/
	      Bomb[z].x=globx;
	      Bomb[z].y=globy;
/*	      locate (1,1);printf("%0.1d",colis);*/
	      ok=check_persons(globx,globy);
	      if (!ok) ok=check_enemy(globx,globy);
	      colis=death_table[MAP[(globx/th)+((globy/th)*mw)]];
	      if (colis==0) {
		if (ok==1) Bomb[z].active=3;
	      }
	      else Bomb[z].active=3;
/*	      if ( colis >= 1 && colis < 3 || colis >=4 )*/
	    }
	  }
	  else { /* when bomb is not in play screen but is still active/falling */
/*	    locate(1,1);
	    printf("Bomb[%d] is off screen",z);
*/
	    Bomb[z].active=4;
	  }
	}
	else { /* when bomb has hit something and is exploding */
	  if (Bomb[z].active != 4) add_explosion(Bomb[z].x,Bomb[z].y);
	    Bomb[z].active=0;

	}
      }
    }
}

void undraw_bomb()
{
    word i;
    int z;
    for(z=bomb_count-1;z>=0;z--) {
      if((Bomb[z].active&0x03)) {
	if(Bomb[z].x > sx+2 && Bomb[z].x-2 < (sx+208) && Bomb[z].y > sy && Bomb[z].y < (sy+112+16)) {
	  undraw_object(&Bomb_Buff[z]);
	}
      }
    }
}

word check_all_persons()
{
  int z,cnt=0;
    for(z=0;z<Level.num_persons;z++) if ( Pers[z].active == 1 && Pers[z].type == 0 ) cnt+=1;
    return(cnt);
}

word check_persons(word x,word y)
{
  int z;
    for (z=0;z<Level.num_persons;z++) {
      if (Pers[z].active==1)
	if (x > (Pers[z].x-3) && x < (Pers[z].x+3) && y > (Pers[z].y-8) && y < (Pers[z].y+1)) { Pers[z].active=2; score+=Pers[z].death; return(1);}
    }
/*    men_left=check_all_persons();*/
    return(0);
}

void draw_persons()
{
    static byte pers_dat[6*17] = {
				   0, 8, 3, 8, 1, 7,
				   3, 8, 3, 8, 1, 7,
				   6, 8, 3, 8, 1, 7,
				   9, 8, 5, 8, 2, 7,
				  14, 8, 3, 8, 1, 7,
				  17, 8, 3, 8, 1, 7,
				  20, 8, 3, 8, 1, 7,
				  23, 8, 5, 8, 2, 7,

				  28, 0, 5, 8, 2, 7,

				   0, 0, 5, 8, 2, 7,
				   5, 0, 3, 8, 1, 7,
				   8, 0, 3, 8, 1, 7,
				  11, 0, 3, 8, 1, 7,
				  14, 0, 5, 8, 2, 7,
				  19, 0, 3, 8, 1, 7,
				  22, 0, 3, 8, 1, 7,
				  25, 0, 3, 8, 1, 7
				 };
    byte under=0;
    word z,offset,rn;

    for (z=0;z<Level.num_persons;z++) {
      if (Pers[z].active == 1) {

	    if (landed && Pers[z].type == 0) {
/*	    locate(1,4); printf("%d %d %d %d",sx+104,sy+62,Pers[z].x,Pers[z].y);
*/
	      if(((Pers[z].dir=(-sign(Pers[z].x-(sx+104)))) == 0) && (Pers[z].y-(sy+62)) == 0 ) {
		Pers[z].active=2;
		collected+=1;
		score+=Pers[z].pickup;
	      }
	    }
	    else {
	      if (Pers[z].dc == 0) {
		rn=(rand()>>10);
		if ( rn > 20 && rn <= 30)  Pers[z].dir=1;
		if ( rn > 10 && rn <= 20 ) Pers[z].dir=0;
		if ( rn > 00 && rn <= 10 ) Pers[z].dir=-1;
		if ( Pers[z].dir == Pers[z].ld ) {
		  Pers[z].dir=0; /*-(Pers[z].dir);*/
		}
		Pers[z].ld=Pers[z].dir;
		Pers[z].dc=(char)(rand()>>11);
/*		Pers[z].frame=0;  */

	      }
	      else Pers[z].dc--;
	    }

	    Pers[z].frame=((Pers[z].frame)&0x0f);
	    if (Pers[z].dir==0) offset=8*6;
	    else {
	      offset=((Pers[z].dir+8)*6)+((Pers[z].dir*(Pers[z].frame>>1))*6);
	    }


	    under=death_table[MAP[((Pers[z].x+Pers[z].dir)/tw)+(((Pers[z].y+1)/th)*mw)]];

	    if(under != 2 )
	    {
	      Pers[z].ld=Pers[z].dir;
	      Pers[z].dir=-(Pers[z].dir);
	      Pers[z].dc=0;
/*	      Pers[z].y+=1;*/
/*	      locate(1,1); printf("%d    %d %d",colis,Pers[z].x,Pers[z].y);*/

	    }

	    Pers[z].frame++;
	    Pers[z].x+=Pers[z].dir;
/*	    locate (0,3);
	    printf("frame :%d o:%0.4d  r:%0.8d  \r",Pers[z].frame,Pers[z].dir,sign(Pers[z].dir-sx+104));
*/

	if(Pers[z].x > sx-2 && Pers[z].x < (sx+208) && Pers[z].y >= sy && Pers[z].y < (sy+112+8)) {
/*	    SCREENPTR[(Pers[z].x-sx+56)  +((Pers[z].y-sy+48+1)*320)]=0x0F;*/
	    draw_object(CRAFT+(pers_dat[offset]+((pers_dat[offset+1]+244)*cw)),&Pers_Buff[z],Pers[z].x-pers_dat[offset+4],Pers[z].y-pers_dat[offset+5],pers_dat[offset+2],pers_dat[offset+3],cw);

/*	      locate(1,1); printf("%d    %d %d",colis,Pers[z].x,Pers[z].y);*/


	}

      }
      else {
	Pers[z].active=0;
      }
    }
}

void undraw_persons()
{
    word i;
    int z;
    for(z=Level.num_persons-1;z>=0;z--) {
      if(Pers[z].active >= 1) {
	if(Pers[z].x > sx-2 && Pers[z].x < (sx+208) && Pers[z].y > sy && Pers[z].y < (sy+112+8)) {
	  undraw_object(&Pers_Buff[z]);
	}
      }
    }

}

void draw_laser()
{
  static byte laser_dat[6*8] = {
				12,24,11,4, 0, 3,
				12,20,12,4, 0, 3,
				12,19,12,1, 0, 0,
				12,16,12,3, 0, 0,

				 0,24,11,4,11, 3,
				 0,20,12,4,11, 3,
				 0,19,12,1,11, 0,
				 0,16,12,3,11, 0

  			      };
  static char laser_mode[12*2] = {  10,-6,
				    10,-6,
				    10,-3,
				    10,-1,
				     0, 0,
				     0, 0,
				     0, 0,
				     0, 0,
				   -10,-6,
				   -10,-6,
				   -10,-3,
				   -10,-1
				};

  byte colis=0,ok=0;
  word z,offset,i;
  word hx,hy;

    for (z=0;z<laser_count;z++) {
      if (Laser[z].active >= 1) {
	hx=(int)laser_mode[((mode*8)+(flypos*2))+0];
	hy=(int)laser_mode[((mode*8)+(flypos*2))+1];

	if (Laser[z].active == 1) {
	  offset=flypos*6;
	  Laser[z].active=2;
	  Laser[z].x =sx+104-hx;
	  Laser[z].y =sy+56-hy;
	  Laser[z].ix=(mode-1)*laser_dat[offset+2];
	  Laser[z].iy=(-1*(flypos-2))*laser_dat[offset+3];
	  Laser[z].frame=flypos+((mode>>1)*4);
      }

	if (Laser[z].active == 2) {
	  if((int)Laser[z].x > (int)(sx-20) && (int)Laser[z].x < (int)(sx+208+20) && (int)Laser[z].y > (int)(sy-24) && (int)Laser[z].y < (int)(sy+112+24)) {
	    offset=Laser[z].frame*6;
	    colis=draw_object(CRAFT+(laser_dat[offset]+((laser_dat[offset+1]+244)*cw)),&Laser_Buff[z],Laser[z].x-laser_dat[offset+4],Laser[z].y-laser_dat[offset+5],laser_dat[offset+2],laser_dat[offset+3],cw);
	    if (!colis) {
	      Laser[z].x+=Laser[z].ix;
	      Laser[z].y+=Laser[z].iy;
	    }
	    else { /* when laser hits something on screen */
	      colis=0;
	      Laser[z].frame=0;
	      Laser[z].active=3;
	      Laser[z].x=globx;
	      Laser[z].y=globy;
	      ok=check_persons(globx,globy);
	      if (!ok) ok=check_enemy(globx,globy);
	      colis=death_table[MAP[(globx/th)+((globy/th)*mw)]];
/*	      locate (1,1);printf("%0.1d",colis);*/
	      if (colis==0) {
		if (ok==1) Laser[z].active=3;
	      }
	      else Laser[z].active=3;

/*	      if ( colis >= 1 && colis < 3 || colis >=4 )*/
	    }
	  }
	  else { /* when laser is not in play screen but is still active */
	    Laser[z].active=4;
	  }
	}
	else { /* when laser has hit something and is exploding */
	    if (Laser[z].active != 4) add_explosion(Laser[z].x,Laser[z].y);
	    Laser[z].active=0;
	}
      }

    }

}

void undraw_laser()
{
  int z;
    for(z=laser_count-1;z>=0;z--) {
      if((Laser[z].active&0x03)) {
/*	if((int)Laser[z].x > (int)(sx-20) && (int)Laser[z].x < (int)(sx+208+20) && (int)Laser[z].y > (int)(sy-24) && (int)Laser[z].y < (int)(sy+112+24))*/ {
	  undraw_object(&Laser_Buff[z]);
	}
      }
    }
}

void add_explosion(word x,word y)
{
  int z;
    for(z=0;z<expl_count;z++) {
      if (Expl[z].active == 0) {
	Expl[z].active=1;
	Expl[z].x=x;
	Expl[z].y=y;
	Expl[z].frame=0;
/*	Expl[z].type=(rand()>>14);*/
	break;
      }
    }
}

void draw_explosion()
{
  static byte expl_dat[6*9] = {
				 0,28, 8, 5, 4, 2,
				 0,33,11, 9, 5, 4,
				 0,42,11, 9, 5, 4,
				 0,51, 8, 6, 4, 3,

				 9,28, 7, 5, 3, 2,
				12,33, 9, 9, 4, 4,
				12,42, 9, 9, 4, 4,
				 9,51, 8, 6, 4, 3,
				 9,51, 8, 6, 4, 3

  			      };

  word z,offset,i;

    for (z=0;z<expl_count;z++) {
      if (Expl[z].active == 1) {
	  if((int)Expl[z].x > (int)(sx-16) && (int)Expl[z].x < (int)(sx+208+16) && (int)Expl[z].y > (int)(sy-16) && (int)Expl[z].y < (int)(sy+112+16)) {
	    offset=(Expl[z].frame*6)+(Expl[z].type*24);
/*	    offset=0;*/
/*	    locate (0,2); printf("%0.4d %0.4d %0.2d",Expl[z].x,Expl[z].y,Expl[z].type);*/
	    draw_object_2(CRAFT+(expl_dat[offset]+((expl_dat[offset+1]+244)*cw)),&Expl_Buff[z],Expl[z].x-expl_dat[offset+4],Expl[z].y-expl_dat[offset+5],expl_dat[offset+2],expl_dat[offset+3],cw);
	    if (Expl[z].frame < 4) Expl[z].frame++; else Expl[z].active=2;
	  }
	  else {
	    Expl[z].active=2;
	  }
      }
      else {
	Expl[z].active=0;
      }

    }
}

void undraw_explosion()
{
    int z;
    for(z=expl_count-1;z>=0;z--) {
      if(Expl[z].active) {
	if((int)Expl[z].x > (int)(sx-16) && (int)Expl[z].x < (int)(sx+208+16) && (int)Expl[z].y > (int)(sy-16) && (int)Expl[z].y < (int)(sy+112+16)) {
	  undraw_object(&Expl_Buff[z]);
	}
      }
    }
}


