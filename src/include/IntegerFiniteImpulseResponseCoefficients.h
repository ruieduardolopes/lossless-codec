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
    static short e_0(vector<short>& x, int n) {
        return x[n];
    }
    static int e_0(vector<int>& x, int n) {
        return x[n];
    }
    static char e_1(vector<char>& x, int n) {
        return e_0(x, n) - e_0(x, n-1);
    }
    static short e_1(vector<short>& x, int n) {
        return e_0(x, n) - e_0(x, n-1);
    }
    static int e_1(vector<int>& x, int n) {
        return e_0(x, n) - e_0(x, n-1);
    }
    static char e_2(vector<char>& x, int n) {
        return e_1(x, n) - e_1(x, n-1);
    }
    static short e_2(vector<short>& x, int n) {
        return e_1(x, n) - e_1(x, n-1);
    }
    static int e_2(vector<int>& x, int n) {
        return e_1(x, n) - e_1(x, n-1);
    }
    static char e_3(vector<char>& x, int n) {
        return e_2(x, n) - e_2(x, n-1);
    }
    static short e_3(vector<short>& x, int n) {
        return e_2(x, n) - e_2(x, n-1);
    }
    static int e_3(vector<int>& x, int n) {
        return e_2(x, n) - e_2(x, n-1);
    }
    static char x_0_sym() {
        return 0;
    }
    static char x_1_sym(vector<char>& p, int i) {
        return p[i];
    }
    static short x_1_sym(vector<short>& p, int i) {
        return p[i];
    }
    static int x_1_sym(vector<int>& p, int i) {
        return p[i];
    }
    static char x_2_sym(vector<char>& p, vector<char>& d, int i) {
        return (d[(i+3) - 1] + p[i]) / 2;
    }
    static short x_2_sym(vector<short>& p, vector<short>& d, int i) {
        cout << "d[i] = (d[i-1] + p[i]) / 2 = " << (d[(i+3) - 1]) << " + " << p[i] << " / 2 = " << ((d[(i+3) - 1] + p[i]) / 2) << endl;
        return (d[(i+3) - 1] + p[i]) / 2;
    }
    static int x_2_sym(vector<int>& p, vector<int>& d, int i) {
        return (d[(i+3) - 1] + p[i]) / 2;
    }
    static char x_3_sym(vector<char>& p, vector<char>& d, int i) {
        return (p[i] + 3*d[(i+3) - 1] - d[(i+3) - 2]) / 3;
    }
    static short x_3_sym(vector<short>& p, vector<short>& d, int i) {
        return (p[i] + 3*d[(i+3) - 1] - d[(i+3) - 2]) / 3;
    }
    static int x_3_sym(vector<int>& p, vector<int>& d, int i) {
        return (p[i] + 3*d[(i+3) - 1] - d[(i+3) - 2]) / 3;
    }
};

#endif