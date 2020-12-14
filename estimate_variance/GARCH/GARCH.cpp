#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

int get_file_length(char * file_name)
{
	int length = 0;
	char str[20];
	double value = -10;

	ifstream infile;
	infile.open(file_name);
	infile >> str >> str;
	while (!infile.eof())
	{
		infile >> str >> value;
		if (value != -10)
		{
			length += 1;
		}
		value = -10;
	}
	infile.close();
	return length;
}

double * read_values(char * file_name, int length)
{
	double * data = new double [length];
	char str[20];
	double value = -10;

	ifstream infile;
	infile.open(file_name);
	infile >> str >> str;
	for (int i=0; i<length; i++)
	{
		infile >> str >> data[i];
	}
	infile.close();
	return data;
}

double * GARCH_predict(double * data, int length, double omega, double alpha, double beta)
{
	double U2 = (data[1]-data[0])/data[0];
	U2 *= U2;

	double * variances = new double [length+1];
	variances[0] = 0;
	variances[1] = 0;
	variances[2] = U2;
	for (int i=3; i<length+1; i++)
	{
		U2 = (data[i-1]-data[i-2])/data[i-2];
		U2 *= U2;
		variances[i] = omega + alpha*U2 + beta*variances[i-1];
	}
	return variances;
}

double get_loss(double * U2, int length, double omega, double alpha, double beta)
{
	double loss = 0;
	double sigma2 = U2[1];

	for (int i=2; i<length; i++)
	{
		loss += log(sigma2)+U2[i]/sigma2;
		sigma2 = omega + alpha*U2[i] + beta*sigma2;
	}
	return loss;
}

double * GARCH_optimal_parameters(double * data, int length, int M1, int M2)
{
	double * U2 = new double [length];
	U2[0] = 0;
	double VL = 0;
	for (int i=1; i<length; i++)
	{
		U2[i] = (data[i]-data[i-1])/data[i-1];
		U2[i] *= U2[i];
		VL += U2[i]/(length-1);
	}

	double min_loss = 1.E10;
	double loss;
	double omega, alpha, beta;
	double opt1_omega, opt1_alpha, opt1_beta;
	for (int i=0; i<M1; i++)
	{
		alpha = 0.01 + 0.5/M1*i;
		for (int j=0; j<M1; j++)
		{
			beta = 0.5 + 0.5/M1*j;
			if (alpha+beta >= 1)
			{
				continue;
			}
			omega = VL * (1-alpha-beta);
			loss = get_loss(U2, length, omega, alpha, beta);
			if (loss < min_loss)
			{
				min_loss = loss;
				opt1_omega = omega;
				opt1_alpha = alpha;
				opt1_beta = beta;
			}
		}
	}
	cout << "Step 1 optimal omega, alpha, beta: " << opt1_omega << ", " << opt1_alpha << ", " << opt1_beta << endl;
	cout << "Total loss: " << min_loss << endl << endl;

	min_loss = 1.E10;
	double opt2_omega, opt2_alpha, opt2_beta;
	for (int i=0; i<M2; i++)
	{
		alpha = opt1_alpha - 0.025 + 0.05/M2*i;
		for (int j=0; j<M2; j++)
		{
			beta = opt1_beta - 0.025 + 0.05/M2*j;
			if (alpha+beta >= 1)
			{
				continue;
			} 
			for (int k=0; k<M2; k++)
			{
				omega = 0.5*opt1_omega + opt1_omega/M2*k;
				loss = get_loss(U2, length, omega, alpha, beta);
				if (loss < min_loss)
				{
					min_loss = loss;
					opt2_omega = omega;
					opt2_alpha = alpha;
					opt2_beta = beta;
				}
			}
		}
	}	

	cout << setprecision(8);
	cout << "Step 2/Final optimal omega, alpha, beta: " << opt2_omega << ", " << opt2_alpha << ", " << opt2_beta << endl;
	cout << "Total loss: " << min_loss << endl; 

	delete [] U2;
	double * parameters = new double [3];
	parameters[0] = opt2_omega;
	parameters[1] = opt2_alpha;
	parameters[2] = opt2_beta;

	return parameters;
}

int main()
{
	char file_name[] = "GARCHCALCSS&P500.txt";
	int length = get_file_length(file_name);
	double * data = read_values(file_name, length);
	double * parameters = GARCH_optimal_parameters(data, length, 200, 100);
	double * variances = GARCH_predict(data, length, parameters[0], parameters[1], parameters[2]);
}


