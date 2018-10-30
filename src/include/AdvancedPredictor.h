#ifndef ADVANCEDPREDICTOR_H
#define ADVANCEDPREDICTOR_H

#include "IntegerFiniteImpulseResponseCoefficients.h"
#include "Predictor.h"

using namespace IntegerFiniteImpulseResponseCoefficients;
using namespace std;

class AdvancedPredictor : public Predictor {
    public:
        int predict();
        int revert();
    private: 
        int evaluatePredictions();
        vector<char> predictorUsed;
        vector<int> predictions;
        vector<int> deviations;
};

#endif