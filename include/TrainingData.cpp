//
// Created by Kike Piera Serra on 05/11/2017.
//

#include "TrainingData.h"

TrainingData::TrainingData(){

}

void TrainingData::getTopology(vector<double> &topology){
    topology.push_back(2);
    topology.push_back(4);
    topology.push_back(1);
}

unsigned TrainingData::getNextInputs(vector<double> &inputs){
    inputs.clear();

    input1 = rand()%2;
    input2 = rand()%2;

    targetOutput = input1^input2; //should be or 0 or 1

    inputs.push_back((double)input1);
    inputs.push_back((double)input2);

    return inputs.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputs){
    targetOutputs.clear();

    targetOutputs.push_back((double)targetOutput);

    return targetOutputs.size();
}
