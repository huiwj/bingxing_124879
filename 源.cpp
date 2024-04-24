
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include <algorithm>  
#include <random> 
#include <numeric>
#include<chrono>
using namespace std;

void initial_matrix(int n,std::vector<std::vector<float>>& a)
{
	// ��������Ǿ���
	srand((unsigned)time(0));
	for(int i=0;i<n;i++)
		for (int j=i;j<n;j++)
		{
			a[i][j] = float(1+rand()%50);
		}
	// ����������Ԫ���Ա��־���Գ���  
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			a[i][j] = a[j][i];
		}
	}
	// ���������  
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::vector<int> indices(n);
	std::iota(indices.begin(), indices.end(), 0); // �����������  
	std::shuffle(indices.begin(), indices.end(), generator); // ��������  

	// ʹ�ô��Һ������������  
	std::vector<std::vector<float>> temp(n, std::vector<float>(n)); // ��ʱ����  
	for (int i = 0; i < n; ++i) {
		temp[i] = a[indices[i]];
	}
	a = temp; // ����ʱ����ֵ��ԭ����  
	return;
}

void initial_VECT(int n, std::vector<float>& b)
{
	srand((unsigned)time(0));
	for (int i = 0; i < n; i++)
	{
		b[i] = float(2 + rand()%50);
	}
}

void Gaussian_Elimination(int n, std::vector<std::vector<float>>& a, std::vector<float>& b, std::vector<float>& x)
{
	//��ȥ
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			float factor = a[j][i] / a[i][i];
			for (int h = i ; h < n; h++)
			{
				a[j][h] = a[j][h] - factor * a[i][h];
			}
			b[j] = b[j] - factor * b[i];
		}
	}
	//�ش�
	x[n-1] = b[n-1] / a[n-1][n-1];
	for (int i = n - 2; i >= 0; i--)
	{
		float sum = b[i];
		for (int j = i + 1; j < n; j++)
		{
			sum = sum - a[i][j] * x[j];
		}
		x[i] = sum / a[i][i];
	}
	
}
int main()
{
	int N = 20;
		for (int k = 0; k < 3; k++)
		{
			std::vector<std::vector<float>> a(N, vector<float>(N));
			std::vector<float> b(N);
			std::vector<float> x(N);
			initial_matrix(N, a);
			initial_VECT(N, b);
			auto start = std::chrono::high_resolution_clock::now();
			Gaussian_Elimination(N, a, b, x);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			std::cout << "Execution time: " << diff.count() << "nanoseconds." << std::endl;
			for (int i = 0; i < N; i++)
			{
				cout << x[i] << " ";
			}
		}
	return 0;
}
