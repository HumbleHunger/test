#include <iostream>
#include <cstring>
#include "String.h"
using namespace std;
void String::set()
{
    char *tmp =new char[1024];
    cout << "请输入字符串" << endl;
    cin >> tmp;
    content =tmp;
}
void String::print()
{
    cout << "当前的字符串是" << content << endl;
}
int String::length()
{
    return strlen(content);
}
void String::add(const char *ch)
{
    cout << "添加字符串" << ch << endl;
   int size =strlen(ch);
   int size1 =strlen(content);
   for(int i =0 ; i < size ; ++i){
       content[size1++] =ch[i];
   }
}
