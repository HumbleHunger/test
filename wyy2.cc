#include <bits/stdc++.h>
using namespace std;

int main() {
	string str;
	cin >> str;
	int ret = 0;
	//vector<bool> used(str.size(), false);
	for (int i = 1; i < str.size() - 1; ++i) {
		if (abs(str[i] - str[i - 1]) <= 1 || abs(str[i] - str[i + 1]) <= 1) {
			int sum1 = 0, sum2 = 0;
			if (abs(str[i] - str[i - 1]) <= 1) {
				sum1 += str[i] - 'a' + 1;
				sum1 += str[i - 1] - 'a' + 1;
			}
			if (abs(str[i] - str[i + 1]) <= 1) {
				sum2 += str[i] - 'a' + 1;
				sum2 += str[i + 1] - 'a' + 1;
			}
			if (sum1 >= sum2) i++;
			else i += 2;
			ret += max(sum1, sum2);
			cout << ret << endl;
		}
	}
	cout << ret << endl;
}