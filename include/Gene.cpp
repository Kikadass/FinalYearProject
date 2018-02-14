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
    setValues(into, out, weight, enabled);
}

Gene::Gene(int into, int out, double weight, bool enabled, int innovation){
    setValues(into, out, weight, enabled);
    Gene::innovation = innovation;
}

//GETTERS
int Gene::getInto() {
    return into;
}

int Gene::getOut() {
    return out;
}

double Gene::getWeight() {
    return weight;
}

bool Gene::isEnabled() {
    return enabled;
}

int Gene::getInnovation() {
    return innovation;
}


//SETTERS

void Gene::setValues(int into, int out, double weight, bool enabled){
    Gene::into = into;
    Gene::out = out;
    Gene::weight = weight;
    Gene::enabled = enabled;
}

void Gene::innitializeWeight() {
    weight = rand() /double{RAND_MAX};
}

void Gene::setWeight(double weight){
    Gene::weight = weight;
}


void Gene::setEnabled(bool enabled) {
    Gene::enabled = enabled;
}

