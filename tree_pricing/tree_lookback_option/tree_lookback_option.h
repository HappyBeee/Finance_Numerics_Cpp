#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <map>

#define E 2.71828182846
#define PI 3.14159265359

struct Node
{
	double S;	
	// S_max or S_min prices with related option prices.
	// The key of the map is the times that the S_0 is decreased.
	std::map<int, double> max_vals;
	std::map<int, double> min_vals;
};


class Tree_lookback_option
{
public:
	Tree_lookback_option(double r, double sigma, double S_0, double T, int steps, double strike_price, bool american);
	
	Node ** get_tree();
	double get_call_price();
	double get_put_price();

protected:
	int steps;
	bool american;
	double r, sigma, S_0, T, strike_price;
	double dt, u, d, p;
	
	Node ** tree;

	void build_tree();
	void calculate_call_price();
	void calculate_put_price();
};
