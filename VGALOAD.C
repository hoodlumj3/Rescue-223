/*  vgaload.c  */
#include "VGAExtrn.h"

void loadgfx()
{
    nc=0;
    level=1;
    setupscreen();
    loadtiles();
    loadcraft();
/*    loadlevel(level);  */

}

void loadtiles(void)
{
  FILE *fp;
  char num_cols=0;
  long int i;
  long tsize;


	TILES=MEMORY_AREA1;
	FONT=MEMORY_AREA1+7680;
/*    printf("Loading Tiles\n");*/
    if ((fp=fopen("tiledat.gfx","rb")) == NULL) cleanup("Couldn't Open Tile File\n",14);
    tw=fgetc(fp);   /* get width and height of blocks */
    th=fgetc(fp);
    ta=fgetc(fp)+(fgetc(fp)*256);
    tsize=tw*th*ta;
/*    printf("W:%0.2d H:%0.2d N:%0.4d S:%0.8ld\n",tw,th,ta,tsize);*/
    if (tsize > 16384) cleanup("I Won't Load Tiles, Too Big\n",2);

    /*
    ** Load tile colours
    */

    num_cols=fgetc(fp);
    for(i=0;i<num_cols*3;i++) RGB[i+nc]=fgetc(fp);
    i=0;
    while ( i < tsize ) {
      TILES[i]=fgetc(fp);
      ++i;
    }
    nc+=num_cols;
    fclose(fp);
}

void loadcraft(void)
{
  FILE *fp;
  long int i;
  char num_cols;
  unsigned char byte;
  long csize;

    CRAFT=MEMORY_AREA1+16384;

/*    printf("Loading Objects\n");*/
    if ((fp=fopen("craftdat.gfx","rb")) == NULL) cleanup("Couldn't Open Craft File\n",14);
    cw=fgetc(fp);   /* get width and height of blocks */
    ch=fgetc(fp);
    ca=fgetc(fp)+(fgetc(fp)*256);
    csize=cw*ch*ca;

    if (csize > 16384) cleanup("I Won't Load Objects, Too Big\n",2);

	/* load tile colours */
    num_cols=fgetc(fp);
    for(i=0;i<num_cols*3;i++) RGB[i+(nc*3)]=fgetc(fp);
    i=0;
    while ( i < csize ) {
      byte=fgetc(fp);
      if (byte != 0) CRAFT[i]=(byte+nc); else CRAFT[i]=byte;
/*      printf("%0.2x\r",byte); */
      ++i;
    }
    nc+=num_cols;
    fclose(fp);
}

void setupscreen(void)
{
    long i;
/*    printf("Setting Up Screen\n");*/
    SCREENBUF=MEMORY_AREA3;
    for(i=0;i<32768;i++) SCREENBUF[i]=0xFF;
}

word loadlevel(int level)
{
  int lev=level;
    while(!load_level_data(lev) && lev != 255) lev++;
    loadmap(lev);
    return(lev);
/*    if(load_level_data(level)) {
      loadmap(level);
      return(1);
    }
    else
      return(0);
*/
}

word load_level_data(int level)
{
  FILE *fp;
  word i;
  int  dummy;

    sprintf(fname,"lev%0.3d.dat",level);
    if ((fp=fopen(fname,"rb")) == NULL) {
/*      printf("Error - Could Not Open %s\n",fname);*/
      return(0);
    }
    Level.num_doors=fgetc(fp);
    for(i=0;i<Level.num_doors;i++) {
      fread(&Door[i],sizeof(Door[i]),1,fp);
    }
    Level.num_persons=fgetc(fp);
    for(i=0;i<Level.num_persons;i++) {
      Pers[i].active=1;
      Pers[i].x=(fgetc(fp)<<3);
      Pers[i].y=(fgetc(fp)<<3);
      Pers[i].y+=7;
      Pers[i].dir=fgetc(fp);
      Pers[i].pickup=fgetc(fp);
      Pers[i].frame=0;
      Pers[i].death=10;
      Pers_Buff[i].buff=&pers_buff[((5*8)*i)];
    }
    Level.num_enemys=fgetc(fp);
    for(i=0;i<Level.num_enemys;i++) {
      Enemy[i].active=1;
      Enemy[i].x=(fgetc(fp)<<3)+4;
      Enemy[i].y=(fgetc(fp)<<3)+4;
      Enemy[i].dir=0;
      Enemy[i].type=0;
      Enemy[i].dc=0;
      Enemy[i].hit_c=fgetc(fp);
      Enemy[i].hit_t=0;
      Enemy[i].frame=0;
      Enemy[i].death=fgetc(fp);
      Enemy_Buff[i].buff=&enemy_buff[((8*8)*i)];
    }
    fread(&Level.start_x,sizeof(Level.start_x),1,fp);
    fread(&Level.start_y,sizeof(Level.start_y),1,fp);
    Level.start_x-=104;
    Level.start_y-=56;
    Level.start_mode=fgetc(fp);
    fread(&Level.finish_x,sizeof(Level.finish_x),1,fp);
    fread(&Level.finish_y,sizeof(Level.finish_y),1,fp);
    Level.finish_x-=104;
    Level.finish_y-=56;
    fclose(fp);
    return(1);
}

void loadmap(int level)
{
  FILE *fp;
  long i,j;
  char dummy=0;
  long msize;

    MAP=MEMORY_AREA2;
    sprintf(fname,"lev%0.3d.map",level);
    if ((fp=fopen(fname,"rb")) == NULL) { printf(fname); cleanup("Couldn't Open Map File",14);}
    mw=(fgetc(fp)*256)+fgetc(fp);
    mh=(fgetc(fp)*256)+fgetc(fp);
    msize=mw*mh;
    if ( msize > 32768 ) cleanup("I Won't Load Map, Too Big\n",2);
    dummy=dummy+1;
    dummy=fgetc(fp)+fgetc(fp);
    i=0;
    while ( i < msize ) {
      MAP[i]=fgetc(fp);
      ++i;
    }
    fclose(fp);
}
