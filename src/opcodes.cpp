#include "../include/CPU.hpp"

// init opcode pointer
void CPU::Init_OPCODE() {
	opcode[0x00] = &CPU::opcode_00;
	opcode[0x01] = &CPU::opcode_01;
	opcode[0x02] = &CPU::opcode_02;
	opcode[0x03] = &CPU::opcode_03;
	opcode[0x04] = &CPU::opcode_04;
	opcode[0x05] = &CPU::opcode_05;
	opcode[0x06] = &CPU::opcode_06;
	opcode[0x07] = &CPU::opcode_07;
	opcode[0x08] = &CPU::opcode_08;
	opcode[0x09] = &CPU::opcode_09;
	opcode[0x0A] = &CPU::opcode_0A;
	opcode[0x0B] = &CPU::opcode_0B;
	opcode[0x0C] = &CPU::opcode_0C;
	opcode[0x0D] = &CPU::opcode_0D;
	opcode[0x0E] = &CPU::opcode_0E;
	opcode[0x0F] = &CPU::opcode_0F;
	opcode[0x10] = &CPU::opcode_10;
	opcode[0x11] = &CPU::opcode_11;
	opcode[0x12] = &CPU::opcode_12;
	opcode[0x13] = &CPU::opcode_13;
	opcode[0x14] = &CPU::opcode_14;
	opcode[0x15] = &CPU::opcode_15;
	opcode[0x16] = &CPU::opcode_16;
	opcode[0x17] = &CPU::opcode_17;
	opcode[0x18] = &CPU::opcode_18;
	opcode[0x19] = &CPU::opcode_19;
	opcode[0x1A] = &CPU::opcode_1A;
	opcode[0x1B] = &CPU::opcode_1B;
	opcode[0x1C] = &CPU::opcode_1C;
	opcode[0x1D] = &CPU::opcode_1D;
	opcode[0x1E] = &CPU::opcode_1E;
	opcode[0x1F] = &CPU::opcode_1F;
	opcode[0x20] = &CPU::opcode_20;
	opcode[0x21] = &CPU::opcode_21;
	opcode[0x22] = &CPU::opcode_22;
	opcode[0x23] = &CPU::opcode_23;
	opcode[0x24] = &CPU::opcode_24;
	opcode[0x25] = &CPU::opcode_25;
	opcode[0x26] = &CPU::opcode_26;
	opcode[0x27] = &CPU::opcode_27;
	opcode[0x28] = &CPU::opcode_28;
	opcode[0x29] = &CPU::opcode_29;
	opcode[0x2A] = &CPU::opcode_2A;
	opcode[0x2B] = &CPU::opcode_2B;
	opcode[0x2C] = &CPU::opcode_2C;
	opcode[0x2D] = &CPU::opcode_2D;
	opcode[0x2E] = &CPU::opcode_2E;
	opcode[0x2F] = &CPU::opcode_2F;
	opcode[0x30] = &CPU::opcode_30;
	opcode[0x31] = &CPU::opcode_31;
	opcode[0x32] = &CPU::opcode_32;
	opcode[0x33] = &CPU::opcode_33;
	opcode[0x34] = &CPU::opcode_34;
	opcode[0x35] = &CPU::opcode_35;
	opcode[0x36] = &CPU::opcode_36;
	opcode[0x37] = &CPU::opcode_37;
	opcode[0x38] = &CPU::opcode_38;
	opcode[0x39] = &CPU::opcode_39;
	opcode[0x3A] = &CPU::opcode_3A;
	opcode[0x3B] = &CPU::opcode_3B;
	opcode[0x3C] = &CPU::opcode_3C;
	opcode[0x3D] = &CPU::opcode_3D;
	opcode[0x3E] = &CPU::opcode_3E;
	opcode[0x3F] = &CPU::opcode_3F;
	opcode[0x40] = &CPU::opcode_40;
	opcode[0x41] = &CPU::opcode_41;
	opcode[0x42] = &CPU::opcode_42;
	opcode[0x43] = &CPU::opcode_43;
	opcode[0x44] = &CPU::opcode_44;
	opcode[0x45] = &CPU::opcode_45;
	opcode[0x46] = &CPU::opcode_46;
	opcode[0x47] = &CPU::opcode_47;
	opcode[0x48] = &CPU::opcode_48;
	opcode[0x49] = &CPU::opcode_49;
	opcode[0x4A] = &CPU::opcode_4A;
	opcode[0x4B] = &CPU::opcode_4B;
	opcode[0x4C] = &CPU::opcode_4C;
	opcode[0x4D] = &CPU::opcode_4D;
	opcode[0x4E] = &CPU::opcode_4E;
	opcode[0x4F] = &CPU::opcode_4F;
	opcode[0x50] = &CPU::opcode_50;
	opcode[0x51] = &CPU::opcode_51;
	opcode[0x52] = &CPU::opcode_52;
	opcode[0x53] = &CPU::opcode_53;
	opcode[0x54] = &CPU::opcode_54;
	opcode[0x55] = &CPU::opcode_55;
	opcode[0x56] = &CPU::opcode_56;
	opcode[0x57] = &CPU::opcode_57;
	opcode[0x58] = &CPU::opcode_58;
	opcode[0x59] = &CPU::opcode_59;
	opcode[0x5A] = &CPU::opcode_5A;
	opcode[0x5B] = &CPU::opcode_5B;
	opcode[0x5C] = &CPU::opcode_5C;
	opcode[0x5D] = &CPU::opcode_5D;
	opcode[0x5E] = &CPU::opcode_5E;
	opcode[0x5F] = &CPU::opcode_5F;
	opcode[0x60] = &CPU::opcode_60;
	opcode[0x61] = &CPU::opcode_61;
	opcode[0x62] = &CPU::opcode_62;
	opcode[0x63] = &CPU::opcode_63;
	opcode[0x64] = &CPU::opcode_64;
	opcode[0x65] = &CPU::opcode_65;
	opcode[0x66] = &CPU::opcode_66;
	opcode[0x67] = &CPU::opcode_67;
	opcode[0x68] = &CPU::opcode_68;
	opcode[0x69] = &CPU::opcode_69;
	opcode[0x6A] = &CPU::opcode_6A;
	opcode[0x6B] = &CPU::opcode_6B;
	opcode[0x6C] = &CPU::opcode_6C;
	opcode[0x6D] = &CPU::opcode_6D;
	opcode[0x6E] = &CPU::opcode_6E;
	opcode[0x6F] = &CPU::opcode_6F;
	opcode[0x70] = &CPU::opcode_70;
	opcode[0x71] = &CPU::opcode_71;
	opcode[0x72] = &CPU::opcode_72;
	opcode[0x73] = &CPU::opcode_73;
	opcode[0x74] = &CPU::opcode_74;
	opcode[0x75] = &CPU::opcode_75;
	opcode[0x76] = &CPU::opcode_76;
	opcode[0x77] = &CPU::opcode_77;
	opcode[0x78] = &CPU::opcode_78;
	opcode[0x79] = &CPU::opcode_79;
	opcode[0x7A] = &CPU::opcode_7A;
	opcode[0x7B] = &CPU::opcode_7B;
	opcode[0x7C] = &CPU::opcode_7C;
	opcode[0x7D] = &CPU::opcode_7D;
	opcode[0x7E] = &CPU::opcode_7E;
	opcode[0x7F] = &CPU::opcode_7F;
	opcode[0x80] = &CPU::opcode_80;
	opcode[0x81] = &CPU::opcode_81;
	opcode[0x82] = &CPU::opcode_82;
	opcode[0x83] = &CPU::opcode_83;
	opcode[0x84] = &CPU::opcode_84;
	opcode[0x85] = &CPU::opcode_85;
	opcode[0x86] = &CPU::opcode_86;
	opcode[0x87] = &CPU::opcode_87;
	opcode[0x88] = &CPU::opcode_88;
	opcode[0x89] = &CPU::opcode_89;
	opcode[0x8A] = &CPU::opcode_8A;
	opcode[0x8B] = &CPU::opcode_8B;
	opcode[0x8C] = &CPU::opcode_8C;
	opcode[0x8D] = &CPU::opcode_8D;
	opcode[0x8E] = &CPU::opcode_8E;
	opcode[0x8F] = &CPU::opcode_8F;
	opcode[0x90] = &CPU::opcode_90;
	opcode[0x91] = &CPU::opcode_91;
	opcode[0x92] = &CPU::opcode_92;
	opcode[0x93] = &CPU::opcode_93;
	opcode[0x94] = &CPU::opcode_94;
	opcode[0x95] = &CPU::opcode_95;
	opcode[0x96] = &CPU::opcode_96;
	opcode[0x97] = &CPU::opcode_97;
	opcode[0x98] = &CPU::opcode_98;
	opcode[0x99] = &CPU::opcode_99;
	opcode[0x9A] = &CPU::opcode_9A;
	opcode[0x9B] = &CPU::opcode_9B;
	opcode[0x9C] = &CPU::opcode_9C;
	opcode[0x9D] = &CPU::opcode_9D;
	opcode[0x9E] = &CPU::opcode_9E;
	opcode[0x9F] = &CPU::opcode_9F;
	opcode[0xA0] = &CPU::opcode_A0;
	opcode[0xA1] = &CPU::opcode_A1;
	opcode[0xA2] = &CPU::opcode_A2;
	opcode[0xA3] = &CPU::opcode_A3;
	opcode[0xA4] = &CPU::opcode_A4;
	opcode[0xA5] = &CPU::opcode_A5;
	opcode[0xA6] = &CPU::opcode_A6;
	opcode[0xA7] = &CPU::opcode_A7;
	opcode[0xA8] = &CPU::opcode_A8;
	opcode[0xA9] = &CPU::opcode_A9;
	opcode[0xAA] = &CPU::opcode_AA;
	opcode[0xAB] = &CPU::opcode_AB;
	opcode[0xAC] = &CPU::opcode_AC;
	opcode[0xAD] = &CPU::opcode_AD;
	opcode[0xAE] = &CPU::opcode_AE;
	opcode[0xAF] = &CPU::opcode_AF;
	opcode[0xB0] = &CPU::opcode_B0;
	opcode[0xB1] = &CPU::opcode_B1;
	opcode[0xB2] = &CPU::opcode_B2;
	opcode[0xB3] = &CPU::opcode_B3;
	opcode[0xB4] = &CPU::opcode_B4;
	opcode[0xB5] = &CPU::opcode_B5;
	opcode[0xB6] = &CPU::opcode_B6;
	opcode[0xB7] = &CPU::opcode_B7;
	opcode[0xB8] = &CPU::opcode_B8;
	opcode[0xB9] = &CPU::opcode_B9;
	opcode[0xBA] = &CPU::opcode_BA;
	opcode[0xBB] = &CPU::opcode_BB;
	opcode[0xBC] = &CPU::opcode_BC;
	opcode[0xBD] = &CPU::opcode_BD;
	opcode[0xBE] = &CPU::opcode_BE;
	opcode[0xBF] = &CPU::opcode_BF;
	opcode[0xC0] = &CPU::opcode_C0;
	opcode[0xC1] = &CPU::opcode_C1;
	opcode[0xC2] = &CPU::opcode_C2;
	opcode[0xC3] = &CPU::opcode_C3;
	opcode[0xC4] = &CPU::opcode_C4;
	opcode[0xC5] = &CPU::opcode_C5;
	opcode[0xC6] = &CPU::opcode_C6;
	opcode[0xC7] = &CPU::opcode_C7;
	opcode[0xC8] = &CPU::opcode_C8;
	opcode[0xC9] = &CPU::opcode_C9;
	opcode[0xCA] = &CPU::opcode_CA;
	opcode[0xCB] = &CPU::opcode_CB;
	opcode[0xCC] = &CPU::opcode_CC;
	opcode[0xCD] = &CPU::opcode_CD;
	opcode[0xCE] = &CPU::opcode_CE;
	opcode[0xCF] = &CPU::opcode_CF;
	opcode[0xD0] = &CPU::opcode_D0;
	opcode[0xD1] = &CPU::opcode_D1;
	opcode[0xD2] = &CPU::opcode_D2;
	opcode[0xD3] = &CPU::opcode_D3;
	opcode[0xD4] = &CPU::opcode_D4;
	opcode[0xD5] = &CPU::opcode_D5;
	opcode[0xD6] = &CPU::opcode_D6;
	opcode[0xD7] = &CPU::opcode_D7;
	opcode[0xD8] = &CPU::opcode_D8;
	opcode[0xD9] = &CPU::opcode_D9;
	opcode[0xDA] = &CPU::opcode_DA;
	opcode[0xDB] = &CPU::opcode_DB;
	opcode[0xDC] = &CPU::opcode_DC;
	opcode[0xDD] = &CPU::opcode_DD;
	opcode[0xDE] = &CPU::opcode_DE;
	opcode[0xDF] = &CPU::opcode_DF;
	opcode[0xE0] = &CPU::opcode_E0;
	opcode[0xE1] = &CPU::opcode_E1;
	opcode[0xE2] = &CPU::opcode_E2;
	opcode[0xE3] = &CPU::opcode_E3;
	opcode[0xE4] = &CPU::opcode_E4;
	opcode[0xE5] = &CPU::opcode_E5;
	opcode[0xE6] = &CPU::opcode_E6;
	opcode[0xE7] = &CPU::opcode_E7;
	opcode[0xE8] = &CPU::opcode_E8;
	opcode[0xE9] = &CPU::opcode_E9;
	opcode[0xEA] = &CPU::opcode_EA;
	opcode[0xEB] = &CPU::opcode_EB;
	opcode[0xEC] = &CPU::opcode_EC;
	opcode[0xED] = &CPU::opcode_ED;
	opcode[0xEE] = &CPU::opcode_EE;
	opcode[0xEF] = &CPU::opcode_EF;
	opcode[0xF0] = &CPU::opcode_F0;
	opcode[0xF1] = &CPU::opcode_F1;
	opcode[0xF2] = &CPU::opcode_F2;
	opcode[0xF3] = &CPU::opcode_F3;
	opcode[0xF4] = &CPU::opcode_F4;
	opcode[0xF5] = &CPU::opcode_F5;
	opcode[0xF6] = &CPU::opcode_F6;
	opcode[0xF7] = &CPU::opcode_F7;
	opcode[0xF8] = &CPU::opcode_F8;
	opcode[0xF9] = &CPU::opcode_F9;
	opcode[0xFA] = &CPU::opcode_FA;
	opcode[0xFB] = &CPU::opcode_FB;
	opcode[0xFC] = &CPU::opcode_FC;
	opcode[0xFD] = &CPU::opcode_FD;
	opcode[0xFE] = &CPU::opcode_FE;
	opcode[0xFF] = &CPU::opcode_FF;
}

// NOP          no operation.
void CPU::opcode_00() {
	
}

// LD BC, nn    load 16-bit immediate into BC.
void CPU::opcode_01() {
	this->registers.setBC(this->memory.LoadWord());
}

// LD (BC), A   saves A to address pointed by BC.
void CPU::opcode_02() {
	this->memory.WriteByte(this->registers.getBC(), this->registers.getA());
}

// INC BC       increment 16-bit BC.
void CPU::opcode_03() {
	this->registers.setBC(this->registers.getBC() + 1);
}

// INC B        increment 8-bit B.
void CPU::opcode_04() {
	this->registers.setB(inc(this->registers.getB()));
}

// DEC B        decrement 8-bit B.
void CPU::opcode_05() {
	this->registers.setB(dec(this->registers.getB()));
}

// LD B, n      load 8 bit immediate into B.
void CPU::opcode_06() {
	this->registers.setB(this->memory.LoadByte());
}

// RLCA		    rotate A left with carry.
void CPU::opcode_07() {
	this->registers.setA(rlca(this->registers.getA()));
}

// LD (nn), SP  save SP to a given address.
void CPU::opcode_08() {
	this->memory.WriteWord(this->memory.LoadWord(), this->registers.getSP());
}

// ADD HL, BC   add 16-bit BC to HL.
void CPU::opcode_09() {
	this->registers.setHL(addWord(this->registers.getHL(), this->registers.getBC()));
}

// LD A, (BC)   load A from address pointed by BC.  
void CPU::opcode_0A() {
	this->registers.setA(this->memory.ReadByte(this->registers.getBC()));
}

// DEC BC       decrement 16-bit BC.
void CPU::opcode_0B() {
	this->registers.setBC(this->registers.getBC() - 1);
}

// INC C        increment 8-bit C.
void CPU::opcode_0C() {
	this->registers.setC(inc(this->registers.getC()));
}

// DEC C        decrement 8-bit C.
void CPU::opcode_0D() {
	this->registers.setC(dec(this->registers.getC()));
}

// LD C, n      load 8-bit immediate to C.
void CPU::opcode_0E() {
	this->registers.setC(this->memory.LoadByte());
}

// RRCA		    rotate A right with carry.
void CPU::opcode_0F() {
	this->registers.setA(rrca(this->registers.getA()));
}

// STOP         stop processor.
void CPU::opcode_10() {
	this->gb_halt = true;
}

// LD DE, nn    load 16-bit immediate into DE.
void CPU::opcode_11() {
	this->registers.setDE(this->memory.LoadWord());
}

// LD (DE), A   save A to address pointed by DE.
void CPU::opcode_12() {
	this->memory.WriteByte(this->registers.getDE(), this->registers.getA());
}

// INC DE       increment 16-bit DE.
void CPU::opcode_13() {
	this->registers.setDE(this->registers.getDE() + 1);
}

// INC D        increment 8-bit D.
void CPU::opcode_14() {
	this->registers.setD(inc(this->registers.getD()));
}

// DEC D        decrement 8-bit D.
void CPU::opcode_15() {
	this->registers.setD(dec(this->registers.getD()));
}

// LD D, n      load 8-bit immediate into D.
void CPU::opcode_16() {
	this->registers.setD(this->memory.LoadByte());
}

// RLA	        rotate A left.
void CPU::opcode_17() {
	this->registers.setA(rla(this->registers.getA()));
}

// JR n         relative jump by signed immediate.
void CPU::opcode_18() {
	jumpRelAddress = this->memory.LoadByte();
	this->registers.setPC(this->registers.getPC() + (sByte) jumpRelAddress);
}

// ADD HL, DE   add 16-bit DE to HL.
void CPU::opcode_19() {
	this->registers.setHL(addWord(this->registers.getHL(), this->registers.getDE()));
}

// LD A, (DE)   load A from address pointed to by DE.
void CPU::opcode_1A() {
	this->registers.setA(this->memory.ReadByte(this->registers.getDE()));
}

// DEC DE       decrement 16-bit DE.
void CPU::opcode_1B() {
	this->registers.setDE(this->registers.getDE() - 1);
}

// INC E        increment 8-bit E.
void CPU::opcode_1C() {
	this->registers.setE(inc(this->registers.getE()));
}

// DEC E        decrement 8-bit E.
void CPU::opcode_1D() {
	this->registers.setE(dec(this->registers.getE()));
}

// LD E, n      Load 8-bit immediate into E.
void CPU::opcode_1E() {
	this->registers.setE(this->memory.LoadByte());
}

// RRA			rotate A right.
void CPU::opcode_1F() {
	this->registers.setA(rra(this->registers.getA()));
}

// JR NZ, n     relative jump by signed immediate if last result was not zero.
void CPU::opcode_20() {
	jumpRelAddress = this->memory.LoadByte();
	if (!registers.getFlag('Z')) {
		this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
		CycleConditionals = true;
	}
}

// LD HL, nn    load 16-bit immediate into HL.
void CPU::opcode_21() {
	this->registers.setHL(this->memory.LoadWord());
}

// LDI (HL), A  save A to address pointed by HL, and increment HL.
void CPU::opcode_22() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getA());
	this->registers.setHL(this->registers.getHL() + 1);
}

// INC HL       increment 16-bit HL.
void CPU::opcode_23() {
	this->registers.setHL(this->registers.getHL() + 1);
}

// INC H        increment 8-bit H.
void CPU::opcode_24() {
	this->registers.setH(inc(this->registers.getH()));
}

// DEC H        decrement 8-bit H.
void CPU::opcode_25() {
	this->registers.setH(dec(this->registers.getH()));
}

// LD H, n      load 8-bit immediate into H.
void CPU::opcode_26() {
	this->registers.setH(this->memory.LoadByte());
}

// DAA          adjust A for BCD addition.
void CPU::opcode_27() {
	daa();
}

// JR Z, n      relative jump by signed immediate if last result was zero.
void CPU::opcode_28() {
	jumpRelAddress = this->memory.LoadByte();
	if (registers.getFlag('Z')) {
		this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
		CycleConditionals = true;
	}
}

// ADD HL, HL   add 16-bit HL to HL.
void CPU::opcode_29() {
	this->registers.setHL(addWord(this->registers.getHL(), this->registers.getHL()));
}

// LDI A, (HL)  load A from address pointed to by HL, and increment HL.
void CPU::opcode_2A() {
	this->registers.setA(this->memory.ReadByte(this->registers.getHL()));
	this->registers.setHL(this->registers.getHL() + 1);
}

// DEC HL       decrement 16-bit HL.
void CPU::opcode_2B() {
	this->registers.setHL(this->registers.getHL() - 1);
}

// INC L        increment 8-bit L.
void CPU::opcode_2C() {
	this->registers.setL(inc(this->registers.getL()));
}

// DEC L        decrement 8-bit L.
void CPU::opcode_2D() {
	this->registers.setL(dec(this->registers.getL()));
}

// LD L, n      load 8-bit immediate into L.
void CPU::opcode_2E() {
	this->registers.setL(this->memory.LoadByte());
}

// CPL          complement (logical NOT) on A.
void CPU::opcode_2F() {
	this->registers.setA(cpl(this->registers.getA()));
}

// JR NC, n     relative jump by signed immediate if last result caused no carry.
void CPU::opcode_30() {
	jumpRelAddress = this->memory.LoadByte();
	if (!registers.getFlag('C')) {
		this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
		CycleConditionals = true;
	}
}

// LD SP, nn    load 16-bit immediate into SP.
void CPU::opcode_31() {
	this->registers.setSP(this->memory.LoadWord());
}

// LDD (HL), A  save A to address pointed by HL, and decrement HL.
void CPU::opcode_32() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getA());
	this->registers.setHL(this->registers.getHL() - 1);
}

// INC SP       increment 16-bit SP.
void CPU::opcode_33() {
	this->registers.setSP(this->registers.getSP() + 1);
}

// INC (HL)     increment value pointed by HL.
void CPU::opcode_34() {
	this->memory.WriteByte(this->registers.getHL(), inc(this->memory.ReadByte(this->registers.getHL())));
}

// DEC (HL)     decrement value pointed by HL.
void CPU::opcode_35() {
	this->memory.WriteByte(this->registers.getHL(), dec(this->memory.ReadByte(this->registers.getHL())));
}

// LD (HL), n   load 8-bit immediate into address pointed by HL.
void CPU::opcode_36() {
	this->memory.WriteByte(this->registers.getHL(), this->memory.LoadByte());
}

// SCF          set carry flag.
void CPU::opcode_37() {
	registers.resetFlag('N');
	registers.resetFlag('H');
	registers.setFlag('C');
}

// JR C, n      relative jump by signed immediate if last result caused carry.
void CPU::opcode_38() {
	jumpRelAddress = this->memory.LoadByte();
	if (registers.getFlag('C')) {
		this->registers.setPC(addSignedWordAddress(this->registers.getPC(), jumpRelAddress));
		CycleConditionals = true;
	}
}

// ADD HL, SP   add 16-bit SP to HL.
void CPU::opcode_39() {
	this->registers.setHL(addWord(this->registers.getHL(), this->registers.getSP()));
}

// LDD A, (HL)  load A from address pointed to by HL, and decrement HL.
void CPU::opcode_3A() {
	this->registers.setA(this->memory.ReadByte(this->registers.getHL()));
	this->registers.setHL(this->registers.getHL() - 1);
}

// DEC SP       decrement 16-bit SP.
void CPU::opcode_3B() {
	this->registers.setSP(this->registers.getSP() - 1);
}

// INC A        increment 8-bit A.
void CPU::opcode_3C() {
	this->registers.setA(inc(this->registers.getA()));
}

// DEC A        decrement 8-bit A.
void CPU::opcode_3D() {
	this->registers.setA(dec(this->registers.getA()));
}

// LD A, n      load 8-bit immediate into A.
void CPU::opcode_3E() {
	this->registers.setA(this->memory.LoadByte());
}

// CCF          complement carry flag.
void CPU::opcode_3F() {
	registers.resetFlag('N');
	registers.resetFlag('H');
	registers.flipFlag('C');
}

// LD B, B      copy B to B.
void CPU::opcode_40() {
	this->registers.setB(this->registers.getB());
}

// LD B, C      copy C to B. 
void CPU::opcode_41() {
	this->registers.setB(this->registers.getC());
}

// LD B, D      copy D to B.
void CPU::opcode_42() {
	this->registers.setB(this->registers.getD());
}

// LD B, E      copy E to B.
void CPU::opcode_43() {
	this->registers.setB(this->registers.getE());
}

// LD B, H      copy H to B.
void CPU::opcode_44() {
	this->registers.setB(this->registers.getH());
}

// LD B, L      copy L to B.
void CPU::opcode_45() {
	this->registers.setB(this->registers.getL());
}

// LD B, (HL)   copy value pointed by HL to B.
void CPU::opcode_46() {
	this->registers.setB(this->memory.ReadByte(this->registers.getHL()));
}

// LD B, A      copy A to B.
void CPU::opcode_47() {
	this->registers.setB(this->registers.getA());
}

// LD C, B      copy B to C.
void CPU::opcode_48() {
	this->registers.setC(this->registers.getB());
}

// LD C, C      copy C to C.
void CPU::opcode_49() {
	this->registers.setC(this->registers.getC());
}

// LD C, D      copy D to C.
void CPU::opcode_4A() {
	this->registers.setC(this->registers.getD());
}

// LD C, E      copy E to C.
void CPU::opcode_4B() {
	this->registers.setC(this->registers.getE());
}

// LD C, H      copy H to C.
void CPU::opcode_4C() {
	this->registers.setC(this->registers.getH());
}

// LD C, L      copy L to C.
void CPU::opcode_4D() {
	this->registers.setC(this->registers.getL());
}

// LD C, (HL)   copy value pointed by HL to C.
void CPU::opcode_4E() {
	this->registers.setC(this->memory.ReadByte(this->registers.getHL()));
}

// LD C, A      copy A to C.
void CPU::opcode_4F() {
	this->registers.setC(this->registers.getA());
}

// LD D, B      copy B to D.
void CPU::opcode_50() {
	this->registers.setD(this->registers.getB());
}

// LD D, C      copy C to D.
void CPU::opcode_51() {
	this->registers.setD(this->registers.getC());
}

// LD D, D      copy D to D.
void CPU::opcode_52() {
	this->registers.setD(this->registers.getD());
}

// LD D, E      copy E to D.
void CPU::opcode_53() {
	this->registers.setD(this->registers.getE());
}

// LD D, H      copy H to D.
void CPU::opcode_54() {
	this->registers.setD(this->registers.getH());
}

// LD D, L      copy L to D.
void CPU::opcode_55() {
	this->registers.setD(this->registers.getL());
}

// LD D, (HL)   copy value pointed by HL to D.
void CPU::opcode_56() {
	this->registers.setD(this->memory.ReadByte(this->registers.getHL()));
}

// LD D, A      copy A to D.
void CPU::opcode_57() {
	this->registers.setD(this->registers.getA());
}

// LD E, B      copy B to E.
void CPU::opcode_58() {
	this->registers.setE(this->registers.getB());
}

// LD E, C      copy C to E.
void CPU::opcode_59() {
	this->registers.setE(this->registers.getC());
}

// LD E, D      copy D to E.
void CPU::opcode_5A() {
	this->registers.setE(this->registers.getD());
}

// LD E, E      copy E to E.
void CPU::opcode_5B() {
	this->registers.setE(this->registers.getE());
}

// LD E, H      copy H to E.
void CPU::opcode_5C() {
	this->registers.setE(this->registers.getH());
}

// LD E, L      copy L to E.
void CPU::opcode_5D() {
	this->registers.setE(this->registers.getL());
}

// LD E, (HL)   copy value pointed by HL to E.
void CPU::opcode_5E() {
	this->registers.setE(this->memory.ReadByte(this->registers.getHL()));
}

// LD E, A      copy A to E.
void CPU::opcode_5F() {
	this->registers.setE(this->registers.getA());
}

// LD H, B      copy B to H.
void CPU::opcode_60() {
	this->registers.setH(this->registers.getB());
}

// LD H, C      copy C to H.
void CPU::opcode_61() {
	this->registers.setH(this->registers.getC());
}

// LD H, D      copy D to H.
void CPU::opcode_62() {
	this->registers.setH(this->registers.getD());
}

// LD H, E      copy E to H.
void CPU::opcode_63() {
	this->registers.setH(this->registers.getE());
}

// LD H, H      copy H to H.
void CPU::opcode_64() {
	this->registers.setH(this->registers.getH());
}

// LD H, L      copy L to H.
void CPU::opcode_65() {
	this->registers.setH(this->registers.getL());
}

// LD H, (HL)   copy value pointed by HL to H.
void CPU::opcode_66() {
	this->registers.setH(this->memory.ReadByte(this->registers.getHL()));
}

// LD H, A      copy A to H.
void CPU::opcode_67() {
	this->registers.setH(this->registers.getA());
}

// LD L, B      copy B to L.
void CPU::opcode_68() {
	this->registers.setL(this->registers.getB());
}

// LD L, C      copy C to L.
void CPU::opcode_69() {
	this->registers.setL(this->registers.getC());
}

// LD L, D      copy D to L.
void CPU::opcode_6A() {
	this->registers.setL(this->registers.getD());
}

// LD L, E      copy E to L. 
void CPU::opcode_6B() {
	this->registers.setL(this->registers.getE());
}

// LD L, H      copy H to L.
void CPU::opcode_6C() {
	this->registers.setL(this->registers.getH());
}

// LD L, L      copy L to L.
void CPU::opcode_6D() {
	this->registers.setL(this->registers.getL());
}

// LD L, (HL)   copy value pointed by HL to L.
void CPU::opcode_6E() {
	this->registers.setL(this->memory.ReadByte(this->registers.getHL()));
}

// LD L, A      copy A to L.
void CPU::opcode_6F() {
	this->registers.setL(this->registers.getA());
}

// LD (HL), B   copy B to address pointed by HL.
void CPU::opcode_70() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getB());
}

// LD (HL), C   copy C to address pointed by HL.
void CPU::opcode_71() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getC());
}

// LD (HL), D   copy D to address pointed by HL.
void CPU::opcode_72() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getD());
}

// LD (HL), E   copy E to address pointed by HL.
void CPU::opcode_73() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getE());
}

// LD (HL), H   copy H to address pointed by HL.
void CPU::opcode_74() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getH());
}

// LD (HL), L   copy L to address pointed by HL.
void CPU::opcode_75() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getL());
}

// HALT         halt processor.
void CPU::opcode_76() {
	this->gb_halt = true;
	this->gb_halt_bug = 0x01;
	this->registers.setPC(this->registers.getPC() + 1);
}

// LD (HL), A   copy A to address pointed by HL.
void CPU::opcode_77() {
	this->memory.WriteByte(this->registers.getHL(), this->registers.getA());
}

// LD A, B      copy B to A.
void CPU::opcode_78() {
	this->registers.setA(this->registers.getB());
}

// LD A, C      copy C to A.
void CPU::opcode_79() {
	this->registers.setA(this->registers.getC());
}

// LD A, D      copy D to A.
void CPU::opcode_7A() {
	this->registers.setA(this->registers.getD());
}

// LD A, E      copy E to A.
void CPU::opcode_7B() {
	this->registers.setA(this->registers.getE());
}

// LD A, H      copy H to A.
void CPU::opcode_7C() {
	this->registers.setA(this->registers.getH());
}

// LD A, L      copy L to A.
void CPU::opcode_7D() {
	this->registers.setA(this->registers.getL());
}

// LD A, (HL)   copy value pointed by HL to A.
void CPU::opcode_7E() {
	this->registers.setA(this->memory.ReadByte(this->registers.getHL()));
}

// LD A, A      copy A to A.
void CPU::opcode_7F() {
	this->registers.setA(this->registers.getA());
}

// ADD A, B     add B to A.
void CPU::opcode_80() {
	this->registers.setA(add(this->registers.getA(), this->registers.getB()));
}

// ADD A, C     add C to A.
void CPU::opcode_81() {
	this->registers.setA(add(this->registers.getA(), this->registers.getC()));
}

// ADD A, D     add D to A.
void CPU::opcode_82() {
	this->registers.setA(add(this->registers.getA(), this->registers.getD()));
}

// ADD A, E     add E to A.
void CPU::opcode_83() {
	this->registers.setA(add(this->registers.getA(), this->registers.getE()));
}

// ADD A, H     add H to A.
void CPU::opcode_84() {
	this->registers.setA(add(this->registers.getA(), this->registers.getH()));
}

// ADD A, L     add L to A.
void CPU::opcode_85() {
	this->registers.setA(add(this->registers.getA(), this->registers.getL()));
}

// ADD A, (HL)  add value pointed by HL to A.
void CPU::opcode_86() {
	this->registers.setA(add(this->registers.getA(), this->memory.ReadByte(this->registers.getHL())));
}

// ADD A, A     add A to A.
void CPU::opcode_87() {
	this->registers.setA(add(this->registers.getA(), this->registers.getA()));
}

// ADC A, B     add B and carry flag to A.
void CPU::opcode_88() {
	this->registers.setA(adc(this->registers.getA(), this->registers.getB()));
}

// ADC A, C     add C and carry flag to A.
void CPU::opcode_89() {
	this->registers.setA(adc(this->registers.getA(), this->registers.getC()));
}

// ADC A, D     add D and carry flag to A.
void CPU::opcode_8A() {
	this->registers.setA(adc(this->registers.getA(), this->registers.getD()));
}

// ADC A, E     add E and carry flag to A.
void CPU::opcode_8B() {
	this->registers.setA(adc(this->registers.getA(), this->registers.getE()));
}

// ADC A, H     add H and carry flag to A.
void CPU::opcode_8C() {
	this->registers.setA(adc(this->registers.getA(), this->registers.getH()));
}

// ADC A, L     add L and carry flag to A.
void CPU::opcode_8D() {
	this->registers.setA(adc(this->registers.getA(), this->registers.getL()));
}

// ADC A, (HL)  add value pointed by HL and carry flag to A.
void CPU::opcode_8E() {
	this->registers.setA(adc(this->registers.getA(), this->memory.ReadByte(this->registers.getHL())));
}

// ADC A, A     add A and carry flag to A.
void CPU::opcode_8F() {
	this->registers.setA(adc(this->registers.getA(), this->registers.getA()));
}

// SUB A, B     subtract B from A.
void CPU::opcode_90() {
	this->registers.setA(sub(this->registers.getA(), this->registers.getB()));
}

// SUB A, C     subtract C from A.
void CPU::opcode_91() {
	this->registers.setA(sub(this->registers.getA(), this->registers.getC()));
}

// SUB A, D     subtract D from A.
void CPU::opcode_92() {
	this->registers.setA(sub(this->registers.getA(), this->registers.getD()));
}

// SUB A, E     subtract E from A.
void CPU::opcode_93() {
	this->registers.setA(sub(this->registers.getA(), this->registers.getE()));
}

// SUB A, H     subtract H from A.
void CPU::opcode_94() {
	this->registers.setA(sub(this->registers.getA(), this->registers.getH()));
}

// SUB A, L     subtract L from A.
void CPU::opcode_95() {
	this->registers.setA(sub(this->registers.getA(), this->registers.getL()));
}

// SUB A, (HL)  subtract value pointed by HL from A.
void CPU::opcode_96() {
	this->registers.setA(sub(this->registers.getA(), this->memory.ReadByte(this->registers.getHL())));
}

// SUB A, A     subtract A from A.
void CPU::opcode_97() {
	this->registers.setA(sub(this->registers.getA(), this->registers.getA()));
}

// SBC A, B     subtract B and carry flag from A.
void CPU::opcode_98() {
	this->registers.setA(sbc(this->registers.getA(), this->registers.getB()));
}

// SBC A, C     subtract C and carry flag from A.
void CPU::opcode_99() {
	this->registers.setA(sbc(this->registers.getA(), this->registers.getC()));
}

// SBC A, D     subtract D and carry flag from A.
void CPU::opcode_9A() {
	this->registers.setA(sbc(this->registers.getA(), this->registers.getD()));
}

// SBC A, E     subtract E and carry flag from A.
void CPU::opcode_9B() {
	this->registers.setA(sbc(this->registers.getA(), this->registers.getE()));
}

// SBC A, H     subtract H and carry flag from A.
void CPU::opcode_9C() {
	this->registers.setA(sbc(this->registers.getA(), this->registers.getH()));
}

// SBC A, L     subtract L and carry flag from A.
void CPU::opcode_9D() {
	this->registers.setA(sbc(this->registers.getA(), this->registers.getL()));
}

// SBC A, (HL)  subtract value pointed by HL and carry flag from A.
void CPU::opcode_9E() {
	this->registers.setA(sbc(this->registers.getA(), this->memory.ReadByte(this->registers.getHL())));
}

// SBC A, A     subtract A and carry flag from A.
void CPU::opcode_9F() {
	this->registers.setA(sbc(this->registers.getA(), this->registers.getA()));
}

// AND B        logical AND B against A.
void CPU::opcode_A0() {
	this->registers.setA(land(this->registers.getA(), this->registers.getB()));
}

// AND C        logical AND C against A.
void CPU::opcode_A1() {
	this->registers.setA(land(this->registers.getA(), this->registers.getC()));
}

// AND D        logical AND D against A.
void CPU::opcode_A2() {
	this->registers.setA(land(this->registers.getA(), this->registers.getD()));
}

// AND E        logical AND E against A.
void CPU::opcode_A3() {
	this->registers.setA(land(this->registers.getA(), this->registers.getE()));
}

// AND H        logical AND H against A.
void CPU::opcode_A4() {
	this->registers.setA(land(this->registers.getA(), this->registers.getH()));
}

// AND L        logical AND L against A.
void CPU::opcode_A5() {
	this->registers.setA(land(this->registers.getA(), this->registers.getL()));
}

// AND (HL)     logical AND value pointed by HL against A.
void CPU::opcode_A6() {
	this->registers.setA(land(this->registers.getA(), this->memory.ReadByte(this->registers.getHL())));
}

// AND A        logical AND A against A.
void CPU::opcode_A7() {
	this->registers.setA(land(this->registers.getA(), this->registers.getA()));
}

// XOR B        logical XOR B against A.
void CPU::opcode_A8() {
	this->registers.setA(lxor(this->registers.getA(), this->registers.getB()));
}

// XOR C        logical XOR C against A.
void CPU::opcode_A9() {
	this->registers.setA(lxor(this->registers.getA(), this->registers.getC()));
}

// XOR D        logical XOR D against A.
void CPU::opcode_AA() {
	this->registers.setA(lxor(this->registers.getA(), this->registers.getD()));
}

// XOR E        logical XOR E against A.
void CPU::opcode_AB() {
	this->registers.setA(lxor(this->registers.getA(), this->registers.getE()));
}

// XOR H        logical XOR H against A.
void CPU::opcode_AC() {
	this->registers.setA(lxor(this->registers.getA(), this->registers.getH()));
}

// XOR L        logical XOR L against A.
void CPU::opcode_AD() {
	this->registers.setA(lxor(this->registers.getA(), this->registers.getL()));
}

// XOR (HL)     logical XOR value pointed by HL against A.
void CPU::opcode_AE() {
	this->registers.setA(lxor(this->registers.getA(), this->memory.ReadByte(this->registers.getHL())));
}

// XOR A        logical XOR A against A.
void CPU::opcode_AF() {
	this->registers.setA(lxor(this->registers.getA(), this->registers.getA()));
}

// OR B         logical OR B against A.
void CPU::opcode_B0() {
	this->registers.setA(lor(this->registers.getA(), this->registers.getB()));
}

// OR C         logical OR C against A.
void CPU::opcode_B1() {
	this->registers.setA(lor(this->registers.getA(), this->registers.getC()));
}

// OR D         logical OR D against A.
void CPU::opcode_B2() {
	this->registers.setA(lor(this->registers.getA(), this->registers.getD()));
}

// OR E         logical OR E against A.
void CPU::opcode_B3() {
	this->registers.setA(lor(this->registers.getA(), this->registers.getE()));
}

// OR H         logical OR H against A.
void CPU::opcode_B4() {
	this->registers.setA(lor(this->registers.getA(), this->registers.getH()));
}

// OR L         logical OR L against A. 
void CPU::opcode_B5() {
	this->registers.setA(lor(this->registers.getA(), this->registers.getL()));
}

// OR (HL)      logical OR value pointed by HL against A.
void CPU::opcode_B6() {
	this->registers.setA(lor(this->registers.getA(), this->memory.ReadByte(this->registers.getHL())));
}

// OR A         logical OR A against A.
void CPU::opcode_B7() {
	this->registers.setA(lor(this->registers.getA(), this->registers.getA()));
}

// CP B         compare B against A.
void CPU::opcode_B8() {
	cp(this->registers.getA(), this->registers.getB());
}

// CP C         compare C against A.
void CPU::opcode_B9() {
	cp(this->registers.getA(), this->registers.getC());
}

// CP D         compare D against A.
void CPU::opcode_BA() {
	cp(this->registers.getA(), this->registers.getD());
}

// CP E         compare E against A.
void CPU::opcode_BB() {
	cp(this->registers.getA(), this->registers.getE());
}

// CP H         compare H against A.
void CPU::opcode_BC() {
	cp(this->registers.getA(), this->registers.getH());
}

// CP L         compare L against A.
void CPU::opcode_BD() {
	cp(this->registers.getA(), this->registers.getL());
}

// CP (HL)      compare value pointed by HL against A.
void CPU::opcode_BE() {
	cp(this->registers.getA(), this->memory.ReadByte(this->registers.getHL()));
}

// CP A         compare A against A.
void CPU::opcode_BF() {
	cp(this->registers.getA(), this->registers.getA());
}

// RET NZ       return if last result was not zero.
void CPU::opcode_C0() {
	if (!registers.getFlag('Z')) {
		this->registers.setPC(this->memory.PopWord());
		jump = true;
		CycleConditionals = true;
	}
}

// POP BC       pop 16-bit value from stack into BC.
void CPU::opcode_C1() {
	this->registers.setBC(this->memory.PopWord());
}

// JP NZ, nn    absolute jump to 16-bit location if last result was not zero.
void CPU::opcode_C2() {
	jumpAddress = this->memory.LoadWord();
	if (!registers.getFlag('Z')) {
		this->registers.setPC(jumpAddress);
		this->jump = true;
		CycleConditionals = true;
	}
}

// JP nn        absolute jump to 16-bit location.
void CPU::opcode_C3() {
	this->registers.setPC(this->memory.LoadWord());
	jump = true;
}

// CALL NZ, nn  call routine at 16-bit location if last result was not zero.
void CPU::opcode_C4() {
	jumpAddress = this->memory.LoadWord();
	if (!registers.getFlag('Z')) {
		call(jumpAddress);
		CycleConditionals = true;
	}
}

// PUSH BC      push 16-bit BC onto stack.
void CPU::opcode_C5() {
	this->memory.PushWord(this->registers.getBC());
}

// ADD A, n     add 8-bit immediate to A. 
void CPU::opcode_C6() {
	this->registers.setA(add(this->registers.getA(), this->memory.LoadByte()));
}

// RST 0        call routine at address 0000h
void CPU::opcode_C7() {
	rst(0x0000);
}

// RET Z        return if last result was zero.
void CPU::opcode_C8() {
	if (registers.getFlag('Z')) {
		this->registers.setPC(this->memory.PopWord());
		jump = true;
		CycleConditionals = true;
	}
}

// RET          return to calling routine.
void CPU::opcode_C9() {
	this->registers.setPC(this->memory.PopWord());
	jump = true;
}

// JP Z, nn     absolute jump to 16-bit location if last result was zero.
void CPU::opcode_CA() {
	jumpAddress = this->memory.LoadWord();
	if (registers.getFlag('Z')) {
		this->registers.setPC(jumpAddress);
		this->jump = true;
		CycleConditionals = true;
	}
}

// Ext ops      extended operations (two-Byte instruction code).
void CPU::opcode_CB() {
	cycles += executeExtendedOpcodes();
}

// CALL Z, nn   call routine at 16-bit location if last result was zero.
void CPU::opcode_CC() {
	jumpAddress = this->memory.LoadWord();
	if (registers.getFlag('Z')) {
		call(jumpAddress);
		CycleConditionals = true;
	}
}

// CALL nn      call routine at 16-bit location.
void CPU::opcode_CD() {
	call(this->memory.LoadWord());
}

// ADC A, n     add 8-bit immediate and carry to A.
void CPU::opcode_CE() {
	this->registers.setA(adc(this->registers.getA(), this->memory.LoadByte()));
}

// RST 8        call routine at adress 0008h.
void CPU::opcode_CF() {
	rst(0x0008);
}

// RET NC       return if last result caused no carry.
void CPU::opcode_D0() {
	if (!registers.getFlag('C')) {
		this->registers.setPC(this->memory.PopWord());
		jump = true;
		CycleConditionals = true;
	}
}

// POP DE       pop 16-bit value from stack into DE.
void CPU::opcode_D1() {
	this->registers.setDE(this->memory.PopWord());
}

// JP NC, nn    absolute jump to 16-bit location if last result caused no carry.
void CPU::opcode_D2() {
	jumpAddress = this->memory.LoadWord();
	if (!registers.getFlag('C')) {
		this->registers.setPC(jumpAddress);
		this->jump = true;
		CycleConditionals = true;
	}
}

// XX           operation removed in this CPU.
void CPU::opcode_D3() {
	removed(0xD3);
}

// CALL NC, nn	call routine at 16-bit location if last result caused no carry.
void CPU::opcode_D4() {
	jumpAddress = this->memory.LoadWord();
	if (!registers.getFlag('C')) {
		call(jumpAddress);
		CycleConditionals = true;
	}
}

// PUSH DE      push 16-bit DE onto stack.
void CPU::opcode_D5() {
	this->memory.PushWord(this->registers.getDE());
}

// SUB A, n     subtract 8-bit immediate from A. 
void CPU::opcode_D6() {
	this->registers.setA(sub(this->registers.getA(), this->memory.LoadByte()));
}

// RST 10       call routine at address 0010h.
void CPU::opcode_D7() {
	rst(0x0010);
}

// RET C        return if last result caused carry.
void CPU::opcode_D8() {
	if (registers.getFlag('C')) {
		this->registers.setPC(this->memory.PopWord());
		jump = true;
		CycleConditionals = true;
	}
}

// RETI         enable interrupts and return to calling routine.
void CPU::opcode_D9() {
	this->gb_ime = true;
	this->jump = true;
	this->registers.setPC(this->memory.PopWord());
}

// JP C, nn     absolute jump to 16-bit location if last result caused carry.
void CPU::opcode_DA() {
	jumpAddress = this->memory.LoadWord();
	if (registers.getFlag('C')) {
		this->registers.setPC(jumpAddress);
		this->jump = true;
		CycleConditionals = true;
	}
}

// XX           operation removed in this CPU. 
void CPU::opcode_DB() {
	removed(0xDB);
}

// CALL C, nn   call routine at 16-bit location if last result caused carry.
void CPU::opcode_DC() {
	jumpAddress = this->memory.LoadWord();
	if (registers.getFlag('C')) {
		call(jumpAddress);
		CycleConditionals = true;
	}
}

// XX           operation removed in this CPU.
void CPU::opcode_DD() {
	removed(0xDD);
}

// SBC A, n     subtract 8-bit immediate and carry from A. 
void CPU::opcode_DE() {
	this->registers.setA(sbc(this->registers.getA(), this->memory.LoadByte()));
}

// RST 18       call routine at address 0018h.
void CPU::opcode_DF() {
	rst(0x0018);
}

// LDH (n), A   save A at address pointed to by (FF00h + 8-bit immediate)
void CPU::opcode_E0() {
	this->memory.WriteByte(ADDR_IO + this->memory.LoadByte(), this->registers.getA());
}
// POP HL       pop 16-bit value from stack into HL.
void CPU::opcode_E1() {
	this->registers.setHL(this->memory.PopWord());
}

// LDH (C), A   save A at address pointed to by (FF00h + C)
void CPU::opcode_E2() {
	this->memory.WriteByte(ADDR_IO + this->registers.getC(), this->registers.getA());
}

// XX           operation removed in this CPU.
void CPU::opcode_E3() {
	removed(0xE3);
}

// XX           operation removed in this CPU.
void CPU::opcode_E4() {
	removed(0xE4);
}

// PUSH HL      Push 16-bit HL onto stack.
void CPU::opcode_E5() {
	this->memory.PushWord(this->registers.getHL());
}

// AND n        logical AND 8-bit immediate against A.
void CPU::opcode_E6() {
	this->registers.setA(land(this->registers.getA(), this->memory.LoadByte()));
}

// RST 20       call routine at address 0020h.
void CPU::opcode_E7() {
	rst(0x0020);
}

// ADD SP, d    add signed 8-bit immediate to SP.
void CPU::opcode_E8() {
	this->registers.setSP(addSignedWord(this->registers.getSP(), this->memory.LoadByte()));
}

// JP (HL)      jump to 16-bit value pointed by HL.
void CPU::opcode_E9() {
	this->registers.setPC(this->registers.getHL());
	this->jump = true;
}

// LD (nn), A   save A at given 16-bit address.
void CPU::opcode_EA() {
	this->memory.WriteByte(this->memory.LoadWord(), this->registers.getA());
}

// XX           operation removed in this CPU.
void CPU::opcode_EB() {
	removed(0xEB);
}

// XX           operation removed in this CPU.
void CPU::opcode_EC() {
	removed(0xEC);
}

// XX           operation removed in this CPU.
void CPU::opcode_ED() {
	removed(0xED);
}

// XOR n        logical XOR 8-bit immediate against A.
void CPU::opcode_EE() {
	this->registers.setA(lxor(this->registers.getA(), this->memory.LoadByte()));
}

// RST 28       call routine at address 0028h.
void CPU::opcode_EF() {
	rst(0x0028);
}

// LDH A, (n)   load A from address pointed to by (FF00h + 8-bit immediate).
void CPU::opcode_F0() {
	this->registers.setA(this->memory.ReadByte(ADDR_IO + this->memory.LoadByte()));
}

// POP AF       pop 16-bit value from stack into AF.
void CPU::opcode_F1() {
	this->registers.setAF(this->memory.PopWord());
}

// LD A, (C)    load A from address pointed to by (FF00h + C)
void CPU::opcode_F2() {
	this->registers.setA(this->memory.ReadByte(ADDR_IO + this->registers.getC()));
}

// DI           disable interrupts
void CPU::opcode_F3() {
	this->gb_ime = false;
}

// XX           operation removed in this CPU.
void CPU::opcode_F4() {
	removed(0xF4);
}

// PUSH AF      push 16-bit AF onto stack.
void CPU::opcode_F5() {
	this->memory.PushWord(this->registers.getAF());
}

// OR n         logical OR 8-bit immediate against A.
void CPU::opcode_F6() {
	this->registers.setA(lor(this->registers.getA(), this->memory.LoadByte()));
}

// RST 30       call routine at address 0030h.
void CPU::opcode_F7() {
	rst(0x0030);
}

// LDHL SP, d   add signed 8-bit immediate to SP and save result in HL.
void CPU::opcode_F8() {
	this->registers.setHL(addSignedWord(this->registers.getSP(), this->memory.LoadByte()));
}

// LD SP, HL    copy HL to SP.
void CPU::opcode_F9() {
	this->registers.setSP(this->registers.getHL());
}

// LD A, (nn)   load A from given 16-bit address.
void CPU::opcode_FA() {
	this->registers.setA(this->memory.ReadByte(this->memory.LoadWord()));
}

// EI           enable interrupts.
void CPU::opcode_FB() {
	this->gb_ime = true;
}

// XX           operation removed in this CPU.
void CPU::opcode_FC() {
	removed(0xFC);
}

// XX           operation removed in this CPU.
void CPU::opcode_FD() {
	removed(0xFD);
}

// CP n         compare 8-bit immediate against A.
void CPU::opcode_FE() {
	cp(this->registers.getA(), this->memory.LoadByte());
}

// RST 38    	call routine at address 0038h.
void CPU::opcode_FF() {
	rst(0x0038);
}