#include "../include/format.hpp"

std::string toBinary(int n) {
	std::string r;

	while (n != 0) {
		r = (n % 2 == 0 ? "0" : "1") + r;
		n /= 2;
	}

	while (r.length() < 8) {
		r = "0" + r;
	}

	return r;
}