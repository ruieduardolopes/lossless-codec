#include <vector>

#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

constexpr int BLOCK_SIZE = 65536;

int writeEncodedPredictorIndex(int writtenSamples, bstream& file, AdvancedPredictor& predictor) {
    if (writtenSamples % BLOCK_SIZE == 0) {
            switch (predictor.getUsedPredictorOn(writtenSamples/BLOCK_SIZE)) {
                case 0: 
                    file.writeBit(0);
                    file.writeBit(0);
                    break;
                case 1:
                    file.writeBit(0);
                    file.writeBit(1);
                    break;
                case 2: 
                    file.writeBit(1);
                    file.writeBit(0);
                    break;
                case 3:
                    file.writeBit(1);
                    file.writeBit(1);
                    break;
                default:
                    break;
            }
        }
        writtenSamples += 1;
        return writtenSamples;
}

int readEncodedPredictorIndex(int readSamples, bstream& file, vector<char>& predictors) {
    if (readSamples % BLOCK_SIZE == 0) {
        predictors.push_back((file.readBit() << 1) | file.readBit());
    }
    readSamples += 1;
    return readSamples;
}

int main(void) {
    AudioHandler audio = AudioHandler("sample07.wav", 1);
    AdvancedPredictor predictor = AdvancedPredictor();
    vector<short> samples;
    predictor.setFramesBufferSize(BLOCK_SIZE);
    int nFrames = 0;
    vector<short> original;
    vector<int> residuals;
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
    residuals = predictor.getResiduals();
    cout << "length: " << residuals.size() << endl;
    bstream output = bstream{ "output.wavz", ios::out|ios::binary };
    Golomb golomb = Golomb(8192);
    int writtenSamples = 0;
    cout << "before:" << endl;
    for (auto sample : predictor.getUsedPredictorVector()) {
        cout << (int)sample << ", ";
    }
    cout << endl;


    for (auto residual : residuals) {
        writtenSamples = writeEncodedPredictorIndex(writtenSamples, output, predictor);
        golomb.encode(residual, output);
    }
    golomb.endEncode(output);


    bstream input = bstream{ "output.wavz", ios::in|ios::binary };
    vector<short> fromFile;
    cout << "DECODED:" << endl;
    int readSamples = 0;
    vector<char> predictorUsed;


    for (int i = 0; i != residuals.size(); i++) {
        readSamples = readEncodedPredictorIndex(readSamples, input, predictorUsed);
        fromFile.push_back(golomb.decode(input));
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

