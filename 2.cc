#include <iostream>
#include <functional>//为了使用std::tr1::function
#include <string>
#include <sstream>
using namespace std;
class A {
public:
    void a(){cout << data << endl;data =1;cout << data << endl;}
private:
    int data =0;
};
int main()
{
    const A s = A();
    function<void()> f;
    f=bind(&A::a,s);
    f();
    std::cout << "Hello world" << std::endl;
    return 0;
}

