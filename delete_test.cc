#include <bits/stdc++.h>
using	namespace std;

class test {
public:
	static int i;
	test() {
		cout << "test" << i++ << endl;
	}
	~test() {
		cout << "~test" << val << endl;
	}
	int val;
};
int test::i = 0;
int main() {
	test* ptr = new test[5];
	test* tmp = ptr;
	for (int i = 0; i < 5; ++i) {
		ptr[i].val = i - 1;
	}
	delete[] (ptr);
	cout << "over" << endl; 
}