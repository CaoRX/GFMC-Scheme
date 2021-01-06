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
    vector<double> energy;
    int retardN;
    int greenL;
    vector<vector<double> > G;

    GFMCScheme(int _kb, int _M);
    void walkOneStep();
    void reconfiguration();
    void simulate(int loops);

    double GSEnergy();
    double averageGSEnergy();
    double greenAverageGSEnergy();
    void generateG(int maxL, int maxN);
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
    greenL = 0; retardN = 0;
}
template<typename Conf>
void GFMCScheme<Conf>::walkOneStep() {
    for (int i = 0; i < M; ++i) {
        walkers[i].walk();
    }
}
template<typename Conf>
void GFMCScheme<Conf>::reconfiguration() {
    // cout << "reconfiguration()" << endl;
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
            energy.push_back(GSEnergy());
            reconfiguration();
        }
    }
}

template<typename Conf>
double GFMCScheme<Conf>::GSEnergy() {
    // measurement before reconfiguration
    // \Sigma w_i b_i / Sigma w_i
    double wSum = 0.0, wbSum = 0.0;
    for (int i = 0; i < M; ++i) {
        wSum += walkers[i].w;
        wbSum += walkers[i].w * walkers[i].conf.getB();
    }
    return wbSum / wSum;
}

template<typename Conf>
double GFMCScheme<Conf>::averageGSEnergy() {
    double tot = 0.0;
    for (int i = 0; i < energy.size(); ++i) {
        tot += energy[i];
    }
    double average = tot / energy.size();
    return -average + walkers[0].conf.energyOffset();
}

template<typename Conf>
double GFMCScheme<Conf>::greenAverageGSEnergy() {
    double tot = 0.0, totw = 0.0;
    for (int i = greenL - 1; i < energy.size(); ++i) {
        tot += G[greenL - 1][i] * energy[i];
        totw += G[greenL - 1][i];
    }
    return -tot / totw + walkers[0].conf.energyOffset();
}

template<typename Conf>
void GFMCScheme<Conf>::generateG(int maxL, int maxN) {
    retardN = maxN;
    greenL = maxL;
    int n = wBar.size();
    // cout << "wBar size = " << n << endl;
    // maxL = min(maxL, n);
    int totL = maxL + maxN;
    G = vector<vector<double> >(totL);
    G[0] = wBar;
    for (int i = 1; i < totL; ++i) {
        G[i] = vector<double>(n, 0.0);
        for (int j = i; j < n; ++j) {
            G[i][j] = G[i - 1][j - 1] * wBar[j];
        }
    }
}