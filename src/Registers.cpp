//----------------------------------------------------------------------------------------------
#include <iostream>
#include <iomanip>

#include "../include/Registers.hpp"
#include "../include/RAM.hpp"
//----------------------------------------------------------------------------------------------

Registers::Registers() {
	this->AF = 0x01B0;
	this->BC = 0x0013;
	this->DE = 0x00D8;
	this->HL = 0x014D;
	this->PC = 0x0150; //0x0100 ||0x0000
	this->SP = 0xFFFE; //ADDR_STACK
}

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


void Registers::printFlags() {
    std::cout << "the flags: " << std::endl;
    for (int i = 7; i >= 0; i--) {
        std::cout << i;
    }
    std::cout << std::endl;
    std::cout << "ZNHC0000" << std::endl;
    std::cout << toBinary((int) this->F) << std::endl;
}

    
Byte Registers::getA() const {
    return this->A;
}
void Registers::setA(Byte value) {
    this->A = value;
}

Byte Registers::getF() const {
    return this->F;
}
void Registers::setF(Byte value) {
    this->F = value;
}

unsigned short Registers::getAF() const {
    return this->AF;
}
void Registers::setAF(unsigned short value) {
    this->AF = value;
}

Byte Registers::getB() const {
    return this->B;
}
void Registers::setB(Byte value) {
    this->B = value;
}

Byte Registers::getC() const {
    return this->C;
}
void Registers::setC(Byte value) {
    this->C = value;
}

unsigned short Registers::getBC() const {
    return this->BC;
}
void Registers::setBC(unsigned short value) {
    this->BC = value;
}

Byte Registers::getD() const {
    return this->D;
}
void Registers::setD(Byte value) {
    this->D = value;
}

Byte Registers::getE() const {
    return this->E;
}
void Registers::setE(Byte value) {
    this->E = value;
}

unsigned short Registers::getDE() const {
    return this->DE;
}
void Registers::setDE(unsigned short value) {
    this->DE = value;
}

Byte Registers::getH() const {
    return this->H;
}
void Registers::setH(Byte value) {
    this->H = value;
}

Byte Registers::getL() const {
    return this->L;
}
void Registers::setL(Byte value) {
    this->L = value;
}

unsigned short Registers::getHL() const {
    return this->HL;
}
void Registers::setHL(unsigned short value) {
    this->HL = value;
}

unsigned short Registers::getPC() const {
    return this->PC;
}
void Registers::setPC(unsigned short value) {
    this->PC = value;
}

unsigned short Registers::getSP() const {
    return this->SP;
}
void Registers::setSP(unsigned short value) {
    this->SP = value;
}