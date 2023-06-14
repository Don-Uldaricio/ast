#include "MenuAST.h"
#include "NodeOperation.h"
#include "NodeVariable.h"
#include "NodeNumber.h"

using namespace std;


int main() {
    string fileName;
    cout << "Enter the file name: ";
    cin >> fileName;
    MenuAST *menu = new MenuAST(new AST(fileName));
    menu->runMenu();
}