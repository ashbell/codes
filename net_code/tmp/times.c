#include    <stdio.h>
#include    <sys/times.h>
#include    <sys/types.h>
#include    <unistd.h>


static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

int main(int argc, char *argv[])
{
    int     i;

    setbuf(stdout, NULL);
    for (i = 1; i < argc; i++)
    {
        do_cmd(argv[i]);
    }
    
}

static void do_cmd(char *cmd)
{
    struct tms tms_start, tms_end;
    clock_t    start, end;
    int        status;

    printf("\nCommand: %s\n", cmd);

    if ((start = times(&tms_start)) == -1)
    {
        printf("status time start error\n");
        
    }
    if ((status = system(cmd)) < 0)
    {
        printf("error when system cmd\n");
        
    }
    if ((end = times(&tms_end)) == -1)
    {
        printf("status time end error\n");
        
    }
    
    pr_times(end - start, &tms_start, &tms_end);
   
}

static void pr_times(clock_t real, struct tms *tms_start,
                    struct tms *tms_end)
{
    static long     clktck = 0;
    if (clktck == 0)
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
            printf("error sysconf times\n");

    printf("    real:   %7.2f\n", real / (double) clktck);
    printf("    user:   %7.2f\n",
        (tms_end->tms_utime - tms_start->tms_utime) / (double) clktck);
    printf("    sys:   %7.2f\n",
        (tms_end->tms_stime - tms_start->tms_stime) / (double) clktck);
    printf("    child user:   %7.2f\n",
        (tms_end->tms_cutime - tms_start->tms_cutime) / (double) clktck);
    printf("    chlid sys:   %7.2f\n",
        (tms_end->tms_cstime - tms_start->tms_cstime) / (double) clktck);
}






