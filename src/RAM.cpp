//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
#include <cstdlib>
//----------------------------------------------------------------------------------------------

void RAM::reserveRamBankMemory(Byte AmountBanks) {
	this->AmountBanks = AmountBanks;

	if ((this->RamBanks = new Byte[0x2000 * AmountBanks]) == NULL) exit(2);

	for (int i = 0; i < 0x2000 * AmountBanks; i++) {
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
	return this->RamBanks[address];
}

void RAM::setRamBankMemory(Word address, Byte value) {
	this->RamBanks[address] = value;
}

void RAM::ChangeRamBank(Byte value) {
	CurrentRamBank = value & 0x03;
}