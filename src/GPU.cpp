//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"
#include "../include/RAM.hpp"
#include "../include/GPU.hpp"
#include "../include/CPU.hpp"
#include "../include/format.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
//----------------------------------------------------------------------------------------------

GPU::GPU(class CPU* cpuLink) {
	this->cpuLink = cpuLink;
	this->ramLink = &cpuLink->ram;
	windowName = this->cpuLink->rom.getGameName(this->ramLink);
	ScanLineCounter = 0;
	this->GpuMode = GPU_MODE_HBLANK;

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
	Byte currentline = this->cpuLink->ReadByte(0xFF44);
	Byte currentmode = status & 0x03, mode = 0x00;
	bool reqInt = false;
	int mode2bounds, mode3bounds;

	if (!IsLCDEnabled()) {
		ScanLineCounter = 456;
		this->cpuLink->ram.setMemory(0xFF44, 0x00);
		status = setBit(status & 0b11111100, 0);
		this->cpuLink->WriteByte(0xFF41, status);
		return;
	} 
	
	if (currentline >= Y_RES) {
		mode = 1;
		status = setBit(status, 0);
		status = resetBit(status, 1);
		reqInt = testBit(status, 4);
	} else {
		mode2bounds = 456 - 80;
		mode3bounds = mode2bounds - 172;

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

Byte GPU::getColor(Byte colorNum, Word address) {
	Byte palette = this->cpuLink->ReadByte(address);
	int high = 0, low = 0, color;

	if (colorNum >= 0 && colorNum <= 3) {
		high = colorNum * 2 + 1;
		low = colorNum * 2 + 0;
	}

	color = ((int) testBit(palette, high) << 1) | (int) testBit(palette, low);

	return color;
}

void GPU::RenderNintendoLogo() {
	Word address = 0x0104;
	Word end = 0x0133;
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
	Byte windowY, windowX, scrollY, scrollX, yPos, xPos, color;
	Byte line, data1, data2;
	signed short tileNum;
	Word tileData = 0, backgroundMemory = 0;
	Word tileAddress, tileLocation, tileRow;
	bool noSign = true, usingWindow;
	int colorBit, colorNum, y;

	scrollY = this->cpuLink->ReadByte(0xFF42);
	scrollX = this->cpuLink->ReadByte(0xFF43);
	windowY = this->cpuLink->ReadByte(0xFF4A);
	windowX = this->cpuLink->ReadByte(0xFF4B) - 7;
	yPos = 0;

	// checks if the window is enabled.
	usingWindow = (testBit(lcdControl, 5) && (windowY <= this->cpuLink->ReadByte(0xFF44)));

	if (testBit(lcdControl, 4)) {
		tileData = ADDR_VRAM_T_S;
	} else {
		tileData = 0x8800;
		noSign = false;
	}

	if (usingWindow) {
		backgroundMemory = testBit(lcdControl, 6) ? ADDR_VRAM_T_M_2 : ADDR_VRAM_T_M_1;
		yPos = this->cpuLink->ReadByte(0xFF44) - windowY;
	} else {
		backgroundMemory = testBit(lcdControl, 3) ? ADDR_VRAM_T_M_2 : ADDR_VRAM_T_M_1;
		yPos = scrollY + this->cpuLink->ReadByte(0xFF44);
	}

	tileRow = (yPos / 8) * 32;

	for (int i = 0; i < X_RES; i++) {

		xPos = (usingWindow && i >= windowX) ? i - windowX : i + scrollX;
		tileAddress = backgroundMemory + tileRow + xPos / 8;

		if (noSign) {
			tileNum = (Byte)this->cpuLink->ReadByte(tileAddress);
			tileLocation = tileData + tileNum * 16;
		} else {
			tileNum = (signed char)this->cpuLink->ReadByte(tileAddress);
			tileLocation = tileData + (tileNum + 128) * 16;
		}

		line = (yPos % 8) * 2;
		data1 = this->cpuLink->ReadByte(tileLocation + line);
		data2 = this->cpuLink->ReadByte(tileLocation + line + 1);
		colorBit = 7 - (xPos % 8);
		colorNum = ((int)testBit(data2, colorBit) << 1) | (int)testBit(data1, colorBit);
		color = getColor(colorNum, 0xFF47);
		y = this->cpuLink->ReadByte(0xFF44);

		if (!((y < 0) || (y > Y_RES - 1) || (i < 0) || (i > X_RES - 1))) {
			display[y][i] = color;
		}
	}

	return;
}

void GPU::RenderSprites(Byte lcdControl) {
	Byte index, yPos, xPos, tileLocation, attributes;
	Byte data1, data2, color;
	bool flipXaxis, flipYaxis;
	int scanline, ysize, line, colorBit, pixel, colorNum;
	Word address, colorAddress;
	const int maxSprites = 40, bytesPerSprite = 4;

	ysize = testBit(lcdControl, 2) ? 16 : 8;

	for (int sprite = 0; sprite < maxSprites; sprite++) {
		index					= sprite * bytesPerSprite;
		yPos					= this->cpuLink->ReadByte(ADDR_OAM + index + 0) - 16;
		xPos					= this->cpuLink->ReadByte(ADDR_OAM + index + 1) - 8;
		tileLocation			= this->cpuLink->ReadByte(ADDR_OAM + index + 2);
		attributes				= this->cpuLink->ReadByte(ADDR_OAM + index + 3);
		flipXaxis				= testBit(attributes, 5);
		flipYaxis				= testBit(attributes, 6);
		scanline				= this->cpuLink->ReadByte(0xFF44);

		if ((scanline >= yPos) && (scanline < (yPos + ysize))) {
			line				= flipYaxis ? (ysize - (scanline - yPos)) * 2 : (scanline - yPos) * 2;
			address				= (ADDR_VRAM_T_S + (tileLocation * 16)) + line;
			data1				= this->cpuLink->ReadByte(address);
			data2				= this->cpuLink->ReadByte(address + 1);

			for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
				colorBit		= flipXaxis ? 7 - tilePixel : tilePixel;
				colorNum		= ((int)testBit(data2, colorBit) << 1) | (int)testBit(data1, colorBit);
				colorAddress	= testBit(attributes, 4) ? 0xFF49 : 0xFF48;
				color			= getColor(colorNum, colorAddress);

				if (!color) { // transparant
					continue;
				}

				pixel = 7 - tilePixel + xPos;

				if (!((scanline < 0) || (scanline > X_RES - 1) || (pixel < 0) || (pixel > Y_RES - 1))) {
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
			case WHITE:
				SDL_SetRenderDrawColor(renderer, white);
				break;
			case LIGHT_GREY: 
				SDL_SetRenderDrawColor(renderer, light_grey);
				break;
			case DARK_GREY: 
				SDL_SetRenderDrawColor(renderer, dark_grey);
				break;
			case BLACK: 
				SDL_SetRenderDrawColor(renderer, black);
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
		ScanLineCounter += cycles;

		if (ScanLineCounter >= 456) {
			this->cpuLink->ram.setMemory(0xFF44, this->cpuLink->ReadByte(0xFF44) + 1);
			currentline = this->cpuLink->ReadByte(0xFF44);

			ScanLineCounter -= 456;

			if (currentline == Y_RES) {
				cpuLink->RequestInterupt(0);
			} else if (currentline > 0x99) {
				this->cpuLink->ram.setMemory(0xFF44, 0);
			} else if (currentline < Y_RES) {
				DrawScanLine();
				renderDisplay(currentline);
			}
		}
	}

	return;
}

void GPU::IncScanline() {
	this->cpuLink->ram.setMemory(0xff44, this->cpuLink->ReadByte(0xff44) + 1);
}

Byte GPU::getScanline() {
	return this->cpuLink->ReadByte(0xff44);
}

void GPU::setScanline(Byte s) {
	this->cpuLink->WriteByte(0xff44, s);
}

void GPU::UpdateGraphics2(int cycles) {
	/*
	SetLCDStatus();

	if (!IsLCDEnabled()) {
		return;
	}
	*/

	ScanLineCounter += cycles;

	//cout << "scanline counter: " << dec << ScanLineCounter << " gpu mode: " << GpuMode << endl;

	switch (GpuMode) {
	case GPU_MODE_HBLANK:
		if (ScanLineCounter >= 204) {
			IncScanline();

			if (getScanline() == Y_RES - 1) {
				//cout << "~~~~~~~~~~~~ request interrupt" << endl;
				this->cpuLink->RequestInterupt(0);

				GpuMode = GPU_MODE_VBLANK;
			}
			else {
				GpuMode = GPU_MODE_OAM;
			}

			ScanLineCounter -= 204;
		}
		break;
	case GPU_MODE_VBLANK:
		if (ScanLineCounter >= 456) {
			IncScanline();

			if (getScanline() > 153) {
				setScanline(0x00);
				GpuMode = GPU_MODE_OAM;
			}

			ScanLineCounter -= 456;
		}
		break;
	case GPU_MODE_OAM:
		if (ScanLineCounter >= 80) {
			GpuMode = GPU_MODE_VRAM;

			ScanLineCounter -= 80;
		}
		break;
	case GPU_MODE_VRAM:
		if (ScanLineCounter >= 172) {
			GpuMode = GPU_MODE_HBLANK;

			//cout << "draw scanline" << endl;
			DrawScanLine();

			ScanLineCounter -= 172;
		}
		break;
	}
}

void GPU::render() {
	SDL_RenderPresent(renderer);

	return;
}

/**
 * @brief  saves the current SDL2 window to a bmp file.
 * @note   uses time() to get a unique name for each screenshot.
 * @retval None
 */
void GPU::screenshot() {
	int w, h;
	time_t c;
	time(&c);
	stringstream ss;
	string name;
	ss << c;
	name = ss.str() + ".bmp";
	cout << "Your screenshot was saved as " << name << "." << endl;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	SDL_Surface *shot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, shot->pixels, shot->pitch);
	SDL_SaveBMP(shot, name.c_str());
	SDL_FreeSurface(shot);
}
