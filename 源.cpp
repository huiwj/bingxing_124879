#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<chrono>
using namespace std;
 long long int** b;
void init(int n)
{

	b = new long long int* [n];
	for (int i = 0; i < n; i++)
	{
		*(b + i) = new long long int[n];
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			b[i][j] = 2*i + j + 1;
}
int main()
{
	long long head, tail, freq;
	int n;
	cin >> n;
	int*a;
	a = new int[n];
	for (int i = 0; i < n; i++)
		a[i]=i+1;
	init(n);
	long long int* sum;
	sum = new long long int[n];
	auto beforeTime1 = std::chrono::steady_clock::now();
	for (int i = 0; i < n; i++)
	{
		sum[i] = 0;
		for (int j = 0; j < n; j++)
		{
			sum[i] += b[j][i] * a[j];
		}
	}
	auto afterTime1 = std::chrono::steady_clock::now();
	double duration_second1 = std::chrono::duration<double,std::nano>(afterTime1 - beforeTime1).count();
	cout << "耗时：" << duration_second1 <<"纳秒" << endl;
	/*
	cout << "结果为：" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << sum[i]<<" ";
	}
	cout << endl;
	*/
	auto beforeTime = std::chrono::steady_clock::now();
	for (int i = 0; i < n; i++)
		sum[i] = 0;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < n; i++)
			sum[i] += b[j][i] * a[j];
	auto afterTime = std::chrono::steady_clock::now();
	double duration_second = std::chrono::duration<double, std::nano>(afterTime - beforeTime).count();
	cout << "耗时：" << duration_second << "纳秒" << endl;
	
	/*
	cout << "结果为：" << endl;;
	for (int i = 0; i < n; i++)
	{
		cout << sum[i] << " ";
	}
	cout << endl;
	*/
	//求和
	int s = 0;
	auto beforeTime2 = std::chrono::steady_clock::now();
	for (int i = 0; i < n; i++)
		s += a[i];
	auto afterTime2 = std::chrono::steady_clock::now();
	double duration_second2 = std::chrono::duration<double, std::nano>(afterTime2 - beforeTime2).count();
	cout << "耗时：" << duration_second2 << "纳秒" << endl;
	cout << "结果：" << s << endl;

	auto beforeTime4 = std::chrono::steady_clock::now();
	long long int s1 = 0, s2 = 0;
	for (int i = 0; i < n; i = i + 2)
	{
		s1 += a[i];
		s2 += a[i + 1];
	}
	s1 = s1 + s2;
	auto afterTime4 = std::chrono::steady_clock::now();
	double duration_second4 = std::chrono::duration<double, std::nano>(afterTime4 - beforeTime4).count();
	cout << "耗时：" << duration_second4 << "纳秒" << endl;
	cout << "结果：" << s1 << endl;

	auto beforeTime3 = std::chrono::steady_clock::now();
	for (int m = n; m > 1; m /= 2)
		for (int i = 0; i < m / 2; i++)
			a[i] = a[i * 2] + a[i * 2 + 1];
	auto afterTime3 = std::chrono::steady_clock::now();
	double duration_second3 = std::chrono::duration<double, std::nano>(afterTime3 - beforeTime3).count();
	cout << "耗时：" << duration_second3 << "纳秒" << endl;
	cout << "结果：" << a[0] << endl;

	
	return 0;
}