#include <bits/stdc++.h>
using namespace std;

int find(vector<int>& nums, int target) {
	int left = 0;
	int right = nums.size() - 1;
	int mid = 0;
	while (right > left) {
		mid = left + (right - left) / 2;

		if (nums[mid] >= target) {
			right = mid;
		} else if (nums[mid] < target) {
			left = mid + 1;
		}
	}

	return nums[left] == target ? left : -1;
}

int main() {
	
	vector<int> nums({0, 1, 2, 2, 3, 4, 4, 5, 5});

	cout << find(nums, 2) << endl;
}