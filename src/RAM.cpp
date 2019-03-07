//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"

typedef unsigned char Byte;
//----------------------------------------------------------------------------------------------

RAM::RAM() {

}

void RAM::setMemory(unsigned short address, Byte value) {
    this->memory[address] = value;
}

Byte RAM::getMemory(unsigned short address) const {
    return this->memory[address];
}