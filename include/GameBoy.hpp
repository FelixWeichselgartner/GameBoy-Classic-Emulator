#ifndef GAMEBOY_HPP_
#define GAMEBOY_HPP_

//----------------------------------------------------------------------------------------------
#include "CPU.hpp"
#include "RAM.hpp"
#include "ROM.hpp"
#include "GPU.hpp"
#include "Registers.hpp"
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class GameBoy {

private:

    class CPU cpu;
    class GPU gpu;

public:

    GameBoy();

	void AdditionTest();

    void run();

};
//----------------------------------------------------------------------------------------------

#endif