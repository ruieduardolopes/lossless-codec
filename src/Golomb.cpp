#include "Golomb.h"

Golomb::Golomb(int m) {
    mValue = m;
}

Golomb::Golomb(int m, bstream file) {
    Golomb(m);
    Golomb::file = file;
}

int Golomb::encode(int n) {
    int quotient = mValue / n;
    int remaindr = n % mValue;
    generate_unary_code(quotient);
    generate_truncated_binary_code(remaindr);
    return 0;
}

int Golomb::generate_unary_code(int quotient) {
    int unaryCode = 0;
    for (int i = 0; i != quotient; i++) {
        unaryCode = (unaryCode + 1) << 1; 
    }
    return unaryCode;
}

int Golomb::generate_truncated_binary_code(int remaindr) {
    if ((mValue != 0) && ((mValue & (mValue - 1)) == 0)) {
        file.writeNBits(remaindr, 8);                       // code remainder in plain binary
    } else {
        int bValue = (int)lb(mValue) + 1;
        if (remaindr < pow(2,bValue) - mValue) {
            file.writeNBits(remaindr, bValue - 1);          // code remainder in bValue - 1 bits
        } else {
            file.writeNBits(remainder + pow(2,bValue) - Golomb::mValue, bValue); // code remainder + pow(2,bValue) - Golomb::mValue in plain binary using bValue bits
        }
    }
    return 0;
}

int Golomb::endEncode() {
    file.grantWrite();
    return 0;
}

double lb(int x) {
    return log10(x) / log10(2);
}
