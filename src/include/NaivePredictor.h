#ifndef NAIVEPREDICTOR_H
#define NAIVEPREDICTOR_H

#include "Predictor.h"

using namespace std;

class NaivePredictor : public Predictor {
    public:
        int predict();
        int revert();
};

#endif