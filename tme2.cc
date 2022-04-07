#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param a int整型vector 
     * @param k int整型 
     * @param x int整型 
     * @return int整型
     */
    int minMax(vector<int>& a, int k, int x) {
        priority_queue<int> q(a.begin(), a.end());
				for (int i = 0; i < k; ++i) {
					int num = q.top();
					q.pop();
					int num1 = num - x;
					q.push(num1);
				}
				return q.top();
    }
};