#include <stdio.h>
#include <cstdlib>
#include <math.h>

using namespace std;

class Golomb {
    public: 
        Golomb(int mValue);
        int encode(int value);
    private:
        int generate_unary_code(int quotient);
        int generate_truncated_binary_code(int remaindr);
        int mValue;
};