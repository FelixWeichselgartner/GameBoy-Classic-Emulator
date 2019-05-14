#ifndef MBC_HPP_
#define MBC_HPP_

#include "datatypes.h"
#include "format.hpp"
#include "MemoryMap.hpp"
#include "ROM.hpp"
#include "RAM.hpp"

class MBC {

protected:

    bool EnableRamBanking;
    Byte CurrentRomBank, CurrentRamBank;

    class ROM *rom;
    class RAM *ram;

public:

    MBC();
    MBC(class ROM*, class RAM*);

    virtual Byte ReadROM(Word);
	virtual void WriteROM(Word, Byte);
	virtual Byte ReadRAM(Word);
	virtual void WriteRAM(Word, Byte);

    bool getRamEnable() const;
    void setRamEnable(bool);

	Byte getCurrentRomBank() { return this->CurrentRomBank; }

};

#endif