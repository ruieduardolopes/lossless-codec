#include "AdvancedPredictor.h"

#include <iostream>

int QUANTIZER_FACTOR = 0;
bool alreadyPrinted = false;
bool alreadyPrinted2 = false;
int LEVEL = 10;

AdvancedPredictor::AdvancedPredictor(int quantizeFactor) {
    QUANTIZER_FACTOR = quantizeFactor;
}

int AdvancedPredictor::predict() {
    resetVectors();
    verifyVectorCompleteness();
    
    // Fill results from predictors
    vector<short> samples2 = originalAudioSamples;
    vector<int> samples = Quantizer::quantize(originalAudioSamples, QUANTIZER_FACTOR);
    samples.insert(samples.begin(), lastThreeSamples.begin(), lastThreeSamples.end());
    int lastResidual1;
    int lastResidual2;
    int lastResidual3;
    int lastResidual4;
    
    for (int n = 3; n != samples.size(); n++) {
        resultsPredictor1.push_back(x_0());
        lastResidual1 = e_0(samples, n);
        deviationPredictor1.push_back(lastResidual1);

        resultsPredictor2.push_back(x_1(samples, n));
        lastResidual2 = e_1(samples, n);
        deviationPredictor2.push_back(lastResidual2);

        resultsPredictor3.push_back(x_2(samples, n));
        lastResidual3 = e_2(samples, n);
        deviationPredictor3.push_back(lastResidual3);

        resultsPredictor4.push_back(x_3(samples, n));
        lastResidual4 = e_3(samples, n);
        deviationPredictor4.push_back(lastResidual4); 
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
            residuals.insert(residuals.end(), deviationPredictor1.begin(), deviationPredictor1.end());
            break;
        case 1:
            residuals.insert(residuals.end(), deviationPredictor2.begin(), deviationPredictor2.end());
            break;
        case 2:
            residuals.insert(residuals.end(), deviationPredictor3.begin(), deviationPredictor3.end());
            break;
        case 3:
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
    finalSamples = Quantizer::dequantize(revertedAudioSamples, QUANTIZER_FACTOR);
    return 0;
}

vector<char> AdvancedPredictor::getUsedPredictorVector() {
    return usedPredictor;
}

vector<short> AdvancedPredictor::getRevertSamples() {
    return finalSamples;
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