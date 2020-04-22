#include "ping_ipv6.hpp"

std::vector<double> ping6(std::string &host_name, std::string host_ip, int &ping_count, 
						int ttl_val, int interval_milli_sec, int timeout_val, bool quiet)
{
	//Pings an IPv6 address based on options
	//provided in arguments.

	//time_values is vector containing time taken
	//by each packet.
	std::vector<double> time_values;
	double time_val = -1;

	int msg_cnt = 0;

	while (ping_count)
	{
		//ping_count 1 means send packets infinitely.
		if (ping_count != -1)
			ping_count--;

		msg_cnt++;

		time_val = send_ipv6_ping(host_name, host_ip, ttl_val, timeout_val, msg_cnt, quiet);
		time_values.push_back(time_val);

		//Sleep/wait for some time before sending next packet.
		usleep(interval_milli_sec * 1000);
	}

	return time_values;
}

double send_ipv6_ping(std::string &host_name, std::string &host_ip, int ttl_val, 
					  int timeout_val, int msg_cnt, bool quiet)
{
	//Sends one IPv6 ping to a given address.

	struct sockaddr_in6 ping_addr, recv_addr;

	ping_addr.sin6_family = AF_INET6;
	ping_addr.sin6_port = htons(0);
	if (inet_pton(AF_INET6, host_ip.c_str(), &ping_addr.sin6_addr) != 1)
	{
		std::cout << "Error in setting IP address of destination" << std::endl;
		return -1.0;
	}

	struct ping_pkt_6 pckt;

	struct timeval time_out;
	time_out.tv_sec = timeout_val;
	time_out.tv_usec = 0;

	int sockfd;
	sockfd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
	if (sockfd < 0)
	{
		std::cout << "FATAL: Error in creating socket." << sockfd << std::endl;
		exit(1);
	}

	if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ttl_val, sizeof(ttl_val)) != 0)
	{
		std::cout << "MAJOR: Error in setting TTL of packet, continuing with default value..." 
				  << std::endl;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&time_out, sizeof(time_out)) != 0)
	{
		std::cout << "MAJOR: Unable to set timeout after TTL, continuing..." 
				  << std::endl;
	}

	unsigned int addr_len;

	bzero(&pckt, sizeof(pckt));
	pckt.hdr.icmp6_type = ICMP6_ECHO_REQUEST;
	pckt.hdr.icmp6_code = 0;
	pckt.hdr.icmp6_id = getpid();

	//Fill packet data with arbitrary characters.
	unsigned int byt = 0;
	for (byt=0; byt<sizeof(pckt.msg)-1; byt++)
		pckt.msg[byt] = byt + '0';

	pckt.msg[byt] = 0;
	pckt.hdr.icmp6_seq = msg_cnt;
	pckt.hdr.icmp6_cksum = 0;
	pckt.hdr.icmp6_cksum = checksum(&pckt, sizeof(pckt));

	//send packet
	int bytes_sent = sendto(sockfd, &pckt, sizeof(pckt), 0, 
							(struct sockaddr *)&ping_addr, sizeof(ping_addr));

	auto start_time = std::chrono::steady_clock::now();

	if (bytes_sent < 0)
	{
		std::cout << "MINOR: Packet sending failed. Trying again..." 
				  << bytes_sent << "\n";
	}

	addr_len = sizeof(recv_addr);
	//receive packet
	int bytes_recv = recvfrom(sockfd, &pckt, sizeof(pckt), 0, 
							(struct sockaddr *)&recv_addr, &addr_len); 

	auto end_time = std::chrono::steady_clock::now();

	if (bytes_recv < 0)
	{
		// cout << "MINOR: Unable to receive packet." << "\n";
		std::cout << "MINOR: **Timeout** Unable to receive packet, took more than " 
				   << time_out.tv_sec << " sec" << std::endl;
		return -1.0;
	}

	auto time_span = std::chrono::duration_cast<std::chrono::duration<double>> (end_time - start_time);

	if (!quiet)
	{
		std::cout << "Bytes sent|rcv: " << bytes_sent << "|" << bytes_recv 
			<< " || Time: " << 1000 * time_span.count() << " ms"
			<< " || Host: " << host_name << '(' << host_ip << ')'
			<< " || TTL: " << ttl_val
			<< " || ICMP seq: " << pckt.hdr.icmp6_seq << std::endl;
	}

	close(sockfd);
	return 1000 * time_span.count();
}

unsigned short checksum(void *b, int len)
{
	unsigned short *buf = (unsigned short *)b;
	unsigned int sum = 0;
	unsigned short result;

	for (sum=0;len>1;len -= 2)
		sum += *buf++;

	if (len == 1)
		sum += *(unsigned char *)buf;

	sum = (sum >> 16) + (sum & 0xFFFF);

	sum += (sum >> 16);

	result = ~sum;
	return result;
}