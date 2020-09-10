/*************************************************************************
	> File Name: 2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月15日 星期三 15时22分48秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main(int argc, char *argv[])
{
for (int i = 0; i < 2; i++)
{
pid_t pid = fork();
printf("hello, world?\n");
}
return 0;
}
