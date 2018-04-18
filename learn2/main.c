#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

int main(void)
{
	char *str[MAXSIZE];
	int i = 0;
	int n, k;
	char s[MAXSIZE] = {",,,gpon1,gpon2,gpon3,gpon10,gpon12,ge1,ge2,ge4,xe1,xe2,xe4,,,gpon1,gpon2,gpon3"};
	char *a, *b;

	n = Input(s, str, MAXSIZE, ",");
	// for(i=0;i<n;i++)
	// printf("str[%d]=%s\n",i ,str[i]);
	printf("src buf=%s\n", s);

	Sort(str, n);
	printf("After Sort :\n");
	for (i = 0; i < n; i++)
		printf("str[%d]=%s\n", i, str[i]);

	n = DelSame(str, n);
	for (i = 0; i < n; i++)
		printf("DelSame str[%d]=%s\n", i, str[i]);

	a = DealString(str, n);
	printf("a=%s\n", a);

	printf("逆反:");
	b = Trans(a);
	printf("b=%s\n", b);

	if (a)
		free(a);
	if (b)
		free(b);

	ReleaseStringArr(str, n);

	return 0;
}
