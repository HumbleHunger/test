/*************************************************************************
	> File Name: test4.c
	> Author: 
	> Mail: 
	> Created Time: 2020年09月10日 星期四 21时48分51秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
    char a[10];
    memset(a,0,sizeof(a));
    while(1){
        int c,i=0;
        while((c=getchar())!='\n' && c!=EOF){
            if(c<48 || c>57 || i>sizeof(a)-2){
                printf("\t\t\t\t\t检测到非法输入!请重新输入\n");
                memset(a,0,sizeof(a));
                i=0;
                while((c=getchar())!='\n');
                break;
            }
            a[i]=c;
            i++;
        }
        if(i==0){
            continue;
        }
        else{
            break;
        }
    }
    printf("%s\n",a);
}
