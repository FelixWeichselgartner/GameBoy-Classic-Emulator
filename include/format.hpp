#ifndef FORMAT_HPP_
#define FORMAT_HPP_

#include <iostream>

#define HEX		hex << setw(2) << setfill('0')
#define HEX16	hex << setw(4) << setfill('0')

std::string toBinary(int n);

#endif