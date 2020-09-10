/*************************************************************************
	> File Name: 12.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月17日 星期二 18时34分02秒
 ************************************************************************/

#include<stdio.h>
void main()
{
    char a[1005];
    int b;
    while(scanf("%s",a)!=EOF){
        int i=0;
        b=0;
        if(a[0]=='0'){
            continue;
        }
        while(1){
            i=0;
            b=0;
            while(a[i]!=0){
                b=b+a[i]-48;
                i++;
            }
            if(b%10==b){
                printf("%d\n",b);
                break;
            }
            else{
                for(i=0;i<1005;i++){
                    a[i]=0;
                }
                sprintf(a,"%d",b);
            }
        }
    }
}

