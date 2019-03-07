#ifndef RAM_HPP_
#define RAM_HPP_

//----------------------------------------------------------------------------------------------
typedef unsigned char Byte;
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