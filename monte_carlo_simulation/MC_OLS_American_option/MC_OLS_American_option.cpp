#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <time.h>
#include <stdlib.h>

#define PI 3.141592653589793

using namespace std;

double normal()
{
	double x, y;
	x = (0.5+rand())/(double(RAND_MAX)+1);
	y = (0.5+rand())/(double(RAND_MAX)+1);
	
	// cout << "  |" << sqrt(-2*log(x))*cos(2*PI*y) << "|  ";
	return sqrt(-2*log(x))*cos(2*PI*y);
}

// default_random_engine generator;
// normal_distribution<double> normal(0.0, 1.0);

double * linear_fitting(double * X, double * Y, int size)
{
	double S[5] = {};
	double V[3] = {};

	for (int i=0; i<size; i++)
	{
		S[0] += 1;
		S[1] += X[i];
		S[2] += X[i]*X[i];
		S[3] += X[i]*X[i]*X[i];
		S[4] += X[i]*X[i]*X[i]*X[i];
		V[0] += Y[i];
		V[1] += Y[i]*X[i];
		V[2] += Y[i]*X[i]*X[i];
	}

	double M[3][3] = {};
	double gamma;
	gamma = (S[0]*S[4]-S[2]*S[2])*(S[0]*S[2]-S[1]*S[1]);
	gamma += (S[0]*S[3]-S[1]*S[2])*(S[1]*S[2]-S[0]*S[3]);

	M[0][1] = S[1]/(S[1]*S[1]-S[0]*S[2]);
	M[0][1] -= S[0]*(S[1]*S[3]-S[2]*S[2])*(S[0]*S[3]-S[1]*S[2])/gamma/(S[0]*S[2]-S[1]*S[1]);
	M[1][0] = M[0][1];
	
	M[1][1] = S[0]/(S[0]*S[2]-S[1]*S[1]);
	M[1][1] += S[0]*(S[1]*S[2]-S[0]*S[3])*(S[1]*S[2]-S[0]*S[3])/gamma/(S[0]*S[2]-S[1]*S[1]);
	
	M[0][2] = S[0]*(S[1]*S[3]-S[2]*S[2])/gamma;
	M[2][0] = M[0][2];
	
	M[1][2] = S[0]*(S[1]*S[2]-S[0]*S[3])/gamma;
	M[2][1] = M[1][2];
	
	M[2][2] = S[0]*(S[0]*S[2]-S[1]*S[1])/gamma;
	M[0][0] = 1/S[0]-M[1][0]*S[1]/S[0]-M[2][0]*S[2]/S[0];

	double coeff[3] = {};
	for (int i=0; i<3; i++){
		coeff[0] += M[0][i]*V[i];
		coeff[1] += M[1][i]*V[i];
		coeff[2] += M[2][i]*V[i];
	}

	double * Y_fitted = new double [size];
	for (int i=0; i<size; i++)
		Y_fitted[i] = coeff[0]+coeff[1]*X[i]+coeff[2]*X[i]*X[i];

	return Y_fitted;
}

double ** sample_paths(double r, double sigma, double S_0, double T, int M, int N)
{
	double ** paths = new double * [N];
	for (int i=0; i<N; i++)
	{
		paths[i] = new double [M+1];
		paths[i][0] = S_0;
		for (int j=0; j<M; j++)
			paths[i][j+1] = paths[i][j]*exp((r-0.5*sigma*sigma)*T/M+sqrt(T/M)*sigma*normal());
	}
	return paths;
}

double MC_Ame_put_price(double r, double sigma, double S_0, double K, double T, int M, int N)
{
	double ** paths_data;
	double * S = new double [N];
	double * option_prices = new double [N];
	double a = exp(-r*T/M);
	double ame_put_price = 0;

	paths_data = sample_paths(r, sigma, S_0, T, M, N);

	// for (int i=0; i<N; i++)
	// {
	// 	for (int j=0; j<M; j++)
	// 	{
	// 		cout << paths_data[i][j] << "  ";
	// 	}
	// 	cout << endl;
	// }

	for (int i=0; i<N; i++) 
	{
		option_prices[i] = max(0., K - paths_data[i][M]);
	}

	for (int i=M-1; i>0; i--)
	{
		// Make a current stock prices array.
		for (int j=0; j<N; j++)
		{
			S[j] = paths_data[j][i];
		}
		// Discount next time step's option prices to current time.
		for (int j=0; j<N; j++)
		{
			option_prices[j] *= a;
		}
		// Update option prices to linearly fitted result.
		option_prices = linear_fitting(S, option_prices, N);
		// Decide wheather to execute or not.
		for (int j=0; j<N; j++)
		{
			option_prices[j] = max(option_prices[j], K - S[j]);
		}
	}
	// Discount one more step back to t = 0.
	// Get the put option price in the mean time.
	for (int j=0; j<N; j++)
	{
		option_prices[j] *= a;
		ame_put_price += option_prices[j]/N;
	}

	return ame_put_price;
}

int main()
{
	srand(time(NULL));

	double Ame_put_price;

	Ame_put_price = MC_Ame_put_price(0.1, 0.4, 50, 60, 5.0/12, 200, 1000000);
	cout << "MC American put option price: " << Ame_put_price << endl;
}