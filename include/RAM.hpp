#ifndef RAM_HPP_
#define RAM_HPP_

//----------------------------------------------------------------------------------------------
#include "datatypes.h"
#include "MemoryMap.hpp"
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class RAM {

private:

	Array_Type(Byte) ram = Array(Byte, ADDR_ECHO - ADDR_EXT_RAM, 0xFF);

	// ram banking.
	Array_Type(Byte)* RamBanks;
	Byte CurrentRamBank, AmountBanks;
	bool EnableRamBanking;
	int RamLength;

public:

    void reset();

    RAM();
	~RAM();

    void reserveRamBankMemory(Byte);

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