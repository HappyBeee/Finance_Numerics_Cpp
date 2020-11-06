#include "tree_asian_option.h"

using namespace std;

Tree_asian_option::Tree_asian_option(double r, double sigma, double S_0, double K, double T,
									 int steps, int points=50)
{
	this->r = r;
	this->sigma = sigma;
	this->S_0 = S_0;
	this->K = K;
	this->T = T;
	this->steps = steps;
	this->points = points;

	this->dt = T/steps;
	this->u = exp(sigma*sqrt(dt));
	this->d = 1/u;
	this->p = (exp(r*dt)-d)/(u-d);

	this->tree = nullptr;
};

Node ** Tree_asian_option::get_tree()
{
	if (tree == nullptr) build_tree();
	return tree;
}

double Tree_asian_option::get_call_price()
{
	calculate_call_price();
	return tree[0][0].F_S[0][1];
}

double Tree_asian_option::interpolation(double x, double ** ref_list)
{
	// Expect ref_list is of (points x 2) size, with {ref_list[i][0]} being increasing array.
	int left = 0;
	int right = points-1;
	int pos;

	if (x < ref_list[0][0])
		return ref_list[0][1];
	if (x > ref_list[points-1][0])
		return ref_list[points-1][1];

	while (left < right)
	{
		pos = (left+right)/2;
		if (x == ref_list[pos][0]) 
			return ref_list[pos][1];
		if (x < ref_list[pos][0])
			right = pos-1;
		else
			left = pos+1;
	}
	if (x == ref_list[left][0])
		return ref_list[left][1];
	if (x < ref_list[left][0])
		pos = left;
	else
		pos = left+1;

	return ref_list[pos-1][1]+(ref_list[pos][1]-ref_list[pos-1][1])*(x-ref_list[pos-1][0])/
			(ref_list[pos][0]-ref_list[pos-1][0]);
}

double * Tree_asian_option::get_mean_range(int lvl, int j)
{	// Get max and min means of historical paths.
	double * result = new double[2];

	result[0] = S_0*(1-pow(u, j+1))/(1-u) + S_0*pow(u, j)*d*(pow(d, lvl-j)-1)/(d-1);
	result[0] /= lvl+1;
	result[1] = S_0*(1-pow(d, lvl-j+1))/(1-d) + S_0*pow(d, lvl-j)*u*(pow(u, j)-1)/(u-1);
	result[1] /= lvl+1;
	
	return result;
}

void Tree_asian_option::build_tree()
{
	tree = new Node * [steps+1];
	for (int lvl=0; lvl<steps+1; lvl++)
	{
		tree[lvl] = new Node[lvl+1];
		for (int j=0; j<lvl+1; j++)
		{
			tree[lvl][j].S = S_0*pow(u, j)*pow(d, lvl-j);
			double * max_min_means = get_mean_range(lvl, j);
			tree[lvl][j].F_S = new double * [points];
			for (int k=0; k<points; k++)
			{
				tree[lvl][j].F_S[k] = new double[2];
				tree[lvl][j].F_S[k][0] = (*max_min_means-*(max_min_means+1))/(points-1)*k + *(max_min_means+1);
				tree[lvl][j].F_S[k][1] = nan("");
			}
		}
	}
	return;
}


void Tree_asian_option::calculate_call_price()
{
	for (int j=0; j<steps+1; j++) 
	{
		for (int k=0; k<points; k++)
		{
			tree[steps][j].F_S[k][1] = max(0., tree[steps][j].F_S[k][0]-K);
		}
	}

	double a = exp(-r*dt);
	double x_u, x_d;
	for (int lvl=steps-1; lvl>-1; lvl--)
	{
		for (int j=0; j<lvl+1; j++)
		{
			for (int k=0; k<points; k++)
			{
				x_u = ((lvl+1)*tree[lvl][j].F_S[k][0]+tree[lvl+1][j+1].S)/(lvl+2);
				x_d = ((lvl+1)*tree[lvl][j].F_S[k][0]+tree[lvl+1][j].S)/(lvl+2);
				tree[lvl][j].F_S[k][1] = a*p*interpolation(x_u, tree[lvl+1][j+1].F_S);
				tree[lvl][j].F_S[k][1] += a*(1-p)*interpolation(x_d, tree[lvl+1][j].F_S);
			}
		}
	}
	return;
}
