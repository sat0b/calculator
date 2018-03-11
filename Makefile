CXXFLAGS := -std=c++14 -g -Wall 
CXX := clang++ $(CXXFLAGS)

calculator: calculator.cpp token.o parser.o stack.o lexer.o ast.o runner.o
	$(CXX) calculator.cpp token.o parser.o stack.o lexer.o ast.o runner.o -o calculator

token.o: token.cpp token.h
	$(CXX) -c token.cpp

lexer.o: lexer.cpp lexer.h
	$(CXX) -c lexer.cpp

parser.o: parser.cpp parser.h
	$(CXX) -c parser.cpp

stack.o: stack.cpp stack.h
	$(CXX) -c stack.cpp

ast.o: ast.cpp ast.h
	$(CXX) -c ast.cpp

runner.o: runner.cpp runner.h
	$(CXX) -c runner.cpp

.PHONY: test
test: test.sh calculator $(wildcard test/*.ca)
	./test.sh

clean:
	rm *.o
