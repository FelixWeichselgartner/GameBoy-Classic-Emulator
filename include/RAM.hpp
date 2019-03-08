#ifndef RAM_HPP_
#define RAM_HPP_

//----------------------------------------------------------------------------------------------
typedef unsigned char Byte;
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Memory Map:
#define ADDR_ROM_0		0x0000
#define ADDR_ROM_SWT	0x4000
#define ADDR_VRAM		0x8000
#define ADDR_RAM_SWT	0xA000
#define ADDR_INT_RAM_1	0xC000
#define ADDR_ECHO_RAM	0xE000
#define ADDR_OAM		0xFE00
#define ADDR_EMPTY_1	0xFEA0
#define ADDR_IO			0xFF00
#define ADDR_EMPTY_2	0xFF4C
#define ADDR_INT_RAM_2	0xFF80
#define ADDR_INTR_EN	0xFFFF
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