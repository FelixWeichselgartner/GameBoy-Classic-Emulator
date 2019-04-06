#include "../include/Joypad.hpp"

Joypad::Joypad(class CPU* cpuLink) {
	this->cpuLink = cpuLink;
	this->JoypadState = 0x00;
}

#include <iostream>

Byte Joypad::getJoypadState() const {
	Byte retval = this->cpuLink->ram.getMemory(0xFF00) ^ 0xFF;
	Byte topJoypad, bottomJoypad;

	if (!testBit(retval, 4)) {
		topJoypad = (JoypadState >> 4) | 0xF0;
		retval &= topJoypad;
	} else if (!testBit(retval, 5)) {
		bottomJoypad = (JoypadState & 0x0F) | 0xF0;
		retval &= bottomJoypad;
	}

	//std::cout << toBinary(retval) << std::endl;
	return retval;
}

void Joypad::KeyPressed(int key) {
	bool previouslyUnset = !testBit(JoypadState, key);
	JoypadState = resetBit(JoypadState, key);
	bool button = key > 3;
	Byte keyReq = this->cpuLink->ReadByte(ADDR_IO);
	bool requestInterupt;

	if (button && testBit(keyReq, 5)) {
		requestInterupt = true;
	} else if (!button && testBit(keyReq, 4)) {
		requestInterupt = true;
	} else {
		requestInterupt = false;
	}

	if (requestInterupt && !previouslyUnset) {
		this->cpuLink->RequestInterupt(4);
	}

	return;
}

void Joypad::KeyReleased(int key) {
	JoypadState = setBit(JoypadState, key);

	return;
}