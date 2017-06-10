all: work

work:
	g++ -Wall -std=c++11 -o work work.cpp

clean:
	rm -f work
