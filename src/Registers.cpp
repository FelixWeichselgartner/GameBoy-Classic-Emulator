//----------------------------------------------------------------------------------------------
#include "../include/Registers.hpp"

#include <iostream>
#include <iomanip>
using namespace std;
//----------------------------------------------------------------------------------------------

Registers::Registers() {
	this->AF = 0x01B0;
	this->BC = 0x0013;
	this->DE = 0x00D8;
	this->HL = 0x014D;
	this->PC = 0x0100; // PC initialised in CPU().
	this->SP = 0xFFFE;
	this->halt_bug_pc_no_increase = false;
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
void Registers::incPC() {
	if (!this->halt_bug_pc_no_increase) {
		this->PC++;
	} else {
		this->halt_bug_pc_no_increase = false;
	}
}

void Registers::halt_bug() {
	this->halt_bug_pc_no_increase = true;
}

Word Registers::getSP() const {
    return this->SP;
}
void Registers::setSP(Word value) {
    this->SP = value;
}
void Registers::incSP() {
	this->SP++;
}
void Registers::decSP() {
	this->SP--;
}

Byte Registers::getFlag(char type) {
	Byte tmp = this->F;

	switch (type) {
		case 'Z': tmp &= 0b10000000; break;
		case 'N': tmp &= 0b01000000; break;
		case 'H': tmp &= 0b00100000; break;
		case 'C': tmp &= 0b00010000; break;
		default: cout << "[type]" << (int)type << " does not exist" << endl; exit(1);
	}

	return tmp ? 0x01 : 0x00;
}

void Registers::setFlag(char type) {
	switch (type) {
	case 'Z': this->setF(this->getF() | 0b10000000); break;
	case 'N': this->setF(this->getF() | 0b01000000); break;
	case 'H': this->setF(this->getF() | 0b00100000); break;
	case 'C': this->setF(this->getF() | 0b00010000); break;
	default: cout << "[type]" << (int)type << " does not exist" << endl; exit(1);
	}
}

void Registers::resetFlag(char type) {
	switch (type) {
		case 'Z': this->setF(this->getF() & 0b01111111); break;
		case 'N': this->setF(this->getF() & 0b10111111); break;
		case 'H': this->setF(this->getF() & 0b11011111); break;
		case 'C': this->setF(this->getF() & 0b11101111); break;
		default: cout << "[type]" << (int)type << " does not exist" << endl; exit(1);
	}
}

void Registers::flipFlag(char type) {
	switch (type) {
		case 'Z': this->F ^= 0b10000000; break;
		case 'N': this->F ^= 0b01000000; break;
		case 'H': this->F ^= 0b00100000; break;
		case 'C': this->F ^= 0b00010000; break;
		default: cout << "[type]" << (int)type << " does not exist" << endl; exit(1);
	}
}

void Registers::setFlagState(char type, bool state) {
	state ? setFlag(type) : resetFlag(type);
}

void Registers::resetFlagAll() {
	this->F = 0x00;
}