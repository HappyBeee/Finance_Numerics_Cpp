#include <iostream>
#include <cmath>
#include "tree_vanilla_option.h"
// #ifndef E
// #include "tree_basics.h"
// #endif
using namespace std;

Tree_vanilla_option::Tree_vanilla_option(double r, double sigma, double S_0, double K, double T, int steps)
{		
		this->r = r;
		this->sigma = sigma;
		this->S_0 = S_0;
		this->K = K;
		this->T = T;
		this->steps = steps;

		this->tree = nullptr;
}

Node ** Tree_vanilla_option::get_tree()
{
	if (tree == nullptr) build_tree();
	
	return tree;
}

double Tree_vanilla_option::get_call_price()
{	
	if (tree==nullptr) build_tree();
	if (isnan(tree[0][0].node_call_price)) calculate_call_price();

	return tree[0][0].node_call_price;
}

double Tree_vanilla_option::get_put_price()
{
	if (tree==nullptr) build_tree();
	if (isnan(tree[0][0].node_put_price)) calculate_put_price();

	return tree[0][0].node_put_price;
}

void Tree_vanilla_option::build_tree()
{
	cout << "\nbuilding tree ... \n\n" ;
	delete tree;
	Node ** new_tree = new Node * [steps+1];
	tree = new_tree;

	double dt = T/steps;
	double up_amp = exp(sigma*sqrt(dt));
	double down_amp = 1/up_amp;
	double up_prob = (exp(r*dt)-down_amp)/(up_amp-down_amp);
	double down_prob = 1-up_prob;

	cout << "^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^\n";
	cout << "branching parameters: \n";
	cout << "  dt = " << dt << endl;
	cout << "  up_amp = " << up_amp << endl;
	cout << "  down_amp = " << down_amp << endl;
	cout << "  up_prob = " << up_prob << endl;
	cout << "  down_prob = " << down_prob << endl; 
	cout << "v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v\n" << endl;

	for (int lvl=0; lvl<steps+1; lvl++)
	{
		Node * nodes = new Node[lvl+1];
		tree[lvl] = nodes;
		for (int j=0; j<lvl+1; j++)
		{
			Node new_node;

			new_node.asset_price = S_0*pow(up_amp,j)*pow(down_amp,lvl-j);
			new_node.down_prob = down_prob;
			new_node.up_prob = up_prob;
			new_node.node_call_price = nan("");
			new_node.node_put_price = nan("");

			nodes[j] = new_node;	
		}
	}
}

void Tree_vanilla_option::calculate_call_price()
{
	if (tree==nullptr) build_tree();

	cout << "\ncalculating European call option price ...\n\n";
	
	double dt = T/steps;
	double discount_factor = exp(-r*dt);

	for (int j=0; j<steps+1; j++) tree[steps][j].node_call_price = max(0., tree[steps][j].asset_price-K);

	for (int lvl=steps-1; lvl>-1; lvl--)
	{
		for (int j=0; j<lvl+1; j++) 
		{
			tree[lvl][j].node_call_price = tree[lvl][j].down_prob * tree[lvl+1][j].node_call_price * discount_factor;
			tree[lvl][j].node_call_price += tree[lvl][j].up_prob * tree[lvl+1][j+1].node_call_price * discount_factor;
		}
	}
}

void Tree_vanilla_option::calculate_put_price()
{
	if (tree==nullptr) build_tree();
	
	cout << "\ncalculating European put option price ...\n\n";

	double dt = T/steps;
	double discount_factor = exp(-r*dt);

	for (int j=0; j<steps+1; j++) tree[steps][j].node_put_price = max(0., K-tree[steps][j].asset_price);

	for (int lvl=steps-1; lvl>-1; lvl--)
	{
		for (int j=0; j<lvl+1; j++)
		{
			tree[lvl][j].node_put_price = tree[lvl][j].down_prob * tree[lvl+1][j].node_put_price * discount_factor;
			tree[lvl][j].node_put_price += tree[lvl][j].up_prob * tree[lvl+1][j+1].node_put_price * discount_factor; 
		}
	}
}


void print_tree(Node ** root, int levels=5, bool priced=true)
{
	cout << fixed << setprecision(6);
	cout << "^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^-^\nprint tree: \n\n";
	for (int lvl=0; lvl<levels+1; lvl++)
	{
		cout << "level: " << lvl << endl << " || ";
		for (int j=0; j<lvl+1; j++)
		{
			cout << root[lvl][j].asset_price <<  " || ";
		}
		cout << endl;
		
		if (priced==true)
		{
			cout << " || ";
			for (int j=0; j<lvl+1; j++)
			{
				cout << root[lvl][j].node_call_price <<  " || ";
			}
			cout << endl;
			cout << " || ";
			for (int j=0; j<lvl+1; j++)
			{
				cout << root[lvl][j].node_put_price <<  " || ";
			}
			cout << endl;
		}
		cout << "----------------------------" << endl;
	}
	cout << "v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v-v\n";

	return;
}



// int main()
// {
// 	int steps;
// 	double r, sigma, S_0, K, T;

// 	steps = 50;
// 	r = 0.05, sigma = 0.2, S_0 = 10, K = 10, T = 3;
// 	Tree_vanilla_option Tree_vanilla_option = Tree_vanilla_option(steps, r, sigma, S_0, K, T);

// 	Node** tree = Tree_vanilla_option.get_tree();
// 	double call_option_price = Tree_vanilla_option.get_call_price();
// 	double put_option_price = Tree_vanilla_option.get_put_price();

// 	cout << "call option price: " << call_option_price << endl;
// 	cout << "put option price: " << put_option_price << endl << endl;

// 	cout << "put call parity:\n  c + Ke^{-rT} = p + S_0 :\n  ";
// 	cout << call_option_price << " + " << K*exp(-r*T) << " = " << put_option_price << " + " << S_0 << endl;
// 	cout << "  " << call_option_price + K*exp(-r*T) << " = " << put_option_price + S_0 << endl;

// 	print_tree(tree, 5, true);	
// }




