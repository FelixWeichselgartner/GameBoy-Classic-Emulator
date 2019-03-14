//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
#include "../include/GPU.hpp"
#include "../include/CPU.hpp"

#include <SDL.h>
#include <iostream>
using namespace std;
//----------------------------------------------------------------------------------------------

GPU::GPU(class CPU* cpuLink) {
	this->cpuLink = cpuLink;
	this->ramLink = &cpuLink->ram;
	windowName = this->cpuLink->rom.getGameName(this->ramLink);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "[Error] SDL coult not be initialised! SDL Error: " << SDL_GetError() << endl;
	}

	char* name = new char[windowName.length()];
	for (int i = 0; i < windowName.length(); i++) {
		name[i] = windowName.at(i);
	}

	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, X_RES * 2, Y_RES * 2, SDL_WINDOW_ALLOW_HIGHDPI);

	if (window == NULL) {
		cout << "[Error] window couldn't be created! Error: " << SDL_GetError() << endl;
	}

}

void GPU::SetLCDStatus() {

}

void GPU::UpdateGraphics() {

	ramLink->setMemory(0xFF40, ramLink->getMemory(0xFF40) + 1);
	Byte currentline = ramLink->getMemory(0xFF44);

	if (currentline == X_RES) {
		//RequestInterupt(0);
	}
	else if (currentline > 153) {
		ramLink->setMemory(0xFF40, 0);
	}
	else if (currentline < X_RES) {
		//DrawScanLine();
	}
}