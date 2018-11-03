#include <vector>

#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

constexpr int BLOCK_SIZE = 65536;

int main(void) {
    AudioHandler audio = AudioHandler("file.wav", 1);
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
    vector<short> predictedSamples = predictor.getPredictedSamples();
    cout << "length: " << predictedSamples.size() << endl;
    bstream output = bstream{ "output.wavz", ios::out|ios::binary };
    Golomb golomb = Golomb(8);
    for (auto sample : predictedSamples) {
        // cout << sample << ", ";
        golomb.encode(sample, output);
    }
    // cout << endl;
    golomb.endEncode(output);
    // --- file should be created

    bstream input = bstream{ "output.wavz", ios::in|ios::binary };
    vector<short> fromFile;
    cout << "DECODED:" << endl;
    for (int i = 0; i != predictedSamples.size(); i++) {
        fromFile.push_back(golomb.decode(input));
        // cout << fromFile[i] << ", ";
    }
    // cout << endl;
    cout << "length: " << fromFile.size() << endl;
    //predictor.setPredictedSamples(predictedSamples);

    predictor.revert();
    samples = predictor.getRevertSamples();
    audio.save("file.compressed.wav", samples, nFrames);
    return 0;

    /*Golomb golomb = Golomb(3);
    bstream file = bstream{ "output", ios::out|ios::binary };
    for (int i = -11; i != 11; i++) {
        cout << "value " << i << endl;
        golomb.encode(i, file);
    }
    golomb.endEncode(file);
    cout << "DECODE" << endl;
    bstream file2 = bstream{ "output", ios::in|ios::binary };
    for (int i = -11; i != 11; i++) {
        cout << "value " << i << " is " << golomb.decode(file2) << endl;
    }
    file2.grantWrite();


    bstream test = bstream{ "test" , ios::out|ios::binary};
    test.writeNBits(1, 10);
    test.grantWrite();*/
}

