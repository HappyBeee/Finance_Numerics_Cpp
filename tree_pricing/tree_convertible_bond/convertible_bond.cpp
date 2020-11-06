#include "convertible_bond.h"

using namespace std;

Tree_convertible_bond::Tree_convertible_bond(double r, double sigma, double S_0, double T, double default_rate, 
											 double conversion_ratio, double callback_price, 
											 double par_value, double recycle_ratio, int steps)
{
	this->r = r;
	this->sigma = sigma;
	this->S_0 = S_0;
	this->T = T;
	this->default_rate = default_rate;
	this->conversion_ratio = conversion_ratio;
	this->callback_price = callback_price;
	this->par_value = par_value;
	this->recycle_ratio = recycle_ratio;
	this->steps = steps;

	this->tree = nullptr;

	dt = T/steps;
	u = exp(sqrt((sigma*sigma-default_rate)*dt));
	d = 1/u;
	p_u = (exp(r*dt)-d*exp(-default_rate*dt))/(u-d);
	p_d = (u*exp(-default_rate*dt)-exp(r*dt))/(u-d);
	p_default = 1-p_u-p_d;
}

Node ** Tree_convertible_bond::get_tree()
{
	if (tree == nullptr) build_tree();
	return tree;
}

double Tree_convertible_bond::get_bond_price()
{
	calculate_bond_price();
	return tree[0][0].B;
}

void Tree_convertible_bond::build_tree()
{
	tree = new Node * [steps+1];
	for (int lvl=0; lvl<steps+1; lvl++)
	{
		tree[lvl] = new Node[lvl+1];
		for (int j=0; j<lvl+1; j++)
		{
			tree[lvl][j].S_t = S_0*pow(u,j)*pow(d,lvl-j);
			tree[lvl][j].B = nan("");
		}
	}
	return;
}

void Tree_convertible_bond::calculate_bond_price()
{
	// Boundary condition.
	for (int j=0; j<steps+1; j++)
	{
		tree[steps][j].B = max(par_value, tree[steps][j].S_t*conversion_ratio);
	}

	// Discount factor.
	double a = exp(-r*dt);

	// Iteratively calculate back to the root node.
	for (int lvl=steps-1; lvl>-1; lvl--)
	{
		for (int j=0; j<lvl+1; j++)
		{
			tree[lvl][j].B = p_u*a*tree[lvl+1][j+1].B + p_d*a*tree[lvl+1][j].B;
			tree[lvl][j].B += p_default*a*par_value*recycle_ratio;
			tree[lvl][j].B = min(tree[lvl][j].B, callback_price);
			tree[lvl][j].B = max(tree[lvl][j].B, conversion_ratio*tree[lvl][j].S_t);
		}
	}
	return;
}