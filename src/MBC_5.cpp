#include "../include/MBC_5.hpp"
#include <iostream>
using namespace std;

MBC_5::MBC_5(class ROM* rom, class RAM* ram): MBC(rom, ram) {
    loadExtRam();
}

int MBC_5::getRomAddress(int bank, int address) {
    return bank * 0x4000 + address;
}

int MBC_5::getRamAddress(int address) {
    return selectedRamBank * 0x2000 + (address - ADDR_EXT_RAM);
}

Byte MBC_5::ReadROM(Word address) {
    if (address >= ADDR_ROM_0 && address < ADDR_ROM_1) {
        return this->rom->getMemory(getRomAddress(0, address));
    } else if (address >= ADDR_ROM_1 && address < ADDR_VRAM_T_S) {
        return this->rom->getMemory(getRomAddress(selectedRomBank, address - 0x4000));
    }
}

void MBC_5::WriteROM(Word address, Byte value) {
    if (address >= ADDR_ROM_0 && address < 0x2000) {
        this->ram->setRamEnable((value & 0b1010) != 0);
        if (!this->ram->getRamEnable()) {
            saveExtRam();
            cout << "saved" << endl;
        }
    } else if (address >= 0x2000 && address < 0x3000) {
        selectedRomBank = (selectedRomBank & 0x100) | value;
    } else if (address >= 0x3000 && address < ADDR_ROM_1) {
        selectedRomBank = (selectedRomBank & 0x0ff) | ((value & 1) << 8);
    } else if (address >= ADDR_ROM_1 && address < 0x6000) {
        int bank = value & 0x0f;
        if (bank < this->ram->getAmountBanks()) {
            selectedRamBank = bank;
        }
    } 
}

Byte MBC_5::ReadRAM(Word address) {
    if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
        int ramAddress = getRamAddress(address);
        return this->ram->getRamBankMemory(ramAddress);
    } else {
        return this->ram->getMemory(address - ADDR_INT_RAM_1);
    }
}

void MBC_5::WriteRAM(Word address, Byte value) {
    if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1 && this->ram->getRamEnable()) {
            int ramAddress = getRamAddress(address);
            this->ram->setRamBankMemory(ramAddress, value);
        }
    else {
        this->ram->setMemory(address - ADDR_INT_RAM_1, value);
    }
}
