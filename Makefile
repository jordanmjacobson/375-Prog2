FLAGS = -Wall -Wextra -DDebug -g -std=c++14

all: BestFirstSearch

BestFirstSearch.o: BestFirstSearch.cpp
	g++ $(FLAGS) -c BestFirstSearch.cpp -o BestFirstSearch.o

BestFirstSearch: BestFirstSearch.o
	g++ $(FLAGS) BestFirstSearch.o -o BestFirstSearch

run: all
	./BestFirstSearch input.txt output.txt

clean:
	rm -rf BestFirstSearch BestFirstSearch.o output.txt
