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

	if ((this->RamBanks = new Byte[this->RamLength]) == NULL) exit(2);

	for (int i = 0; i < this->RamLength; i++) {
		RamBanks[i] = 0xFF;
	}

	this->CurrentRamBank = 0;
}

void RAM::reset() {
    // Byte ram[ADDR_ECHO - ADDR_EXT_RAM] = {0};
    // not needed for first reset.
    // needed for 2nd, 3rd ... reset.
    for (int i = 0; i < ADDR_ECHO - ADDR_EXT_RAM; i++) {
        this->ram[i] = 0;
    }

    this->CurrentRamBank = 0;
    this->AmountBanks = 1;
    bool EnableRamBanking = false;
}

RAM::RAM() {
	this->RamBanks = NULL;
    reset();
}

Byte RAM::getMemory(Word address) const {
	return this->ram[address];
}

void RAM::setMemory(Word address, Byte value) {
    this->ram[address] = value;
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
	if (address >= this->RamLength) {
		cout << HEX16 << address << " not in ram bank memory range of ram length: " << this->RamLength << endl;
		return 0xFF;
	} else {
		return this->RamBanks[address];
	}
}

void RAM::setRamBankMemory(Word address, Byte value) {
	this->RamBanks[address] = value;
}

void RAM::ChangeRamBank(Byte value) {
	CurrentRamBank = value & 0x03;
}