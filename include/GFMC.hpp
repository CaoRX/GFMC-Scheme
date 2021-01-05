#pragma once

#include "funcs.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

template<typename Conf>
class Walker {
public:
    Conf conf;
    double w;
    Walker(double _w = 1.0);
    Walker(const Conf &c, double _w);
    void walk();
    Walker<Conf> copy();
};

template<typename Conf>
class GFMCScheme {
public:
    vector<Walker<Conf> > walkers;
    int kb, M;
    vector<double> wBar;

    GFMCScheme(int _kb, int _M);
    void walkOneStep();
    void reconfiguration();
    void simulate(int loops);
};

template<typename Conf>
Walker<Conf>::Walker(double _w): w(_w) {}

template<typename Conf>
Walker<Conf>::Walker(const Conf &c, double _w): conf(c), w(_w) {}

template<typename Conf>
void Walker<Conf>::walk() {
    double b = conf.walk();
    w *= b;
}

template<typename Conf>
Walker<Conf> Walker<Conf>::copy() {
    return Walker<Conf>(conf, w);
}

template<typename Conf>
GFMCScheme<Conf>::GFMCScheme(int _kb, int _M): kb(_kb), M(_M) {
    walkers = vector<Walker<Conf> >(M);
}
template<typename Conf>
void GFMCScheme<Conf>::walkOneStep() {
    for (int i = 0; i < M; ++i) {
        walkers[i].walk();
    }
    wBar.clear();
}
template<typename Conf>
void GFMCScheme<Conf>::reconfiguration() {
    double totw = 0.0;
    for (int i = 0; i < M; ++i) {
        totw += walkers[i].w;
    }
    wBar.push_back(totw / M);
    vector<Walker<Conf> > newWalkers;
    for (int i = 0; i < M; ++i) {
        double choice = randomDouble() * totw;
        for (int j = 0; j < M; ++j) {
            choice -= walkers[j].w;
            if (choice < 0) {
                newWalkers.push_back(walkers[j].copy());
                newWalkers.back().w = 1.0;
                break;
            }
        }
    }
    swap(newWalkers, walkers);
    newWalkers.clear();
}

template<typename Conf>
void GFMCScheme<Conf>::simulate(int loops) {
    for (int i = 0; i < loops; ++i) {
        // std::cout << "loop " << i << std::endl;
        walkOneStep();
        if ((i + 1) % kb == 0) {
            reconfiguration();
        }
    }
}