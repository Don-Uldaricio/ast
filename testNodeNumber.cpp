#include <iostream>
#include "Node.h"
#include "NodeNumber.h"

using namespace std;

int main() {
    NodeNumber *node = new NodeNumber(2);
    NodeNumber *p = new NodeNumber(4);
    NodeNumber *cloned = node->clone();

    node->parent = p;
    cout << "The value of node: "; node->print(); cout << endl;
    cout << "The value of node's parent: "; node->parent->print(); cout << endl;
    cout << "The value of cloned node: "; cloned->print(); cout << endl;
}