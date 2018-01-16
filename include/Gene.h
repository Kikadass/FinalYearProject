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

    //SETTERS
    void innitializeWeight();
    void setWeight(double weight);
    void setEnabled(bool enabled);

    //GETTERS
    int getInto() const;
    int getOut() const;
    int getInnovation() const;
    bool isEnabled() const;
    double getWeight() const;



private:
    int into;
    int out;
    double weight;
    bool enabled;
    int innovation;
};


#endif //FINALYEARPROJECT_GENE_H
