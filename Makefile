all:
	g++ -pthread main.cpp -o main

clean:
	rm -f main
