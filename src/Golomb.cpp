#include "Golomb.h"

Golomb::Golomb(int m) {
    mValue = m;
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
        // code remainder in plain binary
    } else {
        int bValue = (int)lb(mValue) + 1;
        if (remaindr < pow(2,bValue) - mValue) {
            // code remainder in bValue - 1 bits
        } else {
            // code remainder + pow(2,bValue) - Golomb::mValue in plain binary using bValue bits
        }
    }
    return 0;
}

double lb(int x) {
    return log10(x) / log10(2);
}
