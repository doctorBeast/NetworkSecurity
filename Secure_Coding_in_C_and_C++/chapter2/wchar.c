#include <stdio.h>
#include <wchar.h>

int main()
{
     wchar_t wide_str1[] = L"0123456789";

 	 wchar_t *wide_str2 = (wchar_t*)malloc(strlen(wide_str1)+1);
	 wchar_t *wide_str3 = (wchar_t*)malloc(wcslen(wide_str1)+1);
	 wchar_t *wide_str4 = (wchar_t*)malloc( (wcslen(wide_str1)+1) * sizeof(wchar_t) );
	 wchar_t *wide_str5 = (wchar_t*)malloc(sizeof(wide_str1));

	 printf("wide_str1: %lu\n", sizeof(wide_str1));
	 printf("wide_str2: %lu\n", strlen(wide_str1)+1);
	 printf("wide_str3: %lu\n", wcslen(wide_str1)+1);
	 printf("wide_str4: %lu\n", (wcslen(wide_str1)+1)*sizeof(wchar_t));
	 printf("wide_str5: %lu\n", sizeof(wide_str1));

	 return 0;
}
