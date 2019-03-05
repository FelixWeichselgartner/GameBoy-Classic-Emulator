// this includes the datatype byte - which is basically an unsigned char
#include <cstddef>
using namespace std;

class RAM {

private:

    byte memory[65536];

public:

    void setByte(unsigned short, byte);
    byte getByte(unsigned short);

};