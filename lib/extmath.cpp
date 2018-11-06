#include "extmath.h"

double extmath::lb(int x) {
    return log10(x) / log10(2);
}

double extmath::mean(vector<int> set) {
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

vector<short> extmath::add(vector<short> left, vector<short> right){
    vector<short> tmp;
    if(left.size() >= right.size()){
        for(int i = 0; i < right.size(); i++){
            tmp.push_back(left[i] + right[i]);
        }
        for(int i = 0; i < left.size()-right.size(); i++){
            tmp.push_back(0);
        }
    } else {
         for(int i = 0; i < left.size(); i++){
            tmp.push_back(right[i] + left[i]);
        }
        for(int i = 0; i < right.size()-left.size(); i++){
            tmp.push_back(0);
        }
    }
    return tmp;
}