#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int ret;
   char buf[1024];
    if ((ret = open(argv[1], O_CREAT|O_APPEND, 00700)) < 0)
        printf("error open file\n");
    sprintf(buf, "%s:%s\n", argv[2], argv[3]);
    printf("%s\n", buf);
    lseek(ret, 0, SEEK_END);
    write(ret, buf, sizeof(buf));
    close(ret);
    return 0;





}
