//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
//----------------------------------------------------------------------------------------------

RAM::RAM() {
	// initialise memory with 0.
	for (int i = 0; i < 65536; i++) {
		this->memory[i] = 0;
	}
}

void RAM::setMemory(unsigned short address, Byte value) {
    this->memory[address] = value;
}

Byte RAM::getMemory(unsigned short address) const {
    return this->memory[address];
}