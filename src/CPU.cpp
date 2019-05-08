//----------------------------------------------------------------------------------------------
#include "../include/CPU.hpp"
#include "../include/Registers.hpp"
#include "../include/RAM.hpp"
#include "../include/GameBoy.hpp"
#include "../include/format.hpp"

#include <iostream>
#include <iomanip>
using namespace std;
//----------------------------------------------------------------------------------------------

CPU::CPU() {
	this->joypad = NULL;
	this->running = 0x01;
	this->jump = false;
	this->gb_ime = false;
	this->cycles = 0;
	this->gb_halt = false;
	this->gb_stop = 0x00;
	this->memory.getEnableBootstrap() ? this->registers.setPC(0x0000) : this->registers.setPC(0x0100);
	this->memory.rom.load();
}

CPU::~CPU() {

}

void CPU::setJoypadLink(class Joypad* joypad) {
	if ((this->joypad = joypad) == NULL) exit(2);
	this->memory.setJoypadLink(joypad);
}

Byte CPU::getRunning() {
	return this->running;
}

void CPU::setRunning(Byte running) {
	this->running = running;
}

int CPU::getClockSpeed() const {
	return this->clockSpeed;
}

void CPU::daa() {
	Word s = this->registers.getA();

	if (registers.getFlag('N')) {
		if (registers.getFlag('H')) {
			s = (s - 0x06) & 0xFF;
		}
		if (registers.getFlag('C')) {
			s = s - 0x60;
		}
	} else {
		if (registers.getFlag('H') || (s & 0x0F) > 0x09) {
			s += 0x06;
		}
		if (registers.getFlag('C') || s > 0x9F) {
			s += 0x60;
		}
	}

	registers.resetFlag('H');

	if (s > 0xff) {
		registers.setFlag('C');
	}

	s &= 0xFF;

	registers.setFlagState('Z', !s);

	this->registers.setA((Byte)s);
}

////////////////////////////////////////////////////////////////
// shift operations

Byte CPU::rlca(Byte a) {
	Byte retval = (a << 1) | (a >> 7);

	// previous bit 7 value.
	registers.setFlagState('C', testBit(a, 7));

	// Z, N, H are reset.
	registers.resetFlag('Z');
	registers.resetFlag('N');
	registers.resetFlag('H');

	return retval;
}

Byte CPU::rla(Byte a) {
	Byte retval = (a << 1) | registers.getFlag('C');

	// previous bit 7 value.
	registers.setFlagState('C', testBit(a, 7));

	// Z, N, H are reset.
	registers.resetFlag('Z');
	registers.resetFlag('N');
	registers.resetFlag('H');

	return retval;
}

Byte CPU::rrca(Byte a) {
	Byte retval = (a >> 1) | (a << 7);

	// previous bit 0 value.
	registers.setFlagState('C', testBit(a, 0));

	// Z, N, H are reset.
	registers.resetFlag('Z');
	registers.resetFlag('N');
	registers.resetFlag('H');

	return retval;
}

Byte CPU::rra(Byte a) {
	Byte retval = (a >> 1) | (registers.getFlag('C') << 7);

	// previous bit 0 value.
	registers.setFlagState('C', testBit(a, 0));

	// Z, N, H are reset.
	registers.resetFlag('Z');
	registers.resetFlag('N');
	registers.resetFlag('H');

	return retval;
}

Byte CPU::rlc(Byte a) {
	Byte retval = (a << 1) | (a >> 7);

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');

	// C is set if bit 7 is 1, else reset.
	registers.setFlagState('C', testBit(a, 7));

	return retval;
}

Byte CPU::rl(Byte a) {
	Byte retval = (a << 1) | registers.getFlag('C');

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');

	// C is set if bit 7 is 1, else reset.
	registers.setFlagState('C', testBit(a, 7));

	return retval;
}

Byte CPU::rrc(Byte a) {
	Byte retval = a >> 1 | a << 7;

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');

	// C is set if bit 0 is 1, else reset.
	registers.setFlagState('C', testBit(a, 0));

	return retval;
}

Byte CPU::rr(Byte a) {
	Byte retval = (a >> 1) | (registers.getFlag('C') << 7);

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');

	// C is set if bit 0 is 1, else reset.
	registers.setFlagState('C', testBit(a, 0));

	return retval;
}

Byte CPU::sla(Byte value) {
	Byte retval = value << 1;

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');

	// C is set if bit 7 is 1, else reset.
	registers.setFlagState('C', testBit(value, 7));

	return retval;
}

Byte CPU::sra(Byte value) {
	Byte retval = (value >> 1) | (value & 0x80);

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');

	// C is set if bit 0 is 1, else reset.
	registers.setFlagState('C', testBit(value, 0));

	return retval;
}

Byte CPU::srl(Byte value) {
	Byte retval = value >> 1;

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');

	// C is set if bit 0 is 1, else reset.
	registers.setFlagState('C', testBit(value, 0));

	return retval;
}

////////////////////////////////////////////////////////////////
// arithmetical operations

Byte CPU::inc(Byte value) {
	Byte retval = value + 1;

	registers.setFlagState('Z', !retval);

	registers.resetFlag('N');

	registers.setFlagState('H', !(retval & 0x0F));

	return retval;
}

Byte CPU::dec(Byte value) {
	Byte retval = value - 1;

	registers.setFlagState('Z', !retval);

	registers.setFlag('N');

	registers.setFlagState('H', (retval & 0x0F) == 0x0F);

	return retval;
}

Byte CPU::add(Byte a, Byte b) {
	Word sum = (Word)a + b;

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !(sum & 0xFF));

    // N is set to zero
	registers.resetFlag('N');

    // H is set if overflow from bit 3, else reset
	registers.setFlagState('H', (a ^ b ^ sum) & 0x10);

    // C is set if overflow from bit 7, else reset
	registers.setFlagState('C', sum & 0xFF00);

	return (Byte)(sum & 0xFF);
}

Word CPU::addWord(Word a, Word b) {
	Word retval = a + b;
	unsigned int sum = (unsigned int)a + (unsigned int)b;

	// N is set to zero
	registers.resetFlag('N');

	// H is set if overflow from bit 11, else reset
	registers.setFlagState('H', (((Word)sum & 0xFFFF) ^ a ^ b) & 0x1000);

	// C is set if overflow from bit 15, else reset
	registers.setFlagState('C', sum & 0x00010000);

	return retval;
}

Word CPU::addSignedWord(Word a, Byte b) {
	sByte n = (sByte)b;
	int retval = a + n;

	registers.resetFlag('Z');
	registers.resetFlag('N');
	registers.setFlagState('H', (a ^ n ^ (retval & 0xFFFF)) & 0x10);
	registers.setFlagState('C', (a ^ n ^ (retval & 0xFFFF)) & 0x0100);

	return (Word)retval;
}

Word CPU::addSignedWordAddress(Word a, Byte b) {
	return (Word)(a + (short)((sByte)b));
}

Byte CPU::adc(Byte a, Byte b) {
	Word sum = (Word)a + b + registers.getFlag('C');

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !(sum & 0xFF));

	// N is set to zero
	registers.resetFlag('N');

	// H is set if overflow from bit 3, else reset
	registers.setFlagState('H', (a ^ b ^ sum) & 0x10);

	// C is set if overflow from bit 7, else reset
	registers.setFlagState('C', sum & 0xFF00);

	return (Byte)(sum & 0xFF);
}

Byte CPU::sub(Byte a, Byte b) {
	Word diff = (Word)a - b;

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !(diff & 0xFF));

	// N is set
	registers.setFlag('N');

	// H is set if no borrow from bit 4, else reset
	registers.setFlagState('H', (a ^ b ^ diff) & 0x10);

	// C is set if no borrow from bit 7, else reset
	registers.setFlagState('C', diff & 0xFF00);

	return (Byte)(diff & 0xFF);
}

Byte CPU::sbc(Byte a, Byte b) {
	Word diff = a - b - registers.getFlag('C');

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !(diff & 0xFF));

	// N is set
	registers.setFlag('N');

	// H is set if no borrow from bit 4, else reset
	registers.setFlagState('H', (a ^ b ^ diff) & 0x10);

	// C is set if no borrow from bit 7, else reset
	registers.setFlagState('C', diff & 0xFF00);

	return (Byte)(diff & 0xFF);
}

////////////////////////////////////////////////////////////////
// logical operations

Byte CPU::cpl(Byte a) {
	// N, H are set
	registers.setFlag('N');
	registers.setFlag('H');

	return ~a;
}

Byte CPU::land(Byte a, Byte b) {
	Byte retval = a & b;

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !retval);

	// N is reset
	registers.resetFlag('N');

	// H is set
	registers.setFlag('H');

	// C is reset
	registers.resetFlag('C');

	return retval;
}

Byte CPU::lxor(Byte a, Byte b) {
	Byte retval = a ^ b;

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !retval);

	// N, H, C are reset
	registers.resetFlag('N');
	registers.resetFlag('H');
	registers.resetFlag('C');

	return retval;
}

Byte CPU::lor(Byte a, Byte b) {
	Byte retval = a | b;

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !retval);

	// N, H, C is reset
	registers.resetFlag('N');
	registers.resetFlag('H');
	registers.resetFlag('C');

	return retval;
}

void CPU::cp(Byte A, Byte X) {
	Byte sub = A - X;

	// Z is set if result is zero, else reset
	registers.setFlagState('Z', !sub);

	// N is set
	registers.setFlag('N');

	// H is set if no borrow from bit 4, else reset
	registers.setFlagState('H', (A ^ X ^ sub) & 0x10);

	// C is set if no borrow from bit 7, else reset
	registers.setFlagState('C', X > A);
}

////////////////////////////////////////////////////////////////
// function calls

void CPU::call(Word address) {
	memory.PushWord(this->registers.getPC() + 1);
	this->registers.setPC(address);
	this->jump = true;
}

void CPU::rst(Word address) {
	call(address);
}

////////////////////////////////////////////////////////////////

void CPU::removed(Byte op) {
	cout << "operation " << HEX << (int)op << " was removed from this CPU." << endl;
}

Byte CyclesPerOPCode[0x100] {
//   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	 1,  3,  2,  2,  1,  1,  2,  1,  5,  2,  2,  2,  1,  1,  2,  1, 	// 0x00
	 0,  3,  2,  2,  1,  1,  2,  1,  3,  2,  2,  2,  1,  1,  2,  1, 	// 0x10
	 2,  3,  2,  2,  1,  1,  2,  1,  2,  2,  2,  2,  1,  1,  2,  1, 	// 0x20
	 2,  3,  2,  2,  3,  3,  3,  1,  2,  2,  2,  2,  1,  1,  2,  1, 	// 0x30
	 1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x40
	 1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x50
	 1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x60
	 2,  2,  2,  2,  2,  2,  0,  2,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x70
	 1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x80
	 1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x90
	 1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0xA0
	 1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0xB0
	 2,  3,  3,  4,  3,  4,  2,  4,  2,  4,  3,  0,  3,  6,  2,  4, 	// 0xC0
	 2,  3,  3,  0,  3,  4,  2,  4,  2,  4,  3,  0,  3,  0,  2,  4, 	// 0xD0
	 3,  3,  2,  0,  0,  4,  2,  4,  4,  1,  4,  0,  0,  0,  2,  4, 	// 0xE0
	 3,  3,  2,  1,  0,  4,  2,  4,  3,  2,  4,  1,  0,  0,  2,  4  	// 0xF0
};

Byte CyclesPerCBOPCode[0x100] = {
//   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0x00
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0x10
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0x20
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0x30
	 2,	 2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  2,  2,  2,  3,  2, 	// 0x40
	 2,	 2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  2,  2,  2,  3,  2, 	// 0x50
	 2,	 2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  2,  2,  2,  3,  2, 	// 0x60
	 2,	 2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  2,  2,  2,  3,  2, 	// 0x70
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0x80
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0x90
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0xA0
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0xB0
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0xC0
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0xD0
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2, 	// 0xE0
	 2,	 2,  2,  2,  2,  2,  4,  2,  2,  2,  2,  2,  2,  2,  4,  2 		// 0xF0
};

void CPU::executeInstruction(Byte opcode) {
	this->cycles += CyclesPerOPCode[opcode] * CYCLEFACTOR;
	Word jumpAddress;
	Byte jumpRelAddress;

    switch((int)opcode) {
        case  0x0: // NOP           no operation.
            break;
        case  0x1: // LD BC, nn   load 16-bit immediate into BC.
			this->registers.setBC(memory.LoadWord());
            break;
        case  0x2: // LD (BC), A    saves A to address pointed by BC.
            memory.WriteByte(this->registers.getBC(), this->registers.getA());
            break;
        case  0x3: // INC BC        increment 16-bit BC.
            this->registers.setBC(this->registers.getBC() + 1);
            break;
        case  0x4: // INC B        increment 8-bit B.
            this->registers.setB(inc(this->registers.getB()));
            break;
        case  0x5: // DEC B         decrement 8-bit B.
            this->registers.setB(dec(this->registers.getB()));
            break;
        case  0x6: // LD B, n       load 8 bit immediate into B.
			this->registers.setB(memory.LoadByte());
            break;
        case  0x7: // RLCA			rotate A left with carry.
			this->registers.setA(rlca(this->registers.getA()));
            break;
        case  0x8: // LD (nn), SP   save SP to a given address.
			memory.WriteWord(memory.LoadWord(), this->registers.getSP());
            break;
        case  0x9: // ADD HL, BC    add 16-bit BC to HL.
            this->registers.setHL(addWord(this->registers.getHL(), this->registers.getBC()));
            break;
        case  0xA: // LD A, (BC)    load A from address pointed by BC.  
            this->registers.setA(memory.ReadByte(this->registers.getBC()));
            break;
        case  0xB: // DEC BC        decrement 16-bit BC.
            this->registers.setBC(this->registers.getBC() - 1);
            break;
        case  0xC: // INC C         increment 8-bit C.
            this->registers.setC(inc(this->registers.getC()));
            break;
        case  0xD: // DEC C         decrement 8-bit C.
            this->registers.setC(dec(this->registers.getC()));
            break;
        case  0xE: // LD C, n       load 8-bit immediate to C.
			this->registers.setC(memory.LoadByte());
            break;
        case  0xF: // RRCA			rotate A right with carry.
			this->registers.setA(rrca(this->registers.getA()));
            break;
        case 0x10: // STOP          stop processor.
			this->gb_halt = true;
            break;
        case 0x11: // LD DE, nn     load 16-bit immediate into DE.
			this->registers.setDE(memory.LoadWord());
            break;
        case 0x12: // LD (DE), A    save A to address pointed by DE.
            memory.WriteByte(this->registers.getDE(), this->registers.getA());
            break;
        case 0x13: // INC DE        increment 16-bit DE.
            this->registers.setDE(this->registers.getDE() + 1);
            break;
        case 0x14: // INC D         increment 8-bit D.
            this->registers.setD(inc(this->registers.getD()));
            break;
        case 0x15: // DEC D         decrement 8-bit D.
            this->registers.setD(dec(this->registers.getD()));
            break;
        case 0x16: // LD D, n       load 8-bit immediate into D.
			this->registers.setD(memory.LoadByte());
            break;
        case 0x17: // RLA	        rotate A left.
			this->registers.setA(rla(this->registers.getA()));
            break;
        case 0x18: // JR n          relative jump by signed immediate.
			jumpRelAddress = memory.LoadByte();
			this->registers.setPC(this->registers.getPC() + (sByte) jumpRelAddress);
            break;
        case 0x19: // ADD HL, DE    add 16-bit DE to HL.
			this->registers.setHL(addWord(this->registers.getHL(), this->registers.getDE()));
            break;
        case 0x1A: // LD A, (DE)    load A from address pointed to by DE.
            this->registers.setA(memory.ReadByte(this->registers.getDE()));
            break;
        case 0x1B: // DEC DE        decrement 16-bit DE.
            this->registers.setDE(this->registers.getDE() - 1);
            break;
        case 0x1C: // INC E         increment 8-bit E.
            this->registers.setE(inc(this->registers.getE()));
            break;
        case 0x1D: // DEC E         decrement 8-bit E.
            this->registers.setE(dec(this->registers.getE()));
            break;
        case 0x1E: // LD E, n       Load 8-bit immediate into E.
			this->registers.setE(memory.LoadByte());
            break;
        case 0x1F: // RRA			rotate A right.
			this->registers.setA(rra(this->registers.getA()));
            break;
        case 0x20: // JR NZ, n      relative jump by signed immediate if last result was not zero.
			jumpRelAddress = memory.LoadByte();
			if (!registers.getFlag('Z')) {
				this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
			}
            break;
        case 0x21: // LD HL, nn     load 16-bit immediate into HL.
			this->registers.setHL(memory.LoadWord());
            break;
        case 0x22: // LDI (HL), A   save A to address pointed by HL, and increment HL.
			memory.WriteByte(this->registers.getHL(), this->registers.getA());
			this->registers.setHL(this->registers.getHL() + 1);
            break;
        case 0x23: // INC HL        increment 16-bit HL.
            this->registers.setHL(this->registers.getHL() + 1);
            break;
        case 0x24: // INC H         increment 8-bit H.
            this->registers.setH(inc(this->registers.getH()));
            break;
        case 0x25: // DEC H         decrement 8-bit H.
            this->registers.setH(dec(this->registers.getH()));
            break;
        case 0x26: // LD H, n       load 8-bit immediate into H.
			this->registers.setH(memory.LoadByte());
            break;
        case 0x27: // DAA           adjust A for BCD addition.
			daa();
            break;
        case 0x28: // JR Z, n       relative jump by signed immediate if last result was zero.
			jumpRelAddress = memory.LoadByte();
			if (registers.getFlag('Z')) {
				this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
			}
            break;
        case 0x29: // ADD HL, HL    add 16-bit HL to HL.
			this->registers.setHL(addWord(this->registers.getHL(), this->registers.getHL()));
            break;
        case 0x2A: // LDI A, (HL)   load A from address pointed to by HL, and increment HL.
			this->registers.setA(memory.ReadByte(this->registers.getHL()));
			this->registers.setHL(this->registers.getHL() + 1);
            break;
        case 0x2B: // DEC HL        decrement 16-bit HL.
            this->registers.setHL(this->registers.getHL() - 1);
            break;
        case 0x2C: // INC L         increment 8-bit L.
            this->registers.setL(inc(this->registers.getL()));
            break;
        case 0x2D: // DEC L         decrement 8-bit L.
            this->registers.setL(dec(this->registers.getL()));
            break;
        case 0x2E: // LD L, n       load 8-bit immediate into L.
			this->registers.setL(memory.LoadByte());
            break;
        case 0x2F: // CPL           complement (logical NOT) on A.
			this->registers.setA(cpl(this->registers.getA()));
            break;
        case 0x30: // JR NC, n      relative jump by signed immediate if last result caused no carry.
			jumpRelAddress = memory.LoadByte();
			if (!registers.getFlag('C')) {
				this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
			}
            break;
        case 0x31: // LD SP, nn     load 16-bit immediate into SP.
			this->registers.setSP(memory.LoadWord());
            break;
        case 0x32: // LDD (HL), A   save A to address pointed by HL, and decrement HL.
			memory.WriteByte(this->registers.getHL(), this->registers.getA());
			this->registers.setHL(this->registers.getHL() - 1);
            break;
        case 0x33: // INC SP        increment 16-bit SP.
            this->registers.setSP(this->registers.getSP() + 1);
            break;
        case 0x34: // INC (HL)      increment value pointed by HL.
            memory.WriteByte(this->registers.getHL(), inc(memory.ReadByte(this->registers.getHL())));
            break;
        case 0x35: // DEC (HL)      decrement value pointed by HL.
            memory.WriteByte(this->registers.getHL(), dec(memory.ReadByte(this->registers.getHL())));
            break;
        case 0x36: // LD (HL), n    load 8-bit immediate into address pointed by HL.
			memory.WriteByte(this->registers.getHL(), memory.LoadByte());
            break;
        case 0x37: // SCF           set carry flag.
			registers.resetFlag('N');
			registers.resetFlag('H');
			registers.setFlag('C');
            break;
        case 0x38: // JR C, n       relative jump by signed immediate if last result caused carry.
			jumpRelAddress = memory.LoadByte();
			if (registers.getFlag('C')) {
				this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
			}
            break;
        case 0x39: // ADD HL, SP    add 16-bit SP to HL.
            this->registers.setHL(addWord(this->registers.getHL(), this->registers.getSP()));
            break;
        case 0x3A: // LDD A, (HL)   load A from address pointed to by HL, and decrement HL.
			this->registers.setA(memory.ReadByte(this->registers.getHL()));
			this->registers.setHL(this->registers.getHL() - 1);
            break;
        case 0x3B: // DEC SP        decrement 16-bit SP.
            this->registers.setSP(this->registers.getSP() - 1);
            break;
        case 0x3C: // INC A         increment 8-bit A.
            this->registers.setA(inc(this->registers.getA()));
            break;
        case 0x3D: // DEC A         decrement 8-bit A.
            this->registers.setA(dec(this->registers.getA()));
            break;
        case 0x3E: // LD A, n       load 8-bit immediate into A.
			this->registers.setA(memory.LoadByte());
            break;
        case 0x3F: // CCF           complement carry flag.
			registers.resetFlag('N');
			registers.resetFlag('H');
			registers.flipFlag('C');
            break;
        case 0x40: // LD B, B       copy B to B.
            this->registers.setB(this->registers.getB());
            break;
        case 0x41: // LD B, C       copy C to B. 
            this->registers.setB(this->registers.getC());
            break;
        case 0x42: // LD B, D       copy D to B.
            this->registers.setB(this->registers.getD());
            break;
        case 0x43: // LD B, E       copy E to B.
            this->registers.setB(this->registers.getE());
            break;
        case 0x44: // LD B, H       copy H to B.
            this->registers.setB(this->registers.getH());
            break;
        case 0x45: // LD B, L       copy L to B.
            this->registers.setB(this->registers.getL());
            break;
        case 0x46: // LD B, (HL)    copy value pointed by HL to B.
            this->registers.setB(memory.ReadByte(this->registers.getHL()));
            break;
        case 0x47: // LD B, A       copy A to B.
            this->registers.setB(this->registers.getA());
            break;
        case 0x48: // LD C, B       copy B to C.
            this->registers.setC(this->registers.getB());
            break;
        case 0x49: // LD C, C       copy C to C.
            this->registers.setC(this->registers.getC());
            break;
        case 0x4A: // LD C, D       copy D to C.
            this->registers.setC(this->registers.getD());
            break;
        case 0x4B: // LD C, E       copy E to C.
            this->registers.setC(this->registers.getE());
            break;
        case 0x4C: // LD C, H       copy H to C.
            this->registers.setC(this->registers.getH());
            break;
        case 0x4D: // LD C, L       copy L to C.
            this->registers.setC(this->registers.getL());
            break;
        case 0x4E: // LD C, (HL)    copy value pointed by HL to C.
            this->registers.setC(memory.ReadByte(this->registers.getHL()));
            break;
        case 0x4F: // LD C, A       copy A to C.
            this->registers.setC(this->registers.getA());
            break;
        case 0x50: // LD D, B       copy B to D.
            this->registers.setD(this->registers.getB());
            break;
        case 0x51: // LD D, C       copy C to D.
            this->registers.setD(this->registers.getC());
            break;
        case 0x52: // LD D, D       copy D to D.
            this->registers.setD(this->registers.getD());
            break;
        case 0x53: // LD D, E       copy E to D.
            this->registers.setD(this->registers.getE());
            break;
        case 0x54: // LD D, H       copy H to D.
            this->registers.setD(this->registers.getH());
            break;
        case 0x55: // LD D, L       copy L to D.
            this->registers.setD(this->registers.getL());
            break;
        case 0x56: // LD D, (HL)    copy value pointed by HL to D.
            this->registers.setD(memory.ReadByte(this->registers.getHL()));
            break;
        case 0x57: // LD D, A       copy A to D.
            this->registers.setD(this->registers.getA());
            break;
        case 0x58: // LD E, B       copy B to E.
            this->registers.setE(this->registers.getB());
            break;
        case 0x59: // LD E, C       copy C to E.
            this->registers.setE(this->registers.getC());
            break;
        case 0x5A: // LD E, D       copy D to E.
            this->registers.setE(this->registers.getD());
            break;
        case 0x5B: // LD E, E       copy E to E.
            this->registers.setE(this->registers.getE());
            break;
        case 0x5C: // LD E, H       copy H to E.
            this->registers.setE(this->registers.getH());
            break;
        case 0x5D: // LD E, L       copy L to E.
            this->registers.setE(this->registers.getL());
            break;
        case 0x5E: // LD E, (HL)    copy value pointed by HL to E.
            this->registers.setE(memory.ReadByte(this->registers.getHL()));
            break;
        case 0x5F: // LD E, A       copy A to E.
            this->registers.setE(this->registers.getA());
            break;
        case 0x60: // LD H, B       copy B to H.
            this->registers.setH(this->registers.getB());
            break;
        case 0x61: // LD H, C       copy C to H.
            this->registers.setH(this->registers.getC());
            break;
        case 0x62: // LD H, D       copy D to H.
            this->registers.setH(this->registers.getD());
            break;
        case 0x63: // LD H, E       copy E to H.
            this->registers.setH(this->registers.getE());
            break;
        case 0x64: // LD H, H       copy H to H.
            this->registers.setH(this->registers.getH());
            break;
        case 0x65: // LD H, L       copy L to H.
            this->registers.setH(this->registers.getL());
            break;
        case 0x66: // LD H, (HL)    copy value pointed by HL to H.
            this->registers.setH(memory.ReadByte(this->registers.getHL()));
            break;
        case 0x67: // LD H, A       copy A to H.
            this->registers.setH(this->registers.getA());
            break;
        case 0x68: // LD L, B       copy B to L.
            this->registers.setL(this->registers.getB());
            break;
        case 0x69: // LD L, C       copy C to L.
            this->registers.setL(this->registers.getC());
            break; 
        case 0x6A: // LD L, D       copy D to L.
            this->registers.setL(this->registers.getD());
            break;
        case 0x6B: // LD L, E       copy E to L. 
            this->registers.setL(this->registers.getE());
            break;
        case 0x6C: // LD L, H       copy H to L.
            this->registers.setL(this->registers.getH());
            break;
        case 0x6D: // LD L, L       copy L to L.
            this->registers.setL(this->registers.getL());
            break;
        case 0x6E: // LD L, (HL)    copy value pointed by HL to L.
            this->registers.setL(memory.ReadByte(this->registers.getHL()));
            break;
        case 0x6F: // LD L, A       copy A to L.
            this->registers.setL(this->registers.getA());
            break;
        case 0x70: // LD (HL), B    copy B to address pointed by HL.
            memory.WriteByte(this->registers.getHL(), this->registers.getB());
            break;
        case 0x71: // LD (HL), C    copy C to address pointed by HL.
            memory.WriteByte(this->registers.getHL(), this->registers.getC());
            break;
        case 0x72: // LD (HL), D    copy D to address pointed by HL.
            memory.WriteByte(this->registers.getHL(), this->registers.getD());
            break;
        case 0x73: // LD (HL), E    copy E to address pointed by HL.
            memory.WriteByte(this->registers.getHL(), this->registers.getE());
            break;
        case 0x74: // LD (HL), H    copy H to address pointed by HL.
            memory.WriteByte(this->registers.getHL(), this->registers.getH());
            break;
        case 0x75: // LD (HL), L    copy L to address pointed by HL.
            memory.WriteByte(this->registers.getHL(), this->registers.getL());
            break;
        case 0x76: // HALT          halt processor.
			this->gb_halt = true;
			this->registers.setPC(this->registers.getPC() + 1);
            break;
        case 0x77: // LD (HL), A    copy A to address pointed by HL.
            memory.WriteByte(this->registers.getHL(), this->registers.getA());
            break;
        case 0x78: // LD A, B       copy B to A.
            this->registers.setA(this->registers.getB());
            break;
        case 0x79: // LD A, C       copy C to A.
            this->registers.setA(this->registers.getC());
            break;
        case 0x7A: // LD A, D       copy D to A.
            this->registers.setA(this->registers.getD());
            break;
        case 0x7B: // LD A, E       copy E to A.
            this->registers.setA(this->registers.getE());
            break;
        case 0x7C: // LD A, H       copy H to A.
            this->registers.setA(this->registers.getH());
            break;
        case 0x7D: // LD A, L       copy L to A.
            this->registers.setA(this->registers.getL());
            break;
        case 0x7E: // LD A, (HL)    copy value pointed by HL to A.
            this->registers.setA(memory.ReadByte(this->registers.getHL()));
            break;
        case 0x7F: // LD A, A       copy A to A.
            this->registers.setA(this->registers.getA());
            break;
        case 0x80: // ADD A, B      add B to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getB()));
            break;
        case 0x81: // ADD A, C      add C to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getC()));
            break;
        case 0x82: // ADD A, D      add D to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getD()));
            break;
        case 0x83: // ADD A, E      add E to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getE()));
            break;
        case 0x84: // ADD A, H      add H to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getH()));
            break;
        case 0x85: // ADD A, L      add L to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getL()));
            break;
        case 0x86: // ADD A, (HL)   add value pointed by HL to A.
            this->registers.setA(add(this->registers.getA(), memory.ReadByte(this->registers.getHL())));
            break;
        case 0x87: // ADD A, A      add A to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getA()));
            break;
        case 0x88: // ADC A, B      add B and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), this->registers.getB()));
            break;
        case 0x89: // ADC A, C      add C and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), this->registers.getC()));
            break;
        case 0x8A: // ADC A, D      add D and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), this->registers.getD()));
            break;
        case 0x8B: // ADC A, E      add E and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), this->registers.getE()));
            break;
        case 0x8C: // ADC A, H      add H and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), this->registers.getH()));
            break;
        case 0x8D: // ADC A, L      add L and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), this->registers.getL()));
            break;
        case 0x8E: // ADC A, (HL)   add value pointed by HL and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), memory.ReadByte(this->registers.getHL())));
            break;
        case 0x8F: // ADC A, A      add A and carry flag to A.
			this->registers.setA(adc(this->registers.getA(), this->registers.getA()));
            break;
        case 0x90: // SUB A, B      subtract B from A.
            this->registers.setA(sub(this->registers.getA(), this->registers.getB()));
            break;
        case 0x91: // SUB A, C      subtract C from A.
            this->registers.setA(sub(this->registers.getA(), this->registers.getC()));
            break;
        case 0x92: // SUB A, D      subtract D from A.
            this->registers.setA(sub(this->registers.getA(), this->registers.getD()));
            break;
        case 0x93: // SUB A, E      subtract E from A.
            this->registers.setA(sub(this->registers.getA(), this->registers.getE()));
            break;
        case 0x94: // SUB A, H      subtract H from A.
            this->registers.setA(sub(this->registers.getA(), this->registers.getH()));
            break;
        case 0x95: // SUB A, L      subtract L from A.
            this->registers.setA(sub(this->registers.getA(), this->registers.getL()));
            break;
        case 0x96: // SUB A, (HL)   subtract value pointed by HL from A.
            this->registers.setA(sub(this->registers.getA(), memory.ReadByte(this->registers.getHL())));
            break; 
        case 0x97: // SUB A, A      subtract A from A.
            this->registers.setA(sub(this->registers.getA(), this->registers.getA()));
            break;
        case 0x98: // SBC A, B      subtract B and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), this->registers.getB()));
            break;
        case 0x99: // SBC A, C      subtract C and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), this->registers.getC()));
            break;
        case 0x9A: // SBC A, D      subtract D and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), this->registers.getD()));
            break;
        case 0x9B: // SBC A, E      subtract E and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), this->registers.getE()));
            break;
        case 0x9C: // SBC A, H      subtract H and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), this->registers.getH()));
            break;
        case 0x9D: // SBC A, L      subtract L and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), this->registers.getL()));
            break;
        case 0x9E: // SBC A, (HL)   subtract value pointed by HL and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), memory.ReadByte(this->registers.getHL())));
            break;
        case 0x9F: // SBC A, A      subtract A and carry flag from A.
			this->registers.setA(sbc(this->registers.getA(), this->registers.getA()));
            break; 
        case 0xA0: // AND B         logical AND B against A.
            this->registers.setA(land(this->registers.getA(), this->registers.getB()));
            break;
        case 0xA1: // AND C         logical AND C against A.
            this->registers.setA(land(this->registers.getA(), this->registers.getC()));
            break;
        case 0xA2: // AND D         logical AND D against A.
            this->registers.setA(land(this->registers.getA(), this->registers.getD()));
            break;
        case 0xA3: // AND E         logical AND E against A.
            this->registers.setA(land(this->registers.getA(), this->registers.getE()));
            break;
        case 0xA4: // AND H         logical AND H against A.
            this->registers.setA(land(this->registers.getA(), this->registers.getH()));
            break;
        case 0xA5: // AND L         logical AND L against A.
            this->registers.setA(land(this->registers.getA(), this->registers.getL()));
            break; 
        case 0xA6: // AND (HL)      logical AND value pointed by HL against A.
            this->registers.setA(land(this->registers.getA(), memory.ReadByte(this->registers.getHL())));
            break;
        case 0xA7: // AND A         logical AND A against A.
            this->registers.setA(land(this->registers.getA(), this->registers.getA()));
            break;
        case 0xA8: // XOR B         logical XOR B against A.
            this->registers.setA(lxor(this->registers.getA(), this->registers.getB()));
            break;
        case 0xA9: // XOR C         logical XOR C against A.
            this->registers.setA(lxor(this->registers.getA(), this->registers.getC()));
            break;
        case 0xAA: // XOR D         logical XOR D against A.
            this->registers.setA(lxor(this->registers.getA(), this->registers.getD()));
            break;
        case 0xAB: // XOR E         logical XOR E against A.
            this->registers.setA(lxor(this->registers.getA(), this->registers.getE()));
            break;
        case 0xAC: // XOR H         logical XOR H against A.
            this->registers.setA(lxor(this->registers.getA(), this->registers.getH()));
            break;
        case 0xAD: // XOR L         logical XOR L against A.
            this->registers.setA(lxor(this->registers.getA(), this->registers.getL()));
            break;
        case 0xAE: // XOR (HL)      logical XOR value pointed by HL against A.
            this->registers.setA(lxor(this->registers.getA(), memory.ReadByte(this->registers.getHL())));
            break;
        case 0xAF: // XOR A         logical XOR A against A.
            this->registers.setA(lxor(this->registers.getA(), this->registers.getA()));
            break;
        case 0xB0: // OR B          logical OR B against A.
            this->registers.setA(lor(this->registers.getA(), this->registers.getB()));
            break;
        case 0xB1: // OR C          logical OR C against A.
            this->registers.setA(lor(this->registers.getA(), this->registers.getC()));
            break;
        case 0xB2: // OR D          logical OR D against A.
            this->registers.setA(lor(this->registers.getA(), this->registers.getD()));
            break;
        case 0xB3: // OR E          logical OR E against A.
            this->registers.setA(lor(this->registers.getA(), this->registers.getE()));
            break;
        case 0xB4: // OR H          logical OR H against A.
            this->registers.setA(lor(this->registers.getA(), this->registers.getH()));
            break;
        case 0xB5: // OR L          logical OR L against A. 
            this->registers.setA(lor(this->registers.getA(), this->registers.getL()));
            break;
        case 0xB6: // OR (HL)       logical OR value pointed by HL against A.
            this->registers.setA(lor(this->registers.getA(), memory.ReadByte(this->registers.getHL())));
            break;
        case 0xB7: // OR A          logical OR A against A.
            this->registers.setA(lor(this->registers.getA(), this->registers.getA()));
            break;
        case 0xB8: // CP B          compare B against A.
            cp(this->registers.getA(), this->registers.getB());
            break;
        case 0xB9: // CP C          compare C against A.
            cp(this->registers.getA(), this->registers.getC());
            break;
        case 0xBA: // CP D          compare D against A.
            cp(this->registers.getA(), this->registers.getD());
            break;
        case 0xBB: // CP E          compare E against A.
            cp(this->registers.getA(), this->registers.getE());
            break;
        case 0xBC: // CP H          compare H against A.
            cp(this->registers.getA(), this->registers.getH());
            break;
        case 0xBD: // CP L          compare L against A.
            cp(this->registers.getA(), this->registers.getL());
            break;
        case 0xBE: // CP (HL)       compare value pointed by HL against A.
            cp(this->registers.getA(), memory.ReadByte(this->registers.getHL()));
            break;
        case 0xBF: // CP A          compare A against A.
            cp(this->registers.getA(), this->registers.getA());
            break;
        case 0xC0: // RET NZ        return if last result was not zero.
			if (!registers.getFlag('Z')) {
				this->registers.setPC(memory.PopWord());
				jump = true;
			}
            break;
        case 0xC1: // POP BC        pop 16-bit value from stack into BC.
			this->registers.setBC(memory.PopWord());
            break;
        case 0xC2: // JP NZ, nn     absolute jump to 16-bit location if last result was not zero.
			jumpAddress = memory.LoadWord();
			if (!registers.getFlag('Z')) {
				this->registers.setPC(jumpAddress);
				this->jump = true;
			}
            break;
        case 0xC3: // JP nn         absolute jump to 16-bit location.
			this->registers.setPC(memory.LoadWord());
			jump = true;
            break;
        case 0xC4: // CALL NZ, nn   call routine at 16-bit location if last result was not zero.
			jumpAddress = memory.LoadWord();
			if (!registers.getFlag('Z')) {
				call(jumpAddress);
			}
            break;
        case 0xC5: // PUSH BC       push 16-bit BC onto stack.
			memory.PushWord(this->registers.getBC());
            break;
        case 0xC6: // ADD A, n      add 8-bit immediate to A. 
			this->registers.setA(add(this->registers.getA(), memory.LoadByte()));
            break;
        case 0xC7: // RST 0         call routine at address 0000h
			rst(0x0000);
            break;
        case 0xC8: // RET Z         return if last result was zero.
			if (registers.getFlag('Z')) {
				this->registers.setPC(memory.PopWord());
				jump = true;
			}
            break;
        case 0xC9: // RET           return to calling routine.
			this->registers.setPC(memory.PopWord());
			jump = true;
            break;
        case 0xCA: // JP Z, nn      absolute jump to 16-bit location if last result was zero.
			jumpAddress = memory.LoadWord();
			if (registers.getFlag('Z')) {
				this->registers.setPC(jumpAddress);
				this->jump = true;
			}
            break;
        case 0xCB: // Ext ops       extended operations (two-Byte instruction code).
			cycles += executeExtendedOpcodes();
            break;
        case 0xCC: // CALL Z, nn    call routine at 16-bit location if last result was zero.
			jumpAddress = memory.LoadWord();
			if (registers.getFlag('Z')) {
				call(jumpAddress);
			}
            break;
        case 0xCD: // CALL nn       call routine at 16-bit location.
			call(memory.LoadWord());
            break;
        case 0xCE: // ADC A, n      add 8-bit immediate and carry to A.
			this->registers.setA(adc(this->registers.getA(), memory.LoadByte()));
            break;
        case 0xCF: // RST 8         call routine at adress 0008h.
			rst(0x0008);
            break;
        case 0xD0: // RET NC        return if last result caused no carry.
			if (!registers.getFlag('C')) {
				this->registers.setPC(memory.PopWord());
				jump = true;
			}
            break;
        case 0xD1: // POP DE        pop 16-bit value from stack into DE.
			this->registers.setDE(memory.PopWord());
            break;
        case 0xD2: // JP NC, nn     absolute jump to 16-bit location if last result caused no carry.
			jumpAddress = memory.LoadWord();
			if (!registers.getFlag('C')) {
				this->registers.setPC(jumpAddress);
				this->jump = true;
			}
            break;
        case 0xD3: // XX            operation removed in this CPU.
			removed(0xD3);
            break;
        case 0xD4: // CALL NC, nn	call routine at 16-bit location if last result caused no carry.
			jumpAddress = memory.LoadWord();
			if (!registers.getFlag('C')) {
				call(jumpAddress);
			}
            break;
        case 0xD5: // PUSH DE       push 16-bit DE onto stack.
			memory.PushWord(this->registers.getDE());
            break;
        case 0xD6: // SUB A, n      subtract 8-bit immediate from A. 
			this->registers.setA(sub(this->registers.getA(), memory.LoadByte()));
            break;
        case 0xD7: // RST 10        call routine at address 0010h.
			rst(0x0010);
            break;
        case 0xD8: // RET C         return if last result caused carry.
			if (registers.getFlag('C')) {
				this->registers.setPC(memory.PopWord());
				jump = true;
			}
            break;
        case 0xD9: // RETI          enable interrupts and return to calling routine.
			this->gb_ime = true;
			this->jump = true;
			this->registers.setPC(memory.PopWord());
            break; 
        case 0xDA: // JP C, nn      absolute jump to 16-bit location if last result caused carry.
			jumpAddress = memory.LoadWord();
			if (registers.getFlag('C')) {
				this->registers.setPC(jumpAddress);
				this->jump = true;
			}
            break;
        case 0xDB: // XX            operation removed in this CPU. 
			removed(0xDB);
            break;
        case 0xDC: // CALL C, nn    call routine at 16-bit location if last result caused carry.
			jumpAddress = memory.LoadWord();
			if (registers.getFlag('C')) {
				call(jumpAddress);
			}
            break;
        case 0xDD: // XX            operation removed in this CPU.
			removed(0xDD);
            break;
        case 0xDE: // SBC A, n      subtract 8-bit immediate and carry from A. 
			this->registers.setA(sbc(this->registers.getA(), memory.LoadByte()));
            break;
        case 0xDF: // RST 18        call routine at address 0018h.
			rst(0x0018);
            break;
        case 0xE0: // LDH (n), A    save A at address pointed to by (FF00h + 8-bit immediate)
			memory.WriteByte(ADDR_IO + memory.LoadByte(), this->registers.getA());
            break;
        case 0xE1: // POP HL        pop 16-bit value from stack into HL.
			this->registers.setHL(memory.PopWord());
            break;
        case 0xE2: // LDH (C), A    save A at address pointed to by (FF00h + C)
			memory.WriteByte(ADDR_IO + this->registers.getC(), this->registers.getA());
            break;
        case 0xE3: // XX            operation removed in this CPU.
			removed(0xE3);
            break;
        case 0xE4: // XX            operation removed in this CPU.
			removed(0xE4);
            break;
        case 0xE5: // PUSH HL       Push 16-bit HL onto stack.
			memory.PushWord(this->registers.getHL());
            break;
        case 0xE6: // AND n         logical AND 8-bit immediate against A.
			this->registers.setA(land(this->registers.getA(), memory.LoadByte()));
            break;
        case 0xE7: // RST 20        call routine at address 0020h.
			rst(0x0020);
            break;
        case 0xE8: // ADD SP, d     add signed 8-bit immediate to SP.
			this->registers.setSP(addSignedWord(this->registers.getSP(), memory.LoadByte()));
            break;
        case 0xE9: // JP (HL)       jump to 16-bit value pointed by HL.
			this->registers.setPC(this->registers.getHL());
			this->jump = true;
            break;
        case 0xEA: // LD (nn), A    save A at given 16-bit address.
			memory.WriteByte(memory.LoadWord(), this->registers.getA());
            break;
        case 0xEB: // XX            operation removed in this CPU.
			removed(0xEB);
            break;
        case 0xEC: // XX            operation removed in this CPU.
			removed(0xEC);
            break;
        case 0xED: // XX            operation removed in this CPU.
			removed(0xED);
            break;
        case 0xEE: // XOR n         logical XOR 8-bit immediate against A.
			this->registers.setA(lxor(this->registers.getA(), memory.LoadByte()));
            break;
        case 0xEF: // RST 28        call routine at address 0028h.
			rst(0x0028);
            break;
        case 0xF0: // LDH A, (n)    load A from address pointed to by (FF00h + 8-bit immediate).
			this->registers.setA(memory.ReadByte(ADDR_IO + memory.LoadByte()));
            break;
        case 0xF1: // POP AF        pop 16-bit value from stack into AF.
			this->registers.setAF(memory.PopWord());
            break;
        case 0xF2: // LD A, (C)     load A from address pointed to by (FF00h + C)
			this->registers.setA(memory.ReadByte(ADDR_IO + this->registers.getC()));
            break;
        case 0xF3: // DI            disable interrupts
			this->gb_ime = false;
            break;
        case 0xF4: // XX            operation removed in this CPU.
			removed(0xF4);
            break;
        case 0xF5: // PUSH AF       push 16-bit AF onto stack.
			memory.PushWord(this->registers.getAF());
            break;
        case 0xF6: // OR n          logical OR 8-bit immediate against A.
			this->registers.setA(lor(this->registers.getA(), memory.LoadByte()));
            break;
        case 0xF7: // RST 30        call routine at address 0030h.
			rst(0x0030);
            break;
        case 0xF8: // LDHL SP, d    add signed 8-bit immediate to SP and save result in HL.
			this->registers.setHL(addSignedWord(this->registers.getSP(), memory.LoadByte()));
            break;
        case 0xF9: // LD SP, HL     copy HL to SP.
			this->registers.setSP(this->registers.getHL());
            break;
        case 0xFA: // LD A, (nn)    load A from given 16-bit address.
			this->registers.setA(memory.ReadByte(memory.LoadWord()));
            break;
        case 0xFB: // EI            enable interrupts.
			this->gb_ime = true;
            break;
        case 0xFC: // XX            operation removed in this CPU.
			removed(0xFC);
            break;
        case 0xFD: // XX            operation removed in this CPU.
			removed(0xFD);
            break;
        case 0xFE: // CP n          compare 8-bit immediate against A.
			cp(this->registers.getA(), memory.LoadByte());
            break;
        case 0xFF: // RST 38        call routine at address 0038h.
			rst(0x0038);
            break;
    }
}

Byte CPU::swap(Byte value) {
	Byte upperbyte = UPPER_HALFBYTE(value);
	Byte lowerbyte = LOWER_HALFBYTE(value);
	Byte retval = (lowerbyte << 4) | upperbyte;

	// Z is set if result is zero, else reset.
	registers.setFlagState('Z', !retval);

	// N, H, C are reset.
	registers.resetFlag('N');
	registers.resetFlag('H');
	registers.resetFlag('C');

	return retval;
}

void CPU::bit(int bit, Byte value) {
	// zero flag is set if bit is not set.
	registers.setFlagState('Z', !testBit(value, bit));

	// N is reset.
	registers.resetFlag('N');

	// H is set.
	registers.setFlag('H');
}

Byte CPU::res(int bit, Byte value) {
	// flags are not affected.

	return resetBit(value, bit);
}

Byte CPU::set(int bit, Byte value) {
	// flags are not affected.

	return setBit(value, bit);
}

int CPU::executeExtendedOpcodes() {
	this->registers.setPC(this->registers.getPC() + 1);
	Byte exOpcode = memory.ReadByte(this->registers.getPC());

	switch (exOpcode) {
		case  0x0: // RLC B			rotate B left with carry.
			this->registers.setB(rlc(this->registers.getB()));
			break;
		case  0x1: // RLC C
			this->registers.setC(rlc(this->registers.getC()));
			break;
		case  0x2: // RLC D
			this->registers.setD(rlc(this->registers.getD()));
			break;
		case  0x3: // RLC E
			this->registers.setE(rlc(this->registers.getE()));
			break;
		case  0x4: // RLC H
			this->registers.setH(rlc(this->registers.getH()));
			break;
		case  0x5: // RLC L
			this->registers.setL(rlc(this->registers.getL()));
			break;
		case  0x6: // RLC (HL)
			memory.WriteByte(this->registers.getHL(), rlc(memory.ReadByte(this->registers.getHL())));
			break;
		case  0x7: // RLC A
			this->registers.setA(rlc(this->registers.getA()));
			break;
		case  0x8: // RRC B			rotate B right with carry.
			this->registers.setB(rrc(this->registers.getB()));
			break;
		case  0x9: // RRC C
			this->registers.setC(rrc(this->registers.getC()));
			break;
		case  0xA: // RRC D
			this->registers.setD(rrc(this->registers.getD()));
			break;
		case  0xB: // RRC E 
			this->registers.setE(rrc(this->registers.getE()));
			break;
		case  0xC: // RRC H
			this->registers.setH(rrc(this->registers.getH()));
			break;
		case  0xD: // RRC L
			this->registers.setL(rrc(this->registers.getL()));
			break;
		case  0xE: // RRC (HL)
			memory.WriteByte(this->registers.getHL(), rrc(memory.ReadByte(this->registers.getHL())));
			break;
		case  0xF: // RRC A
			this->registers.setA(rrc(this->registers.getA()));
			break;
		case 0x10: // RL B			rotate B left.
			this->registers.setB(rl(this->registers.getB()));
			break;
		case 0x11: // RL C
			this->registers.setC(rl(this->registers.getC()));
			break;
		case 0x12: // RL D
			this->registers.setD(rl(this->registers.getD()));
			break;
		case 0x13: // RL E
			this->registers.setE(rl(this->registers.getE()));
			break;
		case 0x14: // RL H
			this->registers.setH(rl(this->registers.getH()));
			break;
		case 0x15: // RL L
			this->registers.setL(rl(this->registers.getL()));
			break;
		case 0x16: // RL (HL)
			memory.WriteByte(this->registers.getHL(), rl(memory.ReadByte(this->registers.getHL())));
			break;
		case 0x17: // RL A
			this->registers.setA(rl(this->registers.getA()));
			break;
		case 0x18: // RR B			rotate B right.
			this->registers.setB(rr(this->registers.getB()));
			break;
		case 0x19: // RR C
			this->registers.setC(rr(this->registers.getC()));
			break;
		case 0x1A: // RR D
			this->registers.setD(rr(this->registers.getD()));
			break;
		case 0x1B: // RR E
			this->registers.setE(rr(this->registers.getE()));
			break;
		case 0x1C: // RR H
			this->registers.setH(rr(this->registers.getH()));
			break;
		case 0x1D: // RR L
			this->registers.setL(rr(this->registers.getL()));
			break;
		case 0x1E: // RR (HL)
			memory.WriteByte(this->registers.getHL(), rr(memory.ReadByte(this->registers.getHL())));
			break;
		case 0x1F: // RR A
			this->registers.setA(rr(this->registers.getA()));
			break;
		case 0x20: // SLA B			shift B left preserving sign.
			this->registers.setB(sla(this->registers.getB()));
			break;
		case 0x21: // SLA C
			this->registers.setC(sla(this->registers.getC()));
			break;
		case 0x22: // SLA D
			this->registers.setD(sla(this->registers.getD()));
			break;
		case 0x23: // SLA E
			this->registers.setE(sla(this->registers.getE()));
			break;
		case 0x24: // SLA H
			this->registers.setH(sla(this->registers.getH()));
			break;
		case 0x25: // SLA L
			this->registers.setL(sla(this->registers.getL()));
			break;
		case 0x26: // SLA (HL)
			memory.WriteByte(this->registers.getHL(), sla(memory.ReadByte(this->registers.getHL())));
			break;
		case 0x27: // SLA A
			this->registers.setA(sla(this->registers.getA()));
			break;
		case 0x28: // SRA B			shift B right preserving sign.
			this->registers.setB(sra(this->registers.getB()));
			break;
		case 0x29: // SRA C
			this->registers.setC(sra(this->registers.getC()));
			break;
		case 0x2A: // SRA D
			this->registers.setD(sra(this->registers.getD()));
			break;
		case 0x2B: // SRA E
			this->registers.setE(sra(this->registers.getE()));
			break;
		case 0x2C: // SRA H
			this->registers.setH(sra(this->registers.getH()));
			break;
		case 0x2D: // SRA L
			this->registers.setL(sra(this->registers.getL()));
			break;
		case 0x2E: // SRA (HL)
			memory.WriteByte(this->registers.getHL(), sra(memory.ReadByte(this->registers.getHL())));
			break;
		case 0x2F: // SRA A
			this->registers.setA(sra(this->registers.getA()));
			break;
		case 0x30: // SWAP B		swap nybbles in B.
			this->registers.setB(swap(this->registers.getB()));
			break;
		case 0x31: // SWAP C
			this->registers.setC(swap(this->registers.getC()));
			break;
		case 0x32: // SWAP D
			this->registers.setD(swap(this->registers.getD()));
			break;
		case 0x33: // SWAP E
			this->registers.setE(swap(this->registers.getE()));
			break;
		case 0x34: // SWAP H
			this->registers.setH(swap(this->registers.getH()));
			break;
		case 0x35: // SWAP L
			this->registers.setL(swap(this->registers.getL()));
			break;
		case 0x36: // SWAP (HL)
			memory.WriteByte(this->registers.getHL(), swap(memory.ReadByte(this->registers.getHL())));
			break;
		case 0x37: // SWAP A
			this->registers.setA(swap(this->registers.getA()));
			break;
		case 0x38: // SRL B			shift B right.
			this->registers.setB(srl(this->registers.getB()));
			break;
		case 0x39: // SRL C
			this->registers.setC(srl(this->registers.getC()));
			break;
		case 0x3A: // SRL D
			this->registers.setD(srl(this->registers.getD()));
			break;
		case 0x3B: // SRL E
			this->registers.setE(srl(this->registers.getE()));
			break;
		case 0x3C: // SRL H
			this->registers.setH(srl(this->registers.getH()));
			break;
		case 0x3D: // SRL L
			this->registers.setL(srl(this->registers.getL()));
			break;
		case 0x3E: // SRL (HL)
			memory.WriteByte(this->registers.getHL(), srl(memory.ReadByte(this->registers.getHL())));
			break;
		case 0x3F: // SRL A
			this->registers.setA(srl(this->registers.getA()));
			break;
		case 0x40: // BIT 0, B		test bit 0 of B.
			bit(0, this->registers.getB());
			break;
		case 0x41: // BIT 0, C
			bit(0, this->registers.getC());
			break;
		case 0x42: // BIT 0, D
			bit(0, this->registers.getD());
			break;
		case 0x43: // BIT 0, E
			bit(0, this->registers.getE());
			break;
		case 0x44: // BIT 0, H
			bit(0, this->registers.getH());
			break;
		case 0x45: // BIT 0, L
			bit(0, this->registers.getL());
			break;
		case 0x46: // BIT 0, (HL)
			bit(0, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x47: // BIT 0, A
			bit(0, this->registers.getA());
			break;
		case 0x48: // BIT 1, B
			bit(1, this->registers.getB());
			break;
		case 0x49: // BIT 1, C
			bit(1, this->registers.getC());
			break;
		case 0x4A: // BIT 1, D
			bit(1, this->registers.getD());
			break;
		case 0x4B: // BIT 1, E
			bit(1, this->registers.getE());
			break;
		case 0x4C: // BIT 1, H
			bit(1, this->registers.getH());
			break;
		case 0x4D: // BIT 1, L
			bit(1, this->registers.getL());
			break;
		case 0x4E: // BIT 1, (HL)
			bit(1, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x4F: // BIT 1, A
			bit(1, this->registers.getA());
			break;
		case 0x50: // BIT 2, B
			bit(2, this->registers.getB());
			break;
		case 0x51: // BIT 2, C
			bit(2, this->registers.getC());
			break;
		case 0x52: // BIT 2, D
			bit(2, this->registers.getD());
			break;
		case 0x53: // BIT 2, E
			bit(2, this->registers.getE());
			break;
		case 0x54: // BIT 2, H
			bit(2, this->registers.getH());
			break;
		case 0x55: // BIT 2, L
			bit(2, this->registers.getL());
			break;
		case 0x56: // BIT 2, (HL)
			bit(2, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x57: // BIT 2, A
			bit(2, this->registers.getA());
			break;
		case 0x58: // BIT 3, B
			bit(3, this->registers.getB());
			break;
		case 0x59: // BIT 3, C
			bit(3, this->registers.getC());
			break;
		case 0x5A: // BIT 3, D
			bit(3, this->registers.getD());
			break;
		case 0x5B: // BIT 3, E
			bit(3, this->registers.getE());
			break;
		case 0x5C: // BIT 3, H
			bit(3, this->registers.getH());
			break;
		case 0x5D: // BIT 3, L
			bit(3, this->registers.getL());
			break;
		case 0x5E: // BIT 3, (HL)
			bit(3, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x5F: // BIT 3, A
			bit(3, this->registers.getA());
			break;
		case 0x60: // BIT 4, B
			bit(4, this->registers.getB());
			break;
		case 0x61: // BIT 4, C
			bit(4, this->registers.getC());
			break;
		case 0x62: // BIT 4, D
			bit(4, this->registers.getD());
			break;
		case 0x63: // BIT 4, E
			bit(4, this->registers.getE());
			break;
		case 0x64: // BIT 4, H
			bit(4, this->registers.getH());
			break;
		case 0x65: // BIT 4, L
			bit(4, this->registers.getL());
			break;
		case 0x66: // BIT 4, (HL)
			bit(4, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x67: // BIT 4, A
			bit(4, this->registers.getA());
			break;
		case 0x68: // BIT 5, B
			bit(5, this->registers.getB());
			break;
		case 0x69: // BIT 5, C
			bit(5, this->registers.getC());
			break;
		case 0x6A: // BIT 5, D
			bit(5, this->registers.getD());
			break;
		case 0x6B: // BIT 5, E
			bit(5, this->registers.getE());
			break;
		case 0x6C: // BIT 5, H
			bit(5, this->registers.getH());
			break;
		case 0x6D: // BIT 5, L
			bit(5, this->registers.getL());
			break;
		case 0x6E: // BIT 5, (HL)
			bit(5, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x6F: // BIT 5, A
			bit(5, this->registers.getA());
			break;
		case 0x70: // BIT 6, B
			bit(6, this->registers.getB());
			break;
		case 0x71: // BIT 6, C
			bit(6, this->registers.getC());
			break;
		case 0x72: // BIT 6, D
			bit(6, this->registers.getD());
			break;
		case 0x73: // BIT 6, E
			bit(6, this->registers.getE());
			break;
		case 0x74: // BIT 6, H
			bit(6, this->registers.getH());
			break;
		case 0x75: // BIT 6, L
			bit(6, this->registers.getL());
			break;
		case 0x76: // BIT 6, (HL)
			bit(6, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x77: // BIT 6, A
			bit(6, this->registers.getA());
			break;
		case 0x78: // BIT 7, B
			bit(7, this->registers.getB());
			break;
		case 0x79: // BIT 7, C
			bit(7, this->registers.getC());
			break;
		case 0x7A: // BIT 7, D
			bit(7, this->registers.getD());
			break;
		case 0x7B: // BIT 7, E
			bit(7, this->registers.getE());
			break;
		case 0x7C: // BIT 7, H
			bit(7, this->registers.getH());
			break;
		case 0x7D: // BIT 7, L
			bit(7, this->registers.getL());
			break;
		case 0x7E: // BIT 7, (HL)
			bit(7, memory.ReadByte(this->registers.getHL()));
			break;
		case 0x7F: // BIT 7, A
			bit(7, this->registers.getA());
			break;
		case 0x80: // RES 0, B		clear (reset) bit 0 of B.
			this->registers.setB(res(0, this->registers.getB()));
			break;
		case 0x81: // RES 0, C
			this->registers.setC(res(0, this->registers.getC()));
			break;
		case 0x82: // RES 0, D
			this->registers.setD(res(0, this->registers.getD()));
			break;
		case 0x83: // RES 0, E
			this->registers.setE(res(0, this->registers.getE()));
			break;
		case 0x84: // RES 0, H
			this->registers.setH(res(0, this->registers.getH()));
			break;
		case 0x85: // RES 0, L
			this->registers.setL(res(0, this->registers.getL()));
			break;
		case 0x86: // RES 0, (HL)
			memory.WriteByte(this->registers.getHL(), res(0, memory.ReadByte(this->registers.getHL())));
			break;
		case 0x87: // RES 0, A
			this->registers.setA(res(0, this->registers.getA()));
			break;
		case 0x88: // RES 1, B
			this->registers.setB(res(1, this->registers.getB()));
			break;
		case 0x89: // RES 1, C
			this->registers.setC(res(1, this->registers.getC()));
			break;
		case 0x8A: // RES 1, D
			this->registers.setD(res(1, this->registers.getD()));
			break;
		case 0x8B: // RES 1, E
			this->registers.setE(res(1, this->registers.getE()));
			break;
		case 0x8C: // RES 1, H
			this->registers.setH(res(1, this->registers.getH()));
			break;
		case 0x8D: // RES 1, L
			this->registers.setL(res(1, this->registers.getL()));
			break;
		case 0x8E: // RES 1, (HL)
			memory.WriteByte(this->registers.getHL(), res(1, memory.ReadByte(this->registers.getHL())));
			break;
		case 0x8F: // RES 1, A
			this->registers.setA(res(1, this->registers.getA()));
			break;
		case 0x90: // RES 2, B
			this->registers.setB(res(2, this->registers.getB()));
			break;
		case 0x91: // RES 2, C
			this->registers.setC(res(2, this->registers.getC()));
			break;
		case 0x92: // RES 2, D
			this->registers.setD(res(2, this->registers.getD()));
			break;
		case 0x93: // RES 2, E
			this->registers.setE(res(2, this->registers.getE()));
			break;
		case 0x94: // RES 2, H
			this->registers.setH(res(2, this->registers.getH()));
			break;
		case 0x95: // RES 2, L
			this->registers.setL(res(2, this->registers.getL()));
			break;
		case 0x96: // RES 2, (HL)
			memory.WriteByte(this->registers.getHL(), res(2, memory.ReadByte(this->registers.getHL())));
			break;
		case 0x97: // RES 2, A
			this->registers.setA(res(2, this->registers.getA()));
			break;
		case 0x98: // RES 3, B
			this->registers.setB(res(3, this->registers.getB()));
			break;
		case 0x99: // RES 3, C
			this->registers.setC(res(3, this->registers.getC()));
			break;
		case 0x9A: // RES 3, D
			this->registers.setD(res(3, this->registers.getD()));
			break;
		case 0x9B: // RES 3, E
			this->registers.setE(res(3, this->registers.getE()));
			break;
		case 0x9C: // RES 3, H
			this->registers.setH(res(3, this->registers.getH()));
			break;
		case 0x9D: // RES 3, L
			this->registers.setL(res(3, this->registers.getL()));
			break;
		case 0x9E: // RES 3, (HL)
			memory.WriteByte(this->registers.getHL(), res(3, memory.ReadByte(this->registers.getHL())));
			break;
		case 0x9F: // RES 3, A
			this->registers.setA(res(3, this->registers.getA()));
			break;
		case 0xA0: // RES 4, B
			this->registers.setB(res(4, this->registers.getB()));
			break;
		case 0xA1: // RES 4, C
			this->registers.setC(res(4, this->registers.getC()));
			break;
		case 0xA2: // RES 4, D
			this->registers.setD(res(4, this->registers.getD()));
			break;
		case 0xA3: // RES 4, E
			this->registers.setE(res(4, this->registers.getE()));
			break;
		case 0xA4: // RES 4, H
			this->registers.setH(res(4, this->registers.getH()));
			break;
		case 0xA5: // RES 4, L
			this->registers.setL(res(4, this->registers.getL()));
			break;
		case 0xA6: // RES 4, (HL)
			memory.WriteByte(this->registers.getHL(), res(4, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xA7: // RES 4, A
			this->registers.setA(res(4, this->registers.getA()));
			break;
		case 0xA8: // RES 5, B
			this->registers.setB(res(5, this->registers.getB()));
			break;
		case 0xA9: // RES 5, C
			this->registers.setC(res(5, this->registers.getC()));
			break;
		case 0xAA: // RES 5, D
			this->registers.setD(res(5, this->registers.getD()));
			break;
		case 0xAB: // RES 5, E
			this->registers.setE(res(5, this->registers.getE()));
			break;
		case 0xAC: // RES 5, H
			this->registers.setH(res(5, this->registers.getH()));
			break;
		case 0xAD: // RES 5, L
			this->registers.setL(res(5, this->registers.getL()));
			break;
		case 0xAE: // RES 5, (HL)
			memory.WriteByte(this->registers.getHL(), res(5, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xAF: // RES 5, A
			this->registers.setA(res(5, this->registers.getA()));
			break;
		case 0xB0: // RES 6, B
			this->registers.setB(res(6, this->registers.getB()));
			break;
		case 0xB1: // RES 6, C
			this->registers.setC(res(6, this->registers.getC()));
			break;
		case 0xB2: // RES 6, D
			this->registers.setD(res(6, this->registers.getD()));
			break;
		case 0xB3: // RES 6, E
			this->registers.setE(res(6, this->registers.getE()));
			break;
		case 0xB4: // RES 6, H
			this->registers.setH(res(6, this->registers.getH()));
			break;
		case 0xB5: // RES 6, L
			this->registers.setL(res(6, this->registers.getL()));
			break;
		case 0xB6: // RES 6, (HL)
			memory.WriteByte(this->registers.getHL(), res(6, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xB7: // RES 6, A
			this->registers.setA(res(6, this->registers.getA()));
			break;
		case 0xB8: // RES 7, B
			this->registers.setB(res(7, this->registers.getB()));
			break;
		case 0xB9: // RES 7, C
			this->registers.setC(res(7, this->registers.getC()));
			break;
		case 0xBA: // RES 7, D
			this->registers.setD(res(7, this->registers.getD()));
			break;
		case 0xBB: // RES 7, E
			this->registers.setE(res(7, this->registers.getE()));
			break;
		case 0xBC: // RES 7, H
			this->registers.setH(res(7, this->registers.getH()));
			break;
		case 0xBD: // RES 7, L
			this->registers.setL(res(7, this->registers.getL()));
			break;
		case 0xBE: // RES 7, (HL)
			memory.WriteByte(this->registers.getHL(), res(7, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xBF: // RES 7, A
			this->registers.setA(res(7, this->registers.getA()));
			break;
		case 0xC0: // SET 0, B
			this->registers.setB(set(0, this->registers.getB()));
			break;
		case 0xC1: // SET 0, C
			this->registers.setC(set(0, this->registers.getC()));
			break;
		case 0xC2: // SET 0, D
			this->registers.setD(set(0, this->registers.getD()));
			break;
		case 0xC3: // SET 0, E
			this->registers.setE(set(0, this->registers.getE()));
			break;
		case 0xC4: // SET 0, H
			this->registers.setH(set(0, this->registers.getH()));
			break;
		case 0xC5: // SET 0, L
			this->registers.setL(set(0, this->registers.getL()));
			break;
		case 0xC6: // SET 0, (HL)
			memory.WriteByte(this->registers.getHL(), set(0, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xC7: // SET 0, A
			this->registers.setA(set(0, this->registers.getA()));
			break;
		case 0xC8: // SET 1, B
			this->registers.setB(set(1, this->registers.getB()));
			break;
		case 0xC9: // SET 1, C
			this->registers.setC(set(1, this->registers.getC()));
			break;
		case 0xCA: // SET 1, D
			this->registers.setD(set(1, this->registers.getD()));
			break;
		case 0xCB: // SET 1, E
			this->registers.setE(set(1, this->registers.getE()));
			break;
		case 0xCC: // SET 1, H
			this->registers.setH(set(1, this->registers.getH()));
			break;
		case 0xCD: // SET 1, L
			this->registers.setL(set(1, this->registers.getL()));
			break;
		case 0xCE: // SET 1, (HL)
			memory.WriteByte(this->registers.getHL(), set(1, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xCF: // SET 1, A
			this->registers.setA(set(1, this->registers.getA()));
			break;
		case 0xD0: // SET 2, B
			this->registers.setB(set(2, this->registers.getB()));
			break;
		case 0xD1: // SET 2, C
			this->registers.setC(set(2, this->registers.getC()));
			break;
		case 0xD2: // SET 2, D
			this->registers.setD(set(2, this->registers.getD()));
			break;
		case 0xD3: // SET 2, E
			this->registers.setE(set(2, this->registers.getE()));
			break;
		case 0xD4: // SET 2, H
			this->registers.setH(set(2, this->registers.getH()));
			break;
		case 0xD5: // SET 2, L
			this->registers.setL(set(2, this->registers.getL()));
			break;
		case 0xD6: // SET 2, (HL)
			memory.WriteByte(this->registers.getHL(), set(2, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xD7: // SET 2, A
			this->registers.setA(set(2, this->registers.getA()));
			break;
		case 0xD8: // SET 3, B
			this->registers.setB(set(3, this->registers.getB()));
			break;
		case 0xD9: // SET 3, C
			this->registers.setC(set(3, this->registers.getC()));
			break;
		case 0xDA: // SET 3, D
			this->registers.setD(set(3, this->registers.getD()));
			break;
		case 0xDB: // SET 3, E
			this->registers.setE(set(3, this->registers.getE()));
			break;
		case 0xDC: // SET 3, H
			this->registers.setH(set(3, this->registers.getH()));
			break;
		case 0xDD: // SET 3, L
			this->registers.setL(set(3, this->registers.getL()));
			break;
		case 0xDE: // SET 3, (HL)
			memory.WriteByte(this->registers.getHL(), set(3, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xDF: // SET 3, A
			this->registers.setA(set(3, this->registers.getA()));
			break;
		case 0xE0: // SET 4, B
			this->registers.setB(set(4, this->registers.getB()));
			break;
		case 0xE1: // SET 4, C
			this->registers.setC(set(4, this->registers.getC()));
			break;
		case 0xE2: // SET 4, D
			this->registers.setD(set(4, this->registers.getD()));
			break;
		case 0xE3: // SET 4, E
			this->registers.setE(set(4, this->registers.getE()));
			break;
		case 0xE4: // SET 4, H
			this->registers.setH(set(4, this->registers.getH()));
			break;
		case 0xE5: // SET 4, L
			this->registers.setL(set(4, this->registers.getL()));
			break;
		case 0xE6: // SET 4, (HL)
			memory.WriteByte(this->registers.getHL(), set(4, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xE7: // SET 4, A
			this->registers.setA(set(4, this->registers.getA()));
			break;
		case 0xE8: // SET 5, B
			this->registers.setB(set(5, this->registers.getB()));
			break;
		case 0xE9: // SET 5, C
			this->registers.setC(set(5, this->registers.getC()));
			break;
		case 0xEA: // SET 5, D
			this->registers.setD(set(5, this->registers.getD()));
			break;
		case 0xEB: // SET 5, E
			this->registers.setE(set(5, this->registers.getE()));
			break;
		case 0xEC: // SET 5, H
			this->registers.setH(set(5, this->registers.getH()));
			break;
		case 0xED: // SET 5, L
			this->registers.setL(set(5, this->registers.getL()));
			break;
		case 0xEE: // SET 5, (HL)
			memory.WriteByte(this->registers.getHL(), set(5, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xEF: // SET 5, A
			this->registers.setA(set(5, this->registers.getA()));
			break;
		case 0xF0: // SET 6, B
			this->registers.setB(set(6, this->registers.getB()));
			break;
		case 0xF1: // SET 6, C
			this->registers.setC(set(6, this->registers.getC()));
			break;
		case 0xF2: // SET 6, D
			this->registers.setD(set(6, this->registers.getD()));
			break;
		case 0xF3: // SET 6, E
			this->registers.setE(set(6, this->registers.getE()));
			break;
		case 0xF4: // SET 6, H
			this->registers.setH(set(6, this->registers.getH()));
			break;
		case 0xF5: // SET 6, L
			this->registers.setL(set(6, this->registers.getL()));
			break;
		case 0xF6: // SET 6, (HL)
			memory.WriteByte(this->registers.getHL(), set(6, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xF7: // SET 6, A
			this->registers.setA(set(6, this->registers.getA()));
			break;
		case 0xF8: // SET 7, B
			this->registers.setB(set(7, this->registers.getB()));
			break;
		case 0xF9: // SET 7, C
			this->registers.setC(set(7, this->registers.getC()));
			break;
		case 0xFA: // SET 7, D
			this->registers.setD(set(7, this->registers.getD()));
			break;
		case 0xFB: // SET 7, E
			this->registers.setE(set(7, this->registers.getE()));
			break;
		case 0xFC: // SET 7, H
			this->registers.setH(set(7, this->registers.getH()));
			break;
		case 0xFD: // SET 7, L
			this->registers.setL(set(7, this->registers.getL()));
			break;
		case 0xFE: // SET 7, (HL)
			memory.WriteByte(this->registers.getHL(), set(7, memory.ReadByte(this->registers.getHL())));
			break;
		case 0xFF: // SET 7, A
			this->registers.setA(set(7, this->registers.getA()));
			break;
	}

	return CyclesPerCBOPCode[exOpcode] * CYCLEFACTOR;
}

void CPU::UpdateTimers(int cycles) {
	timer.update(cycles);
}

int CPU::CPUstep() {
	this->cycles = 0;

	executeInstruction(gb_halt ? 0x00 : memory.ReadByte(this->registers.getPC()));

	// may not increase program counter after jumps.
	if (!this->jump) {
		if (!gb_halt)
			this->registers.setPC(this->registers.getPC() + 1);
	} else {
		this->jump = false;
	}

	return this->cycles;
}