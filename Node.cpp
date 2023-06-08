#include <iostream>
#include "Node.h"

using namespace std;

Node::Node() {
    this->parent = nullptr;
    type = INVALID;
}

Node::~Node() {
}

void Node::print() {
    cout << "Invalid Node" << endl;
}