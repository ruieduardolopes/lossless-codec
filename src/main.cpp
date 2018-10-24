#include "Golomb.h"

using namespace std;

int main(void) {
    int m = 5;
    bstream file {"output", ios::out|ios::binary };
    Golomb golomb { m };

    for (int i = 0; i != 16; i++) {
        golomb.encode(i, file);
    }

    return 0;
}