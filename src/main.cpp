#include <vector>

#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

constexpr int BLOCK_SIZE = 65536;

int main(void) {
    /*AdvancedPredictor predictor = AdvancedPredictor();
    predictor.setFramesBufferSize(4);
    vector<short> samples;

    cout << "X: ";

    samples.push_back(8);
    samples.push_back(7);
    samples.push_back(1);
    samples.push_back(2);
    predictor.setSamples(samples);
    predictor.predict();

    for (auto sample : samples) {
        cout << sample << ", ";
    }
    // cout << " | ";

    samples.clear();
    samples.push_back(3);
    samples.push_back(5);
    samples.push_back(20);
    samples.push_back(19);
    predictor.setSamples(samples);
    predictor.predict();

    for (auto sample : samples) {
        cout << sample << ", ";
    }
    // cout << " | ";

    samples.clear();
    samples.push_back(4);
    samples.push_back(5);
    samples.push_back(6);
    samples.push_back(5);
    predictor.setSamples(samples);
    predictor.predict();

    for (auto sample : samples) {
        cout << sample << ", ";
    }
    // cout << " | ";

    samples.clear();
    samples.push_back(1);
    samples.push_back(0);
    samples.push_back(0);
    samples.push_back(2);
    predictor.setSamples(samples);
    predictor.predict();

    for (auto sample : samples) {
        cout << sample << ", ";
    }
    cout << endl;

    cout << "used predictor:" << endl;
    for (auto sample : predictor.getUsedPredictorVector()) {
        cout << (int)sample << ", ";
    }
    cout << endl;

    cout << "r: ";
    int count = 0;
    for (auto sample : predictor.getResiduals()) {
        cout << sample << ", ";
        if (count++ % 4 == 0) {
            // cout << " | ";
        }
    }
    cout << endl;

    predictor.revert();

    cout << "X: ";
    count = 0;
    for (auto sample : predictor.getRevertSamples()) {
        cout << sample << ", ";
        if (count++ % 4 == 0) {
            // cout << " | ";
        }
    }
    cout << endl;
*/

    AudioHandler audio = AudioHandler("sample07.wav", 1);
    AdvancedPredictor predictor = AdvancedPredictor();
    vector<short> samples;
    predictor.setFramesBufferSize(BLOCK_SIZE);
    int nFrames = 0;
    vector<short> original;
    while (true) {
        int frames;
        if ((frames = audio.loadBlock()) == -2) {
            break;
        }
        samples = audio.getSamples_16();
        original.insert(original.end(), samples.begin(), samples.end());
        nFrames += frames;
        for (int i = 0; i != (int)(samples.size()/BLOCK_SIZE); i++) {
            vector<short> block = vector<short>(samples.begin()+(BLOCK_SIZE*i), samples.begin()+(BLOCK_SIZE*(i+1)));
            predictor.setSamples(block);
            predictor.predict();
        }
    }
    // vector<short> predictedSamples = predictor.getPredictedSamples();
    vector<int> residuals = predictor.getResiduals();
    cout << "length: " << residuals.size() << endl;
    bstream output = bstream{ "output.wavz", ios::out|ios::binary };
    Golomb golomb = Golomb(16384);
    int writtenSamples = 0;
    cout << "before:" << endl;
    for (auto sample : predictor.getUsedPredictorVector()) {
        cout << (int)sample << ", ";
    }
    cout << endl;
    for (auto residual : residuals) {
        if (writtenSamples % BLOCK_SIZE == 0) {
            switch (predictor.getUsedPredictorOn(writtenSamples/BLOCK_SIZE)) {
                case 0: 
                    output.writeBit(0);
                    output.writeBit(0);
                    break;
                case 1:
                    output.writeBit(0);
                    output.writeBit(1);
                    break;
                case 2: 
                    output.writeBit(1);
                    output.writeBit(0);
                    break;
                case 3:
                    output.writeBit(1);
                    output.writeBit(1);
                    break;
                default:
                    break;
            }
        }
        golomb.encode(residual, output);
        writtenSamples += 1;
    }
    golomb.endEncode(output);
    bstream input = bstream{ "output.wavz", ios::in|ios::binary };
    vector<short> fromFile;
    cout << "DECODED:" << endl;
    int readSamples = 0;
    vector<char> predictorUsed;
    for (int i = 0; i != residuals.size(); i++) {
        if (readSamples % BLOCK_SIZE == 0) {
            predictorUsed.push_back((input.readBit() << 1) | input.readBit());
        }
        fromFile.push_back(golomb.decode(input));
        readSamples += 1;
    }
    cout << "length: " << fromFile.size() << endl;
    predictor.setUsedPredictor(predictorUsed);

    cout << "after:" << endl;
    for (auto sample : predictor.getUsedPredictorVector()) {
        cout << (int)sample << ", ";
    }
    cout << endl;

    predictor.revert();
    samples = predictor.getRevertSamples();
    audio.save("file.compressed.wav", samples, nFrames);
    return 0;
}

