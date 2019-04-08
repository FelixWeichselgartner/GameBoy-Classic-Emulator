#ifndef JOYPAD_HPP_
#define JOYPAD_HPP_

//----------------------------------------------------------------------------------------------
#include "../include/CPU.hpp"

#define JOY_START	7
#define JOY_SELECT	6
#define JOY_B		5
#define JOY_A		4
#define JOY_DOWN	3
#define JOY_UP		2
#define JOY_LEFT	1
#define JOY_RIGHT	0

#define DIRECTION	4
#define BUTTON		5

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