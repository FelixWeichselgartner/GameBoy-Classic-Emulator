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
	int RomSize;
	bool* EnableBootstrap;

	// rom banking.
	bool MBC_1, MBC_2, MBC_3, MBC_5;
	bool RomBanking;
	Byte CurrentRomBank, RomBankingMode;

	// ram link.
	class RAM* ram;

public:

    ROM();
	ROM(class RAM*, bool*);
	~ROM();

	Byte getMemory(unsigned int) const;

	void load();

	int getRomSize() const { return this->RomSize; }
	void InitialiseRomBaking();
	Byte getCurrentRomBank() const;
	Byte getRomBankingMode() const;
	void ChangeLowRomBank(Byte);
	void ChangeHighRomBank(Byte);
	void ChangeRomRamMode(Byte);
	void EnableRamBank(unsigned short, Byte);
	void HandleBanking(unsigned short, Byte);

	std::string getGameName();

};
//----------------------------------------------------------------------------------------------

#endif