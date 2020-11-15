#include <iostream>
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
	
	return sqrt(-2*log(x))*cos(2*PI*y);
}


int main(){

	// **** test 1
	// double ** p = new double * [2];
	// double ** q; 
	// double arr[2][3] = {{1, 2, 3}, {2, 3, 4}};
	// p[0] = arr[0];
	// p[1] = arr[1];
	// q = p;
	// cout << q[0][1] << " " << q[1][1] << endl;

	// **** test 2
	// double ticks[10];
	// clock_t t = clock();
	// srand(clock());
	// for (int i=0; i<10; i++)
	// {
	// 	rand();
	// 	ticks[i] = clock()-t;
	// }
	// for (int i=0; i<10; i++) cout << ticks[i] << " ";

	// **** test 3
	// double ** mat;
	// double vec[5] = {1,2,3};
	// int M = 4;
	// int N = 5;
	// mat = new double * [M];
	// for (int i=0; i<M; i++) mat[i] = new double [N];

	// cout << end(mat[0]) << " " << sizeof(mat[0][0]) << endl;

	// **** test 4
	// double vec[] = {1,2,3};
	// double mat[2][2] = {-1, -2, -3, -4};
	// double * p0[2];
	// p0[0] = mat[0];
	// p0[1] = mat[1];
	
	// double * p1;
	// double ** p2;
	// p1 = test_array(vec);
	// p2 = test_array(p0);
	// cout << p1[0] << " " << p2[0][0] << endl;

	// **** test 5
	// cout << normal() << " " << normal() << endl;
	// cout << "RAND_MAX: " << RAND_MAX << endl;

	// int n1 = rand();
	// cout << n1 << " " << rand() << endl;
	// srand(n1);
	// cout << rand() << endl; 

}