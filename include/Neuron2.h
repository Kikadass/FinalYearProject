//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_NEURON2_H
#define FINALYEARPROJECT_NEURON2_H

#include "Gene.h"

class Neuron2 {
public:
    Neuron2();
    void addGene(Gene gene);
    void setValue(double x);

private:
    vector<Gene> genes;
    double value;


    double randomWeight();

};


#endif //FINALYEARPROJECT_NEURON2_H
