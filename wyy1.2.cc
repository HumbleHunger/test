#include <bits/stdc++.h>
using namespace std;

int main() {
	int a, b, x, y;
	cin >> a >> b >> x >> y;
	int ret = 0;
	vector<vector<int>> dp(a, vector<int>(b, 1));

	int start = max(x, y);
	for (int i = start; i < a; ++i) {
		for (int j = start; j < b; ++j) {
			dp[i][j] = min({dp[i - x][j], dp[i][j - x], dp[i - y][j - y]}) + 1;
		}
	}
	cout << dp.back().back() << endl;
}