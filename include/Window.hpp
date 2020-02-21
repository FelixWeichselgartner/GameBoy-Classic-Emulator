//
// window is responsible for displaying the in a sdl window.
//

#ifndef GAMEBOY_CLASSIC_EMULATOR_WINDOW_HPP
#define GAMEBOY_CLASSIC_EMULATOR_WINDOW_HPP

#include "CPU.hpp"
#include "datatypes.h"
using std::string;

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

//----------------------------------------------------------------------------------------------
// screen resolution:
// TODO: remove since double with GPU.hpp
// amount of pixels in x direction.
#define X_RES 160
// amount of pixels in y direction.
#define Y_RES 144
//----------------------------------------------------------------------------------------------

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


class Window {

private:
    class CPU* cpu;
    SDL_Window* window;
    SDL_Renderer* renderer;
    const int scaleWidth = 2, scaleHeight = 2;
    const int windowWidth = X_RES * scaleWidth, windowHeight = Y_RES * scaleHeight;
    Byte display[Y_RES][X_RES] = { 0 };
    std::string windowName;

public:
    SDL_Window* getWindow() const { return window; }
    SDL_Renderer* getRenderer() const { return renderer; }

    void reset();

    Window(class CPU*);
    ~Window();

    void set(int, int, int);
    void clearScreen();
    void renderLine(Byte);
    void render();
    void screenshot();
};


#endif //GAMEBOY_CLASSIC_EMULATOR_WINDOW_HPP
