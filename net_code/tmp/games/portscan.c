/* Port Scanner using pure C language */

#include    "portscan.h"


int   check_format(const char *dst)
{
    int    nip0;
    int    nip1;
    int    nip2;
    int    nip3;
    sscanf(dst, "%d.%d.%d.%d", &nip0, &nip1, &nip2, &nip3);
    
    if ((nip0 >= 255) || (nip1 >= 255) || (nip2 >= 255))
    {
       printf("IP Address greater than 255!\n");
       return 0;
    }
    printf("IP integer: %d.%d.%d.%d\n",
            nip0, nip1, nip2, nip3);
    return 1;
}

void  portscan(const char *dst)
{
    char            **pp;
    struct  in_addr addr;
    struct  hostent  *hostp;

    if (!dst)
    {
        fprintf(stderr, "usage: %s [Domain || IP4 Address]\n", "portscan");
        exit(0);
    }
    if (inet_aton(dst, &addr) != 0)
    {
        printf("gethostbyaddr()\n");
        printf("%d\n", inet_aton(dst, &addr));
        hostp = gethostbyaddr((const char *)&addr, sizeof(addr), 0);
    }
    else 
    {
        printf("gethostbyname\n");
        hostp = gethostbyname(dst);
    }
    
    if (!hostp)
    {
        printf("Fatal: Hostp is NULL\n");
        exit(0);
    }
    else
    {
        if (hostp->h_name)
        printf("official hostname: %s\n", hostp->h_name);

        for (pp = hostp->h_aliases; *pp != NULL; pp++)
        {
           printf("alias: %s\n", *pp); 
        }

        for (pp = hostp->h_addr_list; *pp != NULL; pp++)
        {
            addr.s_addr = *((unsigned int *)*pp);
            printf("address: %s\n", inet_ntoa(addr));
        }
    }

}

int main(int argc, char *argv[])
{
    if (isdigit(argv[1][0])) 
    {
        if (!check_format(argv[1]))
        {
            printf("IP check_format() return false\n");
            exit(0);
        }
        else
        {
            portscan(argv[1]);
        }
    }
    else
    {
        portscan(argv[1]);
    }
    return 0;
}




