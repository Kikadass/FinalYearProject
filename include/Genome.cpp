//
// Created by Kike Piera Serra on 08/01/2018.
//

#include <fstream>
#include "Genome.h"
#include "Pool.h"

bool Debug = false;

Genome::Genome(){
    genes;
    network;
    fitness = 0;
    adjustedFitness = 0;
    lastNeuronCreated = 0;
    globalRank = -1;
    mutationRates.connections = Pool::ConnMutateChance;
    mutationRates.link = Pool::LinkMutateChance;
    mutationRates.node = Pool::NodeMutateChance;
    mutationRates.bias = Pool::BiasMutateChance;
    mutationRates.enable = Pool::EnableMutateChance;
    mutationRates.disable = Pool::DisableMutateChance;
    mutationRates.step = Pool::StepSize;
}

Genome::Genome(vector<Gene*> genes, int fitness, int adjustedFitness, int lastNeuronCreated, int globalRank, MutationRates mutationRates){
    // copy genes but creating new pointers
    for (Gene* g : genes){
        Genome::genes.push_back(new Gene(g->getInto(), g->getOut(), g->getWeight(), g->isEnabled(), g->getInnovation()));
    }

    Genome::fitness = fitness;
    Genome::adjustedFitness = adjustedFitness;
    Genome::lastNeuronCreated = lastNeuronCreated;
    Genome::globalRank = globalRank;
    Genome::mutationRates.connections = mutationRates.connections;
    Genome::mutationRates.link = mutationRates.link;
    Genome::mutationRates.node = mutationRates.node;
    Genome::mutationRates.bias = mutationRates.bias;
    Genome::mutationRates.enable = mutationRates.enable;
    Genome::mutationRates.disable = mutationRates.disable;
    Genome::mutationRates.step = mutationRates.step;
}

//GETTERS
MutationRates Genome::getMutationRates(){
    return mutationRates;
}

vector<Gene*> Genome::getGenes() {
    return genes;
}

int Genome::getAdjustedFitness(){
    return adjustedFitness;
}

int Genome::getLastNeuronCreated(){
    return lastNeuronCreated;
}

int Genome::getMaxNeuron() {
    return lastNeuronCreated;
}

int Genome::getGlobalRank(){
    return globalRank;
}

int Genome::getFitness() {
    return fitness;
}

//SETTERS

void Genome::setFitness(int fitness) {
    Genome::fitness = fitness;
}

void Genome::setLastNeuronCreated(int lastNeuronCreated) {
    Genome::lastNeuronCreated = lastNeuronCreated;
}

void Genome::setGlobalRank(int globalRank) {
    Genome::globalRank = globalRank;
}

void Genome::copyMutationRates(MutationRates mR){
    mutationRates = mR;
}

void Genome::addGene(Gene * g){
    // if that gene already exists, merge both weights
    if (containsLink(g)) {
        return;
    }

    // create new gene
    Gene * newGene = new Gene(g->getInto(), g->getOut(), g->getWeight(), g->isEnabled(), g->getInnovation());
    genes.push_back(newGene);
}

void Genome::mutate() {
    mutationRates.connections *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.link *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.bias *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.node *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.enable *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.disable *= ((rand()%2 == 1) ? 0.95 : 1.05263);
    mutationRates.step *= ((rand()%2 == 1) ? 0.95 : 1.05263);

    //CHECK mutationRates values:

    if (Debug) {
        cout << endl << endl << "MUTATE!" << endl;
        cout << "connections: " << mutationRates.connections << endl;
        cout << "link: " << mutationRates.link << endl;
        cout << "bias: " << mutationRates.bias << endl;
        cout << "node: " << mutationRates.node << endl;
        cout << "enable: " << mutationRates.enable << endl;
        cout << "disable: " << mutationRates.disable << endl;
        cout << "step: " << mutationRates.step << endl;
    }

    if (randomPercentage() < mutationRates.connections) weightMutate();
    if (randomPercentage() < mutationRates.link) linkMutate(false);
    if (randomPercentage() < mutationRates.bias) linkMutate(true);
    if (randomPercentage() < mutationRates.node) nodeMutate();
    if (randomPercentage() < mutationRates.enable) enableDisableMutate(true);
    if (randomPercentage() < mutationRates.disable) enableDisableMutate(false);
}

float Genome::innitializeWeight() {
    return randomPercentage() * 4 - 2;
}

void Genome::weightMutate() {
    double step = mutationRates.step;

    for (int i = 0; i < genes.size(); i++) {
        if (rand() % 2 < Pool::PerturbChance) {
            genes[i]->setWeight(genes[i]->getWeight() + randomPercentage() * step * 2 - step);
        } else genes[i]->setWeight(innitializeWeight());
    }
}

// crates a new gene that connects two random neurons that were not connected.
void Genome::linkMutate(bool forceBias) {
    int neuron1 = randomNeuron(true);
    int neuron2 = randomNeuron(false);

    int into = 0;
    int out = 0;
    bool enabled = true;

    if (forceBias) {
        neuron1 = Pool::INPUT_SIZE;
    }

    //If both neurons are input neurons, or
    // neuron1 = neuron2:
    // End function to not create a gene.
    if ((neuron1 <= Pool::INPUT_SIZE && neuron2 <= Pool::INPUT_SIZE) ||
            neuron1 == neuron2) {
        return;
    }



    cout << "New gene created!" << endl;
    cout << "Both neurons: " << neuron1 << " : " << neuron2 << endl;

    if (neuron2 < neuron1) {
        // Swap output and input to have the lowest number in neuron1
        int temp = neuron1;
        neuron1 = neuron2;
        neuron2 = temp;
    }

    into = neuron1;
    out = neuron2;


    Gene * newLink = new Gene(into, out, innitializeWeight(), enabled);

    // if that gene already exists, merge both weights
    if (containsLink(newLink)) {
        return;
    }


    // if the gene doesn't exist, add it to the array
    genes.push_back(newLink);
}

bool Genome::containsLink(Gene * link) {
    for (int i = 0; i < genes.size(); i++){
        if (genes[i]->getInto() == link->getInto() && genes[i]->getOut() == link->getOut()) {

            double newWeight = (genes[i]->getWeight() + link->getWeight())/2;
            genes[i]->setWeight(newWeight);

            return true;
        }
    }
    return false;
}


//create a new neuron between two connected ones    x->y      x->z->y
void Genome::nodeMutate() {
    if (genes.size() == 0)  return;


    int randGene =  rand() % genes.size();
    if (!genes[randGene]->isEnabled())  return;
    if (genes[randGene]->getOut() <= lastNeuronCreated){
        return;
    }

    // delete old gene
    genes[randGene]->setEnabled(false);



    lastNeuronCreated++;


    //Create gene1 that gets gene as input
    int into = genes[randGene]->getInto();
    int out = lastNeuronCreated;
    double weight = innitializeWeight();
    bool enabled = true;
    Gene * g1 = new Gene(into, out, weight, enabled);
    genes.push_back(g1);


    // Create gene2 that outputs to the gene
    into = lastNeuronCreated;
    out = genes[randGene]->getOut();
    weight = genes[randGene]->getWeight();
    enabled = true;
    Gene * g2 = new Gene(into, out, weight, enabled);
    genes.push_back(g2);
}


void Genome::enableDisableMutate(bool enable) {
    vector<int> candidates;
    for (int i = 0; i < genes.size(); i++) {
        if (genes[i]->isEnabled() == !enable) candidates.push_back(i);
    }

    if (candidates.size() == 0) return;


    int random = candidates[rand() % candidates.size()];
    genes[random]->setEnabled(!genes[random]->isEnabled());
}


// count all the neurons there is and give a random index of all neurons
// if there are no genes it will still return a random number between the inputs and the outputs
int Genome::randomNeuron(bool isInput) {
    vector<int> neurons;

    if (isInput) {
        // put into neurons the inputSize + the bias
        for (int i = 0; i <= Pool::INPUT_SIZE; i++) {
            neurons.push_back(i);
        }
    }
    else {
        for (int i = 0; i < Pool::OUTPUT_SIZE; i++){
            neurons.push_back(Pool::MaxNodes + i);
        }
    }

    // if we want an input neuron in return it will add all neurons appart from the outputs
    // if we want a non input neuron, it will add all neurons except of the input ones
    for (int i = 0; i < genes.size(); i++){
        int neuron;
        if (isInput) {
            neuron = genes[i]->getInto();
        }
        else {
            neuron = genes[i]->getOut();
        }
        //check if the neuron is already in the vector
        std::vector<int>::iterator it = find (neurons.begin(), neurons.end(), neuron);

        // if neuron is not in the vector
        if (it == neurons.end())
            neurons.push_back(neuron);
    }

    int randomNeuron = rand() % neurons.size();


    randomNeuron = neurons[randomNeuron];

    return randomNeuron;
}



void Genome::randomGenome() {
    lastNeuronCreated = Pool::INPUT_SIZE;

    for (int i = 0; i < randomPercentage()*1000; i++){
        if (randomPercentage() < mutationRates.link) linkMutate(false);
        if (randomPercentage() < mutationRates.bias) linkMutate(true);
        if (randomPercentage() < mutationRates.node) nodeMutate();
    }
}


bool isRhsOutBigger(Gene* a, Gene* b) { return (*a).getOut() < (*b).getOut() ; }

void Genome::generateNetwork() {
    if(network.size() > 0)
        network.clear();


    if (!genes.empty()) {
        //sort genes having the last ones to the right. biggest output neuron index to the right
        sort(genes.begin(), genes.end(), isRhsOutBigger);
    }

    // create neurons for all Inputs (tiles map) +  a neuron for the bias
    for (int i = 0; i < Pool::INPUT_SIZE + 1; i++) {
        network[i] = Neuron2();
    }

    // create neurons for all outputs (button controls)
    for (int i = 0; i < Pool::OUTPUT_SIZE; i++) {
        network[Pool::MaxNodes+i] = Neuron2();
    }


    for (int i = 0; i < genes.size(); i++) {

        if (genes[i]->isEnabled()) {
            // neuron is not in the map create neuron
            if (network.find(genes[i]->getOut()) == network.end()){
                network[genes[i]->getOut()] = Neuron2();
            }

            // add gene to the output neuron
            network[genes[i]->getOut()].addGene((*genes[i]));
        }
        else {
            // if gene is not enabled, delete gene
            genes.erase(genes.begin()+i);
            i--;
        }
    }
}


void Genome::evaluateNetworkToFile(vector<double> inputs) {
    if (inputs.size() != Pool::INPUT_SIZE) {
        string errorMsg = "INCORRECT NUMBER OF INPUTS\n";
        errorMsg += "Inputs expected: " + Pool::INPUT_SIZE;
        errorMsg += "\nInputs received: " + inputs.size();
        errorMsg += "\n";
        throw runtime_error(errorMsg);
    }

    // inputting the tile values into the "input layer"
    for (int i = 0; i < Pool::INPUT_SIZE; i++) {
        network[i].setValue(inputs[i]);
    }

    map<int, Neuron2>::const_iterator i = network.end();
    for (int x = 0; x < Pool::OUTPUT_SIZE; x++) {
        i--;
        evaluateNeuron(i->first);
    }


    //check which buttons are true and which are false
    int pressedButtons[3] = {false, false, false};
    for (int i = 0; i < Pool::OUTPUT_SIZE; i++) {
        if (network[Pool::MaxNodes + i].getValue() > 0.5) {
            pressedButtons[i] = true;
        }
    }

    // write to file
    ofstream myfile;
    string file = "../../AI-SpaceInvaders/Buttons.txt";
    myfile.open (file);
    if (!myfile.is_open()){
        throw runtime_error("Unable to open the file: "+ file);
    }

    myfile << pressedButtons[0];
    myfile << "\n";
    myfile << pressedButtons[1];
    myfile << "\n";
    myfile << pressedButtons[2];
    myfile << "\n";
    myfile << "Finished!";
    myfile.close();

    if (Debug) cout << "Pressed buttons: " <<  pressedButtons[0] << " " << pressedButtons[1] << " " <<pressedButtons[2] << endl;
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


    map<int, Neuron2>::const_iterator i = network.end();
    for (int x = 0; x < Pool::OUTPUT_SIZE; x++) {
        i--;
        evaluateNeuron(i->first);
    }


    int pressed = -1;
    int maxValue = -1;

    cout << "Network size: " << network.size() << endl;

    // go throught the outputs (in theory 4) and choose the output with the bigger value
    for (int i = 0; i < Pool::OUTPUT_SIZE; i++) {
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

    if (Debug) cout << "Pressed button: " <<  pressed << endl;

    return pressed;
}

double Genome::evaluateNeuron(int neuron){
    double sum = 0;
    for (int j = 0; j < network[neuron].getGenes().size(); j++) {
        Gene gene = network[neuron].getGenes()[j];
        double value = network[gene.getInto()].getValue();

        // if neuron has genes, evaluate neuron
        if (network[gene.getInto()].getGenes().size() > 0) value = evaluateNeuron(gene.getInto());

        sum += gene.getWeight() * value;
    }

    if (network[neuron].getGenes().size() > 0) {
        //sigmoid function. set the value between 0 and 1
        double sigmoid = (double) 1 / ((double)1 + exp(-sum));

        // normalize to -1 to 1
        sigmoid = (sigmoid - 0.5) * 2;
        network[neuron].setValue(sigmoid);
    }

    // if a network has not been initialized, asume it is a bias so return -1
    if (network[neuron].getValue() == -20) return -1;

    return network[neuron].getValue();
}

// print in console the Neural network that is connected to the outputs
void Genome::showGenome() {
    int j = Pool::OUTPUT_SIZE;
    for (map<int, Neuron2>::const_iterator i = network.end(); j > 0; i--) {
        if (i == network.end()) continue;
        j--;
        showGene(0, i->first);
    }
}

// print the genes connected to that neuron
void Genome::showGene(int iterations, int neuron){

    for (int i = 0; i < iterations; i++) {
        cout << "\t";
    }
    cout << neuron << ": " << network[neuron].getValue() << endl;


    for (int i = 0; i < network[neuron].getGenes().size(); i++) {
        showGene(iterations+1, network[neuron].getGenes()[i].getInto());
    }
}






