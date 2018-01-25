//
// Created by Kike Piera Serra on 08/01/2018.
//

#include "Pool.h"


int Pool::ScreenHeight = 16;
int Pool::ScreenWidth = 20;

int Pool::INPUT_SIZE = ScreenHeight*ScreenWidth;

//number of buttons
int Pool::OUTPUT_SIZE = 4;

int Pool::POPULATION = 10;
double Pool::DELTA_DISJOINT = 2.0;
double Pool::DELTA_WEIGHTS = 0.4;
double Pool::DELTA_THRESHOLD = 1.0;

int Pool::STALE_SPECIES = 15;

double Pool::PerturbChance = 0.90;
double Pool::CrossoverChance = 0.75;

double Pool::ConnMutateChance = 0.25;
double Pool::LinkMutateChance = 2.0;
double Pool::NodeMutateChance = 0.50;
double Pool::BiasMutateChance = 0.40;
double Pool::EnableMutateChance = 0.2;
double Pool::DisableMutateChance = 0.4;
double Pool::StepSize = 0.1;

int Pool::TIMEOUT_CONSTANT = 1000;

int Pool::MaxNodes = 1000000;

Pool::Pool() {
    vector<Species> species;
    generation = 0;
    currentSpecies = 0;
    currentGenome = 0;
    currentFrame = 0;
    maxFitness = 0;

    for (int i = 0; i < Pool::POPULATION; i++) {
        Genome basic;
        basic.firstGenome();
        addToSpecies(basic);
    }
}

int Pool::calculateFitness() {
    int total = 0;
    int max = 0;

    for (int specie = 0; specie < species.size(); specie++) {
        int specieFitness = species[specie].getAverageFitness();

        if (specieFitness > max) max = specieFitness;

        total += specieFitness;
    }

    maxFitness = max;

    return total;
}

bool isLhsFitnessBigger(const Genome a, const Genome b) { return a.getFitness() > b.getFitness(); }

void Pool::removeStaleSpecies() {
    vector<Species> survived;

    for (int s = 0; s < species.size(); s++) {

        // sort genomes in the species by fitness
        vector<Genome> &genomes = species[s].getGenomes();
        sort(genomes.begin(), genomes.end(), isLhsFitnessBigger);


        if (genomes[1].getFitness() > species[s].getTopFitness()){

            species[s].setTopFitness(genomes[1].getFitness());
            species[s].setStaleness(0);
        }
        else species[s].setStaleness(species[s].getStaleness() + 1);


        if (species[s].getStaleness() < Pool::STALE_SPECIES || species[s].getTopFitness() >= maxFitness) {
            survived.push_back(species[s]);
        }
    }

    species = survived;
}

void Pool::removeWeakSpecies() {
    vector<Species> survived;

    int sum = calculateFitness();

    for (int s = 0; s < species.size(); s++) {
        double breed = floor(species[s].getAverageFitness() / sum * Pool::POPULATION);

        if (breed >= 1) survived.push_back(species[s]);

    }

    species = survived;
}

bool Pool::sameSpecies(Genome genome1, Genome genome2) {
    double dd = DELTA_DISJOINT * disjoint(genome1.getGenes(), genome2.getGenes());
    double dw = DELTA_WEIGHTS * weights(genome1.getGenes(), genome2.getGenes());
    return (dd + dw) < DELTA_THRESHOLD;
}


double Pool::disjoint(vector<Gene> genes1, vector<Gene> genes2) {
    if (genes1.size() == 0 && genes2.size() == 0){
        return 0;
    }

    map<int, bool> i1;
    for (int i = 0; i < genes1.size(); i++) {
        i1[genes1[i].getInnovation()] = true;
    }

    map<int, bool> i2;
    for (int i = 0; i < genes2.size(); i++) {
        i2[genes2[i].getInnovation()] = true;
    }

    int disjointGenes = 0;
    for (int i = 0; i < genes1.size(); i++) {
        if (!i2[genes1[i].getInnovation()]) {
            disjointGenes++;
        }
    }

    for (int i = 0; i < genes2.size(); i++) {
        if (!i1[genes2[i].getInnovation()]) {
            disjointGenes++;
        }
    }

    int n = max(genes1.size(), genes2.size());

    return disjointGenes / n;
}

double Pool::weights(vector<Gene> genes1, vector<Gene> genes2) {
    if (genes1.size() == 0 && genes2.size() == 0){
        return 0;
    }

    map<int, Gene> i2;
    for (int i = 0; i < genes2.size(); i++) {
        i2[genes2[i].getInnovation()] = genes2[i];
    }

    int sum = 0;
    int coincident = 0;
    for (int i = 0; i < genes1.size(); i++) {
        if (i2.count(genes1[i].getInnovation()) == 0){
            sum = sum + abs(genes1[i].getWeight() - i2[genes1[i].getInnovation()].getWeight());
            coincident++;
        }
    }

    if (coincident == 0) coincident = 1;

    return sum / coincident;
}


void Pool::addToSpecies(Genome child) {
    cout << "Adding child to Species" << endl;
    bool foundSpecies = false;

    for (int s = 0; s < species.size(); s++) {
        if (!foundSpecies && sameSpecies(child, species[s].getGenomes()[0])) {
            species[s].addChild(child);

            foundSpecies = true;
        }
    }

    if (!foundSpecies) {
        vector<Genome> children;
        children.push_back(child);
        Species childSpecies = Species(children);
        species.push_back(childSpecies);
    }
}

void Pool::nextGenome(){

    currentGenome++;

    if (currentGenome == species[currentSpecies].getGenomes().size()) {

        species[currentSpecies].calculateFitness();
        if (species[currentSpecies].getTopFitness() > maxFitness){
            maxFitness = species[currentSpecies].getTopFitness();
        }

        currentSpecies++;
        currentGenome = 0;

        if (currentSpecies == species.size()) {
            currentSpecies = 0;
            // TODO:: mutate, crossover etc+
            newGeneration();
        }
    }
}

int Pool::poolAverageFitness(){
    int sum = 0;

    for (int i = 0; i < species.size(); i++){
        sum += species[i].getAverageFitness();
    }

    return sum/species.size();
}


void Pool::cullSpecies(bool cutToOne) {


    for (int i = 0; i < species.size(); i++) {
        sort(species[i].getGenomes().begin(), species[i].getGenomes().end(), isLhsFitnessBigger);


        int remaining = (species[i].getGenomes().size()/ 2);
        if (cutToOne) remaining = 1;

        while (species[i].getGenomes().size() > remaining) {
            species[i].getGenomes().pop_back();
        }
    }
}

// order genomes from ALL species by fitness
void Pool::rankGlobally() {
    vector<Genome> global;
    for (int i = 0; i < species.size(); i++) {
        for (int g = 0; g < species[i].getGenomes().size(); g++) {
            global.push_back(species[i].getGenomes()[g]);
        }
    }

    sort(global.begin(), global.end(), isLhsFitnessBigger);

    for (int g = 0; g < global.size(); g++) {
        global[g].setGlobalRank(g);
    }
}

void Pool::newGeneration() {
    cullSpecies(false); // Cull the bottom half of each species

    rankGlobally(); // order genomes from ALL species by fitness

    removeStaleSpecies();

    rankGlobally();


    removeWeakSpecies();
    int sum = poolAverageFitness();
    vector<Genome> children;

    for (int i = 0; i < species.size(); i++) {
        int breed = (species[i].getAverageFitness() / sum * Pool::POPULATION) - 1;

        for (int j = 0; j < breed; j++) {
            children.push_back(species[i].breedChild());
        }
    }

    cullSpecies(true); // Cull all but the top member of each species

    // fill up the vector until we have enough population
    while (children.size() + species.size() < Pool::POPULATION) {
        children.push_back(species[rand()%species.size()].breedChild());
    }


    for (int i = 0; i < children.size(); i++) {
        addToSpecies(children[i]);
    }

    generation++;
}

//GETTERS


int Pool::getGeneration() {
    return generation;
}

int Pool::getCurrentSpecies() {
    return currentSpecies;
}

int Pool::getCurrentGenome() {
    return currentGenome;
}

vector<Species> &Pool::getSpecies() {
    return species;
}



