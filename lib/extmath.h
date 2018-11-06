#ifndef EXTMATH_H
#define EXTMATH_H

#include <climits>
#include <cmath>
#include <vector>
#include <cstdint>

using namespace std;

class extmath {
    public:
        static double lb(int x);                    // binary logarithm of x
        static double mean(vector<int> set);      // mean value of a short vector
        static short min(vector<short> set);        // minimum absolute value of a short vector
        static short minIndex(vector<double> set);  // minimum absolute value index of a double vector
        static vector<short> add(vector<short> left, vector<short> right);
};

#endif