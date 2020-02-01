/*  vgaglob.c  global variable declarations  for vga.c */

#include "vga.h"
#include <dos.h>
#include <alloc.h>
#include <stdio.h>

char fname[40];
byte PROGRAMMER[46] = {"Matthew Jacob Edwards"};
byte TITLE[76] = {"RESCUE 223 V1.1 (C) 1993 M.J.EDWARDS"};
byte REGISTERED[90] = {"NOT REGISTERED"};
byte pr=0xfe;
int oldmode;

long score,last_score;
word mw,mh;
byte ww,wh,tw,th,cw,ch,ca,nc;
word ta;
word level;
word key;
word gravity;
word gamespeed;

word col_d=0,cs=0,fadeoff=0;

byte cheat;
word sx,sy,ssx,ssy;
word restart_x,restart_y;
word num_crafts,lnc;
byte restart_mode;
byte crash,landing,landed,colis,flypos;
byte mode,shift,beam,fire;
byte lastmode,shifton,beam_hit,enter;
byte pause;
byte men_left,lml;
byte bomb_count;
byte laser_count;
byte expl_count;
byte bullet_count;
byte hit_count,lhc;
byte complete,collected;

int globx,globy,globw,globh;
int incx,incy;

union REGS regs;

byte RGB[192];
byte fadecols[192];

byte far *SCREENPTR = (char far *) SCREENADDR;
byte far *SHIFT = (char far *) SHIFTADDR;

/*
**  Pointers for Allocate Blocks of Memory (32k each) (I Hope!)
*/

byte far *MEMORY_AREA1;
byte far *MEMORY_AREA2;
byte far *MEMORY_AREA3;

/*
**  Pointers Withing One of the Above Memory Blocks
*/

byte far *SCREENBUF;
byte far *TILES;
byte far *MAP;
byte far *CRAFT;
byte far *FONT;

struct LEVEL Level;

struct DOOR  Door[MAX_DOOR];
struct PERS  Pers[MAX_PERS];
struct BOMB  Bomb[MAX_BOMB];
struct BOMB  Laser[MAX_LASER];
struct EXPL  Expl[MAX_EXPL];
struct ENEMY Enemy[MAX_ENEMY];
struct BOMB  Bullet[MAX_BULLET];


struct BUFF Craft_Buff;
struct BUFF Bomb_Buff[MAX_BOMB];
struct BUFF Laser_Buff[MAX_LASER];
struct BUFF Pers_Buff[MAX_PERS];
struct BUFF Expl_Buff[MAX_EXPL];
struct BUFF Enemy_Buff[MAX_ENEMY];
struct BUFF Bullet_Buff[MAX_BULLET];

byte craft_buff[35*16*MAX_CRAFT];
byte bomb_buff[3*5*MAX_BOMB];
byte laser_buff[12*4*MAX_LASER];
byte pers_buff[5*8*MAX_PERS];
byte expl_buff[9*11*MAX_EXPL];
byte enemy_buff[10*10*MAX_ENEMY];
byte bullet_buff[1*1*MAX_BULLET];

/*
**
**  Death_Table : Each tile in the map has a type, like the ground = 1
**                which my meaning means "die when touched"
**  0 : Pass through
**  1 : Die when touched
**  2 : Touch tile, when touched a certain function activates eg. landing
**  3 : Not Defined, if i hit this tile then do nothing
**  4 : Gate trigger/switch for opening
**  5 : Gate so i can pass through on map without altering map in mem
**
*/

byte death_table[200] = { 0,1,2,2,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,3,3,3,0,0,0,0,3,4,4,4,4,5,5,5,5,5,5,
			  3,3,3,3,1,1,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,0,0,3,3,4,4,4,4,1,5,1,3,5,3,
			  1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
			  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1
			};

