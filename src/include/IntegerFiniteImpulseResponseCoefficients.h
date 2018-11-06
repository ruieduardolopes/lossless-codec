#ifndef INTEGERFINITEIMPULSERESPONSECOEFFICIENTS_H
#define INTEGERFINITEIMPULSERESPONSECOEFFICIENTS_H

#include <vector>
#include <iostream>

using namespace std;

namespace IntegerFiniteImpulseResponseCoefficients {
    static char x_0() {
        return 0;
    }
    static char x_1(vector<char>& x, int n) {
        return x[n - 1];
    }
    static short x_1(vector<short>& x, int n) {
        return x[n - 1];
    }
    static int x_1(vector<int>& x, int n) {
        return x[n - 1];
    }
    static char x_2(vector<char>& x, int n) {
        return 2*x[n - 1] - x[n - 2];
    }
    static short x_2(vector<short>& x, int n) {
        return 2*x[n - 1] - x[n - 2];
    }
    static int x_2(vector<int>& x, int n) {
        return 2*x[n - 1] - x[n - 2];
    }
    static char x_3(vector<char>& x, int n) {
        return 3*x[n - 1] - 3*x[n - 2] + x[n - 3];
    }
    static short x_3(vector<short>& x, int n) {
        return 3*x[n - 1] - 3*x[n - 2] + x[n - 3];
    }
    static int x_3(vector<int>& x, int n) {
        return 3*x[n - 1] - 3*x[n - 2] + x[n - 3];
    }
    static char e_0(vector<char>& x, int n) {
        return x[n];
    }

    static int e_0(vector<short>& x, int n) {
        return x[n];
    }

    static int e_0(vector<int>& x, int n) {
        return x[n];
    }
    static char e_1(vector<char>& x, int n) {
        return e_0(x, n) - e_0(x, n-1);
    }

    static int e_1(vector<short>& x, int n) {
        return e_0(x, n) - e_0(x, n-1);
    }
    
    static int e_1(vector<int>& x, int n) {
        return e_0(x, n) - e_0(x, n-1);
    }
    static char e_2(vector<char>& x, int n) {
        return e_1(x, n) - e_1(x, n-1);
    }
    static int e_2(vector<short>& x, int n) {
        return e_1(x, n) - e_1(x, n-1);
    }
    static int e_2(vector<int>& x, int n) {
        return e_1(x, n) - e_1(x, n-1);
    }
    static char e_3(vector<char>& x, int n) {
        return e_2(x, n) - e_2(x, n-1);
    }
    static int e_3(vector<short>& x, int n) {
        return e_2(x, n) - e_2(x, n-1);
    }
    static int e_3(vector<int>& x, int n) {
        return e_2(x, n) - e_2(x, n-1);
    }
};

#endif