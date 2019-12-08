#ifndef MBC_5_HPP_
#define MBC_5_HPP_

#include "MBC.hpp"

class MBC_5: public MBC {

private:

    int selectedRamBank;
    int selectedRomBank = 1;

public:

    MBC_5(class ROM*, class RAM*);

    Byte ReadROM(Word);
	void WriteROM(Word, Byte);
	Byte ReadRAM(Word);
	void WriteRAM(Word, Byte);

private:

    int getRomAddress(int bank, int address);
    int getRamAddress(int address);
};

#endif