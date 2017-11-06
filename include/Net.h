//
// Created by Kike Piera Serra on 05/11/2017.
//

#ifndef FINALYEARPROJECT_NET_H
#define FINALYEARPROJECT_NET_H
#include <vector>
#include <iostream>
#include "Neuron.h"

using namespace std;

class Net {
public:
    Net(const vector<double> &topology);
    void feedForward(vector<double> &inputs);
    void backProp(vector<double> &targets);
    void getResults(vector<double> &results);
    double getRecentAverageError();

private:
    double error;
    double recentAverageError;
    double recentAverageSmoothingFac;
    vector<vector<Neuron>> m_layers; //m_layers[layerNum][neuronNum]

};


#endif //FINALYEARPROJECT_NET_H
