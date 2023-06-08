#include <iostream>
#include "NodeNumber.h"

using namespace std;

NodeNumber::NodeNumber(int number) {
    this->number = number;
    this->parent = nullptr;
    this->visited = false;
    type = NUMBER;
}

NodeNumber::~NodeNumber() {
    delete this;
}

void NodeNumber::print() {
    cout << this->number;
}