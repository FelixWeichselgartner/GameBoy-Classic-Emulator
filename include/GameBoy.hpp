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

#include <SDL2/SDL.h>
#include <fstream>
//----------------------------------------------------------------------------------------------

extern bool externVar;

//----------------------------------------------------------------------------------------------
class GameBoy {

private:

    class CPU cpu;
    class GPU gpu = GPU(&cpu);
	class Joypad joypad = Joypad(&cpu);

public:

    GameBoy();

	void PrintRegisters();
	void PrintRegistersFile(std::ofstream &);

	void RomTest();
	void Debug_InputAndLog(SDL_Event &);
	

	void tests(int);

	bool winEvent(SDL_Event&, bool &);
    void run();

};
//----------------------------------------------------------------------------------------------

#endif