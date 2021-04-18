#include <iostream>
#include "cylinder.h"
using namespace std;
double cylinder::a =3.14;
void cylinder::set()
{
    double r,h;
    cout << "请分别输入底园半径和高" << endl;
    cin >> r >> h;
    radius =r;
    height =h;
}
double cylinder::V()
{
    double s =a*radius*radius;
    return s*height;
}
void cylinder::print()
{
    cout << "底园半径为" << radius << endl;
    cout << "高为" << height << endl;
    cout << "体积为" << V() << endl;
}

