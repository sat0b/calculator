CXXFLAGS := -std=c++14 -Wall -g
CXX := clang++ $(CXXFLAGS)

calculator: calculator.cpp token.o parser.o stack.o lexer.o
	$(CXX) calculator.cpp token.o parser.o stack.o lexer.o -o calculator

token.o: token.cpp token.h
	$(CXX) -c token.cpp

lexer.o: lexer.cpp lexer.h
	$(CXX) -c lexer.cpp

parser.o: parser.cpp parser.h
	$(CXX) -c parser.cpp

stack.o: stack.cpp stack.h
	$(CXX) -c stack.cpp

.PHONY: test
test: test.sh calculator $(wildcard test/*.ca)
	./test.sh
