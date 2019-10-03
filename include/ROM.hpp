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
	unsigned int RomSize;
	bool* EnableBootstrap;

	// ram link.
	class RAM* ram;

public:

    void reset();

    ROM();
	ROM(class RAM*, bool*);
	~ROM();

	Byte getMemory(unsigned int) const;

	void load(std::string);

	int getRomSize() const { return this->RomSize; }

	std::string getGameName();

};
//----------------------------------------------------------------------------------------------

#endif