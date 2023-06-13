#include <iostream>
#include "Node.h"

using namespace std;

/// @brief Constructor of Node class that initializes its attributes.
Node::Node() {
    this->parent = nullptr;
    type = INVALID;
}

/// @brief Destructor of Node class.
Node::~Node() {
}

/// @brief Prints on screen the content of the Node.
void Node::print() {
    cout << "Invalid Node" << endl;
}