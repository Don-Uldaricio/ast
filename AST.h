#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Node.h"

using namespace std;

class AST {
    public:
        Node *root;

        AST(string fileName);
        Node *derivate(Node *node, char var);
        Node *clone(Node *node);
        void relinkNode(Node *child, Node *parent);

        Node *evalRecursive(Node *node);
        Node *replace(Node *node, char *variables, int *values, int n);

        bool isOperator(string token);
        bool isVariable(string token);
        bool isNumber(string token);

        bool isNodeOperation(Node *node);
        bool isNodeVariable(Node *node);
        bool isNodeNumber(Node *node);

        void printAST();

};