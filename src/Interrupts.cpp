#include "../include/CPU.hpp"

#define INTERRUPT_VBLANK 0x40
#define INTERRUPT_LCD	 0x48
#define INTERRUPT_TIMER	 0x50
#define INTERRUPT_SERIAL 0x58
#define INTERRUPT_JOYPAD 0x60

enum {
    VBLANK,
    LCD,
    TIMER,
	SERIAL,
    JOYPAD
};

void CPU::RequestInterrupt(int id) {
	Byte req = this->memory.ReadByte(ADDR_INTR_REQ);
	req = setBit(req, id);
	this->memory.WriteByte(ADDR_INTR_REQ, req);

	gb_halt = false;

	return;
}

int CPU::DoInterrupts() {
	Byte req, enabled;
	int c = 0;

	if (gb_ime) {
		req = this->memory.ReadByte(ADDR_INTR_REQ);
		enabled = this->memory.ReadByte(ADDR_INTR_EN);

		if (req > 0) {
			for (int i = 0; i < 8; i++) {
				if (testBit(req, i) && testBit(enabled, i)) {
					c += ServiceInterrupts(i);
				}
			}
		}
	}

	return c;
}

int CPU::ServiceInterrupts(int interrupt) {
	Byte req = resetBit(this->memory.ReadByte(ADDR_INTR_REQ), interrupt);
    gb_ime = false;

	this->memory.WriteByte(ADDR_INTR_REQ, req);
	this->memory.PushWord(this->registers.getPC());

	switch (interrupt) {
		case VBLANK:
			this->registers.setPC(INTERRUPT_VBLANK);
			return 20;
		case LCD:
			this->registers.setPC(INTERRUPT_LCD);
			return 20;
		case TIMER:
			this->registers.setPC(INTERRUPT_TIMER);
			return 20;
		case SERIAL:
			//this->registers.setPC(INTERRUPT_SERIAL);
			return 20;
		case JOYPAD:
			this->registers.setPC(INTERRUPT_JOYPAD);
			return 20;
		default:
			return 0;
	}


}