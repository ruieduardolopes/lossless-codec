#include "AdvancedPredictor.h"

#include <iostream>

int AdvancedPredictor::predict() {
    resetVectors();
    // Fill results from predictors
    vector<short> samples = originalAudioSamples;
    samples.insert(samples.begin(), lastThreeSamples.begin(), lastThreeSamples.end());
    samples.push_back(0);

    // for (auto element : samples) {
    //     cout << element << "  ";
    // }
    // cout << endl;

    for (int n = 0; n != samples.size(); n++) {
        if (n > 3) {
            resultsPredictor1.push_back(x_0());
            deviationPredictor1.push_back(e_0(samples, n));
        }

        if (n > 2) {
            resultsPredictor2.push_back(x_1(samples, n));
            deviationPredictor2.push_back(e_1(samples, n));
        }

        if (n > 1) {
            resultsPredictor3.push_back(x_2(samples, n));
            deviationPredictor3.push_back(e_2(samples, n));
        }

        resultsPredictor4.push_back(x_3(samples, n));
        deviationPredictor4.push_back(e_3(samples, n));        
    }

    // cout << "Predictor 0: ";
    // for (auto element : resultsPredictor1) {
    //     cout << element << "  ";
    // }
    // cout << endl;

    // cout << "Predictor 1: ";
    // for (auto element : resultsPredictor2) {
    //     cout << element << "  ";
    // }
    // cout << endl;

    // cout << "Predictor 2: ";
    // for (auto element : resultsPredictor3) {
    //     cout << element << "  ";
    // }
    // cout << endl;

    // cout << "Predictor 3: ";
    // for (auto element : resultsPredictor4) {
    //     cout << element << "  ";
    // }
    // cout << endl;

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
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor1.begin(), resultsPredictor1.end());
            break;
        case 1:
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor2.begin()+1, resultsPredictor2.end());
            break;
        case 2:
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor3.begin()+2, resultsPredictor3.end());
            break;
        case 3:
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor4.begin()+3, resultsPredictor4.end());
            break;
        default:
            break; // TODO : handle this case
    }

    return 0;
}

int AdvancedPredictor::revert() {
    resetVectors();
    int currentPredictor;
    revertedAudioSamples.push_back(0);
    revertedAudioSamples.push_back(0);
    revertedAudioSamples.push_back(0);
    for (int n = 0; n != predictedAudioSamples.size(); n++) {
        if (n % (framesBufferSize+3) == 0) {
            currentPredictor = usedPredictor[n/(framesBufferSize+3)];
            cout << "Decoding with predictor number " << currentPredictor << endl;
        }
        switch (currentPredictor) {
            case 0: 
                revertedAudioSamples.push_back(x_0_sym());
                break;
            case 1:
                revertedAudioSamples.push_back(x_1_sym(predictedAudioSamples, n));
                break;
            case 2:
                revertedAudioSamples.push_back(x_2_sym(predictedAudioSamples, revertedAudioSamples, n));
                break;
            case 3:
                revertedAudioSamples.push_back(x_3_sym(predictedAudioSamples, revertedAudioSamples, n));
                break;
            default:
                break; // TODO : handle this case
        }  
    }
    revertedAudioSamples.erase(revertedAudioSamples.begin(), revertedAudioSamples.begin()+3);
    return 0;
}

vector<char> AdvancedPredictor::getUsedPredictorVector() {
    return usedPredictor;
}

char AdvancedPredictor::getUsedPredictorOn(int frame) {
    return usedPredictor[frame];
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