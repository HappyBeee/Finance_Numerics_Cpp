#include <iostream>
#include <cmath>

#define E 2.71828182846

struct Node
{
	double S_t;
	double B;
};

class Tree_convertible_bond
{
public:
	Tree_convertible_bond(double r, double sigma, double S_0, double T, double default_rate, double conversion_ratio, 
						  double callback_price, double par_value, double recycle_ratio, int steps);
	Node ** get_tree();
	double get_bond_price();
protected:
	double r, sigma, S_0, T, default_rate, par_value;
	double conversion_ratio, callback_price, recycle_ratio;
	int steps;

	double dt, u, d, p_u, p_d, p_default;
	Node ** tree;
	void build_tree();
	void calculate_bond_price();
};