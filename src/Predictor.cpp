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

    return 0;
}

int Predictor::setSamples(vector<short> samples) {
    originalAudioSamples = samples;

    return 0;
}

vector<short> Predictor::getPredictedSamples() {
    return predictedAudioSamples;
}