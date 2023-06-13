#include <iostream>
#include "NodeOperation.h"

using namespace std;

/// @brief Constructor of NodeOperation that initializes all its attributes
///        when it knows its left and right children Nodes.
/// @param operation char that represents the arithmetic operation between left and right nodes.
/// @param left Left child node.
/// @param right Right child node.
NodeOperation::NodeOperation(char operation, Node *left, Node *right) {
    this->operation = operation;
    this->left = left;
    this->right = right;
    this->parent = nullptr;
    this->visited = false;
    type = OPERATOR;
}

/// @brief Constructor of NodeOperation that initializes all its attributes
///        when it DOESN'T know its left and right children nodes.
/// @param operation char that represents the arithmetic operation between left and right nodes.
NodeOperation::NodeOperation(char operation) {
    this->operation = operation;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->visited = false;
    type = OPERATOR;
}

/// @brief Destructor of NodeOperation class
NodeOperation::~NodeOperation() {
    if (left !=nullptr) {
        delete left;
    }
    
    if (right != nullptr) { 
        delete right;
    }
}

/// @brief Prints on screen the value of the Node with its children and operation.
void NodeOperation::print() {
        cout << "(" ;
        this->left->print();
        cout << this->operation;
        this->right->print();
        cout << ")" ;

}