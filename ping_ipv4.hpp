#ifndef PING_IPV4_HPP
#define PING_IPV4_HPP

#include <vector>
#include <string>
#include <iostream>

#include <sys/time.h>
#include <chrono>
#include <unistd.h>

#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <arpa/inet.h>

#define PING_PKT_S 64

struct ping_pkt_4
{
	struct icmphdr hdr;
	char msg[PING_PKT_S - sizeof (struct icmphdr)];
};

std::vector<double> ping4(std::string &host_name, std::string host_ip, int &ping_count, 
						  int ttl_val, int interval_milli_sec, int timeout, bool quiet);

unsigned short checksum(void *b, int len);

#endif