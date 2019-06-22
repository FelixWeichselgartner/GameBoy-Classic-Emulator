#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include "Registers.hpp"
#include "RAM.hpp"
#include "ROM.hpp"
#include "MBC.hpp"
#include "MBC_1.hpp"
#include "SerialDataTransfer.hpp"
#include "Timer.hpp"

#include "bit.hpp"
#include "datatypes.h"
#include "format.hpp"
#include "MemoryMap.hpp"

class GPU;

//----------------------------------------------------------------------------------------------
class Memory {

private:

    bool EnableBootstrap;
    Byte MemoryBankingMode;

	Byte vram[VRAM_SIZE]    = { 0 };
	Byte echo[ECHO_SIZE]    = { 0 };
	Byte oam [OAM_SIZE] 	= { 0 };
	Byte io  [IO_SIZE] 		= { 0 };
	Byte hram[HRAM_SIZE] 	= { 0 };
	Byte interrupt_enable_register = 0;

	class Registers* registers;
	class Joypad* joypad;
	class Timer* timer;
	class GPU* gpu;

public:

	class RAM ram;
	class ROM rom = ROM(&ram, &EnableBootstrap);
	class MBC* mbc;
	class SerialDataTransfer sdt = SerialDataTransfer(true /*false*/);

private:

	Byte ReadVRAM(Word);
	Byte ReadECHO(Word);
	Byte ReadOAM(Word);
	Byte ReadUNUSABLE(Word);
	Byte ReadIO(Word);
	Byte ReadHRAM(Word);

	void WriteVRAM(Word, Byte);
	void WriteECHO(Word, Byte);
	void WriteOAM(Word, Byte);
	void WriteUNUSABLE(Word, Byte);
	void WriteIO(Word, Byte);
	void WriteHRAM(Word, Byte);

public:

    void resetVar();
    void reset();

	Memory();
	Memory(class Registers *, class Timer *);
	~Memory();

	void InitMemory();

	void setJoypadLink(class Joypad*);
	Byte getJoypadRequest();

	void setScanline(Byte);
	void setDividerRegister(Byte);

	// setter and getter for enableBootstrap.
	bool getEnableBootstrap() const;
	void setEnableBootstrap(bool);

	// load 8 bit value in register.
	Byte LoadByte();
	// load 16 bit value in register.
	Word LoadWord();

	// push Byte value on the stack.
	void PushByte(Byte);
	// pop Byte value from the stack.
	Byte PopByte();
	// push Word value on the stack.
	void PushWord(Word);
	// pop Word value from the stack.
	Word PopWord();

	// dma transfer - dma == direct memory access
	void DoDMATransfer(Byte);

	// CPU's memory access.
	Byte ReadByte(Word);
	void WriteByte(Word, Byte);
	void WriteWord(Word, Word);

	// print memory.
	void print(Word, Word);

	// initialise mbc
	void InitialiseMemoryBanking();

};
//----------------------------------------------------------------------------------------------

#endif