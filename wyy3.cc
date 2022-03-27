#include <bits/stdc++.h>
using namespace std;

int main() {
	int n;
	cin >> n;
	int end = n / 2;
	vector<int> nums(n, 0);
	for (int i = 0, j = 2; i < end; ++i, j += 2) {
		nums[i] = j;
	}
	for (int i = end, j = 1; i < n; ++i, j += 2) {
		nums[i] = j;
	}
	for (auto n : nums) {
		cout << n << " ";
	}
}