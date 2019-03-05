#include "../include/CPU.hpp"
#include "../include/Registers.hpp"

// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

CPU::CPU() {

}

byte CPU::ReadByte(unsigned short address) {
    return this->ram.getByte(address);
}

void CPU::WriteByte(unsigned short address, byte value) {
    this->ram.setByte(address, value);
    return;
}

void CPU::executeInstruction(byte opcode) {
    switch((int)opcode) {
        case  0x0: // NOP           no operation.
            break;
        case  0x1: // LD (BC), nn   load 16-bit immediate into BC.
            break;
        case  0x2: // LD (BC), A    saves A to address pointed by BC.
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
            break;
        case  0x7: // RLC A         rotate A left with carry.
            break;
        case  0x8: // LD (nn), SP   save SP to a given address.
            break;
        case  0x9: // ADD HL, BC    add 16-bit BC to HL.
            this->registers.setHL(this->registers.getHL() + this->registers.getBC());
            break;
        case  0xA: // LD A, (BC)    load A from address pointed by BC.
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
            break;
        case  0xF: // RRC A         rotate A right with carry.
            break;
        case 0x10: // STOP          stop processor.
            break;
        case 0x11: // LD DE, nn     load 16-bit immediate into DE.
            break;
        case 0x12: // LD (DE), A    save A to address pointed by DE.
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
            break;
        case 0x17: // RL A          rotate A left.
            break;
        case 0x18: // JR n          relative jump by signed immediate.
            break;
        case 0x19: // ADD HL, DE    add 16-bit DE to HL.
            break;
        case 0x1A: // LD A, (DE)    load A from address pointed to by DE.
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
            break;
        case 0x1F: // RR A          rotate A right.
            break;
        case 0x20: // JR NC, n      relative jump by signed immediate if last result was not zero.
            break;
        case 0x21: // LD HL, nn     load 16-bit immediate into HL.
            break;
        case 0x22: // LDI (HL), A   save A to address pointed by HL, and increment HL.
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
            break;
        case 0x27: // DAA           adjust A for BCD addition.
            break;
        case 0x28: // JR Z, n       relative jump by signed immediate if last result was zero.
            break;
        case 0x29: // ADD HL, HL    add 16-bit HL to HL.
            break;
        case 0x2A: // LDI A, (HL)   load A from address pointed to by HL, and increment HL.
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
            break;
        case 0x2F: // CPL           complement (logical NOT) on A.
            break;
        case 0x30: // JR NC, n      relative jump by signed immediate if last result caused no carry.
            break;
        case 0x31: // LD SP, nn     load 16-bit immediate into SP.
            break;
        case 0x32: // LDD (HL), A   save A to address pointed by HL, and decrement HL.
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
            break;
        case 0x37: // SCF           set carry flag.
            break;
        case 0x38: // JR C, n       relative jump by signed immediate if last result caused carry.
            break;
        case 0x39: // ADD HL, SP    add 16-bit SP to HL.
            break;
        case 0x3A: // LDD A, (HL)   load A from address pointed to by HL, and decrement HL.
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
            break;
        case 0x3F: // CCF           clear carry flag.
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
            this->registers.setA(this->registers.getA() + this->registers.getB());
            break;
        case 0x81: // ADD A, C      add C to A.
            this->registers.setA(this->registers.getA() + this->registers.getC());
            break;
        case 0x82: // ADD A, D      add D to A.
            this->registers.setA(this->registers.getA() + this->registers.getD());
            break;
        case 0x83: // ADD A, E      add E to A.
            this->registers.setA(this->registers.getA() + this->registers.getE());
            break;
        case 0x84: // ADD A, H      add H to A.
            this->registers.setA(this->registers.getA() + this->registers.getH());
            break;
        case 0x85: // ADD A, L      add L to A.
            this->registers.setA(this->registers.getA() + this->registers.getL());
            break;
        case 0x86: // ADD A, (HL)   add value pointed by HL to A.
            this->registers.setA(this->registers.getA() + this->ram.getMemory(this->registers.getHL()));
            break;
        case 0x87: // ADD A, A      add A to A.
            this->registers.setA(this->registers.getA() + this->registers.getA());
            break;
        case 0x88: // ADC A, B      add B and carry flag to A.
            break;
        case 0x89: // ADC A, C      add C and carry flag to A.
            break;
        case 0x8A: // ADC A, D      add D and carry flag to A.
            break;
        case 0x8B: // ADC A, E      add E and carry flag to A.
            break;
        case 0x8C: // ADC A, H      add H and carry flag to A.
            break;
        case 0x8D: // ADC A, L      add L and carry flag to A.
            break;
        case 0x8E: // ADC A, (HL)   add value pointed by HL and carry flag to A.
            break;
        case 0x8F: // ADC A, A      add A and carry flag to A.
            break;
        case 0x90: // SUB A, B      subtract B from A.
            break;
        case 0x91: // SUB A, C      subtract C from A.
            break;
        case 0x92: // SUB A, D      subtract D from A.
            break;
        case 0x93: // SUB A, E      subtract E from A.
            break;
        case 0x94: // SUB A, H      subtract H from A.
            break;
        case 0x95: // SUB A, L      subtract L from A.
            break;
        case 0x96: // SUB A, (HL)   subtract value pointed by HL from A.
            break; 
        case 0x97: // SUB A, A      subtract A from A.
            break;
        case 0x98: // SBC A, B      subtract B and carry flag from A.
            break;
        case 0x99: // SBC A, C      subtract C and carry flag from A.
            break;
        case 0x9A: // SBC A, D      subtract D and carry flag from A.
            break;
        case 0x9B: // SBC A, E      subtract E and carry flag from A.
            break;
        case 0x9C: // SBC A, H      subtract H and carry flag from A.
            break;
        case 0x9D: // SBC A, L      subtract L and carry flag from A.
            break;
        case 0x9E: // SBC A, (HL)   subtract value pointed by HL and carry flag from A.
            break;
        case 0x9F: // SBC A, A      subtract A and carry flag from A.
            break; 
        case 0xA0: // AND B         logical AND B against A.
            break;
        case 0xA1: // AND C         logical AND C against A.
            break;
        case 0xA2: // AND D         logical AND D against A.
            break;
        case 0xA3: // AND E         logical AND E against A.
            break;
        case 0xA4: // AND H         logical AND H against A.
            break;
        case 0xA5: // AND L         logical AND L against A.
            break; 
        case 0xA6: // AND (HL)      logical AND value pointed by HL against A.
            break;
        case 0xA7: // AND A         logical AND A against A.
            break;
        case 0xA8: // XOR B         logical XOR B against A.
            break;
        case 0xA9: // XOR C         logical XOR C against A.
            break;
        case 0xAA: // XOR D         logical XOR D against A.
            break;
        case 0xAB: // XOR E         logical XOR E against A.
            break;
        case 0xAC: // XOR H         logical XOR H against A.
            break;
        case 0xAD: // XOR L         logical XOR L against A.
            break;
        case 0xAE: // XOR (HL)      logical XOR value pointed by HL against A.
            break;
        case 0xAF: // XOR A         logical XOR A against A.
            break;
        case 0xB0: // OR B          logical OR B against A.
            break;
        case 0xB1: // OR C          logical OR C against A.
            break;
        case 0xB2: // OR D          logical OR D against A.
            break;
        case 0xB3: // OR E          logical OR E against A.
            break;
        case 0xB4: // OR H          logical OR H against A.
            break;
        case 0xB5: // OR L          logical OR L against A. 
            break;
        case 0xB6: // OR (HL)       logical OR value pointed by HL against A.
            break;
        case 0xB7: // OR A          logical OR A against A.
            break;
        case 0xB8: // CP B          compare B against A.
            break;
        case 0xB9: // CP C          compare C against A.
            break;
        case 0xBA: // CP D          compare D against A.
            break;
        case 0xBB: // CP E          compare E against A.
            break;
        case 0xBC: // CP H          compare H against A.
            break;
        case 0xBD: // CP L          compare L against A.
            break;
        case 0xBE: // CP (HL)       compare value pointed by HL against A.
            break;
        case 0xBF: // CP A          compare A against A.
            break;
        case 0xC0: // RET NZ        return if last result was not zero.
            break;
        case 0xC1: // POP BC        pop 16-bit value from stack into BC.
            break;
        case 0xC2: // JP NZ, nn     absolute jump to 16-bit location if last result was not zero.
            break;
        case 0xC3: // JP nn         absolute jump to 16-bit location.
            break;
        case 0xC4: // CALL NZ, nn   call routine at 16-bit location if last result was not zero.
            break;
        case 0xC5: // PUSH BC       push 16-bit BC onto stack.
            break;
        case 0xC6: // ADD A, n      add 8-bit immediate to A. 
            break;
        case 0xC7: // RST 0         call routine at address 0000h
            break;
        case 0xC8: // RET Z         return if last result was zero.
            break;
        case 0xC9: // RET           return to calling routine.
            break;
        case 0xCA: // JP Z, nn      absolute jump to 16-bit location if last result was zero.
            break;
        case 0xCB: // Ext ops       extended operations (two-byte instruction code).
            break;
        case 0xCC: // CALL Z, nn    call routine at 16-bit location if last result was zero.
            break;
        case 0xCD: // CALL nn       call routine at 16-bit location.
            break;
        case 0xCE: // ADC A, n      add 8-bit immediate and carry to A.
            break;
        case 0xCF: // RST 8         call routine at adress 0008h.
            break;
        case 0xD0: // RET NC        return if last result caused no carry.
            break;
        case 0xD1: // POP DE        pop 16-bit value from stack into DE.
            break;
        case 0xD2: // JP NC, nn     absolute jump to 16-bit location if last result caused no carry.
            break;
        case 0xD3: // XX            operation removed in this CPU.
            break;
        case 0xD4: // CALL NC, nn   call routine at 16-bit location if last result caused no carry.
            break;
        case 0xD5: // PUST DE       push 16-bit DE onto stack.
            break;
        case 0xD6: // SUB A, n      subtract 8-bit immediate from A. 
            break;
        case 0xD7: // RST 10        call routine at address 0010h.
            break;
        case 0xD8: // RET C         return if last result caused carry.
            break;
        case 0xD9: // RETI          enable interrupts and return to calling routine.
            break; 
        case 0xDA: //JP C, nn       absolute jump to 16-bit location if last result caused carry.
            break;
        case 0xDB: // XX            operation removed in this CPU. 
            break;
        case 0xDC: // CALL C, nn    call routine at 16-bit location if last result caused no carry.
            break;
        case 0xDD: // XX            operation removed in this CPU.
            break;
        case 0xDE: // SBC A, n      subtract 8-bit immediate and carry from A. 
            break;
        case 0xDF: // RST 18        call routine at address 0018h.
            break;
        case 0xE0: // LDH (n), A    save A at address pointed to by (FF00h + 8-bit immediate)
            break;
        case 0xE1: // POP HL        pop 16-bit value from stack into HL.
            break;
        case 0xE2: // LDH (C), A    save A at address pointed to by (FF00h + C)
            break;
        case 0xE3: // XX            operation removed in this CPU.
            break;
        case 0xE4: // XX            operation removed in this CPU.
            break;
        case 0xE5: // PUSH HL       Push 16-bit HL onto stack.
            break;
        case 0xE6: // AND n         logical AND 8-bit immediate against A.
            break;
        case 0xE7: // RST 20        call routine at address 0020h.
            break;
        case 0xE8: // ADD SP, d     add signed 8-bit immediate to SP.
            break;
        case 0xE9: // JP (HL)       jump to 16-bit value pointed by HL.
            break;
        case 0xEA: // LD (nn), A    save A at given 16-bit address.
            break;
        case 0xEB: // XX            operation removed in this CPU.
            break;
        case 0xEC: // XX            operation removed in this CPU.
            break;
        case 0xED: // XX            operation removed in this CPU.
            break;
        case 0xEE: // XOR n         logical XOR 8-bit immediate against A.
            break;
        case 0xEF: // RST 28        call routine at address 0028h.
            break;
        case 0xF0: // LDH A, (n)    load a from address pointed to by (FF00h + 8-bit immediate).
            break;
        case 0xF1: // POP AF        pop 16-bit value from stack into AF.
            break;
        case 0xF2: // XX            operation removed in this CPU.
            break;
        case 0xF3: // DI            disable interrupts
            break;
        case 0xF4: // XX            operation removed in this CPU.
            break;
        case 0xF5: // PUSH AF       push 16-bit AF onto stack.
            break;
        case 0xF6: // OR n          logical OR 8-bit immediate against A.
            break;
        case 0xF7: // RST 30        call routine at address 0030h.
            break;
        case 0xF8: // LDHL SP, d    add signed 8-bit immediate to SP and save result in HL.
            break;
        case 0xF9: // LD SP, HL     copy HL to SP.
            break;
        case 0xFA: // LD A, (nn)    load A from given 16-bit address.
            break;
        case 0xFB: // EI            enable interrupts.
            break;
        case 0xFC: // XX            operation removed in this CPU.
            break;
        case 0xFD: // XX            operation removed in this CPU.
            break;
        case 0xFE: // CP n          compare 8-bit immediate against A.
            break;
        case 0xFF: //RST 38         call routine at address 0038h.
            break;
    }
}