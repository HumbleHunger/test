#include<iostream>
#include"String.h"
using namespace std;
class person
{
	private:
		string name;
		int age;
	public:
		void input(string a,int b)
		{
			name=a;
			age=b;
		}
		void display()
		{
			cout<<"姓名："<<name<<"\n"<<"年龄："<<age<<"\n"; 
		}
};
class student:public person
{
	private:
		string xue,zhuan;
	public:
		void input_s(string c,string d)
		{
			xue=c;
			zhuan=d;
		}
		void display_s()
		{
			cout<<"学号："<<xue<<"\n"<<"专业："<<zhuan<<"\n"; 
		}
};
int main()
{
	student x;
	string a,c,d;
	int b;
	cout<<"姓名：";
	cin>>a;
	cout<<"年龄：";
	cin>>b;
	cout<<"学号：";
	cin>>c;
	cout<<"专业：";
	cin>>d;
	x.input(a,b);
	x.input_s(c,d);
	x.display();
	x.display_s();
}
