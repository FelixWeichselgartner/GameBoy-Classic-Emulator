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

bool GPU::IsLCDEnabled() const {
	return testBit(this->cpuLink->ReadByte(0xFF40), 7);
}

void GPU::SetLCDStatus() {
	Byte status = this->cpuLink->ReadByte(0xFF41);
	if (!IsLCDEnabled()) {
		scanLineCounter = 456;
		this->cpuLink->WriteByte(0xFF44, 0x00);
		status = status & 252;
		status = setBit(status, 0);
		this->cpuLink->WriteByte(0xFF41, status);
		return;
	} 

	Byte currentline = this->cpuLink->ReadByte(0xFF44);
	Byte currentmode = status & 0x03;
	Byte mode = 0x00;
	bool reqInt = false;
	
	if (currentline >= 144) {
		mode = 1;
		status = setBit(status, 0);
		status = resetBit(status, 1);
		reqInt = testBit(status, 4);
	} else {
		int mode2bounds = 456 - 80;
		int mode3bounds = mode2bounds - 172;

		if (scanLineCounter >= mode2bounds) {
			mode = 2;
			status = setBit(status, 1);
			status = resetBit(status, 0);
			reqInt = testBit(status, 5);
		} else if (scanLineCounter >= mode3bounds) {
			mode = 3;
			status = setBit(status, 1);
			status = setBit(status, 0);
		} else {
			mode = 0;
			status = resetBit(status, 1);
			status = resetBit(status, 0);
			reqInt = testBit(status, 3);
		}
	}

	if (reqInt && (mode != currentmode)) {
		cpuLink->RequestInterupt(1);
	}

	if (this->cpuLink->ReadByte(0xFF45)) { // conincidence flag
		status = setBit(status, 2);
		if (testBit(status, 6)) {
			this->cpuLink->RequestInterupt(1);
		}
	} else {
		status = resetBit(status, 2);
	}

	this->cpuLink->WriteByte(0xFF41, status);
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

Byte GPU::getColor(Byte colorNum, unsigned short address) {
	Byte retval;
	Byte palette = this->cpuLink->ReadByte(address);
	int high = 0, low = 0;

	if (colorNum >= 0 && colorNum <= 3) {
		high = colorNum * 2 + 1;
		low = colorNum * 2 + 0;
	}

	int color = ((int) testBit(palette, high) << 1) | (int) testBit(palette, low);

	switch (color) {
		case 0: retval = 0; break; // WHITE = 0
		case 1: retval = 1; break; // LIGHT_GREY = 1
		case 2: retval = 2; break; // DARK_GREY = 2
		case 3: retval = 3; break; // BLACK = 3
		default: retval = NULL;  break;
	}

	return retval;
}

void GPU::RenderTiles(Byte lcdControl) {
	unsigned short tileData = 0, backgroundMemory = 0;
	bool noSign = true, usingWindow = false;

	Byte scrollY = this->cpuLink->ReadByte(0xFF42);
	Byte scrollX = this->cpuLink->ReadByte(0xFF43);
	Byte windowY = this->cpuLink->ReadByte(0xFF4A);
	Byte windowX = this->cpuLink->ReadByte(0xFF4B) - 7;

	Byte yPos = 0, xPos;

	// checks if the window is enabled.
	if (testBit(lcdControl, 5)) {
		if (windowY <= this->cpuLink->ReadByte(0xFF44)) {
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
		yPos = scrollY + this->cpuLink->ReadByte(0xFF44);
	} else {
		if (testBit(lcdControl, 6)) {
			backgroundMemory = 0x9C00;
		} else {
			backgroundMemory = 0x9800;
		}
		yPos = this->cpuLink->ReadByte(0xFF44) - windowY;
	}

	Byte line, data1, data2;
	unsigned short tileRow = ((Byte)yPos / 8) * 32;
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
			tileNum = (Byte)this->cpuLink->ReadByte(tileAddress);
			tileLocation += tileNum * 16;
		}
		else {
			tileNum = (signed char)this->cpuLink->ReadByte(tileAddress);
			tileLocation += (tileNum + 128) * 16;
		}

		line = (yPos % 8) * 2;
		data1 = this->cpuLink->ReadByte(tileLocation + line);
		data2 = this->cpuLink->ReadByte(tileLocation + line + 1);

		colorBit = (((xPos % 8) - 7) * -1);
		colorNum = ((int)testBit(data2, colorBit) << 1) | (int)testBit(data1, colorBit);

		Byte color = getColor(colorNum, 0xFF47);

		int y = this->cpuLink->ReadByte(0xFF44);

		if ((y < 0) || (y > Y_RES - 1) || (i < 0) || (i > X_RES - 1)) {
			return;
		} else {
			display[y][i] = color;
		}
	}
}

void GPU::RenderSprites(Byte lcdControl) {
	bool use8x16tiles = false;
	if (testBit(lcdControl, 2)) {
		use8x16tiles = true;
	}

	const int maxSprites = 40, bytesPerSprite = 4;
	for (int sprite = 0; sprite < maxSprites; sprite++) {
		Byte index = sprite * bytesPerSprite;
		Byte xPos = this->cpuLink->ReadByte(ADDR_OAM + index + 0);
		Byte yPos = this->cpuLink->ReadByte(ADDR_OAM + index + 1);
		Byte tileLocation = this->cpuLink->ReadByte(ADDR_OAM + index + 2);
		Byte attributes = this->cpuLink->ReadByte(ADDR_OAM + index + 3);

		bool flipXaxis = testBit(attributes, 5);
		bool flipYaxis = testBit(attributes, 6);

		int scanline = this->cpuLink->ReadByte(0xFF44);

		int ysize;
		if (use8x16tiles) {
			ysize = 16;
		}
		else {
			ysize = 8;
		}

		if ((scanline >= yPos) && (scanline < (yPos + ysize))) {
			int line = scanline - yPos;

			if (flipYaxis) {
				line = -(line - ysize);
			}

			line *= 2;

			unsigned short address = (ADDR_VRAM_T_S + (tileLocation * 16)) + line;
			Byte data1 = this->cpuLink->ReadByte(address);
			Byte data2 = this->cpuLink->ReadByte(address + 1);

			for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
				int colorBit = tilePixel;

				if (flipXaxis) {
					colorBit = -(colorBit - 7);
				}

				int colorNum = ((int)testBit(data2, colorBit) << 1) | (int)testBit(data1, colorBit);

				unsigned short colorAddress = testBit(attributes, 4) ? 0xFF49 : 0xFF48;
				int color = getColor(colorNum, colorAddress);

				if (color == 0) { // transparant
					continue;
				}

				int xPixel = 0 - tilePixel + 7;
				int pixel = xPixel + xPos;

				if ((scanline < 0) || (scanline > X_RES - 1) || (pixel < 0) || (pixel > Y_RES - 1)) {
					return;
				}

				display[scanline][pixel] = color;
			}
		}
	}
}

void GPU::DrawScanLine() {
	Byte control = this->cpuLink->ReadByte(0xFF40);

	if (testBit(control, 0)) {
		RenderTiles(control);
	}

	if (testBit(control, 1)) {
		RenderSprites(control);
	}
}

void GPU::UpdateGraphics() {
	SDL_RenderSetScale(renderer, scaleWidth, scaleHeight);

	this->cpuLink->WriteByte(0xFF40, this->cpuLink->ReadByte(0xFF40) + 1);
	Byte currentline = this->cpuLink->ReadByte(0xFF44);

	// error: currentline no value;
	cout << "currentline: " << currentline << endl;

	if (currentline == X_RES) {
		cpuLink->RequestInterupt(0);
	} else if (currentline > 153) {
		this->cpuLink->WriteByte(0xFF40, 0);
	} else if (currentline < X_RES) {
		//cout << "scanLine has been drawn" << endl;
		DrawScanLine();
	}

	for (int i = 0; i < X_RES; i++) {
		switch (display[currentline][i]) {
			case 0: 
				SDL_SetRenderDrawColor(renderer, white); 
				break; 
			case 1: SDL_SetRenderDrawColor(renderer, light_grey); 
				break;
			case 2: SDL_SetRenderDrawColor(renderer, dark_grey); 
				break;
			case 3: SDL_SetRenderDrawColor(renderer, black); 
				break;
			default: break;
		}
		SDL_RenderDrawPoint(renderer, i, currentline);
	}

	SDL_RenderPresent(renderer);
}