#include "AdvancedPredictor.h"

#include <iostream>

int AdvancedPredictor::predict() {
    resetVectors();
    verifyVectorCompleteness();
    
    // Fill results from predictors
    vector<short> samples = originalAudioSamples;
    samples.insert(samples.begin(), lastThreeSamples.begin(), lastThreeSamples.end());

    for (int n = 3; n != samples.size(); n++) {
        resultsPredictor1.push_back(x_0());
        deviationPredictor1.push_back(e_0(samples, n));
        resultsPredictor2.push_back(x_1(samples, n));
        deviationPredictor2.push_back(e_1(samples, n));
        resultsPredictor3.push_back(x_2(samples, n));
        deviationPredictor3.push_back(e_2(samples, n));
        resultsPredictor4.push_back(x_3(samples, n));
        deviationPredictor4.push_back(e_3(samples, n));        
    }

    // Update the last three samples
    lastThreeSamples = vector<short>(samples.begin()+(samples.size()-3), samples.end());
    
    // Evaluate the smaller deviation, to choose the predictor
    vector<double> meanDeviations;
    meanDeviations.push_back(extmath::mean(deviationPredictor1));
    meanDeviations.push_back(extmath::mean(deviationPredictor2));
    meanDeviations.push_back(extmath::mean(deviationPredictor3));
    meanDeviations.push_back(extmath::mean(deviationPredictor4));
    char predictorIndex = extmath::minIndex(meanDeviations);
    usedPredictor.push_back(predictorIndex);

    // apply the predictions
    switch (predictorIndex) {
        case 0:
            Quantizer::quantize(deviationPredictor1, 4);
            residuals.insert(residuals.end(), deviationPredictor1.begin(), deviationPredictor1.end());
            break;
        case 1:
            Quantizer::quantize(deviationPredictor2, 4);
            residuals.insert(residuals.end(), deviationPredictor2.begin(), deviationPredictor2.end());
            break;
        case 2:
            Quantizer::quantize(deviationPredictor3, 4);
            residuals.insert(residuals.end(), deviationPredictor3.begin(), deviationPredictor3.end());
            break;
        case 3:
            Quantizer::quantize(deviationPredictor4, 4);
            residuals.insert(residuals.end(), deviationPredictor4.begin(), deviationPredictor4.end());
            break;
        default:
            break; // TODO : handle this case
    }
    return 0;
}

int AdvancedPredictor::revert() {
    resetVectors();
    int predictorResult = 0;
    int currentPredictor = 0;
    revertedAudioSamples.push_back(0);
    revertedAudioSamples.push_back(0);
    revertedAudioSamples.push_back(0);
    revertedAudioSamples.push_back(residuals[0] + predictorResult);
    for (int i = 1; i != residuals.size(); i++) {
        currentPredictor = usedPredictor[i/framesBufferSize];
        switch (currentPredictor) {
            case 0: 
                predictorResult = x_0();
                break;
            case 1:
                predictorResult = x_1(revertedAudioSamples, i+3);
                break;
            case 2: 
                predictorResult = x_2(revertedAudioSamples, i+3);
                break;
            case 3: 
                predictorResult = x_3(revertedAudioSamples, i+3);
                break;
            default:
                break; // TODO : handle this case
        }
        revertedAudioSamples.push_back(residuals[i] + predictorResult);
    }
    revertedAudioSamples.erase(revertedAudioSamples.begin(), revertedAudioSamples.begin()+3);
    return 0;
}

vector<char> AdvancedPredictor::getUsedPredictorVector() {
    return usedPredictor;
}

vector<int> AdvancedPredictor::getResiduals() {
    return residuals;
}

void AdvancedPredictor::setResiduals(vector<int>& deviations) {
    residuals = deviations;
}

char AdvancedPredictor::getUsedPredictorOn(int frame) {
    return usedPredictor[frame];
}

void AdvancedPredictor::setUsedPredictor(vector<char>& used) {
    usedPredictor = used;
}

int AdvancedPredictor::resetVectors() {
    resultsPredictor1.clear();
    resultsPredictor2.clear();
    resultsPredictor3.clear();
    resultsPredictor4.clear();
    deviationPredictor1.clear();
    deviationPredictor2.clear();
    deviationPredictor3.clear();
    deviationPredictor4.clear();
    return 0;
}

int AdvancedPredictor::verifyVectorCompleteness() {
    while (originalAudioSamples.size() % framesBufferSize != 0) {
        originalAudioSamples.push_back(0);
    }

    return 0;
}