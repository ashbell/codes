#include    "apue.h"
#define MAX 1024

int main(void)
{                       
    int     int1, int2;
    char    line[MAX];
    while (fgets(line, MAX, stdin) != NULL) {
        line[n] = 0;
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {
            if (printf("%d\n", int1 + int2) == EOF)
                printf("error printf\n");
        } else {
            if (printf("invalid args\n") == EOF)
                printf("error printf else\n");
        }
    }
    return 0;

}



























