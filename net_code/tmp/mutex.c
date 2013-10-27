
/* 简单的互斥量文件保护 */
/* 2011年 08月 24日 星期三 02:16:59 CST */

#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <pthread.h>
#include    <sys/types.h>

struct foo {
    int     
