//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
#include <cstdlib>
//----------------------------------------------------------------------------------------------

void RAM::reserveRamBankMemory(Byte AmountBanks) {
	if ((this->RamBanks = new Byte[0x2000 * AmountBanks]) == NULL) exit(2);

	for (int i = 0; i < 0x2000 * AmountBanks; i++) {
		RamBanks[i] = 0;
	}

	this->CurrentRamBank = 0;
}

RAM::RAM() {
	this->RamBanks = NULL;
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
	return this->RamBanks[address];
}

void RAM::setRamBankMemory(Word address, Byte value) {
	this->RamBanks[address] = value;
}

void RAM::ChangeRamBank(Byte value) {
	CurrentRamBank = value & 0x03;
}