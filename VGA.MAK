vga.exe:    vgacraft.c vgaglob.c vgaload.c vgamain.c vgamodes.c vgascrn.c vgagfxhd.obj 
 tcc -ml -C vgacraft.c vgaglob.c vgaload.c vgamain.c vgamodes.c vgascrn.c vgagfxhd.obj

vgagfxhd.obj: vgagfxhd.asm
 tcc -ml -c vgagfxhd.asm
