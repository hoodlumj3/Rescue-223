
#include "VGAExtrn.h"

void setmode(int mode)
{
   regs.h.al = mode;
   regs.h.ah = 0;
   int86(0x10,&regs,&regs);
}

int getmode(void)
{
   regs.h.ah = 0x0f;
   int86 (0x10,&regs,&regs);
   return(regs.h.al);
}
