#ifndef NODE_H
#define NODE_H

class Node {
    #define NUMBER 0
    #define OPERATOR 1
    #define VARIABLE 2
    #define INVALID 3

    public:
        int type;
        Node *parent;
        bool visited;

        Node();
        ~Node();
        int getType();
        virtual void print();
};

#endif