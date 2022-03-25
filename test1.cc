#include <iostream>
using namespace std;
int main()
{
    int a = 0;
    int *p = nullptr;
    int &b = *p;
    cout << b << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

