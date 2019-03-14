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
#define X_RES 144
// amount of pixels in y direction.
#define Y_RES 160

//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// format:
// 0xAARRGGBB (Alhpa-Red-Green-Blue)

// black
#define BLACK 0xFF000000
// dark gray
#define DARK_GREY 0xFF555555
// light gray
#define LIGHT_GREY 0xFFAAAAAA
// white
#define WHITE 0xFFFFFFFF
//----------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------
class GPU {

private:

	class CPU* cpuLink;
	class RAM* ramLink;
	SDL_Window* window;
	std::string windowName;

public:

    GPU(class CPU*);

	SDL_Window* getWindow() const { return window; }

	void SetLCDStatus();

	void UpdateGraphics();

};
//----------------------------------------------------------------------------------------------

#endif