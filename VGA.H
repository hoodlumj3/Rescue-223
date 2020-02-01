/* vga.h */
/*#include "keycodes.h"*/

#define SCREENADDR	0xa0000000L
#define SHIFTADDR	0x00000417L

#define BYTESPERLINE	320

#define SETREG(i,val)		{outportb(0x3ce,i);outportb(0x3cf,val);}
#define SETCOLOUR(c,r,g,b)	{outportb(0x3c8,c);outportb(0x3c9,r);outportb(0x3c9,g);outportb(0x3c9,b);}


#define MY_SCREEN_WIDTH   (30*8)
#define MY_SCREEN_HEIGHT  (17*8)


#define SCREEN_WIDTH      320
#define SCREEN_HEIGHT     200

#define XMIN		  0
#define YMIN		  0
#define XMAX		  319
#define YMAX		  199
#define XMID		  160
#define YMID		  100

#define KEY_UPARROW	    0x0001
#define KEY_DOWNARROW	0x0002
#define KEY_SPACE		0x0010
#define KEY_ENTER		0x0020
#define KEY_ESCAPE		0x8000

#define UPARROW			(Game_Key() & 0x0001) /* 0000 0000 0000 0001 = Up     */
#define DOWNARROW		(Game_Key() & 0x0002) /* 0000 0000 0000 0010 = Down   */
#define LEFTARROW		(Game_Key() & 0x0004) /* 0000 0000 0000 0100 = Left   */
#define RIGHTARROW		(Game_Key() & 0x0008) /* 0000 0000 0000 1000 = Right  */
#define SPACEBAR		(Game_Key() & 0x0010) /* 0000 0000 0001 0000 = Space  */
#define ENTER			(Game_Key() & 0x0020) /* 0000 0000 0010 0000 = Enter  */
#define TAB				(Game_Key() & 0x0040) /* 0000 0000 0100 0000 = Tab    */
#define KEY_UNDEF7		(Game_Key() & 0x0080) /* 0000 0000 1000 0000 = ?????  */
#define KEY_UNDEF8		(Game_Key() & 0x0100) /* 0000 0001 0000 0000 = ?????  */
#define KEY_UNDEF9		(Game_Key() & 0x0200) /* 0000 0010 0000 0000 = ?????  */
#define KEY_UNDEFA		(Game_Key() & 0x0400) /* 0000 0100 0000 0000 = ?????  */
#define ANYKEY			(Game_Key() & 0x0800) /* 0000 1000 0000 0000 = ANYKEY */
#define KEY_P			(Game_Key() & 0x1000) /* 0001 0000 0000 0000 = P      */
#define F1				(Game_Key() & 0x2000) /* 0010 0000 0000 0000 = F1     */
#define F2				(Game_Key() & 0x4000) /* 0100 0000 0000 0000 = F2     */
#define ESCAPE			(Game_Key() & 0x8000) /* 1000 0000 0000 0000 = Escape */

#define     MAX_CRAFT	1
#define     MAX_DOOR    20
#define     MAX_PERS    20
#define     MAX_BOMB    20
#define     MAX_LASER   10
#define     MAX_EXPL    25
#define     MAX_ENEMY   25
#define     MAX_BULLET	25

typedef unsigned int word;
typedef unsigned char byte;

struct LEVEL {
		word start_x;
		word start_y;
		byte start_mode;
		word finish_x;
		word finish_y;
		byte num_doors;
		byte num_persons;
		byte num_enemys;
	     };
struct DOOR  {
		byte x;
		byte y;
		byte active;
		byte pos;
		byte timer;
		byte tcount;
		byte speedo;
		byte speedc;
		byte type;
		byte key;
	     };

struct PERS  {
		byte active;
		word x;
		word y;
		char dir;
		byte type;
		byte dc;
		byte ld;
		byte frame;
		word pickup;     /* score for picking up man */
		word death;      /* score for killing man    */
	     };


struct ENEMY {
		byte active;
		word x;
		word y;
		char dir;
		byte type;       /* type of enemy 1 globe, 2 tank */
		byte dc;         /* direction count               */
		byte hit_c;      /* number of times to be hit     */
		byte hit_t;      /* number of times hit           */
		byte frame;
		word death;      /* score for killing enemy       */

	     };
struct BUFF  {
		word x;
		word y;
		word w;
		word h;
		byte far *buff;
	     };

struct EXPL  {
		byte active;
		word x;
		word y;
		byte frame;
		byte type;
	     };
struct BOMB  {
		byte active;
		word x;
		word y;
		char ix;
		char iy;
		byte frame;
	     };

