#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "AST.h"

using namespace std;

class MenuAST {
    public:
        AST *ast;

        MenuAST(AST *ast);
        void runMenu();
};