//----------------------------------------------------------------------------------------------
#include "../include/Registers.hpp"
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include "../include/format.hpp"
#include "../include/MemoryMap.hpp"
#include "../include/bit.hpp"

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
	this->ram = NULL;
}

ROM::ROM(class RAM* ram, bool *EnableBootstrap) {
	this->rom = NULL;
	this->RomSize = 0;
	if ((this->EnableBootstrap = EnableBootstrap) == NULL) exit(2);
	if ((this->ram = ram) == NULL) exit(2);
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
	gbfile.open("mbc1/rom_1Mb.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Asterix.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("Dr. Mario.gb", ios::in | ios::binary | ios::ate);
	//gbfile.open("LinkAwakening.gb", ios::in | ios::binary | ios::ate);

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

	return;
}

string ROM::getGameName() {
	string name = "";
	for (int n = 0x134; n < 0x0142; n++) {
		name += (char)rom[n];
	}
	return name;
}