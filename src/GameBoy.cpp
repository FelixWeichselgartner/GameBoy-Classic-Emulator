//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include "../include/GPU.hpp"
#include "../include/Registers.hpp"

#include <iostream>
using namespace std;
#include <iomanip>
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
	this->cpu.rom.print(&this->cpu.ram);
}

void GameBoy::run() {
	while (this->cpu.getRunning()) {
		//AdditionTest();

		RomTest();

		cpu.setRunning(0);
	}
}

int main() {
    class GameBoy gameboy;
    gameboy.run();
    return 0;
}