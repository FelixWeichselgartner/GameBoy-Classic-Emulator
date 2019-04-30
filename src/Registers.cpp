//----------------------------------------------------------------------------------------------
#include "../include/format.hpp"
#include "../include/Registers.hpp"
#include "../include/RAM.hpp"
//----------------------------------------------------------------------------------------------

Registers::Registers() {
	this->AF = 0x01B0;
	this->BC = 0x0013;
	this->DE = 0x00D8;
	this->HL = 0x014D;
	this->PC = 0x0100; // PC initialised in CPU().
	this->SP = 0xFFFE;
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
    this->F = value & 0xF0;
}

Word Registers::getAF() const {
    return this->AF;
}
void Registers::setAF(Word value) {
    this->AF = value & 0xFFF0;
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

Word Registers::getBC() const {
    return this->BC;
}
void Registers::setBC(Word value) {
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

Word Registers::getDE() const {
    return this->DE;
}
void Registers::setDE(Word value) {
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

Word Registers::getHL() const {
    return this->HL;
}
void Registers::setHL(Word value) {
    this->HL = value;
}

Word Registers::getPC() const {
    return this->PC;
}
void Registers::setPC(Word value) {
    this->PC = value;
}

Word Registers::getSP() const {
    return this->SP;
}
void Registers::setSP(Word value) {
    this->SP = value;
}