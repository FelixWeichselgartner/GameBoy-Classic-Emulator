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
	for (unsigned int i = 0; i < windowName.length(); i++) {
		name[i] = windowName.at(i);
	}

	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_ALLOW_HIGHDPI);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

	if (window == NULL || renderer == NULL) {
		cout << "[Error] window couldn't be created! Error: " << SDL_GetError() << endl;
	}

	SDL_RenderClear(renderer);
}

void GPU::SetLCDStatus() {

}

void GPU::TestGraphics() {
	SDL_RenderSetScale(renderer, scaleWidth, scaleHeight);
	SDL_SetRenderDrawColor(renderer, dark_grey);
	for (int y = 0; y < Y_RES; y++) {
		for (int x = 0; x < X_RES; x++) {
			SDL_RenderDrawPoint(renderer, x, y);
		}

		if (y == Y_RES / 4) {
			SDL_SetRenderDrawColor(renderer, light_grey);
		} else if (y == Y_RES / 2) {
			SDL_SetRenderDrawColor(renderer, white);
		} else if (y == Y_RES * 3 / 4) {
			SDL_SetRenderDrawColor(renderer, black);
		}

	}
	SDL_RenderPresent(renderer);
}

void GPU::RenderBackground() {
	unsigned short tileData = 0, backgroundMemory = 0;
	bool noSign = true, usingWindow = false;

	Byte scrollY = ramLink->getMemory(0xFF42);
	Byte scrollX = ramLink->getMemory(0xFF43);
	Byte windowY = ramLink->getMemory(0xFF4A);
	Byte windowX = ramLink->getMemory(0xFF4B) - 7;

	Byte yPos = 0, xPos;

	// checks if the window is enabled.
	if (testBit(lcdControl, 5)) {
		if (windowY <= ramLink->getMemory(0xFF44)) {
			usingWindow = true;
		}
	}

	if (testBit(lcdControl, 4)) {
		tileData = 0x8000;
	} else {
		tileData = 0x8800;
		noSign = false;
	}

	if (!usingWindow) {
		if (testBit(lcdControl, 3)) {
			backgroundMemory = 0x9C00;
		} else {
			backgroundMemory = 0x9800;
		}
		yPos = scrollY + ramLink->getMemory(0xFF44);
	} else {
		if (testBit(ledControl, 6)) {
			backgroundMemory = 0x9C00;
		} else {
			backgroundMemory = 0x9800;
		}
		yPos = ramLink->getMemory(0xFF44) - windowY;
	}

	Byte line, data1, data2;
	unsigned short tileRow = ((Byte) yPos / 8) * 32;
	unsigned short tileCol, tileAddress, tileLocation;
	signed short tileNum;
	int colorBit, colorNum;

	for (int i = 0; i < X_RES; i++) {
		xPos = i + scrollX;

		if (usingWindow) {
			xPos = i - windowX;
		}

		tileCol = xPos / 8;
		tileAddress = backgroundMemory + tileRow + tileCol;
		tileLocation = tileData;

		if (noSign) {
			tileNum = (Byte)ramLink->getMemory(tileAddress);
			tileLocation += tileNum * 16;
		} else {
			tileNum = (signed char)ramLink->getMemory(tileAddress);
			tileLocation += (tileNum + 128) * 16;
		}

		line = (yPos % 8) * 2;
		data1 = ramLink->getMemory(tileLocation + line);
		data2 = ramLink->getMemory(tileLocation + line + 1);

		colorBit = (((xPos % 8) - 7) * -1);
	}
}

void GPU::UpdateGraphics() {
	SDL_RenderSetScale(renderer, scaleWidth, scaleHeight);

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

	SDL_RenderPresent(renderer);
}