#include "AdvancedPredictor.h"

int AdvancedPredictor::predict() {
    evaluatePredictions();
    return 0;
}

int AdvancedPredictor::revert() {

    return 0;
}

int AdvancedPredictor::evaluatePredictions() {
    for (int i = 0; i != originalAudioSamples.size(); i++) {
        if (predictedAudioSamples.size() == 0) {
            predictedAudioSamples.push_back(x_0());
            predictorUsed.push_back(0);
        } else if (predictedAudioSamples.size() == 1) {
            predictions.push_back(x_0());
            predictions.push_back(x_1(originalAudioSamples, ))
        }
    }
    return 0;
