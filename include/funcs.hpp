#pragma once

#include "settings.hpp"

const int hdx[2] = {0, 0};
const int hdy[2] = {1, -1};
const int vdx[2] = {1, -1};
const int vdy[2] = {0, 0};
const int dx[4] = {0, 0, 1, -1};
const int dy[4] = {1, -1, 0, 0};

extern int mod(int x, int md);
extern double randomDouble();
extern int randomInt(int n);
extern void initRandom(unsigned seed);
extern void flipBool(bool &x);
extern void printBit(ULL x, int bitN);