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
#include <chrono>

#include <time.h>

#include <SDL.h>
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
	// vram
	this->cpu.rom.print(&this->cpu.ram, 0x8000, 0x8080);
}

void GameBoy::tests(int mode) {
	SDL_Event windowEvent;
	bool running = true;
	int delaytime;
	
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
		delaytime = 1000 * 1000 * 1000; // 1 second

		while (this->cpu.getRunning()) {
			if (SDL_PollEvent(&windowEvent)) {
				if (windowEvent.type == SDL_QUIT) {
					return;
				}
			}
			cout << "[PC]: " << setw(4) << setfill('0') << hex << this->cpu.registers.getPC() << "\t [OPCODE]: ";
			cout << hex << setw(2) << setfill('0') << (int)this->cpu.ram.getMemory(this->cpu.registers.getPC()) << "\t\t->   ";
			cout << "A" << setw(2) << setfill('0') << hex << (int)this->cpu.registers.getA() << " ";
			cout << "F" << setw(2) << setfill('0') << (int)this->cpu.registers.getF() << "   ";
			cout << "B" << setw(2) << setfill('0') << hex << (int)this->cpu.registers.getB() << " ";
			cout << "C" << setw(2) << setfill('0') << (int)this->cpu.registers.getC() << "   ";
			cout << "D" << setw(2) << setfill('0') << hex << (int)this->cpu.registers.getD() << " ";
			cout << "E" << setw(2) << setfill('0') << (int)this->cpu.registers.getE() << "   ";
			cout << "HL" << setw(4) << setfill('0') << hex << (int)this->cpu.registers.getHL() << "   " << endl;
			//this->cpu.registers.printFlags();

			cpu.CPUstep();
			gpu.UpdateGraphics();
			cpu.DoInterupts();

			this->cpu.rom.print(&this->cpu.ram, 0x8000, 0x8001);

			this_thread::sleep_for(chrono::nanoseconds(delaytime));
		}

		SDL_DestroyRenderer(gpu.getRenderer());
		SDL_DestroyWindow(gpu.getWindow());
		SDL_Quit();
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
	}
}

void delay(int nano_seconds) {
	clock_t start_time = clock();
	while (clock() < start_time + nano_seconds / 1000)
		;
}

void GameBoy::run() {
	SDL_Event windowEvent;

	// this is not final
	int delaytime = 1000 * 1000 * 1000 / this->cpu.getClockSpeed();
	// 1/clockspeed = 2.5 * 10^-7 = 250 ns
	//

	while (this->cpu.getRunning()) {
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				return;
			}
		}
		cpu.CPUstep();
		gpu.UpdateGraphics();
		cpu.DoInterupts();

		// this is not final
		this_thread::sleep_for(chrono::nanoseconds(delaytime));
		//
	}

	SDL_DestroyRenderer(gpu.getRenderer());
	SDL_DestroyWindow(gpu.getWindow());
	SDL_Quit();
}

#define MODE 5
// MODE 0		normal mode
// MODE 1		cpu debug
// MODE 2		gpu debug
// MODE 3		rom test
// MODE 4		tiles test
// MODE 5		slow motion pc + opcode
// MODE 6		show Nintendo Logo

int main(int argc, char *argv[]) {
    class GameBoy gameboy;
	if (MODE) {
		gameboy.tests(MODE);
	} else {
		gameboy.run();
	}
    return 0;
}