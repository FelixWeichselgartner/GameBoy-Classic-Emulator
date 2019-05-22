#include "../include/CPU.hpp"

void CPU::Init_CBOPCODE() {
    cb_opcode[0x00] = cb_opcode_00;
    cb_opcode[0x01] = cb_opcode_01;
    cb_opcode[0x02] = cb_opcode_02;
    cb_opcode[0x03] = cb_opcode_03;
    cb_opcode[0x04] = cb_opcode_04;
    cb_opcode[0x05] = cb_opcode_05;
    cb_opcode[0x06] = cb_opcode_06;
    cb_opcode[0x07] = cb_opcode_07;
    cb_opcode[0x08] = cb_opcode_08;
    cb_opcode[0x09] = cb_opcode_09;
    cb_opcode[0x0A] = cb_opcode_0A;
    cb_opcode[0x0B] = cb_opcode_0B;
    cb_opcode[0x0C] = cb_opcode_0C;
    cb_opcode[0x0D] = cb_opcode_0D;
    cb_opcode[0x0E] = cb_opcode_0E;
    cb_opcode[0x0F] = cb_opcode_0F;
    cb_opcode[0x10] = cb_opcode_10;
    cb_opcode[0x11] = cb_opcode_11;
    cb_opcode[0x12] = cb_opcode_12;
    cb_opcode[0x13] = cb_opcode_13;
    cb_opcode[0x14] = cb_opcode_14;
    cb_opcode[0x15] = cb_opcode_15;
    cb_opcode[0x16] = cb_opcode_16;
    cb_opcode[0x17] = cb_opcode_17;
    cb_opcode[0x18] = cb_opcode_18;
    cb_opcode[0x19] = cb_opcode_19;
    cb_opcode[0x1A] = cb_opcode_1A;
    cb_opcode[0x1B] = cb_opcode_1B;
    cb_opcode[0x1C] = cb_opcode_1C;
    cb_opcode[0x1D] = cb_opcode_1D;
    cb_opcode[0x1E] = cb_opcode_1E;
    cb_opcode[0x1F] = cb_opcode_1F;
    cb_opcode[0x20] = cb_opcode_20;
    cb_opcode[0x21] = cb_opcode_21;
    cb_opcode[0x22] = cb_opcode_22;
    cb_opcode[0x23] = cb_opcode_23;
    cb_opcode[0x24] = cb_opcode_24;
    cb_opcode[0x25] = cb_opcode_25;
    cb_opcode[0x26] = cb_opcode_26;
    cb_opcode[0x27] = cb_opcode_27;
    cb_opcode[0x28] = cb_opcode_28;
    cb_opcode[0x29] = cb_opcode_29;
    cb_opcode[0x2A] = cb_opcode_2A;
    cb_opcode[0x2B] = cb_opcode_2B;
    cb_opcode[0x2C] = cb_opcode_2C;
    cb_opcode[0x2D] = cb_opcode_2D;
    cb_opcode[0x2E] = cb_opcode_2E;
    cb_opcode[0x2F] = cb_opcode_2F;
    cb_opcode[0x30] = cb_opcode_30;
    cb_opcode[0x31] = cb_opcode_31;
    cb_opcode[0x32] = cb_opcode_32;
    cb_opcode[0x33] = cb_opcode_33;
    cb_opcode[0x34] = cb_opcode_34;
    cb_opcode[0x35] = cb_opcode_35;
    cb_opcode[0x36] = cb_opcode_36;
    cb_opcode[0x37] = cb_opcode_37;
    cb_opcode[0x38] = cb_opcode_38;
    cb_opcode[0x39] = cb_opcode_39;
    cb_opcode[0x3A] = cb_opcode_3A;
    cb_opcode[0x3B] = cb_opcode_3B;
    cb_opcode[0x3C] = cb_opcode_3C;
    cb_opcode[0x3D] = cb_opcode_3D;
    cb_opcode[0x3E] = cb_opcode_3E;
    cb_opcode[0x3F] = cb_opcode_3F;
    cb_opcode[0x40] = cb_opcode_40;
    cb_opcode[0x41] = cb_opcode_41;
    cb_opcode[0x42] = cb_opcode_42;
    cb_opcode[0x43] = cb_opcode_43;
    cb_opcode[0x44] = cb_opcode_44;
    cb_opcode[0x45] = cb_opcode_45;
    cb_opcode[0x46] = cb_opcode_46;
    cb_opcode[0x47] = cb_opcode_47;
    cb_opcode[0x48] = cb_opcode_48;
    cb_opcode[0x49] = cb_opcode_49;
    cb_opcode[0x4A] = cb_opcode_4A;
    cb_opcode[0x4B] = cb_opcode_4B;
    cb_opcode[0x4C] = cb_opcode_4C;
    cb_opcode[0x4D] = cb_opcode_4D;
    cb_opcode[0x4E] = cb_opcode_4E;
    cb_opcode[0x4F] = cb_opcode_4F;
    cb_opcode[0x50] = cb_opcode_50;
    cb_opcode[0x51] = cb_opcode_51;
    cb_opcode[0x52] = cb_opcode_52;
    cb_opcode[0x53] = cb_opcode_53;
    cb_opcode[0x54] = cb_opcode_54;
    cb_opcode[0x55] = cb_opcode_55;
    cb_opcode[0x56] = cb_opcode_56;
    cb_opcode[0x57] = cb_opcode_57;
    cb_opcode[0x58] = cb_opcode_58;
    cb_opcode[0x59] = cb_opcode_59;
    cb_opcode[0x5A] = cb_opcode_5A;
    cb_opcode[0x5B] = cb_opcode_5B;
    cb_opcode[0x5C] = cb_opcode_5C;
    cb_opcode[0x5D] = cb_opcode_5D;
    cb_opcode[0x5E] = cb_opcode_5E;
    cb_opcode[0x5F] = cb_opcode_5F;
    cb_opcode[0x60] = cb_opcode_60;
    cb_opcode[0x61] = cb_opcode_61;
    cb_opcode[0x62] = cb_opcode_62;
    cb_opcode[0x63] = cb_opcode_63;
    cb_opcode[0x64] = cb_opcode_64;
    cb_opcode[0x65] = cb_opcode_65;
    cb_opcode[0x66] = cb_opcode_66;
    cb_opcode[0x67] = cb_opcode_67;
    cb_opcode[0x68] = cb_opcode_68;
    cb_opcode[0x69] = cb_opcode_69;
    cb_opcode[0x6A] = cb_opcode_6A;
    cb_opcode[0x6B] = cb_opcode_6B;
    cb_opcode[0x6C] = cb_opcode_6C;
    cb_opcode[0x6D] = cb_opcode_6D;
    cb_opcode[0x6E] = cb_opcode_6E;
    cb_opcode[0x6F] = cb_opcode_6F;
    cb_opcode[0x70] = cb_opcode_70;
    cb_opcode[0x71] = cb_opcode_71;
    cb_opcode[0x72] = cb_opcode_72;
    cb_opcode[0x73] = cb_opcode_73;
    cb_opcode[0x74] = cb_opcode_74;
    cb_opcode[0x75] = cb_opcode_75;
    cb_opcode[0x76] = cb_opcode_76;
    cb_opcode[0x77] = cb_opcode_77;
    cb_opcode[0x78] = cb_opcode_78;
    cb_opcode[0x79] = cb_opcode_79;
    cb_opcode[0x7A] = cb_opcode_7A;
    cb_opcode[0x7B] = cb_opcode_7B;
    cb_opcode[0x7C] = cb_opcode_7C;
    cb_opcode[0x7D] = cb_opcode_7D;
    cb_opcode[0x7E] = cb_opcode_7E;
    cb_opcode[0x7F] = cb_opcode_7F;
    cb_opcode[0x80] = cb_opcode_80;
    cb_opcode[0x81] = cb_opcode_81;
    cb_opcode[0x82] = cb_opcode_82;
    cb_opcode[0x83] = cb_opcode_83;
    cb_opcode[0x84] = cb_opcode_84;
    cb_opcode[0x85] = cb_opcode_85;
    cb_opcode[0x86] = cb_opcode_86;
    cb_opcode[0x87] = cb_opcode_87;
    cb_opcode[0x88] = cb_opcode_88;
    cb_opcode[0x89] = cb_opcode_89;
    cb_opcode[0x8A] = cb_opcode_8A;
    cb_opcode[0x8B] = cb_opcode_8B;
    cb_opcode[0x8C] = cb_opcode_8C;
    cb_opcode[0x8D] = cb_opcode_8D;
    cb_opcode[0x8E] = cb_opcode_8E;
    cb_opcode[0x8F] = cb_opcode_8F;
    cb_opcode[0x90] = cb_opcode_90;
    cb_opcode[0x91] = cb_opcode_91;
    cb_opcode[0x92] = cb_opcode_92;
    cb_opcode[0x93] = cb_opcode_93;
    cb_opcode[0x94] = cb_opcode_94;
    cb_opcode[0x95] = cb_opcode_95;
    cb_opcode[0x96] = cb_opcode_96;
    cb_opcode[0x97] = cb_opcode_97;
    cb_opcode[0x98] = cb_opcode_98;
    cb_opcode[0x99] = cb_opcode_99;
    cb_opcode[0x9A] = cb_opcode_9A;
    cb_opcode[0x9B] = cb_opcode_9B;
    cb_opcode[0x9C] = cb_opcode_9C;
    cb_opcode[0x9D] = cb_opcode_9D;
    cb_opcode[0x9E] = cb_opcode_9E;
    cb_opcode[0x9F] = cb_opcode_9F;
    cb_opcode[0xA0] = cb_opcode_A0;
    cb_opcode[0xA1] = cb_opcode_A1;
    cb_opcode[0xA2] = cb_opcode_A2;
    cb_opcode[0xA3] = cb_opcode_A3;
    cb_opcode[0xA4] = cb_opcode_A4;
    cb_opcode[0xA5] = cb_opcode_A5;
    cb_opcode[0xA6] = cb_opcode_A6;
    cb_opcode[0xA7] = cb_opcode_A7;
    cb_opcode[0xA8] = cb_opcode_A8;
    cb_opcode[0xA9] = cb_opcode_A9;
    cb_opcode[0xAA] = cb_opcode_AA;
    cb_opcode[0xAB] = cb_opcode_AB;
    cb_opcode[0xAC] = cb_opcode_AC;
    cb_opcode[0xAD] = cb_opcode_AD;
    cb_opcode[0xAE] = cb_opcode_AE;
    cb_opcode[0xAF] = cb_opcode_AF;
    cb_opcode[0xB0] = cb_opcode_B0;
    cb_opcode[0xB1] = cb_opcode_B1;
    cb_opcode[0xB2] = cb_opcode_B2;
    cb_opcode[0xB3] = cb_opcode_B3;
    cb_opcode[0xB4] = cb_opcode_B4;
    cb_opcode[0xB5] = cb_opcode_B5;
    cb_opcode[0xB6] = cb_opcode_B6;
    cb_opcode[0xB7] = cb_opcode_B7;
    cb_opcode[0xB8] = cb_opcode_B8;
    cb_opcode[0xB9] = cb_opcode_B9;
    cb_opcode[0xBA] = cb_opcode_BA;
    cb_opcode[0xBB] = cb_opcode_BB;
    cb_opcode[0xBC] = cb_opcode_BC;
    cb_opcode[0xBD] = cb_opcode_BD;
    cb_opcode[0xBE] = cb_opcode_BE;
    cb_opcode[0xBF] = cb_opcode_BF;
    cb_opcode[0xC0] = cb_opcode_C0;
    cb_opcode[0xC1] = cb_opcode_C1;
    cb_opcode[0xC2] = cb_opcode_C2;
    cb_opcode[0xC3] = cb_opcode_C3;
    cb_opcode[0xC4] = cb_opcode_C4;
    cb_opcode[0xC5] = cb_opcode_C5;
    cb_opcode[0xC6] = cb_opcode_C6;
    cb_opcode[0xC7] = cb_opcode_C7;
    cb_opcode[0xC8] = cb_opcode_C8;
    cb_opcode[0xC9] = cb_opcode_C9;
    cb_opcode[0xCA] = cb_opcode_CA;
    cb_opcode[0xCB] = cb_opcode_CB;
    cb_opcode[0xCC] = cb_opcode_CC;
    cb_opcode[0xCD] = cb_opcode_CD;
    cb_opcode[0xCE] = cb_opcode_CE;
    cb_opcode[0xCF] = cb_opcode_CF;
    cb_opcode[0xD0] = cb_opcode_D0;
    cb_opcode[0xD1] = cb_opcode_D1;
    cb_opcode[0xD2] = cb_opcode_D2;
    cb_opcode[0xD3] = cb_opcode_D3;
    cb_opcode[0xD4] = cb_opcode_D4;
    cb_opcode[0xD5] = cb_opcode_D5;
    cb_opcode[0xD6] = cb_opcode_D6;
    cb_opcode[0xD7] = cb_opcode_D7;
    cb_opcode[0xD8] = cb_opcode_D8;
    cb_opcode[0xD9] = cb_opcode_D9;
    cb_opcode[0xDA] = cb_opcode_DA;
    cb_opcode[0xDB] = cb_opcode_DB;
    cb_opcode[0xDC] = cb_opcode_DC;
    cb_opcode[0xDD] = cb_opcode_DD;
    cb_opcode[0xDE] = cb_opcode_DE;
    cb_opcode[0xDF] = cb_opcode_DF;
    cb_opcode[0xE0] = cb_opcode_E0;
    cb_opcode[0xE1] = cb_opcode_E1;
    cb_opcode[0xE2] = cb_opcode_E2;
    cb_opcode[0xE3] = cb_opcode_E3;
    cb_opcode[0xE4] = cb_opcode_E4;
    cb_opcode[0xE5] = cb_opcode_E5;
    cb_opcode[0xE6] = cb_opcode_E6;
    cb_opcode[0xE7] = cb_opcode_E7;
    cb_opcode[0xE8] = cb_opcode_E8;
    cb_opcode[0xE9] = cb_opcode_E9;
    cb_opcode[0xEA] = cb_opcode_EA;
    cb_opcode[0xEB] = cb_opcode_EB;
    cb_opcode[0xEC] = cb_opcode_EC;
    cb_opcode[0xED] = cb_opcode_ED;
    cb_opcode[0xEE] = cb_opcode_EE;
    cb_opcode[0xEF] = cb_opcode_EF;
    cb_opcode[0xF0] = cb_opcode_F0;
    cb_opcode[0xF1] = cb_opcode_F1;
    cb_opcode[0xF2] = cb_opcode_F2;
    cb_opcode[0xF3] = cb_opcode_F3;
    cb_opcode[0xF4] = cb_opcode_F4;
    cb_opcode[0xF5] = cb_opcode_F5;
    cb_opcode[0xF6] = cb_opcode_F6;
    cb_opcode[0xF7] = cb_opcode_F7;
    cb_opcode[0xF8] = cb_opcode_F8;
    cb_opcode[0xF9] = cb_opcode_F9;
    cb_opcode[0xFA] = cb_opcode_FA;
    cb_opcode[0xFB] = cb_opcode_FB;
    cb_opcode[0xFC] = cb_opcode_FC;
    cb_opcode[0xFD] = cb_opcode_FD;
    cb_opcode[0xFE] = cb_opcode_FE;
    cb_opcode[0xFF] = cb_opcode_FF;
}

void CPU::cb_opcode_00() {
	
}

void CPU::cb_opcode_01() {
	
}

void CPU::cb_opcode_02() {
	
}

void CPU::cb_opcode_03() {
	
}

void CPU::cb_opcode_04() {
	
}

void CPU::cb_opcode_05() {
	
}

void CPU::cb_opcode_06() {
	
}

void CPU::cb_opcode_07() {
	
}

void CPU::cb_opcode_08() {
	
}

void CPU::cb_opcode_09() {
	
}

void CPU::cb_opcode_0A() {
	
}

void CPU::cb_opcode_0B() {
	
}

void CPU::cb_opcode_0C() {
	
}

void CPU::cb_opcode_0D() {
	
}

void CPU::cb_opcode_0E() {
	
}

void CPU::cb_opcode_0F() {
	
}

void CPU::cb_opcode_10() {
	
}

void CPU::cb_opcode_11() {
	
}

void CPU::cb_opcode_12() {
	
}

void CPU::cb_opcode_13() {
	
}

void CPU::cb_opcode_14() {
	
}

void CPU::cb_opcode_15() {
	
}

void CPU::cb_opcode_16() {
	
}

void CPU::cb_opcode_17() {
	
}

void CPU::cb_opcode_18() {
	
}

void CPU::cb_opcode_19() {
	
}

void CPU::cb_opcode_1A() {
	
}

void CPU::cb_opcode_1B() {
	
}

void CPU::cb_opcode_1C() {
	
}

void CPU::cb_opcode_1D() {
	
}

void CPU::cb_opcode_1E() {
	
}

void CPU::cb_opcode_1F() {
	
}

void CPU::cb_opcode_20() {
	
}

void CPU::cb_opcode_21() {
	
}

void CPU::cb_opcode_22() {
	
}

void CPU::cb_opcode_23() {
	
}

void CPU::cb_opcode_24() {
	
}

void CPU::cb_opcode_25() {
	
}

void CPU::cb_opcode_26() {
	
}

void CPU::cb_opcode_27() {
	
}

void CPU::cb_opcode_28() {
	
}

void CPU::cb_opcode_29() {
	
}

void CPU::cb_opcode_2A() {
	
}

void CPU::cb_opcode_2B() {
	
}

void CPU::cb_opcode_2C() {
	
}

void CPU::cb_opcode_2D() {
	
}

void CPU::cb_opcode_2E() {
	
}

void CPU::cb_opcode_2F() {
	
}

void CPU::cb_opcode_30() {
	
}

void CPU::cb_opcode_31() {
	
}

void CPU::cb_opcode_32() {
	
}

void CPU::cb_opcode_33() {
	
}

void CPU::cb_opcode_34() {
	
}

void CPU::cb_opcode_35() {
	
}

void CPU::cb_opcode_36() {
	
}

void CPU::cb_opcode_37() {
	
}

void CPU::cb_opcode_38() {
	
}

void CPU::cb_opcode_39() {
	
}

void CPU::cb_opcode_3A() {
	
}

void CPU::cb_opcode_3B() {
	
}

void CPU::cb_opcode_3C() {
	
}

void CPU::cb_opcode_3D() {
	
}

void CPU::cb_opcode_3E() {
	
}

void CPU::cb_opcode_3F() {
	
}

void CPU::cb_opcode_40() {
	
}

void CPU::cb_opcode_41() {
	
}

void CPU::cb_opcode_42() {
	
}

void CPU::cb_opcode_43() {
	
}

void CPU::cb_opcode_44() {
	
}

void CPU::cb_opcode_45() {
	
}

void CPU::cb_opcode_46() {
	
}

void CPU::cb_opcode_47() {
	
}

void CPU::cb_opcode_48() {
	
}

void CPU::cb_opcode_49() {
	
}

void CPU::cb_opcode_4A() {
	
}

void CPU::cb_opcode_4B() {
	
}

void CPU::cb_opcode_4C() {
	
}

void CPU::cb_opcode_4D() {
	
}

void CPU::cb_opcode_4E() {
	
}

void CPU::cb_opcode_4F() {
	
}

void CPU::cb_opcode_50() {
	
}

void CPU::cb_opcode_51() {
	
}

void CPU::cb_opcode_52() {
	
}

void CPU::cb_opcode_53() {
	
}

void CPU::cb_opcode_54() {
	
}

void CPU::cb_opcode_55() {
	
}

void CPU::cb_opcode_56() {
	
}

void CPU::cb_opcode_57() {
	
}

void CPU::cb_opcode_58() {
	
}

void CPU::cb_opcode_59() {
	
}

void CPU::cb_opcode_5A() {
	
}

void CPU::cb_opcode_5B() {
	
}

void CPU::cb_opcode_5C() {
	
}

void CPU::cb_opcode_5D() {
	
}

void CPU::cb_opcode_5E() {
	
}

void CPU::cb_opcode_5F() {
	
}

void CPU::cb_opcode_60() {
	
}

void CPU::cb_opcode_61() {
	
}

void CPU::cb_opcode_62() {
	
}

void CPU::cb_opcode_63() {
	
}

void CPU::cb_opcode_64() {
	
}

void CPU::cb_opcode_65() {
	
}

void CPU::cb_opcode_66() {
	
}

void CPU::cb_opcode_67() {
	
}

void CPU::cb_opcode_68() {
	
}

void CPU::cb_opcode_69() {
	
}

void CPU::cb_opcode_6A() {
	
}

void CPU::cb_opcode_6B() {
	
}

void CPU::cb_opcode_6C() {
	
}

void CPU::cb_opcode_6D() {
	
}

void CPU::cb_opcode_6E() {
	
}

void CPU::cb_opcode_6F() {
	
}

void CPU::cb_opcode_70() {
	
}

void CPU::cb_opcode_71() {
	
}

void CPU::cb_opcode_72() {
	
}

void CPU::cb_opcode_73() {
	
}

void CPU::cb_opcode_74() {
	
}

void CPU::cb_opcode_75() {
	
}

void CPU::cb_opcode_76() {
	
}

void CPU::cb_opcode_77() {
	
}

void CPU::cb_opcode_78() {
	
}

void CPU::cb_opcode_79() {
	
}

void CPU::cb_opcode_7A() {
	
}

void CPU::cb_opcode_7B() {
	
}

void CPU::cb_opcode_7C() {
	
}

void CPU::cb_opcode_7D() {
	
}

void CPU::cb_opcode_7E() {
	
}

void CPU::cb_opcode_7F() {
	
}

void CPU::cb_opcode_80() {
	
}

void CPU::cb_opcode_81() {
	
}

void CPU::cb_opcode_82() {
	
}

void CPU::cb_opcode_83() {
	
}

void CPU::cb_opcode_84() {
	
}

void CPU::cb_opcode_85() {
	
}

void CPU::cb_opcode_86() {
	
}

void CPU::cb_opcode_87() {
	
}

void CPU::cb_opcode_88() {
	
}

void CPU::cb_opcode_89() {
	
}

void CPU::cb_opcode_8A() {
	
}

void CPU::cb_opcode_8B() {
	
}

void CPU::cb_opcode_8C() {
	
}

void CPU::cb_opcode_8D() {
	
}

void CPU::cb_opcode_8E() {
	
}

void CPU::cb_opcode_8F() {
	
}

void CPU::cb_opcode_90() {
	
}

void CPU::cb_opcode_91() {
	
}

void CPU::cb_opcode_92() {
	
}

void CPU::cb_opcode_93() {
	
}

void CPU::cb_opcode_94() {
	
}

void CPU::cb_opcode_95() {
	
}

void CPU::cb_opcode_96() {
	
}

void CPU::cb_opcode_97() {
	
}

void CPU::cb_opcode_98() {
	
}

void CPU::cb_opcode_99() {
	
}

void CPU::cb_opcode_9A() {
	
}

void CPU::cb_opcode_9B() {
	
}

void CPU::cb_opcode_9C() {
	
}

void CPU::cb_opcode_9D() {
	
}

void CPU::cb_opcode_9E() {
	
}

void CPU::cb_opcode_9F() {
	
}

void CPU::cb_opcode_A0() {
	
}

void CPU::cb_opcode_A1() {
	
}

void CPU::cb_opcode_A2() {
	
}

void CPU::cb_opcode_A3() {
	
}

void CPU::cb_opcode_A4() {
	
}

void CPU::cb_opcode_A5() {
	
}

void CPU::cb_opcode_A6() {
	
}

void CPU::cb_opcode_A7() {
	
}

void CPU::cb_opcode_A8() {
	
}

void CPU::cb_opcode_A9() {
	
}

void CPU::cb_opcode_AA() {
	
}

void CPU::cb_opcode_AB() {
	
}

void CPU::cb_opcode_AC() {
	
}

void CPU::cb_opcode_AD() {
	
}

void CPU::cb_opcode_AE() {
	
}

void CPU::cb_opcode_AF() {
	
}

void CPU::cb_opcode_B0() {
	
}

void CPU::cb_opcode_B1() {
	
}

void CPU::cb_opcode_B2() {
	
}

void CPU::cb_opcode_B3() {
	
}

void CPU::cb_opcode_B4() {
	
}

void CPU::cb_opcode_B5() {
	
}

void CPU::cb_opcode_B6() {
	
}

void CPU::cb_opcode_B7() {
	
}

void CPU::cb_opcode_B8() {
	
}

void CPU::cb_opcode_B9() {
	
}

void CPU::cb_opcode_BA() {
	
}

void CPU::cb_opcode_BB() {
	
}

void CPU::cb_opcode_BC() {
	
}

void CPU::cb_opcode_BD() {
	
}

void CPU::cb_opcode_BE() {
	
}

void CPU::cb_opcode_BF() {
	
}

void CPU::cb_opcode_C0() {
	
}

void CPU::cb_opcode_C1() {
	
}

void CPU::cb_opcode_C2() {
	
}

void CPU::cb_opcode_C3() {
	
}

void CPU::cb_opcode_C4() {
	
}

void CPU::cb_opcode_C5() {
	
}

void CPU::cb_opcode_C6() {
	
}

void CPU::cb_opcode_C7() {
	
}

void CPU::cb_opcode_C8() {
	
}

void CPU::cb_opcode_C9() {
	
}

void CPU::cb_opcode_CA() {
	
}

void CPU::cb_opcode_CB() {
	
}

void CPU::cb_opcode_CC() {
	
}

void CPU::cb_opcode_CD() {
	
}

void CPU::cb_opcode_CE() {
	
}

void CPU::cb_opcode_CF() {
	
}

void CPU::cb_opcode_D0() {
	
}

void CPU::cb_opcode_D1() {
	
}

void CPU::cb_opcode_D2() {
	
}

void CPU::cb_opcode_D3() {
	
}

void CPU::cb_opcode_D4() {
	
}

void CPU::cb_opcode_D5() {
	
}

void CPU::cb_opcode_D6() {
	
}

void CPU::cb_opcode_D7() {
	
}

void CPU::cb_opcode_D8() {
	
}

void CPU::cb_opcode_D9() {
	
}

void CPU::cb_opcode_DA() {
	
}

void CPU::cb_opcode_DB() {
	
}

void CPU::cb_opcode_DC() {
	
}

void CPU::cb_opcode_DD() {
	
}

void CPU::cb_opcode_DE() {
	
}

void CPU::cb_opcode_DF() {
	
}

void CPU::cb_opcode_E0() {
	
}

void CPU::cb_opcode_E1() {
	
}

void CPU::cb_opcode_E2() {
	
}

void CPU::cb_opcode_E3() {
	
}

void CPU::cb_opcode_E4() {
	
}

void CPU::cb_opcode_E5() {
	
}

void CPU::cb_opcode_E6() {
	
}

void CPU::cb_opcode_E7() {
	
}

void CPU::cb_opcode_E8() {
	
}

void CPU::cb_opcode_E9() {
	
}

void CPU::cb_opcode_EA() {
	
}

void CPU::cb_opcode_EB() {
	
}

void CPU::cb_opcode_EC() {
	
}

void CPU::cb_opcode_ED() {
	
}

void CPU::cb_opcode_EE() {
	
}

void CPU::cb_opcode_EF() {
	
}

void CPU::cb_opcode_F0() {
	
}

void CPU::cb_opcode_F1() {
	
}

void CPU::cb_opcode_F2() {
	
}

void CPU::cb_opcode_F3() {
	
}

void CPU::cb_opcode_F4() {
	
}

void CPU::cb_opcode_F5() {
	
}

void CPU::cb_opcode_F6() {
	
}

void CPU::cb_opcode_F7() {
	
}

void CPU::cb_opcode_F8() {
	
}

void CPU::cb_opcode_F9() {
	
}

void CPU::cb_opcode_FA() {
	
}

void CPU::cb_opcode_FB() {
	
}

void CPU::cb_opcode_FC() {
	
}

void CPU::cb_opcode_FD() {
	
}

void CPU::cb_opcode_FE() {
	
}

void CPU::cb_opcode_FF() {
	
}

