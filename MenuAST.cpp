#include <string>
#include "MenuAST.h"

using namespace std;

/// @brief Constructor of MenuAST class that asignes a AST to the ast attribute.
/// @param ast AST to asigned.
MenuAST::MenuAST(AST *ast) {
    this->ast = ast;
}

/// @brief Console menu to interact with the AST and its methods.
void MenuAST::runMenu() {
    bool quit = false;
    int option;
    Node *backupAST = this->ast->root->clone();
    cout << endl << "Welcome to AST" << endl;
    while (!quit) {
        cout << endl << "Actual AST:\n";
        this->ast->printAST();
        cout << endl;
        cout << "1. Print tree form" << endl;
        cout << "2. Evaluate AST" << endl;
        cout << "3. Replace variables" << endl;
        cout << "4. Derivate" << endl;
        cout << "5. Simplify AST" << endl;
        cout << "6. Back to original AST" << endl;
        cout << "7. Exit" << endl << endl;
        cout << "Enter an option: ";
        cin >> option;
        switch(option) {
            case 1:
                this->ast->printAST();
                cout << endl;
                break;
            case 2:
                this->ast->root = this->ast->evalRecursive(this->ast->root);
                break;
            case 3: {
                int value;
                int values[this->ast->nvars];
                for (int i = 0; i < this->ast->nvars; i++) {
                    cout << "Enter a value for " << this->ast->vars[i] << ": ";
                    cin >> value;
                    values[i] = value;
                }
                this->ast->root = this->ast->replace(this->ast->root, this->ast->vars, values, this->ast->nvars);
                cout << "Replacing..." << endl;
                break;
            }
            case 4: {
                char x;
                cout << "Enter the variable to derive: ";
                cin >> x;
                cout << "Deriving..." << endl;
                this->ast->root = this->ast->derive(this->ast->root, x);
                break;
            }
            case 5:
                this->ast->root = this->ast->simplify(this->ast->root);
                cout << "Simplifying..." << endl;
                break;
            case 6:
            cout << "Backing to original AST..." << endl;
                this->ast->root = backupAST;
                break;
            case 7:
                cout << "See you later! :)" << endl;
                quit = true;
                break;
            default:
                cout << "Input error. Enter another value." << endl;
                break;
        }
    }
}