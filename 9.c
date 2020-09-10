#include <stdio.h>
void main()
{
    char a[600],b[600],c[600];
    int j=0;
    int z,x,y,k;
    while(scanf("%s %s",a,b)!=EOF){
        int i=0;
        int p=0;
        z=x=y=k=j=i=p=0;
        while(a[i]!=0){
            z=i;
            i++;
        }
        i=0;
        while(b[i]!=0){
            x=i;
            i++;
        }
        for(;z>=0 && x>=0;z--,x--){
            p=0;
            p=a[z]-48+b[x]-48+y;
            y=(int)p/10;
            k=p%10;
            c[j]=48+k;
            j++;
        }
        if(z<0 && x<0){
            if(y!=0){
                c[j]=y+48;
                j++;
            }
        }
        if(z<0 && x>=0){
            for(;x>=0;x--){
                p=0;
                p=b[x]-48+y;
                y=(int)p/10;
                k=p%10;
                c[j]=48+k;
                j++;
            }
        }
        if(x<0 && z>=0){
            for(;z>=0;z--){
                p=0;
                p=a[z]-48+y;
                y=(int)p/10;
                k=p%10;
                c[j]=48+k;
                j++;
            }
        }
        if(c[j-1]==48 && y!=0){
            c[j]=48+y;
            j++;
        }
        for(j=j-1;j>=0;j--){
            printf("%c",c[j]);
        }printf("\n");
    } 
}
