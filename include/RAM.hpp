#ifndef RAM_HPP_
#define RAM_HPP_

//----------------------------------------------------------------------------------------------
#include "datatypes.h"
#include "MemoryMap.hpp"
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class RAM {

private:

	Byte ram[ADDR_ECHO - ADDR_EXT_RAM] = { 0 };

	// ram banking.
	Byte *RamBanks;
	Byte CurrentRamBank, AmountBanks;
	bool EnableRamBanking;

public:

	void reserveRamBankMemory(Byte);

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

	Byte getAmountBanks() { return this->AmountBanks; }
	
};
//----------------------------------------------------------------------------------------------

#endif