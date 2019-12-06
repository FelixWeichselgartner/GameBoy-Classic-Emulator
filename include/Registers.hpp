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

    void reset();

    Registers();

    // A Register
    Byte getA() const { return this->A; }
    void setA(Byte value) { this->A = value; }

    // F Register
    Byte getF() const { return this->F; }
    void setF(Byte value) { this->F = value & 0xF0; }

    // AF Register
    Word getAF() const { return this->AF; }
    void setAF(Word value) { this->AF = value & 0xFFF0; }


    // B Register
    Byte getB() const { return this->B; }
    void setB(Byte value) { this->B = value; }

    // C Register
    Byte getC() const { return this->C; }
    void setC(Byte value) { this->C = value; }

    // BC Register
    Word getBC() const { return this->BC; }
    void setBC(Word value) { this->BC = value; }


    // D Register
    Byte getD() const { return this->D; }
    void setD(Byte value) { this->D = value; }

    // E Register
    Byte getE() const { return this->E; }
    void setE(Byte value) { this->E = value; }

    // DE Register
    Word getDE() const { return this->DE; }
    void setDE(Word value) { this->DE = value; }


    // H Register
    Byte getH() const { return this->H; }
    void setH(Byte value) { this->H = value; }

    // L Register
    Byte getL() const { return this->L; }
    void setL(Byte value) { this->L = value; }

    // HL Register
    Word getHL() const { return this->HL; }
    void setHL(Word value) { this->HL = value; }


    // PC Register
    Word getPC() const { return this->PC; }
    void setPC(Word value) { this->PC = value; }
	void incPC();
	void halt_bug() { this->halt_bug_pc_no_increase = true; }

    // SP Register
    Word getSP() const { return this->SP; }
    void setSP(Word value) { this->SP = value; }
	void incSP() { this->SP++; }
	void decSP() { this->SP--; }

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