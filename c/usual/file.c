#include 	<stdio.h>

#define MAXLEN 100

struct customers
{
    char name[MAXLEN];
    char no[MAXLEN];
    int order;
};


int read_line(char *argv)
{
    FILE *ofp;
    struct customers out;
    int i = 0;

    if ((ofp = fopen("sar",  "r")) == NULL)
    {
        printf("error when open file: ");
        return 1;
    }
    printf("read line from file and copy to stdout \n");
    while (++i < 4) 
    {
        fscanf(ofp, "%d: %s %s %d\n", &i, out.name,
            out.no, &out.order);
    
        fprintf(stdout, "%-4d %-6s%-6s%-4d\n", i, out.name,
                out.no, out.order);
    }
    fclose(ofp);
}

int main(int argc, char *argv[])
{
	FILE *data;
    int i = 0;
    struct customers customer; 
    printf("Enter the information of customers, ctrl+d\n");
        
    if ((data = fopen(argv[1], "a")) == NULL)
    {
        printf("error when open file: %s\n", argv[1]);
        return 1;
    }
    else do 
    {
        printf("\n>>>");
        fscanf(stdin, "%s %s %d", customer.name, customer.no,
                &customer.order);
        fprintf(data, "%d:  %-6s    %-6s    %-4d        \n", ++i, customer.name,
                customer.no, customer.order);
    } while(i < 3);
    
    fclose(data);
    read_line(argv[1]);
    return 0;
}






