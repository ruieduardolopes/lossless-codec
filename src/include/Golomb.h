#ifndef GOLOMB_H
#define GOLOMB_H

#include <stdio.h>
#include <cstdlib>
#include <math.h>

#include "bstream.h"
#include "extmath.h"

using namespace std;

class Golomb {
    public: 
        Golomb(int mValue);
        int encode(int value, bstream& file);
        int decode(bstream& file);
        int endEncode(bstream& file);
        int mValue;
    private:
        int generate_unary_code(int quotient, bstream& file);
        int generate_truncated_binary_code(int remaindr, bstream& file);
};

#endif