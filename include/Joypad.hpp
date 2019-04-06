#ifndef JOYPAD_HPP_
#define JOYPAD_HPP_

//----------------------------------------------------------------------------------------------
#include "../include/CPU.hpp"

typedef unsigned char Byte;
//----------------------------------------------------------------------------------------------

class Joypad {

private:

	Byte JoypadState;

	class CPU* cpuLink;

public:

	Joypad(class CPU*);
	
	Byte getJoypadState() const;

	void KeyPressed(int);
	void KeyReleased(int);

};

#endif