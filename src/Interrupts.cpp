#include "../include/CPU.hpp"

#define INTERUPT_VBLANK 0x40
#define INTERUPT_LCD	0x48
#define INTERUPT_TIMER	0x50
#define INTERUPT_JOYPAD 0x60

enum {
    VBLANK,
    LCD,
    TIMER,
    JOYPAD = 4
};

void CPU::RequestInterupt(int id) {
	Byte req = this->memory.ReadByte(ADDR_INTR_REQ);
	req = setBit(req, id);
	this->memory.WriteByte(ADDR_INTR_REQ, req);

	gb_halt = false;

	return;
}

void CPU::DoInterupts() {
	Byte req, enabled;

	if (gb_ime) {
		req = this->memory.ReadByte(ADDR_INTR_REQ);
		enabled = this->memory.ReadByte(ADDR_INTR_EN);

		if (req > 0) {
			for (int i = 0; i < 8; i++) {
				if (testBit(req, i) && testBit(enabled, i)) {
					ServiceInterupts(i);
				}
			}
		}
	}

	return;
}

void CPU::ServiceInterupts(int interupt) {
	Byte req = resetBit(this->memory.ReadByte(ADDR_INTR_REQ), interupt);
    gb_ime = false;

	this->memory.WriteByte(ADDR_INTR_REQ, req);
	this->memory.PushWord(this->registers.getPC());

	switch (interupt) {
		case VBLANK:
			this->registers.setPC(INTERUPT_VBLANK);
			break;
		case LCD:
			this->registers.setPC(INTERUPT_LCD);
			break;
		case TIMER:
			this->registers.setPC(INTERUPT_TIMER);
			break;
		case JOYPAD:
			this->registers.setPC(INTERUPT_JOYPAD);
			break;
		default:
			break;
	}
}