//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_POOL_H
#define FINALYEARPROJECT_POOL_H

#include "Species.h"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;


class Pool {
private:
    vector<Genome*> genomes;
    int generation;
    int currentGenome;
    vector<int> maxFitness;
    vector<int> averageFitness;
    vector<int> totalFitness;
    int nBest;
    int nSurvivors;

public:
    static int ScreenHeight;
    static int ScreenWidth;

    static int INPUT_SIZE;

    //number of buttons
    static int OUTPUT_SIZE;

    static int POPULATION;

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

    vector<int> loadFitness(string name, json pool);

    void loadPool(string loadLocation);

    void savePool(string location);

    vector<Genome*> &getGenomes();

    int getGeneration();

    int getCurrentGenome();

    const vector<int> &getMaxFitness() const;

    const vector<int> &getAverageFitness() const;

    const vector<int> &getTotalFitness() const;


    void calculateFitness();

    void nextGenome(string saveLocation);

    void newGeneration();

    void cullSpecies(bool keepBest);

    Genome *crossover(Genome *g1, Genome *g2);

    Genome *breedChild(int index);

    vector<Genome*> rouletteSelection();

    vector<Genome *> eliteSelection();

    vector<Genome *> copyGenomes();

    string createFitnessStr(vector<int> fitness, string name);
};


#endif //FINALYEARPROJECT_POOL_H
