#include "../include/Joypad.hpp"

Joypad::Joypad(class CPU* cpuLink) {
	this->cpuLink = cpuLink;
	this->JoypadState = 0xFF;
}

#include <iostream>

Byte Joypad::getJoypadState() const {
	Byte retval = this->cpuLink->ram.getMemory(ADDR_IO);

	if (!testBit(retval, 4)) {
		for (int i = 4; i < 8; i++) {
			if ((JoypadState >> i) & 1)
				retval |= (1 << (i - 4));
			else
				retval &= ~(1 << (i - 4));
		}
	} else if (!testBit(retval, 5)) {
		for (int i = 0; i < 4; i++) {
			if ((JoypadState >> i) & 1)
				retval |= (1 << i);
			else
				retval &= ~(1 << i);
		}
	}

	//std::cout << toBinary(retval) << std::endl;
	return retval;
}

void Joypad::KeyPressed(int key) {
	bool previouslyUnset = !testBit(JoypadState, key);

	std::cout << "previouslyUnset: " << (int)previouslyUnset << std::endl;

	JoypadState = resetBit(JoypadState, key);

	std::cout << "JoypadState: " << toBinary(JoypadState) << std::endl;

	bool button = key > 3;
	Byte keyReq = this->cpuLink->ram.getMemory(ADDR_IO);
	bool requestInterupt = (button && !testBit(keyReq, 5)) || (!button && !testBit(keyReq, 4));

	std::cout << "requestInterupt: " << (int)requestInterupt << std::endl;

	if (requestInterupt && !previouslyUnset) {
		std::cout << "request interupt 4" << std::endl;
		this->cpuLink->RequestInterupt(4);
	}
	else {
		std::cout << "not request interupt 4" << std::endl;
	}

	return;
}

void Joypad::KeyReleased(int key) {
	JoypadState = setBit(JoypadState, key);

	return;
}