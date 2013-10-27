#include <stdio.h>
#include <getopt.h>

#define null 0
char* const short_options = "absk:g:";
struct option long_options[] = { 
{ "ABCZ", 0, null, 'a'},
{ "BCDE", 0, null, 'b'},
{ "STAR", 0, null, 's'},
{ "KING", 1, null, 'k'},
{ "GSAR", 1, null, 'g'},
{ 0 , 0 , 0 , 0},
};

int main(int argc, char *argv[])
{
    int c;
    while ((c = getopt_long(argc, argv, short_options, \
            long_options, null)) != -1)
    {
        switch (c) {
            case 'a' : printf("it's argv : a (ABCZ)\n"); break;
            case 'b' : printf("it's argv : b (BCDE)\n"); break;
            case 's' : printf("it's argv : s (STAR)\n"); break;
            case 'k' : printf("it's argv : k (%s)\n", optarg); break;
            case 'g' : printf("it's argv : g (%s)\n", optarg); break;
        }
    } 




}
