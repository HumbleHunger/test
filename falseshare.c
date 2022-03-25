#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//int num_0;
//int num_1;
int num_0 __attribute__ ((aligned(64)));
int num_1 __attribute__ ((aligned(64)));


void *thread_0(void *args)
{
        int i = 100000000;
        while (i--) {
                num_0++;
        }
}

void *thread_1(void *args)
{
        int i = 100000000;
        while (i--) {
                num_1++;
        }
}

int main(int argc, char *argv[])
{
        pthread_t thread[2];

        pthread_create(&thread[0], NULL, thread_0, NULL);
        pthread_create(&thread[1], NULL, thread_1, NULL);
        pthread_join(thread[0], NULL);
        pthread_join(thread[1], NULL);

        return EXIT_SUCCESS;
}

