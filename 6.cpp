#include<iostream>
using namespace std;
class shape
{
	public:
		virtual float area(){
		}
};
class circle:public shape
{
	public:
		float a;
		circle(float b){
			a=b;
		}
		float area()
		{
			return 3.14*a*a;
		}
};
class triangle:public shape
{
	public:
		int b,c;
		triangle(int d,int f){
			b=d;
			c=f;
		}
		float area()
		{
			return b*c;
		}
};
int main()
{
	shape *p;
	circle b(2);
	triangle c(3,4);
	p=&b;
	cout<<"矩形的面积："<<p->area()<<endl;
	p=&c;
	cout<<"三角形的面积："<<p->area()<<endl;
}
