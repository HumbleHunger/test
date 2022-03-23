#include <bits/stdc++.h>
using namespace std;

int ret = 0;
void func(string str, unordered_set<string>& set) {
	ret++;
	//cout << str << endl;
	if (str.size() < 2) return;

//	unordered_set<string> set;
	for (int i = 0; i < str.size() - 1; ++i) {
		if (i > 0 && str[i - 1] == str[i] && str[i] == str[i + 1]) continue;
		string tmp = str;

		if (str[i] == str[i + 1]) {
			str.erase(i, 1);
			if (set.count(str) == 0) {
				set.insert(str);
				func(str, set);
			}
		} else {
			if (str[i] == '0') {
				str.erase(i, 1);
			} else {
				str.erase(i + 1, 1);
			}
			if (set.count(str) == 0) {
				set.insert(str);
				func(str, set);
			}
		}

		str = tmp;
	}	
}

int main() {
	string str;
	cin >> str;
	unordered_set<string> set;
	func(str, set);
	cout << ret;
}