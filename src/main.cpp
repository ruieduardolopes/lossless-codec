#include <vector>

#include "Golomb.h"
#include "Predictor.h"

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

    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(30);
    // samples.push_back(30);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(50);
    // samples.push_back(50);

    samples.push_back(1);
    samples.push_back(2);
    samples.push_back(3);
    samples.push_back(4);
    samples.push_back(5);
    samples.push_back(6);
    samples.push_back(7);


     //store samples
    Predictor p;
    p.setSamples(samples);

    

    //predict function
    p.predict();
    vector<short> predict = p.getPredictedSamples();
    cout << "Predict samples..." << endl;
    //show predicted samples
    for (int i = 0; i < predict.size(); i++){
        cout << predict[i] << "  ";
    }



    //revert function
    p.revert();
    vector<short> revert = p.getRevertSamples();
    cout << "\nRevert samples..." << endl;
    //show revert samples
    for (int i = 0; i < revert.size(); i++){
        cout << revert[i] << " ";
    }

    return 0;
}
