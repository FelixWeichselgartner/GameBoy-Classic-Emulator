//----------------------------------------------------------------------------------------------
#include "../include/GameBoy.hpp"
#include "../include/RAM.hpp"
#include "../include/GPU.hpp"
#include "../include/CPU.hpp"
#include "../include/format.hpp"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
//----------------------------------------------------------------------------------------------

void GPU::reset() {
    this->windowName = this->memory->rom.getGameName();
    this->ScanLineCounter = 3;
    this->GpuMode = GPU_MODE_HBLANK;
    this->inc_en = true;

    // Byte display[Y_RES][X_RES] = { 0 };
    // not needed for first.
    // set zero for 2nd, 3rd ... reset.
    for (int i = 0; i < X_RES; i++) {
        for (int k = 0; k < Y_RES; k++) {
            display[k][i] = 0;
        }
    }
}

GPU::GPU(class CPU* cpu) {
 	if ((this->cpu = cpu) == NULL) exit(2);
	if ((this->memory = &this->cpu->memory) == NULL) exit(2);

	reset();

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

////////////////////////////////////////////////////////////////
// video register methods.

// 0xFF40 - control.

Byte GPU::getControl() const {
	return this->memory->ReadByte(0xFF40);
}

void GPU::setControl(Byte value) {
	this->memory->WriteByte(0xFF40, value);
}

bool GPU::IsLCDEnabled() const {
	return testBit(getControl(), 7);
}

// 0xFF41 - status.

/*
http://gbdev.gg8.se/files/docs/mirrors/pandocs.html#videodisplay
FF41 - STAT - LCDC Status (R/W)

  Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
  Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
  Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
  Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
  Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
  Bit 1-0 - Mode Flag       (Mode 0-3, see below) (Read Only)
			0: During H-Blank
			1: During V-Blank
			2: During Searching OAM-RAM
			3: During Transfering Data to LCD Driver
*/

Byte GPU::getStatus() const {
	return this->memory->ReadByte(0xFF41);
}

void GPU::setStatus(Byte value) {
	this->memory->WriteByte(0xFF41, value);
}

void GPU::setStatusBit(int pos, Byte state) {
	Byte status = this->memory->ReadByte(0xFF41);
	Byte mask = 1 << pos;
	state ? status |= mask : status &= ~mask;
	this->memory->WriteByte(0xFF41, status);
}

void GPU::setStatusMode(Byte mode) {
	setStatusBit(1, mode & 0x02);
	setStatusBit(0, mode & 0x01);
}

// 0xFF44 - scanline.

Byte GPU::getScanline() const {
	return this->memory->ReadByte(0xFF44);
}

void GPU::setScanline(Byte s) {
	this->memory->setScanline(s);
}

void GPU::IncScanline() {
	if (this->inc_en)
		this->memory->setScanline(getScanline() + 1);
	else
		this->inc_en = true;
}

void GPU::resetScanline() {
	this->memory->setScanline(0);
}

////////////////////////////////////////////////////////////////

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

void GPU::SetLCDStatus() {
	Byte status = getStatus();
	Byte currentline = getScanline();
	Byte currentmode = status & 0x03, mode = 0x00;
	bool reqInt = false;
	int mode2bounds, mode3bounds;

	if (!IsLCDEnabled()) {
		ScanLineCounter = 0;
		resetScanline();
		status = setBit(status & 0b11111100, 0);
		this->memory->WriteByte(0xFF41, status);
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
		this->cpu->RequestInterrupt(1);
	}

	if (currentline == this->memory->ReadByte(0xFF45)) { // coincidence flag
		status = setBit(status, 2);
		if (testBit(status, 6)) {
			this->cpu->RequestInterrupt(1);
		}
	} else {
		status = resetBit(status, 2);
	}

	this->memory->WriteByte(0xFF41, status);

	return;
}

Byte GPU::getColor(Byte colorNum, Word address) {
	Byte palette = this->memory->ReadByte(address);
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
		data1 = this->memory->ReadByte(i);
		data2 = this->memory->ReadByte(i + 1);

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

	scrollY = this->memory->ReadByte(0xFF42);
	scrollX = this->memory->ReadByte(0xFF43);
	windowY = this->memory->ReadByte(0xFF4A);
	windowX = this->memory->ReadByte(0xFF4B) - 7;
	yPos = 0;

	// checks if the window is enabled.
	usingWindow = (testBit(lcdControl, 5) && (windowY <= getScanline()));

	if (testBit(lcdControl, 4)) {
		tileData = ADDR_VRAM_T_S;
	} else {
		tileData = 0x8800;
		noSign = false;
	}

	if (usingWindow) {
		backgroundMemory = testBit(lcdControl, 6) ? ADDR_VRAM_T_M_2 : ADDR_VRAM_T_M_1;
		yPos = getScanline() - windowY;
	} else {
		backgroundMemory = testBit(lcdControl, 3) ? ADDR_VRAM_T_M_2 : ADDR_VRAM_T_M_1;
		yPos = scrollY + getScanline();
	}

	tileRow = (yPos / 8) * 32;

	for (int i = 0; i < X_RES; i++) {

		xPos = (usingWindow && i >= windowX) ? i - windowX : i + scrollX;
		tileAddress = backgroundMemory + tileRow + xPos / 8;

		if (noSign) {
			tileNum = (Byte)this->memory->ReadByte(tileAddress);
			tileLocation = tileData + tileNum * 16;
		} else {
			tileNum = (signed char)this->memory->ReadByte(tileAddress);
			tileLocation = tileData + (tileNum + 128) * 16;
		}

		line = (yPos % 8) * 2;
		data1 = this->memory->ReadByte(tileLocation + line);
		data2 = this->memory->ReadByte(tileLocation + line + 1);
		colorBit = 7 - (xPos % 8);
		colorNum = ((int)testBit(data2, colorBit) << 1) | (int)testBit(data1, colorBit);
		color = getColor(colorNum, 0xFF47);
		y = getScanline();

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
		index			= sprite * bytesPerSprite;
		yPos			= this->memory->ReadByte(ADDR_OAM + index + 0) - 16;
		xPos			= this->memory->ReadByte(ADDR_OAM + index + 1) - 8;
		tileLocation	= this->memory->ReadByte(ADDR_OAM + index + 2);
		attributes		= this->memory->ReadByte(ADDR_OAM + index + 3);
		flipXaxis		= testBit(attributes, 5);
		flipYaxis		= testBit(attributes, 6);
		scanline		= getScanline();

		if ((scanline >= yPos) && (scanline < (yPos + ysize))) {
			line	= flipYaxis ? (ysize - (scanline - yPos)) * 2 : (scanline - yPos) * 2;
			address	= (ADDR_VRAM_T_S + (tileLocation * 16)) + line;
			data1	= this->memory->ReadByte(address);
			data2	= this->memory->ReadByte(address + 1);

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
	Byte control = this->memory->ReadByte(0xFF40);

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

void GPU::requestLcdcInterrupt(int statBit) {
	if ((this->memory->ReadByte(0xFF41) & (1 << statBit)) != 0) {
		this->cpu->RequestInterrupt(1);
	}
}

const int factor = 1;

const int CLOCKS_PER_HBLANK = 204 * factor; /* Mode 0 */
const int CLOCKS_PER_SCANLINE_OAM = 80 * factor; /* Mode 2 */
const int CLOCKS_PER_SCANLINE_VRAM = 172 * factor; /* Mode 3 */
const int CLOCKS_PER_SCANLINE = (CLOCKS_PER_SCANLINE_OAM + CLOCKS_PER_SCANLINE_VRAM + CLOCKS_PER_HBLANK);

const int CLOCKS_PER_VBLANK = 4560 * factor; /* Mode 1 */
const int SCANLINES_PER_FRAME = 144 * factor;
const int CLOCKS_PER_FRAME = (CLOCKS_PER_SCANLINE * SCANLINES_PER_FRAME) + CLOCKS_PER_VBLANK;

void GPU::tick() {
	Byte currentline = getScanline();

	if (currentline == 153) {
		resetScanline();
		this->inc_en = false;
	}

	/*
	Byte status;

	switch (this->GpuMode) {
	case GPU_MODE_HBLANK:
		if (this->ScanLineCounter >= CLOCKS_PER_HBLANK) {
			DrawScanLine();
			renderDisplay(currentline);
			IncScanline();
			this->ScanLineCounter %= CLOCKS_PER_HBLANK;

			if (getScanline() == 144) {
				this->GpuMode = GPU_MODE_VBLANK;
				setStatusBit(1, 0);
				setStatusBit(0, 1);
				this->cpu->RequestInterrupt(0);
			} else {
				this->GpuMode = GPU_MODE_OAM;
				setStatusBit(1, 1);
				setStatusBit(0, 0);
			}
		}
		break;
	case GPU_MODE_VBLANK:
		if (this->ScanLineCounter >= CLOCKS_PER_SCANLINE) {
			IncScanline();
			this->ScanLineCounter %= CLOCKS_PER_SCANLINE;

			if (getScanline() == 154) {
				RenderSprites(this->memory->ReadByte(0xFF40));
				renderDisplay(currentline);
				resetScanline();
				this->GpuMode = GPU_MODE_OAM;
				setStatusBit(1, 1);
				setStatusBit(0, 0);
			}
		}
		break;
	case GPU_MODE_OAM:
		if (this->ScanLineCounter >= CLOCKS_PER_SCANLINE_OAM) {
			this->ScanLineCounter %= CLOCKS_PER_SCANLINE_OAM;
			setStatusBit(1, 1);
			setStatusBit(0, 1);
			this->GpuMode = GPU_MODE_VRAM;
		}
		break;
	case GPU_MODE_VRAM:
		status = this->memory->ReadByte(0xFF40);

		if (this->ScanLineCounter >= CLOCKS_PER_SCANLINE_VRAM) {
			this->ScanLineCounter %= CLOCKS_PER_SCANLINE_VRAM;
			this->GpuMode = GPU_MODE_HBLANK;

			bool coincidence = currentline == this->memory->ReadByte(0xFF45);

			if ((testBit(status, 3)) || 
				(testBit(status, 6) && coincidence)) {
				this->cpu->RequestInterrupt(1);
			}

			setStatusBit(2, coincidence ? 0xFF : 0x00);
			setStatusBit(1, 0);
			setStatusBit(0, 0);
		}
		break;
	default:
		exit(10);
		break;
	}*/

	
	if (ScanLineCounter >= SCANLINECYCLES) {
		currentline = getScanline();
		ScanLineCounter -= SCANLINECYCLES;

		if (currentline == Y_RES) {
			cpu->RequestInterrupt(0);
			IncScanline();
		}
		// maximum value for scanline is 0x99.
		else if (currentline == 0x99) {
			resetScanline();
			// dont increase scanline here, because
			// line 0 would never be drawn.
		}
		else if (currentline < Y_RES) {
			DrawScanLine();
			renderDisplay(currentline);
			IncScanline();
		}
		else {
			IncScanline();
		}
	}	
}

void GPU::UpdateGraphics(int cycles) {
	SetLCDStatus();

	if (IsLCDEnabled()) {
		ScanLineCounter += cycles;
		tick();
	}

	return;
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
