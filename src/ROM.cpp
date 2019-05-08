//----------------------------------------------------------------------------------------------
#include "../include/Registers.hpp"
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include "../include/format.hpp"
#include "../include/MemoryMap.hpp"

#include <iostream>
using namespace std;
#include <iomanip>
#include <string>
#include <fstream>
#include <math.h>
//----------------------------------------------------------------------------------------------

Byte bootstrap[] = {
//     0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, // 0x00
	0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, // 0x10
	0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B, // 0x20
	0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9, // 0x30
	0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, // 0x40
	0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04, // 0x50
	0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2, // 0x60
	0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, // 0x70
	0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20, // 0x80
	0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17, // 0x90
	0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, // 0xA0
	0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, // 0xB0
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, // 0xC0
	0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, // 0xD0
	0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20, // 0xE0
	0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50, // 0xF0
};

ROM::ROM() {
	this->rom = NULL;
	this->RomSize = 0;
	this->EnableBootstrap = NULL;
	this->MBC_1 = this->MBC_2 = this->MBC_3 = this->MBC_5 = false;
	this->CurrentRomBank = 1;
	this->ram = NULL;
	this->RomBanking = true;
	this->RomBankingMode = 0x00;
}

ROM::ROM(class RAM* ram, bool *EnableBootstrap) {
	this->rom = NULL;
	this->RomSize = 0;
	if ((this->EnableBootstrap = EnableBootstrap) == NULL) exit(2);
	this->MBC_1 = this->MBC_2 = this->MBC_3 = this->MBC_5 = false;
	this->CurrentRomBank = 1;
	if ((this->ram = ram) == NULL) exit(2);
	this->RomBanking = true;
	this->RomBankingMode = 0x00;
}

ROM::~ROM() {
	delete[] rom;
}

Byte ROM::getMemory(unsigned int address) const {
	if (!(*EnableBootstrap) || address > 0x00FF) {
		if (!(address >= RomSize)) {
			return this->rom[address];
		} else {
			cout << "out of range: " << HEX16 << address << endl;
			return 0x00;
		}
	} else {
		return bootstrap[address];
	}
}

void ROM::load() {
	streampos size;

	ifstream gbfile;

	// working:
	//gbfile.open("Tetris.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Minesweeper.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("cpu_instrs.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Game Boy Controller Kensa Cartridge.gb", ios::in | ios::binary | ios::ate);

	// not working:
	//gbfile.open("instr_timing.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("mbc1/rom_512Kb.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Asterix.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Dr. Mario.gb", ios::in | ios::binary | ios::ate);
	gbfile.open("LinkAwakening.gb", ios::in | ios::binary | ios::ate);

	// unknown:
	//gbfile.open("rom_singles/2-causes.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("individual_m/02-write_timing.gb", ios::in | ios::binary | ios::ate);	

	if (gbfile.is_open()) {
		gbfile.seekg(0, ios::end);
		size = gbfile.tellg();
		cout << "file length: " << HEX16 << size << endl;
		gbfile.seekg(0, ios::beg);
		RomSize = (int)size;
		this->rom = new char[RomSize];
		if (this->rom != NULL) {
			gbfile.read(rom, size);
		} else {
			cout << "unable to reserve heap memory" << endl;
		}
		gbfile.close();
	} else {
		cout << "unable to open file" << endl;
	}

	InitialiseRomBaking();

	return;
}

void ROM::InitialiseRomBaking() {
	// memory bank controller modes:
	// http://gbdev.gg8.se/wiki/articles/The_Cartridge_Header
	// http://gbdev.gg8.se/wiki/articles/Memory_Bank_Controllers

	switch (rom[0x0147]) {
		// 00h  ROM ONLY
		case 0x00:
			cout << "Game uses no MBC." << endl;
			break;
		// 01h  MBC1
		case 0x01: 
			MBC_1 = true; 
			break;
		// 02h  MBC1+RAM
		case 2: 
			MBC_1 = true; 
			break;
		// 03h  MBC1+RAM+BATTERY
		case 3: 
			MBC_1 = true; 
			break;
		// 05h  MBC2
		case 5: 
			MBC_2 = true; 
			break;
		// 06h  MBC2+BATTERY
		case 0x06:
			MBC_2 = true;
			break;
		// 08h  ROM+RAM
		// 09h  ROM+RAM+BATTERY
		// 0Bh  MMM01
		// 0Ch  MMM01+RAM
		// 0Dh  MMM01+RAM+BATTERY
		// 0Fh  MBC3+TIMER+BATTERY
		case 0x0F:
			MBC_3 = true;
			break;
		// 10h  MBC3+TIMER+RAM+BATTERY
		case 0x10:
			MBC_3 = true;
			break;
		// 11h  MBC3
		case 0x11:
			MBC_3 = true;
			break;
		// 12h  MBC3+RAM
		case 0x12:
			MBC_3 = true;
			break;
		// 13h  MBC3+RAM+BATTERY
		case 0x13:
			MBC_3 = true;
			break;
		// 19h  MBC5
		case 0x19:
			MBC_5 = true;
			break;
		// 1Ah  MBC5+RAM
		case 0x1A:
			MBC_5 = true;
			break;
	    // 1Bh  MBC5+RAM+BATTERY
		case 0x1B:
			MBC_5 = true;
			break;
	    // 1Ch  MBC5+RUMBLE
		case 0x1C:
			MBC_5 = true;
			break;
	    // 1Dh  MBC5+RUMBLE+RAM
		case 0x1D:
			MBC_5 = true;
			break;
	    // 1Eh  MBC5+RUMBLE+RAM+BATTERY
		case 0x1E:
			MBC_5 = true;
			break;
	    // 20h  MBC6
	    // 22h  MBC7+SENSOR+RUMBLE+RAM+BATTERY
	   	// FCh  POCKET CAMERA
	    // FDh  BANDAI TAMA5
	    // FEh  HuC3
	    // FFh  HuC1+RAM+BATTERY
		default: 
			cout << "The Mode " << HEX << (int)rom[0x0147] << "h is currently not supported" << endl;
			exit(1);
			break;
	}

	if (MBC_1) cout << "Game uses MBC1." << endl;
	if (MBC_2) cout << "Game uses MBC2." << endl;
	if (MBC_3) cout << "Game uses MBC3." << endl;
	if (MBC_5) cout << "Game uses MBC5." << endl;

	ram->reserveRamBankMemory(rom[0x0148]);
}

Byte ROM::getCurrentRomBank() const {
	return this->CurrentRomBank;
}

Byte ROM::getRomBankingMode() const {
	return this->RomBankingMode;
}

void ROM::ChangeLowRomBank(Byte value) {
	if (MBC_2) {
		CurrentRomBank = value & 0x0F;
	} else {
		CurrentRomBank = (CurrentRomBank & 224) | (value & 31);
	}

	//if (CurrentRomBank == 0x04) 
	//	cout << "four" << endl;

	if (CurrentRomBank == 0x00) CurrentRomBank++;

	return;
}

void ROM::ChangeHighRomBank(Byte value) {
	CurrentRomBank &= 0x1F;
	CurrentRomBank |= (value & 0xE0);
	if (CurrentRomBank == 0x00) {
		CurrentRomBank++;
	}

	return;
}

void ROM::ChangeRomRamMode(Byte value) {
	this->RomBanking = value & 0x01;
	if (!this->RomBanking) {
		this->ram->setCurrentRamBank(0x00);
	}
	
	return;
}

void ROM::EnableRamBank(unsigned short address, Byte value) {
	if (!(MBC_2 && (address & 0x0010) == 0x0010)) {
		switch (value & 0x0F) {
			case 0x0A: this->ram->setRamEnable(true); break;
			case 0x00: this->ram->setRamEnable(false); break;
		}
	}

	return;
}

void ROM::HandleBanking(unsigned short address, Byte value) {
	// ram enable.
	if (address < 0x2000) {
		if (MBC_1 || MBC_2) {
			EnableRamBank(address, value);
		}
	}
	// change rom bank.
	else if (address >= 0x2000 && address < 0x4000) {
		if (MBC_1 || MBC_2) {
			ChangeLowRomBank(value);
		}
	}
	// rom or ram bank change.
	else if (address >= 0x4000 && address < 0x6000) {
		if (MBC_1) {
			if (RomBanking) { //RomBanking
				ChangeHighRomBank(value);
			} else {
				this->ram->ChangeRamBank(value);
			}
		}
	}
	// rom and ram banking.
	else if (address >= 0x6000 && address < 0x8000) {
		if (MBC_1) {
			ChangeRomRamMode(value);
		}
	}
}

string ROM::getGameName() {
	string name = "";
	for (int n = 0x134; n < 0x0142; n++) {
		name += (char)rom[n];
	}
	return name;
}