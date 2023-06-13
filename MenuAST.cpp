#include <vector>
#include "MenuAST.h"

using namespace std;

MenuAST::MenuAST(AST *ast) {
    this->ast = ast;
}

void MenuAST::runMenu() {
    bool quit = false;
    int option;
    Node *backupAST = this->ast->clone(this->ast->root);
    cout << "Welcome to AST" << endl;
    while (!quit) {
        cout << "Actual AST:\n\n";
        this->ast->printAST();
        cout << endl;
        cout << "1. Print polynomium" << endl;
        cout << "2. Evaluate AST" << endl;
        cout << "3. Replace variables" << endl;
        cout << "4. Derivate" << endl;
        cout << "5. Simplify AST" << endl;
        cout << "6. Back to original AST" << endl;
        cout << "7. Exit" << endl << endl;
        cout << "Enter an option: ";
        cin >> option;
        cout << endl;
        switch(option) {
            case 1:
                this->ast->printAST();
                cout << endl;
                break;
            case 2:
                this->ast->root = this->ast->evalRecursive(this->ast->root);
                break;
            case 3: {
                int nvar, value;
                char variable;
                cout << "Enter the number of variables: ";
                cin >> nvar;
                int values[nvar];
                char variables[nvar];
                for (int i = 0; i < nvar; i++) {
                    cout << "Enter the " << i+1 << "° variable: ";
                    cin >> variable;
                    variables[i] = variable;
                    cout << "Enter the value of " << i+1 << "° variable: ";
                    cin >> value;
                    values[i] = value;
                }
                this->ast->root = this->ast->replace(this->ast->root, variables, values, nvar);
                break;
            }
            case 4: {
                char x;
                cout << "Enter the derivate variable: ";
                cin >> x;
                this->ast->root = this->ast->derivate(this->ast->root, x);
                break;
            }
            case 5:
                this->ast->root = this->ast->simplify(this->ast->root);
                break;
            case 6:
                this->ast->root = backupAST;
                break;
            case 7:
                cout << "Exit program..." << endl;
                quit = true;
                break;
            default:
                cout << "Input error. Enter another value." << endl;
                break;
        }
    }
}