//
// Created by Kike Piera Serra on 05/11/2017.
//

#ifndef FINALYEARPROJECT_TRAININGDATA_H
#define FINALYEARPROJECT_TRAININGDATA_H
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

class TrainingData {
public:
    TrainingData();
    void getTopology(vector<double> &topology);

    //Returns the number of input values read from the file
    unsigned getNextInputs(vector<double> &input);
    unsigned getTargetOutputs(vector<double> &targetOutput);

private:
    vector<double> targets;

};


#endif //FINALYEARPROJECT_TRAININGDATA_H
