#include <stdio.h>
#include <string.h>
void main()
{
        char a[2][100000];
        int s,k,n,m,i,j;
        while(scanf("%s %s",a[0],a[1])!=EOF){
        char b[2][100000];
	int ch=2;
	for(i=0;i<2;i++){
        ch=2;
        j=0;
		while(a[i][j]!=0){
			if(a[i][j] == '.'){
				ch=1;
				s=j;
				break;
                	}
			j++;
		}
	switch(ch){
		case 1: j=s+1;
			int cd = 0;
			while(a[i][j]!=0){
				if(a[i][j]!='0'){
                			k=0;
                			j=0;
			                m=n=0;
                			while(a[i][j]!=0){
                        			if(a[i][j]>=49 && a[i][j]<=57){
                                			if(k==0){n=j;k=1;}
                                			m=j;
                        			}
                       				j++;
                			}
                			for(k=n,j=0;k<=m;k++,j++){
                        			b[i][j]=a[i][k];
                			}
					cd=1;
					break;
				}
				j++;
			}
        if(cd==0){
            j=m=n=k=0;
                while(a[i][j]!=0){
				if(a[i][j]>=49 && a[i][j]<=57){
					n=j;
					break;
				}j++;}
			k=n;
			j=0;
			while(a[i][k]!='.'){
				b[i][j]=a[i][k];
				j++;k++;
			}
            }break;
		case 2: k=j=n=m=0;
			while(a[i][j]!=0){
                if(a[i][j]>=49 && a[i][j]<=57){
					n=j;
					break;
				}j++;}
			k=n;
			j=0;
			while(a[i][j]!=0){
				b[i][j]=a[i][k];
				j++;k++;
			}break;
			}
	}
	k=strcmp(b[0],b[1]);
    if(k==0){printf("YES\n");}
    else{printf("NO\n");}
    }
	}	
