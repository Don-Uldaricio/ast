#include "AST.h"

using namespace std;


int main() {
    AST *prueba = new AST("expr1.txt");
    prueba->printAST();
    //prueba->derivate(prueba->root, 'x');
    //prueba->printAST();
    char var[2] = {'x', 'y'};
    int values[2] = {3, 1};
    prueba->replace(prueba->root, var, values, 2);
    prueba->printAST();
    prueba->root = prueba->evalRecursive(prueba->root);

}