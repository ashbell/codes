#include 	<stdio.h>
#include    <string.h>
#define MAX 100

int main(void)
{
    char s[] = "Private Message!";
    char  t[MAX];
    int i;
    strcpy(t, s);
    while (t[i])
    printf("%c\n", t[i++]);
	return 0;

}
