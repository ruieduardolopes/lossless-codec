#include "NaivePredictor.h"

/**
 * @brief Predicts the audio compressed file.
 * 
 * Having the class' internal originalAudioSamples structure, simply evaluate the difference between 
 * an element i and its following i+1 and put it on the predictedAudioSamples. The first sample must be
 * written as is on the target vector (at the beginning).
 * 
 * @return int 
 */
int NaivePredictor::predict() {
    predictedAudioSamples.push_back(originalAudioSamples.at(0)); //save first sample
    for (int i = 0; i < originalAudioSamples.size()-1; i++) {
        predictedAudioSamples.push_back(originalAudioSamples.at(i) - originalAudioSamples.at(i+1)); //get difference between sample in i and ihe following sample
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
int NaivePredictor::revert() { 
    revertedAudioSamples.push_back(predictedAudioSamples.at(0));
    for (int i = 0; i < predictedAudioSamples.size()-1; i++){
        revertedAudioSamples.push_back(revertedAudioSamples.at(i) - predictedAudioSamples.at(i+1)); //get sum between the pairs of the sample vector.
    }
    return 0;
}