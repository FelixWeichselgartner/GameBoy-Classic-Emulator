//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include "../include/GPU.hpp"
#include "../include/Registers.hpp"

#include <iostream>
using namespace std;
#include <iomanip>

typedef unsigned char Byte;
//----------------------------------------------------------------------------------------------

GameBoy::GameBoy() {
    
}

void GameBoy::AdditionTest() {
	Byte a = 0b00001010, b = 0b00001100;
	this->cpu.registers.setA(a);
	this->cpu.registers.setB(b);
	this->cpu.executeInstruction(0x80);

	cout << "result: " << hex << (int)this->cpu.registers.getA() << endl;

	this->cpu.registers.printFlags();
}

void GameBoy::run() {
	AdditionTest();
}

int main() {
    class GameBoy gameboy;
    gameboy.run();
    return 0;
}