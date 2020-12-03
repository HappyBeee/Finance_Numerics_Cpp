using namespace std;

void printm(double ** matrix, int size)
{
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	for (int i=0; i<size; i++)
	{
		for (int j=0; j<size; j++)
		{
			cout << matrix[i][j] << "   ";
		}
		cout << endl;
	}
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

double ** matrix_inverse(double ** mat, int size)
{
	double ** matrix = new double * [size];
	double ** result = new double * [size];
	for (int i=0; i<size; i++)
	{
		matrix[i] = new double [size];
		result[i] = new double [size];
		for (int j=0; j<size; j++)
		{
			matrix[i][j] = mat[i][j];
			result[i][j] = 0;
		}
		result[i][i] = 1;
	}
	// printm(matrix, size);
	// cout << endl;
	// printm(result, size);
	
	double * temp_ptr;
	double factor;
	int non_zero_row;

	for (int i=0; i<size; i++)
	{
		// cout << "************************" << endl;
		// cout << "************************" << endl;
		// cout << "\nstep " << i << " :" << endl;
		non_zero_row = -1;
		for (int k=i; k<size; k++)
		{
			if (abs(matrix[k][i]) > 1.E-50)
			{
				non_zero_row = k;
				break;
			}
		}
		if (non_zero_row < 0)
		{
			return nullptr;
		}
		temp_ptr = matrix[i];
		matrix[i] = matrix[non_zero_row];
		matrix[non_zero_row] = temp_ptr;
		temp_ptr = result[i];
		result[i] = result[non_zero_row];
		result[non_zero_row] = temp_ptr;

		factor = matrix[i][i];
		for (int j=0; j<size; j++)
		{
			matrix[i][j] /= factor;
			result[i][j] /= factor;
		}

		// printm(matrix, size);
		// printm(result, size);
		// cout << "\n\n";

		for (int k=0; k<size; k++)
		{
			if (k==i || abs(matrix[k][i])<1.E-50)
			{
				continue;
			}
			factor = matrix[k][i];
			for (int j=0; j<size; j++)
			{
				matrix[k][j] -= matrix[i][j] * factor;
				result[k][j] -= result[i][j] * factor;
			}
		}
		// printm(matrix, size);
		// printm(result, size);
	}

	for  (int i=0; i<size; i++)
	{	
		delete [] matrix[i];
	}
	delete [] matrix;
	delete [] temp_ptr;

	return result;
}