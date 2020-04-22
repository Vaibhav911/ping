#include "ping_ipv4.hpp"

std::vector<double> ping4(std::string &host_name, std::string host_ip, int &ping_count,
						int ttl_val, int interval_milli_sec, int timeout_val, bool quiet)
{	
	//Pings a given IP address and ping options 
	//are controlled by given parameters.

	//time_values store RTT of each packet.
	std::vector<double> time_values;

	struct sockaddr_in ping_addr, recv_addr;

	ping_addr.sin_family = AF_INET;

	if (inet_pton(AF_INET, host_ip.c_str(), &ping_addr.sin_addr) != 1)
	{
		std::cout << "Error in setting IP address of destination" << std::endl;
		return std::vector<double> {};
	}

	struct ping_pkt_4 pckt;

	//msg_cnt is count of sent messages.
	int msg_cnt = 0;

	//time_out is maximum allowable wait time
	//for each packet.
	struct timeval time_out;
	time_out.tv_sec = timeout_val;
	time_out.tv_usec = 0;

	int sockfd;

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)
	{
		std::cout << "FATAL: Error in creating socket." << std::endl;
		exit(0);
	}

	if (setsockopt(sockfd, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0)
	{
		std::cout << "MAJOR: Error in setting TTL of packet, continuing with default value..." << std::endl;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&time_out, sizeof(time_out)) != 0)
	{
		std::cout << "MAJOR: Unable to set timeout of packet, continuing..." << std::endl;
	}

	unsigned int addr_len;

	while (ping_count)
	{
		//ping_count equals -1 means infinite
		//number of iterations.
		if (ping_count != -1)
			ping_count--;

		bzero(&pckt, sizeof(pckt));
		pckt.hdr.type = ICMP_ECHO;
		pckt.hdr.un.echo.id = getpid();

		unsigned int byt = 0;
		//Fill the data with some arbitrary chars.
		for (byt=0; byt<sizeof(pckt.msg)-1; byt++)
			pckt.msg[byt] = byt + '0';

		pckt.msg[byt] = 0;
		pckt.hdr.un.echo.sequence = msg_cnt++;
		pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

		//send packet
		int bytes_sent = sendto(sockfd, &pckt, sizeof(pckt), 0, 
								(struct sockaddr *)&ping_addr, sizeof(ping_addr));

		auto start_time = std::chrono::steady_clock::now();

		if (bytes_sent < 0)
		{
			std::cout << "MINOR: Packet sending failed. Trying again..." << std::endl;
			continue;
		}

		addr_len = sizeof(recv_addr);

		//receive packet
		int bytes_recv = recvfrom(sockfd, &pckt, sizeof(pckt), 0,
								(struct sockaddr *)&recv_addr, &addr_len); 

		auto end_time = std::chrono::steady_clock::now();

		if (bytes_recv < 0)
		{
			//If failure, then add -1 to time_values.
			time_values.push_back(-1.0);
			std::cout << "MINOR: **Timeout** Unable to receive packet, took more than " 
					  << time_out.tv_sec << " sec" << std::endl;
			continue;
		}

		auto time_span = std::chrono::duration_cast<std::chrono::duration<double>> (end_time - start_time);
		time_values.push_back(time_span.count() * 1000);

		if (!quiet)
		{
			//If user has not set quiet flag, then display packet info
			std::cout << "Bytes sent|recv: " << bytes_sent << "|" << bytes_recv 
				<< " || Time: " << 1000 * time_span.count() << " ms"
				<< " || Host: " << host_name << '(' << host_ip << ')'
				<< " || TTL: " << ttl_val
				<< " || ICMP Seq: " << msg_cnt << std::endl;
		}

		//Sleep/wait for sometime before sending next packet.
		usleep(interval_milli_sec * 1000);
	}
	
	return time_values;
}