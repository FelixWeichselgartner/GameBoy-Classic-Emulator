#ifndef CPU_HPP_
#define CPU_HPP_

//----------------------------------------------------------------------------------------------
#include <math.h>
#include "Registers.hpp"
#include "RAM.hpp"
#include "ROM.hpp"

typedef unsigned char Byte;
//----------------------------------------------------------------------------------------------

void testBit(Byte, int);

//----------------------------------------------------------------------------------------------
class CPU {

private:

	Byte running, gb_stop, gb_halt;
	Byte jump, enableInterrupts;
	int clockSpeed = 4000000; // 4MHz

public:

    // general purpose registersm + stack pointer & program counter.
    class Registers registers;
    class RAM ram;
	class ROM rom;

public:

    CPU();

	// setter and getter for running.
	Byte getRunning();
	void setRunning(Byte);

	// getter for clock speed.
	int getClockSpeed() const;

	void setFlag(char);
	void resetFlag(char);

    Byte ReadByte(unsigned short) const;
    void WriteByte(unsigned short, Byte);

	// adjust for register a for bcd addition.
	void daa();
	// load 8 bit value in register.
	Byte load8bit();
	// load 16 bit value in register.
	unsigned short load16bit();
	// save 16 bit value to memory.
	void save16bitToAddress(unsigned short, unsigned short);
	// rotate left carry.
	Byte rlc(Byte);
	// rotate right carry.
	Byte rrc(Byte);
	// rotate right.
	Byte rr(Byte);
	// rotate left.
	Byte rl(Byte);
	// complement of Byte value.
	Byte cpl(Byte);
	// add two Byte values.
    Byte add(Byte, Byte, char);
	// add two 16-bit values.
	unsigned short add16bit(unsigned short, unsigned short, char);
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
	// compare two Byte values.
	void cp(Byte, Byte);
	// push Byte value on the stack.
	void push8bit(Byte);
	// pop Byte value from the stack.
	Byte pop8bit();
	// push 2 Byte value on the stack.
	void push16bit(unsigned short);
	// pop 2 Byte value from the stack.
	unsigned short pop16bit();
	// call routine.
	void call(unsigned short);
	
	// execute instructions according to opcode.
    void executeInstruction(Byte);

	// shift Byte left with preserving sign.
	Byte sla(Byte);
	// shift Byte right with preserving sign.
	Byte sra(Byte);
	// swap nybbles in Byte.
	Byte swap(Byte);
	// shift Byte right.
	Byte srl(Byte);
	// test bit of Byte.
	void bit(Byte, int);
	// clear (reset) bit of Byte.
	Byte res(Byte, int);
	// set bit of Byte.
	Byte set(Byte, int);

	// execute extended opcodes.
	void executeExtendedOpcodes();

	// one cpu step.
	void CPUstep();

};
//----------------------------------------------------------------------------------------------

#endif