CXXFLAGS := -std=c++14 -Wall
CXX := clang++ $(CXXFLAGS)

calculator: calculator.cpp token.o
	$(CXX) calculator.cpp token.o -o calculator

token.o: token.cpp token.h
	$(CXX) -c token.cpp

test: test.sh calculator
	./test.sh
