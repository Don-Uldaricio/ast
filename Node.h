#ifndef NODE_H
#define NODE_H

/*
    * Node class:
    * This class is used to represent a Node in a AST, a binary tree.
*/
class Node {
    #define NUMBER 0
    #define OPERATOR 1
    #define VARIABLE 2
    #define INVALID 3

    public:
        // Attributes
        int type;
        Node *parent;
        bool visited;

        // Methods
        // Constructor and Destructor
        Node();
        ~Node();

        // Other methods
        bool isNodeOperation();
        bool isNodeVariable();
        bool isNodeNumber();
        virtual void print();
};

#endif