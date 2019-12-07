#include <stdio.h>
void main()
{
        int i,j,k;
        int num[2][2];
        for ( i=0;i<2;i++ ) {
                scanf("%d %d",&num[i][0],&num[i][1]);
        }
        for ( i=0;i<2;i++ ) {
		k=0;
                for ( j=num[i][0];j<=num[i][1];j++ ) {
			int g,s,b;
			g=j%10;
			s=(int)j%100/10;
			b=(int)j/100;
			if(j==g*g*g+s*s*s+b*b*b){
				k=printf("%d ",j);
			}
                }
                if(k==0){printf("no");}
		printf("\n");
        }
}

