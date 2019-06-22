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

void CPU::resetVar() {
    this->joypad = NULL;
    this->running = 0x01;
    this->jump = false;
    this->gb_ime = false;
    this->cycles = 0;
    this->gb_halt = false;
    this->gb_stop = 0x00;
    this->memory.getEnableBootstrap() ? this->registers.setPC(0x0000) : this->registers.setPC(0x0100);
    this->CycleConditionals = false;
    this->jumpRelAddress = 0x00;
    this->jumpAddress = 0x0000;
    this->gb_halt_bug = 0x00;
}

void CPU::reset() {
    resetVar();
    this->registers.reset();
    this->timer.reset();
    this->memory.reset();
}

CPU::CPU() {
    resetVar();
	Init_OPCODE();
	Init_CBOPCODE();
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

////////////////////////////////////////////////////////////////
// cb opcodes

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

////////////////////////////////////////////////////////////////

void CPU::UpdateTimers(int cycles) {
	timer.update(cycles);
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
	this->CycleConditionals = false;

	(this->*opcode[op])();

	if (!this->CycleConditionals) {
		this->cycles += CyclesPerOPCode[op] * this->CYCLEFACTOR;
	} else {
		this->cycles += CyclesPerOPCodeConditionals[op] * this->CYCLEFACTOR;
	}
}

int CPU::executeExtendedOpcodes() {
	this->registers.incPC();
	Byte cb_op = this->memory.ReadByte(this->registers.getPC());

	(this->*cb_opcode[cb_op])();

	return CyclesPerCBOPCode[cb_op] * this->CYCLEFACTOR;
}

void CPU::halt_bug() {
	if (!this->gb_ime && (this->memory.ReadByte(0xFF0F) & this->memory.ReadByte(0xFFFF) & 0x1F)) {
	//if (!this->gb_ime) {
		if (this->gb_halt_bug == 0x01) {
			this->gb_halt_bug = 0x02;
		}
		else if (this->gb_halt_bug == 0x02) {
			this->registers.halt_bug();
			this->gb_halt_bug = 0x00;
		}
	}
}

int CPU::CPUstep() {
	this->cycles = 0;

	executeInstruction(this->gb_halt ? 0x00 : this->memory.ReadByte(this->registers.getPC()));

	halt_bug();

	// may not increase program counter after jumps.
	if (!this->jump) {
		if (!this->gb_halt)
			this->registers.incPC();
	} else {
		this->jump = false;
	}

	

	return this->cycles;
}