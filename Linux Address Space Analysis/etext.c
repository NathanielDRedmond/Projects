#include <stdio.h>
#include <stdlib.h>

extern char etext, edata, end;

int main() {
	printf("First address past:\n");
	printf("	program text (etext)	%10p\n", &etext);
	printf("	initialized data (edata)	%10p\n", &edata);
	printf("	unitialized data (end)	%10p\n", &end);
	
	exit(EXIT_SUCCESS);
}
