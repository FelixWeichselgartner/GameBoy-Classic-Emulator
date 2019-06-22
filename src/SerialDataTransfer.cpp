#include "../include/SerialDataTransfer.hpp"

void SerialDataTransfer::reset() {
    this->input = this->output = 0x00;
}

SerialDataTransfer::SerialDataTransfer() {
	this->MasterDevice = false;
    reset();
}

SerialDataTransfer::SerialDataTransfer(bool MasterDevice) {
	this->MasterDevice = MasterDevice;
    reset();
}

void SerialDataTransfer::sendByte() {

}

void SerialDataTransfer::receiveByte() {

}

void SerialDataTransfer::update() {

}