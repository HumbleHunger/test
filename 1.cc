#include<iostream>
#include<vector>
using namespace std;
int main()
{
    for (int i = 0; i < 5; ++i) {
        vector<int> v;
        v.push_back(1);
        cout << v.size() << endl;
    }
}
