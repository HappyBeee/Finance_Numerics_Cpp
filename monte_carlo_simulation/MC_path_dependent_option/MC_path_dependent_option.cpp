#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define PI 3.1415926535898
#define E 2.71828182846

using namespace std;

double normal(){
	double x[2] = {double(rand())/RAND_MAX, double(rand())/RAND_MAX};
	return sqrt(-2*log(x[0]))*cos(2*PI*x[1]);
}

double * sample_path(double r, double sigma, double S_0, double T, int N)
{
	double * path = new double [N+1];
	path[0] = S_0;
	for (int i=0; i<N; i++)
	{
		path[i+1] = path[i]*exp((r-0.5*sigma*sigma)*T/N+normal()*sigma*sqrt(T/N));
	}
	return path;
}

double float_lookback_call(double r, double sigma, double S_0, double T, int N, int M)
{
	double call_price = 0;
	double * path;
	for (int i=0; i<M; i++)
	{
		path = sample_path(r, sigma, S_0, T, N);
		call_price += 1.0/M*max(0.0, path[N] - *min_element(path, path+N));
		delete [] path;
	}
	return call_price*exp(-r*T);
}

double average_price_call(double r, double sigma, double S_0, double K, double T, int N, int M)
{
	double call_price = 0;
	double S_ave;
	double * path;
	for (int i=0; i<M; i++)
	{
		path = sample_path(r, sigma, S_0, T, N);
		S_ave = 0;
		for (int k=0; k<N+1; k++)
		{
			S_ave += 1.0/(N+1)*path[k];
		}
		call_price += 1.0/M*max(0.0, S_ave-K);
		delete [] path;
	}
	return call_price*exp(-r*T);
}