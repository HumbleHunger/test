#include <bits/stdc++.h>
using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	vector<vector<int>> nums(n, vector<int>(m, 0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			cin >> nums[i][j];
		}
	}

	vector<vector<int>> dp(n, vector<int>(m, 0));
	for (int i = 1; i < n; ++i) {
		dp[i][0] = dp[i - 1][0];
		if (nums[i][0] != nums[i - 1][0]) dp[i][0] += 2;
		else dp[i][0]++;
	}
	for (int j = 1; j < m; ++j) {
		dp[0][j] = dp[0][j - 1];
		if (nums[0][j] != nums[0][j - 1]) dp[0][j] += 2;
		else dp[0][j]++;
	}
	for (int i = 1; i < n; ++i) {
		for (int j = 1; j < m; ++j) {
			dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]);

			if (dp[i - 1][j] < dp[i][j - 1]) {
				if (nums[i][j] != nums[i - 1][j]) dp[i][j] += 2;
				else dp[i][j]++;
			} else if (dp[i - 1][j] > dp[i][j - 1]) {
				if (nums[i][j] != nums[i][j - 1]) dp[i][j] += 2;
				else dp[i][j]++;
			} else {
				if (nums[i][j] == nums[i - 1][j] || nums[i][j] == nums[i][j - 1]) dp[i][j]++;
				else dp[i][j] += 2;
			}
			cout << dp[i][j] << " ";
		}
		cout << endl;
	}
	for (auto n : dp) {
		for (auto m : n) {
			cout << m << " ";	
		}
		cout << endl;
	}
	cout << dp.back().back() << endl;
}