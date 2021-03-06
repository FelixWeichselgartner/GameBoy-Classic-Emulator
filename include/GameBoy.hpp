#ifndef GAMEBOY_HPP_
#define GAMEBOY_HPP_

//----------------------------------------------------------------------------------------------
#include "CPU.hpp"
#include "RAM.hpp"
#include "ROM.hpp"
#include "GPU.hpp"
#include "Registers.hpp"
#include "Joypad.hpp"
#include "datatypes.h"
#include "format.hpp"

#ifdef _WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif
#include <fstream>
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class GameBoy {

private:

    class CPU cpu;
    class GPU *gpu;
	class Joypad joypad = Joypad(&cpu);

public:

    void reset();

    GameBoy(char *);
	~GameBoy();

	// debug functions.
	void PrintRegisters();
	void PrintRegistersFile(std::ofstream &);
	void Debug_InputAndLog(SDL_Event &);
	void tests(int);

	// loop functions.
	bool winEvent(SDL_Event&, bool &);
    void run();

};
//----------------------------------------------------------------------------------------------

#endif