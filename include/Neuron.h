//
// Created by Kike Piera Serra on 05/11/2017.
//

#ifndef FINALYEARPROJECT_NEURON_H
#define FINALYEARPROJECT_NEURON_H
#include <vector>
#include <cstdlib>

using namespace std;

struct Connection{
    double weight;
    double deltaWeight;
};

class Neuron {
public:
    Neuron(unsigned outputs, unsigned index);
    void setOutput(double val);
    double getOutput();
    void feedForward(vector<Neuron> & prevLayer);
    void calcOutputGradients(double target);
    void calcHiddenGradients(const vector<Neuron> &nextLayer);
    void updateInputWeights(vector<Neuron> &prevLayer);

private:
    // Variables
    double output;
    unsigned myIndex;
    vector<Connection> outputWeights;
    double gradient;
    static double eta;      // [0 to 1] overall net training rate
    static double alpha;    // [0 to n] multiplier of last weight change [momentun]

    //Functions
    double transferFunction(double x);
    double transferFunctionDerivative(double x);
    double randomWeight();

    double sumDOW(const vector<Neuron> &nextLayer) const;

};


#endif //FINALYEARPROJECT_NEURON_H
