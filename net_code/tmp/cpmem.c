/* 2011年 09月 18日 星期日 21:27:51 CST */ 
#include    "apue.h"
#include    <stdio.h>
#include    <fcntl.h>
#include    <sys/mman.h>

/*
 * 为什么要往里面写入一个字节的内容呢？
 * 大师说映射文件的开始偏移量受系统虚拟存储页长的限制，映射区
 * 的长度必须是页长的整数倍, 所以需要加长 lseek and write
 */
int main(int argc, char *argv[])
{
    int         fdin, fdout;
    void        *src, *dst;
    struct stat statbuf;
    struct stat outstat;

    if ((fdin = open(argv[1], O_RDONLY)) < 0) 
        printf("error when open %s !\n", argv[1]);

    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC |  FILE_MODE)) < 0) 
        printf("error when open %s !\n", argv[1]);
    if (fstat(fdin, &statbuf) < 0)
        printf("error when fstat !\n");
    printf("the file's size is %d\n", (int)statbuf.st_size);
    if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1)
        printf("lseek error !\n");
    if (write(fdout, "", 1) != 1) /*why would u do that? */
        printf("error when write !\n");

    if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED)
        printf("mmap failed !\n");
    if ((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED)
        printf("mmap dst failed !\n");

    memcpy(dst, src, statbuf.st_size);
    close(fdin);
    close(fdout);

    return 0;
}

