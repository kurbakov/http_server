all:
	g++ -Wall -std=c++11 -o main main.cpp

clean:
	rm -f main in.fifo out.fifo
