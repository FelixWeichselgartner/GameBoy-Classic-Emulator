#ifndef GPU_HPP_
#define GPU_HPP_

//----------------------------------------------------------------------------------------------
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "../include/CPU.hpp"
#include <string>
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// screen resolution:

// amount of pixels in x direction.
#define X_RES 160
// amount of pixels in y direction.
#define Y_RES 144
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// GPU MODES:

enum {
	GPU_MODE_HBLANK,
	GPU_MODE_VBLANK,
	GPU_MODE_OAM,
	GPU_MODE_VRAM
};
//----------------------------------------------------------------------------------------------


/*----------------------------------------------------------------------------------------------
// lcd control register @ 0xFF40
Bit 7 - LCD Display Enable(0 = Off, 1 = On)
Bit 6 - Window Tile Map Display Select(0 = 9800 - 9BFF, 1 = 9C00 - 9FFF)
Bit 5 - Window Display Enable(0 = Off, 1 = On)
Bit 4 - BG & Window Tile Data Select(0 = 8800 - 97FF, 1 = 8000 - 8FFF)
Bit 3 - BG Tile Map Display Select(0 = 9800 - 9BFF, 1 = 9C00 - 9FFF)
Bit 2 - OBJ(Sprite) Size(0 = 8x8, 1 = 8x16)
Bit 1 - OBJ(Sprite) Display Enable(0 = Off, 1 = On)
Bit 0 - BG Display(for CGB see below) (0 = Off, 1 = On)
*///--------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// format gameboy:
// 0xAARRGGBB (Alhpa-Red-Green-Blue)

// format sdl2:
// red, green, blue, alpha

// white
#define WHITE 0
#define white 0xFF, 0xFF, 0xFF, 0xFF
// light gray
#define LIGHT_GREY 1
#define light_grey 0xAA, 0xAA, 0xAA, 0xFF
// dark gray
#define DARK_GREY 2
#define dark_grey 0x55, 0x55, 0x55, 0xFF
// black
#define BLACK 3
#define black 0x00, 0x00, 0x00, 0xFF
//----------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------
class GPU {

private:

	class CPU* cpu;
	class Memory* memory;
	SDL_Window* window;
	SDL_Renderer* renderer;
	
public:int ScanLineCounter; // public for debug.
private:
    const int pixelsPerTile = 8;
	const int scaleWidth = 2, scaleHeight = 2;
	const int windowWidth = X_RES * scaleWidth, windowHeight = Y_RES * scaleHeight;
    const int SCANLINECYCLES = 456;

    Byte display[Y_RES][X_RES] = { 0 };
	int GpuMode;
	bool inc_en;
    std::string windowName;

public:

    void reset();

    GPU(class CPU*);

	SDL_Window* getWindow() const { return window; }
	SDL_Renderer* getRenderer() const { return renderer; }

	Byte getControl() const;
	void setControl(Byte);

	Byte getStatus() const;
	void setStatus(Byte);
	void setStatusBit(int, Byte);
	void setStatusMode(Byte);

	Byte getScanline() const;
	void setScanline(Byte);
	void IncScanline();
	void resetScanline();

	void clearScreen();

	bool IsLCDEnabled() const;
	void SetLCDStatus();

	Byte getColor(Byte, Word);

	void RenderNintendoLogo();
	void RenderTiles(Byte);
	void RenderSprites(Byte);

	void DrawScanLine();

	void renderDisplay(Byte);

	void requestLcdcInterrupt(int);

	void tick();
	void UpdateGraphics(int);

	void render();

	void screenshot();

};
//----------------------------------------------------------------------------------------------

#endif