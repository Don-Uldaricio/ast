#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Node.h"

using namespace std;

/*
    * AST class:
    * This class is used to represent an Arithmetic Syntactic Tree, by reading
    * a .txt file for its construction.
    * The AST is represented for a root Node which contains all the operations
    * variables and numbers.
*/
class AST {
    public:
        // Atributtes
        Node *root;

        // Methods
        // Constructor
        AST(string fileName);
        
        // Other methods
        bool isOperator(string token);
        bool isVariable(string token);
        bool isNumber(string token);

        bool equal(Node *n1, Node *n2);
        Node *clone(Node *node);
        Node *sort(Node *node);
        Node *evalRecursive(Node *node);
        Node *replace(Node *node, char *variables, int *values, int n);
        Node *derive(Node *node, char var);
        Node *simplify(Node *node);
        Node *reduceVariable(Node *node, Node *varNode, int n, char x);
        Node *reduceAddNumbers(Node *node, Node *numNode, int sum);
        int sumLevelNumbers(Node *node);
        Node *firstAddNumber(Node *node);
        Node *searchVariable(Node *node, char x);
        int countVar(Node *node, char x);
        void printAST();

};