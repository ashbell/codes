
#include    <stdio.h>
#include    <unistd.h>
#include    <sys/socket.h>
#include    <arpa/inet.h>
#include    <sys/types.h>
#include    <netdb.h>
#include    <stdlib.h>
#include    <netinet/in.h>

void   print_family(struct addrinfo *); 
void   print_type(struct addrinfo *); 
void   print_protocol(struct addrinfo *); 
void   print_flags(struct addrinfo *); 
void   print_hostmsg(char *, char *);





