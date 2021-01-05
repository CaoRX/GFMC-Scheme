#include "funcs.hpp"
#include <random>
#include <cstdlib>
#include <iostream>

std::default_random_engine randomEngine;
std::uniform_real_distribution<double> RandomDouble(0.0, 1.0);

int mod(int a, int md) {
    return ((a % md) + md) % md;
}

void initRandom(unsigned seed = 0) {
    randomEngine = std::default_random_engine(seed);
    srand(seed);
}

double randomDouble() {
    return RandomDouble(randomEngine);
}
int randomInt(int n) {
    return rand() % n;
}

void flipBool(bool &x) {
    x = !x;
}

void printBit(ULL x, int bitN) {
    for (int i = 0; i < bitN; ++i) {
        putchar('0' + (x % 2)); x /= 2;
    }
}