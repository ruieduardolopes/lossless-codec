#include "Golomb.h"

using namespace std;

Golomb::Golomb(int m) {
    mValue = m;
}

int Golomb::generate_truncated_binary_code(int remaindr, bstream& file) {
    if ((mValue != 0) && ((mValue & (mValue - 1)) == 0)) {
        file.writeNBits(remaindr, 8);                       // code remainder in plain binary
        return remaindr;
    } else {
        int bValue = ceil(extmath::lb(mValue));
        if (remaindr < pow(2,bValue) - mValue) {
            file.writeNBits(remaindr, bValue - 1);          // code remainder in bValue - 1 bits
            return remaindr;
        } else {
            file.writeNBits(remaindr + pow(2,bValue) - mValue, bValue); // code remainder + pow(2,bValue) - Golomb::mValue in plain binary using bValue bits
            return remaindr + pow(2,bValue) - mValue;
        }
    }
    return 0;
}

int Golomb::generate_unary_code(int quotient, bstream& file) {
    int unaryCode = 0;
    for (int i = 0; i != quotient; i++) { 
        unaryCode = (unaryCode + 1) << 1; 
    }
    file.writeNBits(unaryCode, quotient+1);
    return unaryCode;
}

int Golomb::encode(int n, bstream& file) {
    int quotient = n / mValue;
    int remaindr = n % mValue;
    generate_unary_code(quotient, file);
    generate_truncated_binary_code(remaindr, file);
    return 0;
}

int Golomb::decode(bstream& file) {
    int bValue = ceil(extmath::lb(mValue));         
    int comparisonValue = pow(2,bValue) - mValue; 
    int numberOfConsecutiveOnes = 0;
    while (true) {
        if (file.readBit() == 1) {
            numberOfConsecutiveOnes++;
        } else {
            break;
        }
    }
    int nextInputBits = 0;
    int finalValue = 0;
    for (int i = 0; i != bValue-1; i++) {
        nextInputBits = nextInputBits << i | file.readBit();
    }
    if (nextInputBits < comparisonValue) {
        finalValue = numberOfConsecutiveOnes * mValue + nextInputBits;
    } else {
        nextInputBits = nextInputBits * 2 + file.readBit();
        finalValue = numberOfConsecutiveOnes * mValue + nextInputBits - comparisonValue;
    }
    return finalValue;
}

int Golomb::endEncode(bstream& file) {
    file.grantWrite();
    return 0;
}