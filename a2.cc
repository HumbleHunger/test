#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<cstring>
using namespace std;
int a[10][10];
int x, y;
int _random()//随机产生1~3范围中的一个数，代表走的方向
{
	return rand() % 4;
}
void process(int e)//e代表走的方向
{
	if (e == 1)
	{
		if ((x & 1)==0 && (x >= 2 && y >= 2))//若当前点是该行第奇数个点三角形往下走，第偶数个点三角形往上走
			x--, y--;
		else								//还要判断是否越界
			if (x <= 7 && y <= 7)
				x++, y++;
	}
	if (e == 2)//往左走
		if (x >= 2)//还要保证不越界
			x--;
	if (e == 3)//往右走
		if (x <= 7)//还要保证不越界
			x++;
}
bool judge()//判断所有三角形是否都已经走过
{
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= i; j++)
			if (a[i][j] == 0) return 0;
	return 1;
}
void print()
{
	for (int i = 1; i <= 8; i++)
	{
		for (int j = 1; j <= i; j++)
			cout << a[i][j] << ' ';
		cout << endl;
	}
}
int main()
{
	srand(time(0));
	int T = 2;
	long long ans = 0;
	for(int i=1;i<=T;i++)
	{
		cout << "第" << i << "次：\n";
		int cnt = 0;
		x = 1; y = 1;
		memset(a, 0, sizeof(a));
		while (1)
		{
			process(_random());
			a[x][y]++;
			for (int i = 1; i <= 8; i++)
			{
				for (int j = 1; j <= i; j++)
					cout << a[i][j] << ' ';
				cout << endl;
			}
			if (judge())//判断所有三角形是否都已经走过
			{
				print();
				break;
			}
			else cnt++;
		}
		cout <<"总步数：" <<cnt << endl << endl;
		ans += cnt;
	}
	cout <<"平均次数为："<< fixed << double(ans) / 100 << endl;
	return 0;
}
