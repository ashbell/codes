#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t tm;
    time (&tm);
    printf("%s\n", asctime(gmtime(&tm)));
    return 0;


}
