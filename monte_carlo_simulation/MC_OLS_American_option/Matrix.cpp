#include <iostream>
using namespace std;

// Matrix is 2 dimensional class object, vector will be (n, 1) or (1, n) depends on initialization.

class Matrix
{
public:
	double ** matrix;
	int row, col;

	Matrix(double ** input_matrix, int dim1, int dim2);
	Matrix(double * input_vector, int dim1, int dim2);
	Matrix(Matrix &mat);
	
	Matrix T();
	
	double * flat_data();
	void print();
};

// Instance initiliazation.

Matrix::Matrix(double ** input_matrix, int dim1, int dim2)
{
	row = dim1;
	col = dim2;
	matrix = new double * [row];
	for (int i=0; i<row; i++){
		matrix[i] = new double [col];
		for (int j=0; j<col; j++)
			matrix[i][j] = input_matrix[i][j];
	}
}

Matrix::Matrix(double * input_vector, int dim1, int dim2)
{
	row = dim1;
	col = dim2;
	matrix = new double * [row];
	for (int i=0; i<row; i++){
		matrix[i] = new double [col];
	}
	for (int i=0; i<row; i++){
		for (int j=0; j<col; j++)
			matrix[i][j] = input_vector[i*col+j];
	}
}

Matrix::Matrix(Matrix &obj)
{
	Matrix new_obj;
	new_obj.matrix = new double [obj.row]
	new_obj.row = obj.row;
	new_obj.col = obj.col;
	for (int i=0; i<obj.row; i++)
	{
		new_obj.matrix[i] = new double [obj.col];
		for (int j=0; j<obj.col; j++)
		{
			new_obj.matrix[i][j] = obj.matrix[i][j];
		}
	}
	return new_obj;
}

void Matrix::print()
{
	cout << endl;
	for (int i=0; i<row; i++)
	{
		for (int j=0; j<col; j++)
			cout << matrix[i][j] << "  ";
		cout << endl;
	}
	return;
}

double * Matrix::flat_data()
{
	double * flat_data = new double [row*col];
	for (int i=0; i<row; i++)
		for (int j=0; j<col; j++)
			flat_data[i*col+j] = matrix[i][j];
	return flat_data;
}

Matrix Matrix::T()
{
	double flat_data[row*col];
	for (int i=0; i<row; i++)
		for (int j=0; j<col; j++)
			flat_data[j*row+i] = matrix[i][j];

	Matrix matrix_t = Matrix(flat_data, col, row);

	return matrix_t;
}



// Overload basic operators.
//

Matrix operator+(Matrix mat, double x)
{
	Matrix result = Matrix(&mat);
	for (int i=0; i<result.row; i++)
		for(int j=0; j<result.col; j++)
			result.matrix[i][j] += x;
	return result;
}

Matrix operator+(double x, Matrix mat)
{
	Matrix result = mat+x;
	return result;
}

Matrix operator-(Matrix mat, double x)
{
	Matrix result = Matrix(&mat);
	for (int i=0; i<result.row; i++)
		for (int j=0; j<result.col; j++)
			result.matrix[i][j] -= x;
	return result;
}

Matrix operator-(double x, Matrix mat)
{
	Matrix result = mat;
	for (int i=0; i<result.row; i++)
		for (int j=0; j<result.col; j++)
			result.matrix[i][j] = x-result.matrix[i][j];
	return result;
}


int main()
{
	double vec[6] = {1, 2, 3, 4, 5, 6};
	double * mat[2];
	double mat1[3] = {-10, -20, -30};
	double mat2[3] = {-40, -50, -60};
	mat[0] = mat1;
	mat[1] = mat2;

	Matrix M1 = Matrix(vec, 2, 3);
	Matrix M2 = Matrix(mat, 2, 3);
	M1.print();
	M1 = M1.T();
	M1.print();

	Matrix M3 = M1 + 100;
	M3.print();
	Matrix M4 = 100+M1;
	M4.print();
	Matrix M5 = 5-M1;
	M5.print();
}



