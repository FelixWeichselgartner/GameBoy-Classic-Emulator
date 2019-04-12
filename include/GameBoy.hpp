#ifndef GAMEBOY_HPP_
#define GAMEBOY_HPP_

//----------------------------------------------------------------------------------------------
#include "CPU.hpp"
#include "RAM.hpp"
#include "ROM.hpp"
#include "GPU.hpp"
#include "Registers.hpp"

#include <fstream>
//----------------------------------------------------------------------------------------------

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

	void AdditionTest();
	void RomTest();
	void PushPopTest();
	void Debug_InputAndLog(SDL_Event &);
	void decFlagTest();
	void add2x16bithl();
	void OP_DA_Test();
	void Signed8bitTo16bit();
	void land_test_e6();

	void tests(int);

    void run();

};
//----------------------------------------------------------------------------------------------

#endif