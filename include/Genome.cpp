//
// Created by Kike Piera Serra on 08/01/2018.
//

#include "Genome.h"
#include "Pool.h"

Genome::Genome(){
    genes;
    network;
    fitness = 0;
    adjustedFitness = 0;
    maxNeuron = 0;
    globalRank = 0;
    mutationRates.connections = Pool::ConnMutateChance;
    mutationRates.link = Pool::LinkMutateChance;
    mutationRates.node = Pool::NodeMutateChance;
    mutationRates.bias = Pool::BiasMutateChance;
    mutationRates.enable = Pool::EnableMutateChance;
    mutationRates.disable = Pool::DisableMutateChance;
    mutationRates.step = Pool::StepSize;
}

//GETTERS
MutationRates Genome::getMutationRates(){
    return mutationRates;
}

int Genome::getMaxNeuron() const {
    return maxNeuron;
}

int Genome::getGlobalRank(){
    return globalRank;
}

int Genome::getFitness() const {
    return fitness;
}

const vector<Gene> Genome::getGenes() const {
    return genes;
}

//SETTERS
void Genome::setMaxNeuron(int maxNeuron) {
    Genome::maxNeuron = maxNeuron;
}

void Genome::copyMutationRates(MutationRates mR){
    mutationRates = mR;
}

void Genome::addGene(Gene g){
    genes.push_back(g);
}

void Genome::mutate() {
    mutationRates.connections *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.link *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.bias *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.node *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.enable *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.disable *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.step *= ((rand()%2 == 1) ? 0.95 : 1.05263);

    //TODO: CHECK mutationRates values:
    cout << "connections" << mutationRates.connections << endl;
    cout << "link" << mutationRates.link << endl;
    cout << "bias" << mutationRates.bias << endl;
    cout << "node" << mutationRates.node << endl;
    cout << "enable" << mutationRates.enable << endl;
    cout << "disable" << mutationRates.disable << endl;
    cout << "step" << mutationRates.step << endl;
    cout << "random" << (rand()%11)/10 << endl;

    if ((rand()%11)/10 < mutationRates.connections) {
        pointMutate();
    }

    for (double p = mutationRates.link; p > 0; p--){
        if ((rand()%11)/10 < p) linkMutate(false);
    }

    for (double p = mutationRates.bias; p > 0; p--){
        if ((rand()%11)/10 < p) linkMutate(true);
    }

    for (double p = mutationRates.node; p > 0; p--){
        if ((rand()%11)/10 < p) nodeMutate();
    }

    for (double p = mutationRates.enable; p > 0; p--){
        if ((rand()%11)/10 < p) enableDisableMutate(true);
    }

    for (double p = mutationRates.disable; p > 0; p--){
        if ((rand()%11)/10 < p) enableDisableMutate(false);
    }
}

void Genome::pointMutate() {
    double step = mutationRates.step;

    for (int i = 0; i < genes.size(); i++) {
        Gene gene = genes[i];
        if (rand() % 2 < Pool::PerturbChance) {
            gene.setWeight(gene.getWeight() + (rand()%11)/10 * step * 2 - step);
        } else gene.setWeight((rand()%11)/10 * 4 - 2);

    }
}

// crates new random genes unless the created gene already exists in the array
void Genome::linkMutate(bool forceBias) {
    int neuron1 = randomNeuron(true);
    int neuron2 = randomNeuron(false);

    int into = 0;
    int out = 0;
    double weight;
    bool enabled = true;

    if (neuron1 <= Pool::INPUT_SIZE && neuron2 <= Pool::INPUT_SIZE) {
        //Both input nodes
        return;
    }
    if (neuron2 <= Pool::INPUT_SIZE) {
        // Swap output and input
        int temp = neuron1;
        neuron1 = neuron2;
        neuron2 = temp;
    }

    into = neuron1;
    out = neuron2;
    if (forceBias) {
        into = Pool::INPUT_SIZE;
    }

    weight = (rand()%11)/10 * 4 - 2;

    Gene newLink = Gene(into, out, weight, enabled);

    // if that gene already exists, don't do anything
    if (containsLink(newLink)) {
        return;
    }


    // if the gene doesn't exist, add it to the array
    genes.push_back(newLink);
}


bool Genome::containsLink(Gene link) {
    for (int i = 0; i < genes.size(); i++){
        Gene gene = genes[i];
        if (gene.getInto() == link.getInto() && gene.getOut() == link.getOut()) {
            return true;
        }
    }
    return false;
}

void Genome::nodeMutate() {
    if (genes.size() == 0)  return;


    maxNeuron++;

    Gene gene = genes[rand() % genes.size()];
    if (!gene.isEnabled())  return;
    gene.setEnabled(false);


    //Create  gene1 that gets gene as input
    int into = gene.getInto();
    int out = maxNeuron;
    double weight = 1.0;
    bool enabled = true;
    genes.push_back(Gene(into, out, weight, enabled));


    // Create gene2 that outputs to the gene
    into = maxNeuron;
    out = gene.getOut();
    weight = gene.getWeight();
    enabled = true;
    genes.push_back(Gene(into, out, weight, enabled));
}





void Genome::enableDisableMutate(bool enable) {
    vector<Gene> candidates;
    for (int i = 0; i < genes.size(); i++) {
        if (genes[i].isEnabled() == !enable) candidates.push_back(genes[i]);
    }

    if (candidates.size() == 0) return;

    Gene gene = candidates[rand() % candidates.size()];
    gene.setEnabled(!gene.isEnabled());
}


// count all the neurons there is and give a random index of all neurons
// if there are no genes it will still return a random number between the inputs and the outputs
int Genome::randomNeuron(bool isInput) {
    int neurons = 0;
    if (isInput) {
        neurons = Pool::INPUT_SIZE;
    }

    neurons += Pool::OUTPUT_SIZE;


    for (int i = 0; i < genes.size(); i++){
        if (isInput) neurons += 2;
        else {
            if(genes[i].getInto() > Pool::INPUT_SIZE) neurons++;
            if(genes[i].getOut() > Pool::INPUT_SIZE) neurons++;
        }
    }


    return int(rand() % neurons);
}


void Genome::firstGenome() {
    maxNeuron = Pool::INPUT_SIZE;
    mutate();
}

void Genome::generateNetwork() {

    //sort genes having the last ones to the right
    sort(genes.begin(), genes.end(), isRhsOutBigger);

    // create neurons for all Inputs (tiles map) +  a neuron for the bias
    for (int i = 0; i < Pool::INPUT_SIZE + 1; i++) {
        network[i] = Neuron2();
    }

    // create neurons for all outputs (button controls)
    for (int i = 0; i < Pool::OUTPUT_SIZE; i++) {
        network[Pool::MaxNodes+i] = Neuron2();
    }

    for (int i = 0; i < genes.size(); i++) {

        if (genes[i].isEnabled()) {
            // neuron is not in the map create neuron
            if (network[genes[i].getOut()] == -1){
                network[genes[i].getOut()] = Neuron2();
            }

            // add gene to the output neuron
            network[genes[i].getOut()].addGene(genes[i]);

            // if input neuron does not exist, create it
            if (network[genes[i].getInto()] == -1) {
                network[genes[i].getInto()] = Neuron2();
            }
        }
    }
}




bool isRhsOutBigger(const Gene a, const Gene b) { return a.getOut() < b.getOut() ; }


