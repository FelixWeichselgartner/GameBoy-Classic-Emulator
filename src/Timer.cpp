#include "../include/Timer.hpp"
#include "../include/CPU.hpp"

Timer::Timer(class CPU* cpu) {
	if ((this->cpu = cpu) == NULL) exit(2);
	this->TimerCounter = 0;
	this->DividerRegister = 0;
}

int Timer::getTimerCounter() {
	return this->TimerCounter;
}

void Timer::enable() {
	this->cpu->memory.WriteByte(ADDR_TMC, this->cpu->memory.ReadByte(ADDR_TMC) | 0b00000100);
}

void Timer::disable() {
	this->cpu->memory.WriteByte(ADDR_TMC, this->cpu->memory.ReadByte(ADDR_TMC) & ~(0b00000100));
}

bool Timer::IsEnabled() const {
	return testBit(this->cpu->memory.ReadByte(ADDR_TMC), 2);
}

int Timer::getClockCycles() const {
	switch (getClockFrequency()) {
		case 0: return 1024; // => frequency = 4096
		case 1: return   16; // => frequency = 262144
		case 2: return   64; // => frequency = 65536
		case 3: return  256; // => frequency = 16384
		default: exit(3);
	}
}

Byte Timer::getClockFrequency() const {
	return this->cpu->memory.ReadByte(ADDR_TMC) & 0b00000011;
}

void Timer::DividerRegisterStep(int cycles) {
	this->DividerRegister += cycles;

	if (this->DividerRegister >= 0xFF) {
		this->DividerRegister = 0x00;
		this->cpu->memory.setDividerRegister(this->cpu->memory.ReadByte(0xFF04) + 1);
	}
}

void Timer::update(int cycles) {
	DividerRegisterStep(cycles);

	if (IsEnabled()) {
		this->TimerCounter += cycles;

		if (getTimerCounter() >= getClockCycles()) {
			this->TimerCounter -= getClockCycles();

			if (this->cpu->memory.ReadByte(ADDR_TIMA) == 0xFF) {
				this->cpu->memory.WriteByte(ADDR_TIMA, this->cpu->memory.ReadByte(ADDR_TMA));
				this->cpu->RequestInterupt(2);
			} else {
				this->cpu->memory.WriteByte(ADDR_TIMA, this->cpu->memory.ReadByte(ADDR_TIMA) + 1);
			}
		}
	}
}