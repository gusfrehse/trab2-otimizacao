
elenco : main.cpp
	g++ $(CXXFLAGS) -Wall -std=c++17 $^ -o $@

debug : CXXFLAGS = -g -DDEBUG
debug : elenco
