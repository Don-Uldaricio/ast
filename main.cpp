#include "AST.h"
#include "NodeOperation.h"

using namespace std;


int main() {
    AST *prueba = new AST("expr1.txt");
    prueba->printAST();
    //prueba->derivate(prueba->root, 'x');
    //prueba->printAST();
    //char var[2] = {'x', 'y'};
    //int values[2] = {3, 1};
    //prueba->replace(prueba->root, var, values, 2);
    //prueba->printAST();
    //prueba->root = prueba->evalRecursive(prueba->root);
    //prueba->printAST();
    prueba->root = prueba->sort(prueba->root);
    prueba->printAST();
    NodeOperation *r = (NodeOperation *)prueba->root;
    cout << prueba->equal(r->left, r->right);

}