#include "Golomb.h"

using namespace std;

Golomb::Golomb(int m) {
    mValue = m;
}


int Golomb::generate_truncated_binary_code(int remaindr, bstream& file) {

    int bValue = ceil(extmath::lb(mValue));
    // file.writeNBits(remaindr, bValue);
    // return remaindr;
    // cout << "n is " << (remaindr < (2*bValue - mValue)) << endl;
    int tValue = pow(2,bValue) - mValue; // 12 - 64 = -52
    if (remaindr < tValue) {
        cout << remaindr << "...." << endl;
        file.writeNBits(remaindr, bValue-1); // 0 em 5 bits
        return remaindr;
    } else {
        remaindr = remaindr + tValue;
        file.writeNBits(remaindr, bValue);
        return remaindr;
    }
}

int Golomb::generate_unary_code(int quotient, bstream& file) {
    int unaryCode = 0;
    for (int i = 0; i != quotient; i++) { 
        // unaryCode = (unaryCode + 1) << 1; 
        file.writeBit(1);
    }
    file.writeBit(0);
    //cout << "Unary code is " << bitset<8>(unaryCode) << " for quotient " << quotient << endl;
    // file.writeNBits(unaryCode, quotient+1);
    //cout << "ended writing unary code" << endl;
    return unaryCode;
}
// 0 1110
int Golomb::encode(int n, bstream& file) {
    if (n >= 0) {
        file.writeBit(0);
    } else {
        file.writeBit(1);
        n = -n;
    }
    int quotient = n / mValue;
    int remaindr = n % mValue;
    generate_unary_code(quotient, file);
    generate_truncated_binary_code(remaindr, file);
    return 0;
}

// 0 0 000000
int Golomb::decode(bstream& file) {
    int valueSignal = file.readBit(); // 0
    int bValue = (int)ceil(extmath::lb(mValue)); // 6
    int tValue = pow(2,bValue) - mValue; // -52
    int numberOfConsecutiveOnes = 0; // 0
    while (true) {
        if (file.readBit() == 1) { // false
            numberOfConsecutiveOnes++;
        } else {
            break;
        }
    }
    int nextInputBits = 0; // 0
    // for (int i = 0; i < bValue; i++) {
    //     nextInputBits = (nextInputBits << 1) | file.readBit(); // 000000 = 0
    // }
    // numberOfConsecutiveOnes = numberOfConsecutiveOnes * mValue + nextInputBits;
    for (int i = 0; i < bValue-1; i++) {
        nextInputBits = (nextInputBits << 1) | file.readBit(); // 00110 = 6
    }
    if (nextInputBits < tValue) {
        numberOfConsecutiveOnes = numberOfConsecutiveOnes * mValue + nextInputBits;
    } else {
        nextInputBits = nextInputBits * 2 + file.readBit();
        numberOfConsecutiveOnes = numberOfConsecutiveOnes * mValue + nextInputBits - tValue;
    }
    return (int)pow(-1,valueSignal) * numberOfConsecutiveOnes;
}

int Golomb::endEncode(bstream& file) {
    file.grantWrite();
    return 0;
}