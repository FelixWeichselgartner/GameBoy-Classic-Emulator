#include "../include/CPU.hpp"

// init cb opcode pointer
void CPU::Init_CBOPCODE() {
    cb_opcode[0x00] = &CPU::cb_opcode_00;
    cb_opcode[0x01] = &CPU::cb_opcode_01;
    cb_opcode[0x02] = &CPU::cb_opcode_02;
    cb_opcode[0x03] = &CPU::cb_opcode_03;
    cb_opcode[0x04] = &CPU::cb_opcode_04;
    cb_opcode[0x05] = &CPU::cb_opcode_05;
    cb_opcode[0x06] = &CPU::cb_opcode_06;
    cb_opcode[0x07] = &CPU::cb_opcode_07;
    cb_opcode[0x08] = &CPU::cb_opcode_08;
    cb_opcode[0x09] = &CPU::cb_opcode_09;
    cb_opcode[0x0A] = &CPU::cb_opcode_0A;
    cb_opcode[0x0B] = &CPU::cb_opcode_0B;
    cb_opcode[0x0C] = &CPU::cb_opcode_0C;
    cb_opcode[0x0D] = &CPU::cb_opcode_0D;
    cb_opcode[0x0E] = &CPU::cb_opcode_0E;
    cb_opcode[0x0F] = &CPU::cb_opcode_0F;
    cb_opcode[0x10] = &CPU::cb_opcode_10;
    cb_opcode[0x11] = &CPU::cb_opcode_11;
    cb_opcode[0x12] = &CPU::cb_opcode_12;
    cb_opcode[0x13] = &CPU::cb_opcode_13;
    cb_opcode[0x14] = &CPU::cb_opcode_14;
    cb_opcode[0x15] = &CPU::cb_opcode_15;
    cb_opcode[0x16] = &CPU::cb_opcode_16;
    cb_opcode[0x17] = &CPU::cb_opcode_17;
    cb_opcode[0x18] = &CPU::cb_opcode_18;
    cb_opcode[0x19] = &CPU::cb_opcode_19;
    cb_opcode[0x1A] = &CPU::cb_opcode_1A;
    cb_opcode[0x1B] = &CPU::cb_opcode_1B;
    cb_opcode[0x1C] = &CPU::cb_opcode_1C;
    cb_opcode[0x1D] = &CPU::cb_opcode_1D;
    cb_opcode[0x1E] = &CPU::cb_opcode_1E;
    cb_opcode[0x1F] = &CPU::cb_opcode_1F;
    cb_opcode[0x20] = &CPU::cb_opcode_20;
    cb_opcode[0x21] = &CPU::cb_opcode_21;
    cb_opcode[0x22] = &CPU::cb_opcode_22;
    cb_opcode[0x23] = &CPU::cb_opcode_23;
    cb_opcode[0x24] = &CPU::cb_opcode_24;
    cb_opcode[0x25] = &CPU::cb_opcode_25;
    cb_opcode[0x26] = &CPU::cb_opcode_26;
    cb_opcode[0x27] = &CPU::cb_opcode_27;
    cb_opcode[0x28] = &CPU::cb_opcode_28;
    cb_opcode[0x29] = &CPU::cb_opcode_29;
    cb_opcode[0x2A] = &CPU::cb_opcode_2A;
    cb_opcode[0x2B] = &CPU::cb_opcode_2B;
    cb_opcode[0x2C] = &CPU::cb_opcode_2C;
    cb_opcode[0x2D] = &CPU::cb_opcode_2D;
    cb_opcode[0x2E] = &CPU::cb_opcode_2E;
    cb_opcode[0x2F] = &CPU::cb_opcode_2F;
    cb_opcode[0x30] = &CPU::cb_opcode_30;
    cb_opcode[0x31] = &CPU::cb_opcode_31;
    cb_opcode[0x32] = &CPU::cb_opcode_32;
    cb_opcode[0x33] = &CPU::cb_opcode_33;
    cb_opcode[0x34] = &CPU::cb_opcode_34;
    cb_opcode[0x35] = &CPU::cb_opcode_35;
    cb_opcode[0x36] = &CPU::cb_opcode_36;
    cb_opcode[0x37] = &CPU::cb_opcode_37;
    cb_opcode[0x38] = &CPU::cb_opcode_38;
    cb_opcode[0x39] = &CPU::cb_opcode_39;
    cb_opcode[0x3A] = &CPU::cb_opcode_3A;
    cb_opcode[0x3B] = &CPU::cb_opcode_3B;
    cb_opcode[0x3C] = &CPU::cb_opcode_3C;
    cb_opcode[0x3D] = &CPU::cb_opcode_3D;
    cb_opcode[0x3E] = &CPU::cb_opcode_3E;
    cb_opcode[0x3F] = &CPU::cb_opcode_3F;
    cb_opcode[0x40] = &CPU::cb_opcode_40;
    cb_opcode[0x41] = &CPU::cb_opcode_41;
    cb_opcode[0x42] = &CPU::cb_opcode_42;
    cb_opcode[0x43] = &CPU::cb_opcode_43;
    cb_opcode[0x44] = &CPU::cb_opcode_44;
    cb_opcode[0x45] = &CPU::cb_opcode_45;
    cb_opcode[0x46] = &CPU::cb_opcode_46;
    cb_opcode[0x47] = &CPU::cb_opcode_47;
    cb_opcode[0x48] = &CPU::cb_opcode_48;
    cb_opcode[0x49] = &CPU::cb_opcode_49;
    cb_opcode[0x4A] = &CPU::cb_opcode_4A;
    cb_opcode[0x4B] = &CPU::cb_opcode_4B;
    cb_opcode[0x4C] = &CPU::cb_opcode_4C;
    cb_opcode[0x4D] = &CPU::cb_opcode_4D;
    cb_opcode[0x4E] = &CPU::cb_opcode_4E;
    cb_opcode[0x4F] = &CPU::cb_opcode_4F;
    cb_opcode[0x50] = &CPU::cb_opcode_50;
    cb_opcode[0x51] = &CPU::cb_opcode_51;
    cb_opcode[0x52] = &CPU::cb_opcode_52;
    cb_opcode[0x53] = &CPU::cb_opcode_53;
    cb_opcode[0x54] = &CPU::cb_opcode_54;
    cb_opcode[0x55] = &CPU::cb_opcode_55;
    cb_opcode[0x56] = &CPU::cb_opcode_56;
    cb_opcode[0x57] = &CPU::cb_opcode_57;
    cb_opcode[0x58] = &CPU::cb_opcode_58;
    cb_opcode[0x59] = &CPU::cb_opcode_59;
    cb_opcode[0x5A] = &CPU::cb_opcode_5A;
    cb_opcode[0x5B] = &CPU::cb_opcode_5B;
    cb_opcode[0x5C] = &CPU::cb_opcode_5C;
    cb_opcode[0x5D] = &CPU::cb_opcode_5D;
    cb_opcode[0x5E] = &CPU::cb_opcode_5E;
    cb_opcode[0x5F] = &CPU::cb_opcode_5F;
    cb_opcode[0x60] = &CPU::cb_opcode_60;
    cb_opcode[0x61] = &CPU::cb_opcode_61;
    cb_opcode[0x62] = &CPU::cb_opcode_62;
    cb_opcode[0x63] = &CPU::cb_opcode_63;
    cb_opcode[0x64] = &CPU::cb_opcode_64;
    cb_opcode[0x65] = &CPU::cb_opcode_65;
    cb_opcode[0x66] = &CPU::cb_opcode_66;
    cb_opcode[0x67] = &CPU::cb_opcode_67;
    cb_opcode[0x68] = &CPU::cb_opcode_68;
    cb_opcode[0x69] = &CPU::cb_opcode_69;
    cb_opcode[0x6A] = &CPU::cb_opcode_6A;
    cb_opcode[0x6B] = &CPU::cb_opcode_6B;
    cb_opcode[0x6C] = &CPU::cb_opcode_6C;
    cb_opcode[0x6D] = &CPU::cb_opcode_6D;
    cb_opcode[0x6E] = &CPU::cb_opcode_6E;
    cb_opcode[0x6F] = &CPU::cb_opcode_6F;
    cb_opcode[0x70] = &CPU::cb_opcode_70;
    cb_opcode[0x71] = &CPU::cb_opcode_71;
    cb_opcode[0x72] = &CPU::cb_opcode_72;
    cb_opcode[0x73] = &CPU::cb_opcode_73;
    cb_opcode[0x74] = &CPU::cb_opcode_74;
    cb_opcode[0x75] = &CPU::cb_opcode_75;
    cb_opcode[0x76] = &CPU::cb_opcode_76;
    cb_opcode[0x77] = &CPU::cb_opcode_77;
    cb_opcode[0x78] = &CPU::cb_opcode_78;
    cb_opcode[0x79] = &CPU::cb_opcode_79;
    cb_opcode[0x7A] = &CPU::cb_opcode_7A;
    cb_opcode[0x7B] = &CPU::cb_opcode_7B;
    cb_opcode[0x7C] = &CPU::cb_opcode_7C;
    cb_opcode[0x7D] = &CPU::cb_opcode_7D;
    cb_opcode[0x7E] = &CPU::cb_opcode_7E;
    cb_opcode[0x7F] = &CPU::cb_opcode_7F;
    cb_opcode[0x80] = &CPU::cb_opcode_80;
    cb_opcode[0x81] = &CPU::cb_opcode_81;
    cb_opcode[0x82] = &CPU::cb_opcode_82;
    cb_opcode[0x83] = &CPU::cb_opcode_83;
    cb_opcode[0x84] = &CPU::cb_opcode_84;
    cb_opcode[0x85] = &CPU::cb_opcode_85;
    cb_opcode[0x86] = &CPU::cb_opcode_86;
    cb_opcode[0x87] = &CPU::cb_opcode_87;
    cb_opcode[0x88] = &CPU::cb_opcode_88;
    cb_opcode[0x89] = &CPU::cb_opcode_89;
    cb_opcode[0x8A] = &CPU::cb_opcode_8A;
    cb_opcode[0x8B] = &CPU::cb_opcode_8B;
    cb_opcode[0x8C] = &CPU::cb_opcode_8C;
    cb_opcode[0x8D] = &CPU::cb_opcode_8D;
    cb_opcode[0x8E] = &CPU::cb_opcode_8E;
    cb_opcode[0x8F] = &CPU::cb_opcode_8F;
    cb_opcode[0x90] = &CPU::cb_opcode_90;
    cb_opcode[0x91] = &CPU::cb_opcode_91;
    cb_opcode[0x92] = &CPU::cb_opcode_92;
    cb_opcode[0x93] = &CPU::cb_opcode_93;
    cb_opcode[0x94] = &CPU::cb_opcode_94;
    cb_opcode[0x95] = &CPU::cb_opcode_95;
    cb_opcode[0x96] = &CPU::cb_opcode_96;
    cb_opcode[0x97] = &CPU::cb_opcode_97;
    cb_opcode[0x98] = &CPU::cb_opcode_98;
    cb_opcode[0x99] = &CPU::cb_opcode_99;
    cb_opcode[0x9A] = &CPU::cb_opcode_9A;
    cb_opcode[0x9B] = &CPU::cb_opcode_9B;
    cb_opcode[0x9C] = &CPU::cb_opcode_9C;
    cb_opcode[0x9D] = &CPU::cb_opcode_9D;
    cb_opcode[0x9E] = &CPU::cb_opcode_9E;
    cb_opcode[0x9F] = &CPU::cb_opcode_9F;
    cb_opcode[0xA0] = &CPU::cb_opcode_A0;
    cb_opcode[0xA1] = &CPU::cb_opcode_A1;
    cb_opcode[0xA2] = &CPU::cb_opcode_A2;
    cb_opcode[0xA3] = &CPU::cb_opcode_A3;
    cb_opcode[0xA4] = &CPU::cb_opcode_A4;
    cb_opcode[0xA5] = &CPU::cb_opcode_A5;
    cb_opcode[0xA6] = &CPU::cb_opcode_A6;
    cb_opcode[0xA7] = &CPU::cb_opcode_A7;
    cb_opcode[0xA8] = &CPU::cb_opcode_A8;
    cb_opcode[0xA9] = &CPU::cb_opcode_A9;
    cb_opcode[0xAA] = &CPU::cb_opcode_AA;
    cb_opcode[0xAB] = &CPU::cb_opcode_AB;
    cb_opcode[0xAC] = &CPU::cb_opcode_AC;
    cb_opcode[0xAD] = &CPU::cb_opcode_AD;
    cb_opcode[0xAE] = &CPU::cb_opcode_AE;
    cb_opcode[0xAF] = &CPU::cb_opcode_AF;
    cb_opcode[0xB0] = &CPU::cb_opcode_B0;
    cb_opcode[0xB1] = &CPU::cb_opcode_B1;
    cb_opcode[0xB2] = &CPU::cb_opcode_B2;
    cb_opcode[0xB3] = &CPU::cb_opcode_B3;
    cb_opcode[0xB4] = &CPU::cb_opcode_B4;
    cb_opcode[0xB5] = &CPU::cb_opcode_B5;
    cb_opcode[0xB6] = &CPU::cb_opcode_B6;
    cb_opcode[0xB7] = &CPU::cb_opcode_B7;
    cb_opcode[0xB8] = &CPU::cb_opcode_B8;
    cb_opcode[0xB9] = &CPU::cb_opcode_B9;
    cb_opcode[0xBA] = &CPU::cb_opcode_BA;
    cb_opcode[0xBB] = &CPU::cb_opcode_BB;
    cb_opcode[0xBC] = &CPU::cb_opcode_BC;
    cb_opcode[0xBD] = &CPU::cb_opcode_BD;
    cb_opcode[0xBE] = &CPU::cb_opcode_BE;
    cb_opcode[0xBF] = &CPU::cb_opcode_BF;
    cb_opcode[0xC0] = &CPU::cb_opcode_C0;
    cb_opcode[0xC1] = &CPU::cb_opcode_C1;
    cb_opcode[0xC2] = &CPU::cb_opcode_C2;
    cb_opcode[0xC3] = &CPU::cb_opcode_C3;
    cb_opcode[0xC4] = &CPU::cb_opcode_C4;
    cb_opcode[0xC5] = &CPU::cb_opcode_C5;
    cb_opcode[0xC6] = &CPU::cb_opcode_C6;
    cb_opcode[0xC7] = &CPU::cb_opcode_C7;
    cb_opcode[0xC8] = &CPU::cb_opcode_C8;
    cb_opcode[0xC9] = &CPU::cb_opcode_C9;
    cb_opcode[0xCA] = &CPU::cb_opcode_CA;
    cb_opcode[0xCB] = &CPU::cb_opcode_CB;
    cb_opcode[0xCC] = &CPU::cb_opcode_CC;
    cb_opcode[0xCD] = &CPU::cb_opcode_CD;
    cb_opcode[0xCE] = &CPU::cb_opcode_CE;
    cb_opcode[0xCF] = &CPU::cb_opcode_CF;
    cb_opcode[0xD0] = &CPU::cb_opcode_D0;
    cb_opcode[0xD1] = &CPU::cb_opcode_D1;
    cb_opcode[0xD2] = &CPU::cb_opcode_D2;
    cb_opcode[0xD3] = &CPU::cb_opcode_D3;
    cb_opcode[0xD4] = &CPU::cb_opcode_D4;
    cb_opcode[0xD5] = &CPU::cb_opcode_D5;
    cb_opcode[0xD6] = &CPU::cb_opcode_D6;
    cb_opcode[0xD7] = &CPU::cb_opcode_D7;
    cb_opcode[0xD8] = &CPU::cb_opcode_D8;
    cb_opcode[0xD9] = &CPU::cb_opcode_D9;
    cb_opcode[0xDA] = &CPU::cb_opcode_DA;
    cb_opcode[0xDB] = &CPU::cb_opcode_DB;
    cb_opcode[0xDC] = &CPU::cb_opcode_DC;
    cb_opcode[0xDD] = &CPU::cb_opcode_DD;
    cb_opcode[0xDE] = &CPU::cb_opcode_DE;
    cb_opcode[0xDF] = &CPU::cb_opcode_DF;
    cb_opcode[0xE0] = &CPU::cb_opcode_E0;
    cb_opcode[0xE1] = &CPU::cb_opcode_E1;
    cb_opcode[0xE2] = &CPU::cb_opcode_E2;
    cb_opcode[0xE3] = &CPU::cb_opcode_E3;
    cb_opcode[0xE4] = &CPU::cb_opcode_E4;
    cb_opcode[0xE5] = &CPU::cb_opcode_E5;
    cb_opcode[0xE6] = &CPU::cb_opcode_E6;
    cb_opcode[0xE7] = &CPU::cb_opcode_E7;
    cb_opcode[0xE8] = &CPU::cb_opcode_E8;
    cb_opcode[0xE9] = &CPU::cb_opcode_E9;
    cb_opcode[0xEA] = &CPU::cb_opcode_EA;
    cb_opcode[0xEB] = &CPU::cb_opcode_EB;
    cb_opcode[0xEC] = &CPU::cb_opcode_EC;
    cb_opcode[0xED] = &CPU::cb_opcode_ED;
    cb_opcode[0xEE] = &CPU::cb_opcode_EE;
    cb_opcode[0xEF] = &CPU::cb_opcode_EF;
    cb_opcode[0xF0] = &CPU::cb_opcode_F0;
    cb_opcode[0xF1] = &CPU::cb_opcode_F1;
    cb_opcode[0xF2] = &CPU::cb_opcode_F2;
    cb_opcode[0xF3] = &CPU::cb_opcode_F3;
    cb_opcode[0xF4] = &CPU::cb_opcode_F4;
    cb_opcode[0xF5] = &CPU::cb_opcode_F5;
    cb_opcode[0xF6] = &CPU::cb_opcode_F6;
    cb_opcode[0xF7] = &CPU::cb_opcode_F7;
    cb_opcode[0xF8] = &CPU::cb_opcode_F8;
    cb_opcode[0xF9] = &CPU::cb_opcode_F9;
    cb_opcode[0xFA] = &CPU::cb_opcode_FA;
    cb_opcode[0xFB] = &CPU::cb_opcode_FB;
    cb_opcode[0xFC] = &CPU::cb_opcode_FC;
    cb_opcode[0xFD] = &CPU::cb_opcode_FD;
    cb_opcode[0xFE] = &CPU::cb_opcode_FE;
    cb_opcode[0xFF] = &CPU::cb_opcode_FF;
}

// RLC B	    rotate B left with carry.
void CPU::cb_opcode_00() {
    this->registers.setB(rlc(this->registers.getB()));
}

// RLC C
void CPU::cb_opcode_01() {
    this->registers.setC(rlc(this->registers.getC()));
}

// RLC D
void CPU::cb_opcode_02() {
    this->registers.setD(rlc(this->registers.getD()));
}

// RLC E
void CPU::cb_opcode_03() {
    this->registers.setE(rlc(this->registers.getE()));
}

// RLC H
void CPU::cb_opcode_04() {
    this->registers.setH(rlc(this->registers.getH()));
}

// RLC L
void CPU::cb_opcode_05() {
    this->registers.setL(rlc(this->registers.getL()));
}

// RLC (HL)
void CPU::cb_opcode_06() {
    this->memory.WriteByte(this->registers.getHL(), rlc(this->memory.ReadByte(this->registers.getHL())));
}

// RLC A
void CPU::cb_opcode_07() {
    this->registers.setA(rlc(this->registers.getA()));
}

// RRC B	    rotate B right with carry.
void CPU::cb_opcode_08() {
    this->registers.setB(rrc(this->registers.getB()));
}

// RRC C
void CPU::cb_opcode_09() {
    this->registers.setC(rrc(this->registers.getC()));
}

// RRC D
void CPU::cb_opcode_0A() {
    this->registers.setD(rrc(this->registers.getD()));
}

// RRC E
void CPU::cb_opcode_0B() {
    this->registers.setE(rrc(this->registers.getE()));
}

// RRC H
void CPU::cb_opcode_0C() {
    this->registers.setH(rrc(this->registers.getH()));
}

// RRC L
void CPU::cb_opcode_0D() {
    this->registers.setL(rrc(this->registers.getL()));
}

// RRC (HL)
void CPU::cb_opcode_0E() {
    this->memory.WriteByte(this->registers.getHL(), rrc(this->memory.ReadByte(this->registers.getHL())));
}

// RRC A
void CPU::cb_opcode_0F() {
    this->registers.setA(rrc(this->registers.getA()));
}

// RL B		    rotate B left.
void CPU::cb_opcode_10() {
    this->registers.setB(rl(this->registers.getB()));
}

// RL C
void CPU::cb_opcode_11() {
    this->registers.setC(rl(this->registers.getC()));
}

// RL D
void CPU::cb_opcode_12() {
    this->registers.setD(rl(this->registers.getD()));
}

// RL E
void CPU::cb_opcode_13() {
    this->registers.setE(rl(this->registers.getE()));
}

// RL H
void CPU::cb_opcode_14() {
    this->registers.setH(rl(this->registers.getH()));
}

// RL L
void CPU::cb_opcode_15() {
    this->registers.setL(rl(this->registers.getL()));
}

// RL (HL)
void CPU::cb_opcode_16() {
    this->memory.WriteByte(this->registers.getHL(), rl(this->memory.ReadByte(this->registers.getHL())));
}

// RL A
void CPU::cb_opcode_17() {
    this->registers.setA(rl(this->registers.getA()));
}

// RR B		    rotate B right.
void CPU::cb_opcode_18() {
    this->registers.setB(rr(this->registers.getB()));
}

// RR C
void CPU::cb_opcode_19() {
    this->registers.setC(rr(this->registers.getC()));
}

// RR D
void CPU::cb_opcode_1A() {
    this->registers.setD(rr(this->registers.getD()));
}

// RR E
void CPU::cb_opcode_1B() {
    this->registers.setE(rr(this->registers.getE()));
}

// RR H
void CPU::cb_opcode_1C() {
    this->registers.setH(rr(this->registers.getH()));
}

// RR L
void CPU::cb_opcode_1D() {
    this->registers.setL(rr(this->registers.getL()));
}

// RR (HL)
void CPU::cb_opcode_1E() {
    this->memory.WriteByte(this->registers.getHL(), rr(this->memory.ReadByte(this->registers.getHL())));
}

// RR A
void CPU::cb_opcode_1F() {
    this->registers.setA(rr(this->registers.getA()));
}

// SLA B	    shift B left preserving sign.
void CPU::cb_opcode_20() {
    this->registers.setB(sla(this->registers.getB()));
}

// SLA C
void CPU::cb_opcode_21() {
    this->registers.setC(sla(this->registers.getC()));
}

// SLA D
void CPU::cb_opcode_22() {
    this->registers.setD(sla(this->registers.getD()));
}

// SLA E
void CPU::cb_opcode_23() {
    this->registers.setE(sla(this->registers.getE()));
}

// SLA H
void CPU::cb_opcode_24() {
    this->registers.setH(sla(this->registers.getH()));
}

// SLA L
void CPU::cb_opcode_25() {
    this->registers.setL(sla(this->registers.getL()));
}

// SLA (HL)
void CPU::cb_opcode_26() {
    this->memory.WriteByte(this->registers.getHL(), sla(this->memory.ReadByte(this->registers.getHL())));
}

// SLA A
void CPU::cb_opcode_27() {
    this->registers.setA(sla(this->registers.getA()));
}

// SRA B	    shift B right preserving sign.
void CPU::cb_opcode_28() {
    this->registers.setB(sra(this->registers.getB()));
}

// SRA C
void CPU::cb_opcode_29() {
    this->registers.setC(sra(this->registers.getC()));
}

// SRA D
void CPU::cb_opcode_2A() {
    this->registers.setD(sra(this->registers.getD()));
}

// SRA E
void CPU::cb_opcode_2B() {
    this->registers.setE(sra(this->registers.getE()));
}

// SRA H
void CPU::cb_opcode_2C() {
    this->registers.setH(sra(this->registers.getH()));
}

// SRA L
void CPU::cb_opcode_2D() {
    this->registers.setL(sra(this->registers.getL()));
}

// SRA (HL)
void CPU::cb_opcode_2E() {
    this->memory.WriteByte(this->registers.getHL(), sra(this->memory.ReadByte(this->registers.getHL())));
}

// SRA A
void CPU::cb_opcode_2F() {
    this->registers.setA(sra(this->registers.getA()));
}

// SWAP B	    swap nybbles in B.
void CPU::cb_opcode_30() {
    this->registers.setB(swap(this->registers.getB()));
}

// SWAP C
void CPU::cb_opcode_31() {
    this->registers.setC(swap(this->registers.getC()));
}

// SWAP D
void CPU::cb_opcode_32() {
    this->registers.setD(swap(this->registers.getD()));
}

// SWAP E
void CPU::cb_opcode_33() {
    this->registers.setE(swap(this->registers.getE()));
}

// SWAP H
void CPU::cb_opcode_34() {
    this->registers.setH(swap(this->registers.getH()));
}

// SWAP L
void CPU::cb_opcode_35() {
    this->registers.setL(swap(this->registers.getL()));
}

// SWAP (HL)
void CPU::cb_opcode_36() {
    this->memory.WriteByte(this->registers.getHL(), swap(this->memory.ReadByte(this->registers.getHL())));
}

// SWAP A
void CPU::cb_opcode_37() {
    this->registers.setA(swap(this->registers.getA()));
}

// SRL B	    shift B right.
void CPU::cb_opcode_38() {
    this->registers.setB(srl(this->registers.getB()));
}

// SRL C
void CPU::cb_opcode_39() {
    this->registers.setC(srl(this->registers.getC()));
}

// SRL D
void CPU::cb_opcode_3A() {
    this->registers.setD(srl(this->registers.getD()));
}

// SRL E
void CPU::cb_opcode_3B() {
    this->registers.setE(srl(this->registers.getE()));
}

// SRL H
void CPU::cb_opcode_3C() {
    this->registers.setH(srl(this->registers.getH()));
}

// SRL L
void CPU::cb_opcode_3D() {
    this->registers.setL(srl(this->registers.getL()));
}

// SRL (HL)
void CPU::cb_opcode_3E() {
    this->memory.WriteByte(this->registers.getHL(), srl(this->memory.ReadByte(this->registers.getHL())));
}

// SRL A
void CPU::cb_opcode_3F() {
    this->registers.setA(srl(this->registers.getA()));
}

// BIT 0, B		test bit 0 of B.
void CPU::cb_opcode_40() {
    bit(0, this->registers.getB());
}

// BIT 0, C
void CPU::cb_opcode_41() {
    bit(0, this->registers.getC());
}

// BIT 0, D
void CPU::cb_opcode_42() {
    bit(0, this->registers.getD());
}

// BIT 0, E
void CPU::cb_opcode_43() {
    bit(0, this->registers.getE());
}

// BIT 0, H
void CPU::cb_opcode_44() {
    bit(0, this->registers.getH());
}

// BIT 0, L
void CPU::cb_opcode_45() {
    bit(0, this->registers.getL());
}

// BIT 0, (HL)
void CPU::cb_opcode_46() {
    bit(0, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 0, A
void CPU::cb_opcode_47() {
    bit(0, this->registers.getA());
}

// BIT 1, B
void CPU::cb_opcode_48() {
    bit(1, this->registers.getB());
}

// BIT 1, C
void CPU::cb_opcode_49() {
    bit(1, this->registers.getC());
}

// BIT 1, D
void CPU::cb_opcode_4A() {
    bit(1, this->registers.getD());
}

// BIT 1, E
void CPU::cb_opcode_4B() {
    bit(1, this->registers.getE());
}

// BIT 1, H
void CPU::cb_opcode_4C() {
    bit(1, this->registers.getH());
}

// BIT 1, L
void CPU::cb_opcode_4D() {
    bit(1, this->registers.getL());
}

// BIT 1, (HL)
void CPU::cb_opcode_4E() {
    bit(1, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 1, A
void CPU::cb_opcode_4F() {
    bit(1, this->registers.getA());
}

// BIT 2, B
void CPU::cb_opcode_50() {
    bit(2, this->registers.getB());
}

// BIT 2, C
void CPU::cb_opcode_51() {
    bit(2, this->registers.getC());
}

// BIT 2, D
void CPU::cb_opcode_52() {
    bit(2, this->registers.getD());
}

// BIT 2, E
void CPU::cb_opcode_53() {
    bit(2, this->registers.getE());
}

// BIT 2, H
void CPU::cb_opcode_54() {
    bit(2, this->registers.getH());
}

// BIT 2, L
void CPU::cb_opcode_55() {
    bit(2, this->registers.getL());
}

// BIT 2, (HL)
void CPU::cb_opcode_56() {
    bit(2, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 2, A
void CPU::cb_opcode_57() {
    bit(2, this->registers.getA());
}

// BIT 3, B
void CPU::cb_opcode_58() {
    bit(3, this->registers.getB());
}

// BIT 3, C
void CPU::cb_opcode_59() {
    bit(3, this->registers.getC());
}

// BIT 3, D
void CPU::cb_opcode_5A() {
    bit(3, this->registers.getD());
}

// BIT 3, E
void CPU::cb_opcode_5B() {
    bit(3, this->registers.getE());
}

// BIT 3, H
void CPU::cb_opcode_5C() {
    bit(3, this->registers.getH());
}

// BIT 3, L
void CPU::cb_opcode_5D() {
    bit(3, this->registers.getL());
}

// BIT 3, (HL)
void CPU::cb_opcode_5E() {
    bit(3, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 3, A
void CPU::cb_opcode_5F() {
    bit(3, this->registers.getA());
}

// BIT 4, B
void CPU::cb_opcode_60() {
    bit(4, this->registers.getB());
}

// BIT 4, C
void CPU::cb_opcode_61() {
    bit(4, this->registers.getC());
}

// BIT 4, D
void CPU::cb_opcode_62() {
    bit(4, this->registers.getD());
}

// BIT 4, E
void CPU::cb_opcode_63() {
    bit(4, this->registers.getE());
}

// BIT 4, H
void CPU::cb_opcode_64() {
    bit(4, this->registers.getH());
}

// BIT 4, L
void CPU::cb_opcode_65() {
    bit(4, this->registers.getL());
}

// BIT 4, (HL)
void CPU::cb_opcode_66() {
    bit(4, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 4, A
void CPU::cb_opcode_67() {
    bit(4, this->registers.getA());
}

// BIT 5, B
void CPU::cb_opcode_68() {
    bit(5, this->registers.getB());
}

// BIT 5, C
void CPU::cb_opcode_69() {
    bit(5, this->registers.getC());
}

// BIT 5, D
void CPU::cb_opcode_6A() {
    bit(5, this->registers.getD());
}

// BIT 5, E
void CPU::cb_opcode_6B() {
    bit(5, this->registers.getE());
}

// BIT 5, H
void CPU::cb_opcode_6C() {
    bit(5, this->registers.getH());
}

// BIT 5, L
void CPU::cb_opcode_6D() {
    bit(5, this->registers.getL());
}

// BIT 5, (HL)
void CPU::cb_opcode_6E() {
    bit(5, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 5, A
void CPU::cb_opcode_6F() {
    bit(5, this->registers.getA());
}

// BIT 6, B
void CPU::cb_opcode_70() {
    bit(6, this->registers.getB());
}

// BIT 6, C
void CPU::cb_opcode_71() {
    bit(6, this->registers.getC());
}

// BIT 6, D
void CPU::cb_opcode_72() {
    bit(6, this->registers.getD());
}

// BIT 6, E
void CPU::cb_opcode_73() {
    bit(6, this->registers.getE());
}

// BIT 6, H
void CPU::cb_opcode_74() {
    bit(6, this->registers.getH());
}

// BIT 6, L
void CPU::cb_opcode_75() {
    bit(6, this->registers.getL());
}

// BIT 6, (HL)
void CPU::cb_opcode_76() {
    bit(6, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 6, A
void CPU::cb_opcode_77() {
    bit(6, this->registers.getA());
}

// BIT 7, B
void CPU::cb_opcode_78() {
    bit(7, this->registers.getB());
}

// BIT 7, C
void CPU::cb_opcode_79() {
    bit(7, this->registers.getC());
}

// BIT 7, D
void CPU::cb_opcode_7A() {
    bit(7, this->registers.getD());
}

// BIT 7, E
void CPU::cb_opcode_7B() {
    bit(7, this->registers.getE());
}

// BIT 7, H
void CPU::cb_opcode_7C() {
    bit(7, this->registers.getH());
}

// BIT 7, L
void CPU::cb_opcode_7D() {
    bit(7, this->registers.getL());
}

// BIT 7, (HL)
void CPU::cb_opcode_7E() {
    bit(7, this->memory.ReadByte(this->registers.getHL()));
}

// BIT 7, A
void CPU::cb_opcode_7F() {
    bit(7, this->registers.getA());
}

// RES 0, B		clear (reset) bit 0 of B.
void CPU::cb_opcode_80() {
    this->registers.setB(res(0, this->registers.getB()));
}

// RES 0, C
void CPU::cb_opcode_81() {
    this->registers.setC(res(0, this->registers.getC()));
}

// RES 0, D
void CPU::cb_opcode_82() {
    this->registers.setD(res(0, this->registers.getD()));
}

// RES 0, E
void CPU::cb_opcode_83() {
    this->registers.setE(res(0, this->registers.getE()));
}

// RES 0, H
void CPU::cb_opcode_84() {
    this->registers.setH(res(0, this->registers.getH()));
}

// RES 0, L
void CPU::cb_opcode_85() {
    this->registers.setL(res(0, this->registers.getL()));
}

// RES 0, (HL)
void CPU::cb_opcode_86() {
    this->memory.WriteByte(this->registers.getHL(), res(0, this->memory.ReadByte(this->registers.getHL())));
}

// RES 0, A
void CPU::cb_opcode_87() {
    this->registers.setA(res(0, this->registers.getA()));
}

// RES 1, B
void CPU::cb_opcode_88() {
    this->registers.setB(res(1, this->registers.getB()));
}

// RES 1, C
void CPU::cb_opcode_89() {
    this->registers.setC(res(1, this->registers.getC()));
}

// RES 1, D
void CPU::cb_opcode_8A() {
    this->registers.setD(res(1, this->registers.getD()));
}

// RES 1, E
void CPU::cb_opcode_8B() {
    this->registers.setE(res(1, this->registers.getE()));
}

// RES 1, H
void CPU::cb_opcode_8C() {
    this->registers.setH(res(1, this->registers.getH()));
}

// RES 1, L
void CPU::cb_opcode_8D() {
    this->registers.setL(res(1, this->registers.getL()));
}

// RES 1, (HL)
void CPU::cb_opcode_8E() {
    this->memory.WriteByte(this->registers.getHL(), res(1, this->memory.ReadByte(this->registers.getHL())));
}

// RES 1, A
void CPU::cb_opcode_8F() {
    this->registers.setA(res(1, this->registers.getA()));
}

// RES 2, B
void CPU::cb_opcode_90() {
    this->registers.setB(res(2, this->registers.getB()));
}

// RES 2, C
void CPU::cb_opcode_91() {
    this->registers.setC(res(2, this->registers.getC()));
}

// RES 2, D
void CPU::cb_opcode_92() {
    this->registers.setD(res(2, this->registers.getD()));
}

// RES 2, E
void CPU::cb_opcode_93() {
    this->registers.setE(res(2, this->registers.getE()));
}

// RES 2, H
void CPU::cb_opcode_94() {
    this->registers.setH(res(2, this->registers.getH()));
}

// RES 2, L
void CPU::cb_opcode_95() {
    this->registers.setL(res(2, this->registers.getL()));
}

// RES 2, (HL)
void CPU::cb_opcode_96() {
    this->memory.WriteByte(this->registers.getHL(), res(2, this->memory.ReadByte(this->registers.getHL())));
}

// RES 2, A
void CPU::cb_opcode_97() {
    this->registers.setA(res(2, this->registers.getA()));
}

// RES 3, B
void CPU::cb_opcode_98() {
    this->registers.setB(res(3, this->registers.getB()));
}

// RES 3, C
void CPU::cb_opcode_99() {
    this->registers.setC(res(3, this->registers.getC()));
}

// RES 3, D
void CPU::cb_opcode_9A() {
    this->registers.setD(res(3, this->registers.getD()));
}

// RES 3, E
void CPU::cb_opcode_9B() {
    this->registers.setE(res(3, this->registers.getE()));
}

// RES 3, H
void CPU::cb_opcode_9C() {
    this->registers.setH(res(3, this->registers.getH()));
}

// RES 3, L
void CPU::cb_opcode_9D() {
    this->registers.setL(res(3, this->registers.getL()));
}

// RES 3, (HL)
void CPU::cb_opcode_9E() {
    this->memory.WriteByte(this->registers.getHL(), res(3, this->memory.ReadByte(this->registers.getHL())));
}

// RES 3, A
void CPU::cb_opcode_9F() {
    this->registers.setA(res(3, this->registers.getA()));
}

// RES 4, B
void CPU::cb_opcode_A0() {
    this->registers.setB(res(4, this->registers.getB()));
}

// RES 4, C
void CPU::cb_opcode_A1() {
    this->registers.setC(res(4, this->registers.getC()));
}

// RES 4, D
void CPU::cb_opcode_A2() {
    this->registers.setD(res(4, this->registers.getD()));
}

// RES 4, E
void CPU::cb_opcode_A3() {
    this->registers.setE(res(4, this->registers.getE()));
}

// RES 4, H
void CPU::cb_opcode_A4() {
    this->registers.setH(res(4, this->registers.getH()));
}

// RES 4, L
void CPU::cb_opcode_A5() {
    this->registers.setL(res(4, this->registers.getL()));
}

// RES 4, (HL)
void CPU::cb_opcode_A6() {
    this->memory.WriteByte(this->registers.getHL(), res(4, this->memory.ReadByte(this->registers.getHL())));
}

// RES 4, A
void CPU::cb_opcode_A7() {
    this->registers.setA(res(4, this->registers.getA()));
}

// RES 5, B
void CPU::cb_opcode_A8() {
    this->registers.setB(res(5, this->registers.getB()));
}

// RES 5, C
void CPU::cb_opcode_A9() {
    this->registers.setC(res(5, this->registers.getC()));
}

// RES 5, D
void CPU::cb_opcode_AA() {
    this->registers.setD(res(5, this->registers.getD()));
}

// RES 5, E
void CPU::cb_opcode_AB() {
    this->registers.setE(res(5, this->registers.getE()));
}

// RES 5, H
void CPU::cb_opcode_AC() {
    this->registers.setH(res(5, this->registers.getH()));
}

// RES 5, L
void CPU::cb_opcode_AD() {
    this->registers.setL(res(5, this->registers.getL()));
}

// RES 5, (HL)
void CPU::cb_opcode_AE() {
    this->memory.WriteByte(this->registers.getHL(), res(5, this->memory.ReadByte(this->registers.getHL())));
}

// RES 5, A
void CPU::cb_opcode_AF() {
    this->registers.setA(res(5, this->registers.getA()));
}

// RES 6, B
void CPU::cb_opcode_B0() {
    this->registers.setB(res(6, this->registers.getB()));
}

// RES 6, C
void CPU::cb_opcode_B1() {
    this->registers.setC(res(6, this->registers.getC()));
}

// RES 6, D
void CPU::cb_opcode_B2() {
    this->registers.setD(res(6, this->registers.getD()));
}

// RES 6, E
void CPU::cb_opcode_B3() {
    this->registers.setE(res(6, this->registers.getE()));
}

// RES 6, H
void CPU::cb_opcode_B4() {
    this->registers.setH(res(6, this->registers.getH()));
}

// RES 6, L
void CPU::cb_opcode_B5() {
    this->registers.setL(res(6, this->registers.getL()));
}

// RES 6, (HL)
void CPU::cb_opcode_B6() {
    this->memory.WriteByte(this->registers.getHL(), res(6, this->memory.ReadByte(this->registers.getHL())));
}

// RES 6, A
void CPU::cb_opcode_B7() {
    this->registers.setA(res(6, this->registers.getA()));
}

// RES 7, B
void CPU::cb_opcode_B8() {
    this->registers.setB(res(7, this->registers.getB()));
}

// RES 7, C
void CPU::cb_opcode_B9() {
    this->registers.setC(res(7, this->registers.getC()));
}

// RES 7, D
void CPU::cb_opcode_BA() {
    this->registers.setD(res(7, this->registers.getD()));
}

// RES 7, E
void CPU::cb_opcode_BB() {
    this->registers.setE(res(7, this->registers.getE()));
}

// RES 7, H
void CPU::cb_opcode_BC() {
    this->registers.setH(res(7, this->registers.getH()));
}

// RES 7, L
void CPU::cb_opcode_BD() {
    this->registers.setL(res(7, this->registers.getL()));
}

// RES 7, (HL)
void CPU::cb_opcode_BE() {
    this->memory.WriteByte(this->registers.getHL(), res(7, this->memory.ReadByte(this->registers.getHL())));
}

// RES 7, A
void CPU::cb_opcode_BF() {
    this->registers.setA(res(7, this->registers.getA()));
}

// SET 0, B
void CPU::cb_opcode_C0() {
    this->registers.setB(set(0, this->registers.getB()));
}

// SET 0, C
void CPU::cb_opcode_C1() {
    this->registers.setC(set(0, this->registers.getC()));
}

// SET 0, D
void CPU::cb_opcode_C2() {
    this->registers.setD(set(0, this->registers.getD()));
}

// SET 0, E
void CPU::cb_opcode_C3() {
    this->registers.setE(set(0, this->registers.getE()));
}

// SET 0, H
void CPU::cb_opcode_C4() {
    this->registers.setH(set(0, this->registers.getH()));
}

// SET 0, L
void CPU::cb_opcode_C5() {
    this->registers.setL(set(0, this->registers.getL()));
}

// SET 0, (HL)
void CPU::cb_opcode_C6() {
    this->memory.WriteByte(this->registers.getHL(), set(0, this->memory.ReadByte(this->registers.getHL())));
}

// SET 0, A
void CPU::cb_opcode_C7() {
    this->registers.setA(set(0, this->registers.getA()));
}

// SET 1, B
void CPU::cb_opcode_C8() {
    this->registers.setB(set(1, this->registers.getB()));
}

// SET 1, C
void CPU::cb_opcode_C9() {
    this->registers.setC(set(1, this->registers.getC()));
}

// SET 1, D
void CPU::cb_opcode_CA() {
    this->registers.setD(set(1, this->registers.getD()));
}

// SET 1, E
void CPU::cb_opcode_CB() {
    this->registers.setE(set(1, this->registers.getE()));
}

// SET 1, H
void CPU::cb_opcode_CC() {
    this->registers.setH(set(1, this->registers.getH()));
}

// SET 1, L
void CPU::cb_opcode_CD() {
    this->registers.setL(set(1, this->registers.getL()));
}

// SET 1, (HL)
void CPU::cb_opcode_CE() {
    this->memory.WriteByte(this->registers.getHL(), set(1, this->memory.ReadByte(this->registers.getHL())));
}

// SET 1, A
void CPU::cb_opcode_CF() {
    this->registers.setA(set(1, this->registers.getA()));
}

// SET 2, B
void CPU::cb_opcode_D0() {
    this->registers.setB(set(2, this->registers.getB()));
}

// SET 2, C
void CPU::cb_opcode_D1() {
    this->registers.setC(set(2, this->registers.getC()));
}

// SET 2, D
void CPU::cb_opcode_D2() {
    this->registers.setD(set(2, this->registers.getD()));
}

// SET 2, E
void CPU::cb_opcode_D3() {
    this->registers.setE(set(2, this->registers.getE()));
}

// SET 2, H
void CPU::cb_opcode_D4() {
    this->registers.setH(set(2, this->registers.getH()));
}

// SET 2, L
void CPU::cb_opcode_D5() {
    this->registers.setL(set(2, this->registers.getL()));
}

// SET 2, (HL)
void CPU::cb_opcode_D6() {
    this->memory.WriteByte(this->registers.getHL(), set(2, this->memory.ReadByte(this->registers.getHL())));
}

// SET 2, A
void CPU::cb_opcode_D7() {
    this->registers.setA(set(2, this->registers.getA()));
}

// SET 3, B
void CPU::cb_opcode_D8() {
    this->registers.setB(set(3, this->registers.getB()));
}

// SET 3, C
void CPU::cb_opcode_D9() {
    this->registers.setC(set(3, this->registers.getC()));
}

// SET 3, D
void CPU::cb_opcode_DA() {
    this->registers.setD(set(3, this->registers.getD()));
}

// SET 3, E
void CPU::cb_opcode_DB() {
    this->registers.setE(set(3, this->registers.getE()));
}

// SET 3, H
void CPU::cb_opcode_DC() {
    this->registers.setH(set(3, this->registers.getH()));
}

// SET 3, L
void CPU::cb_opcode_DD() {
    this->registers.setL(set(3, this->registers.getL()));
}

// SET 3, (HL)
void CPU::cb_opcode_DE() {
    this->memory.WriteByte(this->registers.getHL(), set(3, this->memory.ReadByte(this->registers.getHL())));
}

// SET 3, A
void CPU::cb_opcode_DF() {
    this->registers.setA(set(3, this->registers.getA()));
}

// SET 4, B
void CPU::cb_opcode_E0() {
    this->registers.setB(set(4, this->registers.getB()));
}

// SET 4, C
void CPU::cb_opcode_E1() {
    this->registers.setC(set(4, this->registers.getC()));
}

// SET 4, D
void CPU::cb_opcode_E2() {
    this->registers.setD(set(4, this->registers.getD()));
}

// SET 4, E
void CPU::cb_opcode_E3() {
    this->registers.setE(set(4, this->registers.getE()));
}

// SET 4, H
void CPU::cb_opcode_E4() {
    this->registers.setH(set(4, this->registers.getH()));
}

// SET 4, L
void CPU::cb_opcode_E5() {
    this->registers.setL(set(4, this->registers.getL()));
}

// SET 4, (HL)
void CPU::cb_opcode_E6() {
    this->memory.WriteByte(this->registers.getHL(), set(4, this->memory.ReadByte(this->registers.getHL())));
}

// SET 4, A
void CPU::cb_opcode_E7() {
    this->registers.setA(set(4, this->registers.getA()));
}

// SET 5, B
void CPU::cb_opcode_E8() {
    this->registers.setB(set(5, this->registers.getB()));
}

// SET 5, C
void CPU::cb_opcode_E9() {
    this->registers.setC(set(5, this->registers.getC()));
}

// SET 5, D
void CPU::cb_opcode_EA() {
    this->registers.setD(set(5, this->registers.getD()));
}

// SET 5, E
void CPU::cb_opcode_EB() {
    this->registers.setE(set(5, this->registers.getE()));
}

// SET 5, H
void CPU::cb_opcode_EC() {
    this->registers.setH(set(5, this->registers.getH()));
}

// SET 5, L
void CPU::cb_opcode_ED() {
    this->registers.setL(set(5, this->registers.getL()));
}

// SET 5, (HL)
void CPU::cb_opcode_EE() {
    this->memory.WriteByte(this->registers.getHL(), set(5, this->memory.ReadByte(this->registers.getHL())));
}

// SET 5, A
void CPU::cb_opcode_EF() {
    this->registers.setA(set(5, this->registers.getA()));
}

// SET 6, B
void CPU::cb_opcode_F0() {
    this->registers.setB(set(6, this->registers.getB()));
}

// SET 6, C
void CPU::cb_opcode_F1() {
    this->registers.setC(set(6, this->registers.getC()));
}

// SET 6, D
void CPU::cb_opcode_F2() {
    this->registers.setD(set(6, this->registers.getD()));
}

// SET 6, E
void CPU::cb_opcode_F3() {
    this->registers.setE(set(6, this->registers.getE()));
}

// SET 6, H
void CPU::cb_opcode_F4() {
    this->registers.setH(set(6, this->registers.getH()));
}

// SET 6, L
void CPU::cb_opcode_F5() {
    this->registers.setL(set(6, this->registers.getL()));
}

// SET 6, (HL)
void CPU::cb_opcode_F6() {
    this->memory.WriteByte(this->registers.getHL(), set(6, this->memory.ReadByte(this->registers.getHL())));
}

// SET 6, A
void CPU::cb_opcode_F7() {
    this->registers.setA(set(6, this->registers.getA()));
}

// SET 7, B
void CPU::cb_opcode_F8() {
    this->registers.setB(set(7, this->registers.getB()));
}

// SET 7, C
void CPU::cb_opcode_F9() {
    this->registers.setC(set(7, this->registers.getC()));
}

// SET 7, D
void CPU::cb_opcode_FA() {
    this->registers.setD(set(7, this->registers.getD()));
}

// SET 7, E
void CPU::cb_opcode_FB() {
    this->registers.setE(set(7, this->registers.getE()));
}

// SET 7, H
void CPU::cb_opcode_FC() {
    this->registers.setH(set(7, this->registers.getH()));
}

// SET 7, L
void CPU::cb_opcode_FD() {
    this->registers.setL(set(7, this->registers.getL()));
}

// SET 7, (HL)
void CPU::cb_opcode_FE() {
    this->memory.WriteByte(this->registers.getHL(), set(7, this->memory.ReadByte(this->registers.getHL())));
}

// SET 7, A
void CPU::cb_opcode_FF() {
    this->registers.setA(set(7, this->registers.getA()));
}