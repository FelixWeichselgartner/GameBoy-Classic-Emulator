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

void GameBoy::PrintRegisters() {
	cout << "pc:" << HEX16 << this->cpu.registers.getPC() << " ";
	cout << "op:" << HEX << (int)this->cpu.memory.ReadByte(this->cpu.registers.getPC()) << " ";
	cout << "af:" << HEX16 << this->cpu.registers.getAF() << " ";
	cout << "bc:" << HEX16 << this->cpu.registers.getBC() << " ";
	cout << "de:" << HEX16 << this->cpu.registers.getDE() << " ";
	cout << "hl:" << HEX16 << this->cpu.registers.getHL() << " ";
	cout << "sp:" << HEX16 << this->cpu.registers.getSP() << " ";
	cout << "ly:" << HEX << (int)this->cpu.memory.ReadByte(0xff44) << " ";
	cout << "ir:" << HEX << (int)this->cpu.memory.ReadByte(0xff0f) << " ";
	cout << "ie:" << HEX << (int)this->cpu.memory.ReadByte(0xffff) << endl;
}

void GameBoy::PrintRegistersFile(ofstream &file) {
	file << "pc:" << HEX16 << this->cpu.registers.getPC() << " ";
	file << "op:" << HEX << (int)this->cpu.memory.ReadByte(this->cpu.registers.getPC()) << " ";
	file << "af:" << HEX16 << this->cpu.registers.getAF() << " ";
	file << "bc:" << HEX16 << this->cpu.registers.getBC() << " ";
	file << "de:" << HEX16 << this->cpu.registers.getDE() << " ";
	file << "hl:" << HEX16 << this->cpu.registers.getHL() << " ";
	file << "sp:" << HEX16 << this->cpu.registers.getSP() << " ";
	file << "ly:" << HEX << (int)this->cpu.memory.ReadByte(0xff44) << " ";
	file << "ir:" << HEX << (int)this->cpu.memory.ReadByte(0xff0f) << " ";
	file << "ie:" << HEX << (int)this->cpu.memory.ReadByte(0xffff) << " ";
	file << "tc:" << HEX << (int)this->cpu.memory.ReadByte(0xff05) << " ";
	file << "tcn:" << HEX << (int)this->cpu.memory.ReadByte(0xff07) << " ";
	file << "c:" << dec << this->cpu.timer.getTimerCounter() << endl;
	//file << "cp:" << HEX << (int)this->cpu.memory.ReadByte(0xa000) << " ";
	//file << "rbn:" << HEX << (int)this->cpu.memory.mbc->getCurrentRomBank() << " ";
	//file << "ran:" << HEX << (int)this->cpu.memory.ram.getCurrentRamBank() << endl;
	//file << "mben:" << boolalpha << this->cpu.memory.rom.RomBanking << " ";
	//file << "rben:" << boolalpha << this->cpu.memory.ram.EnableRamBanking << endl;
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
	char game = cpu.memory.rom.getGameName().at(0);
	cout << "GAME: " << cpu.memory.rom.getGameName() << " == " << game << endl;
	bool skip = false;
	int skipCounter = 1;
	game = ' ';

	cout << "ram enable: " << boolalpha << this->cpu.memory.ram.getRamEnable() << endl;

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

					//cout << "current rom bank: " << (int)this->cpu.memory.rom.getCurrentRomBank() << endl;
					//cout << "0x4000: " << HEX << (int)cpu.memory.ReadByte(0x4000) << endl;

					cpu.memory.print(0xdff0, 0xe000);

					if (printVRAMAfterInstruction) {
						cpu.memory.print(0x8000, 0xA000);
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
						cpu.memory.print(0x8800, 0x8850);
					}
					else if (key == 'f') {
						cpu.memory.print(0xff00, 0xffff);
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
			cpu.memory.sdt.update();
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
	case 3:
		this->cpu.memory.print(0x0000, 0x2000);
		break;
	}
}