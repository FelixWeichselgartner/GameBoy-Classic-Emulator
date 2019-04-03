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

public:

    ROM();
	~ROM();

	void load(class RAM *);

	std::string getGameName(class RAM* ram);
    
	void print(class RAM*, unsigned short, unsigned short);
};
//----------------------------------------------------------------------------------------------

#endif