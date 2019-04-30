#ifndef CPU_HPP_
#define CPU_HPP_

//----------------------------------------------------------------------------------------------
#include "datatypes.h"
#include "Registers.hpp"
#include "RAM.hpp"
#include "ROM.hpp"
#include "Joypad.hpp"
#include "Timer.hpp"
//----------------------------------------------------------------------------------------------

bool testBit(Byte, int);
Byte resetBit(Byte, int);
Byte setBit(Byte, int);

//----------------------------------------------------------------------------------------------
class CPU {

private:

	Byte running, gb_stop;
	bool enableBootstrap, jump;
	bool gb_halt, gb_ime;
	int cycles;

public:

	// constants.
	const int clockSpeed = 4194304;
	const float fps = (float)59.7;
	const int MAXCYCLES = (int)(clockSpeed / fps);
	const int CYCLEFACTOR = 8;

    // general purpose registers + stack pointer & program counter.
    class Registers registers;
    class RAM ram;
	class ROM rom = ROM(&ram);
	class Joypad* joypadLink;
	class Timer* timer;

public:

    CPU();
	~CPU();

	void setJoypadLink(class Joypad*);

	// setter and getter for running.
	Byte getRunning();
	void setRunning(Byte);

	// getter for clock speed.
	int getClockSpeed() const;

	// setter and getter for enableBootstrap.
	bool getEnableBootstrap() const;
	void setEnableBootstrap(bool);

	// Flag handling.
	Byte getFlag(char);
	void setFlag(char);
	void resetFlag(char);
	void setFlagState(char, bool);
	void flipFlag(char);
	void resetFlagAll();

	// CPU's memory access.
	Byte ReadIORegisters(Word);
    Byte ReadByte(Word);
    void WriteByte(Word, Byte);

	// adjust for register a for bcd addition.
	void daa();
	// load 8 bit value in register.
	Byte load8bit();
	// load 16 bit value in register.
	Word load16bit();
	// save 16 bit value to memory.
	void save16bitToAddress(Word, Word);

	// shift operations special for akkumulator A.
	Byte rlca(Byte);
	Byte rla(Byte);
	Byte rrca(Byte);
	Byte rra(Byte);
	// rotate left carry.
	Byte rlc(Byte);
	// rotate right carry.
	Byte rrc(Byte);
	// rotate right.
	Byte rr(Byte);
	// rotate left.
	Byte rl(Byte);
	// shift Byte left with preserving sign.
	Byte sla(Byte);
	// shift Byte right with preserving sign.
	Byte sra(Byte);
	// shift Byte right.
	Byte srl(Byte);

	// swap nybbles in Byte.
	Byte swap(Byte);
	
	// increment value.
	Byte inc(Byte);
	// decrement value.
	Byte dec(Byte);
	// add two Byte values.
    Byte add(Byte, Byte);
	// add two 16-bit values.
	Word add16bit(Word, Word);
	// add signed byte to Word.
	Word add16bitSign(Word, Byte);
	// add two 16-bit values for addresses (that means without flags).
	Word add16bitAdrSign(Word, Byte);
	// add two Byte values + carry.
	Byte adc(Byte, Byte);
	// sub two Byte values.
	Byte sub(Byte, Byte);
	// sub two Byte values - carry.
	Byte sbc(Byte, Byte);
	// logical AND on two Byte values.

	Byte land(Byte, Byte); 
	// logical XOR on two Byte values.
	Byte lxor(Byte, Byte);
	// logical OR on two Byte values.
	Byte lor(Byte, Byte);
	// complement of Byte value.
	Byte cpl(Byte);
	// compare two Byte values.
	void cp(Byte, Byte);

	// push Byte value on the stack.
	void push8bit(Byte);
	// pop Byte value from the stack.
	Byte pop8bit();
	// push 2 Byte value on the stack.
	void push16bit(Word);
	// pop 2 Byte value from the stack.
	Word pop16bit();

	// call routine.
	void call(Word);
	// rst
	void rst(Word);

	void removed(Byte);
	
	// execute instructions according to opcode.
    void executeInstruction(Byte);

	// test bit of Byte.
	void bit(int, Byte);
	// clear (reset) bit of Byte.
	Byte res(int, Byte);
	// set bit of Byte.
	Byte set(int, Byte);

	// execute extended opcodes.
	int executeExtendedOpcodes();

	// one cpu step.
	int CPUstep();

	// interupts
	void RequestInterupt(int);
	void DoInterupts();
	void ServiceInterupts(int);

	// dma transfer - dma == direct memory access
	void DoDMATransfer(Byte);

	void UpdateTimers(int);
};
//----------------------------------------------------------------------------------------------

#endif