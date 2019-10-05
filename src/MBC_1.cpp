#include "../include/MBC_1.hpp"
#include "../include/bit.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

#define MODE_ROM 0
#define MODE_RAM 1

void MBC_1::resetVar() {
	this->CurrentRomBank = 0x01;
	this->CurrentRamBank = 0x00;
	this->Mode = 0x00;
	this->HigherRBBits = 0x00;
	this->RomBanking = true;
}

void MBC_1::reset() {
    MBC::reset();
    resetVar();
}

MBC_1::MBC_1(): MBC() {
	resetVar();
}

MBC_1::MBC_1(class ROM* rom, class RAM* ram): MBC(rom, ram) {
	resetVar();
}

Byte MBC_1::getCurrentRomBank() const {
	return this->CurrentRomBank;
}

void MBC_1::EnableRamBank(unsigned short address, Byte value) {
	this->ram->setRamEnable((value & 0x0F) == 0x0A);

	return;
}

void MBC_1::ChangeLowRomBank(Byte value) {
	if (!value) {
		this->SelectedRomBank = 1;
	} else {
		this->SelectedRomBank &= 0b01100000;
		this->SelectedRomBank |= (value & 0x1F);
	}

	return;
}

void MBC_1::ChangeHighRomBank(Byte value) {
	this->SelectedRomBank &= 0x1F;
	this->SelectedRomBank |= (value & 0b01100000);
	
	return;
}

void MBC_1::ChangeRamBank(Byte value) {
	this->CurrentRamBank &= 0x03;

	return;
}

void MBC_1::ChangeMode(Byte value) {
	this->Mode = value & 0x01;

	return;
}

void MBC_1::HandleBanking(Word address, Byte value) {
	// ram enable.
	if (address < 0x2000) {
		EnableRamBank(address, value);
	}
	// change rom bank.
	else if (address >= 0x2000 && address < 0x4000) {
		ChangeLowRomBank(value);
	}
	// rom or ram bank change.
	else if (address >= 0x4000 && address < 0x6000) {
		if (this->Mode == MODE_ROM) {
			ChangeHighRomBank(value);
		}
		// RAM mode.
		else if (this->Mode == MODE_RAM) {
			ChangeRamBank(value);
		}
	}
	// rom and ram banking.
	else if (address >= 0x6000 && address < 0x8000) {
		ChangeMode(value);
	}

	return;
}

Byte MBC_1::ReadROM(Word address) {
	if (address < ADDR_ROM_1) {
		return this->rom->getMemory(address);
	} else {
		return this->rom->getMemory(address + (this->SelectedRomBank - 1) * 0x4000);
	}
}

void MBC_1::WriteROM(Word address, Byte value) {
	HandleBanking(address, value);

	return;
}

Byte MBC_1::ReadRAM(Word address) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram->getRamEnable()) {
			return this->ram->getRamBankMemory(address - ADDR_EXT_RAM);
		} else {
			return 0xFF;
		}
	} else {
		return this->ram->getMemory(address - ADDR_INT_RAM_1);
	}
}

void MBC_1::WriteRAM(Word address, Byte value) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram->getRamEnable()) {
			if (this->Mode == 0) {
				this->ram->setRamBankMemory(address - ADDR_EXT_RAM, value);
			} else {
				this->ram->setRamBankMemory(address - ADDR_EXT_RAM + this->ram->getCurrentRamBank() * 0x2000, value);
			}
		}
		else {
			cout << "didnt write to " << HEX16 << address << " with " << HEX << (int)value << endl;
		}
	} else {
		this->ram->setMemory(address - ADDR_INT_RAM_1, value);
	}
}
