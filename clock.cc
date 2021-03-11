#include <iostream>
#include <time.h>
using namespace std;
int main()
{
    struct timespec abstime;
    struct timespec abstime1;
    clock_gettime(CLOCK_REALTIME,&abstime);
    cout << abstime.tv_sec << " " << abstime.tv_nsec << endl;
    clock_gettime(CLOCK_MONOTONIC,&abstime1);
    cout << abstime1.tv_sec << " " << abstime1.tv_nsec << endl;
    return 0;
}

