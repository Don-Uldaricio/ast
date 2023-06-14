#include <iostream>
#include "Node.h"
#include "NodeOperation.h"
#include "NodeNumber.h"
#include "NodeVariable.h"

using namespace std;

int main() {
    NodeOperation *op2 = new NodeOperation('*');
    NodeOperation *op3 = new NodeOperation('-');
    NodeOperation *root = new NodeOperation('+', op2, op3);
    NodeVariable *v1 = new NodeVariable('x');
    NodeVariable *v2 = new NodeVariable('y');
    NodeNumber *n1 = new NodeNumber(2);
    NodeNumber *n2 = new NodeNumber(1);

    // Asign children to op1
    op2->left = v1;
    op2->right = n1;
    op3->left = v2;
    op3->right = n2;

    // Prints op1 node
    cout << "The root node is: "; root->print(); cout << endl;
}