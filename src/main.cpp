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
    // create an empty vector
    vector<short> samples;

    samples.push_back(10);
    samples.push_back(10);
    samples.push_back(20);
    samples.push_back(20);
    samples.push_back(30);
    samples.push_back(30);
    samples.push_back(40);
    samples.push_back(40);
    samples.push_back(50);
    samples.push_back(50);

     //store samples
    Predictor p;
    p.setSamples(samples);

    //predict function
    p.predict();
    vector<short> predict = p.getPredictedSamples();
    //show predicted samples
    for (int i = 0; i < predict.size(); i++){
        cout << "Predict samples..." << endl;
        cout << predict[i] << endl;
    }

    //revert function
    p.revert();
    vector<short> revert = p.getRevertSamples();
    //show revert samples
    for (int i = 0; i < revert.size(); i++){
        cout << "Revert samples..." << endl;
        cout << revert[i] << endl;
    }

    return 0;
}
