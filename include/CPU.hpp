#ifndef CPU_HPP_
#define CPU_HPP_

//----------------------------------------------------------------------------------------------
#include "Registers.hpp"
#include "Timer.hpp"
#include "Memory.hpp"
#include "Joypad.hpp"
#include "datatypes.h"
#include "bit.hpp"
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class CPU {

private:

	bool CycleConditionals;
	Word jumpAddress;
	Byte jumpRelAddress;
	void (CPU::*opcode[0x0100])();
	void (CPU::*cb_opcode[0x0100])();

	Byte running, gb_stop, gb_halt_bug;
	bool jump;
	bool gb_halt, gb_ime;
	int cycles;

public:

	// constants.
	const int clockSpeed = 4194304;
	const float fps = (float)59.7;
	const int MAXCYCLES = (int)(clockSpeed / fps);
	// 1 machine cycle = 4 cpu cycles
	const int CYCLEFACTOR = 4;

    // general purpose registers + stack pointer & program counter.
    class Registers registers;
	class Timer timer = Timer(this);
	class Joypad* joypad;
	class Memory memory = Memory(&registers, &timer);

public:

    CPU();
	~CPU();

	void Init_OPCODE();
	void Init_CBOPCODE();

	void setJoypadLink(class Joypad*);

	// setter and getter for running.
	Byte getRunning();
	void setRunning(Byte);

	// getter for clock speed.
	int getClockSpeed() const;

	// adjust for register a for bcd addition.
	void daa();

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
	Word addWord(Word, Word);
	// add signed byte to Word.
	Word addSignedWord(Word, Byte);
	// add two 16-bit values for addresses (that means without flags).
	Word addSignedWordAddress(Word, Byte);
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

	// call routine.
	void call(Word);
	// rst routine.
	void rst(Word);

	// removed/illegal instructions.
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

	// interrupts
	void RequestInterrupt(int);
	int DoInterrupts();
	int ServiceInterrupts(int);

	// update the timers.
	void UpdateTimers(int);

	// halt bug.
	void halt_bug();

	// one cpu step.
	int CPUstep();

	#include "opcodes.hpp"
	#include "cb_opcodes.hpp"
};
//----------------------------------------------------------------------------------------------

#endif