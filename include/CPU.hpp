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

public:

    // general purpose registersm + stack pointer & program counter.
    class Registers registers;

    class RAM ram;

public:

    CPU();

    Byte ReadByte(unsigned short) const;
    void WriteByte(unsigned short, Byte);

    void compare(Byte, Byte);

    void add(Byte, Byte);

    void executeInstruction(Byte);

};
//----------------------------------------------------------------------------------------------

#endif