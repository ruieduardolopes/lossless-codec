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
        // cout << "Smaples..." << endl;
        // for (int i = 0; i < samples.size(); i++) {
        //     cout << samples[i] << endl;
        // }
        predictor.setSamples(samples);
        predictor.predict();
    }
    predictor.revert();
    samples = predictor.getRevertSamples();
    cout << "Samples..." << endl;
    for (int i = 0; i < samples.size(); i++){
        cout << samples[i] << endl;
    }
    nFrames = samples.size();
    cout << "Number of frames: " << nFrames;
    audio.save("file.compressed.wav", samples, nFrames);

    return 0;
}
