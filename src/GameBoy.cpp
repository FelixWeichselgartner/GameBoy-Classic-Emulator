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

#include <time.h>

#include <SDL2/SDL.h>

#include <fstream>
#include <string>
//----------------------------------------------------------------------------------------------

void delay(int milli_seconds) {
	if (milli_seconds < 0) {
		return;
	} else {
		clock_t start_time = clock();
		while (clock() < start_time + milli_seconds)
			;
	}
}

GameBoy::GameBoy() {
	cpu.setJoypadLink(&joypad);
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
	cout << "SP" << HEX16 << this->cpu.registers.getSP() << "   ";
	cout << "lY" << HEX << (int)this->cpu.ReadByte(0xff44) << endl;
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
	this->cpu.rom.load(&this->cpu.ram, false);
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
	this->cpu.registers.setHL(this->cpu.add16bit(this->cpu.registers.getHL(), this->cpu.registers.getHL()));
	cout << "AFTER:\tHL:" << HEX16 << this->cpu.registers.getHL() << " F: " << HEX << (int)this->cpu.registers.getF() << endl;
	this->cpu.registers.printFlags();
}

void GameBoy::OP_DA_Test() {
	this->cpu.registers.setF(0x30);
	cout << "BEFORE: " << " F" << HEX << this->cpu.registers.getF() << endl;
	this->cpu.registers.printFlags();

	// da code
	if ((this->cpu.registers.getF() & 0b00010000) == 0b00010000) {
		cout << "working as supposed" << endl;
	} else {
		cout << "not working as supposed" << endl;
	}
	//
}

void GameBoy::Signed8bitTo16bit() {
	unsigned short z1 = 0x02f0;
	Byte  z2 = 0xfb;
	cout << "z1 = " << z1 << " z2 = (u)" << (int)z2 << " (s) " << (int)((signed char)z2) << endl;
	cout << "conversion: " << HEX16 << (short)((signed char)z2) << endl;
	cout << "sum: " << HEX16 << cpu.add16bitAdrSign(z1, z2) << endl;
	cout << "expected result: 0x02ed - 2 (+1 for load8bit +1 for inc pc" << endl;
}

int checkInfiniteLoop = 0;

void GameBoy::Debug_InputAndLog(SDL_Event &windowEvent) {
	int c, cyclesInstruction, delaytime = 1000 / 60;
	
	ofstream logFile;
	logFile.open("logFile.log");

	char key; bool keyEn = true, keyHardEn = false;
	char game = 'N'; // 'M' == MINESWEEPER

	cout << "You can quit by pressing 'r'" << endl << endl;

	while (this->cpu.getRunning()) {
		cyclesInstruction = 0;
		bool printVRAMAfterInstruction = false;

		while (cyclesInstruction < cpu.MAXCYCLES) {
			if (SDL_PollEvent(&windowEvent)) {
				if (windowEvent.type == SDL_QUIT) {
					return;
				}
			}

			if (this->cpu.registers.getPC() == 0x0100 && cpu.getEnableBootstrap()) {
				cout << "switch back rom 0x0000 - 0x0100" << endl;
				this->cpu.rom.dltBootstrap(&cpu.ram);
				cpu.setEnableBootstrap(false);
			}

			if (checkInfiniteLoop == 0) {
				keyEn = false;
			}

			if (this->cpu.registers.getPC() == 0x0038) {
				keyHardEn = true;
			}

			PrintRegistersFile(logFile);
			if (checkInfiniteLoop >= 200000) { //82 //0x1144x
				cout << "hard key is enabled" << endl;
				cout << "counter: " << dec << checkInfiniteLoop << endl;
				//cout << HEX << (int)this->cpu.ram.getMemory(0xff44) << endl;
				keyHardEn = true;
			}

			if (keyEn || keyHardEn) {
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
				} else if (key == 's') {
					cpu.rom.print(&cpu.ram, 0x8800, 0x8850);
				}

				PrintRegisters();
				cout << "counter: " << checkInfiniteLoop << endl;
				PrintRegistersFile(logFile);
			}

			c = cpu.CPUstep();
			cyclesInstruction += c;
			cpu.UpdateTimers(c);
			gpu.UpdateGraphics(c);
			cpu.DoInterupts();

			checkInfiniteLoop++;
		}
		gpu.render();
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
	case 10:
		OP_DA_Test();
		break;
	case 11:
		Signed8bitTo16bit();
		break;
	}
}

void GameBoy::run() {
	SDL_Event windowEvent;

	int c, cyclesInstruction;
	int delaytime = 1000 / 60;

	clock_t starttime;

	while (this->cpu.getRunning()) {

		cyclesInstruction = 0;
		starttime = clock();

		while (cyclesInstruction < cpu.MAXCYCLES) {
			if (SDL_PollEvent(&windowEvent)) {
				switch (windowEvent.type) {
					case SDL_QUIT:
						return;
					case SDL_KEYDOWN:
						switch (windowEvent.key.keysym.sym) {
							case SDLK_a:
								cout << "a" << endl;
								this->joypad.KeyPressed(4);
								break;
							case SDLK_s:
								this->joypad.KeyPressed(5);
								break;
							case SDLK_RETURN:
								this->joypad.KeyPressed(7);
								break;
							case SDLK_SPACE:
								this->joypad.KeyPressed(6);
								break;
							case SDLK_RIGHT:	// right
								cout << "right pressed" << endl;
								this->joypad.KeyPressed(0);
								break;
							case SDLK_LEFT:		// left
								cout << "left pressed" << endl;
								this->joypad.KeyPressed(1);
								break;
							case SDLK_UP:		// up
								this->joypad.KeyPressed(2);
								break;
							case SDLK_DOWN:		// down
								this->joypad.KeyPressed(3);
								break;
						}
						break;
					case SDL_KEYUP:
						switch (windowEvent.key.keysym.sym) {
							case SDLK_a:
								cout << "a" << endl;
								this->joypad.KeyReleased(4);
								break;
							case SDLK_s:
								this->joypad.KeyReleased(5);
								break;
							case SDLK_RETURN:
								this->joypad.KeyReleased(7);
								break;
							case SDLK_SPACE:
								this->joypad.KeyReleased(6);
								break;
							case SDLK_RIGHT:	// right
								cout << "right released" << endl;
								this->joypad.KeyReleased(0);
								break;
							case SDLK_LEFT:		// left
								cout << "left released" << endl;
								this->joypad.KeyReleased(1);
								break;
							case SDLK_UP:		// up
								this->joypad.KeyReleased(2);
								break;
							case SDLK_DOWN:		// down
								this->joypad.KeyReleased(3);
								break;
							}
						break;
					
				}
			}

			if (this->cpu.registers.getPC() == 0x0100 && cpu.getEnableBootstrap()) {
				cout << "switch back rom 0x0000 - 0x0100" << endl;
				this->cpu.rom.dltBootstrap(&cpu.ram);
				cpu.setEnableBootstrap(false);
			}

			c = cpu.CPUstep();
			cyclesInstruction += c;
			cpu.UpdateTimers(c);
			gpu.UpdateGraphics(c);
			cpu.DoInterupts();

		}

		gpu.render();
		delay(delaytime - (int)difftime(clock(), starttime));
		//cout << "timediff: " << difftime(clock(), starttime);
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
// MODE 10		opcode DA test
// MODE 11		signed 8 bit to signed 16 bit

int main(int argc, char *argv[]) {
	cout << "You are running Felix Weichselgartner's GameBoy-Classic-Emulator." << endl;
    class GameBoy gameboy;
	if (MODE) {
		gameboy.tests(MODE);
	} else {
		gameboy.run();
	}
	cout << "Gameboy-Classic-Emulator closed." << endl;
    return 0;
}