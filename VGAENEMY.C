#include "VGAExtrn.h"

word check_enemy(word x,word y)
{
  int z;
    for (z=0;z<Level.num_enemys;z++) {
      if (Enemy[z].active==1)
	if (x > (Enemy[z].x-4) && x < (Enemy[z].x+4) && y > (Enemy[z].y-4) && y < (Enemy[z].y+4)) {
	   Enemy[z].hit_t+=1;
	   if (Enemy[z].hit_t >= Enemy[z].hit_c) {
	     Enemy[z].active=2; score+=Enemy[z].death; return(1);
	   }
	   return(1);
	}
    }
    return(0);
}

void draw_enemy()
{
    static char enemy_dat[6*8] = {
				   26,16,7,7,-2,-2,
				   26,16,7,7, 0,-2,
				   26,16,7,7, 2,-2,
				   26,16,7,7, 2, 0,
				   26,16,7,7, 2, 2,
				   26,16,7,7, 0, 2,
				   26,16,7,7,-2, 2,
				   26,16,7,7,-2, 0
				 };
    byte colis=0;/*,under=0;*/
    word z,offset,rn;

    for (z=0;z<Level.num_enemys;z++) {
      if (Enemy[z].active == 1) {
	if((int)Enemy[z].x > (int)(sx-8) && (int)Enemy[z].x < (int)(sx+208+8) && (int)Enemy[z].y >= (int)(sy-8) && (int)Enemy[z].y < (int)(sy+112+8)) {
	  switch(Enemy[z].type) {
	    case 0 :

/*	      if (Enemy[z].dir==0) offset=8*6;
	      else {
	        offset=((Enemy[z].dir+8)*6)+((Enemy[z].dir*Enemy[z].frame)*6);
	      }
*/
	      offset=Enemy[z].frame*6;

	      if (Enemy[z].dc == 0) {

		Enemy[z].dc=(char)(rand()>>12);
		add_bullet(Enemy[z].x-(enemy_dat[offset+4]*2),Enemy[z].y-(enemy_dat[offset+5]*2),Enemy[z].x-(enemy_dat[offset+4]*4),Enemy[z].y-(enemy_dat[offset+5]*4));
	      }
	      else {
		Enemy[z].dc--;
	      }


/*	      under=death_table[MAP[((Enemy[z].x+Enemy[z].dir)/tw)+(((Enemy[z].y+1)/th)*mw)]];*/
/*	      if(under != 2 ) {
	        Enemy[z].dir=-(Enemy[z].dir);
/*		Enemy[z].dc=0;*/

	      }
*/

	      Enemy[z].frame++;
	      Enemy[z].frame=((Enemy[z].frame)&0x07);
/*	      if(Enemy[z].x > sx-3 && Enemy[z].x < (sx+208+3) && Enemy[z].y >= sy && Enemy[z].y < (sy+112+8)) {*/
/*	        SCREENPTR[(Pers[z].x-sx+56)  +((Pers[z].y-sy+48+1)*320)]=0x0F;*/
		colis+=draw_object(CRAFT+(enemy_dat[offset]+((enemy_dat[offset+1]+244)*cw)),&Enemy_Buff[z],Enemy[z].x-4,Enemy[z].y-4,enemy_dat[offset+2],enemy_dat[offset+3],cw);
/*	      }*/
	      break;
	    case 1 :
	      break;
	  }
	}
      }
      else {
	Enemy[z].active=0;
      }
    }
}

void undraw_enemy()
{
    word i;
    int z;
    for(z=Level.num_enemys-1;z>=0;z--) {
      if(Enemy[z].active) {
	if((int)Enemy[z].x > (int)(sx-8) && (int)Enemy[z].x < (int)(sx+208+8) && (int)Enemy[z].y > (int)(sy-8) && (int)Enemy[z].y < (int)(sy+112+8)) {
	  undraw_object(&Enemy_Buff[z]);
	}
      }
    }
}


word check_bullet(word x,word y)
{
  int z;
    for (z=0;z<bullet_count;z++) {
      if (Bullet[z].active==1)
	if (x == Bullet[z].x && y == Bullet[z].y) {
	   Bullet[z].active=2;
	   return(1);
	}
    }
    return(0);
}

void add_bullet(word x1,word y1,word x2,word y2)
{
  int z;
  int intx,inty;
  int xstep,ystep;

    for(z=0;z<bullet_count;z++) {
      if (Bullet[z].active == 0) {
	Bullet[z].active=1;
	Bullet[z].x=(x1*8);
	Bullet[z].y=(y1*8);
	Bullet[z].ix=(x2-x1)*4;
	Bullet[z].iy=(y2-y1)*4;
/*	if (abs(Bullet[z].ix) < 8 ) Bullet[z].ix=sign(Bullet[z].ix)*8;
	if (abs(Bullet[z].iy) < 8 ) Bullet[z].iy=sign(Bullet[z].iy)*8;
*/
	Bullet[z].frame=0;
	break;
      }
    }
}

void draw_bullet()
{
/*  static byte bullet_dat[6*3] = { 30, 1, 3, 2, 1, 0,
				30, 2, 3, 4, 1, 0,
				30, 5, 3, 5, 1, 0
			      };
*/
  byte colis=0,ok=0;
  word z;

    for (z=0;z<bullet_count;z++) {
      if (Bullet[z].active >=1 ) {
	if (Bullet[z].active == 1) {
	  if((int)(Bullet[z].x>>3) > (int)(sx-4) && (int)(Bullet[z].x>>3) < (int)(sx+208+4) && (int)(Bullet[z].y>>3) > (int)(sy-4) && (int)(Bullet[z].y>>3) < (int)(sy+112+4)) {
	    colis=draw_object(CRAFT+33,&Bullet_Buff[z],Bullet[z].x>>3,Bullet[z].y>>3,1,1,cw);
	    if (!colis) {
	      Bullet[z].x+=(Bullet[z].ix/2);
	      Bullet[z].y+=(Bullet[z].iy/2);
	      Bullet[z].frame++;
	      if (Bullet[z].frame >= 80) Bullet[z].active=2;
	    }
	    else { /* when bullet hits something on screen */
	      Bullet[z].frame=0;
	      Bullet[z].x=globx<<3;
	      Bullet[z].y=globy<<3;
	      ok=check_bullet(globx,globy);
	      if (!ok) ok=check_craft(globx,globy);
/*	      ok=check_persons(globx,globy);*/
/*	      if (!ok) ok=check_enemy(globx,globy);
*/
	      colis=death_table[MAP[((globx)/th)+(((globy)/th)*mw)]];
	      if (colis==0) {
		if (ok==1) Bullet[z].active=2;
	      }
	      else Bullet[z].active=2;
/*	      if ( colis >= 1 && colis < 3 || colis >=4 )*/
	    }
	  }
	  else { /* when bomb is not in play screen but is still active/falling */
	    Bullet[z].active=3;
	  }
	}
	else { /* when bomb has hit something and is exploding */
	  if (Bullet[z].active != 3) add_explosion((Bullet[z].x>>3),(Bullet[z].y>>3));
	    Bullet[z].active=0;

	}
      }
    }
}

void undraw_bullet()
{
    int z;
    for(z=bullet_count-1;z>=0;z--) {
      if(Bullet[z].active ) {
	if((int)(Bullet[z].x>>3) > (int)(sx-31) && (int)(Bullet[z].x>>3) < (int)(sx+208+31) && (int)(Bullet[z].y>>3) > (int)(sy-31) && (int)(Bullet[z].y>>3) < (int)(sy+112+31)) {
	  undraw_object(&Bullet_Buff[z]);
	}
      }
    }
}

