//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_POOL_H
#define FINALYEARPROJECT_POOL_H

#include "Species.h"


class Pool {
private:
    vector<Genome*> genomes;
    int generation;
    int currentGenome;
    int maxFitness;
    int averageFitness;
    int nBest;

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

    void loadPool(string loadLocation);

    void savePool(string location);

    vector<Genome*> &getGenomes();

    int getGeneration();

    int getCurrentGenome();

    void calculateFitness();

    void addToGenome(Genome* child);

    double disjoint(vector<Gene*> genes1, vector<Gene*> genes2);

    double weights(vector<Gene*> genes1, vector<Gene*> genes2);

    void nextGenome(string saveLocation);

    void newGeneration();

    void cullSpecies(bool keepBest);

    void rankGlobally();

    Genome *crossover(Genome *g1, Genome *g2);

    Genome *breedChild();
};


#endif //FINALYEARPROJECT_POOL_H
