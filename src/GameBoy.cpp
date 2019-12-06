//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"

#include <iostream>
#include <iomanip>
using namespace std;
//----------------------------------------------------------------------------------------------

void GameBoy::reset() {
    this->cpu.reset();
    this->gpu->reset();
    this->joypad.reset();
}

GameBoy::GameBoy(char * inputFile) {
	if (inputFile != NULL) {
		this->cpu.loadROM(inputFile);
		this->gpu = new GPU(&this->cpu);
		if (&joypad != NULL)
			cpu.setJoypadLink(&joypad);
		else
			exit(2);
	} else {
		// usage
		cout << "specify a valid input file." << endl;
		exit(3);
	}
}

GameBoy::~GameBoy() {
	delete this->gpu;
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
						gpu->screenshot();
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

	int c, cyclesInstruction = cpu.MAXCYCLES;
	bool screen = false;

	while (this->cpu.getRunning()) {
		cyclesInstruction -= cpu.MAXCYCLES;

		while (cyclesInstruction < cpu.MAXCYCLES) {

			if (SDL_PollEvent(&windowEvent)) {
				if (winEvent(windowEvent, screen)) {
					cpu.setRunning(false);
				}
			}

			if (this->cpu.registers.getPC() == 0x0100) {
				this->cpu.memory.setEnableBootstrap(false);
			}

			c = cpu.CPUstep();
			cyclesInstruction += c;
			cpu.UpdateTimers(c);
			cpu.memory.sdt.update();
			gpu->UpdateGraphics(c);
			c += cpu.DoInterrupts();
		}

		gpu->render();
	}

	SDL_DestroyRenderer(gpu->getRenderer());
	SDL_DestroyWindow(gpu->getWindow());
	SDL_Quit();
}


#define MODE 0
// MODE 0		normal mode
// MODE 1		input -> next instruction and save in log file
// MODE 2		show Nintendo Logo
// MODE 3		prints lowest rom bank to console

int main(int argc, char *argv[]) {
	cout << "You are running Felix Weichselgartner's GameBoy-Classic-Emulator." << endl;
    //class GameBoy gameboy(argv[1]);
	char game[] = "mbc1/bits_ram_en.gb";
    class GameBoy gameboy(game);
	(MODE) ? gameboy.tests(MODE) : gameboy.run();
	cout << "Gameboy-Classic-Emulator closed." << endl;
    return 0;
}