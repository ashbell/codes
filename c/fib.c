
#include <stdio.h>
#define MAX 100

int main(void)
{
	/*int t[MAX] = {1,1};
	int R = 0;
	int n;
	int i;
	scanf("%d", &n);
	for (i = 1; i < n; i++) {
	    R = t[i-1] + t[i];
	    t[i-1] = t[i];
	    t[i+1] = R;
	 }
	printf ("that is %d\n", R);
	*/
	int i, t0, t1, n, t;
	scanf("%d", &n);
	t0 = t1 = 1;
	for (i = 1; i < n; i++) {
	    t = t0 + t1;
	    t0 = t1;
	    t1 = t;
	}
	printf("that fib is %d\n", t);
	return 0;


}
