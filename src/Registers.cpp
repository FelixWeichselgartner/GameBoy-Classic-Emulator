#include "../include/Registers.hpp"

// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

Registers::Registers() {

}
    
byte Registers::getA() const {
    return this->A;
}
void Registers::setA(byte value) {
    this->A = value;
}

byte Registers::getF() const {
    return this->F;
}
void Registers::setF(byte value) {
    this->F = value;
}

unsigned short Registers::getAF() const {
    return this->AF;
}

void Registers::setAF(unsigned short value) {
    this->AF = value;
}

byte Registers::getB() const {
    return this->B;
}

void Registers::setB(byte value) {
    this->B = value;
}

byte Registers::getC() const {
    return this->C;
}
void Registers::setC(byte value) {
    this-> C = value;
}

unsigned short Registers::getBC() const {
    return this->BC;
}

void Registers::setBC(unsigned short value) {
    this->BC = value;
}

byte Registers::getD() const {
    return this->A;
}
void Registers::setD(byte value) {
    this->A = value;
}

byte Registers::getE() const {
    return this->E;
}

void Registers::setE(byte value) {
    this->E = value;
}

unsigned short Registers::getDE() const {
    return this->DE;
}
void Registers::setDE(unsigned short value) {
    this->DE = value;
}

byte Registers::getH() const {
    return this->H;
}
void Registers::setH(byte value) {
    this->H = value;
}

byte Registers::getL() const {
    return this->L;
}
void Registers::setL(byte value) {
    this->L = value;
}

unsigned short Registers::getHL() const {
    return this->HL;
}

void Registers::setHL(unsigned short value) {
    this->HL = value;
}

unsigned short Registers::getPC() const {
    return this->PC;
}

void Registers::setPC(unsigned short value) {
    this->PC = value;
}

unsigned short Registers::getSP() const {
    return this->SP;
}

void Registers::setSP(unsigned short value) {
    this->SP = value;
}