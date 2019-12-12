#include <stdio.h>
#include <stdlib.h>
void main()
{
    char a[502],b[502],c[503];
    int j=0;
    int z,x,y,k;
    while(scanf("%s %s",a,b)!=EOF){
        int i=0;
        int p=0;
        z=x=y=k=j=i=0;
        while(a[i]!=0){
            z=i;
            i++;
        }
        i=0;
        while(b[i]!=0){
            x=i;
            i++;
        }
        for(;z>=0,x>=0;z--,x--){
            p=atoi(&a[z])+atoi(&b[x])+y;
            y=(int)p/10;
            k=p%10;
            c[j]='0'+k;
            j++;
        }
        if(z<0){
            for(;x>=0;x--){
                p=atoi(&b[x])+y;
                y=(int)p/10;
                k=p%10;
                c[j]='0'+k;
                j++;
            }
        }
        if(x<0){
            for(;z>=0;z--){
                p=atoi(&b[z])+y;
                y=(int)p/10;
                k=p%10;
                c[j]='0'+k;
                j++;
            }
            }
        for(j=j-1;j>=0;j--){
            printf("%c",c[j]);
        }
    } 
}
