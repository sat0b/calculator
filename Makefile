CXXFLAGS := -std=c++14 -Wall -g
CXX := clang++ $(CXXFLAGS)

calculator: calculator.cpp token.o parser.o stack.o
	$(CXX) calculator.cpp token.o parser.o stack.o -o calculator

token.o: token.cpp token.h
	$(CXX) -c token.cpp

parser.o: parser.cpp parser.h
	$(CXX) -c parser.cpp

stack.o: stack.cpp stack.h
	$(CXX) -c stack.cpp

test: test.sh calculator
	./test.sh
