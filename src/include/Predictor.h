#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>
#include <sndfile.hh>

using namespace std;

class Predictor {
    public:
        short predict(short value1, short value2);
    private:
        vector<short> originalAudioSamples;
};

#endif