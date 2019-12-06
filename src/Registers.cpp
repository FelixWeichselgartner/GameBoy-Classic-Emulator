//----------------------------------------------------------------------------------------------
#include "../include/Registers.hpp"

#include <iostream>
#include <iomanip>
using namespace std;
//----------------------------------------------------------------------------------------------

void Registers::reset() {
    /*
	this->AF = 0x01B0;
	this->BC = 0x0013;
	this->DE = 0x00D8;
	this->HL = 0x014D;
	*/
    this->AF = 0x11b0;
    this->BC = 0x0013;
    this->DE = 0x00d8;
    this->HL = 0x014d;
    this->PC = 0x0100; // PC initialised in CPU().
    this->SP = 0xFFFE;
    this->halt_bug_pc_no_increase = false;
}

Registers::Registers() {
	reset();
}

/**
 * @brief increase the program counter according to halt bug.
 * 
 */
void Registers::incPC() {
	if (!this->halt_bug_pc_no_increase) {
		this->PC++;
	} else {
		this->halt_bug_pc_no_increase = false;
	}
}

/**
 * @brief get one flag from flag register.
 * 
 * @param type character representing the flag.
 * @return Byte 0x01 if flag is set,
 * 				0x00 if flag is not set.
 */
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

/**
 * @brief set one flag in flag register.
 * 
 * @param type character representing the flag to set.
 */
void Registers::setFlag(char type) {
	switch (type) {
	case 'Z': this->setF(this->getF() | 0b10000000); break;
	case 'N': this->setF(this->getF() | 0b01000000); break;
	case 'H': this->setF(this->getF() | 0b00100000); break;
	case 'C': this->setF(this->getF() | 0b00010000); break;
	default: cout << "[type]" << (int)type << " does not exist" << endl; exit(1);
	}
}

/**
 * @brief reset one flag in flag register.
 * 
 * @param type character representing the flag to reset.
 */
void Registers::resetFlag(char type) {
	switch (type) {
		case 'Z': this->setF(this->getF() & 0b01111111); break;
		case 'N': this->setF(this->getF() & 0b10111111); break;
		case 'H': this->setF(this->getF() & 0b11011111); break;
		case 'C': this->setF(this->getF() & 0b11101111); break;
		default: cout << "[type]" << (int)type << " does not exist" << endl; exit(1);
	}
}

/**
 * @brief flip one flag in flag register.
 * 
 * @param type character representing the flag to flip.
 */
void Registers::flipFlag(char type) {
	switch (type) {
		case 'Z': this->F ^= 0b10000000; break;
		case 'N': this->F ^= 0b01000000; break;
		case 'H': this->F ^= 0b00100000; break;
		case 'C': this->F ^= 0b00010000; break;
		default: cout << "[type]" << (int)type << " does not exist" << endl; exit(1);
	}
}

/**
 * @brief set one flag in flag register to specified state.
 * 
 * @param type character representing the flag to set.
 * @param state state to set the flag to.
 */
void Registers::setFlagState(char type, bool state) {
	state ? setFlag(type) : resetFlag(type);
}

/**
 * @brief reset all flags to zero.
 * 
 */
void Registers::resetFlagAll() {
	this->F = 0x00;
}