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
        void relinkNode(Node *child, Node *parent); // NO HECHO AUN
        bool equal(Node *n1, Node *n2);

        Node *sort(Node *node);

        Node *evalRecursive(Node *node);
        Node *replace(Node *node, char *variables, int *values, int n);
        Node *derivate(Node *node, char var);
        Node *simplify(Node *node);
        Node *reduceVariable(Node *node, Node *varNode, int n, char x);
        Node *reduceAddNumbers(Node *node, Node *numNode, int sum);
        int sumLevelNumbers(Node *node);
        Node *firstAddNumber(Node *node);
        void deleteVarNode(Node *node, Node *parent);
        Node *searchVariable(Node *node, char x);
        int countVar(Node *node, char x);

        Node *clone(Node *node);

        bool isOperator(string token);
        bool isVariable(string token);
        bool isNumber(string token);

        bool isVarMultiply(Node *node, char x);

        bool isNodeOperation(Node *node);
        bool isNodeVariable(Node *node);
        bool isNodeNumber(Node *node);

        void printAST();

};