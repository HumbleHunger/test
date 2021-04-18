#include <iostream>
#include "cylinder.h"
using namespace std;
int main()
{
    cylinder a;
    a.set();
    cout<< "体积为" << a.V() << endl;
    a.print();
    return 0;
}

