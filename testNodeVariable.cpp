#include <iostream>
#include "Node.h"
#include "NodeVariable.h"

using namespace std;

int main() {
    NodeVariable *node = new NodeVariable('x');
    NodeVariable *p = new NodeVariable('f');
    NodeVariable *cloned = node->clone();

    node->parent = p;
    cout << "The value of node: "; node->print(); cout << endl;
    cout << "The value of node's parent: "; node->parent->print(); cout << endl;
    cout << "The value of cloned node: "; cloned->print(); cout << endl;
}