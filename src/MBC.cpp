#include "../include/MBC.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

MBC::MBC() {
	this->rom = NULL;
    this->ram = NULL;
	this->EnableRamBanking = false;
	this->CurrentRomBank = this->CurrentRamBank = 0;
}

MBC::MBC(class ROM *rom, class RAM * ram) {
	this->rom = rom;
    this->ram = ram;
	this->EnableRamBanking = false;
	this->CurrentRomBank = this->CurrentRamBank = 0;
}

Byte MBC::ReadROM(Word address) {
	return this->rom->getMemory(address);
}

void MBC::WriteROM(Word address, Byte value) {
	//cout << "wrote to base class" << endl;
	return;
}

Byte MBC::ReadRAM(Word address) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		return 0xFF;
	} else {
		return this->ram->getMemory(address - ADDR_INT_RAM_1);
	}
}

void MBC::WriteRAM(Word address, Byte value) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		;
	} else {
		this->ram->setMemory(address - ADDR_INT_RAM_1, value);
	}
}

bool MBC::getRamEnable() const {
    return this->EnableRamBanking;
}

void MBC::setRamEnable(bool EnableRamBanking) {
    this->EnableRamBanking = EnableRamBanking;
}