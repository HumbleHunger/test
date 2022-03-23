#include <bits/stdc++.h>
using namespace std;

int main() {
	int n, k;
	cin >> n;
	cin >> k;
	vector<vector<int>> nums(n, vector<int>(n, 0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cin >> nums[i][j];
		}
	}

	for (int i = 0; i < n; ++i) {
		vector<int> tmp;
		for (int j = 0; j < n; ++j) {
			for (int l = 0; l < k; ++l) {
				cout << nums[i][j] << " ";
				tmp.push_back(nums[i][j]);
			}
		}
		cout << endl;
		for (int j = 1; j < k; ++j) {
			for (auto n : tmp) cout << n << " ";
			cout << endl;
		}
	}
}