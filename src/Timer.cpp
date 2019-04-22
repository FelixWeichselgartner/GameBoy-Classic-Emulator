#include "../include/Timer.hpp"

Timer::Timer(class CPU* cpuLink) {
	this->cpuLink = cpuLink;
	this->TimerCounter = 1024;
	this->DividerRegister = 0;
}

int Timer::getTimerCounter() {
	return this->TimerCounter;
}

void Timer::enable() {
	cpuLink->WriteByte(ADDR_TMC, cpuLink->ReadByte(ADDR_TMC) | 0b00000100);
}

void Timer::disable() {
	cpuLink->WriteByte(ADDR_TMC, cpuLink->ReadByte(ADDR_TMC) & ~(0b00000100));
}

bool Timer::IsEnabled() const {
	return testBit(cpuLink->ReadByte(ADDR_TMC), 2);
}

Byte Timer::getClockFrequency() const {
	return cpuLink->ReadByte(ADDR_TMC) & 0b00000011;
}

void Timer::setClockFrequency() {
	switch (getClockFrequency()) {
		case 0: this->TimerCounter = 1024; break; // => frequency = 4096
		case 1: this->TimerCounter = 16;   break; // => frequency = 262144
		case 2: this->TimerCounter = 64;   break; // => frequency = 65536
		case 3: this->TimerCounter = 256;  break; // => frequency = 16382
	}
}

void Timer::DividerRegisterStep(int cycles) {
	this->DividerRegister += cycles;

	if (this->DividerRegister >= 0xFF) {
		this->DividerRegister = 0x00;
		cpuLink->ram.setMemory(0xFF04, cpuLink->ram.getMemory(0xFF04) + 1);
	}
}

void Timer::update(int cycles) {
	DividerRegisterStep(cycles);

	if (IsEnabled()) {
		this->TimerCounter -= cycles;

		if (getTimerCounter() <= 0) {
			setClockFrequency();

			if (cpuLink->ReadByte(ADDR_TIMA) == 0xFF) {
				cpuLink->WriteByte(ADDR_TIMA, cpuLink->ReadByte(ADDR_TMA));
				cpuLink->RequestInterupt(2);
			} else {
				cpuLink->WriteByte(ADDR_TIMA, cpuLink->ReadByte(ADDR_TIMA) + 1);
			}
		}
	}
}