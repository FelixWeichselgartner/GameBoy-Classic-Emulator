#include "../include/RAM.hpp"

// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

void RAM::setMemory(unsigned short address, byte value) {
    this->memory[address] = value;
}

byte RAM::getMemory(unsigned short address) const {
    return this->memory[address];
}