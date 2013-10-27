#include    <stdio.h>
#include    <stropts.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <fcntl.h>


int isastream(int fd)
{
    return (ioctl(fd, I_CANPUT, 0) != -1);
}

int main(int argc, char *argv[])
{
    int     i, fd;

    for (i = 1; i < argc; i++) {
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
            printf("error when open()!\n");
            continue;
        }
        if (isastream(fd) == 0)
            printf("%s in not a stream!\n", argv[i]);
        else
            printf("%s stream device\n", argv[i]);
    }
    return 0;
}





























