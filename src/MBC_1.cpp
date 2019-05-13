#include "../include/MBC_1.hpp"
#include "../include/bit.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

MBC_1::MBC_1() {
	this->CurrentRomBank = 0x01;
	this->CurrentRamBank = 0x00;
	this->RomBankingMode = 0x00;

	this->rom = NULL;
	this->ram = NULL;

	RomBanking = true;
}

MBC_1::MBC_1(class ROM* rom, class RAM* ram) {
	this->CurrentRomBank = 0x01;
	this->CurrentRamBank = 0x00;
	this->RomBankingMode = 0x00;

	this->rom = rom;
	this->ram = ram;

	RomBanking = true;
}

Byte MBC_1::getCurrentRomBank() const {
	return this->CurrentRomBank;
}

void MBC_1::ChangeLowRomBank(Byte value) {
	CurrentRomBank = (CurrentRomBank & 0xE0) | (value & 0x1F);
	if (CurrentRomBank == 0x00 || CurrentRomBank == 0x20
		|| CurrentRomBank == 0x40 || CurrentRomBank == 0x60) {
		CurrentRomBank++;
	}
	CurrentRomBank &= (this->rom->getRomSize() / 0x4000 - 1);

	return;
}

void MBC_1::ChangeHighRomBank(Byte value) {
	cout << "changed high rom bank" << endl;
	CurrentRomBank = (CurrentRomBank & 0x1F) | ((value & 0x03) << 5);
	if (CurrentRomBank == 0x00 || CurrentRomBank == 0x20
		|| CurrentRomBank == 0x40 || CurrentRomBank == 0x60) {
		CurrentRomBank++;
	}
	CurrentRomBank &= (this->rom->getRomSize() / 0x4000 - 1);

	return;
}

void MBC_1::ChangeRomRamMode(Byte value) {
	this->RomBanking = !testBit(value, 0);

	return;
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
		if (RomBanking) { //RomBanking
			ChangeHighRomBank(value);
		} else {
			this->ram->ChangeRamBank(value);
		}
	}
	// rom and ram banking.
	else if (address >= 0x6000 && address < 0x8000) {
		ChangeRomRamMode(value);
	}
}

Byte MBC_1::ReadROM(Word address) {
	if (address < ADDR_ROM_1) {
		return this->rom->getMemory(address);
	} else {
		//if (address > 0x8000) 
		//	cout << HEX16 << address - ADDR_ROM_1 + (this->rom->getCurrentRomBank() * 0x4000) << endl;
		return this->rom->getMemory(address + ((this->CurrentRomBank % (this->rom->getRomSize() / 0x4000) - 1) * 0x4000));
	}
}

void MBC_1::WriteROM(Word address, Byte value) {
	cout << "wrote to rom" << endl;
	HandleBanking(address, value);
}

Byte MBC_1::ReadRAM(Word address) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram->getRamEnable())
			return this->ram->getRamBankMemory(address - ADDR_EXT_RAM + (this->ram->getCurrentRamBank() * 0x2000));
		else return 0xFF; //not quite sure if this is correct
	} else {
		return this->ram->getMemory(address - ADDR_INT_RAM_1);
	}
}

void MBC_1::WriteRAM(Word address, Byte value) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram->getRamEnable())
			this->ram->setRamBankMemory(address - ADDR_EXT_RAM + (this->ram->getCurrentRamBank() * 0x2000), value);
		else cout << "didnt write @ address: " << HEX16 << address << " value: " << HEX << (int)value << endl;
	} else {
		this->ram->setMemory(address - ADDR_INT_RAM_1, value);
	}
}