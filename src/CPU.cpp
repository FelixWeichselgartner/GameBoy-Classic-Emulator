//----------------------------------------------------------------------------------------------
#include "../include/CPU.hpp"
#include "../include/Registers.hpp"
#include "../include/RAM.hpp"
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Flags - F register is the flag register
// Z = this->registers.F (Byte 7)
// N = this->registers.F (Byte 6)
// H = this->registers.F (Byte 5)
// C = this->registers.F (Byte 4)
// Byte 3 = Byte 2 = Byte 1 = Byte 0 = not used
//----------------------------------------------------------------------------------------------

CPU::CPU() {
	this->running = 0x01;
	rom.load(&ram);
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

Byte CPU::ReadByte(unsigned short address) const {
    return this->ram.getMemory(address);
}

void CPU::WriteByte(unsigned short address, Byte value) {
    this->ram.setMemory(address, value);
    return;
}

Byte CPU::load8bit() {
	Byte retval = this->ram.getMemory(this->registers.getPC());
	this->registers.setPC(this->registers.getPC() + 1);
	return retval;
}

unsigned short CPU::load16bit() {
	unsigned short retval = 0x0000;
	this->registers.setPC(this->registers.getPC() + 1);
	retval = this->ram.getMemory(this->registers.getPC());
	retval = retval << 8;
	this->registers.setPC(this->registers.getPC() + 1);
	retval = retval | this->ram.getMemory(this->registers.getPC());
	return retval;
}

//----------------------------------------------------------------------------------------------
// split a 16-bit number in 2 x 8-bit numbers to save to memory
#define LOW_BYTE(x)        (x & 0xff)
#define HIGH_BYTE(x)       ((x >> 8) & 0xff)
//----------------------------------------------------------------------------------------------

void CPU::save16bitToAddress(unsigned short address, unsigned short value) {
	Byte firstHalf, secondHalf;
	firstHalf = HIGH_BYTE(value);
	secondHalf = LOW_BYTE(value);
	this->ram.setMemory(address, firstHalf);
	this->ram.setMemory(address + 1, secondHalf);
}

Byte CPU::rlc(Byte a) {
	Byte retval = a << 1 | a >> 7;

	// Z, N, H are reset
	this->registers.setF(this->registers.getF() & 0b00011111);

	// C is set if bit 0 is 1, else reset
	if (this->registers.getA() & 0x01) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::rrc(Byte a) {
	Byte retval = a >> 1 | a << 7;

	// Z, N, H are reset
	this->registers.setF(this->registers.getF() & 0b00011111);

	// C is set if bit 0 is 1, else reset
	if (this->registers.getA() & 0x01) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::rr(Byte a) {
	Byte retval = a >> 1 | a << 7;

	// Z is set if result is zero, else reset
	if (retval == (Byte)0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	} else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N, H are reset
	this->registers.setF(this->registers.getF() & 0b10011111);

	// C is set if bit 0 is 1, else reset
	if (this->registers.getA() & 0x01) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::rl(Byte a) {
	Byte retval = a << 1 | a >> 7;

	// Z is set if result is zero, else reset
	if (retval == (Byte)0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N, H are reset
	this->registers.setF(this->registers.getF() & 0b10011111);

	// C is set if bit 0 is 1, else reset
	if (this->registers.getA() & 0x01) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::cpl(Byte a) {
	// N is set
	this->registers.setF(this->registers.getF() | 0b01000000);

	// H is set
	this->registers.setF(this->registers.getF() | 0b00100000);

	return !a;
}

Byte CPU::add(Byte a, Byte b, char type) {
	Byte retval;

	if (type == 'u') { //unsigned
		retval = a + b;
	}else if (type == 's') { //signed
		retval = a + (signed char)b;
	}

	// Z is set if result is zero, else reset
    if (retval == (Byte) 0x00) {
        this->registers.setF(this->registers.getF() | 0b10000000);
	} else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

    // N is set to zero
    this->registers.setF(this->registers.getF() & 0b10111111);

    // H is set if overflow from bit 3, else reset
    if ((((a & 0xf) + (b & 0xf)) & 0x10) == 0x10) {
        this->registers.setF(this->registers.getF() | 0b00100000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11011111);
	}

    // C is set if overflow from bit 7, else reset
    if (((int) a + (int) b) > (Byte) 255) {
        this->registers.setF(this->registers.getF() | 0b00010000);
    } else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

unsigned short signed8to16(Byte n) {
	unsigned short retval = n & 0b01111111;;

	// check if minus or plus.
	if ((n & 0b10000000) == 0b10000000) { // minus
		retval |= 0x8000;
	} else { // plus
		retval &= 0x7fff;
	}

	return retval;
}

unsigned short CPU::add16bit(unsigned short a, unsigned short b, char type) {
	unsigned short retval;


	if (type == 'u') { //unsigned
		retval = a + b;
	} else if (type == 's') { //signed
		retval = a + (signed char)b;
	}

	// Z is set if result is zero, else reset
	if (retval == (unsigned short)0x0000) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is set to zero
	this->registers.setF(this->registers.getF() & 0b10111111);

	// H is set if overflow from bit 7, else reset
	if ((retval ^ a ^ b) & 0x1000) {
		this->registers.setF(this->registers.getF() | 0b00100000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11011111);
	}

	// C is set if overflow from bit 15, else reset
	if (((int)a + (int)b) > 65536) { // 65536 == pow(2, 16) - 1
		this->registers.setF(this->registers.getF() | 0b00010000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::adc(Byte a, Byte b) {
	Byte c;

	if ((this->registers.getF() & 0b00010000) == 0b00010000) {
		c = 0b00000001;
	} else {
		c = 0x00;
	}

	Byte retval = a + b + c;

	// Z is set if result is zero, else reset
	if (retval == (Byte)0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	} else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is set to zero
	this->registers.setF(this->registers.getF() & 0b10111111);

	// H is set if overflow from bit 3, else reset
	if ((((a & 0xf) + (b & 0xf)) & 0x10) == 0x10) {
		this->registers.setF(this->registers.getF() | 0b00100000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b11011111);
	}

	// C is set if overflow from bit 7, else reset
	if (((int)a + (int)b) > (Byte)255) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::sub(Byte a, Byte b) {
	Byte retval = a - b;

	// Z is set if result is zero, else reset
	if (retval == 0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	} else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is set
	this->registers.setF(this->registers.getF() | 0b01000000);

	// H is set if no borrow from bit 4, else reset
	if (((a ^ b ^ retval) & 0x10) == 0x01) {
		this->registers.setF(this->registers.getF() | 0b00100000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11011111);
	}

	// C is set if no borrow from bit 7, else reset
	if (b > a) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::sbc(Byte a, Byte b) {
	Byte c;

	if ((this->registers.getF() & 0b00010000) == 0b00010000) {
		c = 0b00000001;
	} else {
		c = 0x00;
	}

	Byte retval = a - b - c;

	// Z is set if result is zero, else reset
	if (retval == 0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	} else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is set
	this->registers.setF(this->registers.getF() | 0b01000000);

	// H is set if no borrow from bit 4, else reset
	if (((a ^ b ^ retval) & 0x10) == 0x01) {
		this->registers.setF(this->registers.getF() | 0b00100000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11011111);
	}

	// C is set if no borrow from bit 7, else reset
	if (b > a) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}

	return retval;
}

Byte CPU::land(Byte a, Byte b) {
	Byte retval = a & b;

	// Z is set if result is zero, else reset
	if (retval == (Byte)0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is reset
	this->registers.setF(this->registers.getF() & 0b10111111);

	// H is set
	this->registers.setF(this->registers.getF() | 0b00100000);

	// C is reset
	this->registers.setF(this->registers.getF() & 0b11101111);

	return retval;
}

Byte CPU::lxor(Byte a, Byte b) {
	Byte retval = a ^ b;

	// Z is set if result is zero, else reset
	if (retval == (Byte)0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is reset
	this->registers.setF(this->registers.getF() & 0b10111111);

	// H is reset
	this->registers.setF(this->registers.getF() & 0b11011111);

	// C is reset
	this->registers.setF(this->registers.getF() & 0b11101111);

	return retval;
}

Byte CPU::lor(Byte a, Byte b) {
	Byte retval = a | b;

	// Z is set if result is zero, else reset
	if (retval == (Byte)0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	}
	else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is reset
	this->registers.setF(this->registers.getF() & 0b10111111);

	// H is reset
	this->registers.setF(this->registers.getF() & 0b11011111);

	// C is reset
	this->registers.setF(this->registers.getF() & 0b11101111);

	return retval;
}

void CPU::cp(Byte A, Byte X) {
	Byte sub = A - X;

	// Z is set if result is zero, else reset
	if (sub == (Byte)0x00) {
		this->registers.setF(this->registers.getF() | 0b10000000);
	} else {
		this->registers.setF(this->registers.getF() & 0b01111111);
	}

	// N is set
	this->registers.setF(this->registers.getF() | 0b01000000);

	// H is set if no borrow from bit 4, else reset
	if (((A ^ X ^ sub) & 0x10) == 0x01) {
		this->registers.setF(this->registers.getF() | 0b00100000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11011111);
	}

	// C is set if no borrow from bit 7, else reset
	if (X > A) {
		this->registers.setF(this->registers.getF() | 0b00010000);
	} else {
		this->registers.setF(this->registers.getF() & 0b11101111);
	}
}

void CPU::push8bit(Byte value) {
	this->ram.setMemory(this->registers.getSP(), value);
	this->registers.setSP(this->registers.getSP() + 1);
}

Byte CPU::pop8bit() {
	this->registers.setSP(this->registers.getSP() - 1);
	Byte retval = this->ram.getMemory(this->registers.getSP());
	return retval;
}

void CPU::push16bit(unsigned short value) {
	Byte firstHalf, secondHalf;
	firstHalf = HIGH_BYTE(value);
	secondHalf = LOW_BYTE(value);
	push8bit(firstHalf);
	push8bit(secondHalf);
}

unsigned short CPU::pop16bit() {
	unsigned short retval = 0x0000;
	retval = pop8bit();
	retval = retval << 8;
	retval = retval | pop8bit();
	return retval;
}

void CPU::call(unsigned short address) {
	push16bit(this->registers.getPC());
	this->registers.setPC(address);
}

void CPU::executeInstruction(Byte opcode) {
    switch((int)opcode) {
        case  0x0: // NOP           no operation.
            break;
        case  0x1: // LD (BC), nn   load 16-bit immediate into BC.
			save16bitToAddress(this->registers.getBC(), load16bit());
            break;
        case  0x2: // LD (BC), A    saves A to address pointed by BC.
            this->ram.setMemory(this->registers.getBC(), this->registers.getA());
            break;
        case  0x3: // INC BC        increment 16-bit BC.
            this->registers.setBC(this->registers.getBC() + 1);
            break;
        case  0x4: // INC B        increment 8-bit B.
            this->registers.setB(this->registers.getB() + 1);
            break;
        case  0x5: // DEC B         decrement 8-bit B.
            this->registers.setB(this->registers.getB() - 1);
            break;
        case  0x6: // LD B, n       load 8 bit immediate into B.
			this->registers.setB(load8bit());
            break;
        case  0x7: // RLC A         rotate A left with carry.
			this->registers.setA(rlc(this->registers.getA()));
            break;
        case  0x8: // LD (nn), SP   save SP to a given address.
			save16bitToAddress(load16bit(), this->registers.getSP());
            break;
        case  0x9: // ADD HL, BC    add 16-bit BC to HL.
            this->registers.setHL(this->registers.getHL() + this->registers.getBC());
            break;
        case  0xA: // LD A, (BC)    load A from address pointed by BC.  
            this->registers.setA(this->ram.getMemory(this->registers.getBC()));
            break;
        case  0xB: // DEC BC        decrement 16-bit BC.
            this->registers.setBC(this->registers.getBC() - 1);
            break;
        case  0xC: // INC C         increment 8-bit C.
            this->registers.setC(this->registers.getC() + 1);
            break;
        case  0xD: // DEC C         decrement 8-bit C.
            this->registers.setC(this->registers.getC() - 1);
            break;
        case  0xE: // LD C, n       load 8-bit immediate to C.
			this->registers.setC(load8bit());
            break;
        case  0xF: // RRC A         rotate A right with carry.
			this->registers.setA(rrc(this->registers.getA()));
            break;
        case 0x10: // STOP          stop processor.
			this->gb_stop = 0x01;
            break;
        case 0x11: // LD DE, nn     load 16-bit immediate into DE.
			this->registers.setDE(load16bit());
            break;
        case 0x12: // LD (DE), A    save A to address pointed by DE.
            this->ram.setMemory(this->registers.getDE(), this->registers.getA());
            break;
        case 0x13: // INC DE        increment 16-bit DE.
            this->registers.setDE(this->registers.getDE() + 1);
            break;
        case 0x14: // INC D         increment 8-bit D.
            this->registers.setD(this->registers.getD() + 1);
            break;
        case 0x15: // DEC D         decrement 8-bit D.
            this->registers.setD(this->registers.getD() - 1);
            break;
        case 0x16: // LD D, n       load 8-bit immediate into D.
			this->registers.setD(load8bit());
            break;
        case 0x17: // RL A          rotate A left.
			this->registers.setA(rl(this->registers.getA()));
            break;
        case 0x18: // JR n          relative jump by signed immediate.
			this->registers.setPC(this->registers.getPC() + (signed char) load8bit());
            break;
        case 0x19: // ADD HL, DE    add 16-bit DE to HL.
			this->registers.setHL(add16bit(this->registers.getHL(), this->registers.getDE(), 'u'));
            break;
        case 0x1A: // LD A, (DE)    load A from address pointed to by DE.
            this->registers.setA(this->ram.getMemory(this->registers.getDE()));
            break;
        case 0x1B: // DEC DE        decrement 16-bit DE.
            this->registers.setDE(this->registers.getDE() - 1);
            break;
        case 0x1C: // INC E         increment 8-bit E.
            this->registers.setE(this->registers.getE() + 1);
            break;
        case 0x1D: // DEC E         decrement 8-bit E.
            this->registers.setE(this->registers.getE() - 1);
            break;
        case 0x1E: // LD E, n       Load 8-bit immediate into E.
			this->registers.setE(load8bit());
            break;
        case 0x1F: // RR A          rotate A right.
			this->registers.setA(rr(this->registers.getA()));
            break;
        case 0x20: // JR NC, n      relative jump by signed immediate if last result was not zero.
			if ((this->registers.getF() & 0b10000000) != 0b1000000) {
				this->registers.setPC(this->registers.getPC() + (signed char) load8bit());
			}
            break;
        case 0x21: // LD HL, nn     load 16-bit immediate into HL.
			this->registers.setHL(load16bit());
            break;
        case 0x22: // LDI (HL), A   save A to address pointed by HL, and increment HL.
			this->ram.setMemory(this->registers.getHL(), this->registers.getA());
			this->registers.setHL(this->registers.getHL() + 1);
            break;
        case 0x23: // INC HL        increment 16-bit HL.
            this->registers.setHL(this->registers.getHL() + 1);
            break;
        case 0x24: // INC H         increment 8-bit H.
            this->registers.setH(this->registers.getH() + 1);
            break;
        case 0x25: // DEC H         decrement 8-bit H.
            this->registers.setH(this->registers.getH() - 1);
            break;
        case 0x26: // LD H, n       load 8-bit immediate into H.
			this->registers.setH(load8bit());
            break;
        case 0x27: // DAA           adjust A for BCD addition.
            break;
        case 0x28: // JR Z, n       relative jump by signed immediate if last result was zero.
			if ((this->registers.getF() & 0b10000000) == 0b1000000) {
				this->registers.setPC(add16bit(this->registers.getPC(), signed8to16(load8bit()), 's'));
			}
            break;
        case 0x29: // ADD HL, HL    add 16-bit HL to HL.
			this->registers.setHL(this->registers.getHL() + this->registers.getHL());
            break;
        case 0x2A: // LDI A, (HL)   load A from address pointed to by HL, and increment HL.
			this->registers.setA(this->ram.getMemory(this->registers.getHL()));
			this->registers.setHL(this->registers.getHL() + 1);
            break;
        case 0x2B: // DEC HL        decrement 16-bit HL.
            this->registers.setHL(this->registers.getHL() - 1);
            break;
        case 0x2C: // INC L         increment 8-bit L.
            this->registers.setL(this->registers.getL() + 1);
            break;
        case 0x2D: // DEC L         decrement 8-bit L.
            this->registers.setL(this->registers.getL() - 1);
            break;
        case 0x2E: // LD L, n       load 8-bit immediate into L.
			this->registers.setL(load8bit());
            break;
        case 0x2F: // CPL           complement (logical NOT) on A.
			this->registers.setA(cpl(this->registers.getA()));
            break;
        case 0x30: // JR NC, n      relative jump by signed immediate if last result caused no carry.
			if ((this->registers.getF() & 0b00010000) != 0b00010000) {
				this->registers.setPC(add16bit(this->registers.getPC(), signed8to16(load8bit()), 's'));
			}
            break;
        case 0x31: // LD SP, nn     load 16-bit immediate into SP.
			this->registers.setSP(load16bit());
            break;
        case 0x32: // LDD (HL), A   save A to address pointed by HL, and decrement HL.
			this->registers.setA(this->ram.getMemory(this->registers.getHL()));
			this->registers.setHL(this->registers.getHL() - 1);
            break;
        case 0x33: // INC SP        increment 16-bit SP.
            this->registers.setSP(this->registers.getSP() + 1);
            break;
        case 0x34: // INC (HL)      increment value pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->ram.getMemory(this->registers.getHL()) + 1);
            break;
        case 0x35: // DEC (HL)      decrement value pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->ram.getMemory(this->registers.getHL()) - 1);
            break;
        case 0x36: // LD (HL), n    load 8-bit immediate into address pointed by HL.
			this->ram.setMemory(this->registers.getHL(), load8bit());
            break;
        case 0x37: // SCF           set carry flag.
			this->registers.setF(this->registers.getF() | 0b00010000);
            break;
        case 0x38: // JR C, n       relative jump by signed immediate if last result caused carry.
			if ((this->registers.getF() & 0b00010000) == 0b00010000) {
				this->registers.setPC(add16bit(this->registers.getPC(), signed8to16(load8bit()), 's'));
			}
            break;
        case 0x39: // ADD HL, SP    add 16-bit SP to HL.
            this->registers.setHL(this->registers.getHL() + this->registers.getSP());
            break;
        case 0x3A: // LDD A, (HL)   load A from address pointed to by HL, and decrement HL.
			this->registers.setA(this->ram.getMemory(this->registers.getHL()));
			this->registers.setHL(this->registers.getHL() - 1);
            break;
        case 0x3B: // DEC SP        decrement 16-bit SP.
            this->registers.setSP(this->registers.getSP() - 1);
            break;
        case 0x3C: // INC A         increment 8-bit A.
            this->registers.setA(this->registers.getA() + 1);
            break;
        case 0x3D: // DEC A         decrement 8-bit A.
            this->registers.setA(this->registers.getA() - 1);
            break;
        case 0x3E: // LD A, n       load 8-bit immediate into A.
			this->registers.setA(load8bit());
            break;
        case 0x3F: // CCF           complement carry flag.
			if ((this->registers.getF() & 0b00010000) == 0b00010000) {
				this->registers.setF(this->registers.getF() & 0b11101111);
			} else {
				this->registers.setF(this->registers.getF() | 0b00010000);
			}
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
            this->registers.setB(this->ram.getMemory(this->registers.getHL()));
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
            this->registers.setC(this->ram.getMemory(this->registers.getHL()));
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
            this->registers.setD(this->ram.getMemory(this->registers.getHL()));
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
            this->registers.setE(this->ram.getMemory(this->registers.getHL()));
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
            this->registers.setH(this->ram.getMemory(this->registers.getHL()));
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
            this->registers.setL(this->ram.getMemory(this->registers.getHL()));
            break;
        case 0x6F: // LD L, A       copy A to L.
            this->registers.setL(this->registers.getA());
            break;
        case 0x70: // LD (HL), B    copy B to address pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->registers.getB());
            break;
        case 0x71: // LD (HL), C    copy C to address pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->registers.getC());
            break;
        case 0x72: // LD (HL), D    copy D to address pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->registers.getD());
            break;
        case 0x73: // LD (HL), E    copy E to address pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->registers.getE());
            break;
        case 0x74: // LD (HL), H    copy H to address pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->registers.getH());
            break;
        case 0x75: // LD (HL), L    copy L to address pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->registers.getL());
            break;
        case 0x76: // HALT          halt processor.
			this->gb_halt = 0x01;
            break;
        case 0x77: // LD (HL), A    copy A to address pointed by HL.
            this->ram.setMemory(this->registers.getHL(), this->registers.getA());
            break;
        case 0x78: // LD A, B       copy B to A.
            this->registers.setA(this->registers.getA() + this->registers.getB());
            break;
        case 0x79: // LD A, C       copy C to A.
            this->registers.setA(this->registers.getA() + this->registers.getC());
            break;
        case 0x7A: // LD A, D       copy D to A.
            this->registers.setA(this->registers.getA() + this->registers.getD());
            break;
        case 0x7B: // LD A, E       copy E to A.
            this->registers.setA(this->registers.getA() + this->registers.getE());
            break;
        case 0x7C: // LD A, H       copy H to A.
            this->registers.setA(this->registers.getA() + this->registers.getH());
            break;
        case 0x7D: // LD A, L       copy L to A.
            this->registers.setA(this->registers.getA() + this->registers.getL());
            break;
        case 0x7E: // LD A, (HL)    copy value pointed by HL to A.
            this->registers.setA(this->ram.getMemory(this->registers.getHL()));
            break;
        case 0x7F: // LD A, A       copy A to A.
            this->registers.setA(this->registers.getA());
            break;
        case 0x80: // ADD A, B      add B to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getB(), 'u'));
            break;
        case 0x81: // ADD A, C      add C to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getC(), 'u'));
            break;
        case 0x82: // ADD A, D      add D to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getD(), 'u'));
            break;
        case 0x83: // ADD A, E      add E to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getE(), 'u'));
            break;
        case 0x84: // ADD A, H      add H to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getH(), 'u'));
            break;
        case 0x85: // ADD A, L      add L to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getL(), 'u'));
            break;
        case 0x86: // ADD A, (HL)   add value pointed by HL to A.
            this->registers.setA(add(this->registers.getA(), this->ram.getMemory(this->registers.getHL()), 'u'));
            break;
        case 0x87: // ADD A, A      add A to A.
            this->registers.setA(add(this->registers.getA(), this->registers.getA(), 'u'));
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
			this->registers.setA(adc(this->registers.getA(), this->ram.getMemory(this->registers.getHL())));
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
            this->registers.setA(sub(this->registers.getA(), this->ram.getMemory(this->registers.getB())));
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
			this->registers.setA(sbc(this->registers.getA(), this->ram.getMemory(this->registers.getHL())));
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
            this->registers.setA(land(this->registers.getA(), this->ram.getMemory(this->registers.getHL())));
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
            this->registers.setA(lxor(this->registers.getA(), this->ram.getMemory(this->registers.getHL())));
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
            this->registers.setA(lor(this->registers.getA(), this->ram.getMemory(this->registers.getHL())));
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
            cp(this->registers.getA(), this->ram.getMemory(this->registers.getHL()));
            break;
        case 0xBF: // CP A          compare A against A.
            cp(this->registers.getA(), this->registers.getA());
            break;
        case 0xC0: // RET NZ        return if last result was not zero.
			if ((this->registers.getF() & 0b10000000) != 0b10000000) {
				this->registers.setPC(pop16bit());
			}
            break;
        case 0xC1: // POP BC        pop 16-bit value from stack into BC.
			this->registers.setBC(pop16bit());
            break;
        case 0xC2: // JP NZ, nn     absolute jump to 16-bit location if last result was not zero.
			if ((this->registers.getF() & 0b10000000) != 0b1000000) {
				this->registers.setPC(load16bit());
			}
            break;
        case 0xC3: // JP nn         absolute jump to 16-bit location.
			this->registers.setPC(load16bit());
			jump = 0x01;
            break;
        case 0xC4: // CALL NZ, nn   call routine at 16-bit location if last result was not zero.
			if ((this->registers.getF() & 0b10000000) != 0b10000000) {
				call(load16bit());
			}
            break;
        case 0xC5: // PUSH BC       push 16-bit BC onto stack.
			push16bit(this->registers.getBC());
            break;
        case 0xC6: // ADD A, n      add 8-bit immediate to A. 
			this->registers.setA(add(this->registers.getA(), load8bit(), 'u'));
            break;
        case 0xC7: // RST 0         call routine at address 0000h
			call(0x0000);
            break;
        case 0xC8: // RET Z         return if last result was zero.
			if ((this->registers.getF() & 0b10000000) == 0b10000000) {
				this->registers.setPC(pop16bit());
			}
            break;
        case 0xC9: // RET           return to calling routine.
			this->registers.setPC(pop16bit());
            break;
        case 0xCA: // JP Z, nn      absolute jump to 16-bit location if last result was zero.
			if ((this->registers.getF() & 0b10000000) == 0b10000000) {
				this->registers.setPC(load16bit());
			}
            break;
        case 0xCB: // Ext ops       extended operations (two-Byte instruction code).
			executeExtendedOpcodes();
            break;
        case 0xCC: // CALL Z, nn    call routine at 16-bit location if last result was zero.
			if ((this->registers.getF() & 0b10000000) == 0b10000000) {
				call(load16bit());
			}
            break;
        case 0xCD: // CALL nn       call routine at 16-bit location.
			call(load16bit());
            break;
        case 0xCE: // ADC A, n      add 8-bit immediate and carry to A.
			this->registers.setA(adc(this->registers.getA(), load8bit()));
            break;
        case 0xCF: // RST 8         call routine at adress 0008h.
			call(0x0008);
            break;
        case 0xD0: // RET NC        return if last result caused no carry.
			if ((this->registers.getF() & 0b00010000) != 0b00010000) {
				this->registers.setPC(pop16bit());
			}
            break;
        case 0xD1: // POP DE        pop 16-bit value from stack into DE.
			this->registers.setDE(pop16bit());
            break;
        case 0xD2: // JP NC, nn     absolute jump to 16-bit location if last result caused no carry.
			if ((this->registers.getF() & 0b00010000) != 0b00010000) {
				this->registers.setPC(load16bit());
			}
            break;
        case 0xD3: // XX            operation removed in this CPU.
            break;
        case 0xD4: // CALL NC, nn   call routine at 16-bit location if last result caused no carry.
			if ((this->registers.getF() & 0b00010000) != 0b00010000) {
				call(load16bit());
			}
            break;
        case 0xD5: // PUSH DE       push 16-bit DE onto stack.
			push16bit(this->registers.getDE());
            break;
        case 0xD6: // SUB A, n      subtract 8-bit immediate from A. 
			this->registers.setA(sub(this->registers.getA(), load8bit()));
            break;
        case 0xD7: // RST 10        call routine at address 0010h.
			call(0x0010);
            break;
        case 0xD8: // RET C         return if last result caused carry.
			if ((this->registers.getF() & 0b00010000) != 0b00010000) {
				this->registers.setPC(pop16bit());
			}
            break;
        case 0xD9: // RETI          enable interrupts and return to calling routine.
			this->enableInterrupts = 0x01;
			this->registers.setPC(pop16bit());
            break; 
        case 0xDA: // JP C, nn      absolute jump to 16-bit location if last result caused carry.
			if ((this->registers.getF() & 0b00010000) != 0b00010000) {
				this->registers.setPC(load16bit());
			}
            break;
        case 0xDB: // XX            operation removed in this CPU. 
            break;
        case 0xDC: // CALL C, nn    call routine at 16-bit location if last result caused no carry.
			if ((this->registers.getF() & 0b00010000) != 0b00010000) {
				call(load16bit());
			}
            break;
        case 0xDD: // XX            operation removed in this CPU.
            break;
        case 0xDE: // SBC A, n      subtract 8-bit immediate and carry from A. 
			this->registers.setA(sbc(this->registers.getA(), load8bit()));
            break;
        case 0xDF: // RST 18        call routine at address 0018h.
			call(0x0018);
            break;
        case 0xE0: // LDH (n), A    save A at address pointed to by (FF00h + 8-bit immediate)
			this->ram.setMemory(ADDR_IO + load8bit(), this->registers.getA());
            break;
        case 0xE1: // POP HL        pop 16-bit value from stack into HL.
			this->registers.setHL(pop16bit());
            break;
        case 0xE2: // LDH (C), A    save A at address pointed to by (FF00h + C)
			this->ram.setMemory(ADDR_IO + this->registers.getC(), this->registers.getA());
            break;
        case 0xE3: // XX            operation removed in this CPU.
            break;
        case 0xE4: // XX            operation removed in this CPU.
            break;
        case 0xE5: // PUSH HL       Push 16-bit HL onto stack.
			push16bit(this->registers.getHL());
            break;
        case 0xE6: // AND n         logical AND 8-bit immediate against A.
			this->registers.setA(land(this->registers.getA(), load8bit()));
            break;
        case 0xE7: // RST 20        call routine at address 0020h.
			call(0x0020);
            break;
        case 0xE8: // ADD SP, d     add signed 8-bit immediate to SP.
			this->registers.setSP(add16bit(this->registers.getSP(), signed8to16(load8bit()), 's'));
            break;
        case 0xE9: // JP (HL)       jump to 16-bit value pointed by HL.
			this->registers.setPC(this->registers.getHL());
            break;
        case 0xEA: // LD (nn), A    save A at given 16-bit address.
			this->ram.setMemory(load16bit(), this->registers.getA());
            break;
        case 0xEB: // XX            operation removed in this CPU.
            break;
        case 0xEC: // XX            operation removed in this CPU.
            break;
        case 0xED: // XX            operation removed in this CPU.
            break;
        case 0xEE: // XOR n         logical XOR 8-bit immediate against A.
			this->registers.setA(lxor(this->registers.getA(), load8bit()));
            break;
        case 0xEF: // RST 28        call routine at address 0028h.
			call(0x0028);
            break;
        case 0xF0: // LDH A, (n)    load A from address pointed to by (FF00h + 8-bit immediate).
			this->registers.setA(this->ram.getMemory(ADDR_IO + load8bit()));
            break;
        case 0xF1: // POP AF        pop 16-bit value from stack into AF.
			this->registers.setAF(pop16bit());
            break;
        case 0xF2: // XX            operation removed in this CPU.
            break;
        case 0xF3: // DI            disable interrupts
			this->enableInterrupts = 0x00;
            break;
        case 0xF4: // XX            operation removed in this CPU.
            break;
        case 0xF5: // PUSH AF       push 16-bit AF onto stack.
			//this->registers.setAF(this->registers.getA() << 8 | this->registers.getF());		// yet don't now if this is needed.
			push16bit(this->registers.getAF());
            break;
        case 0xF6: // OR n          logical OR 8-bit immediate against A.
			this->registers.setA(lor(this->registers.getA(), load8bit()));
            break;
        case 0xF7: // RST 30        call routine at address 0030h.
			call(0x0030);
            break;
        case 0xF8: // LDHL SP, d    add signed 8-bit immediate to SP and save result in HL.
			this->registers.setHL(add16bit(this->registers.getSP(), signed8to16(load8bit()), 's'));
            break;
        case 0xF9: // LD SP, HL     copy HL to SP.
			this->registers.setSP(this->registers.getHL());
            break;
        case 0xFA: // LD A, (nn)    load A from given 16-bit address.
			this->registers.setA(this->ram.getMemory(load16bit()));
            break;
        case 0xFB: // EI            enable interrupts.
			this->enableInterrupts = 0x01;
            break;
        case 0xFC: // XX            operation removed in this CPU.
            break;
        case 0xFD: // XX            operation removed in this CPU.
            break;
        case 0xFE: // CP n          compare 8-bit immediate against A.
			cp(this->registers.getA(), load8bit());
            break;
        case 0xFF: // RST 38        call routine at address 0038h.
			call(0x0038);
            break;
    }
}

void CPU::sla() {
	// Z is set if result is zero, else reset.

	// N, H are reset.

	// C is set according to result.
}

void CPU::sra() {
	// Z is set if result is zero, else reset.

	// N, H are reset.

	// C is set according to result.
}

void CPU::swap() {
	// Z is set if result is zero, else reset.

	// N, H, C are reset.
}

void CPU::srl() {
	// Z is set if result is zero, else reset.

	// N, H are reset.

	// C is set according to result.
}

void CPU::bit() {
	// zero flag is set if bit is not set.

	// N is reset.

	// H is set.

}

void CPU::res() {
	// flags are not affected.
}

void CPU::set() {
	// flags are not affected.
}

void CPU::executeExtendedOpcodes() {
	this->registers.setPC(this->registers.getPC() + 1);
	Byte exOpcode = this->ram.getMemory(this->registers.getPC());

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
			this->ram.setMemory(this->registers.getHL(), rlc(this->ram.getMemory(this->registers.getHL())));
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
			this->ram.setMemory(this->registers.getHL(), rrc(this->ram.getMemory(this->registers.getHL())));
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
			this->ram.setMemory(this->registers.getHL(), rl(this->ram.getMemory(this->registers.getHL())));
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
			this->ram.setMemory(this->registers.getHL(), rr(this->ram.getMemory(this->registers.getHL())));
			break;
		case 0x1F: // RR A
			this->registers.setA(rr(this->registers.getA()));
			break;
		case 0x20: // SLA B			shift B left preserving sign.
			break;
		case 0x21: // 
			break;
		case 0x22: // 
			break;
		case 0x23: // 
			break;
		case 0x24: // 
			break;
		case 0x25: // 
			break;
		case 0x26: // 
			break;
		case 0x27: // 
			break;
		case 0x28: // SRA B			shift B right preserving sign.
			break;
		case 0x29: // 
			break;
		case 0x2A: // 
			break;
		case 0x2B: // 
			break;
		case 0x2C: // 
			break;
		case 0x2D: // 
			break;
		case 0x2E: // 
			break;
		case 0x2F: // 
			break;
		case 0x30: // SWAP B		swap nybbles in B.
			break;
		case 0x31: // 
			break;
		case 0x32: // 
			break;
		case 0x33: // 
			break;
		case 0x34: // 
			break;
		case 0x35: // 
			break;
		case 0x36: // 
			break;
		case 0x37: // 
			break;
		case 0x38: // SRL B			shift B right.
			break;
		case 0x39: // 
			break;
		case 0x3A: // 
			break;
		case 0x3B: // 
			break;
		case 0x3C: // 
			break;
		case 0x3D: // 
			break;
		case 0x3E: // 
			break;
		case 0x3F: // 
			break;
		case 0x40: // BIT 0, B		test bit 0 of B.
			break;
		case 0x41: // BIT 0, C
			break;
		case 0x42: // BIT 0, D
			break;
		case 0x43: // BIT 0, E
			break;
		case 0x44: // BIT 0, H
			break;
		case 0x45: // BIT 0, L
			break;
		case 0x46: // BIT 0, (HL)
			break;
		case 0x47: // BIT 0, A
			break;
		case 0x48: // BIT 1, B
			break;
		case 0x49: // BIT 1, C
			break;
		case 0x4A: // BIT 1, D
			break;
		case 0x4B: // BIT 1, E
			break;
		case 0x4C: // BIT 1, H
			break;
		case 0x4D: // BIT 1, L
			break;
		case 0x4E: // BIT 1, (HL)
			break;
		case 0x4F: // BIT 1, A
			break;
		case 0x50: // BIT 2, B
			break;
		case 0x51: // BIT 2, C
			break;
		case 0x52: // BIT 2, D
			break;
		case 0x53: // BIT 2, E
			break;
		case 0x54: // BIT 2, H
			break;
		case 0x55: // BIT 2, L
			break;
		case 0x56: // BIT 2, (HL)
			break;
		case 0x57: // BIT 2, A
			break;
		case 0x58: // BIT 3, B
			break;
		case 0x59: // BIT 3, C
			break;
		case 0x5A: // BIT 3, D
			break;
		case 0x5B: // BIT 3, E
			break;
		case 0x5C: // BIT 3, H
			break;
		case 0x5D: // BIT 3, L
			break;
		case 0x5E: // BIT 3, (HL)
			break;
		case 0x5F: // BIT 3, A
			break;
		case 0x60: // BIT 4, B
			break;
		case 0x61: // BIT 4, C
			break;
		case 0x62: // BIT 4, D
			break;
		case 0x63: // BIT 4, E
			break;
		case 0x64: // BIT 4, H
			break;
		case 0x65: // BIT 4, L
			break;
		case 0x66: // BIT 4, (HL)
			break;
		case 0x67: // BIT 4, A
			break;
		case 0x68: // BIT 5, B
			break;
		case 0x69: // BIT 5, C
			break;
		case 0x6A: // BIT 5, D
			break;
		case 0x6B: // BIT 5, E
			break;
		case 0x6C: // BIT 5, H
			break;
		case 0x6D: // BIT 5, L
			break;
		case 0x6E: // BIT 5, (HL)
			break;
		case 0x6F: // BIT 5, A
			break;
		case 0x70: // BIT 6, B
			break;
		case 0x71: // BIT 6, C
			break;
		case 0x72: // BIT 6, D
			break;
		case 0x73: // BIT 6, E
			break;
		case 0x74: // BIT 6, H
			break;
		case 0x75: // BIT 6, L
			break;
		case 0x76: // BIT 6, (HL)
			break;
		case 0x77: // BIT 6, A
			break;
		case 0x78: // BIT 7, B
			break;
		case 0x79: // BIT 7, C
			break;
		case 0x7A: // BIT 7, D
			break;
		case 0x7B: // BIT 7, E
			break;
		case 0x7C: // BIT 7, H
			break;
		case 0x7D: // BIT 7, L
			break;
		case 0x7E: // BIT 7, (HL)
			break;
		case 0x7F: // BIT 7, A
			break;
		case 0x80: // RES 0, B		clear (reset) bit 0 of B.
			break;
		case 0x81: // RES 0, C
			break;
		case 0x82: // RES 0, D
			break;
		case 0x83: // RES 0, E
			break;
		case 0x84: // RES 0, H
			break;
		case 0x85: // RES 0, L
			break;
		case 0x86: // RES 0, (HL)
			break;
		case 0x87: // RES 0, A
			break;
		case 0x88: // RES 1, B
			break;
		case 0x89: // RES 1, C
			break;
		case 0x8A: // RES 1, D
			break;
		case 0x8B: // RES 1, E
			break;
		case 0x8C: // RES 1, H
			break;
		case 0x8D: // RES 1, L
			break;
		case 0x8E: // RES 1, (HL)
			break;
		case 0x8F: // RES 1, A
			break;
		case 0x90: // RES 2, B
			break;
		case 0x91: // RES 2, C
			break;
		case 0x92: // RES 2, D
			break;
		case 0x93: // RES 2, E
			break;
		case 0x94: // RES 2, H
			break;
		case 0x95: // RES 2, L
			break;
		case 0x96: // RES 2, (HL)
			break;
		case 0x97: // RES 2, A
			break;
		case 0x98: // RES 3, B
			break;
		case 0x99: // RES 3, C
			break;
		case 0x9A: // RES 3, D
			break;
		case 0x9B: // RES 3, E
			break;
		case 0x9C: // RES 3, H
			break;
		case 0x9D: // RES 3, L
			break;
		case 0x9E: // RES 3, (HL)
			break;
		case 0x9F: // RES 3, A
			break;
		case 0xA0: // RES 4, B
			break;
		case 0xA1: // RES 4, C
			break;
		case 0xA2: // RES 4, D
			break;
		case 0xA3: // RES 4, E
			break;
		case 0xA4: // RES 4, H
			break;
		case 0xA5: // RES 4, L
			break;
		case 0xA6: // RES 4, (HL)
			break;
		case 0xA7: // RES 4, A
			break;
		case 0xA8: // RES 5, B
			break;
		case 0xA9: // RES 5, C
			break;
		case 0xAA: // RES 5, D
			break;
		case 0xAB: // RES 5, E
			break;
		case 0xAC: // RES 5, H
			break;
		case 0xAD: // RES 5, L
			break;
		case 0xAE: // RES 5, (HL)
			break;
		case 0xAF: // RES 5, A
			break;
		case 0xB0: // RES 6, B
			break;
		case 0xB1: // RES 6, C
			break;
		case 0xB2: // RES 6, D
			break;
		case 0xB3: // RES 6, E
			break;
		case 0xB4: // RES 6, H
			break;
		case 0xB5: // RES 6, L
			break;
		case 0xB6: // RES 6, (HL)
			break;
		case 0xB7: // RES 6, A
			break;
		case 0xB8: // RES 6, B
			break;
		case 0xB9: // RES 7, C
			break;
		case 0xBA: // RES 7, D
			break;
		case 0xBB: // RES 7, E
			break;
		case 0xBC: // RES 7, H
			break;
		case 0xBD: // RES 7, L
			break;
		case 0xBE: // RES 7, (HL)
			break;
		case 0xBF: // RES 7, A
			break;
		case 0xC0: // SET 0, B
			break;
		case 0xC1: // SET 0, C
			break;
		case 0xC2: // SET 0, D
			break;
		case 0xC3: // SET 0, E
			break;
		case 0xC4: // SET 0, H
			break;
		case 0xC5: // SET 0, L
			break;
		case 0xC6: // SET 0, (HL)
			break;
		case 0xC7: // SET 0, A
			break;
		case 0xC8: // SET 1, B
			break;
		case 0xC9: // SET 1, C
			break;
		case 0xCA: // SET 1, D
			break;
		case 0xCB: // SET 1, E
			break;
		case 0xCC: // SET 1, H
			break;
		case 0xCD: // SET 1, L
			break;
		case 0xCE: // SET 1, (HL)
			break;
		case 0xCF: // SET 1, A
			break;
		case 0xD0: // SET 2, B
			break;
		case 0xD1: // SET 2, C
			break;
		case 0xD2: // SET 2, D
			break;
		case 0xD3: // SET 2, E
			break;
		case 0xD4: // SET 2, H
			break;
		case 0xD5: // SET 2, L
			break;
		case 0xD6: // SET 2, (HL)
			break;
		case 0xD7: // SET 2, A
			break;
		case 0xD8: // SET 3, B
			break;
		case 0xD9: // SET 3, C
			break;
		case 0xDA: // SET 3, D
			break;
		case 0xDB: // SET 3, E
			break;
		case 0xDC: // SET 3, H
			break;
		case 0xDD: // SET 3, L
			break;
		case 0xDE: // SET 3, (HL)
			break;
		case 0xDF: // SET 3, A
			break;
		case 0xE0: // SET 4, B
			break;
		case 0xE1: // SET 4, C
			break;
		case 0xE2: // SET 4, D
			break;
		case 0xE3: // SET 4, E
			break;
		case 0xE4: // SET 4, H
			break;
		case 0xE5: // SET 4, L
			break;
		case 0xE6: // SET 4, (HL)
			break;
		case 0xE7: // SET 4, A
			break;
		case 0xE8: // SET 5, B
			break;
		case 0xE9: // SET 5, C
			break;
		case 0xEA: // SET 5, D
			break;
		case 0xEB: // SET 5, E
			break;
		case 0xEC: // SET 5, H
			break;
		case 0xED: // SET 5, L
			break;
		case 0xEE: // SET 5, (HL)
			break;
		case 0xEF: // SET 5, A
			break;
		case 0xF0: // SET 6, B
			break;
		case 0xF1: // SET 6, C
			break;
		case 0xF2: // SET 6, D
			break;
		case 0xF3: // SET 6, E
			break;
		case 0xF4: // SET 6, H
			break;
		case 0xF5: // SET 6, L
			break;
		case 0xF6: // SET 6, (HL)
			break;
		case 0xF7: // SET 6, A
			break;
		case 0xF8: // SET 7, B
			break;
		case 0xF9: // SET 7, C
			break;
		case 0xFA: // SET 7, D
			break;
		case 0xFB: // SET 7, E
			break;
		case 0xFC: // SET 7, H
			break;
		case 0xFD: // SET 7, L
			break;
		case 0xFE: // SET 7, (HL)
			break;
		case 0xFF: // SET 7, A
			break;
	}
}

void CPU::CPUstep() {
	executeInstruction(this->ram.getMemory(this->registers.getPC()));

	// may not increase program counter after jumps.
	if (!this->jump) {
		this->registers.setPC(this->registers.getPC() + 1);
	} else {
		this->jump = 0x00;
	}
}