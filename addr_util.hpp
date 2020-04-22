#ifndef ADDR_UTIL_H
#define ADDR_UTIL_H

#include <vector>
#include <iostream>
#include <string>
#include <string.h>

#include <netdb.h>
#include <arpa/inet.h>

std::vector<std::pair<int, std::string>> get_ip_addr(std::string host);

std::string get_addr_name(std::string host, int type) ;

unsigned int get_host_type(const std::string &host);

#endif