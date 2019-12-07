#include <stdio.h>
void main()
{
	int a,i,j,k;
	int n,m;
	int num[a][2];
	scanf("%d",&a);
	for ( i=0;i<a;i++ ) {
		scanf("%d %d",&num[i][0],&num[i][1]);
	}
	for ( i=0;i<a;i++ ) {
		m=0;
		for ( j=num[i][0];j<=num[i][1];j++ ) {
			n=1;
			for ( k=2;k<j;k++ ) {
				if( j%k==0 ){
					n=n+k;
				}
			}
			if(n==j){
			m++;
			}
		}
		printf("%d\n",m);
	}
}
