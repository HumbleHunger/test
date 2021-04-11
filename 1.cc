#include<iostream>
class A
{
  int num;
public:
  A(int i){num=i;}
  A(A &a){num=a.num++;}
    void print(){std::cout<<num;}
};
int main()
{
  A a(1),b(a);
  a.print();
  b.print();
}
