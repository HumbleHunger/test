#include <stdio.h>
void main()
{
	int i,n,k;
	int j=0;
	scanf("%d %d",&n,&k);
	char a[n+1];
	for(i=0;i<n-k+2;i++){
		if(i%2==0){
			a[i]='a';
		}
		else{
			a[i]='b';
		}
	}
	for(i=n-k+2;i<n;i++){
		a[i]='c'+j;
		j++;
	}
	for(i=0;i<n;i++){
		printf("%c",a[i]); 	
	}
}

