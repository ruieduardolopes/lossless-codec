#ifndef EXTMATH_H
#define EXTMATH_H

#include <climits>
#include <cmath>
#include <vector>
#include <cmath>

using namespace std;

class extmath {
    public:
        static double lb(int x);                    // binary logarithm of x
        static double mean(vector<short> set);      // mean value of a short vector
        static short min(vector<short> set);        // minimum absolute value of a short vector
        static short minIndex(vector<double> set);  // minimum absolute value index of a double vector
};

#endif