//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_POOL_H
#define FINALYEARPROJECT_POOL_H

#include "Species.h"

class Pool {
private:
    vector<Species> species;
    int generation;
    int currentSpecies;
    int currentGenome;
    int currentFrame;
    int maxFitness;

public:
    static int ScreenHeight;
    static int ScreenWidth;

    static int INPUT_SIZE;

    //number of buttons
    static int OUTPUT_SIZE;

    static int POPULATION;
    static double DELTA_DISJOINT;
    static double DELTA_WEIGHTS;
    static double DELTA_THRESHOLD;

    static int STALE_SPECIES;

    static double PerturbChance;
    static double CrossoverChance;

    static double ConnMutateChance;
    static double LinkMutateChance;
    static double NodeMutateChance;
    static double BiasMutateChance;
    static double EnableMutateChance;
    static double DisableMutateChance;
    static double StepSize;


    static int TIMEOUT_CONSTANT;

    static int MaxNodes;

    Pool();

    vector<Species> &getSpecies();

    int getGeneration();

    int getCurrentSpecies();

    int getCurrentGenome();

    int calculateFitness();

    void removeStaleSpecies();

    void removeWeakSpecies();

    void addToSpecies(Genome child);

    bool sameSpecies(Genome genome1, Genome genome2);

    double disjoint(vector<Gene> genes1, vector<Gene> genes2);

    double weights(vector<Gene> genes1, vector<Gene> genes2);

    void nextGenome();

    void newGeneration();

    int poolAverageFitness();

    void cullSpecies(bool cutToOne);

    void rankGlobally();
};


#endif //FINALYEARPROJECT_POOL_H
