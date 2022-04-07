#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param s string字符串 
     * @return int整型
     */
    int minCnt(string s) {
				int ret = 0;
        for (int i = 1; i < s.size(); ++i) {
					if (s[i] == '1') ret++;
				}
				return ret;
    }
};