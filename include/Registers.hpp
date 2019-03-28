#ifndef REGISTERS_HPP_
#define REGISTERS_HPP_

//----------------------------------------------------------------------------------------------
typedef unsigned char Byte;
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
        unsigned short AF;
    };

    union {
        struct {
			Byte C;
            Byte B;
        };
        unsigned short BC;
    };

    union {
        struct {
            Byte E;
			Byte D;
        };
        unsigned short DE;
    };

    union {
        struct {
			Byte L;
            Byte H;
        };
        unsigned short HL;
    };

    // program counter - the memory address of the next instruction to be fetched.
    unsigned short PC;
    // stack pointer - the memory address of the top of the stack.
    unsigned short SP;

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
    unsigned short getAF() const;
    void setAF(unsigned short);


    // B Register
    Byte getB() const;
    void setB(Byte);

    // C Register
    Byte getC() const;
    void setC(Byte);

    // BC Register
    unsigned short getBC() const;
    void setBC(unsigned short);


    // D Register
    Byte getD() const;
    void setD(Byte);

    // E Register
    Byte getE() const;
    void setE(Byte);

    // DE Register
    unsigned short getDE() const;
    void setDE(unsigned short);


    // H Register
    Byte getH() const;
    void setH(Byte);

    // L Register
    Byte getL() const;
    void setL(Byte);

    // HL Register
    unsigned short getHL() const;
    void setHL(unsigned short);


    // PC Register
    unsigned short getPC() const;
    void setPC(unsigned short);

    // SP Register
    unsigned short getSP() const;
    void setSP(unsigned short);
};
//----------------------------------------------------------------------------------------------

#endif