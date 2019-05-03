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

	// rom banking.
	bool MBC_1, MBC_2;
	bool RomBanking;
	Byte CurrentRomBank, RomBankingMode;

	// ram link.
	class RAM* ram;

public:

    ROM();
	ROM(class RAM*);
	~ROM();

	Byte getMemory(unsigned int) const;

	void load(class RAM*, bool);
	void dltBootstrap(class RAM*);

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