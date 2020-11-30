#include <iostream>

using namespace std;

double explicit_American_put(double r, double sigma, double S_0, double K, double T, int M, int N)
{
	double dS = S_0*3/M;
	double dt = T/N;

	double * f1 = new double [M+1];
	double * f2 = new double [M+1];
	double * temp;
	
	for (int i=0; i<M+1; i++)
	{
		f1[i] = max(0.0, K-i*dS);
	}
	
	for (int i=N-1; i>-1; i--)
	{
		temp = f2;
		f2 = f1;
		f1 = temp;
		f1[0] = K;
		f1[M] = 0;
		for (int j=1; j<M; j++)
		{
			f1[j] = f2[j-1]*(-0.5*r*j*dt+0.5*sigma*sigma*j*j*dt)/(1+r*dt);
			f1[j] += f2[j]*(1-sigma*sigma*j*j*dt)/(1+r*dt);
			f1[j] += f2[j+1]*(0.5*r*j*dt+0.5*sigma*sigma*j*j*dt)/(1+r*dt);
			f1[j] = max(f1[j], K-j*dS);
		}
	}
	
	int pos = S_0/dS;
	double result = f1[pos] + (f1[pos+1]-f1[pos])/dS*(S_0-pos*dS);

	return result;
}

int main()
{
	double put_price = explicit_American_put(0.1, 0.4, 50, 60, 1, 150, 50000);
	cout << "American put price: " << put_price << endl;
}