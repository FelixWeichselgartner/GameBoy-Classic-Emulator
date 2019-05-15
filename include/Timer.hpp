#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "datatypes.h"

class CPU;

class Timer {

private:

	int TimerCounter;
	int DividerRegister;

	class CPU* cpu;

public:

	Timer(class CPU*);

	int getTimerCounter();

	void enable();
	void disable();

	bool IsEnabled() const;

	int getClockCycles() const;
	Byte getClockFrequency() const;
	void setClockFrequency();

	void DividerRegisterStep(int);

	void update(int);

};

#endif