#include "Predictor.h"

short Predictor::predict(short value1, short value2) {

    return 0;
}

int Predictor::setSamples(vector<short> samples) {
    originalAudioSamples = samples;

    return 0;
}

vector<short> Predictor::getPredictedSamples() {
    return predictedAudioSamples;
}