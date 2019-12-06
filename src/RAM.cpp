//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
#include <cstdlib>
#include <iostream>
#include "../include/format.hpp"
using namespace std;
//----------------------------------------------------------------------------------------------

void RAM::reserveRamBankMemory(Byte AmountBanks) {
	this->AmountBanks = AmountBanks;
	this->RamLength = 0x2000 * this->AmountBanks;

	*this->RamBanks = Array(Byte, this->RamLength);

	for (int i = 0; i < this->RamLength; i++) {
		this->RamBanks->set(i, 0xFF);
	}

	this->CurrentRamBank = 0;
}

void RAM::reset() {
    // Byte ram[ADDR_ECHO - ADDR_EXT_RAM] = {0};
    // not needed for first reset.
    // needed for 2nd, 3rd ... reset.
    for (int i = 0; i < ADDR_ECHO - ADDR_EXT_RAM; i++) this->ram.set(i, 0);

    this->CurrentRamBank = 0;
    this->AmountBanks = 1;
    this->EnableRamBanking = false;
}

RAM::RAM() {
    reset();
}

Byte RAM::getMemory(Word address) const {
	return this->ram.get((int)address);
}

void RAM::setMemory(Word address, Byte value) {
    this->ram.set(address, value);
}

Byte RAM::getCurrentRamBank() const {
	return this->CurrentRamBank;
}

void RAM::setCurrentRamBank(Byte value) {
	this->CurrentRamBank = value;
}

bool RAM::getRamEnable() const {
	return this->EnableRamBanking;
}

void RAM::setRamEnable(bool en) {
	this->EnableRamBanking = en;
}

Byte RAM::getRamBankMemory(Word address) const {
	return this->RamBanks->get(address);
}

void RAM::setRamBankMemory(Word address, Byte value) {
	this->RamBanks->set(address, value);
}

void RAM::ChangeRamBank(Byte value) {
	CurrentRamBank = value & 0x03;
}