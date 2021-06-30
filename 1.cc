#include<iostream>
#include<vector>
using namespace std;
int main()
{
    vector<int> s;
    s.reserve(5);
    cout << s.size() << endl;
    cout << s.capacity() << endl;
    s.resize(10);
    cout << s.size() << endl;
    cout << s.capacity() << endl;

}
