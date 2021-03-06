//
// Created by Kike Piera Serra on 08/01/2018.
//

#include "Neuron2.h"

Neuron2::Neuron2() {
    Neuron2::value = -20;
}

void Neuron2::addGene(Gene gene){
    genes.push_back(gene);
}

double Neuron2::randomWeight(){
    return rand() /double{RAND_MAX};
}

void Neuron2::setValue(double x) {
    value = x;
}

vector<Gene> &Neuron2::getGenes() {
    return genes;
}

const double Neuron2::getValue() const{
    return value;
}
