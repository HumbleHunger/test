#include <stdio.h>
#include <unistd.h>
 
int main() {
    pid_t pid;
    pid = fork();
    if(pid  == 0) //返回子进程
    {
        printf("child pid: %d\n", getpid());
    } else {
        printf("pid: %d\n", pid);//父进程中返回子进程的pid
        printf("father pid: %d\n", getpid());
    }
}
