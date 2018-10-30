#include <vector>

#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

int main(void) {
    AdvancedPredictor predictor = AdvancedPredictor();
    vector<short> samples;

    predictor.setFramesBufferSize(8);
    // samples.push_back(8);
    // samples.push_back(2);
    // samples.push_back(6);
    // samples.push_back(4);
    // samples.push_back(5);
    // samples.push_back(3);
    // samples.push_back(7);
    // samples.push_back(1);

    samples.push_back(1);
    samples.push_back(2);
    samples.push_back(3);
    samples.push_back(4);
    samples.push_back(5);
    samples.push_back(6);
    samples.push_back(7);
    samples.push_back(8);

    predictor.setSamples(samples);
    predictor.predict();

    cout << "Samples:" << endl;
    for (auto element : samples) {
        cout << element << "  ";
    }

    // samples.clear();
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(70);
    // samples.push_back(70);
    // samples.push_back(70);
    // samples.push_back(70); 

    // predictor.setSamples(samples);
    // predictor.predict();

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }

    // samples.clear();
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);

    // predictor.setSamples(samples);
    // predictor.predict();

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }

    // samples.clear();
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(30);
    // samples.push_back(30);
    // samples.push_back(30);
    // samples.push_back(30);

    // predictor.setSamples(samples);
    // predictor.predict();

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }

    // samples.clear();
    // samples.push_back(80);
    // samples.push_back(80);
    // samples.push_back(50);
    // samples.push_back(50);
    // samples.push_back(80);
    // samples.push_back(80);
    // samples.push_back(50);
    // samples.push_back(50);

    // predictor.setSamples(samples);
    // predictor.predict();

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }

    // samples.clear();
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(10);
    // samples.push_back(10);

    // predictor.setSamples(samples);
    // predictor.predict();

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }

    // samples.clear();
    // samples.push_back(70);
    // samples.push_back(70);
    // samples.push_back(70);
    // samples.push_back(70);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);

    // predictor.setSamples(samples);
    // predictor.predict();

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }

    // samples.clear();
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);

    // predictor.setSamples(samples);
    // predictor.predict();

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }
    cout << endl;

    cout << "Predictions:" << endl;
    for (auto element : predictor.getPredictedSamples()) {
        cout << element << "  ";
    }
    cout << endl;

    cout << "Predictor Used:" << endl;
    for (auto element : predictor.getUsedPredictorVector()) {
        cout << (short)element << "  ";
    }
    cout << endl;

    predictor.revert();

    cout << "Decoded:" << endl;
    for (auto element : predictor.getRevertSamples()) {
        cout << (short)element << "  ";
    }
    cout << endl;

    // int m = 5;
    // bstream file {"output", ios::out|ios::binary };
    // Golomb golomb { m };

    // for (int i = -15; i != 16; i++) {
    //     golomb.encode(i, file);
    // }

    // file.grantWrite();

    // bstream input {"output", ios::in|ios::binary};

    // for (int i = -15; i != 16; i++) {
    //     cout << "Decoding value… ";
    //     cout << golomb.decode(input) << endl;
    // }

    // input.close();

    // size_t size = 10;
    // // create an empty vector
    // vector<short> samples;

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

    // samples.push_back(1);
    // samples.push_back(2);
    // samples.push_back(3);
    // samples.push_back(4);
    // samples.push_back(5);
    // samples.push_back(6);
    // samples.push_back(7);


    //  //store samples
    // NaivePredictor p;
    // p.setSamples(samples);    

    // //predict function
    // p.predict();
    // vector<short> predict = p.getPredictedSamples();
    // cout << "Predict samples..." << endl;
    // //show predicted samples
    // for (int i = 0; i < predict.size(); i++){
    //     cout << predict[i] << "  ";
    // }



    // //revert function
    // p.revert();
    // vector<short> revert = p.getRevertSamples();
    // cout << "\nRevert samples..." << endl;
    // //show revert samples
    // for (int i = 0; i < revert.size(); i++){
    //     cout << revert[i] << " ";
    // }

    return 0;
}
