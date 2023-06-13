#include <iostream>
#include "NodeNumber.h"

using namespace std;

/// @brief Constructor of NodeNumber class that initializes all its attributes
///        even type of Node.
/// @param number Represents the value of Node Number.
NodeNumber::NodeNumber(int number) {
    this->number = number;
    this->parent = nullptr;
    this->visited = false;
    type = NUMBER;
}

/// @brief Destructor of NodeNumber class.
NodeNumber::~NodeNumber() {
    delete this;
}

/// @brief Prints on screen the number value of the Node.
void NodeNumber::print() {
    cout << this->number;
}