#ifndef REGISTER_HPP_
#define REGISTER_HPP_

// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

//----------------------------------------------------------------------------------------------
class Register {

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

    Register();
    // A register
    byte getA() const;
    void setA(byte);
    // F register
    byte getF() const;
    void setF(byte);
    // AF register
    unsigned short getAF() const;
    void setAF(unsigned short);

    // B register
    byte getB() const;
    void setB(byte);
    // C register
    byte getC() const;
    void setC(byte);
    // BC register
    unsigned short getBC() const;
    void setBC(unsigned short);

    // D register
    byte getA() const;
    void setA(byte);
    // E register
    byte getA() const;
    void setA(byte);
    // DE register
    unsigned short getDE() const;
    void setDE(unsigned short);

    // H register
    byte getA() const;
    void setA(byte);
    // L register
    byte getA() const;
    void setA(byte);
    // BC register
    unsigned short getBC() const;
    void setBC(unsigned short);

    // PC register
    unsigned short getPC() const;
    void setPC(unsigned short);

    // SP register
    unsigned short getSP() const;
    void setSP(unsigned short);
};
//----------------------------------------------------------------------------------------------

#endif