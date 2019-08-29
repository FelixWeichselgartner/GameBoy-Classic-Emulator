#ifndef MBC_Cartridge_HPP_
#define MBC_Cartridge_HPP_

#include "MBC.hpp"

class MBC_Cartridge: public MBC {

private:

public:

    void reset();

    MBC_Cartridge();
    MBC_Cartridge(class ROM*, class RAM*);

    Byte ReadROM(Word);
	void WriteROM(Word, Byte);
	Byte ReadRAM(Word);
	void WriteRAM(Word, Byte);

    Byte ReadByte(Word);
    void WriteByte(Word, Byte);

};

#endif