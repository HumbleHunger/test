#include <pthread.h>
#include <stdio.h>
int count = 0; 

pthread_mutex_t mutex;
pthread_cond_t cond;

void *thread_entry(void *arg)
{
    printf("This is thread_entry\n");
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("child count: %d\n", count);
        count++;

        pthread_mutex_unlock(&mutex);
        if (count > 100){
            pthread_cond_signal(&cond);
            return (void *) count;
        }
    }
}

void *pthread_cond_test(void *arg)
{
    pthread_mutex_lock(&mutex);
    if(count < 100){
        printf("Thread %d is waiting for cond\n",pthread_self());
        pthread_cond_wait(&cond,&mutex);
    }
    printf("Thread %d is awakened\n",pthread_self());
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
    printf("This is pthread_test\n");
    for(int i = 0; i < argc; ++i){
        printf("argv[%d]= %s\n",i,argv[i]);
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond,NULL);
    pthread_t thread,thread1;
    if (pthread_create(&thread, NULL, pthread_cond_test, NULL) < 0) {
        printf("create thread failed!\n");
        return -1;
    }
    if (pthread_create(&thread1, NULL, thread_entry, NULL) < 0) {
        printf("create thread failed!\n");
        return -1;
    }

    while (1) {
        pthread_mutex_lock(&mutex);
        printf("parent count: %d\n", count);
        count++;
        
        pthread_mutex_unlock(&mutex);
        if (count > 100)
            break;
    }

    void *status;
    pthread_join(thread, &status);
    printf("thread exit with %x!\n", status);
    return 0;
}
