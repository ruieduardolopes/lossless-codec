#include <stdio.h>
#include <cstdlib>
#include <math.h>

#include "bstream.h"

using namespace std;

class Golomb {
    public: 
        Golomb(int mValue);
        Golomb(int mValue, bstream file);
        int encode(int value);
        int endEncode();
    private:
        int generate_unary_code(int quotient);
        int generate_truncated_binary_code(int remaindr);
        int mValue;
        bstream file;
};