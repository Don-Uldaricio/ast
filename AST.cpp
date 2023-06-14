#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <stack>
#include <cmath>

#include "AST.h"
#include "NodeOperation.h"
#include "NodeNumber.h"
#include "NodeVariable.h"

using namespace std;

/// @brief Constructor of AST that initializes the root attribute by reading the name file.
/// @param fileName String that represents the file name.
AST::AST(string fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Could not open file " << endl;
        exit(1);
    }
    NodeOperation *op = nullptr; 
    NodeNumber *num = nullptr;
    NodeVariable *var = nullptr;
    Node* node= nullptr;
    this->root=nullptr;

    string line, token;
    stringstream ss;

    stack<NodeOperation*> pila;

    // It initializes the array "vars" and the amount of variables in the file
    nvars = 0;
    for (int i = 0; i < 10; i++) {
        this->vars[i] = '.';
    }

    if (getline(file, line, '\n')) {
        ss << line;
        while(getline(ss, token, ' ')) {
            // 1. Create Node depending on the token type
            if (isOperator(token)) {
                node = new NodeOperation(token[0]);
                if (!pila.empty()) { 
                    ((NodeOperation*)node)->parent = pila.top();
                } else {
                    ((NodeOperation*)node)->parent = nullptr;
                }
            }
            else if (isNumber(token)) {
                int valor = stoi(token);
                node = new NodeNumber(valor);

            }
            else if (isVariable(token)) {
                char name = token[0];
                // Verificate if variable is already in "vars" array
                bool found = false;
                int i = 0;
                while (!found && i < 10) {
                    if (name == this->vars[i]) {
                        found = true;
                    }
                    else if (i == nvars) {
                        this->vars[i] = name;
                        found = true;
                        nvars++;
                    }
                    else {
                        i++;
                    }
                }
                node = new NodeVariable(name);
            }
            // 2. If it's first Node it's root
            if (this->root == nullptr) {
                this->root = node;
            }
            // 3. Depending on the stack top, it's add on the left or right
            if (!pila.empty()) {
                // Parent asignation
                node->parent = pila.top();    
                if (pila.top()->left == nullptr) {  
                    pila.top()->left = node;
                } 
                else if (pila.top()->right == nullptr) {
                    pila.top()->right = node;
                    pila.pop();
                }
                else {
                    cout << "Error" << endl;
                    exit(1);
                }
            }
            // 4. If it's an operator it's added to pill
            if (node->type == OPERATOR) {
                    pila.push((NodeOperation*)node);
                
            }
        }
    }
    else {
        cout << "Could not read line" << endl;
        exit(1);
    }
    file.close();
}

/// @brief Decide if the read token it's an operator.
/// @param token Read string
/// @return True if it's an operator; false if it's not.
bool AST::isOperator(string token) {
    return token == "+" || token == "-" || token == "*" || token == "^";
}

/// @brief Decide if read token it's a number.
/// @param token Read string
/// @return True if it's a number; false if it's not.
bool AST::isNumber(string token) {
    return token[0] >= '0' && token[0] <= '9';
}

/// @brief Decide if read token it's a variable.
/// @param token Read string
/// @return True if it's a variable (character). False if it's not.
bool AST::isVariable(string token) {
    return (token[0] >= 'a' && token[0] <= 'z') || (token[0] >= 'A' && token[0] <= 'Z');
}

/// @brief Operate two numbers.
/// @param operation Operation character (+, -, *, ^)
/// @param l First number
/// @param r Second number
/// @return Integer result of operate the two numbers.
int operate(char operation, int l, int r) {
        switch(operation) {
            case '+': //
                return l + r;
            case '-':
                return l - r;
            case '*':
                return l * r;
            case '^':
                return pow(l,r);
            default:
                cout << "Error" << endl;
                exit(1);
        }
}

/// @brief Evaluate recursively all the operations with numbers Nodes.
/// @param node Root Node.
/// @return Node with the result after operate all numbers Nodes.
Node *AST::evalRecursive(Node* node) {
    if (node->isNodeOperation()) {
        // Post-order evaluate
        NodeOperation *op = (NodeOperation *)node;
        Node *l = op->left;
        Node *r = op->right;
        l = evalRecursive(l);
        r = evalRecursive(r);
        
        if (l->type == NUMBER && r->type == NUMBER) {
            NodeNumber* num = new NodeNumber(operate(op->operation, 
                                                       ((NodeNumber*)l)->number, 
                                                       ((NodeNumber*)r)->number));
            // Updating parent link
            num->parent = op->parent;
            NodeOperation *parent = (NodeOperation *)op->parent;
            // Updating left or right parent child
            if (op->parent!=nullptr && parent->left==op) {
                parent->left = num;
            }
            else if (op->parent!=nullptr && parent->right==op) {
                parent->right = num;
            }
            if (op == this->root) {
                this->root = num;
            }
            return num;
        }
        else {
            return op;
        }
    }
    else if (node->isNodeNumber() || node->isNodeVariable()) {
        return node;
    }
    else {
        cout << "Error: Indefined type of Node." << endl;
        exit(1);
    }
}

/// @brief Replace all the Variable Nodes that match with the selected variable 
///        with the asigned number.
/// @param node Root Node.
/// @param variables Array with the selected variables to replace.
/// @param values Array with the asigned values to the variables.
/// @param n Number of variables.
/// @return Node with the result after replace the variables.
Node *AST::replace(Node *node, char *variables, int *values, int n) {
    if (node->isNodeOperation()) {
        NodeOperation *op = (NodeOperation *)node;
        op->left = replace(((NodeOperation *)node)->left, variables, values, n);
        op->right = replace(((NodeOperation *)node)->right, variables, values, n);
        return evalRecursive(op);
    }
    else if (node->isNodeVariable()) {
        NodeVariable *v = (NodeVariable *)node;
        int i = 0;
        while (i < n) {
            if (v->var == variables[i]) {
                return new NodeNumber(values[i]);
            }
            i++;
        }
        return node;
    }
    else if (node->isNodeNumber()) {
        return node;
    }
    else {
        return nullptr;
    }
}

/// @brief derive the AST with respect to a variable.
/// @param node Root Node.
/// @param x Variable (char) to derive.
/// @return Root Node after derive the AST.
Node *AST::derive(Node *node, char x) {
    // Derive an Operation Node
    if (node->isNodeOperation()) {
        char op = ((NodeOperation *)node)->operation;
        NodeOperation *oldRoot = (NodeOperation *)node;
        // Derive cases (+, -, *, ^)
        if (op == '+' || op == '-') {
            oldRoot->left = derive(oldRoot->left, x);
            oldRoot->right = derive(oldRoot->right, x);
            if (node == this->root) {
                this->root = node;
            }
            return evalRecursive(oldRoot);
        }
        else if (op == '*') {
            NodeOperation *newParent = new NodeOperation('+');
            NodeOperation *leftMult = oldRoot;
            NodeOperation *rightMult = new NodeOperation('*');
            Node *leftClone = oldRoot->left->clone();
            Node *rightClone = oldRoot->right->clone();

            // Asign new parents
            newParent->parent = node->parent;
            leftMult->parent = newParent;
            rightMult->parent = newParent;
            leftClone->parent = leftMult;
            rightClone->parent = rightMult;
            oldRoot->left->parent = rightMult;

            // Asign new childs of operation add nodes
            newParent->left = leftMult;
            newParent->right = rightMult;
            rightMult->left = oldRoot->left;
            oldRoot->left = derive(leftClone, x);
            rightMult->right = derive(rightClone, x);

            if (node == this->root) {
                this->root = newParent;
            }

            return evalRecursive(newParent);
        }
        else if (op == '^') {
            Node *leftClone = oldRoot->left->clone();
            NodeOperation *newParent = new NodeOperation('*');
            NodeOperation *auxMult = new NodeOperation('*');
            NodeNumber *newExp = new NodeNumber((((NodeNumber *)oldRoot->right)->number) - 1);

            // Asign new parents
            newParent->parent = oldRoot->parent;
            oldRoot->right->parent = newParent;
            auxMult->parent = newParent;
            oldRoot->parent = auxMult;
            leftClone->parent = auxMult;
            newExp->parent = oldRoot;

            // Asign new childs
            newParent->left = oldRoot->right;
            newParent->right = auxMult;
            auxMult->left = oldRoot;
            auxMult->right = derive(leftClone, x);
            oldRoot->right = newExp;

            if (node == this->root) {
                this->root = newParent;
            }

            return evalRecursive(newParent);
        }
        else {
            return nullptr;
        }
    }
    // Derive a Variable Node
    else if (node->isNodeVariable()) {
        if (((NodeVariable *)node)->var == x) {
            Node *auxNode = new NodeNumber(1);
            return auxNode;
        }
        else {
            Node *auxNode = new NodeNumber(0);
            return auxNode;
        }
    }
    // Derive a Node Number. Spoiler: equal to zero
    else if (node->isNodeNumber()) {
        Node *auxNode = new NodeNumber(0);
        return auxNode;
    }
    else {
        return nullptr;
    }
}

/// @brief Compare two Nodes and decides if their contents are equal.
/// @param n1 Node
/// @param n2 Node
/// @return True if their contets are equal. False if are not.
bool AST::equal(Node *n1, Node *n2) {
    if (n1->isNodeOperation() && n2->isNodeOperation()) {
        NodeOperation *aux1 = (NodeOperation *)n1;
        NodeOperation *aux2 = (NodeOperation *)n2;
        if (aux1->operation == aux2->operation) {
            return (equal(aux1->left, aux2->left) && equal(aux1->right, aux2->right));
        }
        else {
            return false;
        }
    }
    else if (n1->isNodeNumber() && n2->isNodeNumber()) {
        return ((NodeNumber *)n1)->number == ((NodeNumber *)n2)->number;
    }
    else if (n1->isNodeVariable() && n2->isNodeVariable()) {
        return ((NodeVariable *)n1)->var == ((NodeVariable *)n2)->var;
    }
    else {
        return false;
    }
}

/// @brief Sort the Node leaving the NumberNode on the left 
///        and VariableNode on the right.
/// @param node Root Node.
/// @return Node after sort all the child Nodes.
Node *AST::sort(Node *node) {
    if (node->isNodeOperation()) {
        NodeOperation *auxNode = (NodeOperation *)node;
        if ((auxNode->operation == '*' || auxNode->operation == '+') && 
            auxNode->left->isNodeVariable() && auxNode->right->isNodeNumber()) {
            NodeVariable *auxVar = new NodeVariable(((NodeVariable *)auxNode->left)->var);
            auxNode->left = auxNode->right;
            auxNode->right = auxVar;
            auxVar->parent = auxNode;
        }
        else if (auxNode->left->isNodeOperation() && auxNode->right->isNodeNumber() && 
                auxNode->operation == '*') {
            NodeNumber *auxNumber = new NodeNumber(((NodeNumber *)auxNode->right)->number);
            auxNode->right = sort(auxNode->left);
            auxNode->left = auxNumber;
            auxNumber->parent = auxNode;
        }
        else {
            auxNode->left = sort(auxNode->left);
            auxNode->right = sort(auxNode->right);
        }
        return node;
    }
    else {
        return node;
    }
}

/// @brief Calculate the sum of the x variable out of multiply Nodes.
/// @param node Root Node.
/// @param x Variable to be added
/// @return The result of adding all the x variables outside of multiply Nodes.
int AST::countVar(Node *node, char x) {
    if (node->isNodeOperation()) {
        NodeOperation *auxNode = (NodeOperation *)node;
        if (auxNode->operation == '+') {
            return countVar(auxNode->left, x) + countVar(auxNode->right, x);
        }
        else if (auxNode->operation == '*') {
            if (auxNode->left->isNodeNumber() && auxNode->right->isNodeVariable()) {
                if (((NodeVariable *)auxNode->right)->var == x) {
                    return ((NodeNumber *)auxNode->left)->number;
                }
            }
        }
    }
    else if (node->isNodeVariable()) {
        if (((NodeVariable *)node)->var == x) {
            return 1;
        }
    }
    return 0;
}

/// @brief Calculate the sum of all Number Nodes out of the multiply Nodes.
/// @param node Root node.
/// @return The sum of all Number Nodes out of multiply Nodes.
int AST::sumLevelNumbers(Node *node) {
    if (node->isNodeOperation()) {
        NodeOperation *auxNode = (NodeOperation *)node;
        if (auxNode->operation == '+') {
            return sumLevelNumbers(auxNode->left) + sumLevelNumbers(auxNode->right);
        }
    }
    else if (node->isNodeNumber()) {
        return ((NodeNumber *)node)->number;
    }
    return 0;
}

/// @brief Searches the first Number Node out of a multiply Node.
/// @param node Root Node.
/// @return First Number Node found.
Node *AST::firstAddNumber(Node *node) {
    if (node->isNodeOperation()) {
        if (((NodeOperation *)node)->operation == '+') {
            NodeOperation *auxNode = (NodeOperation *)node;
            Node *l = firstAddNumber(auxNode->left);
            Node *r = firstAddNumber(auxNode->right);
            if (l != nullptr) {
                return l;
            }
            else if (r != nullptr) {
                return r;
            }
        }
    }
    else if (node->isNodeNumber()) {
        return node;
    }
    return nullptr;
}

/// @brief Reduce the AST by replace the first Number Node found in the AST
///        out of multiply Nodes and eliminates the rest of Number Nodes.
/// @param node Root Node.
/// @param numNode First Number Node in the AST.
/// @param sum Sum of the all Node Numbers.
/// @return Root Node after reduce the AST.
Node *AST::reduceAddNumbers(Node *node, Node *numNode, int sum) {
    node = evalRecursive(node);
    if (node->isNodeOperation()) {
        NodeOperation *auxNode = (NodeOperation *)node;
        if (auxNode->operation == '+') {
            if (auxNode->left->isNodeNumber() && auxNode->left != numNode) {
                if (auxNode == this->root) {
                    this->root = auxNode->right;
                }
                node = auxNode->right;
                node = reduceAddNumbers(node, numNode, sum);
                return node;
            }
            else if (auxNode->right->isNodeNumber() && auxNode->right != numNode) {
                if (auxNode == this->root) {
                    this->root = auxNode->left;
                }
                node = auxNode->left;
                node = reduceAddNumbers(node, numNode, sum);
                return node;
            }
            auxNode->left = reduceAddNumbers(auxNode->left, numNode, sum);
            auxNode->right = reduceAddNumbers(auxNode->right, numNode, sum);
            return auxNode;
        }
        else {
            auxNode->left = reduceAddNumbers(auxNode->left, firstAddNumber(auxNode->left), sumLevelNumbers(auxNode->left));
            auxNode->right = reduceAddNumbers(auxNode->right, firstAddNumber(auxNode->right), sumLevelNumbers(auxNode->right));
        }
        return node;
    }
    else if (node->isNodeNumber()) {
        if (node == numNode) {
            ((NodeNumber *)node)->number = sum;
        }
    }
    return node;
}

/// @brief Reduce the AST by replace the first Variable Node of x found in the AST
///        out of multiply Nodes and eliminates the rest of Variables Nodes.
/// @param node Root Node.
/// @param varNode First Variable Node in the AST.
/// @param n Sum of the all Node Numbers.
/// @param x Variable to reduce.
/// @return Root Node after reduce the AST.
Node *AST::reduceVariable(Node *node, Node *varNode, int n, char x) {
    if (node->isNodeOperation()) {
        NodeOperation *auxNode = (NodeOperation *)node;
        // Reduce cases
        // 1. If operation is +
        if (auxNode->operation == '+') {
            // 1.1. If left Node is x variable and not equal to first Variable Node
            if (auxNode->left->isNodeVariable() && auxNode->left != varNode) {
                if (((NodeVariable *)auxNode->left)->var == x) {
                    if (auxNode == this->root) {
                        this->root = auxNode->right;
                    }
                    node = auxNode->right;
                    return node;
                }
            }
            // 1.2. If right Node is x variable and not equal to first Variable Node
            else if (auxNode->right->isNodeVariable() && auxNode->right != varNode) {
                if (((NodeVariable *)auxNode->right)->var == x) {
                    if (auxNode == this->root) {
                        this->root = auxNode->left;
                    }
                    node = auxNode->left;
                    return node;
                }
            }
            // 1.3. If left Node is multiply Node and there's a number multiplying the x variable.
            else if (auxNode->left->isNodeOperation()) {
                NodeOperation *auxOp = (NodeOperation *)auxNode->left;
                if (auxOp->operation == '*' && auxOp->left->isNodeNumber() && auxOp->right->isNodeVariable()) {
                    NodeVariable *auxVar = (NodeVariable *)auxOp->right;
                    if (auxVar->var == x) {
                        if (auxVar == varNode) {
                            ((NodeNumber *)auxOp->left)->number = n;
                        }
                    }
                }
            }
            // 1.4. If right Node is mupltiply Node and there's a number multiplying the x variable.
            else if (auxNode->right->isNodeOperation()) {
                NodeOperation *auxOp = (NodeOperation *)auxNode->right;
                if (auxOp->operation == '*' && auxOp->left->isNodeNumber() && auxOp->right->isNodeVariable()) {
                    NodeVariable *auxVar = (NodeVariable *)auxOp->right;
                    if (auxVar->var == x) {
                        if (auxVar == varNode) {
                            ((NodeNumber *)auxOp->left)->number = n;
                        }
                    }
                }
            }
            // Continues to reducing the left and right child
            auxNode->left = reduceVariable(auxNode->left, varNode, n, x);
            auxNode->right = reduceVariable(auxNode->right, varNode, n, x);
            return auxNode;
        }
        // If operation is another one (-, *, ^)
        else {
            auxNode->left = reduceVariable(auxNode->left, searchVariable(auxNode->left, x), countVar(auxNode->left, x), x);
            auxNode->right = reduceVariable(auxNode->right, searchVariable(auxNode->right, x), countVar(auxNode->right, x), x);
        }
        return auxNode;
    }
    else if (node->isNodeVariable()) {
        if (node == varNode && node->visited == false) {
            NodeOperation *newOp = new NodeOperation('*');
            newOp->left = new NodeNumber(n);
            newOp->right = new NodeVariable(x);
            newOp->left->parent = newOp;
            newOp->right->parent = newOp;
            newOp->right->visited = true;
            if (((NodeOperation *)node->parent)->right == node) {
                ((NodeOperation *)node->parent)->right = newOp;
                newOp->parent = node->parent;
            }
            else if (((NodeOperation *)node->parent)->left == node) {
                ((NodeOperation *)node->parent)->left = newOp;
                newOp->parent = node->parent;
            }
            return newOp;
        }
        else if (((NodeVariable *)node)->var == x) {
            if (((NodeOperation *)node->parent)->left == node) {
                node->parent = ((NodeOperation *)node->parent)->right;
            }
            else if (((NodeOperation *)node->parent)->right == node) {
                node->parent = ((NodeOperation *)node->parent)->left;
            }
        }
        return node;
    }
    return node;
}

/// @brief Search the first add Variable Node of x out of multiply Node
/// @param node Root Node.
/// @param x Searched variable.
/// @return First add Variable Node of x variable.
Node *AST::searchVariable(Node *node, char x) {
    if (node->isNodeOperation()) {
        if (((NodeOperation *)node)->operation == '+') {
            NodeOperation *auxNode = (NodeOperation *)node;
            if (auxNode->left->isNodeOperation()) {
                NodeOperation *auxOp = (NodeOperation *)auxNode->left;
                if (auxOp->operation == '*' && auxOp->right->isNodeVariable() && auxOp->left->isNodeNumber()) {
                    if (((NodeVariable *)auxOp->right)->var == x) {
                        return (NodeVariable *)auxOp->right;
                    }
                }
            }
            else if (auxNode->right->isNodeOperation()) {
                NodeOperation *auxOp = (NodeOperation *)auxNode->right;
                if (auxOp->operation == '*' && auxOp->right->isNodeVariable() && auxOp->left->isNodeNumber()) {
                    if (((NodeVariable *)auxOp->right)->var == x) {
                        return (NodeVariable *)auxOp->right;
                    }
                }
            }
            Node *l = searchVariable(auxNode->left, x);
            Node *r = searchVariable(auxNode->right, x);
            if (l != nullptr) {
                return l;
            }
            else if (r != nullptr) {
                return r;
            }
        }
    }
    else if (node->isNodeVariable()) {
        if (((NodeVariable *)node)->var == x) {
            return node;
        }
    }
    return nullptr;
}

/// @brief Reduces the AST by adding the x variable and numbers.
///        Applies the reduceVariable() and reduceAddNumbers() methods. 
/// @param node Root node.
/// @return Root node after reducing the AST.
Node *AST::simplify(Node *node) {
    node = sort(node);
    Node *clonedNode = node->clone();
    bool same = false;
    while (!same) {
        for (int i = 0; i < this->nvars; i++) {
            char v = this->vars[i];
            node = reduceVariable(node, searchVariable(node, v), countVar(node, v), v);
        }
        node = reduceAddNumbers(node, firstAddNumber(node), sumLevelNumbers(node));
        if (equal(clonedNode, node)) {
            same = true;
        }
        else {
            clonedNode = node->clone();
        }
    }
    if (node->isNodeOperation()) {
        // Here are all conditions for simplify polynomiums
        NodeOperation *auxNode = (NodeOperation *)node;
        auxNode->left = simplify(auxNode->left);
        auxNode->right = simplify(auxNode->right);

        // 1. Substract cases
        if (auxNode->operation == '-') {
            // 1.1. Equal nodes
            if (equal(auxNode->left, auxNode->right)) {
                return new NodeNumber(0);
            }
            // 1.2. Minus zero
            else if (auxNode->right->isNodeNumber()) {
                if (((NodeNumber *)auxNode->right)->number == 0) {
                    return auxNode->left;
                }
            }
        }

        // 2. Add cases
        else if (auxNode->operation == '+') {
            // 2.1. Add equal nodes
            if (equal(auxNode->left, auxNode->right)) {
                auxNode->operation = '*';
                auxNode->left = new NodeNumber(2);
            }
            // 2.2. Add zero
            else if (auxNode->left->isNodeNumber()) {
                if (((NodeNumber *)auxNode->left)->number == 0) {
                    return auxNode->right;
                }
            }
            else if (auxNode->right->isNodeNumber()) {
                if (((NodeNumber *)auxNode->right)->number == 0) {
                    return auxNode->left;
                }
            }
        }

        // 3. Multiply cases
        else if (auxNode->operation == '*') {
            // 3.1. Multiply equal nodes
            if (equal(auxNode->left, auxNode->right)) {
                auxNode->operation = '^';
                auxNode->right = new NodeNumber(2);
                return auxNode;
            }
            // 3.2. Multiply by one or zero on the left
            if (auxNode->left->isNodeNumber()) {
                if (((NodeNumber *)auxNode->left)->number == 1) {
                    node = auxNode->right;
                    return node;
                }
                else if (((NodeNumber *)auxNode->left)->number == 0) {
                    return new NodeNumber(0);
                }
            }
            // 3.3 Multiply by one or zero on the right
            if (auxNode->right->isNodeNumber()) {
                if (((NodeNumber *)auxNode->right)->number == 1) {
                    node = auxNode->left;
                    return node;
                }
                else if (((NodeNumber *)auxNode->right)->number == 0) {
                    return new NodeNumber(0);
                }
            }
        }
    }
    return node;
}

/// @brief Prints the AST as a polynomium
void AST::printAST() { 
    this->root->print();
    cout << endl;
}