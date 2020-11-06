#include <iostream>
#include "tree_vanilla_option.h"
#include "tree_vanilla_option.cpp"

using namespace std;

int main()
{
	int steps;
	double r, sigma, S_0, K, T;

	steps = 50;
	r = 0.05, sigma = 0.2, S_0 = 10, K = 10, T = 3;
	Tree_vanilla_option basic_tree = Tree_vanilla_option(r, sigma, S_0, K, T, steps);

	Node ** tree = basic_tree.get_tree();
	double call_option_price = basic_tree.get_call_price();
	double put_option_price = basic_tree.get_put_price();

	cout << "call option price: " << call_option_price << endl;
	cout << "put option price: " << put_option_price << endl << endl;

	cout << "put call parity:\n  c + Ke^{-rT} = p + S_0 :\n  ";
	cout << call_option_price << " + " << K*exp(-r*T) << " = " << put_option_price << " + " << S_0 << endl;
	cout << "  " << call_option_price + K*exp(-r*T) << " = " << put_option_price + S_0 << endl;

	print_tree(tree, 5);	
}