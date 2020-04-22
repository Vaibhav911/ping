#include "help.hpp"

void help()
{
	std::cout << "ping-cf\n\n";

	std::cout << "Following are supported flags\n\n"
		<< "-4 or -6 to use IPv4(default)/IPv6\n"
		<< "--help or -h to show help\n"
		<< "--quiet or -q to suppress details\n"
		<< "--version or -v to get version\n";
	std::cout << "----------------------------------------\n";

	std::cout << "Following are supported options\n\n"
		<< "--ttl or -t to modify time-to-live(default 64)\n"
		<< "--interval or -i to change interval between pings (in milli-sec) (default 1s)\n"
		<< "--count or -c to modify number of ping counts(default infinite)\n"
		<< "--timeout or -to to modify timeout(max waiting time) of each packet (in sec) (default 5s)\n";
	std::cout << "----------------------------------------\n";

	std::cout << "USE <CTRL-C> to stop the program.\n";
	std::cout << "----------------------------------------\n";

	std::cout << std::string("*NOTE*: If your program doesn't work properly,\n")
				+ std::string("try running it with sudo.\n");
	std::cout << "----------------------------------------\n";

	std::cout << "Few examples\n";
	std::cout << "sudo ./ping-cf www.google.com --ttl 20 --interval 2000\n";
	std::cout << "sudo ./ping-cf 157.240.192.35 -c 10 --timeout 1\n";
	std::cout << "sudo ./ping-cf www.facebook.com -6 -t 50 -i 2000 -to 4\n";
	std::cout << "sudo ./ping-cf 2406:2000:e4:a1a::10\n";

	std::cout << std::endl;
	exit(0);
}

void version()
{
	std::cout << "Ping-Cf: v1.0.0" << std::endl;;
	exit(0);
}