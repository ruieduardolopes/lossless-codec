#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>
#include <sndfile.hh>

using namespace std;

class Predictor {
    public:
        short predict(short value1, short value2);
        int setSamples(vector<short> samples);
        vector<short> getPredictedSamples();
    private:
        vector<short> originalAudioSamples;
        vector<short> predictedAudioSamples;
};

#endif