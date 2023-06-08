#include "Node.h"

class NodeNumber : public Node {
public:
    int number;

    NodeNumber(int number);
    ~NodeNumber();
    void print();
};