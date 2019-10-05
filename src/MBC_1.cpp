#include "../include/MBC_1.hpp"
#include "../include/bit.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

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
	if (!this->ram->getRamEnable()) {
		saveExtRam();
	}

	return;
}

void MBC_1::ChangeLowRomBank(Byte value) {
	this->CurrentRomBank &= 0b01100000;
	this->CurrentRomBank |= (value & 0x0F);

	return;
}

void MBC_1::ChangeHighRomBank(Byte value) {
	this->CurrentRomBank &= 0x0F;
	this->CurrentRomBank |= (value & 0b01100000);
	
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
	cout << "write " << address << " with " << hex << (int)value << endl;
	// ram enable.
	if (address < 0x2000) {
		EnableRamBank(address, value);
	}
	// change rom bank.
	else if (address >= 0x2000 && address < 0x4000) {
		this->LowRomBank = this->HighRomBank = -1;
		// ROM mode.
		if (this->Mode == 0) {
			ChangeLowRomBank(value);
		}
		// RAM mode.
		else if (this->Mode == 1) {
			ChangeRamBank(value);
		}
	}
	// rom or ram bank change.
	else if (address >= 0x4000 && address < 0x6000) {
		this->LowRomBank = this->HighRomBank = -1;
		ChangeHighRomBank(value);
	}
	// rom and ram banking.
	else if (address >= 0x6000 && address < 0x8000) {
		this->LowRomBank = this->HighRomBank = -1;
		ChangeMode(value);
	}

	return;
}

int MBC_1::getAdjustedRomBankLow() {
	// not calculated so far.
	if (this->LowRomBank == -1) {
		if (this->Mode == 0) {
			this->LowRomBank = 0;
		} else {
			this->LowRomBank = (this->CurrentRomBank << 5) % (this->rom->getRomSize() / 0x4000);
		}
	}
	return this->LowRomBank;
}

int MBC_1::getAdjustedRomBankHigh() {
	// not calculated so far.
	if (this->HighRomBank == -1) {
		this->HighRomBank = this->CurrentRomBank;
		if (!(CurrentRomBank % 0x20)) {
			this->HighRomBank++;
		}
		if (this->Mode == 1) {
			this->HighRomBank = (this->HighRomBank & 0x0F) | (this->CurrentRomBank << 5);
		}
		this->HighRomBank %= (this->rom->getRomSize() / 0x4000);
	}
	return this->HighRomBank;
}

Byte MBC_1::ReadROM(Word address) {
	cout << "current: " << (int)this->CurrentRomBank << endl;
	if (address < ADDR_ROM_1) {
		getAdjustedRomBankLow();
		cout << "Low rom bank: " << this->LowRomBank << endl;
		cout << "address" << endl;
		cout << (int)address + this->LowRomBank * 0x4000 << endl;
		return this->rom->getMemory(address + this->LowRomBank * 0x4000);
	} else {
		getAdjustedRomBankHigh();
		cout << "high rom bank: " << this->HighRomBank << endl;
		return this->rom->getMemory(address - 0x4000 + this->HighRomBank * 0x4000);
	}
}

void MBC_1::WriteROM(Word address, Byte value) {
	HandleBanking(address, value);

	return;
}

Byte MBC_1::ReadRAM(Word address) {
	//cout << "read ram" << endl;
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

void MBC_1::saveExtRam() {

}

void MBC_1::loadExtRam() {

}