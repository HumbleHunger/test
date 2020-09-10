/*************************************************************************
	> File Name: 32.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月15日 星期三 16时28分57秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    pid_t pid;
    pid=fork();
    switch(pid){
        case 0:
        exit(0);
        break;
        default:
        sleep(3);
    }
    return 0;
}
