#include <stdio.h>
#include <stdlib.h>
/*
[chapter2] ./gets                                                                             14:40:11 
Continue?[y] n:
111111111111111122222222
over=0x7ffdf5e7a270 response=0x7ffdf5e7a260
22222222

*/


void get_y_or_n(void)
{
      
   char response[8];
   char over[8]={0};
   puts("Continue?[y] n:");

   gets(response);
   if(response[0]=='n') 
	  exit(0);

   printf("over=%p response=%p\n", over,response);
   
   puts(over);

   return;
}

int main()
{
   get_y_or_n();

   return 0;  
}
