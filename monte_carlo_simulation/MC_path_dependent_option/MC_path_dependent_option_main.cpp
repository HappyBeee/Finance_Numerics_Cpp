#include "MC_path_dependent_option.cpp"

using namespace std;

int main(){
	srand(time(nullptr));

	double r = 0.1;
	double sigma = 0.4;
	double S_0 = 50;
	double K = 50;
	double T = 1;
	int N = 150;
	int M =40000;

	double float_lookback_call_price = float_lookback_call(r, sigma, S_0, T, N, M);
	double average_price_call_price = average_price_call(r, sigma, S_0, K, T, N, M);
	cout << "Float lookback call option price: " << float_lookback_call_price << endl;
	cout << "Average price call option price: " << average_price_call_price << endl;
}