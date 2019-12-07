#include <stdio.h>
void main()
{
	int a;
	scanf("%d", &a);
	if ( a%2 == 0 ) {
		if ( a%4 == 0 ){
	printf("%d %d",a/4,a/2);
		}
		else {
	printf("%d %d",a/4+1,a/2);
		}
	}
	else {
	printf("0 0");
	}
}
