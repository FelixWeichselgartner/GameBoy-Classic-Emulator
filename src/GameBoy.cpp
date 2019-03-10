//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include "../include/GPU.hpp"
#include "../include/Registers.hpp"

#include <iostream>
using namespace std;
#include <iomanip>
#include <thread>

#include <time.h>
//----------------------------------------------------------------------------------------------

GameBoy::GameBoy() {
    
}

// seems to be working
void GameBoy::AdditionTest() {
	Byte a = 0b00001010, b = 0b00001100;
	this->cpu.registers.setA(a);
	this->cpu.registers.setB(b);
	cout << endl << dec << (int)this->cpu.registers.getA() << " + " << dec << (int)this->cpu.registers.getB() << endl;
	this->cpu.executeInstruction(0x80);
	cout << "result: " << dec << (int)this->cpu.registers.getA() << endl;
	this->cpu.registers.printFlags();

	a = b = 0b10000000;
	this->cpu.registers.setA(a);
	this->cpu.registers.setC(b);
	this->cpu.executeInstruction(0x41);
	cout << endl << dec << (int)this->cpu.registers.getA() << " + " << dec << (int)this->cpu.registers.getB() << endl;
	this->cpu.executeInstruction(0x80);
	cout << "result: " << dec << (int)this->cpu.registers.getA() << endl;
	this->cpu.registers.printFlags();

	a = b = 0b00000000;
	this->cpu.registers.setA(a);
	this->cpu.registers.setB(b);
	cout << endl << dec << (int)this->cpu.registers.getA() << " + " << dec << (int)this->cpu.registers.getB() << endl;
	this->cpu.executeInstruction(0x80);
	cout << "result: " << dec << (int)this->cpu.registers.getA() << endl;
	this->cpu.registers.printFlags();

	this->cpu.executeInstruction(0x10);
}

void GameBoy::RomTest() {
	this->cpu.rom.load(&this->cpu.ram);
	// Entry point to start of game code
	this->cpu.rom.print(&this->cpu.ram, 0x0100, 0x014F);
	// game code starts here
	this->cpu.rom.print(&this->cpu.ram, 0x0150, 0x01FF);
}

void GameBoy::tests() {
	AdditionTest();
	RomTest();
}

void delay(int micro_seconds) {
	clock_t start_time = clock();
	while (clock() < start_time + micro_seconds/1000)
		;
}

void GameBoy::run() {

	// this is not final
	int delaytime = 1000 * 1000 / this->cpu.getClockSpeed();
	//

	int count = 0;
	while (this->cpu.getRunning()) {
		cpu.CPUstep();

		/*
		count++;
		if (count % 10000 == 0) {
			cout << count << endl;
			//cpu.setRunning(0);
			//break;
		}
		*/

		// this is not final
		delay(delaytime);
		//
	}
}

#define MODE 1

int main() {
    class GameBoy gameboy;
	if (MODE) {
		gameboy.tests();
	} else {
		gameboy.run();
	}
    return 0;
}