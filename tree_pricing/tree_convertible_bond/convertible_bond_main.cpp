#include "convertible_bond.cpp"

using namespace std;

int main()
{
	cout << "r: 0.05,  sigma: 0.3,  S_0: 50,  T: 0.75,  lambda: 0.01,  conversion_ratio: 2," << endl;
	cout << "callback_price: 113,  par_value: 100,  recycle_ratio: 0.4,  steps: 10 ." << endl << endl;

	Tree_convertible_bond tree_obj = Tree_convertible_bond(0.05, 0.3, 50, 0.75, 0.01, 2, 113, 100, 0.4, 10);

	Node ** tree = tree_obj.get_tree();

	double bond_price = tree_obj.get_bond_price();

	cout << "Convertible bond price: " << bond_price << endl;
}