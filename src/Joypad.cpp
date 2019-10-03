#include "../include/Joypad.hpp"
#include "../include/CPU.hpp"

#include <iostream>
using namespace std;

void Joypad::reset() {
    this->JoypadState = 0xFF;
}

Joypad::Joypad(class CPU* cpu) {
	if ((this->cpu = cpu) == NULL) exit(2);
	reset();
}

Byte Joypad::getJoypadState() const {
	Byte retval = ~(cpu->memory.getJoypadRequest());

	// gameboy: for direction:
	// 3 = down, 2 = up, 1 = left, 0 = down.
	if (!testBit(retval, DIRECTION)) { 
		for (int i = 4; i < 8; i++) {
			if ((JoypadState >> i) & 1) {
				retval |= (1 << (i - 4));
            } else {
				retval &= ~(1 << (i - 4));
            }
		}
	} 
    
	// gameboy: for buttons:
	// 3 = start, 2 = select, 1 = B, 0 = A.
	else if (!testBit(retval, BUTTON)) {
		for (int i = 0; i < 4; i++) {
			if ((JoypadState >> i) & 1) {
				retval |= (1 << i);
            } else {
				retval &= ~(1 << i);
            }
		}
	}

	return retval | 0b11000000;
}

void Joypad::KeyPressed(int key) {
	// 0 (set) -> true, 1 (unset) -> false.
	bool previouslySet = !testBit(JoypadState, key);
	JoypadState = resetBit(JoypadState, key);
	Byte keyReq = this->cpu->memory.getJoypadRequest();
	bool button = key > 3;
	bool requestInterupt = (button && testBit(keyReq, BUTTON)) || (!button && testBit(keyReq, DIRECTION));

	if (requestInterupt && !previouslySet) {
		this->cpu->RequestInterrupt(4);
	}

	return;
}

void Joypad::KeyReleased(int key) {
	JoypadState = setBit(JoypadState, key);

	return;
}