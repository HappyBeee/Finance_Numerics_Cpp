#include <iostream>
#include <cmath>
#include "tree_lookback_option.h"
// #ifndef E
// #include "tree_basics.h"
// #endif
using namespace std;

Tree_lookback_option::Tree_lookback_option(double r, double sigma, double S_0, double T, int steps, double strike_price, bool american=false)
{		
		this->r = r;
		this->sigma = sigma;
		this->S_0 = S_0;
		this->T = T;
		this->steps = steps;
		// If the input strike_price is negative, imply the option is float type, otherwise fixed.
		this->strike_price = strike_price;
		if (strike_price < 0) this->strike_price = nan("");
		this->american = american;

		this->tree = nullptr;
}

Node ** Tree_lookback_option::get_tree()
{
	if (tree == nullptr) build_tree();
	
	return tree;
}

double Tree_lookback_option::get_call_price()
{	
	calculate_call_price();
	// Return option price value, depend on if the option is float or fixed type.
	if (isnan(strike_price))
	{
		return tree[0][0].min_vals[0];
	}
	else
	{
		return tree[0][0].max_vals[0];
	}
}

double Tree_lookback_option::get_put_price()
{	
	calculate_put_price();
	
	if (isnan(strike_price))
	{
		return tree[0][0].max_vals[0];
	}
	else
	{
		return tree[0][0].min_vals[0];
	}
}

void Tree_lookback_option::build_tree()
{
	// cout << "\nbuilding tree ... \n\n" ;
	delete [] tree;
	tree = new Node * [steps+1];

	dt = T/steps;
	u = exp(sigma*sqrt(dt));
	d = 1/u;
	p = (exp(r*dt)-d)/(u-d);
	
	// cout << "^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^\n";
	// cout << "branching parameters: \n";
	// cout << "  dt = " << dt << endl;
	// cout << "  up_amp = " << u << endl;
	// cout << "  down_amp = " << d << endl;
	// cout << "  up_prob = " << p << endl;
	// cout << "  down_prob = " << 1-p << endl; 
	// cout << "v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v\n" << endl;

	for (int lvl=0; lvl<steps+1; lvl++)
	{
		tree[lvl] = new Node[lvl+1];

		for (int j=0; j<lvl+1; j++)
		{
			Node new_node;
			new_node.S = S_0*pow(u,j)*pow(d,lvl-j);

			int mins = lvl-j-max(0, lvl-2*j)+1;
			int maxs = j-max(0, 2*j-lvl)+1;
			
			for (int k=0; k < mins; k++) new_node.min_vals[lvl-j-k] = nan("");

			for (int k=0; k < maxs; k++) new_node.max_vals[-j+k] = nan("");
			
			tree[lvl][j] = new_node;	
		}
	}
}

void Tree_lookback_option::calculate_call_price()
{
	if (tree==nullptr) build_tree();
	
	// Discount factor for each step.
	double a = exp(-r*dt);

	// Boundary condition.
	for (int j=0; j<steps+1; j++)
	{
		Node * node = &tree[steps][j];
		// If the lookback option is float or fixed type.
		if (isnan(strike_price))
		{
			for (map<int, double>::iterator it = node->min_vals.begin(); it != node->min_vals.end(); it++)
			{
				it->second = max(0., node->S-S_0*pow(d, it->first));
				// cout << "lvl:" << steps << " j:" << j << " ds:" << it->first 
				// 	<< " S_min: " << S_0*pow(d, it->first) << " opt_val:" << it->second << endl;
			}
		}
		else
		{
			for (map<int, double>::iterator it = node->max_vals.begin(); it != node->max_vals.end(); it++)
			{
				it->second = max(0., S_0*pow(d, it->first)-strike_price);
				// cout << "lvl:" << steps << " j:" << j << " ds:" << it->first 
				// 	<< " S_min: " << S_0*pow(d, it->first) << " opt_val:" << it->second << endl;
			}
		}
	}

	// Iteratively calculate back to the root node.
	for (int lvl=steps-1; lvl>-1; lvl--)
	{
		for (int j=0; j<lvl+1; j++)
		{
			Node * node = &tree[lvl][j];
			if (isnan(strike_price))
			{
				for (map<int, double>::iterator it = node->min_vals.begin(); it != node->min_vals.end(); it++)
				{
					it->second = p*tree[lvl+1][j+1].min_vals[it->first]*a;
					it->second += (1-p)*tree[lvl+1][j].min_vals[max(it->first, lvl-2*j+1)]*a;
					if (american) it->second = max(it->second, node->S-S_0*pow(d, it->first));
					// cout << "lvl:" << lvl << " j:" << j << " ds:" << it->first 
					// << " S_min: " << S_0*pow(d, it->first) << " opt_val:" << it->second << endl;
				}
			}
			else
			{
				for (map<int, double>::iterator it = node->max_vals.begin(); it != node->max_vals.end(); it++)
				{
					it->second = (1-p)*tree[lvl+1][j].max_vals[it->first]*a;
					it->second += p*tree[lvl+1][j+1].max_vals[min(it->first, lvl-2*j-1)]*a;
					if (american) it->second = max(it->second, S_0*pow(d, it->first)-strike_price);
					// cout << "lvl:" << lvl << " j:" << j << " ds:" << it->first 
					// << " S_min: " << S_0*pow(d, it->first) << " opt_val:" << it->second << endl;
				}
			}
		}
	}
}

void Tree_lookback_option::calculate_put_price()
{
	if (tree==nullptr) build_tree();
	
	double a = exp(-r*dt);

	// Boundary condition.
	for (int j=0; j<steps+1; j++)
	{
		Node * node = &tree[steps][j];
		if (isnan(strike_price))
		{
			for (map<int, double>::iterator it = node->max_vals.begin(); it != node->max_vals.end(); it++)
			{
				it->second = max(0., S_0*pow(d, it->first)-node->S);
				// cout << "lvl:" << steps << " j:" << j << " ds:" << it->first 
				// 	<< " S_min: " << S_0*pow(d, it->first) << " opt_val:" << it->second << endl;
			}
		}
		else
		{
			for (map<int, double>::iterator it = node->min_vals.begin(); it != node->min_vals.end(); it++)
			{
				it->second = max(0., strike_price-S_0*pow(d, it->first));
			}
		}
	}
	for (int lvl = steps-1; lvl > -1; lvl--)
	{
		for (int j=0; j < lvl+1; j++)
		{
			Node *node = &tree[lvl][j];
			if (isnan(strike_price))
			{
				for (map<int, double>::iterator it = node->max_vals.begin(); it != node->max_vals.end(); it++)
				{
					it->second = p*a*tree[lvl+1][j+1].max_vals[min(it->first,lvl-2*j-1)];
					it->second += (1-p)*a*tree[lvl+1][j].max_vals[it->first];
					if (american) it->second = max(it->second, S_0*pow(d, it->first)-node->S);
					// cout << "lvl:" << lvl << " j:" << j << " ds:" << it->first 
					// << " S_min: " << S_0*pow(d, it->first) << " opt_val:" << it->second << endl;
				}
			}
			else
			{
				for (map<int, double>::iterator it = node->min_vals.begin(); it != node->min_vals.end(); it++)
				{
					it->second = p*a*tree[lvl+1][j+1].min_vals[it->first];
					it->second += (1-p)*a*tree[lvl+1][j].min_vals[max(it->first, lvl-2*j+1)];
					if (american) it->second = max(it->second, strike_price-S_0*pow(d, it->first));
				}	
			}
		}
	}
}






// int main()
// {
// 	int depth;
// 	double r, sigma, S_0, K, T;

// 	depth = 50;
// 	r = 0.05, sigma = 0.2, S_0 = 10, K = 10, T = 3;
// 	Tree_binomial Tree_binomial = Tree_binomial(depth, r, sigma, S_0, K, T);

// 	Node_binary** tree = Tree_binomial.get_tree();
// 	double call_option_price = Tree_binomial.get_call_price();
// 	double put_option_price = Tree_binomial.get_put_price();

// 	cout << "call option price: " << call_option_price << endl;
// 	cout << "put option price: " << put_option_price << endl << endl;

// 	cout << "put call parity:\n  c + Ke^{-rT} = p + S_0 :\n  ";
// 	cout << call_option_price << " + " << K*exp(-r*T) << " = " << put_option_price << " + " << S_0 << endl;
// 	cout << "  " << call_option_price + K*exp(-r*T) << " = " << put_option_price + S_0 << endl;

// 	print_tree(tree, 5, true);	
// }




