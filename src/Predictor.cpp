#include "Predictor.h"

/**
 * @brief Predicts the audio compressed file.
 * 
 * Having the class' internal originalAudioSamples structure, simply evaluate the difference between 
 * an element i and its following i+1 and put it on the predictedAudioSamples. The first sample must be
 * written as is on the target vector (at the beginning).
 * 
 * @return int 
 */
int Predictor::predict() {
    for (int i = 0; i < originalAudioSamples.size()-1; i++) {
        for (int j = 1; j < originalAudioSamples.size(); j++) {
            predictedAudioSamples.push_back(originalAudioSamples.at(i) - originalAudioSamples.at(j)); //get difference between sample in i and ihe following sample.
        }
    }
    return 0;
}

/**
 * @brief Reverts the operation made by predict method.
 * 
 * Having the class' internal predictedAudioSamples structure, simply evaluate the sum between pairs of this 
 * vector. Starting at the beginning of the vector one should simply add the element i with the symmetric value 
 * of the element i+1, till no element is left on the vector.
 * 
 * The output vector should be revertedAudioSamples and its content should be audible if an audio file is created 
 * from it. But first, in order to reduce the size of the samples' representations, it is important to give these 
 * data to the Golomb module.
 * 
 * @return int 
 */
int Predictor::revert() { 
    for (int i = 0; i < predictedAudioSamples.size()-1; i++){
        for (int j = 1; j < predictedAudioSamples.size(); j++){
            revertedAudioSamples.push_back(predictedAudioSamples.at(i) + predictedAudioSamples.at(j)); //get sum between the pairs of the sample vector.
        }
    }
    return 0;
}

int Predictor::setSamples(vector<short> samples) {
    originalAudioSamples = samples;
    return 0;
}

int Predictor::setPredictedSamples(vector<short> samples) {
    predictedAudioSamples = samples;
    return 0;
}

int Predictor::setRevertSamples(vector<short> samples) {
    revertedAudioSamples = samples;
    return 0;
}

vector<short> Predictor::getPredictedSamples() {
    return predictedAudioSamples;
}

vector<short> Predictor::getRevertSamples() {
    return revertedAudioSamples;
}

vector<short> Predictor::getOriginalSamples() {
    return originalAudioSamples;
}