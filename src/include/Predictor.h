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
        vector<short> getPredictedSamples();
    private:
        vector<short> originalAudioSamples;
        vector<short> predictedAudioSamples;
        vector<short> revertedAudioSamples;
};

#endif