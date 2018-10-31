#ifndef ADVANCEDPREDICTOR_H
#define ADVANCEDPREDICTOR_H

#include "extmath.h"
#include "IntegerFiniteImpulseResponseCoefficients.h"
#include "Predictor.h"

using namespace IntegerFiniteImpulseResponseCoefficients;
using namespace std;

class AdvancedPredictor : public Predictor {
    public:
        int predict();
        int revert();
        vector<char> getUsedPredictorVector();
        char getUsedPredictorOn(int frame);
    private: 
        int resetVectors();
        vector<char> usedPredictor;
        vector<short> lastThreeSamples = vector<short>(3,0);
        vector<short> resultsPredictor1;
        vector<short> resultsPredictor2;
        vector<short> resultsPredictor3;
        vector<short> resultsPredictor4;
        vector<short> deviationPredictor1;
        vector<short> deviationPredictor2;
        vector<short> deviationPredictor3;
        vector<short> deviationPredictor4;
};

#endif