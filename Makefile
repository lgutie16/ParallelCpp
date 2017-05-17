runme: ctrsis.cpp ctrsis.o
	 g++ ./src/ctrsis.cpp ctrsis.o -o runme

ctrsis.o: ctreval.cpp evaluator.o
	g++ ./src/ctreval.cpp evaluator.o -o ctrsis.o

evaluator.o: evaluator.cpp 
	g++ ./src/evaluator.cpp -o evaluator.o