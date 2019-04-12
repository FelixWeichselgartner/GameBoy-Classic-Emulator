//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
#include <cstdlib>
//----------------------------------------------------------------------------------------------

RAM::RAM() {
	this->RamBanks = NULL;
	this->RamBanks = new unsigned char[0x8000];
	for (int i = 0; i < 0x8000; i++) {
		RamBanks[i] = 0;
	}
	this->CurrentRamBank = 0;

	memory[0xFF05] = 0x00;
	memory[0xFF06] = 0x00;
	memory[0xFF07] = 0x00;
	memory[0xFF10] = 0x80;
	memory[0xFF11] = 0xBF;
	memory[0xFF12] = 0xF3;
	memory[0xFF14] = 0xBF;
	memory[0xFF16] = 0x3F;
	memory[0xFF17] = 0x00;
	memory[0xFF19] = 0xBF;
	memory[0xFF1A] = 0x7F;
	memory[0xFF1B] = 0xFF;
	memory[0xFF1C] = 0x9F;
	memory[0xFF1E] = 0xBF;
	memory[0xFF20] = 0xFF;
	memory[0xFF21] = 0x00;
	memory[0xFF22] = 0x00;
	memory[0xFF23] = 0xBF;
	memory[0xFF24] = 0x77;
	memory[0xFF25] = 0xF3;
	memory[0xFF26] = 0xF1;
	memory[0xFF40] = 0x91;
	memory[0xFF42] = 0x00;
	memory[0xFF43] = 0x00;
	memory[0xFF45] = 0x00;
	memory[0xFF47] = 0xFC;
	memory[0xFF48] = 0xFF;
	memory[0xFF49] = 0xFF;
	memory[0xFF4A] = 0x00;
	memory[0xFF4B] = 0x00;
	memory[0xFFFF] = 0x00;
}

Byte RAM::getMemory(unsigned short address) const {
	return this->memory[address];
}

void RAM::setMemory(unsigned short address, Byte value) {
    this->memory[address] = value;
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

void RAM::ChangeRamBank(Byte value) {
	CurrentRamBank = value & 0x03;
}