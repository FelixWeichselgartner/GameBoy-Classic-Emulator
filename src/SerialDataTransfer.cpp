#include "../include/SerialDataTransfer.hpp"

SerialDataTransfer::SerialDataTransfer() {
	this->MasterDevice = false;
	this->input = this->output = 0x00;
}

SerialDataTransfer::SerialDataTransfer(bool MasterDevice) {
	this->MasterDevice = MasterDevice;
	this->input = this->output = 0x00;
}

void SerialDataTransfer::sendByte() {

}

void SerialDataTransfer::receiveByte() {

}

void SerialDataTransfer::update() {

}