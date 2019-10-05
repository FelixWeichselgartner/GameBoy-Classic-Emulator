#ifndef MBC_1_HPP_
#define MBC_1_HPP_

#include "MBC.hpp"

class MBC_1: public MBC {

private:

	bool RomBanking;
	Byte Mode, HigherRBBits;
	int LowRomBank = -1, HighRomBank = -1;
	Byte SelectedRomBank = 1, SelectedRamBank = 0;

public:

	void resetVar();
    void reset();

	MBC_1();
	MBC_1(class ROM*, class RAM*);

	Byte getCurrentRomBank() const;
	void ChangeLowRomBank(Byte);
	void ChangeHighRomBank(Byte);
	void ChangeRamBank(Byte);
	void ChangeMode(Byte);
	void EnableRamBank(Word, Byte);
	void HandleBanking(Word, Byte);
	int getAdjustedRomBankLow();
	int getAdjustedRomBankHigh();

	Byte ReadROM(Word);
	void WriteROM(Word, Byte);
	Byte ReadRAM(Word);
	void WriteRAM(Word, Byte);

};

#endif