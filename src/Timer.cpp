#include "../include/Timer.hpp"
#include "../include/CPU.hpp"

/*
Thanks go to @https://github.com/trekawek.
My timer is inspired by his and saved me a lot of trouble.

////////////////////////////////////////////////////////////////

His license (MIT License):
https://github.com/trekawek/coffee-gb/blob/master/LICENSE

MIT License

Copyright (c) 2017 Tomasz Rękawek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

////////////////////////////////////////////////////////////////

His source code:
https://github.com/trekawek/coffee-gb/blob/master/src/main/java/eu/rekawek/coffeegb/timer/Timer.java
*/

// private methods.
void Timer::incTima() {
    this->tima++;
    this->tima %= 0x100;
    if (this->tima == 0) {
        this->TimerOverflowFlag = true;
        this->TicksSinceOverflow = 0;
    }
}

// public methods.

Timer::Timer(class CPU* cpu) {
	if ((this->cpu = cpu) == NULL) exit(2);
	this->TimerOverflowFlag = false;
	this->TicksSinceOverflow = 0;
	this->div = this->tima = this->tma = this->tac = 0x00;
	this->previousBit = true;
}

void Timer::setTima(Byte tima) { 
	if (this->TicksSinceOverflow < 5) {
		this->tima = tima;
		this->TimerOverflowFlag = false;
		this->TicksSinceOverflow = 0;
	}
}

void Timer::updateDiv(int div) {
    this->div = div;
    int bitPos = FreqBitPos[this->tac & 0b11];
    //bitPos <<= speedMode.getSpeedMode() - 1;
    bool bit = (this->div & (1 << bitPos)) != 0;
    bit &= (this->tac & (1 << 2)) != 0;
    if (!bit && this->previousBit) {
        incTima();
    }
    this->previousBit = bit;
}

void Timer::tick() {
    updateDiv((this->div + 1) & 0xFFFF);
    if (this->TimerOverflowFlag) {
        this->TicksSinceOverflow++;
		
        switch(this->TicksSinceOverflow) {
            case 4:
                cpu->RequestInterrupt(2);
                break;
            case 5:
                this->tima = this->tma;
                break;
            case 6:
                this->tima = this->tma;
                this->TimerOverflowFlag = false;
                this->TicksSinceOverflow = 0;
                break;
        }
    }
}

void Timer::update(int cycles) {
	for (int i = 0; i < cycles; i++) {
		tick();
	}
}