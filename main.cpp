
#include <iostream>
#include <cmath>
#include <vector>
#include <assert.h>
#include "include/Net.h"
#include "include/TrainingData.h"


using namespace std;

// distance between point 1 and point 2
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void showVector(string label, vector<double> &v){
    cout << label << " ";
    for (int i = 0; i < v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
}

int main( int argc, char** argv ) {

    //srand(time(nullptr));

    TrainingData trainData;

    vector<double> topology;
    trainData.getTopology(topology);


    Net myNet(topology);



    vector<double> inputs;
    vector<double> targets;
    vector<double> results;



    int trainingPass = 0;

    while (trainingPass < 2000){
        trainingPass++;
        cout << endl << "Pass "<< trainingPass;

        // Get new inputs data and feed it forward;
        inputs.clear();
        if (trainData.getNextInputs(inputs) != topology[0]){
            break;
        }

        showVector(": Inputs:", inputs);
        myNet.feedForward(inputs);


        //Collect the net's actual results
        myNet.getResults(results);
        showVector("Outputs:", results);

        //Train the net what the ouputs should have been
        trainData.getTargetOutputs(targets);
        showVector("Targets:", targets);

        assert(targets.size() == topology.back());

        myNet.backProp(targets);

        //Report how well the training is working. averaged over recent...
        cout << "Net recent average error: " << myNet.getRecentAverageError() << endl;

    }

    cout << "DONE" << endl;
}