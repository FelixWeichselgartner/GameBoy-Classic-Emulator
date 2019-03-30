//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"
#include "../include/RAM.hpp"
#include "../include/ROM.hpp"
#include "../include/GPU.hpp"
#include "../include/Registers.hpp"
#include "../include/format.hpp"

#include <iostream>
using namespace std;
#include <iomanip>

#include <thread>
#include <chrono>

#include <time.h>

#include "../lib/SDL2/include/SDL2/SDL.h"

#include <fstream>
#include <string>
//----------------------------------------------------------------------------------------------

GameBoy::GameBoy() {
	
}

void GameBoy::PrintRegisters() {
	cout << "[PC]: " << HEX16 << this->cpu.registers.getPC() << "\t [OPCODE]: ";
	cout << HEX << (int)this->cpu.ram.getMemory(this->cpu.registers.getPC()) << "\t\t<-   ";
	cout << "A" << HEX << (int)this->cpu.registers.getA() << " ";
	cout << "F" << HEX << (int)this->cpu.registers.getF() << "   ";
	cout << "B" << HEX << (int)this->cpu.registers.getB() << " ";
	cout << "C" << HEX << (int)this->cpu.registers.getC() << "   ";
	cout << "D" << HEX << (int)this->cpu.registers.getD() << " ";
	cout << "E" << HEX << (int)this->cpu.registers.getE() << "   ";
	cout << "HL" << HEX16 << this->cpu.registers.getHL() << "   ";
	cout << "SP" << HEX16 << this->cpu.registers.getSP() << endl;
	//cout << "Memory[0xFF44] = " << (int)this->cpu.ReadByte(0xFF44) << endl;
	//this->cpu.registers.printFlags();
}

void GameBoy::PrintRegistersFile(ofstream &file) {
	file << "[PC]: " << HEX16 << this->cpu.registers.getPC() << "\t [OPCODE]: ";
	file << HEX << (int)this->cpu.ram.getMemory(this->cpu.registers.getPC()) << "\t\t<-   ";
	file << "A" << HEX << (int)this->cpu.registers.getA() << " ";
	file << "F" << HEX << (int)this->cpu.registers.getF() << "   ";
	file << "B" << HEX << (int)this->cpu.registers.getB() << " ";
	file << "C" << HEX << (int)this->cpu.registers.getC() << "   ";
	file << "D" << HEX << (int)this->cpu.registers.getD() << " ";
	file << "E" << HEX << (int)this->cpu.registers.getE() << "   ";
	file << "HL" << HEX16 << this->cpu.registers.getHL() << "   ";
	file << "SP" << HEX16 << this->cpu.registers.getSP() << endl;
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
	this->cpu.rom.print(&this->cpu.ram, 0x0000, 0x014F);
	// game code starts here
	this->cpu.rom.print(&this->cpu.ram, 0x0150, 0x01FF);
	// vram
	this->cpu.rom.print(&this->cpu.ram, 0x8000, 0x8080);
}

void GameBoy::PushPopTest() {
	Byte n1 = 0x12, n2 = 0x34, m1, m2;
	unsigned short s1 = 0x5678, s2;

	cout << "SP: " << HEX << cpu.registers.getSP() << endl;
	cpu.push8bit(n1);
	cout << "SP: " << HEX << cpu.registers.getSP() << endl;
	cpu.push8bit(n2);
	cout << "SP: " << HEX << cpu.registers.getSP() << endl;
	m2 = cpu.pop8bit();
	cout << "SP: " << HEX << cpu.registers.getSP() << endl;
	m1 = cpu.pop8bit();
	cout << "SP: " << HEX << cpu.registers.getSP() << endl;
	cpu.push16bit(s1);
	cout << "SP: " << HEX << cpu.registers.getSP() << endl;
	s2 = cpu.pop16bit();
	cout << "SP: " << HEX << cpu.registers.getSP() << endl << endl;

	cout << "n1: " << HEX << (int)n1 << " n2: " << HEX << (int)n2 << endl;
	cout << "m1: " << HEX << (int)m1 << " m2: " << HEX << (int)m2 << endl;
	cout << "s1: " << HEX16 << s1 << " s2: " << HEX16 << s2 << endl;

}

void GameBoy::decFlagTest() {
	this->cpu.registers.setB(0x0F);
	this->cpu.registers.setF(0x60);
	cout << "B: 0x0F, F: 0x60" << endl;
	this->cpu.registers.printFlags();
	this->cpu.registers.setB(cpu.dec(this->cpu.registers.getB()));
	cout << "F: " << HEX << (int)this->cpu.registers.getF() << endl;
	this->cpu.registers.printFlags();
}

void GameBoy::add2x16bithl() {
	this->cpu.registers.setHL(0xff0f);
	this->cpu.registers.setF(0x40);
	cout << "BEFORE:\tHL:" << HEX16 << this->cpu.registers.getHL() << " F: " << HEX << (int)this->cpu.registers.getF() << endl;
	this->cpu.registers.printFlags();
	this->cpu.registers.setHL(this->cpu.add16bit(this->cpu.registers.getHL(), this->cpu.registers.getHL(), 'u'));
	cout << "AFTER:\tHL:" << HEX16 << this->cpu.registers.getHL() << " F: " << HEX << (int)this->cpu.registers.getF() << endl;
	this->cpu.registers.printFlags();
}

void GameBoy::Debug_InputAndLog(SDL_Event &windowEvent) {
	int c, cyclesInstruction, delaytime = 1000 / 60;
	
	ofstream logFile;
	logFile.open("logFile.log");

	char key; bool keyEn = true;
	char game = 'M'; // 'M' == MINESWEEPER

	cout << "You can quit by pressing 'r'" << endl << endl;

	while (this->cpu.getRunning()) {
		cyclesInstruction = 0;
		bool printVRAMAfterInstruction = false;

		while (cyclesInstruction < MAXCYCLES) {
			if (SDL_PollEvent(&windowEvent)) {
				if (windowEvent.type == SDL_QUIT) {
					return;
				}
			}

			if (game == 'M') {
				switch (this->cpu.registers.getPC()) {
				case 0x06e6:
					keyEn = false;
					cout << "[call] @ 0x06e6" << endl;
					break;
				case 0x0157:
					keyEn = true;
					break;
				case 0x05fd:
					keyEn = false;
					cout << "[call] @ 0x05fd" << endl;
					break;
				case 0x015a:
					keyEn = true; 
					break;
				case 0x5879:
					keyEn = false;
					cout << "[call] @ 0x5879" << endl;
					break;
				case 0x015d:
					keyEn = true;
					break;
				case 0x06ff:
					if (keyEn) {
						keyEn = false;
						cout << "[call] @ 0x06ff" << endl;
					}
					break;
				case 0x0166:
					keyEn = true;
					break;

				case 0x08ef:
					keyEn = false;
					cout << "[call] @ 0x08ef" << endl;
					break;
				case 0x4cd4:
					keyEn = true;
					break;
				case 0x0708:
					keyEn = false;
					cout << "[call] @ 0x0708" << endl;
					break;
				case 0x4cd7:
					keyEn = true;
					break;
				case 0x4d25: 
					keyEn = false; // true;
					cout << "[call] @ 0x4d25" << endl;
					cout << "==== Tiles are being copied here ====" << endl;
					break;
				case 0x4cda:
					keyEn = true;
					break;
				case 0x0297:
					keyEn = false;
					cout << "[call] @ 0x0297" << endl;
					break;
				case 0x4cdd:
					keyEn = true;
					break;
				case 0x02e5:
					keyEn = false;
					cout << "[call] @ 0x02e5" << endl;
					break;
				case 0x4ce0:
					keyEn = true;
					break;
				case 0x07c0:
					keyEn = false; //true
					cout << "[call] @ 0x07c0 -- copying numbers + letters" << endl;
					// registers don't fit in the end
					break;
				case 0x4d2e:
					keyEn = true;
					break;
				case 0x4d37:
					keyEn = true;
					break;

				case 0x0800:
					keyEn = true;
					break;
				}
			}

			if (keyEn) {
				if (printVRAMAfterInstruction) {
					cpu.rom.print(&cpu.ram, 0x8000, 0xA000);
					printVRAMAfterInstruction = false;
				}

				key = cin.get();
				if (key == 'r') {
					cpu.setRunning(false);
					break;
				} else if (key == 'p') {
					printVRAMAfterInstruction = true;
				}

				PrintRegisters();
				PrintRegistersFile(logFile);
			}

			c = cpu.CPUstep();
			cyclesInstruction += c;
			cpu.UpdateTimers(c);
			gpu.UpdateGraphics(c);
			cpu.DoInterupts();
		}
		gpu.render();
		this_thread::sleep_for(chrono::milliseconds(delaytime));
	}

	cpu.rom.print(&cpu.ram, 0x8000, 0xA000);

	logFile.close();

	SDL_DestroyRenderer(gpu.getRenderer());
	SDL_DestroyWindow(gpu.getWindow());
	SDL_Quit();
}

void GameBoy::tests(int mode) {
	SDL_Event windowEvent;
	bool running = true;
	
	switch (mode) {
	case 1: AdditionTest(); break;
	case 2:
		while (running) {
			if (SDL_PollEvent(&windowEvent)) {
				if (windowEvent.type == SDL_QUIT) {
					running = false;
				}
			}
			this->gpu.TestGraphics();
		}
		break;
	case 3: RomTest(); break;
	case 4: gpu.TestTiles(); break;
	case 5:
		Debug_InputAndLog(windowEvent);
		break;
	case 6:
		gpu.RenderNintendoLogo();
		for (int i = 0; i < Y_RES; i++) {
			gpu.renderDisplay(i);
		}

		while (1) {

		}

		SDL_DestroyRenderer(gpu.getRenderer());
		SDL_DestroyWindow(gpu.getWindow());
		SDL_Quit();
		break;
	case 7:
		PushPopTest();
		break;
	case 8:
		decFlagTest();
		break;
	case 9:
		add2x16bithl();
		break;
	}
}

void GameBoy::run() {
	SDL_Event windowEvent;

	int c, cyclesInstruction;
	int delaytime = 1000 / 60;

	while (this->cpu.getRunning()) {

		cyclesInstruction = 0;

		while (cyclesInstruction < MAXCYCLES) {
			if (SDL_PollEvent(&windowEvent)) {
				if (windowEvent.type == SDL_QUIT) {
					return;
				}
			}

			c = cpu.CPUstep();
			cyclesInstruction += c;
			cpu.UpdateTimers(c);
			gpu.UpdateGraphics(c);
			cpu.DoInterupts();

		}

		gpu.render();
		this_thread::sleep_for(chrono::milliseconds(delaytime));
	}

	SDL_DestroyRenderer(gpu.getRenderer());
	SDL_DestroyWindow(gpu.getWindow());
	SDL_Quit();
}

#define MODE 0
// MODE 0		normal mode
// MODE 1		addition test
// MODE 2		gpu debug
// MODE 3		rom test
// MODE 4		tiles test
// MODE 5		input -> next instruction and save in log file
// MODE 6		show Nintendo Logo
// MODE 7		push and pop test
// MODE 8		dec flag test
// MODE 9		add 2 times 16 bit (hl)

int main(int argc, char *argv[]) {
    class GameBoy gameboy;
	if (MODE) {
		gameboy.tests(MODE);
	} else {
		gameboy.run();
	}
    return 0;
}