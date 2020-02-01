#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <mem.h>
#include <math.h>
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vga.h"


extern union REGS regs;

extern char fname[30];
extern byte PROGRAMMER[34];
extern byte TITLE[76];
extern byte REGISTERED[90];

extern byte fadecols[192];
extern byte RGB[192];

extern char far *SCREENPTR;
extern char far *SHIFT;

extern byte far *MEMORY_AREA1;
extern byte far *MEMORY_AREA2;
extern byte far *MEMORY_AREA3;

extern char far *SCREENBUF;
extern char far *TILES;
extern char far *MAP;
extern char far *CRAFT;
extern char far *FONT;

extern byte pr;

extern int oldmode;

extern word key;
extern word fadeoff,cs,col_d;
extern long score,last_score;
extern word mw,mh;
extern byte ww,wh,tw,th,cw,ch,ca,nc;
extern word ta;
extern word level;
extern word gravity;
extern word gamespeed;

extern byte cheat;
extern word sx,sy,ssx,ssy;
extern word restart_x,restart_y;
extern word num_crafts,lnc;
extern byte restart_mode;
extern byte crash,landing,landed,colis,flypos;
extern byte mode,shift,beam,fire;
extern byte lastmode,shifton,beam_hit,enter;
extern byte pause;
extern byte men_left,lml;
extern byte bomb_count;
extern byte laser_count;
extern byte expl_count;
extern byte bullet_count;
extern byte complete,collected;
extern byte hit_count,lhc;



extern int globx,globy,globw,globh;
extern int incx,incy;

extern struct DOOR  Door[MAX_DOOR];
extern struct PERS  Pers[MAX_PERS];
extern struct BOMB  Bomb[MAX_BOMB];
extern struct BOMB  Laser[MAX_LASER];
extern struct EXPL  Expl[MAX_EXPL];
extern struct ENEMY Enemy[MAX_ENEMY];
extern struct BOMB  Bullet[MAX_BULLET];

extern struct LEVEL Level;

extern struct BUFF Craft_Buff;
extern struct BUFF Bomb_Buff[MAX_BOMB];
extern struct BUFF Laser_Buff[MAX_LASER];
extern struct BUFF Pers_Buff[MAX_PERS];
extern struct BUFF Expl_Buff[MAX_EXPL];
extern struct BUFF Enemy_Buff[MAX_ENEMY];
extern struct BUFF Bullet_Buff[MAX_BULLET];

extern byte craft_buff[(35*16)*MAX_CRAFT];
extern byte bomb_buff[(3*5)*MAX_BOMB];
extern byte laser_buff[(12*4)*MAX_LASER];
extern byte pers_buff[(5*8)*MAX_PERS];
extern byte expl_buff[(9*11)*MAX_EXPL];
extern byte enemy_buff[(8*8)*MAX_ENEMY];
extern byte bullet_buff[(1*1)*MAX_BULLET];

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

extern byte death_table[200];


void setup();
void cleanup(char *,int);

int    abs(int);
void   locate(int,int);
void   writetile(int,int,byte);
void   writetile2(int,int,byte);
void   writetile3(int,int,byte);

int    introscreen();
int    optionscreen();
int    instructscreen();

void   initialize();
void   setup_display();
void   draw_display();
void   init_level();
void   check_under_craft(int,int,int,int,int,int);

void   drawmap(int,int);
void   drawmap2(int,int);
void   updatemap(int,int);

byte   draw_object(char far *,struct BUFF *,word,word,word,word,word);
byte   draw_object_2(char far *,struct BUFF *,word,word,word,word,word);

void   undraw_object(struct BUFF *);
void   undraw_object_2(struct BUFF *);

void   draw_craft(word,word,int,int,word);
void   undraw_craft();

void   draw_bomb();
void   undraw_bomb();

void   draw_explosion();
void   undraw_explosion();
void   add_explosion(word,word);

void   draw_bullet();
void   undraw_bullet();
void   add_bullet(word,word,word,word);

void   draw_objects(void);
void   undraw_objects(void);

void   draw_persons();
void   undraw_persons();
word   check_all_persons();
word   check_persons(word,word);

void   draw_enemy();
void   undraw_enemy();
word   check_enemy(word,word);

void   draw_laser();
void   undraw_laser();

byte   draw_beam(word,word,byte);


void   open_door_2();

void   draw_screen_2(word,word);

void   Draw_Screen(char far *,char far *,int,int,int,int,int,int);
void   Draw_Screen_2(char far *,char far *,int,int,int,int,int,int);
void   Fill_Rectangle(word,word,word,word,word,char far *);

word   loadlevel(int);
word   load_level_data(int);
void   loadmap(int);
void   loadtiles(void);
void   loadcraft(void);
void   setupscreen(void);

int    getmode(void);
void   setmode(int);
void   setcolour(byte, byte, byte, byte);
void   setcolours(byte *);
void   fade();
void   colourbar(void);
void   loadgfx(void);

void   matts_keys(void);
void   slowkey(void);
void   fastkey(void);

void   Install_Game_Key();
void   Uninstall_Game_Key();
word   Game_Key();
void   Draw_Screen(char far *,char far *,int,int,int,int,int,int);
void   Draw_Screen_2(char far *,char far *,int,int,int,int,int,int);
void   Fill_Rectangle(word,word,word,word,word,char far *);
void   Setup_ASM_Vars(char far *,char far *,char far *,char far *,char far *,char far *);
void   printch(word,word,byte,byte,byte);
char far * Get_LineOffsets();
char far * Get_CharOffsets();

void   movescreen(void);
void   Fill_Rect(word,word,word,word,word);
void   raise_box(word,word,word,word,word);
void   lower_box(word,word,word,word,word);

void   print(word,word,char *,byte,byte);
void   writetile_ch(word,word,byte,byte,byte);
void   final_screen();
byte   *decode(byte *);