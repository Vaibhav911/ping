#include "stats.hpp"

void calc_stats(std::vector<double> &arr, std::string &host_name)
{
	//Calculate and print all the necessary statistics 
	//regarding packets sent and received. 

	std::sort(arr.begin(), arr.end());

	double mean = 0;

	double min_val = DBL_MAX;
	
	//Since array is sorted, max_val is last element.
	double max_val = arr.back();

	//Number of success and failed packets.
	int succ = 0;
	int fail = 0;

	for (auto elem: arr)
	{
		if (elem < -0.5)
		{
			//If elem is less than zero, it's failure
			//But to be sure, -0.5 is used.
			fail++;
		}
		else
		{
			succ++;
			mean += elem;
			min_val = std::min(min_val, elem);
		}
	}

	mean /= succ;

	std::cout << "=============" << "Statistics: " << host_name << "===============" << std::endl;
	std::cout << "Packets Sent: " << arr.size() << " | Received: " << succ 
		<< " | Failed: " << fail << ", " << ((double)fail / (fail + succ)) * 100 << "%" << std::endl;
	std::cout << "-----------------------------------------------------------------" << std::endl;
	std::cout << "min | max | mean || "
		<< min_val << " | " << max_val << " | " << mean << " ms" << std::endl;

	return;
}