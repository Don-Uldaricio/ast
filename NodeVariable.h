#include "Node.h"

class NodeVariable : public Node {
public:
    char var;
    
    NodeVariable(char var);
    ~NodeVariable();
    void print();
};