#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>
#include <sndfile.hh>

using namespace std;

class Predictor {
    public:
        int predict();
        int revert();
        int setSamples(vector<short> samples);
        int setPredictedSamples(vector<short> samples);
        int setRevertSamples(vector<short> samples);
        vector<short> getPredictedSamples();
        vector<short> getRevertSamples();
        vector<short> getOriginalSamples();
    private:
        vector<short> originalAudioSamples;
        vector<short> predictedAudioSamples;
        vector<short> revertedAudioSamples;
};

#endif