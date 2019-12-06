#include "../include/MBC_1.hpp"
#include "../include/bit.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

#define MODE_ROM 0
#define MODE_RAM 1

void MBC_1::resetVar() {
	this->SelectedRomBank = 0x01;
	this->Bank1 = 0x01;
	this->Bank2 = 0x00;
	this->SelectedRamBank = 0x00;
	this->Mode = 0x00;
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
	return this->SelectedRomBank;
}

void MBC_1::EnableRamBank(Word address, Byte value) {
	this->ram->setRamEnable((value & 0x0F) == 0x0A);
	
	/*if ((value & 0x0F) == 0x0A) {
		cout << ">>ram enabled: " << HEX16 << address << " " << dec << (int)value << endl;
	} else {
		cout << ">>ram disabled: " << HEX16 << address << " " << dec << (int)value << endl;
	}*/

	return;
}

void MBC_1::BankReg1(Byte value) {
	this->Bank1 = value & 0x1F;

	if (this->Bank1 == 0x00) {
		this->Bank1 = 0x01;
	}

	return;
}

void MBC_1::BankReg2(Byte value) {
	this->Bank2 = value & 0x03;

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
	// lower 5 bit of rom bank.
	else if (address >= 0x2000 && address < 0x4000) {
		BankReg1(value);
	}
	// rom or ram bank change.
	else if (address >= 0x4000 && address < 0x6000) {
		BankReg2(value);
	}
	// change rom and ram banking mode.
	else if (address >= 0x6000 && address < 0x8000) {
		ChangeMode(value);
	}

	return;
}

Byte MBC_1::ReadROM(Word address) {
	// 0x0000 - 0x4000 is always bank 00h.
	if (address < ADDR_ROM_1) {
		if (this->Mode == MODE_ROM) {
			this->SelectedRomBank = 0;
		} else {
			this->SelectedRomBank = this->Bank2 << 5;
		}
		this->SelectedRomBank %= this->rom->getAmountRomBanks();
		if (address + (this->SelectedRomBank - 1) * 0x4000 >= this->rom->getRomSize()) {
			cout << ">>Error: MBC_1.cpp (l.100): org address: " << address << " address: " << HEX16 << (int)address + (this->SelectedRomBank - 1) * 0x4000 << " rbn: " << (int)this->SelectedRomBank << endl;
			cout << "this->bank2 = " << HEX << (int)this->Bank2 << endl;
		}
		return this->rom->getMemory(address + this->SelectedRomBank * 0x4000);
	} 
	// rom banks.
	// only ROM Banks 00-1Fh can be used during Mode 1.
	else {
		if (this->Mode == MODE_ROM) {
			this->SelectedRomBank = this->Bank1 | (this->Bank2 << 5);
		} else {
			this->SelectedRomBank = this->Bank1;
		}
		this->SelectedRomBank %= this->rom->getAmountRomBanks();
		if (address + (this->SelectedRomBank - 1) * 0x4000 >= this->rom->getRomSize()) {
			cout << ">>Error: MBC_1.cpp (l.121): org address: " << address << " address: " << HEX16 << (int)address + (this->SelectedRomBank - 1) * 0x4000 << " rbn: " << (int)this->SelectedRomBank << endl;
		}
		return this->rom->getMemory(address + (this->SelectedRomBank - 1) * 0x4000);
	}
}

void MBC_1::WriteROM(Word address, Byte value) {
	HandleBanking(address, value);

	return;
}

Byte MBC_1::ReadRAM(Word address) {
	// external RAM.
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		// external RAM can only be accessed if RAM is enabled.
		if (this->ram->getRamEnable()) {
			// only RAM Bank 00h can be used during MODE_ROM.
			if (this->Mode == MODE_ROM) {
				return this->ram->getRamBankMemory(address - ADDR_EXT_RAM);
			} 
			// all RAM Banks can be accessed during MODE_RAM.
			else {
				this->SelectedRamBank = this->Bank2;
				this->SelectedRamBank %= this->ram->getAmountBanks();
				return this->ram->getRamBankMemory(address - ADDR_EXT_RAM + this->SelectedRamBank * 0x2000);
			}
		} 
		// if external RAM is disabled return 0xFF.
		else {
			return 0xFF;
		}
	} 
	// internal RAM.
	else {
		return this->ram->getMemory(address - ADDR_INT_RAM_1);
	}
}

void MBC_1::WriteRAM(Word address, Byte value) {
	// external RAM.
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		// external RAM can only be accessed if RAM is enabled.
		if (this->ram->getRamEnable()) {
			// only RAM Bank 00h can be used during MODE_ROM.
			if (this->Mode == MODE_ROM) {
				this->ram->setRamBankMemory(address - ADDR_EXT_RAM, value);
			} 
			// all RAM Banks can be accessed during MODE_RAM.
			else {
				this->SelectedRamBank = this->Bank2;
				this->SelectedRamBank %= this->ram->getAmountBanks();
				this->ram->setRamBankMemory(address - ADDR_EXT_RAM + this->SelectedRamBank * 0x2000, value);
			}
		}
		// dont write if not enabled.
		else {
			cout << ">>Error: MBC_1.cpp (l.155): didnt write to " << HEX16 << address << " with " << HEX << (int)value << " -- " << dec << (int)this->SelectedRomBank << endl;
		}
	} 
	// internal RAM.
	else {
		this->ram->setMemory(address - ADDR_INT_RAM_1, value);
	}
}
