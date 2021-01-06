#pragma once

#include "settings.hpp"
#include <vector>
using namespace std;

const int pDimerDx[4] = {0, 0, 0, 1};
const int pDimerDy[4] = {0, 0, 1, 0};
const int pDimerDir[4] = {0, 1, 0, 1};

const int sDimerDx[4] = {0, 0, -1, 0};
const int sDimerDy[4] = {0, 0, 0, -1};
const int sDimerDir[4] = {0, 1, 0, 1};

const int sSiteDx[4] = {1, 0, -1, 0};
const int sSiteDy[4] = {0, 1, 0, -1};

const int dSiteDx[2][2] = {{0, 1}, {0, 0}};
const int dSiteDy[2][2] = {{0, 0}, {0, 1}};

class SquareDimer2D {
public:
    static int nn, mm;
    static double VV;

    double V, VOffSet;
    int n, m;
    bool *dMem;
    bool ***d;

    SquareDimer2D();
    SquareDimer2D(const SquareDimer2D &a);
    ~SquareDimer2D();

    bool get(int dir, int x, int y);
    int isFP(int x, int y);
    // double flipDeltaE(int x, int y);
    void flip(int x, int y);
    void flipWinding();

    double walk();
    double getB();
    ULL hash();

    double energyOffset();
    vector<double> measureCorrelation();
    double dimerToDouble(bool dimer);
};