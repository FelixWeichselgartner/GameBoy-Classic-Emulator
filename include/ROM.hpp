#ifndef ROM_HPP_
#define ROM_HPP_

//----------------------------------------------------------------------------------------------
#include "../include/RAM.hpp"
#include <string>

//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class ROM {

private:

	char* rom;
	unsigned int RomSize, AmountRomBanks;
	bool* EnableBootstrap;

public:

    void reset();

    ROM();
	ROM(bool*);
	~ROM();

	Byte getMemory(unsigned int) const;

	void load(std::string);

	int getRomSize() const { return this->RomSize; }
	int getAmountRomBanks() const { return this->AmountRomBanks; }

	std::string getGameName();

};
//----------------------------------------------------------------------------------------------

#endif