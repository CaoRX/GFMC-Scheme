#include <iostream>
#include "GFMC.hpp"
#include "SquareDimer2D.hpp"
#include "funcs.hpp"
#include <ctime>

using namespace std;

// const int nn = 4, mm = 4;

int SquareDimer2D::nn = 4;
int SquareDimer2D::mm = 4;
double SquareDimer2D::VV = 0.9;

const int maxL = 5;
const int maxN = 4;
const int M = 20;
const int kb = 10;

int main() {
    initRandom((unsigned)time(NULL));
    GFMCScheme<SquareDimer2D> gfmc(kb, M);
    gfmc.simulate(10000);
    // for (int i = 0; i < 5; ++i) {
    //     gfmc.walkOneStep();
    // }
    int nn = SquareDimer2D::nn, mm = SquareDimer2D::mm;
    for (int i = 0; i < M; ++i) {
        // cout << "i = " << i << endl;
        cout << gfmc.walkers[i].w << ' ';
        printBit(gfmc.walkers[i].conf.hash(), 2 * nn * mm);
        putchar('\n');
    }
    // cout << endl;
    // cout << "Hello world!" << endl;
    // cout << randomDouble() << endl;
    // double VOffset = gfmc.walkers[0].conf.VOffSet;

    int systemSize = SquareDimer2D::mm * SquareDimer2D::nn;
    gfmc.generateG(maxL, maxN);

    cout << "ground state energy = " << gfmc.averageGSEnergy() / systemSize << endl;
    cout << "green ground state energy = " << gfmc.greenAverageGSEnergy() / systemSize << endl;

    freopen("output.dat", "w", stdout);
    for (int j = 0; j < maxL + maxN; ++j) {
        for (int i = 0; i < gfmc.G[j].size(); ++i) {
            cout << gfmc.G[j][i] << ' ';
        }
        cout << endl;
    }
    fclose(stdout);

    return 0;
}