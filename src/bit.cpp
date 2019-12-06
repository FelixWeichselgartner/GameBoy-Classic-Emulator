#include "../include/bit.hpp"

bool testBit(Byte value, int bit) {
	Byte checker = 0x01 << bit;

	return (value & checker) == checker;
}

Byte resetBit(Byte value, int bit) {
	Byte mask = 0x01 << bit;

	return value & (~mask);
}

Byte setBit(Byte value, int bit) {
	Byte setter = 0x01 << bit;

	return value | setter;
}

Byte flipBit(Byte value, int bit) {
	Byte mask = 0x01 << bit;

	return value ^ mask;
}