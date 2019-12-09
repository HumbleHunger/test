#include <stdio.h>
void main()
{
        int j,k;
        int num[2];
        while(scanf("%d %d",&num[0],&num[1])!=EOF){
		k=0;
                for ( j=num[0];j<=num[1];j++ ) {
			int g,s,b;
			g=j%10;
			s=(int)j%100/10;
			b=(int)j/100;
			if(j==g*g*g+s*s*s+b*b*b){
				if(k==0){printf("%d",j);}
				else{printf(" %d",j);}
				k++;
			}
                }
                if(k==0){printf("no");}
		printf("\n");
       }
}
