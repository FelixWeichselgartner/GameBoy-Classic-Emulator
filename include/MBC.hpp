#ifndef MBC_HPP_
#define MBC_HPP_

#include "datatypes.h"
#include "format.hpp"
#include "MemoryMap.hpp"
#include "ROM.hpp"
#include "RAM.hpp"

class MBC {

protected:

    bool EnableRamBanking, battery;
    Byte CurrentRomBank, CurrentRamBank;

    class ROM *rom;
    class RAM *ram;

public:

	void resetVar();
    void reset();

    MBC();
    MBC(class ROM*, class RAM*);

    virtual Byte ReadROM(Word);
	virtual void WriteROM(Word, Byte);
	virtual Byte ReadRAM(Word);
	virtual void WriteRAM(Word, Byte);

    bool getRamEnable() const;
    void setRamEnable(bool);
	bool getBattery() const { return this->battery; }
	void setBattery(bool battery) { this->battery = battery; }

	Byte getCurrentRomBank() { return this->CurrentRomBank; }

};

#endif