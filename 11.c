/*************************************************************************
	> File Name: 11.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月15日 星期日 16时23分55秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
void main()
{
    char a[100];
    int i=0;
    int ch;
    while(scanf("%s",a)!=EOF){
        if(strcmp(a,"END")==0){ch=0;printf("\n\n");continue;}
        if(strcmp(a,"START")==0){ch=1;printf("\n");continue;}
        i=0;
        if(ch==1){
        while(a[i]!=0){
            if(a[i]>=65 && a[i]<=90){
                if(a[i]<=69){
                    printf("%c",21+a[i]);
                }
                else{
                    printf("%c",a[i]-5);
                }
            }
            else{
                printf("%c",a[i]);
            }
            i++;
        }
        for(i=0;i<100;i++){
            if(a[i]!=0){
                a[i]=0;
            }
        }printf(" ");
        }
    }
}
