#include <stdio.h>
#include <dos.h>

/*void menu();*/

unsigned char door[20*10],person[20];
unsigned int start_x,start_y;

main(int argc,char *argv[])
{
  FILE *fp;
  unsigned int key;

/*    if (argc != 2) {
      printf("No Filename\n");
      exit(1);
    }
    if (argv[1][0] == '?') {
      printf("\nLevel Data Editor (C) 1993 M.J.Edwards Inc.\n");
      printf("Usage %s <filename>\n",argv[0]);
      exit(2);
    }

    if ((fp=fopen(argv[1],"wb")) == NULL) {
      printf("Could Not Open File %s\n",argv[1]);
      exit(0);
    }
    menu();
    key=getch();
    while(key != 'q') {
      menu();
      key=getch();
      printf("%c\n",key);
    }
*/

    if ((fp=fopen("lev003.dat","wb")) == NULL) {
      printf("Could Not Open File \n");
      exit(0);
    }


    fclose(fp);
}

void level001_init()
{
    write_level();
}
void level002_init()
{
    write_level();
}
void level003_init()
{
    write_level();
}


void write_level()
{

    fputc(fp,num_doors);
    for (i=0;i<num_doors*10;i++) {
      fputc(fp,door[i]);
    }

    fputc(fp,num_persons);
    for (i=0;i<num_persons;i++) {
      fputc(fp,persons[i]);
    }
    fwrite(&start_x,sizeof(start_x),1,fp);
    fwrite(&start_y,sizeof(start_y),1,fp);
    fputc(start_mode);

}