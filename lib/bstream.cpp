#include "bstream.h"
using namespace std;

/**
 * @brief Writes a bit (the value's LSB) onto the file.
 * 
 * @param value the value containing the bit to assign (its LSB).
 * @return int {@code 0} if method runs successfully; otherwise the error code.
 */
int bstream::writeBit(uint8_t value) {
    bits |= (value & 0x01) << position;             // passes to the next position (one step closer to LSB)
    if (position-- >= 1) {                          // if the position is valid, then
        return 0;                                   //     exit successfully.
    }                                               //
    write((char*)&bits, 1);                         // we write the completed byte (TODO)
    resetBitPointers(false);                        // we reset the bits to 0 and the position back to 7
    return 0;                                       // and then exit successfully.
}

/**
 * @brief Writes n bits (the value's LSB) from a certain position onto the file.
 * 
 * @param value the value containing the bit to assign (its LSB).
 * @param pos bit position from which to start writting.
 * @return int {@code 0} if method runs successfully; otherwise the error code.
 */
int bstream::writeNBits(uint8_t value, int num){
    for (int i = num-1; i >= 0; i--) {
        writeBit((value >> i) & 0x01);
    }
    return 0;
}

/**
 * @brief Gets a bit from a file.
 * 
 * @return uint8_t the byte containing solely the wanted bit on its LSB.
 */
uint8_t bstream::readBit() {
    if (readWasNotUsed) {
        read((char*)&bits, 1);
        readWasNotUsed = false;
    }
    if (position < 0) {                             // if the position is already invalid, then
        read((char*)&bits, 1);                      //     we must read a new byte from the file to bits variable
        resetBitPointers(true);                     //     and reset the position back to 7;
    }                                               //
    return ((bits >> position--) & 0x01);             // return the proper value.
}

/**
 * @brief Get n bits from a file.
 * @param num number of bits to get from file.
 * @return uint8_t the byte containing solely the wanted bits on its LSB.
 */
uint8_t bstream::readNBits(int num){
    uint8_t value;
    for (int i = 0; i < num; i++){
        value = value << 1 | readBit();
    }
    return value;
}

int bstream::grantWrite() {
    if (position < 7 && position >= 0) {    // if the position does not cover a completed byte, then
        writeNBits(0, 7-position);          //     write a byte with the leftovers; (TODO)
    }                                       //
    close();                                // then close the file and exit.
}

/**
 * @brief Destroy the bstream::bstream object.
 */
bstream::~bstream() {
    grantWrite();
}
