/*************************************************************************
	> File Name: test6.c
	> Author: 
	> Mail: 
	> Created Time: 2020年09月11日 星期五 23时17分54秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    const int a=0;
    const int &b=a;
    std::cout << "b is " << b << std::endl;
}
