#ifndef GPU_HPP_
#define GPU_HPP_

//----------------------------------------------------------------------------------------------
#include <SDL.h>
#include "../include/CPU.hpp"
#include <string>
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// screen resolution:

/*
// amount of pixels in horizontal direction.
#define LCD_HOR_LINES 
// amount of pixels in vertical direction.
#define LCD_VERT_LINES 154
*/
// amount of pixels in x direction.
#define X_RES 160
// amount of pixels in y direction.
#define Y_RES 144

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

// black
#define BLACK 0xFF000000
#define black 0x00, 0x00, 0x00, 0xFF
// dark gray
#define DARK_GREY 0xFF555555
#define dark_grey 0x55, 0x55, 0x55, 0xFF
// light gray
#define LIGHT_GREY 0xFFAAAAAA
#define light_grey 0xAA, 0xAA, 0xAA, 0xFF
// white
#define WHITE 0xFFFFFFFF
#define white 0xFF, 0xFF, 0xFF, 0xFF
//----------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------
class GPU {

private:

	class CPU* cpuLink;
	class RAM* ramLink;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::string windowName;
	const int pixelsPerTile = 8;
	const int scaleWidth = 4, scaleHeight = 4;
	const int windowWidth = X_RES * scaleWidth, windowHeight = Y_RES * scaleHeight;
	unsigned short scanLineCounter;
	Byte display[Y_RES][X_RES];

public:

    GPU(class CPU*);

	SDL_Window* getWindow() const { return window; }
	SDL_Renderer* getRenderer() const { return renderer; }

	void clearScreen();

	bool IsLCDEnabled() const;
	void SetLCDStatus();

	void TestGraphics();
	void TestTiles();

	Byte getColor(Byte, unsigned short);

	void RenderTiles(Byte);
	void RenderSprites(Byte);

	void DrawScanLine();

	void renderDisplay(Byte);
	void UpdateGraphics();

};
//----------------------------------------------------------------------------------------------

#endif