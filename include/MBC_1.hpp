#ifndef MBC_1_HPP_
#define MBC_1_HPP_

#include "MBC.hpp"

class MBC_1: public MBC {

private:

	Byte Mode;
	Byte SelectedRomBank = 1, SelectedRamBank = 0;
	Byte Bank1, Bank2;

public:

	void resetVar();
    void reset();

	MBC_1();
	MBC_1(class ROM*, class RAM*);

	Byte getCurrentRomBank() const;
	void BankReg1(Byte);
	void BankReg2(Byte);
	void ChangeRamBank(Byte);
	void ChangeMode(Byte);
	void EnableRamBank(Word, Byte);
	void HandleBanking(Word, Byte);

	Byte ReadROM(Word);
	void WriteROM(Word, Byte);
	Byte ReadRAM(Word);
	void WriteRAM(Word, Byte);

};

#endif