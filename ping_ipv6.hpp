#ifndef PING_IPV6_HPP
#define PING_IPV6_HPP

#include <vector>
#include <string>
#include <iostream>

#include <sys/time.h>
#include <chrono>
#include <unistd.h>

#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <arpa/inet.h>

#include "ping_ipv4.hpp"

#define PING_PKT_S 64

struct ping_pkt_6
{
	struct icmp6_hdr hdr;
	char msg[PING_PKT_S - sizeof (struct icmp6_hdr)];
};

std::vector<double> ping6(std::string &host_name, std::string host_ip, int &ping_count, 
						int ttl_val, int interval_milli_sec, int timeout, bool quiet);

double send_ipv6_ping(std::string &host_name, std::string &host_ip, 
					  int ttl_val, int timeout, int msg_cnt, bool quiet);

#endif