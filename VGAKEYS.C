#include <dos.h>
extern union REGS regs;

unsigned int getkey(void)
{
  regs.h.ah = 0;
  int86 (0x16, &regs, &regs);
  if (regs.h.al)
     return ((int) regs.h.al);
  return (regs.x.ax);
}

int keypressed()
{
	regs.h.ah = 11;             /* 11 = see if key is pressed */
	int86 ( 0x21, &regs, &regs );
	return ( regs.h.al );
	/* returns 0xff if key pressed, otherwise zero */
}

void fastkey(void)
{
    regs.h.ah = 3;
    regs.h.al = 5;
    regs.h.bh = 0;
    regs.h.bl = 0;
    int86 (0x16,&regs,&regs);
}