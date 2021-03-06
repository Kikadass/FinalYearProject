//
// Created by Kike Piera Serra on 08/01/2018.
//

#ifndef FINALYEARPROJECT_GENE_H
#define FINALYEARPROJECT_GENE_H


#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <map>

using namespace std;

class Gene {
public:
    Gene();

    Gene(int into, int out, double weight, bool enabled);

    Gene(int into, int out, double weight, bool enabled, int innovation);


    //SETTERS
    void setWeight(double weight);
    void setEnabled(bool enabled);

    //GETTERS
    int getInto();
    int getOut();
    int getInnovation();
    bool isEnabled();
    double getWeight();


private:
    int into;
    int out;
    double weight;
    bool enabled;
    int innovation;

    void setValues(int into, int out, double weight, bool enabled);
};


#endif //FINALYEARPROJECT_GENE_H
