#include <iostream>
#include "matrix_inverse.cpp"

using namespace std;

double American_put_implicit(double r, double sigma, double S_0, double K, double T, int M, int N)
{
	double dS = 3*S_0/M;
	double dt = T/N;
	double * f1 = new double [M+1];
	double * f2 = new double [M+1];
	double * temp_ptr;
	double ** coeffs = new double * [M-1];
	double ** coeffs_inv;

	for (int i=0; i<M+1; i++)
	{
		f1[i] = max(0.0, K-i*dS);
	}

	for (int i=0; i<M-1; i++)
	{
		coeffs[i] = new double [M-1];
		for (int j=0; j<M-1; j++)
		{
			coeffs[i][j] = 0;
		}
	}

	for (int i=N-1; i>-1; i--)
	{
		temp_ptr = f2;
		f2 = f1;
		f1 = temp_ptr;
		coeffs[0][0] = 1+r*dt+sigma*sigma*dt;
		coeffs[0][1] = -0.5*r*dt-0.5*sigma*sigma*dt;
		coeffs[M-2][M-2] = 1+r*dt+sigma*sigma*(M-1)*(M-1)*dt;
		coeffs[M-2][M-3] = 0.5*r*(M-1)*dt-0.5*sigma*sigma*(M-1)*(M-1)*dt;
		for (int j=2; j<M-1; j++)
		{
			coeffs[j-1][j-2] = 0.5*r*j*dt-0.5*sigma*sigma*j*j*dt;
			coeffs[j-1][j-1] = 1+r*dt+sigma*sigma*j*j*dt;
			coeffs[j-1][j] = -0.5*r*j*dt-0.5*sigma*sigma*j*j*dt;
		}
		coeffs_inv = matrix_inverse(coeffs, M-1);

		f2[1] -= K*(0.5*r*dt-0.5*sigma*sigma*dt);
		for (int j=1; j<M; j++)
		{
			double value = 0;
			for (int k=0; k<M-1; k++)
			{
				value += coeffs_inv[j-1][k] * f2[k+1];
			}
			f1[j] = value;
		}
		f1[0] = K;
		f1[M] = 0;

		for (int j=0; j<M+1; j++)
		{
			f1[j] = max(f1[j], K-j*dS);
		}
	}

	int pos = S_0/dS;
	double put_price = f1[pos] + (f1[pos+1]-f1[pos])/dS*(S_0-dS*pos);

	return put_price;
}


int main(){
	double put_price = American_put_implicit(0.1, 0.4, 50, 50, 5/12.0, 300, 300);

	cout << "American put price: " << put_price << endl;

	// const int size = 4;
	// double data[size][size] = {0.19, 0.07, 0.1, 0.88, 0.01, 0.83, 0.45, 0.89, 0.36, 0.1, 0.29, 0.09, 0.8, 0.61, 0.51, 0.52}; 
	// double ** matrix = new double * [size];
	// for (int i=0; i<size; i++)
	// {
	// 	matrix[i] = data[i];
	// }
	// matrix[0][0] = 0;
	// matrix[0][2] = 0;
	// matrix[1][0] = 0;
	// matrix[1][3] = 0;
	// matrix[3][3] = 0;
	// matrix[3][0] = 0;

	// double ** matrix_inv;

	// matrix_inv = matrix_inverse(matrix, size);

	// printm(matrix);
	// printm(matrix_inv);
}