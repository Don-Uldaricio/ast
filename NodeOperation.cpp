#include <iostream>
#include "NodeOperation.h"

using namespace std;

NodeOperation::NodeOperation(char operation, Node *left, Node *right) {
    this->operation = operation;
    this->left = left;
    this->right = right;
    this->parent = nullptr;
    this->visited = false;
    type = OPERATOR;
}

NodeOperation::NodeOperation(char operation) {
    this->operation = operation;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->visited = false;
    type = OPERATOR;
}

NodeOperation::~NodeOperation() {
    if (left !=nullptr) {
        delete left;
    }
    
    if (right != nullptr) { 
        delete right;
    }
}

void NodeOperation::print() {

        cout << "(" ;
        this->left->print();
        cout << this->operation;
        this->right->print();
        cout << ")" ;
    
}