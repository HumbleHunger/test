#include <stdio.h>
void main()
{
    int i,a;
    scanf("%d",&a);
    char p[a][100];
    for(i=0;i<a;i++){
    scanf("%s",p[i]);
    }
   int j,n;
   for (i=0; i<a; i++) {
   	j=0;
   	n=0;
   	while(p[i][j] != '\0'){
   			if (p[i][j]>47 && p[i][j] <58) {
   			n++;
			}j++;
   		}
   	printf("%d\n",n);
    }
}
