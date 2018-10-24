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
        int decode(int value, bstream& file);
        int endEncode(bstream& file);
    private:
        int generate_unary_code(int quotient, bstream& file);
        int generate_truncated_binary_code(int remaindr, bstream& file);
        int mValue;
};