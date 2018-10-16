#include "Golomb.h"

Golomb::Golomb(int m) {
    Golomb::mValue = m;
}

int Golomb::encode(int n) {
    int quotient = Golomb::mValue / n;
    int remaindr = n % Golomb::mValue;
    Golomb::generate_unary_code(quotient);
    Golomb::generate_truncated_binary_code(remaindr);
    return 0;
}