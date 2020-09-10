/*************************************************************************
	> File Name: 5.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月15日 星期三 15时43分33秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
pthread_mutex_t a,b,c;
typedef struct {
    pthread_mutex_t *mutex;
    char ch;
    pthread_mutex_t *next;
}date;
date A,B,C;
void *thread(void *arg)
{
    while(1){
        pthread_mutex_lock(((date *)arg)->mutex);
        printf("%c",((date *)arg)->ch);
        pthread_mutex_unlock(((date *)arg)->next);
    }
}
int main()
{
    pthread_mutex_init(&a,NULL);
    pthread_mutex_init(&b,NULL);
    pthread_mutex_init(&c,NULL);
    pthread_t th[3];
    A.mutex=&a;
    A.ch='A';
    A.next=&b;
    B.mutex=&b;
    B.ch='B';
    B.next=&c;
    C.mutex=&c;
    C.ch='C';
    C.next=&a;
    pthread_mutex_lock(&b);
    pthread_mutex_lock(&c);
    pthread_create(&th[0],NULL,thread,(void *)&A);
    pthread_create(&th[1],NULL,thread,(void *)&B);
    pthread_create(&th[2],NULL,thread,(void *)&C);
    sleep(1);
    close(1);
    pthread_mutex_destroy(&a);
    pthread_mutex_destroy(&b);
    pthread_mutex_destroy(&c);
    return 0;
}
