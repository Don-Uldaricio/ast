testAST: main.cpp MenuAST.cpp AST.cpp Node.cpp NodeOperation.cpp NodeNumber.cpp NodeVariable.cpp
	g++ -o testAST main.cpp MenuAST.cpp AST.cpp Node.cpp NodeOperation.cpp NodeNumber.cpp NodeVariable.cpp -O2
runTestAST: testAST
	./testAST
clean:
	rm test*