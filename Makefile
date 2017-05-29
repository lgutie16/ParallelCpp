all: main clean init

main: src/ctrsis.cpp src/ctreval.cpp src/evaluator.cpp
	g++ -pthread src/ctrsis.cpp -o ctrsis -std=c++11
	g++ -pthread -o ctreval src/ctreval.cpp
	g++ -pthread -o evaluator src/evaluator.cpp 

clean:
	rm  src/ctrsis
	rm  src/ctreval
	rm  src/evaluator

init:
	mv ctrsis src/
	mv ctreval src/
	mv evaluator src/	