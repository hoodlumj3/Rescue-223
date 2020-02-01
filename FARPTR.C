#include <stdio.h>
#include <alloc.h>

char far *getptr();

main()
{
	char far *b;

	b = getptr();
	printf("%08lx\n",b);
	return;
}

char far *getptr()
{
  char far *mem;

	if ((mem=farmalloc((long)1024)) == NULL) {
		puts("Failed mem alloc");
	}
	printf("%08lx\n",mem);
	free(mem);
	return (mem);
}