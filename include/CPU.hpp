#ifndef CPU_HPP_
#define CPU_HPP_

// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

#include <math.h>
#include "Registers.hpp"
#include "RAM.hpp"

//----------------------------------------------------------------------------------------------
// Zero Flag - set if the result of the operation is zero.
#define ZF 0
// Operation Flag - set if the operation was subtraction.
#define OF
// Half-Carry Flag - set if there was an overflow in the lower half of the result (omitted).
#define HCF
// Carry Flag - set if there was an overflow in the result.
#define CF
//----------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------
class CPU {

private:

    // flags
    byte Flags[8];

    // general purpose registersm + stack pointer & program counter.
    class Registers registers();

    class RAM ram();
    


public:

    CPU();

    byte ReadByte(unsigned short);
    void WriteByte(unsigned short, byte);
    void executeInstruction(byte);

};
//----------------------------------------------------------------------------------------------

#endif