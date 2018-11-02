#include <vector>

#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

constexpr int BLOCK_SIZE = 65536;

bool isDifferent(vector<short>& a, vector<short>& b) {
    if (a.size() != b.size()) {
        cout << "different sizes" << endl;
        cout << a.size() << "   " << b.size() << endl;
        return true;
    }
    for (int i = 0; i != a.size(); i++) {
        if (a[i] != b[i]) {
            cout << "different content" << endl;
            return true;
        }
    }
    return false;
}

void printDifferences(vector<short>& original, vector<short>& modified) {
    int countError = 0;
    int count = 0;
    cout << "Modified samples:" << endl;
    for (int i = 0; i != original.size(); i++) {
        if (original[i] != modified[i]) {
            cout << "\033[1;31m" << modified[i] << "\033[0m" << ", ";
            if (++countError == 20) {
                count = i;
                break;
            }
        } else {
            cout << modified[i] << ", ";
        }
    }
    cout << endl;
    cout << "Original samples:" << endl;
    int counter = 0;
    for (auto element : original) {
        cout << element << ", ";
        if (counter++ == count) {
            break;
        }
    }
    cout << endl;
}

int main(void) {
    // AdvancedPredictor predictor = AdvancedPredictor();
    // vector<short> samples;
    // vector<short> all;

    // predictor.setFramesBufferSize(9);

    // // uses 0
    // samples.clear();
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // samples.clear();
    // samples.push_back(8);
    // samples.push_back(2);
    // samples.push_back(6);
    // samples.push_back(4);
    // samples.push_back(5);
    // samples.push_back(3);
    // samples.push_back(7);
    // samples.push_back(1);
    // samples.push_back(1);
    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // samples.clear();

    // samples.push_back(1);
    // samples.push_back(2);
    // samples.push_back(3);
    // samples.push_back(4);
    // samples.push_back(5);
    // samples.push_back(6);
    // samples.push_back(7);
    // samples.push_back(8);
    // samples.push_back(8);
    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());
    // samples.clear();

    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10);
    // samples.push_back(10); 
    // samples.push_back(10); 
    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // samples.clear();

    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0); 
    // samples.push_back(0); 
    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // samples.clear();
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // samples.clear();
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // predictor.setSamples(samples);
    // predictor.predict();

    // all.insert(all.end(), samples.begin(), samples.end());

    // samples.clear();

    // // predictor.setSamples(samples);
    // // predictor.predict();

    // // for (auto element : samples) {
    // //     cout << element << "  ";
    // // }

    // // samples.clear();
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(40);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // // for (auto element : samples) {
    // //     cout << element << "  ";
    // // }

    // samples.clear();
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(20);
    // samples.push_back(30);
    // samples.push_back(30);
    // samples.push_back(30);
    // samples.push_back(30);
    // samples.push_back(30);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());
    
    // // uses 3
    // samples.clear();
    // samples.push_back(20);
    // samples.push_back(400);
    // samples.push_back(30);
    // samples.push_back(300);
    // samples.push_back(40);
    // samples.push_back(50);
    // samples.push_back(200);
    // samples.push_back(500);
    // samples.push_back(500);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // // uses 0
    // samples.clear();
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // // uses 0
    // samples.clear();
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // // uses 0
    // samples.clear();
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // // uses 0
    // samples.clear();
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // // uses 0
    // samples.clear();
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);
    // samples.push_back(0);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());

    // // uses 3
    // samples.clear();
    // samples.push_back(20);
    // samples.push_back(400);
    // samples.push_back(30);
    // samples.push_back(300);
    // samples.push_back(40);
    // samples.push_back(50);
    // samples.push_back(200);
    // samples.push_back(500);
    // samples.push_back(500);

    // predictor.setSamples(samples);
    // predictor.predict();
    
    // all.insert(all.end(), samples.begin(), samples.end());



    // // for (auto element : samples) {
    // //     cout << element << "  ";
    // // }

    // samples.clear();
    // samples.push_back(80);
    // samples.push_back(80);
    // samples.push_back(50);
    // samples.push_back(50);
    // samples.push_back(80);
    // samples.push_back(80);
    // samples.push_back(50);
    // samples.push_back(50);
    // samples.push_back(50);

    // predictor.setSamples(samples);
    // predictor.predict();

    // all.insert(all.end(), samples.begin(), samples.end());

    // cout << "Samples: " << endl;
    // for (auto element : all) {
    //     cout << element << "  ";
    // }
    // // for (auto element : samples) {
    // //     cout << element << "  ";
    // // }

    // // samples.clear();
    // // samples.push_back(0);
    // // samples.push_back(0);
    // // samples.push_back(10);
    // // samples.push_back(10);
    // // samples.push_back(0);
    // // samples.push_back(0);
    // // samples.push_back(10);
    // // samples.push_back(10);

    // // predictor.setSamples(samples);
    // // predictor.predict();

    // // for (auto element : samples) {
    // //     cout << element << "  ";
    // // }

    // // samples.clear();
    // // samples.push_back(70);
    // // samples.push_back(70);
    // // samples.push_back(70);
    // // samples.push_back(70);
    // // samples.push_back(20);
    // // samples.push_back(20);
    // // samples.push_back(20);
    // // samples.push_back(20);

    // // predictor.setSamples(samples);
    // // predictor.predict();

    // // for (auto element : samples) {
    // //     cout << element << "  ";
    // // }

    // // samples.clear();
    // // samples.push_back(10);
    // // samples.push_back(10);
    // // samples.push_back(10);
    // // samples.push_back(10);
    // // samples.push_back(20);
    // // samples.push_back(20);
    // // samples.push_back(20);
    // // samples.push_back(20);

    // // predictor.setSamples(samples);
    // // predictor.predict();

    // // for (auto element : samples) {
    // //     cout << element << "  ";
    // // }
    // cout << endl;

    // cout << "Predictions:" << endl;
    // for (auto element : predictor.getPredictedSamples()) {
    //     cout << element << "  ";
    // }
    // cout << endl;

    // cout << "Predictor Used:" << endl;
    // for (auto element : predictor.getUsedPredictorVector()) {
    //     cout << (short)element << "  ";
    // }
    // cout << endl;

    // predictor.revert();

    // cout << "Decoded:" << endl;
    // // for (auto element : predictor.getRevertSamples()) {
    // //     cout << (short)element << "  ";
    // // }
    // // cout << endl;

    // vector<short> finalVector = predictor.getRevertSamples();

    // printDifferences(all, finalVector);

    // // int m = 5;
    // // bstream file {"output", ios::out|ios::binary };
    // // Golomb golomb { m };

    // // for (int i = -15; i != 16; i++) {
    // //     golomb.encode(i, file);
    // // }

    // // file.grantWrite();

    // // bstream input {"output", ios::in|ios::binary};

    // // for (int i = -15; i != 16; i++) {
    // //     cout << "Decoding value… ";
    // //     cout << golomb.decode(input) << endl;
    // // }

    // // input.close();

    // // size_t size = 10;
    // // // create an empty vector
    // // vector<short> samples;

    // // samples.push_back(10);
    // // samples.push_back(10);
    // // samples.push_back(20);
    // // samples.push_back(20);
    // // samples.push_back(30);
    // // samples.push_back(30);
    // // samples.push_back(40);
    // // samples.push_back(40);
    // // samples.push_back(50);
    // // samples.push_back(50);

    // // samples.push_back(1);
    // // samples.push_back(2);
    // // samples.push_back(3);
    // // samples.push_back(4);
    // // samples.push_back(5);
    // // samples.push_back(6);
    // // samples.push_back(7);


    // //  //store samples
    // // NaivePredictor p;
    // // p.setSamples(samples);    

    // // //predict function
    // // p.predict();
    // // vector<short> predict = p.getPredictedSamples();
    // // cout << "Predict samples..." << endl;
    // // //show predicted samples
    // // for (int i = 0; i < predict.size(); i++){
    // //     cout << predict[i] << "  ";
    // // }



    // // //revert function
    // // p.revert();
    // // vector<short> revert = p.getRevertSamples();
    // // cout << "\nRevert samples..." << endl;
    // // //show revert samples
    // // for (int i = 0; i < revert.size(); i++){
    // //     cout << revert[i] << " ";
    // // }

    // return 0;


    AudioHandler audio = AudioHandler("file.wav", 1);
    AdvancedPredictor predictor = AdvancedPredictor();
    vector<short> samples;
    predictor.setFramesBufferSize(BLOCK_SIZE);
    //predictor.setFramesBufferSize(4096);
    int nFrames = 0;
    vector<short> original;
    while (true) {
        int frames;
        if ((frames = audio.loadBlock()) == -2) {
            break;
        }
        // vector<short> a = audio.getSamples_16();
        // samples.insert(samples.end(), a.begin(), a.end());
        samples = audio.getSamples_16();
        cout << samples.size() << " is the sample size" << endl;
        original.insert(original.end(), samples.begin(), samples.end());
        cout << frames << endl;
        nFrames += frames;
        for (int i = 0; i != (int)(samples.size()/BLOCK_SIZE); i++) {
            vector<short> block = vector<short>(samples.begin()+(BLOCK_SIZE*i), samples.begin()+(BLOCK_SIZE*(i+1)));
            cout << block.size() << "aaa" << endl;
            predictor.setSamples(block);
            predictor.predict();
        }
    }
    //nFrames = samples.size();
    // cout << "Number of frames(Predict): " << nFrames << endl;
    predictor.revert();
    samples = predictor.getRevertSamples();
    // cout << "Samples: ";
    // for (int i = 0; i != 10; i++) {
    //     cout << samples[i] << ", ";
    // }
    // cout << endl;
    // cout << "Original: ";
    // for (int i = 0; i != 10; i++) {
    //     cout << original[i] << ", ";
    // }
    // cout << endl;   

    // printDifferences(original, samples);
    // cout << "Prediction: " << endl;
    // for (int i = 0; i != 20; i++) {
    //     cout << predictor.getPredictedSamples()[i] << ", ";
    // }
    // cout << endl;
    // cout << "Used : " << endl;
    // for (int i = 0; i != 20; i++) {
    //     cout << (int)predictor.getUsedPredictorVector()[i] << ", ";
    // }
    // cout << endl;

    //cout << isDifferent(original, samples) << endl;
    // Golomb golomb = Golomb(512);
    // bstream file {"output", ios::out|ios::binary };
    // for (auto sample : samples) {
    //     golomb.encode(sample, file);
    // }
    // file.close();
    // bstream file2 = bstream{ "output", ios::in|ios::binary};
    // vector<short> retrievedSamples;
    // while (file2.)
    // golomb.decode(file2);
    // cout << "used preditor was ";
    // for (int i = 0; i < predictor.getUsedPredictorVector().size(); i++){
    //     cout << (int)(predictor.getUsedPredictorVector()[i]) << ", ";
    // }
    // cout << endl;
    //nFrames = samples.size();
    cout << "Number of frames: " << nFrames;

    audio.save("file.compressed.wav", samples, nFrames);

    return 0;
}

