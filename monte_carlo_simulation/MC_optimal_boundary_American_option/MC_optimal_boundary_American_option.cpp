#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

bool compare(double * x, double * y)
{
	return (x[0] < y[0]);
}

double normal()
{
	double x, y;
	x = (0.5+double(rand()))/(double(RAND_MAX)+1);
	y = (0.5+double(rand()))/(double(RAND_MAX)+1);
	return sqrt(-2*log(x))*cos(2*3.14159265359*y);
}

double ** sample_paths(double r, double sigma, double S_0, double T, int M, int N)
{
	double ** paths = new double * [N];
	for (int i=0; i<N; i++)
	{
		paths[i] = new double [M+1];
		paths[i][0] = S_0;
		for (int j=0; j<M; j++){
			paths[i][j+1] = paths[i][j]*exp((r-0.5*sigma*sigma)*T/M + sigma*normal()*sqrt(T/M));
		}
	}
	return paths;
}

double optimal_boundary_eval(double * S, double * P, int N, double K)
{
	double cutoff = 0;
	double ** pairs = new double * [N];
	for (int i=0; i<N; i++)
	{
		pairs[i] = new double [2];
		pairs[i][0] = S[i];
		pairs[i][1] = P[i];
	}
	sort(pairs, pairs+N, compare);

	double total = 0;
	double total_max = 0;
	for (int i=N-1; i>-1; i--)
	{
		total += pairs[i][1];
		total -= max(0.0, K-pairs[i][0]);
		if (total > total_max)
		{
			total_max = total;
			cutoff = pairs[i][0];
		}
	}
	for (int i=0; i<N; i++)
	{
		if (S[i] < cutoff)
		{
			P[i] = max(0.0, K-S[i]);
		}
		delete [] pairs[i];
	}
	delete [] pairs;

	return cutoff;
}

double MC_optimal_boundary_American_put(double r, double sigma, double S_0, double K, double T, int M, int N)
{
	double put_price = 0;
	double ** paths = sample_paths(r, sigma, S_0, T, M, N);
	double * cutoffs = new double [M+1];
	cutoffs[M] = K;
	double * stock_prices = new double [N];
	double * put_prices = new double [N];
	for (int i=0; i<N; i++)
	{
		stock_prices[i] = paths[i][M];
		put_prices[i] = max(0.0, K-stock_prices[i]);
	}
	for (int i=M-1; i>-1; i--)
	{
		for (int j=0; j<N; j++)
		{
			stock_prices[j] = paths[j][i];
			put_prices[j] = put_prices[j] * exp(-r*T/M);
		}
		cutoffs[i] = optimal_boundary_eval(stock_prices, put_prices, N, K);
	}
	for (int i=0; i<N; i++)
	{
		put_price += put_prices[i]/N;
		delete [] paths[i];
	}
	delete [] paths;
	delete [] stock_prices;
	delete [] put_prices;

	cout << "Execution boundary: \n";
	for (int i=0; i<M+1; i++)
	{
		cout << cutoffs[i] << ",  ";
	}
	cout << "\n\n";

	return put_price;
}

int main()
{
	srand(time(NULL));
	double MC_American_put = MC_optimal_boundary_American_put(0.1, 0.4, 50, 60, 5.0/12, 100, 300000);
	cout << "MC American put price: " << MC_American_put << endl;
}
