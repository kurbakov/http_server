CC = g++
CFLAGS = -Wall -std=c++11

SOURCES = final.cpp http_server.cpp
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = final

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

final.o: final.cpp
	$(CC) $(CFLAGS) -c $< -o $@

http_server.o: http_server.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) *.o