//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include <iostream>
using namespace std;
#include <iomanip>
#include <fstream>
#include <math.h>
//----------------------------------------------------------------------------------------------

ROM::ROM() {
    
}

void ROM::load(class RAM* ram) {
	char* buffer;
	streampos size;

	ifstream gbfile;
	gbfile.open("Tetris.gb", ios::in | ios::binary | ios::ate);
	if (gbfile.is_open()) {
		gbfile.seekg(0, ios::beg);
		size = 32768;
		buffer = new char[size];
		gbfile.read(buffer, size);
		for (int i = 0; i < size; i++) {
			ram->setMemory(i, (Byte)buffer[i]);
		}
		gbfile.close();
		delete[] buffer;
	} else {
		cout << "unable to open file" << endl;
	}
}

void ROM::print(class RAM* ram) {
	cout << "File Hexdump: " << endl << endl;
	cout << "          ";
	for (int i = 0; i < 16; i++) {
		cout << setw(2) << hex << setfill('0') << i << " ";
	}
	cout << endl;

	for (int i = 0; i < 16; i++) {
		cout << setw(8) << hex << setfill('0') << i * 16 << ": ";
		for (int k = 0; k < 16; k++) {
			cout << setw(2) << hex << (int)ram->getMemory(i * 16 + k) << " ";
		}
		cout << endl;
	}
}