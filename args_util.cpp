#include "args_util.hpp"

void init_default(std::unordered_map<std::string, int> &parameters)
{
	//Set default value to all parameters
	//used by the program.

	parameters["ip_ver"] = 4;
	parameters["ttl"] = 64;
	//ping_cnt equal to -1 means infinite pings will be send.
	parameters["ping_cnt"] = -1;
	//time between pings is in milli-seconds.
	parameters["time_bw_ping"] = 1000;
	//timeout means how long at max to wait for each packet
	parameters["timeout"] = 5;
	//quiet set to 0 means false
	//quiet enabled show only final statistics.
	parameters["quiet"] = 0;
}

std::string handle_args(int argc, char **argv, std::unordered_map<std::string, int> &parameters)
{
	//Handles all the command line arguments entered 
	//in the terminal window, and set the parameter 
	//values accordingly. Finally, returns the hostname.

	std::string argument;

	//host is IP-addr/Webserver name entered by user.
	std::string host;

	bool host_found = false;

	for (int ind=1;ind<argc;ind++)
	{
		//ignore ind=0, as it is name of current program
		argument = std::string(argv[ind]);
		
		if (argument == "-4")
		{
			parameters["ip_ver"] = 4;
		}
		else if (argument == "-6")
		{
			parameters["ip_ver"] = 6;
		}
		else if (argument == "--ttl" || argument == "-t")
		{
			ind++;
			parameters["ttl"] = atoi(argv[ind]);
		}
		else if (argument == "--interval" || argument == "-i")
		{
			ind++;
			parameters["time_bw_ping"] = atoi(argv[ind]);
		}
		else if (argument == "--count" || argument == "-c")
		{
			ind++;
			parameters["ping_cnt"] = atoi(argv[ind]);
		}
		else if (argument == "--timeout" || argument == "-to")
		{
			ind++;
			parameters["timeout"] = atoi(argv[ind]);
		}
		else if (argument == "--help" || argument == "-h")
		{
			help();
			exit(0);
		}
		else if (argument == "--version" || argument == "-v")
		{
			version();
			exit(0);
		}
		else if (argument == "--quiet" || argument == "-q")
		{
			parameters["quiet"] = 1;
		}
		else
		{
			if (!host_found)
			{
				host = argument;
				host_found = true;
			}
			else
			{
				std::cout << "unable to understand argument" << std::endl;
				exit(1);
			}
		}
	}
	return host;
} 