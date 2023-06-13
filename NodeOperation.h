#include "Node.h"

/*
    * NodeOperation class:
    * This class heredates Node class attributes and methods, and represents
    * an arithmetic operation like (+, -, *, ^). Includes the left and right
    * Nodes whose represent mathematical operands and operation attributes
    * that represents the operator.
*/
class NodeOperation : public Node {
    public:
        // Attributes
        Node *left;
        Node *right;
        char operation;
        
        // Methods
        // Constructors
        NodeOperation(char operation, Node *left, Node *right);
        NodeOperation(char operation); // In case of no children

        // Destructor
        ~NodeOperation();

        // Other methods
        void print();
};