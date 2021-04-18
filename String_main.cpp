#include <iostream>
#include "String.h"
using namespace std;
int main()
{
    String a;
    a.set();
    cout << "长度为" << a.length() << endl;
    a.print();
    char ch[1024];
    cout << "请输入要添加的字符串";
    cin >> ch;
    a.add(ch);
    a.print();
    return 0;
}

