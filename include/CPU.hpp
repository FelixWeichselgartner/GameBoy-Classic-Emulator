#ifndef CPU_HPP_
#define CPU_HPP_

//----------------------------------------------------------------------------------------------
#include <math.h>
#include "Registers.hpp"
#include "RAM.hpp"

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

public:

    CPU();

	Byte getRunning();
	void setRunning(Byte);

    Byte ReadByte(unsigned short) const;
    void WriteByte(unsigned short, Byte);

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

};
//----------------------------------------------------------------------------------------------

#endif