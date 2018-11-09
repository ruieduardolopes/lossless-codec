#ifndef ADVANCEDPREDICTOR_H
#define ADVANCEDPREDICTOR_H

#include "extmath.h"
#include "IntegerFiniteImpulseResponseCoefficients.h"
#include "Predictor.h"
#include "Quantizer.h"

using namespace IntegerFiniteImpulseResponseCoefficients;
using namespace std;

class AdvancedPredictor : public Predictor {
    public:
        AdvancedPredictor(int quantizeFactor);
        int predict();
        int revert();
        vector<char> getUsedPredictorVector();
        char getUsedPredictorOn(int frame);
        void setUsedPredictor(vector<char>& used);
        vector<int> getResiduals();
        void setResiduals(vector<int>& deviations);
        vector<short> getRevertSamples();
    private: 
        // Quantizer quantizer;
        int resetVectors();
        int verifyVectorCompleteness();
        vector<char> usedPredictor;
        vector<short> lastThreeSamples = vector<short>(3,0);
        vector<short> resultsPredictor1;
        vector<short> resultsPredictor2;
        vector<short> resultsPredictor3;
        vector<short> resultsPredictor4;
        vector<int> deviationPredictor1;
        vector<int> deviationPredictor2;
        vector<int> deviationPredictor3;
        vector<int> deviationPredictor4;
        vector<int> residuals;
        vector<short> finalSamples;
};

#endif