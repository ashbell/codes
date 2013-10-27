#include 	<stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;
    FILE *out;
	void filecopy(FILE *, FILE *);

	if (argc == 1)
		filecopy(stdin, stdout);
	else
	   while (--argc > 0)
		   if ((fp = fopen(argv[1], "r")) == NULL) {
			   printf("cat: can't open %s\n", *argv);
			   return 1;
		   } else {
               //if ((out = fopen(argv[2], "r")) == NULL) {
                 //   printf("error open file\n");
                   // return 1;
                //}
			   filecopy(fp,  stdout);
			   fclose(fp);
              // fclose(out);
		   }
	return 0;
}

void filecopy(FILE *ifp, FILE *ofp)
{
	int c;

	while ((c = getc(ifp)) != EOF)
		putc(c, ofp);
}




