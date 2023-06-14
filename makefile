main: main.cpp MenuAST.cpp AST.cpp Node.cpp NodeOperation.cpp NodeNumber.cpp NodeVariable.cpp
	g++ -o main.o main.cpp MenuAST.cpp AST.cpp Node.cpp NodeOperation.cpp NodeNumber.cpp NodeVariable.cpp -O2
runmain: main main.o
	./main.o
testNodeNumber: testNodeNumber.cpp NodeNumber.cpp Node.cpp
	g++ -o testNodeNumber.o testNodeNumber.cpp NodeNumber.cpp Node.cpp
runTestNodeNumber: testNodeNumber testNodeNumber.o
	./testNodeNumber.o
testNodeVariable: testNodeVariable.cpp NodeVariable.cpp Node.cpp
	g++ -o testNodeVariable.o testNodeVariable.cpp NodeVariable.cpp Node.cpp
runTestNodeVariable: testNodeVariable testNodeVariable.o
	./testNodeVariable.o
testNodeOperation: testNodeOperation.cpp NodeOperation.cpp NodeNumber.cpp NodeVariable.cpp Node.cpp
	g++ -o testNodeOperation.o testNodeOperation.cpp NodeOperation.cpp NodeNumber.cpp NodeVariable.cpp Node.cpp
runTestNodeOperation: testNodeOperation testNodeOperation.o
	./testNodeOperation.o
clean:
	rm *.o