#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int get_file_length(char * file_name)
{
	double value = -10;
	int length = 0;
	char str[20];

	ifstream infile;
	infile.open(file_name);
	infile >> str >> str;
	while (!infile.eof())
	{
		infile >> str >> value;
		if (value!=-10)
		{
			length++;
		}
		value = -10;
	}
	infile.close();

	return length;
}

double * read_values(char * file_name, int length)
{
	double * values = new double [length];
	char str[20];

	ifstream infile;
	infile.open(file_name);
	infile >> str >> str;
	for (int i=0; i<length; i++)
	{
		infile >> str >> values[i];
	}
	infile.close();

	return values;
}

double * EWMA_predict(double * data, double lbd, int length)
{
	double * U = new double [length];
	double * Vars = new double [length+1];

	for (int i=1; i<length; i++)
	{
		U[i] = (data[i]-data[i-1])/data[i-1];
		U[i] = U[i]*U[i];
	}
	U[0] = 0;
	Vars[0] = 0;
	Vars[1] = 0;
	Vars[2] = U[1];
	for (int i=3; i<length+1; i++)
	{
		Vars[i] = Vars[i-1]*lbd+(1-lbd)*U[i-1];
	} 
	delete [] U;

	return Vars;
}

double EWMA_opt_lbd(double * data, int length, int M)
{
	double * U = new double [length];
	for (int i=1; i<length; i++)
	{
		U[i] = (data[i]-data[i-1])/data[i-1];
		U[i] = U[i]*U[i];
	}
	U[0] = 0;

	double opt_lbd = -1;
	double lbd = -1;
	double sigma2 = -1;
	double loss = 0;
	double min_loss = 1.E10;
	for (int i=1; i<M; i++)
	{
		lbd = double(i)/M;
		sigma2 = U[1];
		loss = 0;
		for (int j=2; j<length; j++)
		{
			loss += log(sigma2)+U[j]/sigma2;
			sigma2 = lbd*sigma2+(1-lbd)*U[j];
		}
		if (loss<min_loss)
		{
			min_loss = loss;
			opt_lbd = lbd;
		}
	}
	delete [] U;

	return opt_lbd;
}


int main()
{	
	char file_name[] = "EURUSDExchangerates.txt";
	int length = get_file_length(file_name);
	double * data = read_values(file_name, length);
	double opt_lbd = EWMA_opt_lbd(data, length, 10000);
	double * Vars = EWMA_predict(data, opt_lbd, length);
	cout << opt_lbd << endl;
}