#include <iostream>
#include "GFMC.hpp"
#include "SquareDimer2D.hpp"
#include "funcs.hpp"
#include <ctime>

using namespace std;

// const int nn = 4, mm = 4;

int SquareDimer2D::nn = 16;
int SquareDimer2D::mm = 16;
double SquareDimer2D::VV = 1.0;
const int loopN = 1000000;
string fileName = "corr1616101e6.dat";

const int maxL = 5;
const int maxN = 4;
const int M = 10;
const int kb = 10;

int main() {
    initRandom((unsigned)time(NULL));
    GFMCScheme<SquareDimer2D> gfmc(kb, M);
    gfmc.simulate(loopN);
    // for (int i = 0; i < 5; ++i) {
    //     gfmc.walkOneStep();
    // }
    int nn = SquareDimer2D::nn, mm = SquareDimer2D::mm;
    // for (int i = 0; i < M; ++i) {
    //     // cout << "i = " << i << endl;
    //     cout << gfmc.walkers[i].w << ' ';
    //     printBit(gfmc.walkers[i].conf.hash(), 2 * nn * mm);
    //     putchar('\n');
    // }
    // cout << endl;
    // cout << "Hello world!" << endl;
    // cout << randomDouble() << endl;
    // double VOffset = gfmc.walkers[0].conf.VOffSet;

    int systemSize = SquareDimer2D::mm * SquareDimer2D::nn;
    gfmc.generateG(maxL, maxN);

    cout << "ground state energy = " << gfmc.averageGSEnergy() / systemSize << endl;
    double averageEnergy = gfmc.greenAverageGSEnergy() / systemSize;
    cout << "green ground state energy = " << averageEnergy << endl;

    freopen(fileName.c_str(), "w", stdout);
    for (int rN = maxN; rN <= maxN; ++rN) {
        vector<double> corr = gfmc.greenAverageGSCorr(rN);
        for (int i = 0; i < corr.size(); ++i) {
            cout << corr[i] << ' ';
        }
        cout << endl;
    }
    cout << averageEnergy << endl;
    // for (int j = 0; j < maxL + maxN; ++j) {
    //     for (int i = 0; i < gfmc.G[j].size(); ++i) {
    //         cout << gfmc.G[j][i] << ' ';
    //     }
    //     cout << endl;
    // }
    fclose(stdout);

    return 0;
}