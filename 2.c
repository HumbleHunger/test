#include <stdio.h>
typedef struct a {
    int *a;
    int *b;
}S;
int main()
{
    S p;
    int q=0;
    int e=0;
    p.a = &q;
    p.b = &e;
    p->a = 1 ;
    p->b = 1;
    p->a = 2 ;
    p->a = 2 ;
    p->a = 3 ;
    p->a = 3 ;
    p->b = 4;
    p->b = 4;
    p->b = 5;
    p->b = 5;

}
