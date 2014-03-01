#include <stdio.h>
#include <unistd.h>

int main(void)
{
	char sz[1000];
	char *buf = sz;
	size_t len = 1000;
	printf("cwd=%s\n", getcwd(buf,len ));
	return 0;
		


}





