#include <stdio.h>
void main()
{
	int n;
	int j,k,l;
	while(scanf("%d",&n)!=EOF){
		for(j=1;j<=n;j++){
			for(k=0;k<=j-1;k++){
				long double cn1=1;
				long double cn2=1;
				for(l=j-1;l>j-k-1;l--){
					cn1=cn1*l;
				}
				for(l=k;l>0;l--){
					if(k==0){break;}
					cn2=cn2*l;
				}
				if(k==0){printf("1");}
				else{printf(" %.0Lf",cn1/cn2);}
			}
		printf("\n");
		}
		printf("\n");
	}
}
