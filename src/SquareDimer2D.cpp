#include "SquareDimer2D.hpp"
#include "funcs.hpp"
#include <iostream>
#include <vector>

using namespace std;

SquareDimer2D::SquareDimer2D() {
    n = nn; m = mm; V = VV;
    VOffSet = (V > 0) ? V : 0;
    // VOffSet >= V, so V - VOffset < 0

    int bondN = n * m * 2;
    dMem = new bool[bondN];
    d = new bool**[2];
    for (int dir = 0; dir < 2; ++dir) {
        d[dir] = new bool*[n];
        for (int x = 0; x < n; ++x) {
            d[dir][x] = dMem + dir * n * m + x * m;
            for (int y = 0; y < m; ++y) {
                d[dir][x][y] = (dir == 0 && x % 2 == 0);
                // std::cerr << "(" << dir << ", " << x << ", " << y << ") = " << d[dir][x][y] << std::endl;
            }
        }
    }
    // printBit(hash(), bondN);
    // putchar('\n');
    // std::cout << isFP(0, 0) << std::endl;
}
SquareDimer2D::SquareDimer2D(const SquareDimer2D &a) {
    n = a.n; m = a.m; V = a.V;
    VOffSet = a.VOffSet;

    int bondN = n * m * 2;
    dMem = new bool[bondN];
    d = new bool**[2];
    for (int dir = 0; dir < 2; ++dir) {
        d[dir] = new bool*[n];
        for (int x = 0; x < n; ++x) {
            d[dir][x] = dMem + dir * n * m + x * m;
            for (int y = 0; y < m; ++y) {
                d[dir][x][y] = a.d[dir][x][y];
                // std::cerr << "(" << dir << ", " << x << ", " << y << ") = " << d[dir][x][y] << std::endl;
            }
        }
    }
}

bool SquareDimer2D::get(int dir, int x, int y) {
    x = mod(x, n);
    y = mod(y, m);
    return d[dir][x][y];
}
int SquareDimer2D::isFP(int x, int y) {
    // 1 & -1 for vertical / horizontal FP
    bool isH = true, isV = true;
    bool maskH = true, maskV = false;
    for (int i = 0; i < 4; ++i) {
        int dir = pDimerDir[i];
        int xx = x + pDimerDx[i], yy = y + pDimerDy[i];
        bool isDimer = get(dir, xx, yy);
        // std::cout << "isDimer = " << isDimer << std::endl;
        isH = isH && (maskH ^ isDimer);
        isV = isV && (maskV ^ isDimer);
        maskH = !maskH; maskV = !maskV;
    }
    // std::cout << isH << ' ' << isV << std::endl;
    if (isH) {
        return -1;
    }
    if (isV) {
        return 1;
    }
    return 0;
}

// double SquareDimer2D::flipDeltaE(int x, int y) {
//     int dFP = 0;
//     for (int i = 0; i < 4; ++i) {
//         if (isFP(x + dx[i], y + dy[i])) {
//             dFP -= 1;
//         }
//     }
//     flip(x, y);
//     for (int i = 0; i < 4; ++i) {
//         if (isFP(x + dx[i], y + dy[i])) {
//             dFP += 1;
//         }
//     }
//     flip(x, y);
//     return dFP * V;
// }
void SquareDimer2D::flip(int x, int y) {
    int xx, yy, dir;
    for (int i = 0; i < 4; ++i) {
        xx = mod(x + pDimerDx[i], n); 
        yy = mod(y + pDimerDy[i], m);
        dir = pDimerDir[i];
        flipBool(d[dir][xx][yy]);
    }
}
void SquareDimer2D::flipWinding() {
    for (int x = 0; x < n; ++x) {
        flipBool(d[0][x][0]);
    }
}
double SquareDimer2D::walk() {
    double currV = V - VOffSet; // negative
    double b = 0.0;
    std::vector<int> fx, fy;
    int countFP = 0;
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            if (isFP(x, y)) {
                fx.push_back(x); fy.push_back(y);
                countFP += 1;
            }
        }
    }
    // instead of V only on FPs, VOffset is on all the plaquettes
    // so our hamiltonian with -1.0 on the off-diagonal, and V - VOffset for FP, -VOffset on others
    // we choose VOffset > V
    // std::cout << "countFP = " << countFP << std::endl;
    b = (-currV + 1.0) * countFP + (VOffSet * (n * m - countFP));

    double stopPoss = ((-currV) * countFP + (VOffSet * (n * m - countFP))) / b;
    if (randomDouble() < stopPoss) {
        return b;
    }
    // std::cout << "here" << std::endl;
    int choice = randomInt(countFP);
    // std::cout << "here" << std::endl;
    flip(fx[choice], fy[choice]);
    return b;
}

double SquareDimer2D::getB() {
    double currV = V - VOffSet; // negative
    double b = 0.0;
    int countFP = 0;
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            if (isFP(x, y)) {
                countFP += 1;
            }
        }
    }
    // std::cout << "countFP = " << countFP << std::endl;
    // b = (-currV + 1.0) * countFP;
    b = (-currV + 1.0) * countFP + (VOffSet * (n * m - countFP));
    return b;
}

double SquareDimer2D::energyOffset() {
    // we add -VOffSet for every plaquette
    // so after we got e as the eigenvalue of the -H
    // as we have substracted VOffset * (m * n) diagonally from H
    // we should have the real energy -e + (m * n) * VOffSet
    return (m * n) * VOffSet;
}

ULL SquareDimer2D::hash() {
    ULL res = 0;
    for (int i = 0; i < n * m * 2; ++i) {
        if (dMem[i]) {
            res |= (1 << i);
        }
    }
    return res;
}

vector<double> SquareDimer2D::measureCorrelation() {
    double originDimer = dimerToDouble(get(0, 0, 0));
    vector<double> res(2 * n * m);
    for (int dir = 0; dir < 2; ++dir) {
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < m; ++y) {
                res[dir * m * n + x * m + y] = dimerToDouble(get(dir, x, y)) * originDimer;
            }
        }
    }
    return res;
}
double SquareDimer2D::dimerToDouble(bool dimer) {
    return dimer ? 0.75 : -0.25;
}

SquareDimer2D::~SquareDimer2D() {
    delete []dMem;
}