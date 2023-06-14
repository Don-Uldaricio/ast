#include "Node.h"

/*
    * NodeNumber class:
    * This class heredates Node class attributes and methods,
    * and represents a Node Number which contains a number as a attribute.
*/
class NodeNumber : public Node {
public:
    // Attributes
    int number;

    // Constructor
    NodeNumber(int number);

    // Destructor
    ~NodeNumber();

    // Other methods
    NodeNumber *clone();
    void print();
};