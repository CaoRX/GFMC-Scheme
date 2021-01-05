#include <iostream>
#include "GFMC.hpp"
#include "SquareDimer2D.hpp"
#include "funcs.hpp"

using namespace std;

const int nn = 4, mm = 4;

int SquareDimer2D::nn = 4;
int SquareDimer2D::mm = 4;
double SquareDimer2D::VV = 0.5;

const int M = 10;
const int kb = 10;

int main() {
    GFMCScheme<SquareDimer2D> gfmc(kb, M);
    gfmc.simulate(10);
    // for (int i = 0; i < 5; ++i) {
    //     gfmc.walkOneStep();
    // }
    for (int i = 0; i < M; ++i) {
        // cout << "i = " << i << endl;
        cout << gfmc.walkers[i].w << ' ';
        printBit(gfmc.walkers[i].conf.hash(), 2 * nn * mm);
        putchar('\n');
    }
    // cout << endl;
    // cout << "Hello world!" << endl;
    // cout << randomDouble() << endl;
    return 0;
}