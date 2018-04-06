//
// Created by Kike Piera Serra on 08/01/2018.
//

#include <fstream>
#include "Pool.h"


int Pool::ScreenHeight = 16;
int Pool::ScreenWidth = 20;

int Pool::INPUT_SIZE = ScreenHeight*ScreenWidth;

//number of buttons
int Pool::OUTPUT_SIZE = 3;

int Pool::POPULATION = 20;

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
    currentGenome = 0;
    nBest = 5;
    nSurvivors = Pool::POPULATION/2;

    for (int i = 0; i < Pool::POPULATION; i++) {
        Genome* basic = new Genome();
        basic->firstGenome();
        genomes.push_back(basic);
    }
}


// go through array of fitness and push all values into vector
vector<int> Pool::loadFitness(string name, json pool){
    vector<int> fitnessVector;

    string f = name;
    int exists = pool.count(f);
    json fitness = pool.at(f);

    //check if the array exists
    if (exists > 0) {
        int y = 0;
        f = to_string(y);
        exists = fitness.count(f);
        for (y = 1; exists > 0; y++) {
            fitnessVector.push_back(fitness.at(f).get<int>());


            // prepare for next iteration
            f = to_string(y);
            exists = fitness.count(f);
        }
    }
    else{
        throw (name + " NOT FOUND!");
    }

    return fitnessVector;
}

void Pool::loadPool(string loadLocation) {
    ifstream i(loadLocation);
    json pool;
    i >> pool;

    cout << pool << endl;

    pool = pool.at("pool").get<json>();
    cout << pool << endl;

    generation = pool.at("generation").get<int>();
    currentGenome = pool.at("currentGenome").get<int>();
    ScreenHeight = pool.at("ScreenHeight").get<int>();
    ScreenWidth = pool.at("ScreenWidth").get<int>();
    INPUT_SIZE = pool.at("INPUT_SIZE").get<int>();
    OUTPUT_SIZE = pool.at("OUTPUT_SIZE").get<int>();
    POPULATION = pool.at("POPULATION").get<int>();
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


    maxFitness = loadFitness("maxFitness", pool);
    averageFitness = loadFitness("averageFitness", pool);;
    totalFitness = loadFitness("totalFitness", pool);;

    vector<Genome*> children;

    int y = 0;
    string G = "genome" + to_string(y);
    int countG = pool.count(G);
    for (y = 1; countG > 0; y++){
        json genome = pool.at(G);

        vector<Gene*> genes;

        int z = 0;
        string g = "gene" + to_string(z);
        int countg = genome.count(g);
        for (z = 1; countg > 0; z++){
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
        countG = pool.count(G);
    }


    this->genomes = children;
}

void Pool::savePool(string location){
    string poolStr = "{ \"pool\": {";

    int j = 0;
    for (Genome *g : genomes){
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

        genomeStr += "\"lastNeuronCreated\": " + to_string(g->getLastNeuronCreated()) + ", ";
        genomeStr += "\"fitness\": " + to_string(g->getFitness());

        genomeStr += "}, ";

        poolStr += genomeStr;
    }

    string maxFitnessStr = "\"maxFitness\": {";
    for (int i = 0; i < maxFitness.size(); i++){
        maxFitnessStr += "\"" + to_string(i) +"\": " +  to_string(maxFitness[i]);
    }

    string averageFitnessStr = "\"averageFitness\": {";
    for (int i = 0; i < averageFitness.size(); i++){
        averageFitnessStr += "\"" + to_string(i) +"\": " +  to_string(averageFitness[i]);
    }

    string totalFitnessStr = "\"totalFitness\": {";
    for (int i = 0; i < totalFitness.size(); i++){
        totalFitnessStr += "\"" + to_string(i) +"\": " +  to_string(totalFitness[i]);
    }



    poolStr += "\"generation\": " + to_string(generation) + ", ";
    poolStr += "\"currentGenome\": " + to_string(currentGenome) + ", ";
    poolStr += "\"ScreenHeight\": " + to_string(Pool::ScreenHeight) + ", ";
    poolStr += "\"ScreenWidth\": " + to_string(Pool::ScreenWidth) + ", ";
    poolStr += "\"INPUT_SIZE\": " + to_string(Pool::INPUT_SIZE) + ", ";
    poolStr += "\"OUTPUT_SIZE\": " + to_string(Pool::OUTPUT_SIZE) + ", ";
    poolStr += "\"POPULATION\": " + to_string(Pool::POPULATION) + ", ";
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

    for (int genome = 0; genome < genomes.size(); genome++) {
        int genomeFitness = genomes[genome]->getFitness();

        if (genomeFitness > max) max = genomeFitness;

        total += genomeFitness;
    }

    maxFitness.push_back(max);

    averageFitness.push_back(total/genomes.size());
    totalFitness.push_back(total);
}

bool isLhsFitnessBigger(Genome* a, Genome* b) { return a->getFitness() > b->getFitness(); }

double Round(double x, int p) {
    if (x != 0.0) {
        return ((floor((fabs(x)*pow(double(10.0),p))+0.5))/pow(double(10.0),p))*(x/fabs(x));
    } else {
        return 0;
    }
}

void Pool::nextGenome(string saveLocation){
    currentGenome++;

    if (currentGenome == genomes.size()) {

        calculateFitness();
        // save pool in file location
        savePool(saveLocation);

        currentGenome = 0;

        // select survivors, breeding and mutation
        newGeneration();
    }
}

void Pool::cullSpecies(bool keepBest) {

    sort(genomes.begin(), genomes.end(), isLhsFitnessBigger);


    int remaining = (genomes.size()/ 2);
    if (keepBest) remaining = nBest;

    while (genomes.size() > remaining) {
        genomes.pop_back();
    }
}

Genome* Pool::crossover(Genome* g1, Genome* g2) {

    // Make sure g1 is the higher fitness genome
    if (g2->getFitness() > g1->getFitness()) {
        Genome* tempg = g1;
        g1 = g2;
        g2 = tempg;
    }

    Genome* child = new Genome();

    vector<Gene*> genes1 = g1->getGenes();
    vector<Gene*> genes2 = g2->getGenes();

    for (int i = 0; i < genes1.size(); i++) {
        child->addGene(genes1[i]);
    }

    for (int i = 0; i < genes2.size(); i++) {
        child->addGene(genes2[i]);
    }

    child->setLastNeuronCreated(max(g1->getMaxNeuron(), g2->getMaxNeuron()));

    child->copyMutationRates(g1->getMutationRates());

    return child;
}

Genome* Pool::breedChild() {
    Genome* child = new Genome();

    if (rand()%101 < Pool::CrossoverChance) {
        Genome* g1 = genomes[rand()%genomes.size()];
        Genome* g2 = genomes[rand()%genomes.size()];
        child = crossover(g1, g2);
    }
    else {
        Genome genome = *genomes[rand()%genomes.size()];
        child = new Genome(genome.getGenes(), genome.getFitness(), genome.getAdjustedFitness(), genome.getLastNeuronCreated(), genome.getGlobalRank(), genome.getMutationRates());
    }

    return child;
}

vector<Genome*> Pool::copyGenomes(){
    vector<Genome*> copy;
    for (Genome* g: genomes){
        copy.push_back(new Genome(g->getGenes(), g->getFitness(), g->getAdjustedFitness(), g->getLastNeuronCreated(), g->getGlobalRank(), g->getMutationRates()));
    }

    return copy;
}

vector<Genome*> Pool::rouletteSelection(){
    // total fitness of the last generation
    float totalFitness = this->totalFitness[generation];

    // sum of all remaining genomes' survivingChance
    float totalChance = 100;

    vector<Genome*> survivingGenomes;

    //array of genomes not chosen for survival
    vector<Genome*> genomesArray = copyGenomes();

    // if total fitness is 0, all genomes have 0 chance of survival.
    if (totalFitness == 0) return survivingGenomes;


    // the n best genomes always survive, n = keepBest
    // if the fitness is = 0, delete genome
    for (int i = 0; i < genomesArray.size(); i++) {
        if (genomesArray[i]->getFitness() == 0){
            genomesArray.erase(genomesArray.begin()+i);
            i--;
        }
        else if (survivingGenomes.size() < nBest) {
            totalChance -= 100.0f * (float) genomesArray[i]->getFitness() / totalFitness;

            survivingGenomes.push_back(genomesArray[i]);
            genomesArray.erase(genomesArray.begin()+i);
            i--;
        }
    }

    // while there are not enough genomes selected... and there are genomes in the array
    while (survivingGenomes.size() < nSurvivors && genomesArray.size() != 0) {

        // select a float randomly between 0.0 and 1.0
        float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        // select a random number depending on the totalchance left
        x *= totalChance;
        float percentageCounter = 0;    // sum of percentages of genomes already gone through


        for (int i = 0; i < genomesArray.size(); i++) {
            float survivingChance = 100.0f*(float)genomesArray[i]->getFitness() / totalFitness;

            // if randomNumber is between the percentageCounter and the percentageCounter+current survivingChance:
            // genome survives and survivingChance is subtracted to totalChance
            // genome is removed from the array, so that it cannot be chosen again
            if (x >= percentageCounter && x <= (percentageCounter + survivingChance)) {
                survivingGenomes.push_back(genomesArray[i]);
                totalChance -= survivingChance;
                genomesArray.erase(genomesArray.begin()+i);
                i--;
                break;
            }
                // else: survivingChance is added to percentageCounter
            else percentageCounter += survivingChance;
        }
    }

    return survivingGenomes;
}


vector<Genome*> Pool::eliteSelection(){
    vector<Genome*> children;

    // add
    for (int g = 0; g < genomes.size(); g++) {
        double average = 0;
        int l = averageFitness[generation];

        // avoid having NaN
        if (averageFitness[generation] > 0)
            // calculate number of children to breed, and decrease it depending on nSurvivors
            average = (nSurvivors/POPULATION) * ((double)genomes[g]->getFitness()/(double)averageFitness[generation]);

        int breed = round(average);


        for (int j = 0; j < breed; j++) {
            children.push_back(breedChild());
        }
    }

    return children;
}

// remove the bottom half of the genomes of each species
// remove
void Pool::newGeneration() {
    sort(genomes.begin(), genomes.end(), isLhsFitnessBigger);
    vector<Genome *> survivingGenomes = rouletteSelection();

    // if the roulette selection did not take any survivors, that means that totalFitness = 0
    if (survivingGenomes.size() == 0) {
        cullSpecies(false); // Cull the bottom half of each species
    } else {
        genomes = survivingGenomes;
    }


    //breed by elite selection
    vector<Genome *> children = eliteSelection();


    // add random children until the population limit is reached
    while (children.size() + genomes.size() < Pool::POPULATION) {
        children.push_back(breedChild());
    }

    for (int i = 0; i < children.size(); i++) {
        genomes.push_back(children[i]);
    }

    // mutate all but the best
    for (int i = nBest; i < genomes.size(); i++) {
        genomes[i]->mutate();
    }

    generation++;
}


//GETTERS
int Pool::getGeneration() {
    return generation;
}

int Pool::getCurrentGenome() {
    return currentGenome;
}

vector<Genome*> &Pool::getGenomes() {
    return genomes;
}



