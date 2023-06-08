#include <iostream>
#include "NodeVariable.h"

using namespace std;

NodeVariable::NodeVariable(char var) {
    this->var = var;
    this->parent = nullptr;
    this->visited = false;
    type = VARIABLE;
}

NodeVariable::~NodeVariable() {
    delete this;
}

void NodeVariable::print() {
    cout << this->var;
}