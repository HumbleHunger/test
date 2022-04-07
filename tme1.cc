#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param n int整型 
     * @param k int整型 
     * @return int整型
     */
		int ret = 0;
    void func(int n, int m, int count, int index) {
        if (count >= m || index >= n) {
					if (count == m) ret++;
					return;
				}

				for (int i = 0; i <= m - count; ++i) {
					func(n, m, count + i, index + 1);
				}
    }
    
    int numsOfStrings(int n, int k) {
				//if (n % 2 == 1) n--;
        int m = n - 2 * k;
				int count = 26;
				for (int i = 1; i < k; ++i) {
					count *= 25;
				}

				func(k, m, 0, 0);
				int tmp = 1;
                for (int i = 0; i < 6; ++i) {
                    tmp *= 10;
                }
				return (count * ret) % tmp;
    }
};