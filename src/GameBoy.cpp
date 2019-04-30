//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"

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
	file << "pc:" << HEX16 << this->cpu.registers.getPC() << " ";
	file << "op:" << HEX << (int)this->cpu.ReadByte(this->cpu.registers.getPC()) << " ";
	file << "af:" << HEX16 << this->cpu.registers.getAF() << " ";
	file << "bc:" << HEX16 << this->cpu.registers.getBC() << " ";
	file << "de:" << HEX16 << this->cpu.registers.getDE() << " ";
	file << "hl:" << HEX16 << this->cpu.registers.getHL() << " ";
	file << "sp:" << HEX16 << this->cpu.registers.getSP() << " ";
	file << "ly:" << HEX << (int)this->cpu.ReadByte(0xff44) << " ";
	file << "ir:" << HEX << (int)this->cpu.ReadByte(0xff0f) << " ";
	file << "ie:" << HEX << (int)this->cpu.ReadByte(0xffff) << " ";
	file << "ff80:" << HEX << (int)this->cpu.ReadByte(0xff80) << endl;
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

void GameBoy::Debug_InputAndLog(SDL_Event &windowEvent) {
	int c, cyclesInstruction;
	int delaytime = 1000 / 60;
	unsigned long long counter = 0;
	bool screen = false;

	ofstream logFile;
	logFile.open("logFile.log");

	bool printVRAMAfterInstruction = false, keyEn = false, keyHardEn = false;
	char key;
	char game = cpu.rom.getGameName(&cpu.ram).at(0);
	cout << "GAME: " << cpu.rom.getGameName(&cpu.ram) << " == " << game << endl;
	bool skip = false;
	int skipCounter = 1;
	game = ' ';
	Byte oldRomBank = 0x01, newRomBank;

	cout << "ram enable: " << boolalpha << this->cpu.ram.getRamEnable() << endl;

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
						this->joypad.KeyPressed(JOY_A);
						break;
					case SDLK_s:
						this->joypad.KeyPressed(JOY_B);
						break;
					case SDLK_RETURN:
						this->joypad.KeyPressed(JOY_START);
						break;
					case SDLK_SPACE:
						this->joypad.KeyPressed(JOY_SELECT);
						break;
					case SDLK_RIGHT:
						this->joypad.KeyPressed(JOY_RIGHT);
						break;
					case SDLK_LEFT:
						this->joypad.KeyPressed(JOY_LEFT);
						break;
					case SDLK_UP:
						this->joypad.KeyPressed(JOY_UP);
						break;
					case SDLK_DOWN:
						this->joypad.KeyPressed(JOY_DOWN);
						break;
					case SDLK_LSHIFT:
						keyEn = true;
						break;
					case SDLK_F6:
						if (!screen)
							gpu.screenshot();
						screen = true;
						break;
					}
					break;
				case SDL_KEYUP:
					switch (windowEvent.key.keysym.sym) {
					case SDLK_a:
						this->joypad.KeyReleased(JOY_A);
						break;
					case SDLK_s:
						this->joypad.KeyReleased(JOY_B);
						break;
					case SDLK_RETURN:
						this->joypad.KeyReleased(JOY_START);
						break;
					case SDLK_SPACE:
						this->joypad.KeyReleased(JOY_SELECT);
						break;
					case SDLK_RIGHT:
						this->joypad.KeyReleased(JOY_RIGHT);
						break;
					case SDLK_LEFT:
						this->joypad.KeyReleased(JOY_LEFT);
						break;
					case SDLK_UP:
						this->joypad.KeyReleased(JOY_UP);
						break;
					case SDLK_DOWN:
						this->joypad.KeyReleased(JOY_DOWN);
						break;
					case SDLK_F6:
						screen = false;
						break;
					}
					break;
				}
			}

			newRomBank = cpu.rom.getCurrentRomBank();

			if (newRomBank != oldRomBank && false) {
				cout << "new rb: " << HEX << (int)this->cpu.rom.getCurrentRomBank() << endl;
				cout << "address: " << HEX16 << this->cpu.registers.getPC() << endl;
			}

			oldRomBank = newRomBank;

			if (this->cpu.registers.getPC() == 0x0100 && cpu.getEnableBootstrap()) {
				cout << "switch back rom 0x0000 - 0x0100" << endl;
				this->cpu.rom.dltBootstrap(&cpu.ram);
				cpu.setEnableBootstrap(false);
				keyEn = true;
			}

			if (skip && (skipCounter % 32 == 0) && skipCounter > 31) {
				cout << "reached" << endl;
				skipCounter = 0;
				keyEn = true;
				skip = false;
			}

			if (this->cpu.registers.getPC() == 0x2349 && this->cpu.registers.getA() != 0 && false) {
				keyEn = true;
			}

			if (counter > 0x90 && false) {
				keyEn = true;
			}

			if (this->cpu.registers.getPC() == 0xc2c7 && false) {
				keyEn = true;
			}
			
			if (!skip) {

				
				/*
				if (this->cpu.registers.getPC() == 0x1b9d) {
					keyEn = true;
				}
				*/

				

				if (counter > 0x1b94a - 150 && game == 'R') {
					// jumps in false rom bank 0x5876
					keyHardEn = true;
				}

				if ((counter > 5000000 || this->cpu.registers.getPC() == 0x1c63) && false) {
					keyEn = true;
				}

				if (keyEn || keyHardEn) {

					//cout << "current rom bank: " << (int)this->cpu.rom.getCurrentRomBank() << endl;
					//cout << "0x4000: " << HEX << (int)cpu.ReadByte(0x4000) << endl;

					cpu.rom.print(&cpu.ram, 0xdff0, 0xe000);

					if (printVRAMAfterInstruction) {
						cpu.rom.print(&cpu.ram, 0x8000, 0xA000);
						printVRAMAfterInstruction = false;
					}

					key = cin.get();
					if (key == 'r') {
						cpu.setRunning(false);
						break;
					}
					else if (key == 'p') {
						printVRAMAfterInstruction = true;
					}
					else if (key == 's') {
						cpu.rom.print(&cpu.ram, 0x8800, 0x8850);
					}
					else if (key == 'f') {
						cpu.rom.print(&cpu.ram, 0xff00, 0xffff);
					}
					else if (key == 'k') {
						// skip 32 instructions
						cout << "skipping" << endl;
						keyEn = false;
						skip = true;
					}
					else if (key == 'd') {
						keyEn = false;
					}

					PrintRegisters();
					cout << "counter: " << counter << endl;
					//PrintRegistersFile(logFile);
				}
			} else {
				skipCounter++;
				//cout << skip << endl;
			}
			
			PrintRegistersFile(logFile);
			c = cpu.CPUstep();
			cyclesInstruction += c;
			cpu.UpdateTimers(c);
			gpu.UpdateGraphics(c);
			cpu.DoInterupts();
			counter++;

		}

		gpu.render();
	}

	SDL_DestroyRenderer(gpu.getRenderer());
	SDL_DestroyWindow(gpu.getWindow());
	SDL_Quit();
}

void GameBoy::tests(int mode) {
	SDL_Event windowEvent;
	bool running = true;
	
	switch (mode) {
	case 1:
		Debug_InputAndLog(windowEvent);
		break;
	case 2:
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

bool GameBoy::winEvent(SDL_Event &windowEvent, bool &screen) {
	switch (windowEvent.type) {
		case SDL_QUIT:
			return true;
		case SDL_KEYDOWN:
			switch (windowEvent.key.keysym.sym) {
				case SDLK_a:
					this->joypad.KeyPressed(JOY_A);
					break;
				case SDLK_s:
					this->joypad.KeyPressed(JOY_B);
					break;
				case SDLK_RETURN:
					this->joypad.KeyPressed(JOY_START);
					break;
				case SDLK_SPACE:
					this->joypad.KeyPressed(JOY_SELECT);
					break;
				case SDLK_RIGHT:
					this->joypad.KeyPressed(JOY_RIGHT);
					break;
				case SDLK_LEFT:
					this->joypad.KeyPressed(JOY_LEFT);
					break;
				case SDLK_UP:
					this->joypad.KeyPressed(JOY_UP);
					break;
				case SDLK_DOWN:
					this->joypad.KeyPressed(JOY_DOWN);
					break;
				case SDLK_F6:
					if (!screen)
						gpu.screenshot();
					screen = true;
					break;
			}
			break;
		case SDL_KEYUP:
			switch (windowEvent.key.keysym.sym) {
				case SDLK_a:
					this->joypad.KeyReleased(JOY_A);
					break;
				case SDLK_s:
					this->joypad.KeyReleased(JOY_B);
					break;
				case SDLK_RETURN:
					this->joypad.KeyReleased(JOY_START);
					break;
				case SDLK_SPACE:
					this->joypad.KeyReleased(JOY_SELECT);
					break;
				case SDLK_RIGHT:
					this->joypad.KeyReleased(JOY_RIGHT);
					break;
				case SDLK_LEFT:
					this->joypad.KeyReleased(JOY_LEFT);
					break;
				case SDLK_UP:
					this->joypad.KeyReleased(JOY_UP);
					break;
				case SDLK_DOWN:
					this->joypad.KeyReleased(JOY_DOWN);
					break;
				case SDLK_F6:
					screen = false;
					break;
			}
			break;
	}	

	return false;
}

void GameBoy::run() {
	SDL_Event windowEvent;

	int c, cyclesInstruction;
	int delaytime = 1000 / 60;
	bool screen = false;

	clock_t starttime;

	while (this->cpu.getRunning()) {
		cyclesInstruction = 0;
		starttime = clock();

		while (cyclesInstruction < cpu.MAXCYCLES) {
			if (SDL_PollEvent(&windowEvent)) {
				if (winEvent(windowEvent, screen)) {
					cpu.setRunning(false);
				}
			}

			/*
			if (this->cpu.registers.getPC() == 0x0100 && cpu.getEnableBootstrap()) {
				cout << "switch back rom 0x0000 - 0x0100" << endl;
				this->cpu.rom.dltBootstrap(&cpu.ram);
				cpu.setEnableBootstrap(false);
			}
			*/

			c = cpu.CPUstep();
			cyclesInstruction += c;
			cpu.UpdateTimers(c);
			gpu.UpdateGraphics(c);
			cpu.DoInterupts();
		}

		gpu.render();

		//delay(delaytime - (int)difftime(clock(), starttime));
		//cout << "timediff: " << difftime(clock(), starttime);
	}

	SDL_DestroyRenderer(gpu.getRenderer());
	SDL_DestroyWindow(gpu.getWindow());
	SDL_Quit();
}

#define MODE 0
// MODE 0		normal mode
// MODE 1		input -> next instruction and save in log file
// MODE 2		show Nintendo Logo

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