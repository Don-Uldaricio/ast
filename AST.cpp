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

// sstream es un stream de caracteres
using namespace std;

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

    if (getline(file, line, '\n')) {
        //cout << line << endl;
        ss << line;
        // si stream ss es "+ * - 1 2 3"
        // el primer get line obtiene "+" 
        while(getline(ss, token, ' ')) {
            //cout << token << endl;

            // 1) creamos el nodo dependiendo del token
            if (isOperator(token)) {
                //cout << "Es un operador" << token << endl;
                node = new NodeOperation(token[0]);
                if (!pila.empty()) { 
                    ((NodeOperation*)node)->parent = pila.top();
                } else {
                    ((NodeOperation*)node)->parent = nullptr;
                }

            }
            else if (isNumber(token)) {
                //cout << "Es un numero: " << token << endl;
                int valor = stoi(token);
                node = new NodeNumber(valor);

            }
            else if (isVariable(token)) {
                //cout << "Es un identificador: " << token << endl;
                char name= token[0];
                node = new NodeVariable(name);
            }

            // 2) si es el primer nodo es la raiz
            if (this->root == nullptr) {
                this->root = node;
            }

            // 3) dependiendo del top del stack se agrega a la izquierda o derecha
            if (!pila.empty()) {
                // asignacion del padre
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

            // 4) si es un operador se agrega a la pila
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


bool AST::isOperator(string token) {
    return token == "+" || token == "-" || token == "*" || token == "^";
}

bool AST::isNumber(string token) {
    return token[0] >= '0' && token[0] <= '9';
}

bool AST::isVariable(string token) {
    return (token[0] >= 'a' && token[0] <= 'z') || (token[0] >= 'A' && token[0] <= 'Z');
}

bool AST::isNodeOperation(Node *node) {
    return node->type == OPERATOR;
}

bool AST::isNodeVariable(Node *node) {
    return node->type == VARIABLE;
}

bool AST::isNodeNumber(Node *node) {
    return node->type == NUMBER;
}

// Solo cuando ambos operandos son numeros
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

// solo evalua num op num y modifica el arbol!!!
Node *AST::evalRecursive(Node* node) {
    if (isNodeOperation(node)) {
            // evaluamos POSTORDER
        NodeOperation *op = (NodeOperation *)node;
        Node *l = op->left;
        Node *r = op->right;
        l = evalRecursive(l);
        r = evalRecursive(r);
        
        if (l->type == NUMBER && r->type == NUMBER) {
            NodeNumber* num = new NodeNumber(operate(op->operation, 
                                                       ((NodeNumber*)l)->number, 
                                                       ((NodeNumber*)r)->number));
            // actualizando el link al padre
            num->parent = op->parent;
            NodeOperation *parent = (NodeOperation *)op->parent;
            // actualizando el izq o derecho del padre
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
    else if (isNodeNumber(node) || isNodeVariable(node)) {
        return node;
    }
    else {
        cout << "Error: se encontro algo distinto a operador o numero o variable" << endl;
        exit(1);
    }
}

Node *AST::replace(Node *node, char *variables, int *values, int n) {
    if (isNodeOperation(node)) {
        NodeOperation *op = (NodeOperation *)node;
        op->left = replace(((NodeOperation *)node)->left, variables, values, n);
        op->right = replace(((NodeOperation *)node)->right, variables, values, n);
        return evalRecursive(op);
    }
    else if (isNodeVariable(node)) {
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
    else if (isNodeNumber(node)) {
        return node;
    }
    else {
        return nullptr;
    }
}

Node *AST::derivate(Node *node, char x) {
    if (isNodeOperation(node)) {
        char op = ((NodeOperation *)node)->operation;
        NodeOperation *oldRoot = (NodeOperation *)node;
        if (op == '+' || op == '-') {
            oldRoot->left = derivate(oldRoot->left, x);
            oldRoot->right = derivate(oldRoot->right, x);
            this->root = oldRoot;
            return evalRecursive(node);
        }
        else if (op == '*') {
            NodeOperation *newParent = new NodeOperation('+');
            NodeOperation *leftMult = oldRoot;
            NodeOperation *rightMult = new NodeOperation('*');

            newParent->left = leftMult;
            newParent->right = rightMult;
            
            Node *leftClone = clone(oldRoot->left);
            Node *rightClone = clone(oldRoot->right);

            rightMult->left = oldRoot->left;
            oldRoot->left = derivate(leftClone, x);
            rightMult->right = derivate(rightClone, x);
            
            leftMult->parent = newParent;
            rightMult->parent = newParent;
            leftClone->parent = node;
            leftClone->parent = rightMult;
            oldRoot->left->parent = rightMult;
            newParent->parent = leftMult->parent;
            
            this->root = newParent;

            return evalRecursive(newParent);
        }
        else if (op == '^') {
            Node *leftClone = clone(oldRoot->left);
            NodeOperation *newParent = new NodeOperation('*');
            NodeOperation *auxMult = new NodeOperation('*');
            NodeNumber *newExp = new NodeNumber((((NodeNumber *)oldRoot->right)->number) - 1);

            newParent->left = oldRoot->right;
            newParent->right = auxMult;
            auxMult->left = oldRoot;
            auxMult->right = derivate(leftClone, x);
            oldRoot->right = newExp;

            this->root = newParent;

            return evalRecursive(newParent);
        }
        else {
            return nullptr;
        }
    }
    else if (isNodeVariable(node)) {
        if (((NodeVariable *)node)->var == x) {
            Node *auxNode = new NodeNumber(1);
            return auxNode;
        }
        else {
            Node *auxNode = new NodeNumber(0);
            return auxNode;
        }
    }
    else if (isNodeNumber(node)) {
        Node *auxNode = new NodeNumber(0);
        return auxNode;
    }
    else {
        return nullptr;
    }
}

bool AST::equal(Node *n1, Node *n2) {
    if (isNodeOperation(n1) && isNodeOperation(n2)) {
        NodeOperation *aux1 = (NodeOperation *)n1;
        NodeOperation *aux2 = (NodeOperation *)n2;
        if (aux1->operation == aux2->operation) {
            return (equal(aux1->left, aux2->left) && equal(aux1->right, aux2->right));
        }
        else {
            return false;
        }
    }
    else if (isNodeNumber(n1) && isNodeNumber(n2)) {
        return ((NodeNumber *)n1)->number == ((NodeNumber *)n2)->number;
    }
    else if (isNodeVariable(n1) && isNodeVariable(n2)) {
        return ((NodeVariable *)n1)->var == ((NodeVariable *)n2)->var;
    }
    else {
        return false;
    }
}

Node *AST::sort(Node *node) {
    if (isNodeOperation(node)) {
        NodeOperation *auxNode = (NodeOperation *)node;
        if ((auxNode->operation == '*' || auxNode->operation == '+') && 
            isNodeVariable(auxNode->left) && isNodeNumber(auxNode->right)) {
            NodeVariable *auxVar = new NodeVariable(((NodeVariable *)auxNode->left)->var);
            auxNode->left = auxNode->right;
            auxNode->right = auxVar;
        }
        else if (isNodeOperation(auxNode->left) && isNodeNumber(auxNode->right) && 
                auxNode->operation == '*') {
            NodeNumber *auxNumber = new NodeNumber(((NodeNumber *)auxNode->right)->number);
            auxNode->right = sort(auxNode->left);
            auxNode->left = auxNumber;
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

Node *AST::reduceVariable(Node *node, char x) {

}

Node *AST::simplify(Node *node) {
    if (isNodeOperation(node)) {
        // Here are all conditions for simplify polynomiums
        NodeOperation *auxNode = (NodeOperation *)node;
        // Substracting equal nodes results in 0
        if (auxNode->operation == '-' && equal(auxNode->left, auxNode->right)) {
            return new NodeNumber(0);
        }

    }
    else {
        return node;
    }
}

Node *AST::clone(Node *node) {
    if (isNodeOperation(node)) {
        NodeOperation* auxNode = new NodeOperation(((NodeOperation *)node)->operation);
        auxNode->left = clone(((NodeOperation *)node)->left);
        auxNode->right = clone(((NodeOperation *)node)->right);
        return auxNode;
    }
    else if (isNodeVariable(node)) {
        NodeVariable *auxNode = new NodeVariable(((NodeVariable *)node)->var);
        return auxNode;
    }
    else if (isNodeNumber(node)) {
        NodeNumber *auxNode = new NodeNumber(((NodeNumber *)node)->number);
        return auxNode;
    }
    else {
        return nullptr;
    }
}

void AST::relinkNode(Node *child, Node *parent) {

}

void AST::printAST() { 
    this->root->print();
    cout << endl;
}