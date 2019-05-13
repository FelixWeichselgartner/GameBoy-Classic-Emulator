#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include "Registers.hpp"
#include "RAM.hpp"
#include "ROM.hpp"
#include "MBC.hpp"
#include "MBC_1.hpp"
#include "Timer.hpp"

#include "bit.hpp"
#include "datatypes.h"
#include "format.hpp"
#include "MemoryMap.hpp"

//----------------------------------------------------------------------------------------------
class Memory {

private:

	bool EnableBootstrap = false;
	Byte MemoryBankingMode;

	Byte vram[ADDR_EXT_RAM - ADDR_VRAM_T_S] = { 0 };
	Byte echo[ADDR_OAM - ADDR_ECHO] 		= { 0 };
	Byte oam[ADDR_UNUSABLE - ADDR_OAM] 		= { 0 };
	Byte io[ADDR_HRAM - ADDR_IO] 			= { 0 };
	Byte hram[ADDR_INTR_EN - ADDR_HRAM] 	= { 0 };
	Byte interrupt_enable_register 			= 0;

	class Registers* registers;
	class Joypad* joypad;
	class Timer* timer;

public:

	class RAM ram;
	class ROM rom = ROM(&ram, &EnableBootstrap);
	class MBC* mbc;

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