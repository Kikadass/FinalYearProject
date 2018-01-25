//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_SPECIES_H
#define FINALYEARPROJECT_SPECIES_H

#include "Genome.h"
#include <map>


class Species {
public:
    Species(vector<Genome> genomes);

    //GETTERS
    vector<Genome> &getGenomes();
    int getAverageFitness() const;
    int getTopFitness() const;
    int getStaleness() const;

    //SETTERS
    void setTopFitness(int topFitness);
    void setStaleness(int staleness);
    void calculateFitness();

    Genome breedChild();
    void addChild(Genome child);
    Genome crossover(Genome g1, Genome g2);



private:
    int topFitness;
    int staleness;
    vector<Genome> genomes;
    int averageFitness;

};


#endif //FINALYEARPROJECT_SPECIES_H
