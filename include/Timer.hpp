#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "CPU.hpp"

class Timer {

private:

	int TimerCounter;
	int DividerRegister;

	class CPU* cpuLink;

public:

	Timer(class CPU*);

	int getTimerCounter();

	void enable();

	void disable();

	bool IsEnabled() const;

	Byte getClockFrequency() const;

	void setClockFrequency();

	void DividerRegisterStep(int);

	void update(int);

};

#endif