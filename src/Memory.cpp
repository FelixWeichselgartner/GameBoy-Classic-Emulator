#include "../include/Memory.hpp"
#include "../include/Joypad.hpp"
#include "../include/GPU.hpp"

#include <iostream>
#include <iomanip>
using namespace std;

////////////////////////////////////////////////////////////////
// private read/write utility functions.

Byte Memory::ReadVRAM(Word address) {
	return vram[address];
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
	if (WordAddress >= 0xFF30 && WordAddress <= 0xFF3F || WordAddress >= 0xFF45 && WordAddress <= 0xFF4B || WordAddress == 0xFF42 || WordAddress == 0xFF43) {
		return io[address];
	}

	//if (address == 0x0044) cout << "address:" << HEX16 << (int)address << endl;
	switch (address) {
		// joypad register.
		case 0x00:
			return this->joypad->getJoypadState();

		// serial data transfer.
		case 0x01:
			return sdt.getInput();

		// serial transfer control.
		case 0x02:
			return io[address] | 0b01111110;

		// divider register (r/w).
		case 0x04:
			return this->timer->getDiv();

		// timer counter (r/w).
		case 0x05:
			return this->timer->getTima();

		// timer modulo (r/w).
		case 0x06:
			return this->timer->getTma();

		// timer control.
		case 0x07:
			return this->timer->getTac();

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

void Memory::WriteVRAM(Word address, Byte value) {
	vram[address] = value;
}

void Memory::WriteECHO(Word address, Byte value) {
	echo[address] = value;	
}

void Memory::WriteOAM(Word address, Byte value) {
	oam[address] = value;
	
}

void Memory::WriteUNUSABLE(Word, Byte) {
	return;
}

void Memory::WriteIO(Word address, Byte value) {
	Word setAddress = address - 0xFF00;

	/* serial data transfer.
	http://gbdev.gg8.se/wiki/articles/Serial_Data_Transfer_(Link_Cable)
	Bit 7 - Transfer Start Flag(0 = No transfer is in progress or requested, 1 = Transfer in progress, or requested)
	Bit 1 - Clock Speed(0 = Normal, 1 = Fast) * *CGB Mode Only * *
	Bit 0 - Shift Clock(0 = External Clock, 1 = Internal Clock) */
	if (address == 0xFF00) {
		sdt.setOutput(value);
	}
	// timer registers.
	if (address == 0xFF04) {
		this->timer->setDiv(0x00);
	}
	else if (address == 0xFF05) {
		this->timer->setTima(value);
	}
	else if (address == 0xFF06) {
		this->timer->setTma(value);
	}
	else if (address == 0xFF07) {
		this->timer->setTac(value);
	}
	// set current line to 0.
	else if (address == 0xFF44) {
		io[setAddress] = 0x00;
	}
	// do the dma transfer.
	else if (address == 0xFF46) {
		DoDMATransfer(value);
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

Memory::Memory(class Registers* registers, class Timer *timer) {
	if ((this->registers = registers) == NULL) exit(2);
	if ((this->timer = timer) == NULL) exit(2);
	this->joypad = NULL;
	InitMemory();
	this->rom.load();
	InitialiseMemoryBanking();
}

Memory::~Memory() {
	delete mbc;
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
// enable bootstrap getter & setter.

bool Memory::getEnableBootstrap() const {
	return this->EnableBootstrap;
}

void Memory::setEnableBootstrap(bool EnableBootstrap) {
	this->EnableBootstrap = EnableBootstrap;
}

////////////////////////////////////////////////////////////////
// load Byte/Word from Rom.

Byte Memory::LoadByte() {
	this->registers->incPC();
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
	this->registers->decSP();
	WriteByte(this->registers->getSP(), value);
}

Byte Memory::PopByte() {
	Byte retval = ReadByte(this->registers->getSP());
	this->registers->incSP();
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
		return mbc->ReadROM(address);
	}
	// vram.
	else if (address >= ADDR_VRAM_T_S && address < ADDR_EXT_RAM) {
		return ReadVRAM(address - ADDR_VRAM_T_S);
	}
	// ram memory bank.
	else if (address >= ADDR_EXT_RAM && address < ADDR_ECHO) {
		return mbc->ReadRAM(address);
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
		//if (address == 0xFF44) cout << "well" << endl;
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
	// this should never occour.
	else {
		cout << "address: " << (int)address << endl;
		return 0xFF;
	}
}

void Memory::WriteByte(Word address, Byte value) {
	// rom banking.
	if (address < ADDR_VRAM_T_S) {
		mbc->WriteROM(address, value);
	}
	// vram.
	else if (address >= ADDR_VRAM_T_S && address < ADDR_EXT_RAM) {
		WriteVRAM(address - ADDR_VRAM_T_S, value);
	}
	// ram banking.
	else if (address >= ADDR_EXT_RAM && address < ADDR_ECHO) {
		mbc->WriteRAM(address, value);
	}
	// echo is copy of oam.
	else if ((address >= ADDR_ECHO) && (address < ADDR_OAM)) {
		WriteECHO(address - ADDR_ECHO, value);
		WriteOAM(address - ADDR_ECHO, value);
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
	// this should never occour.
	else {
		cout << "address: " << (int)address << endl;
	}

	return;
}

void Memory::print(Word start, Word end) {
	char content;
	cout << endl << "Hexdump:  ";
	for (int i = 0; i < 16; i++) {
		cout << HEX << i << " ";
	}
	cout << endl;
	for (int i = start, c = 0; i < start + (end + 1 - start) / 16; i++, c++) {
		cout << HEX16 << setw(8) << start + c * 16 << ": ";
		for (int k = 0; k < 16; k++) {
			cout << HEX << (int)ReadByte(start + c * 16 + k) << " ";
		}
		cout << "\t";
		for (int k = 0; k < 16; k++) {
			content = ReadByte(start + c * 16 + k);
			if (content > 31 && content < 127) {
				cout << content;
			} else {
				cout << ".";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void NotSupported() {
	cout << "this mode is currently not supported." << endl;
	exit(5);
}

////////////////////////////////////////////////////////////////
// initialise mbc

void Memory::InitialiseMemoryBanking() {
	// memory bank controller modes:
	// http://gbdev.gg8.se/wiki/articles/The_Cartridge_Header
	// http://gbdev.gg8.se/wiki/articles/Memory_Bank_Controllers

	switch (this->rom.getMemory(0x0147)) {
		// 00h  ROM ONLY
		case 0x00:
			this->MemoryBankingMode = 0;
			break;
		// 01h  MBC1
		case 0x01: 
			this->MemoryBankingMode = 1;
			break;
		// 02h  MBC1+RAM
		case 2: 
			this->MemoryBankingMode = 1;
			break;
		// 03h  MBC1+RAM+BATTERY
		case 3: 
			this->MemoryBankingMode = 1;
			break;
		// 05h  MBC2
		case 5: 
			this->MemoryBankingMode = 2;
			break;
		// 06h  MBC2+BATTERY
		case 0x06:
			this->MemoryBankingMode = 2;
			break;
		// 08h  ROM+RAM
		// 09h  ROM+RAM+BATTERY
		// 0Bh  MMM01
		// 0Ch  MMM01+RAM
		// 0Dh  MMM01+RAM+BATTERY
		// 0Fh  MBC3+TIMER+BATTERY
		case 0x0F:
			this->MemoryBankingMode = 3;
			break;
		// 10h  MBC3+TIMER+RAM+BATTERY
		case 0x10:
			this->MemoryBankingMode = 3;
			break;
		// 11h  MBC3
		case 0x11:
			this->MemoryBankingMode = 3;
			break;
		// 12h  MBC3+RAM
		case 0x12:
			this->MemoryBankingMode = 3;
			break;
		// 13h  MBC3+RAM+BATTERY
		case 0x13:
			this->MemoryBankingMode = 3;
			break;
		// 19h  MBC5
		case 0x19:
			this->MemoryBankingMode = 5;
			break;
		// 1Ah  MBC5+RAM
		case 0x1A:
			this->MemoryBankingMode = 5;
			break;
	    // 1Bh  MBC5+RAM+BATTERY
		case 0x1B:
			this->MemoryBankingMode = 5;
			break;
	    // 1Ch  MBC5+RUMBLE
		case 0x1C:
			this->MemoryBankingMode = 5;
			break;
	    // 1Dh  MBC5+RUMBLE+RAM
		case 0x1D:
			this->MemoryBankingMode = 5;
			break;
	    // 1Eh  MBC5+RUMBLE+RAM+BATTERY
		case 0x1E:
			this->MemoryBankingMode = 5;
			break;
	    // 20h  MBC6
	    // 22h  MBC7+SENSOR+RUMBLE+RAM+BATTERY
	   	// FCh  POCKET CAMERA
	    // FDh  BANDAI TAMA5
	    // FEh  HuC3
	    // FFh  HuC1+RAM+BATTERY
		default: 
			cout << "The Mode " << HEX << (int)rom.getMemory(0x0147) << "h is currently not supported" << endl;
			exit(1);
			break;
	}

	switch (this->MemoryBankingMode) {
		case 0x00:
			cout << "Game uses no Memory Banking." << endl;
			if ((this->mbc = new MBC(&rom, &ram)) == NULL) exit(3);
			break;
		case 0x01:
			cout << "Game uses MBC1." << endl;
			if ((this->mbc = new MBC_1(&rom, &ram)) == NULL) exit(3);
			break;
		case 0x02:
			cout << "Game uses MBC2." << endl;
			NotSupported();
			break;
		case 0x03:
			cout << "Game uses MBC3." << endl;
			NotSupported();
			break;
		case 0x05:
			cout << "Game uses MBC5." << endl;
			NotSupported();
			break;
		default:
			cout << "Memory Banking Mode is undefined." << endl;
			NotSupported();
			break;
	}

	bool presumeMultiMBC1 = ((MemoryBankingMode == 1)
		&& (this->rom.getMemory(0x0149) == 0)
		&& (this->rom.getMemory(0x0148) == 0x05));

	if (presumeMultiMBC1) {
		cout << "this MBC1 type is Multi and not supported." << endl;
		exit(7);
	}

	cout << "ROM Size: ";
	switch (this->rom.getMemory(0x0148)) {
		case 0x00: cout << "32KByte(no ROM banking)" << endl; break;
		case 0x01: cout << "64KByte(4 banks)" << endl; break;
		case 0x02: cout << "128KByte(8 banks)" << endl; break;
		case 0x03: cout << "256KByte(16 banks)" << endl; break;
		case 0x04: cout << "512KByte(32 banks)" << endl; break;
		case 0x05: cout << "1MByte(64 banks) - only 63 banks used by MBC1" << endl; break;
		case 0x06: cout << "2MByte(128 banks) - only 125 banks used by MBC1" << endl; break;
		case 0x07: cout << "4MByte(256 banks)" << endl; break;
		case 0x08: cout << "8MByte(512 banks)" << endl; break;
		case 0x52: cout << "1.1MByte(72 banks)" << endl; break;
		case 0x53: cout << "1.2MByte(80 banks)" << endl; break;
		case 0x54: cout << "1.5MByte(96 banks)" << endl; break;
	}

	int AmountRamBanks = 0;
	cout << "RAM Size: ";
	switch (this->rom.getMemory(0x0149)) {
		case 0x00: 
			cout << "None" << endl; 
			break;
		case 0x01:
			AmountRamBanks = 1;
			cout << "2 KBytes" << endl; 
			break;
		case 0x02: 
			AmountRamBanks = 1;
			cout << "8 Kbytes" << endl; 
			break;
		case 0x03: 
			AmountRamBanks = 4;
			cout << "32 KBytes(4 banks of 8KBytes each)" << endl; 
			break;
		case 0x04:
			AmountRamBanks = 16;
			cout << "128 KBytes(16 banks of 8KBytes each)" << endl; 
			break;
		case 0x05: 
			AmountRamBanks = 8;
			cout << "64 KBytes(8 banks of 8KBytes each)" << endl; 
			break;
	}

	cout << "reserving 0x" << HEX16 << AmountRamBanks * 0x2000 << " for RamBankMemory." << endl;
	this->ram.reserveRamBankMemory(AmountRamBanks);	
}