#include <bits/stdc++.h>
using namespace std;

int main() {
	int a, b, x, y;
	cin >> a >> b >> x >> y;
	int ret = 0;
	int c = 0;
	if (2 * y > x) {
		while (a > 0 && b > 0) {
			a -= 2 * y;
			b -= 2 * y;
			ret++;
		}
		c = max(a, b);
	} else {
		while (a > 0) {
			a -= x;
			ret++;
		}
		c = b;
	}

	if (y > x) {
		while (c > 0) {
			c -= y;
			ret++;
		}
	} else {
		while (c > 0) {
			c -= x;
			ret++;
		}
	}
	cout << ret << endl;
}