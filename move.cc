#include <iostream>
#include <vector>
using namespace std;

class test
{
    public:
    test():i(1){
        cout << "test()" << endl;
        cout << this << endl;
    }

    test(const test& t):i(1)
    {
        cout << "test(const test&)" << endl;
        cout << this << endl;
    }

    test operator=(test& r)
    {
        cout << "test operator=(test&)" << endl;
    }

    ~test()
    {
        cout << "~test()   and   i  =  " << i  << endl;
    }

    int i;
};

vector<test> v;
void func()
{
    test t1;
    cout << t1.i << endl;
    v.push_back(std::move(t1));
    t1.i = 9;
    cout << v.begin()->i << endl;
}

int main()
{
    func();
    cout << v.begin()->i << endl;
    cout << &(v.begin()->i) << endl;
    return 0;
}
