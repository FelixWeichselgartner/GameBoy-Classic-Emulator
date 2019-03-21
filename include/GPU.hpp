#ifndef GPU_HPP_
#define GPU_HPP_

//----------------------------------------------------------------------------------------------
#include <SDL.h>
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

public:

    GPU(class CPU*);

	SDL_Window* getWindow() const { return window; }
	SDL_Renderer* getRenderer() const { return renderer; }

	bool IsLCDEnabled() const;
	void SetLCDStatus();

	void TestGraphics();

	void RenderBackground();

	void DrawScanLine();

	void UpdateGraphics();

};
//----------------------------------------------------------------------------------------------

#endif