#include    "opend.h"
#include    <fcntl.h>

void request(char *, int, int);
extern int cli_args (int , char **);
extern int send_err(int, int, const char *);
extern int send_fd(int, int );
extern char errmsg[];
extern int oflag;
extern char *pathname;




