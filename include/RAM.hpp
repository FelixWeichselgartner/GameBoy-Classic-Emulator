// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

class RAM {

private:

    byte memory[65536];

public:

    void setMemory(unsigned short, byte);
    byte getMemory(unsigned short);

};