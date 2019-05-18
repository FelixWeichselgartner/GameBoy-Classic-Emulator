#ifndef SERIAL_DATA_TRANSFER_HPP_
#define SERIAL_DATA_TRANSFER_HPP_

#include "datatypes.h"

class SerialDataTransfer {

private:

	bool MasterDevice;
    Byte input, output;

public:

	SerialDataTransfer();
	SerialDataTransfer(bool);

	Byte getInput() const { return this->input; }
	void setInput(Byte input) { this->input = input; }
	Byte getOutput() const { return this->output; }
	void setOutput(Byte output) { this->output = output; }

	// send & receive data from another device.
	void sendByte();
	void receiveByte();

	void update();

};

#endif