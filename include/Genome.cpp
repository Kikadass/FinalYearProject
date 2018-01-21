//
// Created by Kike Piera Serra on 08/01/2018.
//

#include "Genome.h"
#include "Pool.h"
bool Debug = false;

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

    if (Debug) {
        cout << endl << endl << "MUTATE!" << endl;
        cout << "connections: " << mutationRates.connections << endl;
        cout << "link: " << mutationRates.link << endl;
        cout << "bias: " << mutationRates.bias << endl;
        cout << "node: " << mutationRates.node << endl;
        cout << "enable: " << mutationRates.enable << endl;
        cout << "disable: " << mutationRates.disable << endl;
        cout << "step: " << mutationRates.step << endl;
        cout << "random: " << (rand() % 11) / 10 << endl;
    }

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

    cout << "Both neurons are input neurons: " << neuron1 << " : " << neuron2 << endl;
    if (neuron1 <= Pool::INPUT_SIZE && neuron2 <= Pool::INPUT_SIZE) {
        //Both input nodes
        cout << "No new gene created" << endl;

        return;
    }

    cout << "New gene created!" << endl;

    if (neuron2 <= Pool::INPUT_SIZE) {
        // Swap output and input to have the lowest number in neuron1
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

    genes[rand() % genes.size()].setEnabled(false);


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
    vector<int> candidates;
    for (int i = 0; i < genes.size(); i++) {
        if (genes[i].isEnabled() == !enable) candidates.push_back(i);
    }

    if (candidates.size() == 0) return;


    int random = candidates[rand() % candidates.size()];
    genes[random].setEnabled(!genes[random].isEnabled());
}


// count all the neurons there is and give a random index of all neurons
// if there are no genes it will still return a random number between the inputs and the outputs
int Genome::randomNeuron(bool isInput) {
    map<int, bool> neurons;

    // put into neurons the inputSize + the bias
    for (int i = 0; i <= Pool::INPUT_SIZE; i++){
        neurons[i] = true;
    }

    if (!isInput) {
        for (int i = 0; i <= Pool::OUTPUT_SIZE; i++){
            neurons[maxNeuron + i] = true;
        }
    }

    // add genes' neurons and avoid the output ones if we are looking only for input neurons
    for (int i = 0; i < genes.size(); i++){
        if (isInput || genes[i].getInto() < Pool::OUTPUT_SIZE) {
            neurons[genes[i].getInto()] = true;
        }
        if (isInput || genes[i].getOut() < Pool::OUTPUT_SIZE) {
            neurons[genes[i].getOut()] = true;
        }
    }

    int counter = 0;
    for (int i = 0; i < neurons.size(); i++){
        if (neurons[i]){
            counter++;
        }
    }

    return rand() % counter;
}


void Genome::firstGenome() {
    maxNeuron = Pool::INPUT_SIZE;
    mutate();
}


bool isRhsOutBigger(const Gene a, const Gene b) { return a.getOut() < b.getOut() ; }

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
            auto x = network.find(genes[i].getOut())->second;
            cout << "Getting neurons!" << endl;
            cout << (x.getValue() == -1) << endl;
            if (x.getValue() == -1){
                network[genes[i].getOut()] = Neuron2();
            }

            // add gene to the output neuron
            network[genes[i].getOut()].addGene(genes[i]);

            // if input neuron does not exist, create it
            if (network[genes[i].getInto()].getValue() == -1) {
                network[genes[i].getInto()] = Neuron2();
            }
        }
    }
}




int Genome::evaluateNetwork(vector<double> inputs) {


    if (inputs.size() != Pool::INPUT_SIZE) {
        cout << "INCORRECT NUMBER OF INPUTS" << endl;
        cout << "Inputs expected: " << Pool::INPUT_SIZE << endl;
        cout << "Inputs received: " << inputs.size() << endl;
        return -1;
    }

    // inputting the tile values into the "input layer"
    for (int i = 0; i < Pool::INPUT_SIZE; i++) {
        network[i].setValue(inputs[i]);
    }


    // go through the genes in each neuron and get the sum of the into.Neuron.values * the gene weight
    // use sigmoid function with the sum
    for (int i = 0; i < network.size(); i++) {
        int sum = 0;
        for (int j = 0; j < network[i].getGenes().size(); i++) {
            Gene gene = network[i].getGenes()[j];
            Neuron2 neuron = network[gene.getInto()];
            sum += gene.getWeight() * neuron.getValue();
        }

        if (network[i].getGenes().size() > 0) {
            //sigmoid function. set the value between 0 and 1
            network[i].setValue(sum / (1 + abs(sum)));
        }
    }


    // TODO:: just choose the one with bigger value
    int pressed = -1;
    int maxValue = 0;
    cout << network.size();
    for (int i = 0; i < Pool::OUTPUT_SIZE; i++) {
        cout << network[Pool::MaxNodes + i].getValue() << " : " << maxValue << " = " << (network[Pool::MaxNodes + i].getValue() > maxValue) << endl;
        if (network[Pool::MaxNodes + i].getValue() > maxValue) {
            maxValue = network[Pool::MaxNodes + i].getValue();
            pressed = i+1;
        }
    }


    //check which buttons are true and which ara false

    /*
    int anyPressed = false;
    for (int i = 0; i < Pool::OUTPUT_SIZE; i++) {
        if (network[Pool::MaxNodes + i].getValue() > 0.5) {
            //Down up left right

            // Avoid having more then 1 button true
            if (anyPressed){
                anyPressed = false;
                pressed = -1;
                break;
            }
            else{
                anyPressed = true;
                pressed = i+1;
            }
        }
    }

    */

    return pressed;
}
