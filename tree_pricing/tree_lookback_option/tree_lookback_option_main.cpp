#include <iostream>
#include "tree_lookback_option.h"
#include "tree_lookback_option.cpp"

using namespace std;

int main()
{
	Tree_lookback_option tree_euro = Tree_lookback_option(0.1, 0.4, 50, 0.25, 3, -1);
	Node ** tree = tree_euro.get_tree();
	double call_price = tree_euro.get_call_price();
	
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "r = 0.1, sigma = 0.4, S_0 = 50, T = 0.25, steps = 3 " << endl;
	cout << "European float call price: " << call_price << endl;

	double put_price = tree_euro.get_put_price();
	cout << "European float put price: " << put_price << endl;


	Tree_lookback_option tree_ame = Tree_lookback_option(0.1, 0.4, 50, 0.25, 3, -1, true);
	tree = tree_ame.get_tree();
	call_price = tree_ame.get_call_price();
	
	cout << "American float call price: " << call_price << endl;

	put_price = tree_ame.get_put_price();
	cout << "American float put price: " << put_price << endl;


	tree_euro = Tree_lookback_option(0.1, 0.4, 50, 0.25, 5, 49);
	tree = tree_euro.get_tree();
	call_price = tree_euro.get_call_price();
	
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "r = 0.1, sigma = 0.4, S_0 = 50, T = 0.25, steps = 5, strike_price = 49 " << endl;
	cout << "European fixed call price: " << call_price << endl;

	put_price = tree_euro.get_put_price();
	cout << "European fixed put price: " << put_price << endl;


	tree_ame = Tree_lookback_option(0.1, 0.4, 50, 0.25, 5, 49, true);
	tree = tree_ame.get_tree();
	call_price = tree_ame.get_call_price();
	
	cout << "American fixed call price: " << call_price << endl;

	put_price = tree_ame.get_put_price();
	cout << "American fixed put price: " << put_price << endl;
}