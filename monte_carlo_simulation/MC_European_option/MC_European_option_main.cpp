#include "MC_European_option.cpp"

int main(){
	srand(time(nullptr));

	MC_European_option MC_obj = MC_European_option(0.05, 0.2, 90, 100, 1, 400, 40000);
	MC_obj.MC_simulation();
	std::cout << "r = 0.05, sigma = 0.2, S_0 = 90, K = 100, T = 1, N = 400, M = 40000 ." 
	<< "\n\n" << "European call option price: " << MC_obj.get_call_price() << endl 
	<< "European put option price: " << MC_obj.get_put_price() << endl; 

	// int size = 1000000;
	// double mean = 0;
	// double var = 0;
	// double rand_nums[size];
	// for (int k=0; k<size; k++)
	// {
	// 	rand_nums[k] = normal();
	// 	mean += 1.0/size*rand_nums[k];
	// }
	// for (int k=0; k<size; k++)
	// {
	// 	var += 1.0/size*(rand_nums[k]-mean)*(rand_nums[k]-mean);
	// }
	// std::cout << "mean: " << mean << " , var: " << var << endl;
}