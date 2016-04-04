#include <stdio.h>

#define C  'h' //==sizeof(int)

int main()
{  
   char A='h';
   const  char* B = (const char*)'h';
   printf("sizeof(int): %lu\n", sizeof(int));
   printf("A:%lu\nB:%lu\nC:%lu\n",sizeof(A), sizeof(B), sizeof(C));
 
  return 0;
}
