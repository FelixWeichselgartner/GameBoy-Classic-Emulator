#ifndef CPU_HPP_
#define CPU_HPP_

//----------------------------------------------------------------------------------------------
#include <math.h>
#include "Registers.hpp"
#include "RAM.hpp"
#include "ROM.hpp"

typedef unsigned char Byte;
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class CPU {

private:

	Byte running, gb_stop, gb_halt;

public:

    // general purpose registersm + stack pointer & program counter.
    class Registers registers;
    class RAM ram;
	class ROM rom;

public:

    CPU();

	Byte getRunning();
	void setRunning(Byte);

    Byte ReadByte(unsigned short) const;
    void WriteByte(unsigned short, Byte);

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
    Byte add(Byte, Byte);
	// add two 16-bit values.
	unsigned short add16bit(unsigned short, unsigned short);
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
	void compare(Byte, Byte);
	
	// execute instructions according to opcode.
    void executeInstruction(Byte);

	void CPUstep();

};
//----------------------------------------------------------------------------------------------

#endif