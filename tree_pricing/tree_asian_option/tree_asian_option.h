#include <iostream>
#include <cmath>

#define E 2.71828182846

struct Node
{
	double S;

	// The array of pairs of stock average price and related option price. 
	double ** F_S;
};

class Tree_asian_option
{
public:
	Tree_asian_option(double r, double sigma, double S_0, double K, double T, int steps, int points);

	Node ** get_tree();
	double get_call_price();
	double interpolation(double x, double ** ref_list);

protected:
	double r, sigma, S_0, K, T;
	int steps, points;

	double dt, u, d, p;

	Node ** tree;
	void build_tree();
	void calculate_call_price();

	// Get linear interpolation value.
	// Get max and min averages of historical path stock prices. 
	double * get_mean_range(int lvl, int j);
};
