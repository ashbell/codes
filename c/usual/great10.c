#include    <stdio.h>
#define     GREAT  10 
#define     MAXLINE 1000

int  igetline(char s[], int lim);
void  copy(char to[], char from[]);

/* 程序在输入时候会自动加上一个\n 并自加一个 1
 * 导致字符串的长度与输入的长度差 1 的情况  实属
 * 正常的............
 */
int main(void)
{
    char store[MAXLINE][MAXLINE];
    char input[MAXLINE];
    int len;
    int i = 0;
   
    len = 0;
    while ((len = igetline(input, MAXLINE)) > 0)
        if (len > GREAT)
            copy(store[i++], input);
    if (i > 0)
        while (i-- > 0)
            printf("Great 10's strings:\t%s\n", store[i]);
    return 0;
}

int igetline(char s[], int lim)
{
    int i;
    int c;
    for (i = 0; i < lim -1 && (c = getchar()) != EOF \
            && c != '\n'; ++i)
            s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i]  = '\0';

    return i;
}

void copy(char to[], char from[])
{
    int i = 0;
    while ((to[i] = from[i]) != '\0')
        ++i;
}
