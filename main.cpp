#include "AST.h"

using namespace std;


int main() {
    AST *prueba = new AST("expr1.txt");
    prueba->printAST();
    //prueba->derivate(prueba->root, 'x');
    //prueba->printAST();
    Node * nodo = prueba->evalRecursive(prueba->root);
    prueba->printAST();
}