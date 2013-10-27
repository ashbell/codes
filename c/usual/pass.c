#include    <stdio.h>
#include    <string.h>

#define PASSWORD "1234567"

int verify_password(char *password)
{
    int authenticated;
    char buffer[8];
    authenticated = strcmp(password, PASSWORD);
    strcpy(buffer, password);
    return authenticated;
}

int main(void)
{
    int valid_flag = 0;
    char password[1024];

    while(1)
    {
        printf("Enter input password:    ");
        scanf("%s", password);
        valid_flag = verify_password(password);
        if (valid_flag)
            printf("incorrect password\n\n");
        else
            printf("YES right\n");
    }
}
