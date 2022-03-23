#include <bits/stdc++.h>
using namespace std;

int main() {
	string num;
	cin >> num;
	int size = num.size();
	vector<vector<vector<int>>> dp(size, vector<vector<int>>(size, vector<int>(2, 0)));
	//if (num[0] == '0') dp[0][0][0] = 1;
	//else dp[0][0][1] = 1;
	for (int i = 0; i < size; ++i) {
		for (int j = i; j < size; ++j) {
			if (i == j) {
				if (num[j] == '0') dp[i][j][0] = 1;
				else dp[i][j][1] = 1;
			cout << i << j << " " << dp[i][j][0] << " " << dp[i][j][1] << endl;
				continue;
			}

			if (num[j] == '0') {
				dp[i][j][0] = dp[i][j - 1][0] + 1;
				dp[i][j][1] = dp[i][j - 1][1];
			} else {
				dp[i][j][0] = dp[i][j - 1][0];
				dp[i][j][1] = dp[i][j - 1][1] + 1;
			}
			cout << i << j << " " << dp[i][j][0] << " " << dp[i][j][1] << endl;
		}
	}
	cout << " test " << endl;
	map<pair<int, int>, pair<int, int>> map;
	typedef pair<int, int> item;
	for (int i = size - 2; i > 0; --i) {
		map.clear();
		for (int l = 0, r = i; r < size; ++l, ++r) {
			if (map.count(item({dp[l][r][0], dp[l][r][1]})) == 0) {
				item key({dp[l][r][0], dp[l][r][1]});
				item value({l, r});
				map.insert({key, value});
			} else {
				item key({dp[l][r][0], dp[l][r][1]});
				cout << map[key].first + 1 << " " << map[key].second + 1 << " ";
				cout << l + 1 << " " << r + 1 << endl;
				return 0;
			}
		}
	}
}