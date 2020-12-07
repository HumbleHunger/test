#include <stdio.h>
int main()
{
        int j;
        int n;
        scanf("%d",&n);
        int num[2]={1,10};
        for(int i=1;i<n;i++){
            num[0]*=10;
            num[1]*=10;
        }
        num[1]=num[1]-1;
        for(j=num[0];j<=num[1];j++){
		    int g,s,b;
			g=j%10;
			s=(int)j%100/10;
			b=(int)j/100;
			if(j==g*g*g+s*s*s+b*b*b){
				printf("%d\n",j);
			}
        }
}
