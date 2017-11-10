//
// Created by Kike Piera Serra on 05/11/2017.
//

#include <cmath>
#include <iostream>
#include "Neuron.h"

Neuron::Neuron(unsigned outputs, unsigned index){
    for (int c = 0; c < outputs; c++){
        outputWeights.push_back(Connection());
        outputWeights.back().weight = randomWeight();
        outputWeights.back().deltaWeight = 1;
    }
    myIndex = index;
}

double Neuron::eta = 0.5; // overall net learning rate
double Neuron::alpha = 0.1; // momentum, multiplier of last deltaWeight [0 to n]

double Neuron::transferFunction(double x){
    // tanh - output range [-1.0..1.0]
    return tanh(x);
}

double Neuron::transferFunctionDerivative(double x){
    // tanh derivative
    return 1-x*x;
}


double Neuron::randomWeight(){
    return rand() /double{RAND_MAX};
}

void Neuron::setOutput(double val){
    output = val;
}

double Neuron::getOutput(){
    return output;
}

void Neuron::feedForward(vector<Neuron> & prevLayer){
    double sum = 0;

    // Sum the previous layer's outputs (which are our inputs)
    // Include the bias node from the previous layer

    for (int i = 0; i < prevLayer.size(); i++){
        sum += prevLayer[i].getOutput() * prevLayer[i].outputWeights[myIndex].weight;
        //cout << "Prev output: " << prevLayer[i].getOutput() << endl;
        //cout << "Weight: " << prevLayer[i].outputWeights[myIndex].weight << endl;
    }

    output = Neuron::transferFunction(sum);
    //cout << "Output: " << output << endl;
}

void Neuron::calcOutputGradients(double target){
    double delta = target - output;
    gradient = delta * transferFunctionDerivative(output);
    /*
    cout << "gradient of output: " << gradient << endl;
    cout << "transferFunctionDerivative(output): " << transferFunctionDerivative(output) << endl;
    cout << "delta: " << delta << endl;
     */

}

void Neuron::calcHiddenGradients(const vector<Neuron> &nextLayer){
    double dow = sumDOW(nextLayer);

    gradient = dow * Neuron::transferFunctionDerivative(output);
}

double Neuron::sumDOW(const vector<Neuron> &nextLayer) const{
    double sum = 0;

    // Sum our contributions of the errors at the nodes we feed
    for(int i = 0; i < nextLayer.size(); i++){
        sum += outputWeights[i].weight * nextLayer[i].gradient;
    }

    return sum;
}

void Neuron::updateInputWeights(vector<Neuron> &prevLayer){
    // The weights to be updated are in the Connection container in the neurons in the preceding layer
    for (int i = 0; i < prevLayer.size(); i++){
        Neuron &neuron = prevLayer[i];
        double oldDeltaWeight = neuron.outputWeights[myIndex].deltaWeight;



        double newDetlaWeight =
                // Individual input. Magnified by the gradient and train raite
                eta* neuron.getOutput()
                * gradient
                // Also add momentum = a fraction of the previous weight
                +alpha*oldDeltaWeight;

/*
        cout << "ETA " << eta << endl;
        cout << "neuron.getOutput() " << neuron.getOutput() << endl;
        cout << "gradient " << gradient << endl;
        cout << "alpha " << alpha << endl;
        cout << "oldDeltaWeight " << oldDeltaWeight << endl;
        cout << "newDetlaWeight " << newDetlaWeight << endl;
*/

        if (newDetlaWeight == 0){
            while (true){}
        }
        neuron.outputWeights[myIndex].deltaWeight = newDetlaWeight;
        neuron.outputWeights[myIndex].weight += newDetlaWeight;


        //cout << "New Delta Weight: " << neuron.outputWeights[myIndex].deltaWeight << endl;
        //cout << "New Weight: " << neuron.outputWeights[myIndex].weight << endl;
    }

}
