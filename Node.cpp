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

/// @brief Decide if the read Node it's a Operation Node.
/// @param node Read Node
/// @return True if it's. False if it's not.
bool Node::isNodeOperation() {
    return this->type == OPERATOR;
}

/// @brief Decide if the read Node it's a Variable Node.
/// @param node Read Node
/// @return True if it's. False if it's not.
bool Node::isNodeVariable() {
    return this->type == VARIABLE;
}

/// @brief Decide if the read Node it's a Number Node.
/// @param node Read Node
/// @return True if it's. False if it's not.
bool Node::isNodeNumber() {
    return this->type == NUMBER;
}

/// @brief Prints on screen the content of the Node.
void Node::print() {
    cout << "Invalid Node" << endl;
}