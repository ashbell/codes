#include    <stdio.h>
#include    <unistd.h>

int main(int argc, char *argv[])
{
    char *arg[2];
    arg[0] = "";
    arg[1] = NULL;
    execv("/bin/ls", arg);
    return 0;




}
