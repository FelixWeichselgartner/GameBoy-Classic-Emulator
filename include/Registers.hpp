#ifndef REGISTERS_HPP_
#define REGISTERS_HPP_

// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

//----------------------------------------------------------------------------------------------
class Registers {

private:
    union {
        struct {
            byte A;
            byte F;
        };
        unsigned short AF;
    };

    union {
        struct {
            byte B;
            byte C;
        };
        unsigned short BC;
    };

    union {
        struct {
            byte D;
            byte E;
        };
        unsigned short DE;
    };

    union {
        struct {
            byte H;
            byte L;
        };
        unsigned short HL;
    };

    // program counter - the memory address of the next instruction to be fetched.
    unsigned short PC;
    // stack pointer - the memory address of the top of the stack.
    unsigned short SP;

  public:

    Registers();

    // A Register
    byte getA() const;
    void setA(byte);
    // F Register
    byte getF() const;
    void setF(byte);
    // AF Register
    unsigned short getAF() const;
    void setAF(unsigned short);

    // B Register
    byte getB() const;
    void setB(byte);
    // C Register
    byte getC() const;
    void setC(byte);
    // BC Register
    unsigned short getBC() const;
    void setBC(unsigned short);

    // D Register
    byte getA() const;
    void setA(byte);
    // E Register
    byte getA() const;
    void setA(byte);
    // DE Register
    unsigned short getDE() const;
    void setDE(unsigned short);

    // H Register
    byte getA() const;
    void setA(byte);
    // L Register
    byte getA() const;
    void setA(byte);
    // BC Register
    unsigned short getBC() const;
    void setBC(unsigned short);

    // PC Register
    unsigned short getPC() const;
    void setPC(unsigned short);

    // SP Register
    unsigned short getSP() const;
    void setSP(unsigned short);
};
//----------------------------------------------------------------------------------------------

#endif