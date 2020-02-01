#include <stdio.h>
#include <dos.h>

unsigned int num_doors,num_persons,num_enemys;
unsigned int start_x,start_y;
unsigned int finish_x,finish_y;
unsigned char start_mode;

/*void write_level(int,char *,char *);*/

void level001_init();
void level002_init();
void level003_init();
void level008_init();
void level255_init();

main()
{

    level001_init();
    level002_init();
    level003_init();
    level008_init();
    level255_init();

}

void write_level(int lev,unsigned char *door,unsigned char *person,unsigned char *enemys)
{
  FILE *fp;
  char fname[15];
  int i,j;


    sprintf(fname,"lev%0.3d.dat",lev);
    printf("%s",fname);
    if ((fp=fopen(fname,"wb")) == NULL) {
      printf(" .. Couldn't Write\n");
/*      exit(0);*/
    return;
    }

    fputc(num_doors,fp);
    for (i=0;i<num_doors*10;i++) {
	fputc(*door++,fp);
    }

    fputc(num_persons,fp);
    for (i=0;i<num_persons*4;i++) {
	fputc(*person++,fp);
    }
    fputc(num_enemys,fp);
    for (i=0;i<num_enemys*4;i++) {
	fputc(*enemys++,fp);
    }
    fwrite(&start_x,sizeof(start_x),1,fp);
    fwrite(&start_y,sizeof(start_y),1,fp);
    fputc(start_mode,fp);
    fwrite(&finish_x,sizeof(finish_x),1,fp);
    fwrite(&finish_y,sizeof(finish_y),1,fp);

    fclose(fp);
    printf(" .. OK\n");
}

void level001_init()
{

  static unsigned char doors[3][10] = {
    { 59,16,0,0,0,24, 2, 4,3,0 },
    { 84,16,0,0,0,24, 2, 2,3,0 },
    { 97,11,0,0,0,24, 8, 2,3,0 }
				     };
  static unsigned char persons[5][4] = {
    { 67,19, 0,100},
    {108,23, 1,200},
    { 71,20,-1,100},
    { 41,18, 0,200},
    { 43,18,-1,200}
				       };
  static unsigned char enemys[2][4] = {
    { 47, 11,10,50},
    { 67, 10, 1,20}
				      };
    num_doors=3;
    num_persons=5;
    num_enemys=2;
    start_x=26<<3;
    start_y=(20<<3)+1;
    finish_x=26<<3;
    finish_y=(20<<3)+1;
    start_mode=0;

    write_level(1,&doors,&persons,&enemys);
}

void level002_init()
{
  static unsigned char doors[6][10] = {
    { 28,20,0,0,0,24, 2, 2,3,1 },
    { 41,21,0,0,0,24, 2, 2,3,2 },
    { 56,20,0,0,0,24, 2, 2,3,0 },
    { 76,20,0,0,0,24, 4, 4,3,0 },
    { 96,20,0,0,0,24, 2, 2,3,0 },
    {105,14,0,0,0,24, 2, 2,3,0 }
				    };
  static unsigned char persons[2][4] = {
    { 66, 22, 0,200},
    { 35, 23, 1,200}
				       };
  static unsigned char enemys[2][4] = {
    {113,  8,10,20},
    { 47, 24,15,50}
				      };
    num_doors=6;
    num_persons=2;
    num_enemys=2;
    start_x=18<<3;
    start_y=(23<<3)+1;
    finish_x=109<<3;
    finish_y=(23<<3)+1;
    start_mode=0;
    write_level(2,&doors,&persons,&enemys);
}
void level003_init()
{
  static unsigned char doors[5][10] = {
    { 32,18,0,0,0,24,2,2,3,0 },
    { 52,18,0,0,0,24,2,2,3,0 },
    { 77,18,0,0,0,24,2,2,3,0 },
    { 87,16,0,0,0,24,2,2,3,0 },
    { 97,14,0,0,0,24,2,2,3,0 }
				    };
  static unsigned char persons[3][4] = {
    { 105,22, 0,200},
    {  60,20, 1,200},
    {  22,22,-1,200}

				       };
  static unsigned char enemys[4][4] =  {
    { 28, 12, 1, 50},
    { 14, 18,12,150},
    {112, 20, 5, 20},
    { 43, 12, 1, 10}
				       };

    num_doors=5;
    num_persons=3;
    num_enemys=4;
    start_x=64<<3;
    start_y=(14<<3)+1;
    finish_x=64<<3;
    finish_y=(14<<3)+1;
    start_mode=2;
    write_level(3,&doors,&persons,&enemys);
}
void level008_init()
{
  static unsigned char doors[7][10] =  {
    { 24,46,0,0,0,24,2,2,1,0 },
    { 54,49,0,0,0,24,2,2,2,0 },
    { 66,72,0,0,0,24,2,2,3,0 },
    { 95,31,0,0,0,24,2,2,3,0 },
    {108,50,0,0,0,24,2,2,2,0 },
    {100,79,0,0,0,24,2,2,3,0 },
    { 59,97,0,0,0,24,2,2,3,0 }
				       };
  static unsigned char persons[20][4] = {
    { 25, 13, 0,100},
    { 34, 12, 0, 80},
    { 78, 17,-1,100},
    { 70, 17, 1,150},
    {102, 19,-1,100},
    {112, 15, 0, 25},
    {116, 21, 0,100},
    {111, 29,-1,250},
    { 86, 53, 1,100},
    { 64, 62,-1,200},
    { 71, 62, 1,100},
    { 75, 62, 0,100},
    { 78, 62, 0,220},
    { 27, 64,-1,100},
    {111, 83,-1,100},
    {102,118,-1, 50},
    {105, 83,-1, 25},
    {100, 83,-1, 75},
    { 88,118,-1,150},
    { 37, 52,-1,100}
				       };

  static unsigned char enemys[10][4] =  {
    { 49,  7,10,0},
    { 51, 25,10,0},
    { 78, 56,10,0},
    { 65, 53,10,0},
    { 29, 90,10,0},
    { 72, 92,10,0},
    { 72,113,10,0},
    { 27, 55,10,0},
    {101, 13,10,0},
    {113, 26,10,0}
				       };

    num_doors=7;
    num_persons=20;
    num_enemys=10;
    start_x=42<<3;
    start_y=(52<<3)+1;
    finish_x=112<<3;
    finish_y=(118<<3)+1;
    start_mode=1;
    write_level(8,&doors,&persons,&enemys);
}
void level255_init()
{
  static unsigned char doors[1][10] = {
    { 87,95,0,0,0,24,2,2,3,0 }
				     };
  static unsigned char persons[10][4] = {
    { 61, 82, 1,100},
    { 26, 82, 1,100},
    { 27, 82, 1,100},
    { 28, 82, 1,100},
    { 36, 82, 1,100},
    { 20, 59, 1,100},
    { 32, 47, 1,100},
    { 30, 47, 1,100},
    { 28, 47, 1,100},
    {110, 98, 1,100}
				       };
  static unsigned char enemys[6][4] = {
    {32,70,10, 50},
    {32,72,50,200},
    {32,75,10, 50},
    {23,36,30, 25},
    {47,35,10, 25},
    {61,74,20, 80}
				      };

    num_doors=1;
    num_persons=10;
    num_enemys=6;
    start_x=41<<3;
    start_y=(52<<3)+1;
    finish_x=108<<3;
    finish_y=(98<<3)+1;
    start_mode=1;
    write_level(255,&doors,&persons,&enemys);
}

