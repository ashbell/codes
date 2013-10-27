#include <stdio.h>
#include <fcntl.h>
#include <utime.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, char *argv[])
{
    int i, fd;
    struct stat     statbuf;
    struct utimbuf  timebuf;

    for (i = 1; i < argc; i++)
    {
        if (stat(argv[i], &statbuf) < 0)
        {
            printf("\n Error read stat files: %s", argv[i]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR|O_TRUNC)) < 0)
        {
            printf("\n Error open file: %s", argv[i]);
            continue;
        }
        close(fd);

        timebuf.actime  = statbuf.st_atime;
        timebuf.modtime = statbuf.st_mtime;
        if (utime(argv[i], &timebuf) < 0)
        {
            printf("\n Error utime file: %s", argv[i]);
            continue;
        }
    }
    return 0;
}
