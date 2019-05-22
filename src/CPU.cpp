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
	this->this->memory.getEnableBootstrap() ? this->registers.setPC(0x0000) : this->registers.setPC(0x0100);
}

CPU::~CPU() {

}

void CPU::setJoypadLink(class Joypad* joypad) {
	if ((this->joypad = joypad) == NULL) exit(2);
	this->this->memory.setJoypadLink(joypad);
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
	this->memory.PushWord(this->registers.getPC() + 1);
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
Byte CyclesPerOPCodeConditionals[0x100] {
//   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	 1,  3,  2,  2,  1,  1,  2,  1,  5,  2,  2,  2,  1,  1,  2,  1, 	// 0x00
     0,  3,  2,  2,  1,  1,  2,  1,  3,  2,  2,  2,  1,  1,  2,  1, 	// 0x10
     3,  3,  2,  2,  1,  1,  2,  1,  3,  2,  2,  2,  1,  1,  2,  1, 	// 0x20
     3,  3,  2,  2,  3,  3,  3,  1,  3,  2,  2,  2,  1,  1,  2,  1, 	// 0x30
     1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x40
     1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x50
     1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x60
     2,  2,  2,  2,  2,  2,  0,  2,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x70
     1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x80
     1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0x90
     1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0xA0
     1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  2,  1, 	// 0xB0
     5,  3,  4,  4,  6,  4,  2,  4,  5,  4,  4,  0,  6,  6,  2,  4, 	// 0xC0
     5,  3,  4,  0,  6,  4,  2,  4,  5,  4,  4,  0,  6,  0,  2,  4, 	// 0xD0
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

void CPU::executeInstruction(Byte op) {
	CycleConditionals = false;
	
	(*opcode[op])();

	if (!CycleConditionals) {
		this->cycles += CyclesPerOPCode[op] * CYCLEFACTOR;
	} else {
		this->cycles += CyclesPerOPCodeConditionals[op] * CYCLEFACTOR;
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
	this->registers.incPC();
	Byte exOpcode = this->memory.ReadByte(this->registers.getPC());

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
			this->memory.WriteByte(this->registers.getHL(), rlc(this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), rrc(this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), rl(this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), rr(this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), sla(this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), sra(this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), swap(this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), srl(this->memory.ReadByte(this->registers.getHL())));
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
			bit(0, this->memory.ReadByte(this->registers.getHL()));
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
			bit(1, this->memory.ReadByte(this->registers.getHL()));
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
			bit(2, this->memory.ReadByte(this->registers.getHL()));
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
			bit(3, this->memory.ReadByte(this->registers.getHL()));
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
			bit(4, this->memory.ReadByte(this->registers.getHL()));
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
			bit(5, this->memory.ReadByte(this->registers.getHL()));
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
			bit(6, this->memory.ReadByte(this->registers.getHL()));
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
			bit(7, this->memory.ReadByte(this->registers.getHL()));
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
			this->memory.WriteByte(this->registers.getHL(), res(0, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), res(1, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), res(2, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), res(3, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), res(4, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), res(5, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), res(6, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), res(7, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(0, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(1, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(2, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(3, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(4, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(5, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(6, this->memory.ReadByte(this->registers.getHL())));
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
			this->memory.WriteByte(this->registers.getHL(), set(7, this->memory.ReadByte(this->registers.getHL())));
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

	executeInstruction(gb_halt ? 0x00 : this->memory.ReadByte(this->registers.getPC()));

	// may not increase program counter after jumps.
	if (!this->jump) {
		if (!gb_halt)
			this->registers.incPC();
	} else {
		this->jump = false;
	}

	return this->cycles;
}