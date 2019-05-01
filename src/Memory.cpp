#include "../include/Memory.hpp"
#include "../include/Joypad.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

Memory::Memory() {
	this->registers = NULL;
	this->joypad = NULL;
	this->timer = NULL;
}

Memory::Memory(class Registers* registers, class Timer *timer) {
	if ((this->registers = registers) == NULL) exit(2);
	if ((this->timer = timer) == NULL) exit(2);
}

Memory::~Memory() {

}

void Memory::setJoypadLink(class Joypad* joypad) {
	if ((this->joypad = joypad) == NULL) exit(2);
}

////////////////////////////////////////////////////////////////
// load Byte/Word from Rom.

Byte Memory::LoadByte() {
	this->registers->setPC(this->registers->getPC() + 1);
	return ReadByte(this->registers->getPC());
}

Word Memory::LoadWord() {
	Word retval = 0x0000;
	retval = LoadByte();
	retval |= (LoadByte() << 8);
	return retval;
}

////////////////////////////////////////////////////////////////
// stack operations

void Memory::PushByte(Byte value) {
	this->registers->setSP(this->registers->getSP() - 1);
	WriteByte(this->registers->getSP(), value);
}

Byte Memory::PopByte() {
	Byte retval = ReadByte(this->registers->getSP());
	this->registers->setSP(this->registers->getSP() + 1);
	return retval;
}

void Memory::PushWord(Word value) {
	Byte firstHalf, secondHalf;
	firstHalf = HIGH_BYTE(value);
	secondHalf = LOW_BYTE(value);
	PushByte(firstHalf);
	PushByte(secondHalf);
}

Word Memory::PopWord() {
	Word retval;
	Byte HighHalf, LowHalf;
	LowHalf = PopByte();
	HighHalf = PopByte();
	retval = HighHalf << 8 | LowHalf;
	return retval;
}

////////////////////////////////////////////////////////////////
// dma transfer.

void Memory::DoDMATransfer(Byte data) {
	Word address = data << 8;
	
	for (int i = 0; i < 0xA0; i++) {
		WriteByte(0xFE00 + i, ReadByte(address + i));
	}
}

////////////////////////////////////////////////////////////////
// read & write to memory.

void Memory::WriteWord(Word address, Word value) {
	Byte UpperHalf, LowerHalf;
	UpperHalf = HIGH_BYTE(value);
	LowerHalf = LOW_BYTE(value);
	WriteByte(address, LowerHalf);
	WriteByte(address + 1, UpperHalf);
}

Byte Memory::ReadIORegisters(Word address) {
	// waveform storage for arbitrary sound data, some lcd registers.
	if (address >= 0xFF30 && address <= 0xFF3F || address >= 0xFF42 && address <= 0xFF4B) {
		return this->ram.getMemory(address);
	}

	switch ((Byte)(address & 0x00FF)) {
		// joypad register.
	case 0x00:
		return this->joypad->getJoypadState();

		// SIO control.
	case 0x02:
		return this->ram.getMemory(address) | 0b01111110;

		// divider register (r/w).
	case 0x04:
		return this->ram.getMemory(address);

		// timer counter (r/w).
	case 0x05:
		return this->ram.getMemory(address);

		// timer modulo (r/w).
	case 0x06:
		return this->ram.getMemory(address);

		// timer control.
	case 0x07:
		return this->ram.getMemory(address) | 0b11111000;

		// interrupt flags.
	case 0x0F:
		return this->ram.getMemory(address) | 0b11100000;

		// sound mode 1 register, sweep register (r/w).
	case 0x10:
		return this->ram.getMemory(address) | 0b10000000;

		// sound mode 1 register, sound length/wave pattern duty (r/w).
	case 0x11:
		return this->ram.getMemory(address);

		// sound mode 1 register, envelope (r/w).
	case 0x12:
		return this->ram.getMemory(address);

		// sound mode 1 register, frequency lo (w).
	case 0x13:
		return this->ram.getMemory(address); // maybe 0xff

	// sound mode 1 register, frequency hi (r/w).
	case 0x14:
		return this->ram.getMemory(address) | 0b00111000;

		// sound mode 2 register, sound length; wave pattern duty (r/w).
	case 0x16:
		return this->ram.getMemory(address);

		// sound mode 2 register, envelope (r/w).
	case 0x17:
		return this->ram.getMemory(address);

		// sound mode 2 register, frequency lo data (w).
	case 0x18:
		return this->ram.getMemory(address); //0xff

	// sound mode 2 register, frequency hi data (r/w).
	case 0x19:
		return 0xBF;

		// sound mode 3 register, sound on/off (r/w).
	case 0x1A:
		return this->ram.getMemory(address) | 0b01111111;

		// sound mode 3 register, sound length (r/w).
	case 0x1B:
		return this->ram.getMemory(address);

		// sound mode 3 register, select output.
	case 0x1C:
		return this->ram.getMemory(address) | 0b10011111;

		// sound mode 3 register, frequency's lower data (w).
	case 0x1D:
		return this->ram.getMemory(address); // maybe 0xff

	// sound mode 3 register, frequency's higher data (r/w).
	case 0x1E:
		return this->ram.getMemory(address) | 0b10111111;

		// sound mode 4 register, sound length (r/w);
	case 0x20:
		//return this->ram.getMemory(address) | 0b11000000; this has to be changed later, when sound is implemented
		return 0xFF;

		// sound mode 4 register, envelope (r/w).
	case 0x21:
		return this->ram.getMemory(address);

		// sound mode 4 register, polynomial counter (r/w).
	case 0x22:
		return this->ram.getMemory(address);

		// sound mode 4 register, counter/consecutive; initial (r/w).
	case 0x23:
		return this->ram.getMemory(address) | 0b00111111;

		// channel control / on-off / volume (r/w).
	case 0x24:
		return this->ram.getMemory(address);

		// selection of sound ouput terminal (r/w).
	case 0x25:
		return this->ram.getMemory(address);

		// sound on/off
	case 0x26:
		return this->ram.getMemory(address) | 0b01110000;

		// lcd control.
	case 0x40:
		return this->ram.getMemory(address);

		// lcd status.
	case 0x41:
		return this->ram.getMemory(address) | 0b10000000;

	case 0x72:
		return this->ram.getMemory(address);

	case 0x73:
		return this->ram.getMemory(address);

	case 0x75:
		return this->ram.getMemory(address) | 0x8F;

	case 0x76:
		return 0x00;

	case 0x77:
		return 0x00;

	// return 0xFF if reading from not allowed memory space.
	default:
		return 0xFF;
	}
}

Byte Memory::ReadByte(Word address) {
	// rom memory bank.
	if (address >= ADDR_ROM_1 && address < ADDR_VRAM_T_S) {
		return this->rom.getMemory(address - ADDR_ROM_1 + (this->rom.getCurrentRomBank() * 0x4000));
	}
	// ram memory bank.
	else if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram.getRamEnable())
			return this->ram.getRamBankMemory(address - ADDR_EXT_RAM + (this->ram.getCurrentRamBank() * 0x2000));
		else return 0xFF; //not quite sure if this is correct
	}
	// io registers & high ram.
	else if ((address & 0xFF00) == 0xFF00) {
		if (address >= 0xFF80 && address <= ADDR_INTR_EN) {
			return this->ram.getMemory(address);
		} else {
			return ReadIORegisters(address);
		}
	}
	// normal memory.
	else {
		return this->ram.getMemory(address);
	}
}

void Memory::WriteByte(Word address, Byte value) {
	// rom banking.
	if (address < ADDR_VRAM_T_S) {
		this->rom.HandleBanking(address, value);
		return;
	}
	// ram banking.
	else if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram.getRamEnable())
			this->ram.setRamBankMemory(address - ADDR_EXT_RAM + (this->ram.getCurrentRamBank() * 0x2000), value);
		else
			cout << "didnt write @ address: " << HEX16 << address << " value: " << HEX << (int)value << endl;
	}
	// echo is copy of oam.
	else if ((address >= ADDR_ECHO) && (address < ADDR_OAM)) {
		this->ram.setMemory(address, value);
		this->ram.setMemory(address - 0x2000, value);
	}
	// unused area.
	else if ((address >= ADDR_UNUSABLE) && (address < (ADDR_IO - 1))) {
		return;
	}
	// divider register.
	else if (address == 0xFF04) {
		this->ram.setMemory(address, 0x00);
	}
	// set current line to 0.
	else if (address == 0xFF44) {
		this->ram.setMemory(address, 0x00);
	}
	// do the dma transfer.
	else if (address == 0xFF46) {
		DoDMATransfer(value);
	}
	// timer handling.
	else if (address == ADDR_TMC) {
		Byte currentFrequency = this->timer->getClockFrequency();
		this->ram.setMemory(address, value);
		Byte newFrequency = this->timer->getClockFrequency();

		if (currentFrequency != newFrequency) {
			this->timer->setClockFrequency();
		}
	}
	// return normal memory.
	else {
		this->ram.setMemory(address, value);
	}

	return;
}