.default: all

all: Pathfinder

clean:
	rm -f Pathfinder *.o

Pathfinder: Node.o NodeList.o PathPlanner.o main.o milestone4.o
	g++ -Werror -Wall -std=c++14  -O -o $@ $^

%.o: %.cpp
	g++ -Werror -Wall -std=c++14  -O -c $^
