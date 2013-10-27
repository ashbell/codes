#include    <stdio.h>
#include    <unistd.h>
#include    <sys/types.h>
#include    <fcntl.h>
#include    <stdlib.h>
#include    <sys/stat.h>
#include    <signal.h>
#include    "tellwait.c"


int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock    lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;
    return (fcntl(fd, cmd, &lock));
}

#define read_lock(fd, offset, whence, len) \
        lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
        lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
        lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
        lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
        lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

static void lockabyte(const char *name, int fd, off_t offset)
{
    printf("in lockabyte(): %s !\n", name); 
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
    {
        printf("error when writew_lock()!\n");
    }
    printf("%s: got the lock, byte %ld\n", name, offset);
    printf("lockabyte() exit!\n");
}

int main(void)
{
    int     fd;
    pid_t   pid;

    if ((fd = creat("templock", FILE_MODE)) < 0)
    {
        printf("error when creat()!\n");
    }
    if (write(fd, "ab", 2) != 2)
        printf("error when write 2 bytes!\n");
    TELL_WAIT();
    if ((pid = fork()) < 0)
    {
        printf("fork() error!\n");
    }
    else if (pid == 0)
    {
        lockabyte("child", fd, 0);
        printf("The pid we will send %ld!\n", (long int)getpid());
        TELL_PARENT(getpid());
        WAIT_PARENT();
        lockabyte("child", fd, 1);
        printf("lockabyte child ok!\n");
    }
    else
    {
        lockabyte("parent", fd, 1);
        TELL_CHILD(pid);
        WAIT_CHILD();
        lockabyte("parent", fd, 0);
    }
    return 0;
}


