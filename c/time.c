#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define ERR_QUIT(MSG)  do {printf("error msg: %s.\n", MSG); exit(0);} while(0)

/*Fri Nov 16 20:10:52 CST 2012 */
int main(int argc, char *argv[])
{
    struct timeval *timeval;
    struct tm      *tm;
    time_t time0;
    
    if (!(gettimeofday(timeval, NULL))) {
       ERR_QUIT("gettimeodfay return non-zero!\n");
    }
    tm = gmtime (&(timeval->tv_sec));
    printf("TIME-> gettimeofday(): %s\n", asctime(tm));
    time0 = time(NULL);
    printf("TIME-> time(): %s\n", asctime(gmtime(&time0)));
    return 0;
    

 




}








