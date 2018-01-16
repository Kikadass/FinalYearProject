//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_NETWORK_H
#define FINALYEARPROJECT_NETWORK_H

#include "Neuron2.h"

class Network {
private:
    map<int, Neuron2> network;

public:
    Network(vector<Gene> genes);

};


#endif //FINALYEARPROJECT_NETWORK_H
