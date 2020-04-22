#include "addr_util.hpp"

std::string get_addr_name(std::string host_addr, int type=4)
{
	//Fetches the name of a given IP address. 
	char host_name[1024];
	char service[20];
	if (4 == type)
	{
		//IPv4
		struct sockaddr_in ipv4addr;
		ipv4addr.sin_family = AF_INET;
		ipv4addr.sin_port = htons(3490);
		
		if (inet_pton(AF_INET, host_addr.c_str(), &ipv4addr.sin_addr) != 1)
		{
			return std::string("");
		}

		if (getnameinfo((struct sockaddr *)&ipv4addr, sizeof (ipv4addr),
					host_name, sizeof (host_name), service, sizeof (service), 0) != 0)
		{
			return std::string("");
		}
	}
	else
	{
		//IPv6	
		struct sockaddr_in6 ipv6addr;
		ipv6addr.sin6_family = AF_INET6;
		ipv6addr.sin6_port = htons(4950);
		if (inet_pton(AF_INET6, host_addr.c_str(), &ipv6addr.sin6_addr) != 1)
		{
			return std::string("");
		}

		if (getnameinfo((struct sockaddr *)&ipv6addr, sizeof (ipv6addr),
					host_name, sizeof (host_name), service, sizeof (service), 0) != 0)
		{
			return std::string("");
		}
	}
	return std::string(host_name);
}

std::vector<std::pair<int, std::string>> get_ip_addr(std::string host)
{
	///Fetches all pairs of IP address and IP versions
	//of a given host.
	//These values are stored in addresses.
	std::vector<std::pair<int, std::string>> addresses;

	struct addrinfo hints, *res, *ptr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	char ip_str[INET6_ADDRSTRLEN];
	std::string ip_ver;

	if (getaddrinfo(host.c_str(), NULL, &hints, &res) != 0)
	{
		std::cout << "Error in fetching ip address." << std::endl;
		//If unable to fetch ip addresses, return empty vector.
		return std::vector<std::pair<int, std::string>> {};
	}
	for (ptr = res; ptr != NULL; ptr = ptr->ai_next)
	{
		void *addr;

		if (ptr->ai_family == AF_INET)
		{
			//Address type is IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			addr = &(ipv4->sin_addr);
			ip_ver = "IPv4";
		}
		else
		{
			//Address type is IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
			addr = &(ipv6->sin6_addr);
			ip_ver = "IPv6";
		}

		//convert ip addr to a string
		inet_ntop(ptr->ai_family, addr, ip_str, sizeof (ip_str) );

		if (ip_ver == "IPv4")
		{
			addresses.push_back(std::make_pair(4, ip_str));
		}
		else
		{
			addresses.push_back(std::make_pair(6, ip_str));
		}
	}

	freeaddrinfo(res);

	return addresses;
}

unsigned int get_host_type(const std::string &host)
{
	//Find whether string entered by user is IPv4 address,
	//or IPv6 address or a usual web server name.
	struct sockaddr_in sa;
	struct sockaddr_in6 sa6;

	if (inet_pton(AF_INET, host.c_str(), &(sa.sin_addr)) != 0)
	{
		// 4 implies IPv4
		return 4;
	}
	else if (inet_pton(AF_INET6, host.c_str(), &(sa6.sin6_addr)) != 0)
	{
		// 6 implies IPv6
		return 6;
	}
	else
	{
		// 0 implies normal website name
		return 0;
	}	
}