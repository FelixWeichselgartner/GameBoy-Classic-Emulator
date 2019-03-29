//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"
#include "../include/RAM.hpp"
#include "../include/GPU.hpp"
#include "../include/CPU.hpp"

#include "../lib/SDL2/include/SDL2/SDL.h"
#include <iostream>
#include <iomanip>
using namespace std;
//----------------------------------------------------------------------------------------------

GPU::GPU(class CPU* cpuLink) {
	this->cpuLink = cpuLink;
	this->ramLink = &cpuLink->ram;
	windowName = this->cpuLink->rom.getGameName(this->ramLink);
	ScanLineCounter = 456;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "[Error] SDL coult not be initialised! SDL Error: " << SDL_GetError() << endl;
	}

	this->window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_ALLOW_HIGHDPI);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

	if (window == NULL || renderer == NULL) {
		cout << "[Error] window couldn't be created! Error: " << SDL_GetError() << endl;
	}

	SDL_RenderClear(renderer);

	clearScreen();

	return;
}

void GPU::clearScreen() {
	SDL_RenderSetScale(renderer, (float)scaleWidth, (float)scaleHeight);

	for (int x = 0; x < X_RES; x++) {
		for (int y = 0; y < Y_RES; y++) {
			display[y][x] = 0;
			SDL_SetRenderDrawColor(renderer, white);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}

	SDL_RenderPresent(renderer);

	return;
}

bool GPU::IsLCDEnabled() const {
	return testBit(this->cpuLink->ReadByte(0xFF40), 7);
}

void GPU::SetLCDStatus() {
	Byte status = this->cpuLink->ReadByte(0xFF41);

	if (!IsLCDEnabled()) {
		ScanLineCounter = 456;
		this->cpuLink->WriteByte(0xFF44, 0x00);
		status &= 252;
		status = setBit(status, 0);
		this->cpuLink->WriteByte(0xFF41, status);
		return;
	} 

	Byte currentline = this->cpuLink->ReadByte(0xFF44);
	Byte currentmode = status & 0x03;
	Byte mode = 0x00;
	bool reqInt = false;
	
	if (currentline >= Y_RES) {
		mode = 1;
		status = setBit(status, 0);
		status = resetBit(status, 1);
		reqInt = testBit(status, 4);
	} else {
		int mode2bounds = 456 - 80;
		int mode3bounds = mode2bounds - 172;

		if (ScanLineCounter >= mode2bounds) {
			mode = 2;
			status = setBit(status, 1);
			status = resetBit(status, 0);
			reqInt = testBit(status, 5);
		} else if (ScanLineCounter >= mode3bounds) {
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

	if (currentline == this->cpuLink->ReadByte(0xFF45)) { // conincidence flag
		status = setBit(status, 2);
		if (testBit(status, 6)) {
			this->cpuLink->RequestInterupt(1);
		}
	} else {
		status = resetBit(status, 2);
	}

	this->cpuLink->WriteByte(0xFF41, status);

	return;
}

void GPU::TestGraphics() {
	SDL_RenderSetScale(renderer, (float) scaleWidth, (float) scaleHeight);
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

	return;
}

void GPU::TestTiles() {
	SDL_RenderSetScale(renderer, (float)scaleWidth, (float)scaleHeight);
	SDL_SetRenderDrawColor(renderer, dark_grey);

	for (int y = 0; y < Y_RES; y++) {
		for (int x = 0; x < X_RES; x++) {
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}

	SDL_RenderPresent(renderer);

	return;
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

void GPU::RenderNintendoLogo() {
	unsigned short address = 0x0104;
	unsigned short end = 0x0133;
	int x1, y1, x2, y2, c1, c2;
	Byte data1, data2;

	for (int i = address, c = 0; i < end; i += 2, c++) {
		data1 = this->cpuLink->ReadByte(i);
		data2 = this->cpuLink->ReadByte(i + 1);

		for (int k = 7, t = 0; k >= 0; k--, t++) {
			x1 = (c % 12) * 4 + t % 4;
			y1 = (c / 12) * 4 + t / 4;
			c1 = testBit(data1, k);
			display[y1][x1] = c1;
			x2 = x1;
			y2 = y1 + 2;
			c2 = testBit(data2, k);
			display[y2][x2] = c2;
		}
	}

	return;
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
	} else {
		usingWindow = false;
	}

	if (testBit(lcdControl, 4)) {
		tileData = ADDR_VRAM_T_S;
	} else {
		tileData = 0x8800;
		noSign = false;
	}

	if (!usingWindow) {
		if (testBit(lcdControl, 3)) {
			backgroundMemory = ADDR_VRAM_T_M_2;
		} else {
			backgroundMemory = ADDR_VRAM_T_M_1;
		}
		yPos = scrollY + this->cpuLink->ReadByte(0xFF44);
	} else {
		if (testBit(lcdControl, 6)) {
			backgroundMemory = ADDR_VRAM_T_M_2;
		} else {
			backgroundMemory = ADDR_VRAM_T_M_1;
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
			if (i >= windowX) {
				xPos = i - windowX;
			}
		}

		tileCol = xPos / 8;
		tileAddress = backgroundMemory + tileRow + tileCol;
		tileLocation = tileData;

		if (noSign) {
			tileNum = (Byte)this->cpuLink->ReadByte(tileAddress);
			tileLocation += tileNum * 16;
		} else {
			tileNum = (signed char)this->cpuLink->ReadByte(tileAddress);
			tileLocation += (tileNum + 128) * 16;
		}

		line = (yPos % 8) * 2;
		data1 = this->cpuLink->ReadByte(tileLocation + line);
		data2 = this->cpuLink->ReadByte(tileLocation + line + 1);
		//cout << HEX << data1 << HEX << data2;

		colorBit = -((xPos % 8) - 7);
		colorNum = ((int)testBit(data2, colorBit) << 1) | (int)testBit(data1, colorBit);

		Byte color = getColor(colorNum, 0xFF47);

		/*
		cout << " " << "color: " << HEX << (int)color;
		cout << " " << "colorBit: " << HEX << (int)colorBit;
		cout << " " << "colorNum: " << HEX << (int)colorNum;
		cout << " " << "(0xFF47): " << HEX << (int)this->cpuLink->ReadByte(0xFF47);
		*/

		int y = this->cpuLink->ReadByte(0xFF44);

		if ((y < 0) || (y > Y_RES - 1) || (i < 0) || (i > X_RES - 1)) {
			return;
		} else {
			display[y][i] = color;
		}
		//cout << endl;
	}

	return;
}

void GPU::RenderSprites(Byte lcdControl) {
	if (testBit(lcdControl, 1)) {
		bool use8x16tiles = false;
		if (testBit(lcdControl, 2)) {
			use8x16tiles = true;
		}

		const int maxSprites = 40, bytesPerSprite = 4;
		for (int sprite = 0; sprite < maxSprites; sprite++) {
			Byte index = sprite * bytesPerSprite;
			Byte yPos = this->cpuLink->ReadByte(ADDR_OAM + index + 0) - 16;
			Byte xPos = this->cpuLink->ReadByte(ADDR_OAM + index + 1) - 8;
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
						continue;
					}

					if (testBit(attributes, 7)) {

					}

					display[scanline][pixel] = color;
				}
			}
		}
	}

	return;
}

void GPU::DrawScanLine() {
	Byte control = this->cpuLink->ReadByte(0xFF40);

	if (testBit(control, 0)) {
		RenderTiles(control);
	}

	if (testBit(control, 1)) {
		RenderSprites(control);
	}

	return;
}

void GPU::renderDisplay(Byte currentline) {
	SDL_RenderSetScale(renderer, (float)scaleWidth, (float)scaleHeight);

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

	return;
}

void GPU::UpdateGraphics(int cycles) {
	Byte currentline;

	SetLCDStatus();

	if (IsLCDEnabled()) {
		ScanLineCounter -= cycles;

		if (ScanLineCounter <= 0) {
			this->cpuLink->WriteByte(0xFF44, this->cpuLink->ReadByte(0xFF44) + 1);
			//this->cpuLink->WriteByte(0xFF40, this->cpuLink->ReadByte(0xFF40) + 1);

			ScanLineCounter = 456;

			currentline = this->cpuLink->ReadByte(0xFF44);
			//cout << "currentline: " << (int)currentline << endl;

			// error: currentline no value;
			// cout << "currentline: " << (int) currentline << endl;

			if (currentline == Y_RES) {
				cpuLink->RequestInterupt(0);
			} else if (currentline > 153) {
				this->cpuLink->WriteByte(0xFF44, 0);
			} else if (currentline < Y_RES) {
				DrawScanLine();
				renderDisplay(currentline);
			}
		}

		return;
	} else {
		//cout << "lcd is disabled " << endl;
		return;
	}
}

void GPU::render() {
	SDL_RenderPresent(renderer);

	return;
}