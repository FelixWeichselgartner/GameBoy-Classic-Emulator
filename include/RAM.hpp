#ifndef RAM_HPP_
#define RAM_HPP_

//----------------------------------------------------------------------------------------------
#include "datatypes.h"
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class RAM {

private:

	Byte memory[65536] = { 0 };

	// ram banking.
	Byte *RamBanks;
	Byte CurrentRamBank;
	bool EnableRamBanking;

public:

    RAM();

	Byte getMemory(Word) const;
    void setMemory(Word, Byte);

	Byte getCurrentRamBank() const;
	void setCurrentRamBank(Byte);

	bool getRamEnable() const;
	void setRamEnable(bool);

	Byte getRamBankMemory(Word) const;
	void setRamBankMemory(Word, Byte);

	void ChangeRamBank(Byte);
	
};
//----------------------------------------------------------------------------------------------

#endif