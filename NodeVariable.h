#include "Node.h"

/*
    * NodeVariable class:
    * This class heredates Node class attributes and methods and represents
    * a variable Node (like x or y in a mathematical function) with the char var
    * as its value.
*/
class NodeVariable : public Node {
public:
    // Attributes
    char var;
    
    // Methods
    // Constructor
    NodeVariable(char var);

    //Destructor
    ~NodeVariable();

    // Other methods
    NodeVariable *clone();
    void print();
};