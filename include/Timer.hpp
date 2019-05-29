#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "datatypes.h"

class CPU;

class Timer {

private:

	// timer registers.
	int div;
	Byte tima, tma, tac;

	// supporting variables.
	bool TimerOverflowFlag, previousBit;
	int TicksSinceOverflow;
	int FreqBitPos[4] = { 9, 3, 5, 7 };

	class CPU* cpu;

	// private methods.
	void updateDiv(int);
	void incTima();
	void tick();

public:

	Timer(class CPU*);

	// getter and setter.
	Byte getDiv() const { return this->div >> 8; }
	Byte getTima() const { return this->tima; }
	Byte getTma() const { return this->tma; }
	Byte getTac() const { return this->tac | 0b11111000; }
	void setDiv(Byte div) { updateDiv(0); }
	void setTima(Byte);
	void setTma(Byte tma) { this->tma = tma; }
	void setTac(Byte tac) { this->tac = tac; }

	// update the timer.
	void update(int);

};

#endif