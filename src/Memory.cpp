#include "../include/Memory.hpp"
#include "../include/Joypad.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

////////////////////////////////////////////////////////////////
// private read/write utility functions.

Byte Memory::ReadROM(Word address) {
	if (address < ADDR_ROM_1) {
		return this->rom.getMemory(address);
	} else {
		return this->rom.getMemory(address - ADDR_ROM_1 + (this->rom.getCurrentRomBank() * 0x4000));
	}
}

Byte Memory::ReadVRAM(Word address) {
	return vram[address];
}

Byte Memory::ReadRAM(Word address) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram.getRamEnable())
			return this->ram.getRamBankMemory(address - ADDR_EXT_RAM + (this->ram.getCurrentRamBank() * 0x2000));
		else return 0xFF; //not quite sure if this is correct
	} else {
		return this->ram.getMemory(address - ADDR_INT_RAM_1);
	}
}

Byte Memory::ReadECHO(Word address) {
	return echo[address];
}

Byte Memory::ReadOAM(Word address) {
	return oam[address];
}

Byte Memory::ReadUNUSABLE(Word) {
	return 0xFF;
}

Byte Memory::ReadIO(Word WordAddress) {
	Byte address = (Byte)(WordAddress & 0x00FF);
	// waveform storage for arbitrary sound data, some lcd registers.
	if (WordAddress >= 0xFF30 && WordAddress <= 0xFF3F || WordAddress >= 0xFF42 && WordAddress <= 0xFF4B) {
		return io[address];
	}

	switch (address) {
		// joypad register.
		case 0x00:
			return this->joypad->getJoypadState();

		// SIO control.
		case 0x02:
			return io[address] | 0b01111110;

		// divider register (r/w).
		case 0x04:
			return io[address];

		// timer counter (r/w).
		case 0x05:
			return io[address];

		// timer modulo (r/w).
		case 0x06:
			return io[address];

		// timer control.
		case 0x07:
			return io[address] | 0b11111000;

		// interrupt flags.
		case 0x0F:
			return io[address] | 0b11100000;

		// sound mode 1 register, sweep register (r/w).
		case 0x10:
			return io[address] | 0b10000000;

		// sound mode 1 register, sound length/wave pattern duty (r/w).
		case 0x11:
			return io[address];

		// sound mode 1 register, envelope (r/w).
		case 0x12:
			return io[address];

		// sound mode 1 register, frequency lo (w).
		case 0x13:
			return io[address]; // maybe 0xff

		// sound mode 1 register, frequency hi (r/w).
		case 0x14:
			return io[address] | 0b00111000;

		// sound mode 2 register, sound length; wave pattern duty (r/w).
		case 0x16:
			return io[address];

		// sound mode 2 register, envelope (r/w).
		case 0x17:
			return io[address];

		// sound mode 2 register, frequency lo data (w).
		case 0x18:
			return io[address]; //0xff

		// sound mode 2 register, frequency hi data (r/w).
		case 0x19:
			return 0xBF;

		// sound mode 3 register, sound on/off (r/w).
		case 0x1A:
			return io[address] | 0b01111111;

		// sound mode 3 register, sound length (r/w).
		case 0x1B:
			return io[address];

		// sound mode 3 register, select output.
		case 0x1C:
			return io[address] | 0b10011111;

		// sound mode 3 register, frequency's lower data (w).
		case 0x1D:
			return io[address]; // maybe 0xff

		// sound mode 3 register, frequency's higher data (r/w).
		case 0x1E:
			return io[address] | 0b10111111;

		// sound mode 4 register, sound length (r/w);
		case 0x20:
			//return this->ram.getMemory(address) | 0b11000000; this has to be changed later, when sound is implemented
			return 0xFF;

		// sound mode 4 register, envelope (r/w).
		case 0x21:
			return io[address];

		// sound mode 4 register, polynomial counter (r/w).
		case 0x22:
			return io[address];

		// sound mode 4 register, counter/consecutive; initial (r/w).
		case 0x23:
			return io[address] | 0b00111111;

		// channel control / on-off / volume (r/w).
		case 0x24:
			return io[address];

		// selection of sound ouput terminal (r/w).
		case 0x25:
			return io[address];

		// sound on/off
		case 0x26:
			return io[address] | 0b01110000;

		// lcd control.
		case 0x40:
			return io[address];

		// lcd status.
		case 0x41:
			return io[address] | 0b10000000;

		// scanline.
		case 0x44:
			cout << (int)io[address] << endl;
			return io[address];

		case 0x72:
			return io[address];

		case 0x73:
			return io[address];

		case 0x75:
			return io[address] | 0x8F;

		case 0x76:
			return 0x00;

		case 0x77:
			return 0x00;

		// return 0xFF if reading from not allowed memory space.
		default:
			return 0xFF;
	}
}

Byte Memory::ReadHRAM(Word address) {
	return hram[address];
}

void Memory::WriteROM(Word address, Byte value) {
	this->rom.HandleBanking(address, value);
}

void Memory::WriteVRAM(Word address, Byte value) {
	vram[address] = value;
}

void Memory::WriteRAM(Word address, Byte value) {
	if (address >= ADDR_EXT_RAM && address < ADDR_INT_RAM_1) {
		if (this->ram.getRamEnable())
			this->ram.setRamBankMemory(address - ADDR_EXT_RAM + (this->ram.getCurrentRamBank() * 0x2000), value);
		else
			cout << "didnt write @ address: " << HEX16 << address << " value: " << HEX << (int)value << endl;
	} else {
		ram.setMemory(address - ADDR_INT_RAM_1, value);
	}
}

void Memory::WriteECHO(Word address, Byte value) {
	echo[address - 0x2000 - ADDR_OAM] = value;	
}

void Memory::WriteOAM(Word address, Byte value) {
	oam[address] = value;
	
}

void Memory::WriteUNUSABLE(Word, Byte) {
	return;
}

void Memory::WriteIO(Word address, Byte value) {
	Word setAddress = address - 0xFF00;
	Byte currentFrequency, newFrequency;

	// divider register.
	if (address == 0xFF04) {
		io[setAddress] = 0x00;
	}
	// set current line to 0.
	else if (address == 0xFF44) {
		io[setAddress] = 0x00;
	}
	// do the dma transfer.
	else if (address == 0xFF46) {
		DoDMATransfer(value);
	}
	// timer handling.
	else if (address == ADDR_TMC) {
		currentFrequency = this->timer->getClockFrequency();
		io[setAddress] = value;
		newFrequency = this->timer->getClockFrequency();

		if (currentFrequency != newFrequency) {
			this->timer->setClockFrequency();
		}
	}
	// other io registers.
	else {
		io[setAddress] = value;
	}
}

void Memory::WriteHRAM(Word address, Byte value) {
	hram[address] = value;
}

////////////////////////////////////////////////////////////////
// constructor and destructor.

Memory::Memory() {
	this->registers = NULL;
	this->joypad = NULL;
	this->timer = NULL;
	InitMemory();
}

// this->registers gets changed somehow
int i = 0;

Memory::Memory(class Registers* registers, class Timer *timer) {
	if ((this->registers = registers) == NULL) exit(2);
	if ((this->timer = timer) == NULL) exit(2);
	if (i == 0) {
		this->reg = this->registers;
		i++;
	}
}

Memory::~Memory() {

}

////////////////////////////////////////////////////////////////
// initialise memory.

void Memory::InitMemory() {
	io[0xFF05 - 0xFF00] = 0x00;
	io[0xFF06 - 0xFF00] = 0x00;
	io[0xFF07 - 0xFF00] = 0x00;
	io[0xFF10 - 0xFF00] = 0x80;
	io[0xFF11 - 0xFF00] = 0xBF;
	io[0xFF12 - 0xFF00] = 0xF3;
	io[0xFF14 - 0xFF00] = 0xBF;
	io[0xFF16 - 0xFF00] = 0x3F;
	io[0xFF17 - 0xFF00] = 0x00;
	io[0xFF19 - 0xFF00] = 0xBF;
	io[0xFF1A - 0xFF00] = 0x7F;
	io[0xFF1B - 0xFF00] = 0xFF;
	io[0xFF1C - 0xFF00] = 0x9F;
	io[0xFF1E - 0xFF00] = 0xBF;
	io[0xFF20 - 0xFF00] = 0xFF;
	io[0xFF21 - 0xFF00] = 0x00;
	io[0xFF22 - 0xFF00] = 0x00;
	io[0xFF23 - 0xFF00] = 0xBF;
	io[0xFF24 - 0xFF00] = 0x77;
	io[0xFF25 - 0xFF00] = 0xF3;
	io[0xFF26 - 0xFF00] = 0xF1;
	io[0xFF40 - 0xFF00] = 0x91;
	io[0xFF42 - 0xFF00] = 0x00;
	io[0xFF43 - 0xFF00] = 0x00;
	io[0xFF45 - 0xFF00] = 0x00;
	io[0xFF47 - 0xFF00] = 0xFC;
	io[0xFF48 - 0xFF00] = 0xFF;
	io[0xFF49 - 0xFF00] = 0xFF;
	io[0xFF4A - 0xFF00] = 0x00;
	io[0xFF4B - 0xFF00] = 0x00;
	interrupt_enable_register = 0x00;
}

////////////////////////////////////////////////////////////////
// pointer to joypad and joypad request.

void Memory::setJoypadLink(class Joypad* joypad) {
	if ((this->joypad = joypad) == NULL) exit(2);
}

Byte Memory::getJoypadRequest() {
	return io[0x0000];
}

////////////////////////////////////////////////////////////////
// direct access to scanline & divider register.

void Memory::setScanline(Byte scanline) {
	io[0x0044] = scanline;
}

void Memory::setDividerRegister(Byte reg) {
	io[0x0004] = reg;
}

////////////////////////////////////////////////////////////////
// load Byte/Word from Rom.

Byte Memory::LoadByte() {
	if (reg != this->registers) {
		cout << "it happened" << endl;
	}
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
// stack operations,

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

Byte Memory::ReadByte(Word address) {
	// rom memory bank.
	if (address < ADDR_VRAM_T_S) {
		return ReadROM(address);
	}
	// vram.
	else if (address >= ADDR_VRAM_T_S && address < ADDR_EXT_RAM) {
		return ReadVRAM(address - ADDR_VRAM_T_S);
	}
	// ram memory bank.
	else if (address >= ADDR_EXT_RAM && address < ADDR_ECHO) {
		return ReadRAM(address);
	}
	// echo memory.
	else if (address >= ADDR_ECHO && address < ADDR_OAM) {
		return ReadECHO(address - ADDR_ECHO);
	}
	// oam.
	else if (address >= ADDR_OAM && address < ADDR_UNUSABLE) {
		return ReadOAM(address - ADDR_OAM);
	}
	// unuseable memory.
	else if (address >= ADDR_UNUSABLE && address < ADDR_IO) {
		return ReadUNUSABLE(address - ADDR_UNUSABLE);
	}
	// io registers
	else if (address >= ADDR_IO && address < ADDR_HRAM) {
		return ReadIO(address);
	} 
	// high ram.
	else if (address >= ADDR_HRAM && address < ADDR_INTR_EN) {
		return ReadHRAM(address - ADDR_HRAM);
	} 
	// interrupt enable register.
	else if (address == ADDR_INTR_EN) {
		return interrupt_enable_register;
	}
	else {
		cout << "address: " << (int)address << endl;
	}
}

void Memory::WriteByte(Word address, Byte value) {
	// rom banking.
	if (address < ADDR_VRAM_T_S) {
		WriteROM(address, value);
	}
	// vram.
	else if (address >= ADDR_VRAM_T_S && address < ADDR_EXT_RAM) {
		WriteVRAM(address - ADDR_VRAM_T_S, value);
	}
	// ram banking.
	else if (address >= ADDR_EXT_RAM && address < ADDR_ECHO) {
		WriteRAM(address, value);
	}
	// echo is copy of oam.
	else if ((address >= ADDR_ECHO) && (address < ADDR_OAM)) {
		WriteECHO(address - ADDR_ECHO, value);
		WriteOAM(address - ADDR_OAM, value);
	}
	// oam.
	else if (address >= ADDR_OAM && address < ADDR_UNUSABLE) {
		WriteOAM(address - ADDR_OAM, value);
	}
	// unused area.
	else if (address >= ADDR_UNUSABLE && address < ADDR_IO) {
		WriteUNUSABLE(address - ADDR_UNUSABLE, value);
	}
	// io registers.
	else if (address >= ADDR_IO && address < ADDR_HRAM) {
		WriteIO(address, value);
	}
	// hram.
	else if (address >= ADDR_HRAM && address < ADDR_INTR_EN) {
		WriteHRAM(address - ADDR_HRAM, value);
	}
	// interrupt enable register.
	else if (address == ADDR_INTR_EN) {
		interrupt_enable_register = value;
	}

	return;
}

void Memory::print(Word start, Word end) {
	cout << "Hexdump: " << endl;
	cout << "          ";
	for (int i = 0; i < 16; i++) {
		cout << HEX << i << " ";
	}
	cout << endl;
	for (int i = start, c = 0; i < start + (end + 1 - start) / 16; i++, c++) {
		cout << HEX16 << setw(8) << start + c * 16 << ": ";
		for (int k = 0; k < 16; k++) {
			cout << HEX << (int)ReadByte(start + c * 16 + k) << " ";
		}
		cout << endl;
	}
}