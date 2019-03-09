#ifndef RAM_HPP_
#define RAM_HPP_

//----------------------------------------------------------------------------------------------
typedef unsigned char Byte;
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Memory Map:
#define ADDR_ROM_0			0x0000		// 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
#define ADDR_ROM_1			0x4000		// 16KB ROM Bank 01...NN (in cartridge, witchable bank number)
#define ADDR_VRAM_T_S		0x8000		// VRAM: Tiles and Sprites
#define ADDR_VRAM_T_A		0x9000		// VRAM: Tiles Alt
#define ADDR_VRAM_T_M_1		0x9800		// VRAM: Tilemap 1
#define ADDR_VRAM_T_M_2		0x9C00		// VRAM: Tilemap 2
#define ADDR_EXT_RAM		0xA000		// 8KB External RAM (in cartridge, witchable bank, if any)
#define ADDR_INT_RAM_1		0xC000		// 4KB Work RAM Bank 0 (WRAM)
#define ADDR_INT_RAM_2		0xD000		// 4KB Work RAM Bank 1 (WRAM) (switchable bank 1-7 in CGB Mode)
#define ADDR_ECHO			0xE000		// Same as C000-DDFF (ECHO) (typically not used)
#define ADDR_OAM			0xFE00		// Sprite Attribute Table (OAM) (Can't change during screen redraw)
#define ADDR_UNUSABLE		0xFEA0		// Not Usable
#define ADDR_IO				0xFF00		// I/O Ports
#define ADDR_STACK			0xFF80		// High RAM (HRAM) (Stack)
#define ADDR_INTR_EN		0xFFFF		// Interupt Enable Register
//----------------------------------------------------------------------------------------------

class RAM {

private:

    Byte memory[65536];

public:

    RAM();

    void setMemory(unsigned short, Byte);
    Byte getMemory(unsigned short) const;

};
//----------------------------------------------------------------------------------------------

#endif