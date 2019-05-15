#include "../include/MBC_1.hpp"
#include "../include/bit.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

MBC_1::MBC_1() {
	this->CurrentRomBank = 0x01;
	this->CurrentRamBank = 0x00;
	this->Mode = 0x00;
	this->HigherRBBits = 0x00;
	this->rom = NULL;
	this->ram = NULL;
	RomBanking = true;
}

MBC_1::MBC_1(class ROM* rom, class RAM* ram) {
	this->CurrentRomBank = 0x01;
	this->CurrentRamBank = 0x00;
	this->Mode = 0x00;
	this->HigherRBBits = 0x00;
	this->rom = rom;
	this->ram = ram;
	this->RomBanking = true;
}

Byte MBC_1::getCurrentRomBank() const {
	return this->CurrentRomBank;
}

void MBC_1::EnableRamBank(unsigned short address, Byte value) {
	if ((address & 0x0010) != 0x0010) {
		switch (value & 0x0F) {
			case 0x0A: this->ram->setRamEnable(true); break;
			case 0x00: this->ram->setRamEnable(false); break;
		}
	}

	return;
}

void MBC_1::ChangeLowRomBank(Byte value) {
	if (this->Mode == 0)
		this->CurrentRomBank = value & 0x1F | (HigherRBBits << 5);
	else
		this->CurrentRomBank = value & 0x1F;

	if (CurrentRomBank == 0x00 || CurrentRomBank == 0x20
		|| CurrentRomBank == 0x40 || CurrentRomBank == 0x60) {
		CurrentRomBank++;
	}

	CurrentRomBank &= (this->rom->getRomSize() / 0x4000 - 1); // -2 if bank0 doesnt count

	return;
}

void MBC_1::ChangeHighRomBank(Byte value) {
	if (this->Mode == 1) {
		this->CurrentRamBank = value & 0x03;
		this->CurrentRamBank &= (this->ram->getAmountBanks() - 1);
	} else {
		HigherRBBits = value & 0x03;
		CurrentRomBank = (CurrentRomBank & 0x1F) | (HigherRBBits << 5);
		/*
		if (CurrentRomBank == 0x00 || CurrentRomBank == 0x20
			|| CurrentRomBank == 0x40 || CurrentRomBank == 0x60) {
			CurrentRomBank++;
		} */
		CurrentRomBank &= (this->rom->getRomSize() / 0x4000 - 1);
	}
	
	return;
}

void MBC_1::ChangeMode(Byte value) {
	if (!(this->ram->getAmountBanks() != 3 && testBit(value, 0)))
		this->Mode = value & 0x01;

	return;
}

void MBC_1::HandleBanking(unsigned short address, Byte value) {
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
		ChangeHighRomBank(value);
	}
	// rom and ram banking.
	else if (address >= 0x6000 && address < 0x8000) {
		ChangeMode(value);
	}
}

Byte MBC_1::ReadROM(Word address) {
	if (address < ADDR_ROM_1) {
		return this->rom->getMemory(address);
	} else {
		return this->rom->getMemory(address - 0x4000 + this->CurrentRomBank * 0x4000);
	}
}

void MBC_1::WriteROM(Word address, Byte value) {
	HandleBanking(address, value);
}

Byte MBC_1::ReadRAM(Word address) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram->getRamEnable()) {
			if (this->Mode == 0) {
				return this->ram->getRamBankMemory(address - ADDR_EXT_RAM);
			} else {
				return this->ram->getRamBankMemory(address - ADDR_EXT_RAM + this->ram->getCurrentRamBank() * 0x2000);
			}
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