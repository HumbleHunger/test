#include <iostream>
using namespace std;
int main()
{
    int a=0;
    int &b=a;
    cout << &a << endl;
    cout << ++b << endl;
    cout << &b << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

