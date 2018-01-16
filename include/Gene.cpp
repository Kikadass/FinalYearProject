//
// Created by Kike Piera Serra on 08/01/2018.
//

#include "Gene.h"

static int generalInnovation = 0;


Gene::Gene() {
    into = -1;
    out = -1;
    weight = 0;
    enabled = true;
    innovation = ++generalInnovation;
}


Gene::Gene(int into, int out, double weight, bool enabled){
    Gene::into = into;
    Gene::out = out;
    Gene::weight = weight;
    Gene::enabled = enabled;
    Gene::innovation = ++generalInnovation;
}

//GETTERS
int Gene::getInto() const {
    return into;
}

int Gene::getOut() const {
    return out;
}

double Gene::getWeight() const {
    return weight;
}

bool Gene::isEnabled() const {
    return enabled;
}

int Gene::getInnovation() const {
    return innovation;
}


//SETTERS

void Gene::innitializeWeight() {
    weight = rand() /double{RAND_MAX};
}

void Gene::setWeight(double weight){
    Gene::weight = weight;
}


void Gene::setEnabled(bool enabled) {
    Gene::enabled = enabled;
}

