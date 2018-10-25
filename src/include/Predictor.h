#ifndef PREDICTOR_H
#define PREDICTOR_H

class Predictor {
    public:
        int predict(int value1, int value2);
    private:
        int sample;
};

#endif