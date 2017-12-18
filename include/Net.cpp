//
// Created by Kike Piera Serra on 05/11/2017.
//

#include <assert.h>
#include <cmath>
#include "Net.h"


Net::Net(const vector<double> &topology){
    int numLayers = topology.size();

    for (int layerNum = 0; layerNum < numLayers; ++layerNum){
        m_layers.push_back(vector<Neuron>());
        unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

        //We have made a new Layer, now fill it with neurons
        // and add a bias neuron to the layer
        for (int neuronNum = 0; neuronNum < topology[layerNum]+1; neuronNum++){
            m_layers.back().push_back(Neuron(numOutputs, neuronNum));
            cout << "Made a Neuron" << endl;
        }

        //Force the bias node's output value to 1
        m_layers.back().back().setOutput(1);
    }

    recentAverageSmoothingFac = 100.0;
}


void Net::feedForward(vector<double> &inputs){

    // this is a check to know if our assumption is true. if not an error will appear
    assert(inputs.size() == m_layers[0].size()-1);

    //Assign (latch) the input values into the input neurons
    // Don't take bias into account
    for(int i = 0; i < inputs.size(); i++){
        m_layers[0][i].setOutput(inputs[i]);
    }

    //Forward propagation
    for (int layerNum = 1; layerNum < m_layers.size(); layerNum++) {

        vector<Neuron> &prevLayer = m_layers[layerNum-1];
        //cout << endl << "DIFFERENT LAYER:  " << layerNum << endl;

        // do not take into account bias
        for (int i = 0; i < m_layers[layerNum].size()-1; i++) {
            //cout << endl << "DIFFERENT Neuron:  " << i << endl;

            m_layers[layerNum][i].feedForward(prevLayer);
        }
    }
}


void Net::backProp(vector<double> &targets){
    // Caluclate overall net error RMS (Root mean square error) of output neuron errors
    vector<Neuron> &outputLayer = m_layers.back();
    error = 0;


    // not including the bias
    for (int i = 0; i < outputLayer.size()-1; i++){
        double delta = targets[i] - outputLayer[i].getOutput();
        //cout << "Delta: " << targets[i] << "-" << outputLayer[i].getOutput() << endl;

        error += delta*delta;
    }

    error /= outputLayer.size()-1; // get average error squared
    error = sqrt(error); //RMS


    //Implement a recent average measurement

    /*
    cout << "error" << error << endl;
    cout << "RAE" << recentAverageError<< endl;
    cout << "RASF" << recentAverageSmoothingFac<< endl;
    cout << "top" << (recentAverageError * recentAverageSmoothingFac + error)<< endl;
    cout << "bottom" << (recentAverageSmoothingFac + 1)<< endl;
    */

    recentAverageError = (recentAverageError * recentAverageSmoothingFac + error) / (recentAverageSmoothingFac + 1);

    // Calculate output layer gradients(bias not taken into account)
    for (int i = 0; i < outputLayer.size()-1; i++){
        outputLayer[i].calcOutputGradients(targets[i]);
    }

    // Calculate gradients on hidden layers
    for (int layerNum = m_layers.size()-2; layerNum > 0; --layerNum){
        vector<Neuron> &hiddenLayer = m_layers[layerNum];
        vector<Neuron> &nextLayer = m_layers[layerNum+1];

        for (int i = 0; i < hiddenLayer.size(); i++){
            hiddenLayer[i].calcHiddenGradients(nextLayer);
        }
    }


    // For all layers from outputs to firs hidden layer
    // update connection weights

    cout << "Updating Weights" << endl;
    for (int layerNum = m_layers.size()-1; layerNum > 0; --layerNum){
        vector<Neuron> &layer = m_layers[layerNum];
        vector<Neuron> &prevLayer = m_layers[layerNum -1];

        //cout << "DIFFERENT LAYER:  " << layerNum << endl;
        // don't take into account the bias as there are no connections from previous layer to the bias
        for (int i = 0; i < layer.size()-1; i++){
            //cout << "NEW Neuron: " <<i << endl << endl;

            layer[i].updateInputWeights(prevLayer);
        }
    }


}

void Net::getResults(vector<double> &result){
    result.clear();

    for(int i = 0; i < m_layers.back().size()-1; i++){
        result.push_back(m_layers.back()[i].getOutput());
    }
}

double Net::getRecentAverageError(){
    return recentAverageError;
}
