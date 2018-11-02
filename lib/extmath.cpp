#include "extmath.h"

double extmath::lb(int x) {
    return log10(x) / log10(2);
}

double extmath::mean(vector<short> set) {
    double sum;
    for (auto element : set) {
        sum += element;
    }
    return sum/set.size();
}

short extmath::min(vector<short> set) {
    short minimum = INT16_MAX;
    for (auto element : set) {
        if (abs(element) < minimum) {
            minimum = abs(element);
        }
    }
    return minimum;
}

short extmath::minIndex(vector<double> set) {
    short minimum = INT16_MAX;
    short minimumIndex;
    for (int index = 0; index != set.size(); index++) {
        if (abs(set[index]) < minimum) {
            minimum = abs(set[index]);
            minimumIndex = index;
        }
    }
    return minimumIndex;
}