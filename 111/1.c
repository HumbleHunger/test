/*************************************************************************
	> File Name: 1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月15日 星期三 15时06分33秒
 ************************************************************************/

#include<stdio.h>
typedef struct tag_unimportant {
char *t2;
int t1;
} A;
void func(A **a)
{
    *a = (A *)malloc(sizeof(A));
    (*a)->t1 = 0x20200011;
    (*a)->t2 = (char* )&((*a)->t1);
    *((*a)->t2) = 0x00;
    //strcpy(a->t2, "xiyoulinux");
}
int main(int argc, char *argv[])
{
    A *a;
    func(&a);
    printf("%x\n", a->t1);
    //printf("%s\n", a->t2);
    return 0;
}
