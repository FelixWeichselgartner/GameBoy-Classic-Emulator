//----------------------------------------------------------------------------------------------
#include "../include/Registers.hpp"
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include "../include/GameBoy.hpp"
#include "../include/format.hpp"

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

void cpBootstrap(class RAM *ram) {
	for (int i = 0; i < 0x0100; i++) {
		ram->setMemory(i, bootstrap[i]);
	}
}

void ROM::dltBootstrap(class RAM* ram) {
	for (int i = 0; i < 0x0100; i++) {
		ram->setMemory(i, rom[i]);
	}
}

ROM::ROM() {
	
}

ROM::ROM(class RAM* ram) {
	this->rom = NULL;
	this->RomSize = 0;
	this->MBC_1 = this->MBC_2 = false;
	this->CurrentRomBank = 1;
	this->ram = ram;
	this->RomBanking = true;
}

ROM::~ROM() {
	delete[] rom;
}

Byte ROM::getMemory(unsigned int address) const {
	return this->rom[address];
}

void ROM::load(class RAM* ram, bool enableBootstrap) {
	streampos size;

	ifstream gbfile;
	//gbfile.open("individual/07-jr,jp,call,ret,rst.gb", ios::in | ios::binary | ios::ate);
	gbfile.open("individual/11-op a,(hl).gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Asterix.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("individual/02-interrupts.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("09-op r,r.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Dr. Mario.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Tetris.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("rom_singles/8-instr_effect.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("individual_m/02-write_timing.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("cpu_instrs.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("unused_hwio-C.gb", ios::in | ios::binary | ios::ate);

	if (gbfile.is_open()) {
		gbfile.seekg(0, ios::end);
		size = gbfile.tellg();
		cout << "file length: " << HEX16 << size << endl;

		gbfile.seekg(0, ios::beg);
		RomSize = (int)size;
		this->rom = new char[RomSize];
		if (this->rom != NULL) {
			gbfile.read(rom, size);
			for (unsigned short i = enableBootstrap ? 0x0100 : 0x0000; i < ADDR_VRAM_T_S; i++) {
				ram->setMemory(i, (Byte)rom[i]);
			}
		} else {
			cout << "unable to reserve heap memory" << endl;
		}
		gbfile.close();
	} else {
		cout << "unable to open file" << endl;
	}

	InitialiseRomBaking();

	if (enableBootstrap) {
		cpBootstrap(ram);
	}

	return;
}

void ROM::InitialiseRomBaking() {
	/*
	memory bank controller modes (http://gbdev.gg8.se/wiki/articles/The_Cartridge_Header):

	00h  ROM ONLY                 19h  MBC5
	01h  MBC1                     1Ah  MBC5+RAM
	02h  MBC1+RAM                 1Bh  MBC5+RAM+BATTERY
	03h  MBC1+RAM+BATTERY         1Ch  MBC5+RUMBLE
	05h  MBC2                     1Dh  MBC5+RUMBLE+RAM
	06h  MBC2+BATTERY             1Eh  MBC5+RUMBLE+RAM+BATTERY
	08h  ROM+RAM                  20h  MBC6
	09h  ROM+RAM+BATTERY          22h  MBC7+SENSOR+RUMBLE+RAM+BATTERY
	0Bh  MMM01
	0Ch  MMM01+RAM
	0Dh  MMM01+RAM+BATTERY
	0Fh  MBC3+TIMER+BATTERY
	10h  MBC3+TIMER+RAM+BATTERY   FCh  POCKET CAMERA
	11h  MBC3                     FDh  BANDAI TAMA5
	12h  MBC3+RAM                 FEh  HuC3
	13h  MBC3+RAM+BATTERY         FFh  HuC1+RAM+BATTERY
	*/

	switch (rom[0x0147]) {
		case 0:
			cout << "Game uses no MBC." << endl;
			break;
		case 1: 
			MBC_1 = true; 
			break;
		case 2: 
			MBC_1 = true; 
			break;
		case 3: 
			MBC_1 = true; 
			break;
		case 4: 
			MBC_2 = true; 
			break;
		case 5: 
			MBC_2 = true; 
			break;
		default: 
			cout << "The Mode " << HEX << (int)rom[0x0147] << "h is currently not supported" << endl;
			exit(1);
			break;
	}

	if (MBC_1) {
		cout << "Game uses MBC1." << endl;
	}

	if (MBC_2) {
		cout << "Game uses MBC2." << endl;
	}
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

	if (CurrentRomBank == 0x00) {
		CurrentRomBank++;
	}

	return;
}

void ROM::ChangeHighRomBank(Byte value) {
	CurrentRomBank = (CurrentRomBank & 0x1F) | (value & 0xE0);
	if (CurrentRomBank == 0x00) {
		CurrentRomBank++;
	}

	return;
}

void ROM::ChangeRomRamMode(Byte value) {
	this->RomBanking = value & 0x01;
	if (this->RomBanking) {
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
	if (address < 0x2000 && (MBC_1 || MBC_2)) {
		EnableRamBank(address, value);
	}
	// change rom bank.
	else if (address >= 0x2000 && address < 0x4000 && (MBC_1 || MBC_2)) {
		ChangeLowRomBank(value);
	}
	// rom or ram bank change.
	else if (address >= 0x4000 && address < 0x6000 && MBC_1) {
		if (RomBanking) { //RomBanking
			ChangeHighRomBank(value);
		} else {
			this->ram->ChangeRamBank(value);
		}
	}
	// rom and ram banking.
	else if (address >= 0x6000 && address < 0x8000 || MBC_1) {
		ChangeRomRamMode(value);
	}
}

string ROM::getGameName(class RAM* ram) {
	string name = "";
	for (int n = 0x134; n < 0x0142; n++) {
		name += (char) ram->getMemory(n);
	}
	return name;
}

void ROM::print(class RAM* ram, unsigned short start, unsigned short end) {
	cout << "Name: " << getGameName(ram) << endl;
	cout << "File Hexdump: " << endl << endl;
	cout << "          ";
	for (int i = 0; i < 16; i++) {
		cout << HEX << i << " ";
	}
	cout << endl;
	for (int i = start, c = 0; i < start + (end + 1 - start ) / 16; i++, c++) {
		cout << HEX16 << setw(8) << start + c * 16 << ": ";
		for (int k = 0; k < 16; k++) {
			cout << HEX << (int)ram->getMemory(start + c * 16 + k) << " ";
		}
		cout << endl;
	}
}