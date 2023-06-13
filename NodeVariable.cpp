#include <iostream>
#include "NodeVariable.h"

using namespace std;

/// @brief Constructor of NodeVariable class that initializes all
///        its values and its type of node.
/// @param var Represents the variable
NodeVariable::NodeVariable(char var) {
    this->var = var;
    this->parent = nullptr;
    this->visited = false;
    type = VARIABLE;
}

/// @brief Destructor of NodeVariable
NodeVariable::~NodeVariable() {
    delete this;
}

/// @brief Prints on screen the variable
void NodeVariable::print() {
    cout << this->var;
}