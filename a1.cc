#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;
 
int matrix[99][99] = {0};
//生成奇数幻方
void CreateOddMagicSquare(int n)
{
	int x=0,y,mun =1;
	y=n/2;
	while ( mun <= n*n )
	{
		matrix[x][y] = mun;
		
		//通过x0、y0检测右上的是否已经填入数字
		int x0=x;
		int y0=y;
		x0--;
		y0++;
		//超界处理
		if(x0<0)
			x0+=n;
		if(y0 == n)
			y0= n - y0;
		if(0 == matrix[x0][y0] )
		{
			x = x0;
			y = y0;
		}
		else
		{
		//若有数字填入之前数字的下方
			x++;
			if(x == n)
				x = x-n;
		}
		
		mun ++;
			
	}	
}
 
 
//生成双偶幻方
void CreateDoubleEvenMagicSqure( int n )
{
	int num = 1;
	//从1到n的平方依次赋值
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			matrix[i][j] = num++ ;
			
	//小正方形的对角线上的数字取其补数
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			if(i%4==0 && abs(i-j)%4 == 0)
				for(int k=0;k<4;k++)
					matrix[i+k][j+k] = abs( n*n +1 - matrix[i+k][j+k] );
			else if (i%4==3 && (i+j)%4 == 3)
				for(int k=0;k<4;k++)
					matrix[i-k][j+k] = abs( n*n +1 - matrix[i-k][j+k] );
				
		}
}
 
 
//生成单偶幻方
void CreateSingleEvenMagicSqure(int n)
{
	int k = n/2;
	CreateOddMagicSquare(k);
	
	//赋初值，左上最小，右下其次，右上再次，左下最大
	for(int i=0;i<k;i++)
		for(int j=0;j<k;j++)
		{
			matrix[i+k][j+k] = matrix[i][j] + k*k;
			matrix[i][j+k]   = matrix[i][j] + k*k*2;
			matrix[i+k][j]   = matrix[i][j] + k*k*3;
		}
		
		
	//公式 n=4m+2 
	int m = (n-2) / 4;
	
	//交换x方向正中行的从左至右m-1个
	for(int i=0;i<m-1;i++)
	{
		int buf = matrix[k/2][i];
		matrix[k/2][i] = matrix[k/2+k][i]; 
		matrix[k/2+k][i] = buf;
	}
	int buf = matrix[k/2][k/2];
	//以及正中间的数
	matrix[k/2][k/2] = matrix[k/2+k][k/2];
	matrix[k/2+k][k/2] = buf;
	
	//交换除x正中间行的其他行对应数字m个
	for(int i=0;i<k;i++)
		for(int j=0;j<k/2;j++)
		{
			if(i != k/2)
			{
				int buf = matrix[i][j];
				matrix[i][j] = matrix[i+k][j];
				matrix[i+k][j] = buf;
			}
		}
	
	//交换最右边m-1个数字
	for(int i=0;i<k;i++)
		for(int j=n-1;j>n-1-(m-1) ; j--)
		{
			int buf = matrix[i][j];
			matrix[i][j] = matrix[i+k][j];
			matrix[i+k][j] = buf;
		}
		
 
}
 
 
//幻方正确检查
bool Check(int n)
{
	int sum = (n*(n*n+1))/2;
	int SumA=0,SumB=0;
	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			SumA += matrix[i][j];
		if(SumA != sum)
			return false;
		SumA = 0; 
	}
	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			SumA += matrix[j][i];
		if(SumA != sum)
			return false;
		SumA = 0; 
	}
	
	for(int i=0;i<n;i++)
	{
		SumA+=matrix[i][i];
		SumB+=matrix[i][n-i-1];
	}
	if(SumA!=sum||SumB!=sum)
		return false;
	
	return true;
	
}
 
int main()
{
	int n;
	cin>>n;
	if(n%2!=0)
		CreateOddMagicSquare(n);
	else if (n%4 == 0)
		CreateDoubleEvenMagicSqure(n);
	else if (n%2 == 0)
		CreateSingleEvenMagicSqure(n);
	 
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			cout<<matrix[i][j]<<"\t";
		cout<<endl<<endl;
	}
	
	if(!Check(n))
		cout<<"the ans is wrong"<<endl;
	else 
		cout<<"right answer"<<endl;
		 
	return 0;
}
