#include "AdvancedPredictor.h"

#include <iostream>

int AdvancedPredictor::predict() {
    evaluatePredictions();
    return 0;
}

int AdvancedPredictor::revert() {

    return 0;
}

vector<char> AdvancedPredictor::getUsedPredictorVector() {
    return usedPredictor;
}

char AdvancedPredictor::getUsedPredictorOn(int frame) {
    return usedPredictor[frame];
}

/**
 * @brief 
 * 
 * Let's consider that originalAudioSamples is a frame.
 * 
 * @return int 
 */
int AdvancedPredictor::evaluatePredictions() {
    resetVectors();
    // Fill results from predictors
    for (int n = 0; n != originalAudioSamples.size(); n++) {
        resultsPredictor1.push_back(x_0());
        deviationPredictor1.push_back(e_0(originalAudioSamples, n));

        if (n == 0) {
            resultsPredictor2.push_back(originalAudioSamples[n]);
            deviationPredictor2.push_back(0);
        } else {
            resultsPredictor2.push_back(x_1(originalAudioSamples, n));
            deviationPredictor2.push_back(e_1(originalAudioSamples, n));
        }

        if (n <= 1) {
            resultsPredictor3.push_back(originalAudioSamples[n]);
            deviationPredictor3.push_back(0);
        } else {
            resultsPredictor3.push_back(x_2(originalAudioSamples, n));
            deviationPredictor3.push_back(e_2(originalAudioSamples, n));
        }

        if (n <= 2) {
            resultsPredictor4.push_back(originalAudioSamples[n]);
            deviationPredictor4.push_back(0);
        } else {
            resultsPredictor4.push_back(x_3(originalAudioSamples, n));
            deviationPredictor4.push_back(e_3(originalAudioSamples, n));
        }
    }
    
    // cout << "\n\n1:  " << endl;
    // for (auto element : resultsPredictor1) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;
    // cout << "2:  " << endl;
    // for (auto element : resultsPredictor2) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;
    // cout << "3:  " << endl;
    // for (auto element : resultsPredictor3) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;
    // cout << "4:  " << endl;
    // for (auto element : resultsPredictor4) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;

    // cout << "\n\n1:  " << endl;
    // for (auto element : deviationPredictor1) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;
    // cout << "2:  " << endl;
    // for (auto element : deviationPredictor2) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;
    // cout << "3:  " << endl;
    // for (auto element : deviationPredictor3) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;
    // cout << "4:  " << endl;
    // for (auto element : deviationPredictor4) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;

    // Evaluate the smaller deviation, to choose the predictor
    vector<double> meanDeviations;
    meanDeviations.push_back(extmath::mean(deviationPredictor1));
    meanDeviations.push_back(extmath::mean(deviationPredictor2));
    meanDeviations.push_back(extmath::mean(deviationPredictor3));
    meanDeviations.push_back(extmath::mean(deviationPredictor4));
    // cout << "\n\nDeviations:" << endl;
    // for (auto element : meanDeviations) {
    //     cout << element << "  ";
    // }
    // cout << endl << endl;
    char predictorIndex = extmath::minIndex(meanDeviations);
    usedPredictor.push_back(predictorIndex);

    // apply the predictions
    switch (predictorIndex) {
        case 0:
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor1.begin(), resultsPredictor1.end());
            break;
        case 1:
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor2.begin(), resultsPredictor2.end());
            break;
        case 2:
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor3.begin(), resultsPredictor3.end());
            break;
        case 3:
            predictedAudioSamples.insert(predictedAudioSamples.end(), resultsPredictor4.begin(), resultsPredictor4.end());
            break;
        default:
            break; // TODO : handle this case
    }


    return 0;
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