#ifndef MBC_1_HPP_
#define MBC_1_HPP_

#include "MBC.hpp"

class MBC_1: public MBC {

private:

	bool RomBanking;
	Byte RomBankingMode;

public:

	MBC_1();
	MBC_1(class ROM*, class RAM*);

	Byte getCurrentRomBank() const;
	void ChangeLowRomBank(Byte);
	void ChangeHighRomBank(Byte);
	void ChangeRomRamMode(Byte);
	void EnableRamBank(unsigned short, Byte);
	void HandleBanking(unsigned short, Byte);

	Byte ReadROM(Word);
	void WriteROM(Word, Byte);
	Byte ReadRAM(Word);
	void WriteRAM(Word, Byte);

};

#endif