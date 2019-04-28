#ifndef REGISTERS_HPP_
#define REGISTERS_HPP_

//----------------------------------------------------------------------------------------------
#include "datatypes.h"
//----------------------------------------------------------------------------------------------

#include <string>
std::string toBinary(int);

//----------------------------------------------------------------------------------------------
class Registers {

private:
    union {
        struct {
            Byte F;
			Byte A;
        };
        Word AF;
    };

    union {
        struct {
			Byte C;
            Byte B;
        };
        Word BC;
    };

    union {
        struct {
            Byte E;
			Byte D;
        };
        Word DE;
    };

    union {
        struct {
			Byte L;
            Byte H;
        };
        Word HL;
    };

    // program counter - the memory address of the next instruction to be fetched.
    Word PC;
    // stack pointer - the memory address of the top of the stack.
    Word SP;

  public:

    Registers();

    void printFlags();

    // A Register
    Byte getA() const;
    void setA(Byte);

    // F Register
    Byte getF() const;
    void setF(Byte);

    // AF Register
    Word getAF() const;
    void setAF(Word);


    // B Register
    Byte getB() const;
    void setB(Byte);

    // C Register
    Byte getC() const;
    void setC(Byte);

    // BC Register
    Word getBC() const;
    void setBC(Word);


    // D Register
    Byte getD() const;
    void setD(Byte);

    // E Register
    Byte getE() const;
    void setE(Byte);

    // DE Register
    Word getDE() const;
    void setDE(Word);


    // H Register
    Byte getH() const;
    void setH(Byte);

    // L Register
    Byte getL() const;
    void setL(Byte);

    // HL Register
    Word getHL() const;
    void setHL(Word);


    // PC Register
    Word getPC() const;
    void setPC(Word);

    // SP Register
    Word getSP() const;
    void setSP(Word);
};
//----------------------------------------------------------------------------------------------

#endif