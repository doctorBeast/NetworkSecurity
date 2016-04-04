#include <stdio.h>
#include <string.h>

/*
  
*/

int main(int argc, char* argv[])
{

	char prog_name[8];
	char over[8]={0};

	strcpy(prog_name, argv[0]);

	printf("prog_name: %p\nover: %p\n", prog_name, over);

	printf("over=%s\n", over);

	return 0;
}	

