/* 2011年 08月 30日 星期二 09:57:19 CST */

#include 	"print_hostmsg.h"

void print_family(struct addrinfo *aip)
{
	switch (aip->ai_family)
	{
	  case AF_INET:
		  printf("%16s", "inet");
		  break;
	  case AF_INET6:
		  printf("%16s", "inet6");
		  break;
	  case AF_UNIX:
		  printf("%16s", "unix");
		  break;
	  case AF_UNSPEC:
		  printf("%16s", "unspecified");
		  break;
	  default:
		  printf("%16s", "unknown");
	}
}

void print_type(struct addrinfo *aip)
{
	switch (aip->ai_socktype)
	{
	  case SOCK_STREAM:
		  printf("%16s", "stream");
		  break;
	  case SOCK_DGRAM:
		  printf("%16s", "datagram");
		  break;
	  case SOCK_SEQPACKET:
		  printf("%16s", "seqpacket");
		  break;
	  case SOCK_RAW:
		  printf("%16s", "raw");
		  break;
	  default:
		  printf("%16s", "unknown");
	}
}


void print_protocol(struct addrinfo *aip)
{
	switch (aip->ai_protocol)
	{
	  case 0:
		  printf("%16s", "default");
		  break;
	  case IPPROTO_TCP:
		  printf("%16s", "tcp");
		  break;
	  case IPPROTO_UDP:
		  printf("%16s", "udp");
		  break;
	  case IPPROTO_RAW:
		  printf("%16s", "raw");
		  break;
	  default:
		  printf("%16s", "unknown");
	}
}

void print_flags(struct addrinfo *aip)
{
	if (aip->ai_flags == 0)
		printf("%16s", "0");
	else
	{
		if (aip->ai_flags & AI_PASSIVE)
			printf("%16s", "passive");
		if (aip->ai_flags & AI_CANONNAME)
			printf("%16s", "canon");
		if (aip->ai_flags & AI_NUMERICHOST)
			printf("%16s", "numghost");
#if defined(AI_NUMERICSERV)
		if (aip->ai_flags & AI_NUMERICSERV)
			printf("%16s", "numserv");
#endif
#if defined(AI_V4MAPPED)
		if (aip->ai_flags & AI_V4MAPPED)
			printf("%16s", "v4mapped");
#endif
#if defined(AI_ALL)
		if (aip->ai_flags & AI_ALL)
			printf("%16s", "all");
#endif
	}
}

void print_hostmsg(char *hostname, char *hostserv)
{
	struct addrinfo 	*ailist, *aip;
	struct addrinfo 	hint;
	struct sockaddr_in  *sinp;
	const char 			*addr;
	int 				err;
	char 				abuf[INET_ADDRSTRLEN];
	
	hint.ai_flags 	= AI_CANONNAME;
	hint.ai_family 	= 0; 
	hint.ai_socktype = 0;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr  = NULL;
	hint.ai_next  = NULL;
	if ((err = getaddrinfo(hostname, hostserv, &hint, &ailist)) != 0)
	{
		printf("getaddinfo(): unknown servername: %s\n", hostserv);
//		exit(0);
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		printf("host %s", aip->ai_canonname ? aip->ai_canonname : "unkonwn");
		if (aip->ai_family == AF_INET)
		{
			sinp = (struct sockaddr_in *)aip->ai_addr;
			addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
			printf("\naddress %s", addr ? addr : "unknown");
			printf("    port:   %d", ntohs(sinp->sin_port));
		}
		printf("\n%16s%16s%16s%16s", "Family", "Type", "Prot", "Flags"); 
        printf("\n");
        print_family(aip);
		print_type(aip);
		print_protocol(aip);
        print_flags(aip);
		printf("\n");
	}
	exit(0);
}

/* 2011年 08月 30日 星期二 11:03:49 CST */
