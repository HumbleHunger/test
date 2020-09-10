/*************************************************************************
	> File Name: 3.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月15日 星期三 16时27分22秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid;
    pid=fork();
    switch(pid){
        case 0:
        while(1){
            printf("ppid %d",getppid());
            sleep(1);
        }
        break;
        case -1:
        printf("fork faild\n");
        break;
        default:
        exit(0);
    }
    return 0;
}
