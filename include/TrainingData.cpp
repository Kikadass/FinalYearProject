//
// Created by Kike Piera Serra on 05/11/2017.
//

#include "TrainingData.h"

TrainingData::TrainingData(){

}

void TrainingData::getTopology(vector<double> &topology){
    topology.push_back(2);
    topology.push_back(4);
    //topology.push_back(4);
    topology.push_back(1);
}

unsigned TrainingData::getNextInputs(vector<double> &inputs){
    inputs.clear();
    targets.clear();

    inputs.push_back((double)(rand()%5)/10.0);
    inputs.push_back(-(double)(rand()%5)/10.0);

    //targetOutput = inputs[0]^inputs[1]; //XOR
    //targetOutput = ((inputs[0]^inputs[1]) == 1) ? 0 : 1; // !XOR
    //targetOutput = (inputs[0] == inputs[1] && inputs[0] == 1) ? 1 : 0; // And
    //targetOutput = (inputs[0] == inputs[1] && inputs[0] == 1) ? 0 : 1; // !And
    //targetOutput = (inputs[0] == 1 || inputs[1] == 1) ? 1 : 0; // OR
    //targetOutput = (inputs[0] == 1 || inputs[1] == 1) ? 0 : 1; // !OR


    targets.push_back(inputs[0] - inputs[1]);
    //targets.push_back(inputs[0] + inputs[1]);

    return inputs.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputs){
    targetOutputs.clear();

    for (int i = 0; i < targets.size(); i++) {
        targetOutputs.push_back(targets[i]);
    }

    return targetOutputs.size();
}
