

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>

typedef unsigned char byte;

byte *code(byte *);
byte *decode(byte *);
void find_replace(byte *,byte *,byte *,int,byte);
int  find(byte *,byte *);
byte flipbits(byte);

byte codestr[90];

main(int argc, char *argv[])
{

  FILE *fp;
  byte i;
  int ch;
  unsigned long pos;

    if (argc < 2) {
      printf("Usage : %s <filename> <text>\n",argv[0]);
      exit(0);
    }
/*    printf("c : %0.1d\n",argc);*/


    if (argc <= 2) {
      printf("Enter Text : ");
      gets(codestr);
      printf("Interagating File %s\n",argv[1]);
      find_replace(argv[1],codestr,codestr,0,1);
    }
    else {
      printf("Interagating File %s\n",argv[1]);
      for (i=2;i<argc;i++) {
	strcpy(codestr,argv[i]);
	find_replace(argv[1],codestr,codestr,0,1);
      }
    }

    if(find(argv[1],"REGISTERED:")) {
      printf("File Not Registered\n");
      printf("Register Name : ");
      gets(codestr);
      find_replace(argv[1],"REGISTERED:",codestr,0,0);
/*      for(i=0;codestr[i];i++) printf("%0.2x ",codestr[i]); printf("\n");*/
    }
/*    code (codestr);*/
/*    decode(codestr);*/
/*    find_replace(argv[1],"REGD:",codestr,5);*/
/*    printf("orig |%s|\n",codestr);
/*    for(i=0;codestr[i];i++) printf("%0.2x ",codestr[i]); printf("\n");*/
*/


/*     for(i=0;i<40;i++) {codestr[i]=0;}
    code(codestr);
    printf("\ndecoded |%s|\n",codestr);
    for(i=0;codestr[i];i++) printf("%0.2x ",codestr[i]); printf("\n");
/*    printf("\n  coded |%s|\n",codestr);*/
    decode(codestr);
    printf("\ndecoded |%s|\n",codestr);
    for(i=0;codestr[i];i++) printf("%0.2x ",codestr[i]); printf("\n");
*/

}

byte *code(byte *text)
{
  byte str[90],i,ch,c=0,crc=0;
    for(i=0;i<strlen(text);i++) {
      ch=text[i];
      crc+=ch;
      str[c++]=(i+flipbits(ch&0xf0));
      str[c++]=(i+(ch&0x0f));
    }
    str[c++]=crc;
    str[c++]=0;
/*    for(i=0;i<strlen(str);i++) printf("%0.2x ",str[i]);*/
/*    printf("crc:%0.2x\n",crc);*/
/*    strcpy(text,str);*/
    return(str);
}

byte *decode(byte *text)
{
  byte str[90],i,ch1,ch2,c=0,crc=0;
    for(i=0;i<strlen(text)/2;i++) {
      str[i]=0;
      ch1=(flipbits(text[c++]-i));
      ch2=(text[c++]-i);
      str[i]=ch2|ch1;
      crc+=str[i];
/*      printf("%0.2x ",str[i]);*/
    }
    str[i]=0;

/*    for(i=0;i<strlen(text);i++) printf("%0.2x ",text[i]);*/
/*    printf(" c:%0.2x ",crc);*/

    if (text[c] != crc) printf("Not needed CRC\n");
    strcpy(text,str);
/*    return(str);*/

}

byte flipbits(byte ch)
{
  byte j,ch2;
    ch2=ch;
    ch=0;
      for(j=0;j<8;j++) {
	ch|=(ch2&0x80);
	if (j != 7) {ch>>=1;ch2<<=1;}
      }
      return(ch);
}

void find_replace(byte *filename,byte *search,byte *replace,int skip,byte type)
{
    int i;
    unsigned long pos;
    byte ch,str[80];
    FILE *fp;

/*    printf("Interagating File %s\n",filename);*/
    for(i=0;i<80;i++) str[i]=0;
    if ((fp=fopen(filename,"rb+")) == NULL) {
      printf("Couldn't Open File %s\n",filename);
      exit (1);
    }
    i=0;
    pos=0;
    ch=fgetc(fp);
    while (!feof(fp)) {
      if ( ch==search[i] && i < strlen(search) ) {
/*	if (i==0) printf("\n");*/
/*	printf("%0.4d %0.8ld : %0.2x : %c \n",i,pos,ch,ch);*/
/*	printf("%c",ch);*/
/*	printf("%0.8ld %0.2x\n",pos,ch);*/
	str[i]=ch;
	i++;
      }
      else {
	if (i==strlen(search)) {
/*	  printf("Found %0.8lx %s\n",pos-strlen(search),str);*/
	  while((!feof(fp)) && ch != 0) {
/*	    printf("%0.2x ",ch);*/
	    str[i]=ch;
	    ch=fgetc(fp);
	    i++;
	  }
	  if (fseek(fp,(pos-strlen(search)+skip),0) == 0) {
/*	    printf("Encoding : %s\n",replace);*/
	    if (type == 0) {
	      strcpy(replace,code(replace));
	    }
	    else {
	      strcpy(replace,code(str));
	    }
	      for(i=0;i<strlen(replace);i++) fputc(replace[i],fp);
	  }

	  break;
	}
	i=0;
      }
      pos+=1;
      ch=fgetc(fp);
    }
/*    printf("%0.8lx ",pos-strlen(search)); */
    fclose(fp);
}

int find(byte *filename,byte *search)
{
    int i;
    unsigned long pos;
    byte ch;
    FILE *fp;

    if ((fp=fopen(filename,"rb+")) == NULL) {
      printf("Couldn't Open File %s\n",filename);
      exit (1);
    }
    i=0;
    pos=0;
    ch=fgetc(fp);
    while (!feof(fp)) {
      if ( ch==search[i] && i < strlen(search) ) {
	i++;
      }
      else {
	if (i==strlen(search)) {
	  return(1);  /* indicate string found */
	}
	i=0;
      }
      pos+=1;
      ch=fgetc(fp);
    }
/*    printf("%0.8lx ",pos-strlen(search)); */
    fclose(fp);
    return(0);
}
