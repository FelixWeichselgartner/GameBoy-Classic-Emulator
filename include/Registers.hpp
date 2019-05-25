#ifndef REGISTERS_HPP_
#define REGISTERS_HPP_

//----------------------------------------------------------------------------------------------
#include "datatypes.h"
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Flags - F register is the flag register
// Z = this->registers.F (Byte 7)
// N = this->registers.F (Byte 6)
// H = this->registers.F (Byte 5)
// C = this->registers.F (Byte 4)
// Byte 3 = Byte 2 = Byte 1 = Byte 0 = not used
//----------------------------------------------------------------------------------------------

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
	bool halt_bug_pc_no_increase;
    // stack pointer - the memory address of the top of the stack.
    Word SP;

  public:

    Registers();

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
	void incPC();
	void halt_bug();

    // SP Register
    Word getSP() const;
    void setSP(Word);
	void incSP();
	void decSP();

    // Flag handling.
	Byte getFlag(char);
	void setFlag(char);
	void resetFlag(char);
	void setFlagState(char, bool);
	void flipFlag(char);
	void resetFlagAll();
};
//----------------------------------------------------------------------------------------------

#endif