#include <stdio.h>
void main()
{
	int n[2];
	int j,k,l,i;
	scanf("%d %d",&n[0],&n[1]);
	for(i=0;i<2;i++){
		for(j=1;j<=n[i];j++){
			for(k=0;k<=j-1;k++){
				int cn1=1;
				int cn2=1;
				for(l=j-1;l>j-k-1;l--){
					cn1=cn1*l;
				}
				for(l=k;l>0;l--){
					if(k==0){break;}
					cn2=cn2*l;
				}
				if(k==0){printf("1 ");}
				else{printf("%d ",cn1/cn2);}
			}
		printf("\n");
		}
		printf("\n");
	}
	
}
