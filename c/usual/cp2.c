/* Cat like, tiny code using C language */

#include    <stdio.h>

int main(int argc,  char *argv[])
{
    FILE *ifp;
    FILE *ofp;
    void cp2file(FILE *, FILE *);

    if (1 == argc)
        cp2file(stdin, stdout);
    else
        {
            if ((ifp = fopen(argv[1], "r")) == NULL)
            {
                printf("open file: %s error\n", argv[1]);
                return 1;
            }

            if ((ofp = fopen(argv[2], "a+")) == NULL)
            {
                printf("open file: %s error\n",  argv[2]);
                return 1;
            }
            
            cp2file(ifp, ofp);
            fclose(ifp);
            fclose(ofp);
        }
        return 0;
}

void cp2file(FILE *ifp, FILE *ofp)
{
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);

}
