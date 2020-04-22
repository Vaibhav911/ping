#include "addr_util.hpp"
#include "args_util.hpp"
#include "ping_ipv4.hpp"
#include "ping_ipv6.hpp"
#include "stats.hpp"
#include "help.hpp"

#include <csignal>

void signal_handler(int signal_num);

//"params" contains value of all the parameters
//used by programs such as TTL, ip_version,
//value of flags etc.
std::unordered_map<std::string, int> params;

int main(int argc, char **argv)
{
    //Signal handler for SIGINT (CTRL-C)
	signal(SIGINT, signal_handler);

	//Contains the RTT of each packet.
	std::vector<double> time_values;
	
	init_default(params);

	std::string host;
	std::string host_name;
	int host_t = 0;

	host = handle_args(argc, argv, params);
	host_t = get_host_type(host);

	if (params["ping_cnt"] != -1 && params["ping_cnt"] <= 0)
	{
		std::cout << "Bad number of counts" << std::endl;
		return 0;
	}

	if (4 == host_t)
	{
		//IPv4 host entered by user
		host_name = get_addr_name(host, 4);
		std::cout << "Host_name " << host_name << std::endl;
		time_values = ping4(host_name, host, params["ping_cnt"], params["ttl"], 
							params["time_bw_ping"], params["timeout"], params["quiet"]);
	}
	else if (6 == host_t)
	{
		//IPv6 host entered by user
		host_name = get_addr_name(host, 6);
		std::cout << "Host_name " << host_name << std::endl;
		time_values = ping6(host_name, host, params["ping_cnt"], params["ttl"], 
							params["time_bw_ping"], params["timeout"], params["quiet"]);
	}
	else
	{
		//Normal website name entered by user
		std::vector<std::pair<int, std::string>> addresses;
		addresses = get_ip_addr(host);

		if (addresses.size() == 0)
		{
            //If address size is 0, means unable to get
            //IP address of website.
			return -1;
		}

		std::cout << "IP addresses of " << host<< std::endl;
		std::cout << "----------------------------------------------" << std::endl;
		for (auto val: addresses)
		{
			std::cout << "IPv" << val.first << ' ' << val.second << std::endl;
		}
		std::cout << "==============================================" << std::endl;

		if (4 == params["ip_ver"])
		{
			//IPv4 is user preference.
			for (auto addr: addresses)
			{
				if (addr.first == 4)
				{
					host_name = get_addr_name(addr.second, 4);
					time_values = ping4(host_name, addr.second, params["ping_cnt"], params["ttl"],
						 				params["time_bw_ping"], params["timeout"], params["quiet"]);
					calc_stats(time_values, host);
					return 0;
				}
			}
			//If program still not exited, means no IPv4 address present.
			std::cout << "No IPv4 address returned by DNS." << std::endl;
			return 0;
		}
		else
		{
			//IPv6 is user preference.
			for (auto addr: addresses)
			{
				if (addr.first == 6)
				{
					host_name = get_addr_name(addr.second, 6);
					time_values = ping6(host_name, addr.second, params["ping_cnt"], params["ttl"],
						 				params["time_bw_ping"], params["timeout"], params["quiet"]);
					calc_stats(time_values, host);						 
					return 0;
				}
			}
			//If program still not exited, means no IPv6 address present.
			std::cout << "No IPv6 address returned by DNS." << std::endl;
			return 0;
		}
	}
	calc_stats(time_values, host);

	return 0;
}

void signal_handler(int signal_num)
{
	params["ping_cnt"] = 0;
}