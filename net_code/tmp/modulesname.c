/* 2011年 09月 15日 星期四 23:55:49 CST */

#include    "apue.h"
#include    <fcntl.h>
#include    <stropts.h>
//#include    <sys/conf.h>
#include    <unistd.h>

int isastream(int fd)
{
    return (ioctl(fd, I_CANPUT, 0) != -1);
}


int main(int argc, char *argv[])
{
    int                 fd, i, nmods;
    struct str_list     list;
    if ((fd = open(argv[1], O_RDONLY)) < 0)
        printf("can't open fd!\n");
    if (isastream(fd) == 0)
        printf("%s is not a stream\n", argv[1]);

    if (nmods = ioctl(fd, I_LIST, (void *) 0) < 0)
        printf("I_LIST err from nmods\n");
    printf("#modules = %d\n", nmods);
    list.sl_modlist = calloc(nmods, sizeof(struct str_list));
    if (list.sl_modlist = NULL)
        printf("calloc error\n");
    list.sl_nmods = nmods;

    if (ioctl(fd, I_LIST, &list) < 0)
        printf("I_LIST error for list!\n");

    for (i = 1; i <= nmods; i++)
        printf(" %s\n", (i == nmods) ? "driver" : "modules", list.sl_modlist++->l_name);
    return 0;
}
/* 2011年 09月 16日 星期五 17:21:07 CST */









