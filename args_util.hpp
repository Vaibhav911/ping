#ifndef ARSG_UTIL_HPP
#define ARSG_UTIL_HPP

#include <unordered_map>
#include <string>
#include <iostream>

#include "help.hpp"

void init_default(std::unordered_map<std::string, int> &parameters);
std::string handle_args(int argc, char **argv, std::unordered_map<std::string, int> &parameters);

#endif