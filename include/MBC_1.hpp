#ifndef MBC_1_HPP_
#define MBC_1_HPP_

#include "MBC.hpp"

class MBC_1: public MBC {

private:

	bool RomBanking;
	Byte Mode, HigherRBBits;

public:

	void resetVar();
    void reset();

	MBC_1();
	MBC_1(class ROM*, class RAM*);

	Byte getCurrentRomBank() const;
	void ChangeLowRomBank(Byte);
	void ChangeHighRomBank(Byte);
	void ChangeMode(Byte);
	void EnableRamBank(unsigned short, Byte);
	void HandleBanking(unsigned short, Byte);

	Byte ReadROM(Word);
	void WriteROM(Word, Byte);
	Byte ReadRAM(Word);
	void WriteRAM(Word, Byte);

	// save external ram to .sav file.
	void saveExtRam();
	// load external ram from .sav file.
	void loadExtRam();

};

#endif