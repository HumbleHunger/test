#include<iostream>
using namespace std;
template <typename T>
T f(T a,T b)
{
	return (a<b)?a:b;
}
int main()
{  
	cout<<"3和4中的最小值："<<f(3,4)<<endl;
	cout<<"4.56和4.12中的最小值："<<f(5.56,4.12)<<endl;
	cout<<"'d'和'y'中的最小值："<<f('d','y')<<endl;
}
