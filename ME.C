
#include <stdio.h>
#include <alloc.h>
#include <mem.h>
#include <dos.h>

main()
{
  unsigned long i;
  char far *memory1;
  char far *memory2;
  char far *memory3;
  char far *memory4;

	puts("New Run");
	i=(128L*1024L);
	printf("%ld\n",i);
	if ((memory1=farmalloc(i)) == NULL) {
      printf("Couldn't malloc mem1 of size %0.8ld\n",(long)i);
      exit(1);
    }
    printf("Malloc'd %0.8ld bytes of mem1 \n",(long)i);
	i=(128L*1024L);
    if ((memory2=farmalloc(i)) == NULL) {
      printf("Couldn't malloc mem2 of size %0.8ld\n",(long)i);
      exit(1);
    }
    printf("Malloc'd %0.8ld bytes of mem2 \n",(long)i);
	i=(128L*1024L);
    if ((memory3=farmalloc(i)) == NULL) {
      printf("Couldn't malloc mem3 of size %0.8ld\n",(long)i);
      exit(1);
    }
    printf("Malloc'd %0.8ld bytes of mem3 \n",(long)i);
	i=(128L*1024L);
    if ((memory4=farmalloc(i)) == NULL) {
      printf("Couldn't malloc mem4 of size %0.8ld\n",(long)i);
      exit(1);
    }
    printf("Malloc'd %0.8ld bytes of mem4 \n",(long)i);
    farfree(memory4);
    farfree(memory3);
    farfree(memory2);
    farfree(memory1);
  return(0);

}

