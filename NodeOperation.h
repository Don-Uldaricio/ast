#include "Node.h"

class NodeOperation : public Node {
    public:
        Node *left;
        Node *right;
        char operation;
        void print();
        
        NodeOperation(char operation, Node *left, Node *right);
        NodeOperation(char operation); // caso de no conocer hijos
        ~NodeOperation();
};