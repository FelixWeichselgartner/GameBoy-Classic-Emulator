#ifndef JOYPAD_HPP_
#define JOYPAD_HPP_

//----------------------------------------------------------------------------------------------
#include "../include/CPU.hpp"

enum {
	JOY_RIGHT,
	JOY_LEFT,
	JOY_UP,
	JOY_DOWN,
	JOY_A,
	JOY_B,
	JOY_SELECT,
	JOY_START
};

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