#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "AST.h"

using namespace std;

/*
    * MenuAST class:
    * This class is used as a console interface to operate an AST
    * with all its methods.
*/
class MenuAST {
    public:
        // Attributes
        AST *ast;

        // Methods
        // Constructor

        MenuAST(AST *ast);

        // Other methods

        void runMenu();
};