/* Port Scanner header */
/* 2011年 09月 03日 星期六 01:24:57 CST */

#include    <sys/socket.h>
#include    <sys/types.h>
#include    <stdio.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <netdb.h>
#include 	<arpa/inet.h>
#include 	<netinet/in.h>
#include 	<string.h>
#include 	<ctype.h>


int   check_format(const char *dst);
void  port_scan(const char *dst);























