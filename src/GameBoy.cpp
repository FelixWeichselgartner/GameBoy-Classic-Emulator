//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

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
	if (&joypad != NULL)
		cpu.setJoypadLink(&joypad);
	else
		exit(2);
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

unsigned long long counter = 0;

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
	(MODE) ? gameboy.tests(MODE) : gameboy.run();
	cout << "Gameboy-Classic-Emulator closed." << endl;
    return 0;
}