#include "../include/MBC_Cartridge.hpp"
#include "../include/datatypes.h"

/*
The idea if this class is to enable you playing on an ACTUAL GAMEBOY CARTRIDGE.

In the past I have made an Arduino Shield to dump GameBoy Cartridges. Here the Repo:
https://github.com/FelixWeichselgartner/GB-Cartridge-Reader-Writer (May be privat at the time)

This project was inspired by: https://www.insidegadgets.com/2011/03/19/gbcartread-arduino-based-gameboy-cart-reader-%E2%80%93-part-1-read-the-rom/

With the abilitiy to read and write to a GameBoy Cartridge I saw the opportunity to combine these two projects.
Therefore this class will enable you to play with your REAL GAMEBOY CARTRIDGE on my Emulator on a Raspberry Pi, if you have the necessary hardware.

Proper schematics and layouts will follow, but its similar to the one of InsideGadgets or my GC-Cartridge-Reader-Writer.

********************************************************************************************************************************

This class should be pretty easy as no knowledge of how a Cartridge operates on the inside is needed.

The operation for reading rom and ram are the same -> Therefore both functions will just call the ReadByte function.
The operation for writing rom and ram are the same -> Therefore both functions will just call the WriteByte function.
*/

//#include "GB-Catridge.hpp"

/*
GB-Cartridge.hpp is part of: https://github.com/FelixWeichselgartner/GB-Cartridge-Reader-Writer (May be privat at the time)
and handels the operations needed for Cartridge reading and writing.

WiringPi (http://wiringpi.com/) will be needed for this to run.
Or you can use an Arduino for Serial-Communication to receive the Data. (<- Similar how I used it in my project)
*/

void MBC_Cartridge::reset() {
    MBC::reset();
}

MBC_Cartridge::MBC_Cartridge(): MBC() {

}

MBC_Cartridge::MBC_Cartridge(class ROM* rom, class RAM* ram): MBC(rom, ram) {

}

Byte MBC_Cartridge::ReadROM(Word address) {
    return ReadByte(address);
}

void MBC_Cartridge::WriteROM(Word address, Byte data) {
    WriteByte(address, data);
}

Byte MBC_Cartridge::ReadRAM(Word address) {
    return ReadRAM(address);
}

void MBC_Cartridge::WriteRAM(Word address, Byte data) {
    WriteRAM(address, data);
}

Byte MBC_Cartridge::ReadByte(Word address) {
    
}

void MBC_Cartridge::WriteByte(Word address, Byte data){

}