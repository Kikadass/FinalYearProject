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

    Genome(vector<Gene *> genes/*, map<int, Neuron2> network*/, int fitness, int adjustedFitness, int lastNeuronCreated,
               int globalRank, MutationRates mutationRates);

    //GETTERS
    vector<Gene*> getGenes();
    int getGlobalRank();
    int getFitness();
    int getAdjustedFitness();
    int getLastNeuronCreated();
    int getMaxNeuron();
    MutationRates getMutationRates();
    void showGenome();

    //SETTERS
    void addGene(Gene* g);
    void setFitness(int fitness);
    void setLastNeuronCreated(int lastNeuronCreated);
    void setGlobalRank(int globalRank);
    void copyMutationRates(MutationRates mR);
    void mutate();
    void weightMutate();
    void linkMutate(bool forceBias);
    void firstGenome();
    void generateNetwork();
    int evaluateNetwork(vector<double> inputs);
    void evaluateNetworkToFile(vector<double> inputs);
     // returns a value between 0 and 1 with a maximum of 2 decimals
    static float randomPercentage(){
        return ((float)(rand()%101))/100.0f;
    }



private:
    vector<Gene*> genes;
    int fitness;
    int adjustedFitness;
    map<int, Neuron2> network;
    int lastNeuronCreated;      // this is the index of the last neuron created not taking into account the output neurons
    int globalRank;

    // in which rank they are in
    MutationRates mutationRates;

    void nodeMutate();

    void enableDisableMutate(bool enable);

    int randomNeuron(bool nonInput);

    bool containsLink(Gene* link);

    void showGene(int iterations, int neuron);

    double evaluateNeuron(int neuron);
};


#endif //FINALYEARPROJECT_GENOME_H
