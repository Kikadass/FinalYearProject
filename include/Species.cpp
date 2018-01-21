//
// Created by Kike Piera Serra on 08/01/2018.
//

#include "Species.h"

Species::Species(vector<Genome> genomes) {
    topFitness = 0;
    staleness = 0;
    averageFitness = 0;
    Species::genomes = genomes;
}

void Species::calculateFitness() {
    int total = 0;
    int max = 0;

    for (int g = 0; g < genomes.size(); g++) {
        int genomeRank = genomes[g].getGlobalRank();

        if (genomeRank > max) max = genomeRank;

        total += genomeRank;
    }

    topFitness = max;

    averageFitness = total / genomes.size();
}

int Species::getAverageFitness() const {
    return averageFitness;
}

Genome Species::crossover(Genome g1, Genome g2) {

    // Make sure g1 is the higher fitness genome
    if (g2.getFitness() > g1.getFitness()) {
        Genome tempg = g1;
        g1 = g2;
        g2 = tempg;
    }

    Genome child;

    vector<Gene> genes1 = g1.getGenes();
    vector<Gene> genes2 = g2.getGenes();


    map<int, Gene> innovations2;
    for (int i = 0; i < genes2.size(); i++) {
        innovations2[genes2[i].getInnovation()] = genes2[i];
    }

    for (int i = 0; i < genes1.size(); i++) {
        Gene gene1 = genes1[i];
        Gene gene2 = innovations2[gene1.getInnovation()];

        if (rand()%2 == 1 and gene2.isEnabled()) {
            child.addGene(gene2);
        } else child.addGene(gene1);
    }

    child.setMaxNeuron(max(g1.getMaxNeuron(), g2.getMaxNeuron()));

    child.copyMutationRates(g1.getMutationRates());


    return child;
}

Genome Species::breedChild(int crossoverChance) {
    Genome child;

    if (rand()%101 < crossoverChance) {
        Genome g1 = genomes[rand()%genomes.size()];
        Genome g2 = genomes[rand()%genomes.size()];
        child = crossover(g1, g2);
    }
    else {
        Genome g = genomes[rand()%genomes.size()];

        child = g;
    }

    child.mutate();

    return child;
}

vector<Genome> &Species::getGenomes() {
    return genomes;
}

int Species::getTopFitness() const {
    return topFitness;
}

void Species::setTopFitness(int topFitness) {
    Species::topFitness = topFitness;
}

void Species::setStaleness(int staleness) {
    Species::staleness = staleness;
}

int Species::getStaleness() const {
    return staleness;
}

void Species::addChild(Genome child){
    genomes.push_back(child);
}
