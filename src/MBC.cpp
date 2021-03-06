#include "../include/MBC.hpp"
#include <fstream>

#include <iostream>
#include <iomanip>
using namespace std;

void MBC::resetVar() {
	this->battery = false;
}

void MBC::reset() {
    resetVar();
}

MBC::MBC() {
	this->rom = NULL;
    this->ram = NULL;
	resetVar();
}

MBC::MBC(class ROM *rom, class RAM * ram) {
	this->rom = rom;
    this->ram = ram;
	resetVar();
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

void MBC::saveExtRam() {
	if (this->battery) {
		int length = this->ram->length();
		char *temp = new char[length];
		for (int i = 0; i < length; i++) {
			temp[i] = this->ram->getRamBankMemory(i);
		}
		ofstream savefile;
		savefile.open(this->rom->getGameName() + ".sav", ios::out | ios::binary);

		if (savefile.is_open()) {
			savefile.write(temp, length);
			//savefile.write() // buffer, size
		}
		cout << "savved" << endl;
		delete[] temp;
	}
}

void MBC::loadExtRam() {
	ifstream savefile;
	savefile.open(this->rom->getGameName() + ".sav", ios::in | ios::binary);

	if (savefile.is_open()) {
		int length = this->ram->length();
		char *temp = new char[length];
		if (temp != NULL) {
			savefile.read(temp, length);
		}
		for (int i = 0; i < length; i++) {
			this->ram->setRamBankMemory(i, temp[i]);
		}
		cout << "loaded" << endl;
		delete temp;
	}
}