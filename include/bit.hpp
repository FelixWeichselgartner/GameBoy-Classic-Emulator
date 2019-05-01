#ifndef BIT_HPP_
#define BIT_HPP_

#include "datatypes.h"

//----------------------------------------------------------------------------------------------
// split a 16-bit number in 2 x 8-bit numbers to save to memory
#define LOW_BYTE(x)				(x & 0xFF)
#define HIGH_BYTE(x)			((x >> 8) & 0xFF)
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// split an 8-bit number in 2 x 4 bit
#define LOWER_HALFBYTE(x)       (x & 0b00001111)
#define UPPER_HALFBYTE(x)		((x >> 4) & 0b00001111)
//----------------------------------------------------------------------------------------------

bool testBit(Byte, int);

Byte resetBit(Byte, int);

Byte setBit(Byte, int);

Byte flipBit(Byte, int);

#endif