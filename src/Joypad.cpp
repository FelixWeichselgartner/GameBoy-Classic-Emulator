#include "../include/Joypad.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

Joypad::Joypad(class CPU* cpuLink) {
	this->cpuLink = cpuLink;
	this->JoypadState = 0xFF;
}

Byte Joypad::getJoypadState() const {
	Byte retval = this->cpuLink->ram.getMemory(ADDR_IO);
	//cout << "in get joypad state: " << toBinary(retval) << endl;

	// gameboy: for direction:
	// 3 = down, 2 = up, 1 = left, 0 = down.
	if (!testBit(retval, DIRECTION)) { 
		for (int i = 4; i < 8; i++) {
			if ((JoypadState >> i) & 1)
				retval |= (1 << (i - 4));
			else
				retval &= ~(1 << (i - 4));
		}
	} 
	// gameboy: for buttons:
	// 3 = start, 2 = select, 1 = B, 0 = A.
	else if (!testBit(retval, BUTTON)) {
		for (int i = 0; i < 4; i++) {
			if ((JoypadState >> i) & 1)
				retval |= (1 << i);
			else
				retval &= ~(1 << i);
		}
	}

	//cout << "retval: " << toBinary(retval) << endl;
	return retval;
}

void Joypad::KeyPressed(int key) {
	bool previouslySet = !testBit(JoypadState, key);
	Byte keyReq = this->cpuLink->ram.getMemory(ADDR_IO);
	bool button = key > 3;

	cout << "key req" << toBinary(this->cpuLink->ram.getMemory(0xff00)) << endl;
	cout << "button boolean: " << boolalpha << button << endl;
	cout << "button req: " << boolalpha << testBit(keyReq, BUTTON) << " dir req: " << boolalpha << testBit(keyReq, DIRECTION) << endl;

	JoypadState = resetBit(JoypadState, key);

	bool requestInterupt = (button && !testBit(keyReq, BUTTON)) || (!button && !testBit(keyReq, DIRECTION));

	cout << "requestInterupt: " << boolalpha << requestInterupt << endl;

	if (requestInterupt && previouslySet) {
		cout << "request interupt 4" << endl;
		this->cpuLink->RequestInterupt(4);
	}
	else {
		cout << "not request interupt 4" << endl;
	}

	return;
}

void Joypad::KeyReleased(int key) {
	JoypadState = setBit(JoypadState, key);

	return;
}