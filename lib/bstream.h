#ifndef BSTREAM_H
#define BSTREAM_H

#include <iostream>
#include <fstream>
#include <bitset>

using std::fstream;

class bstream : public fstream {
    public:
        bstream(const char *fname, ios_base::openmode mode) : fstream(fname, mode) {  };
        ~bstream();
        int writeBit(uint8_t value);
        int writeNBits(uint32_t value, int num);
        uint8_t readBit();
        uint32_t readNBits(int num);
        int grantWrite();
        void resetBitPointers(bool readOperation) {
            if (!readOperation) {
                bits = 0;
            }
            position = 7;
        }
    private:
        uint8_t bits = 0;
        int position = 7;
        int byteCount;
        bool readWasNotUsed = true;
};

#endif  