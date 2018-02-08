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
    generation = 0;
    currentSpecies = 0;
    currentGenome = 0;
    currentFrame = 0;
    maxFitness = 0;

    for (int i = 0; i < Pool::POPULATION; i++) {
        Genome* basic = new Genome();
        basic->firstGenome();
        addToSpecies(basic);
    }
}

void Pool::calculateFitness() {
    int total = 0;
    int max = 0;

    for (int specie = 0; specie < species.size(); specie++) {
        int specieFitness = species[specie].getAverageFitness();

        if (specieFitness > max) max = specieFitness;

        total += specieFitness;
    }

    maxFitness = max;

    averageFitness = total/species.size();
}

bool isLhsFitnessBigger(Genome* a, Genome* b) { return a->getFitness() > b->getFitness(); }


// if fitness has not improved in 15 generations. get rid of species unless it is the best species in the pool
void Pool::removeStaleSpecies() {
    for (int s = 0; s < species.size(); s++) {
        vector<Genome*> genomes = species[s].getGenomes();

        // sort genomes in the species by fitness
        if (species[s].getGenomes().size() >= 2) {

            sort(genomes.begin(), genomes.end(), isLhsFitnessBigger);
        }


        if (genomes[0]->getFitness() > species[s].getTopFitness()){
            species[s].setTopFitness(genomes[0]->getFitness());
            species[s].setStaleness(0);
        }
        else species[s].setStaleness(species[s].getStaleness() + 1);


        if (species[s].getStaleness() > Pool::STALE_SPECIES && species[s].getTopFitness() < maxFitness) {
            species.erase(species.begin()+s);
        }
    }
}

double Round(double x, int p) {
    if (x != 0.0) {
        return ((floor((fabs(x)*pow(double(10.0),p))+0.5))/pow(double(10.0),p))*(x/fabs(x));
    } else {
        return 0;
    }
}

void Pool::removeWeakSpecies() {
    calculateFitness();

    for (int s = 0; s < species.size(); s++) {
        double average = (double)species[s].getAverageFitness()/(double)averageFitness;
        int breed = Round(average, 0);

        if (breed < 1) species.erase(species.begin()+s);
    }
}

bool Pool::sameSpecies(Genome* genome1, Genome* genome2) {
    double dd = DELTA_DISJOINT * disjoint(genome1->getGenes(), genome2->getGenes());
    double dw = DELTA_WEIGHTS * weights(genome1->getGenes(), genome2->getGenes());
    return (dd + dw) < DELTA_THRESHOLD;
}


double Pool::disjoint(vector<Gene*> genes1, vector<Gene*> genes2) {
    if (genes1.size() == 0 && genes2.size() == 0){
        return 0;
    }

    map<int, bool> i1;
    for (int i = 0; i < genes1.size(); i++) {
        i1[genes1[i]->getInnovation()] = true;
    }

    map<int, bool> i2;
    for (int i = 0; i < genes2.size(); i++) {
        i2[genes2[i]->getInnovation()] = true;
    }

    // count how many innovation numbers are different
    int disjointGenes = 0;
    for (int i = 0; i < genes1.size(); i++) {
        if (!i2[genes1[i]->getInnovation()]) {
            disjointGenes++;
        }
    }

    for (int i = 0; i < genes2.size(); i++) {
        if (!i1[genes2[i]->getInnovation()]) {
            disjointGenes++;
        }
    }

    int n = max(genes1.size(), genes2.size());

    return disjointGenes / n;
}

double Pool::weights(vector<Gene*> genes1, vector<Gene*> genes2) {
    if (genes1.size() == 0 && genes2.size() == 0){
        return 0;
    }

    map<int, Gene*> i2;
    for (int i = 0; i < genes2.size(); i++) {
        i2[genes2[i]->getInnovation()] = genes2[i];
    }

    int sum = 0;
    int coincident = 0;
    for (int i = 0; i < genes1.size(); i++) {
        if (i2.find(genes1[i]->getInnovation()) != i2.end()){
            sum = sum + abs(genes1[i]->getWeight() - i2[genes1[i]->getInnovation()]->getWeight());
            coincident++;
        }
    }

    if (coincident == 0) coincident = 1;

    return sum / coincident;
}


void Pool::addToSpecies(Genome* child) {
    cout << "Adding child to Species" << endl;
    bool foundSpecies = false;

    for (int s = 0; s < species.size(); s++) {
        if (!foundSpecies && sameSpecies(child, species[s].getGenomes()[0])) {
            species[s].addChild(child);

            foundSpecies = true;
        }
    }

    if (!foundSpecies) {
        vector<Genome*> children;
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

void Pool::cullSpecies(bool cutToOne) {

    for (int i = 0; i < species.size(); i++) {
        vector<Genome*> genomes = species[i].getGenomes();
        sort(genomes.begin(), genomes.end(), isLhsFitnessBigger);


        int remaining = (genomes.size()/ 2);
        if (cutToOne || remaining < 1) remaining = 1;

        while (genomes.size() > remaining) {
            genomes.pop_back();
        }


        species[i].setGenomes(genomes);
    }
}


// order genomes from ALL species by fitness
void Pool::rankGlobally() {
    vector<Genome*> global;

    for (int i = 0; i < species.size(); i++) {
        for (int g = 0; g < species[i].getGenomes().size(); g++) {
            global.push_back(species[i].getGenomes()[g]);
        }
    }

    sort(global.begin(), global.end(), isLhsFitnessBigger);

    for (int g = 0; g < global.size(); g++) {
        global[g]->setGlobalRank(g);
    }
}


// remove the bottom half of the genomes of each species
// remove
void Pool::newGeneration() {
    cullSpecies(false); // Cull the bottom half of each species

    rankGlobally(); // order genomes from ALL species by fitness

    removeStaleSpecies();

    rankGlobally();


    removeWeakSpecies();

    vector<Genome*> children;

    // add
    for (int s = 0; s < species.size(); s++) {
        double average = (double)species[s].getAverageFitness()/(double)averageFitness;
        int breed = Round(average, 0)-1;


        for (int j = 0; j < breed; j++) {
            children.push_back(species[s].breedChild());
        }
    }

    cullSpecies(true); // Cull all but the top member of each species


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

