//
// Created by Kike Piera Serra on 08/01/2018.
//

#include <fstream>
#include "Pool.h"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;


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
    maxFitness = 0;

    for (int i = 0; i < Pool::POPULATION; i++) {
        Genome* basic = new Genome();
        basic->firstGenome();
        addToSpecies(basic);
    }
}

void Pool::loadPool(string loadLocation) {
    ifstream i(loadLocation);
    json pool;
    i >> pool;

    cout << pool << endl;

    pool = pool.at("pool").get<json>();
    cout << pool << endl;

    generation = pool.at("generation").get<int>();
    currentSpecies = pool.at("currentSpecies").get<int>();
    currentGenome = pool.at("currentGenome").get<int>();
    ScreenHeight = pool.at("ScreenHeight").get<int>();
    ScreenWidth = pool.at("ScreenWidth").get<int>();
    INPUT_SIZE = pool.at("INPUT_SIZE").get<int>();
    OUTPUT_SIZE = pool.at("OUTPUT_SIZE").get<int>();
    POPULATION = pool.at("POPULATION").get<int>();
    DELTA_DISJOINT = pool.at("DELTA_DISJOINT").get<double>();
    DELTA_WEIGHTS = pool.at("DELTA_WEIGHTS").get<double>();
    DELTA_THRESHOLD = pool.at("DELTA_THRESHOLD").get<double>();
    STALE_SPECIES = pool.at("STALE_SPECIES").get<int>();
    PerturbChance = pool.at("PerturbChance").get<double>();
    CrossoverChance = pool.at("CrossoverChance").get<double>();
    ConnMutateChance = pool.at("ConnMutateChance").get<double>();
    LinkMutateChance = pool.at("LinkMutateChance").get<double>();
    NodeMutateChance = pool.at("NodeMutateChance").get<double>();
    BiasMutateChance = pool.at("BiasMutateChance").get<double>();
    EnableMutateChance = pool.at("EnableMutateChance").get<double>();
    DisableMutateChance = pool.at("DisableMutateChance").get<double>();
    StepSize = pool.at("StepSize").get<double>();
    MaxNodes = pool.at("MaxNodes").get<int>();

    vector<Species> newSpecies;

    int x = 0;
    string s = "species" + to_string(x);
    int countS = pool.count(s);
    for (int x = 1; countS > 0; x++){
        json species = pool.at(s);
        cout << species << endl;

        vector<Genome*> children;

        int y = 0;
        string G = "genome" + to_string(y);
        int countG = species.count(G);
        for (int y = 1; countG > 0; y++){
            json genome = species.at(G);

            vector<Gene*> genes;

            int z = 0;
            string g = "gene" + to_string(z);
            int countg = genome.count(g);
            for (int z = 1; countg > 0; z++){
                json gene = genome.at(g);

                int into = gene.at("into").get<int>();
                int out = gene.at("out").get<int>();
                double weight = gene.at("weight").get<double>();
                bool enabled = gene.at("enabled").get<int>();
                int innovation = gene.at("innovation").get<int>();

                Gene * newGene = new Gene(into, out, weight, enabled, innovation);

                genes.push_back(newGene);

                // prepare for next iteration
                g = "gene" + to_string(z);
                countg = genome.count(g);
            }


            int lastNeuronCreated = genome.at("lastNeuronCreated").get<int>();


            json mR = genome.at("mutationRates");

            MutationRates mutationRates;
            mutationRates.connections = mR.at("connections").get<double>();
            mutationRates.link = mR.at("link").get<double>();
            mutationRates.bias = mR.at("bias").get<double>();
            mutationRates.node = mR.at("node").get<double>();
            mutationRates.enable = mR.at("enable").get<double>();
            mutationRates.disable = mR.at("disable").get<double>();
            mutationRates.step = mR.at("step").get<double>();

            Genome* newGenome = new Genome(genes, 0, 0, lastNeuronCreated, 0, mutationRates);
            children.push_back(newGenome);

            // prepare for next iteration
            G = "genome" + to_string(y);
            countG = species.count(G);
        }


        Species childSpecies = Species(children);
        childSpecies.setStaleness(species.at("staleness").get<int>());

        newSpecies.push_back(childSpecies);

        // prepare for next iteration
        s = "species" + to_string(x);
        countS = pool.count(s);
    }

    this->species = newSpecies;
}

void Pool::savePool(string location){
    string poolStr = "{ \"pool\": {";

    int i = 0;
    for (Species s : species){
        string speciesStr = "\"species"+ to_string(i) +"\": {";
        i++;

        int j = 0;
        for (Genome *g : s.getGenomes()){
            string genomeStr = "\"genome"+ to_string(j) +"\": {";
            j++;

            int k = 0;
            for (Gene *gene: g->getGenes()){
                string geneStr = "\"gene"+ to_string(k) +"\": {";
                k++;

                geneStr += "\"into\": " + to_string(gene->getInto()) + ", ";
                geneStr += "\"out\": " + to_string(gene->getOut()) + ", ";
                geneStr += "\"weight\": " + to_string(gene->getWeight()) + ", ";
                geneStr += "\"enabled\": " + to_string(gene->isEnabled()) + ", ";
                geneStr += "\"innovation\": " + to_string(gene->getInnovation());

                geneStr += "}, ";

                genomeStr += geneStr;
            }



            MutationRates mR= g->getMutationRates();
            string mutationRatesStr = "\"mutationRates\": {";
            mutationRatesStr += "\"connections\": " + to_string(mR.connections) + ", ";
            mutationRatesStr += "\"link\": " + to_string(mR.link) + ", ";
            mutationRatesStr += "\"bias\": " + to_string(mR.bias) + ", ";
            mutationRatesStr += "\"node\": " + to_string(mR.node) + ", ";
            mutationRatesStr += "\"enable\": " + to_string(mR.enable) + ", ";
            mutationRatesStr += "\"disable\": " + to_string(mR.disable) + ", ";
            mutationRatesStr += "\"step\": " + to_string(mR.step);

            mutationRatesStr += "}, ";

            genomeStr += mutationRatesStr;

            genomeStr += "\"lastNeuronCreated\": " + to_string(g->getLastNeuronCreated());

            genomeStr += "}, ";

            speciesStr += genomeStr;
        }

        speciesStr += "\"staleness\": " + to_string(s.getStaleness());

        speciesStr += "}, ";


        poolStr += speciesStr;
    }

    poolStr += "\"generation\": " + to_string(generation) + ", ";
    poolStr += "\"currentSpecies\": " + to_string(currentSpecies) + ", ";
    poolStr += "\"currentGenome\": " + to_string(currentGenome) + ", ";
    poolStr += "\"ScreenHeight\": " + to_string(Pool::ScreenHeight) + ", ";
    poolStr += "\"ScreenWidth\": " + to_string(Pool::ScreenWidth) + ", ";
    poolStr += "\"INPUT_SIZE\": " + to_string(Pool::INPUT_SIZE) + ", ";
    poolStr += "\"OUTPUT_SIZE\": " + to_string(Pool::OUTPUT_SIZE) + ", ";
    poolStr += "\"POPULATION\": " + to_string(Pool::POPULATION) + ", ";
    poolStr += "\"DELTA_DISJOINT\": " + to_string(Pool::DELTA_DISJOINT) + ", ";
    poolStr += "\"DELTA_WEIGHTS\": " + to_string(Pool::DELTA_WEIGHTS) + ", ";
    poolStr += "\"DELTA_THRESHOLD\": " + to_string(Pool::DELTA_THRESHOLD) + ", ";
    poolStr += "\"STALE_SPECIES\": " + to_string(Pool::STALE_SPECIES) + ", ";
    poolStr += "\"PerturbChance\": " + to_string(Pool::PerturbChance) + ", ";
    poolStr += "\"CrossoverChance\": " + to_string(Pool::CrossoverChance) + ", ";
    poolStr += "\"ConnMutateChance\": " + to_string(Pool::ConnMutateChance) + ", ";
    poolStr += "\"LinkMutateChance\": " + to_string(Pool::LinkMutateChance) + ", ";
    poolStr += "\"NodeMutateChance\": " + to_string(Pool::NodeMutateChance) + ", ";
    poolStr += "\"BiasMutateChance\": " + to_string(Pool::BiasMutateChance) + ", ";
    poolStr += "\"EnableMutateChance\": " + to_string(Pool::EnableMutateChance) + ", ";
    poolStr += "\"DisableMutateChance\": " + to_string(Pool::DisableMutateChance) + ", ";
    poolStr += "\"StepSize\": " + to_string(Pool::StepSize) + ", ";
    poolStr += "\"TIMEOUT_CONSTANT\": " + to_string(Pool::TIMEOUT_CONSTANT) + ", ";
    poolStr += "\"MaxNodes\": " + to_string(Pool::MaxNodes) + ", ";
    poolStr += "\"EnableMutateChance\": " + to_string(Pool::EnableMutateChance);


    poolStr += "}}";

    json poolJson = json::parse(poolStr);

    // put object to file with indentation (setw(4)) not in just 1 line
    ofstream file(location);
    file << setw(4) <<  poolJson << endl;
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

        if (breed < 1) {
            species.erase(species.begin()+s);
            s--;
        }
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

void Pool::nextGenome(string saveLocation){

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
            // save pool in file location
            savePool(saveLocation);
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



