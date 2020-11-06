#include "tree_asian_option.cpp"
#include <ctime>
#include <cstdlib>


using namespace std;

int main()
{
	Tree_asian_option tree_obj = Tree_asian_option(0.1, 0.4, 50, 50, 1, 60, 100);
	Node ** tree = tree_obj.get_tree();

	// for (int lvl=0; lvl<4; lvl++)
	// 	for (int j=0; j<lvl+1; j++)
	// 		for (int k=0; k<4; k++)
	// 			cout << "lvl: " << lvl << "  j: " << j << "  k: " << k << "  S_ave: " << tree[lvl][j].F_S[k][0] << endl;




	double call_price = tree_obj.get_call_price();

	cout << "r = 0.1, sigma = 0.4, S_0 = 50, K = 50," << endl;
	cout << "T = 1, steps = 60, points = 100 ." << endl;
	cout << "Asian average price call option: " << call_price << endl;
}