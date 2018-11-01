#include <vector>

#include "AdvancedPredictor.h"
#include "Golomb.h"
#include "NaivePredictor.h"
#include "AudioHandler.h"

using namespace std;

int main(void) {
    AudioHandler audio = AudioHandler("file.wav", 1);
    AdvancedPredictor predictor = AdvancedPredictor();
    vector<short> samples;
    predictor.setFramesBufferSize(65536);
    int nFrames = 0;
    while (true) {
        int frames = audio.loadBlock();
        if (frames == -2) {
            cout << "c'mon" << endl;
            break;
        }
        nFrames += frames;
        samples = audio.getSamples_16();
        predictor.setSamples(samples);
        predictor.predict();
    }
    predictor.revert();
    samples = predictor.getRevertSamples();
    audio.save("file.compressed.wav", samples, nFrames);

    return 0;
}
