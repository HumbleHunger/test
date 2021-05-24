#include<iostream>
using namespace std;
class Rectangle
{
	protected :
		int length,width;
	public:

		void showsize()
		{
			cout<<length<<" "<<width<<"\n";
		}
};
class cube:public Rectangle
{
	private:
		int height;
    public:
    	void input(int a,int b,int c)
    	{
    		length=a;
			width=b;
			height=c;
		}
		void displaysize()
		{
			cout<<"长："<<length<<"\n"<<"宽："<<width<<"\n"<<"高："<<height<<"\n";
		}
		int volume()
		{
			cout<<"体积："<<length*width*height<<"\n";
		}
};
int main()
{
	cube x;
	int a,b,c;
	x.input(3,4,5);
	x.displaysize();
	x.volume();
}
