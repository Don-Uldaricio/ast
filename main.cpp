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
    prueba->root = prueba->simplify(prueba->root);
    prueba->printAST();
    //cout << prueba->countVar(prueba->root, 'x') << endl;
    //prueba->root = prueba->reduceVariable(prueba->root, prueba->searchVariable(prueba->root, 'x'), prueba->countVar(prueba->root, 'x'),'x');
    //prueba->printAST();
    //prueba->root = prueba->evalRecursive(prueba->root);
    //prueba->printAST();
}