#include "Golomb.h"

using namespace std;

int main(void) {
    int m = 5;
    bstream file {"output", ios::out|ios::binary };
    Golomb golomb { m };

    for (int i = -15; i != 16; i++) {
        golomb.encode(i, file);
    }

    file.grantWrite();

    bstream input {"output", ios::in|ios::binary};

    for (int i = -15; i != 16; i++) {
        cout << "Decoding value… ";
        cout << golomb.decode(input) << endl;
    }

    input.close();

    return 0;
}