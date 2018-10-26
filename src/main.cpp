#include "Golomb.h"
#include "Predictor.h"
#include <vector>

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

    size_t size = 10;
    vector<short> samples(size);  //make room for 10 integers

    //fill vector
    for (int i =0; i < size; i++){
        cout << "Inserting samples...";
        samples[i] = i;
    }
    
    //store samples
    Predictor p;
    p.setSamples(samples);

    //predict function
    p.predict();
    vector<short> predict = p.getPredictedSamples();
    //show predicted samples
    for (int i = 0; i < size; i++){
        cout << "Predict samples...";
        cout << predict.at(i) << endl;
    }

    //revert function
    p.revert();
    vector<short> revert = p.getRevertSamples();
    //show revert samples
    for (int i = 0; i < size; i++){
        cout << "Revert samples...";
        cout << revert.at(i) << endl;
    }


    return 0;
}
