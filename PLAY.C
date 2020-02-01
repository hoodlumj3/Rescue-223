
#include <stdio.h>
#include <stdlib.h>

#define MAX_OCTAVES 4

enum notes {a,as,b,c,cs,d,ds,e,f,fs,g,gs};

int  note[MAX_OCTAVES][12]={
			    {110,117,123,131,139,147,156,165,175,185,196,208},
			    {220,233,248,262,277,294,311,330,349,370,392,415},
			    {440,465,493,525,555,586,623,660,695,738,785,830},
			    {880,929,985,1046,1110,1168,1248,1318,1398,1480,1568,1660}
			   };

main(int argc,char *argv[])
{
	int  notes[3][50];
	int  rests[3][50];

	unsigned int  a,b,c,ln=0;
	long counter;
	int rest0=0,rest1=0,rest2=0;
	char s[25][80];
	FILE *fname;

	printf("Here0:\n");
	if (argc!=2)
	{	printf("\nYou must include a filename!\n");
		exit(0); }
	clrscr();delay(1);
	fname=fopen(argv[1],"rt");
	while (!feof(fname))
	{	fscanf(fname,"%s\n",s[ln]);
		ln++; }
	fclose(fname);
	printf("Here3:\n");
	for (a=0;a<3;a++)
		setup(s[a],&notes[a][0],&rests[a][0]);

	a=b=c=0;
	for (counter=0;counter<2360000;counter++) {
	   if (rest0 < rests[0][a]) {rest0+=1; } else { sound(notes[0][a++]); rest0=0; }
	   if (rest1 < rests[1][b]) {rest1+=1; } else { sound(notes[1][b++]); rest1=0; }
	   if (rest2 < rests[2][c]) {rest2+=1; } else { sound(notes[2][c++]); rest2=0; }
/*	delay(1);*/
/*	sleep(1);*/
	}

  nosound();
}

setup(char str[200],int *notes,int *rests)
{    int    cnt=0,length;
	static int rest=250,restc=0,octv=0;
	int    t,n,cnt2;
	char   data[4],temp[2];
	int inc=0;
	cnt2=0;
	length=strlen(str);
	while (cnt<length) {
/*	  if (restc == rest) {
	  restc=0;
*/
	  data[0]=str[cnt];
	  data[1]=str[cnt+1];
	  data[2]=str[cnt+2];
	  data[3]=str[cnt+3];
	  if (data[0] !=',' ) {
	  printf("%c%c%c%c ",data[0],data[1],data[2],data[3]);
	  temp[0]='0';temp[1]=data[0];
	  t=atoi(temp);
/*	  printf("%0.3d : ",t);*/
	  if ((t<MAX_OCTAVES) && (t>-1))   octv=t;
	  if (data[1]!='#') {
	    switch(data[1]) {
	      case 'a':		n=a;break;
	      case 'b':		n=b;break;
	      case 'c':		n=c;break;
	      case 'd':		n=d;break;
	      case 'e':		n=e;break;
	      case 'f':		n=f;break;
	      case 'g':		n=g;
	    }
	    inc=2;
	  }
	  else {
	    switch(data[1]) {
	      case 'a':		n=as;break;
	      case 'c':		n=cs;break;
	      case 'd':		n=ds;break;
	      case 'f':		n=fs;break;
	      case 'g':		n=gs;
	    }
	    inc=3;
	  }
	  temp[0]=data[inc]; temp[1]=data[inc+1];
	  rest=atoi(temp);
	  if (rest<10)
	    inc+=1;
	  else
	    inc+=2;
/*	  rest=1000/rest;*/
	  rest=(rest*10000);
	  notes[cnt2]=note[octv][n];
	  rests[cnt]=rest;
	  printf("%0.3d %0.3d :\n",notes[cnt2],rest);
	  cnt2++;
/*	  sound(note[octv][n]);*/
/*	  delay(rest);*/
	  }
	  else {
	  inc=1;
	  }
	  cnt+=inc;
	}
/*	restc+=1;
	}
*/
	nosound();
}
