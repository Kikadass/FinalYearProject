//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_GENOME_H
#define FINALYEARPROJECT_GENOME_H


#include "Gene.h"
#include "Neuron2.h"

struct MutationRates{
    double connections;
    double link;
    double bias;
    double node;
    double enable;
    double disable;
    double step;
};

class Genome {
public:
    Genome();

    //GETTERS
    int getGlobalRank();
    const vector<Gene> getGenes() const;
    int getFitness() const;
    int getMaxNeuron() const;
    MutationRates getMutationRates();

    //SETTERS
    void addGene(Gene g);
    void setMaxNeuron(int maxNeuron);
    void copyMutationRates(MutationRates mR);
    void mutate();
    void pointMutate();
    void linkMutate(bool forceBias);
    void firstGenome();
    void generateNetwork();


private:
    vector<Gene> genes;
    int fitness;
    int adjustedFitness;
    map<int, Neuron2> network;
    int maxNeuron;
    int globalRank;     // fitness for that genome
    MutationRates mutationRates;

    void nodeMutate();

    void enableDisableMutate(bool enable);

    int randomNeuron(bool nonInput);

    bool containsLink(Gene link);

};


#endif //FINALYEARPROJECT_GENOME_H
