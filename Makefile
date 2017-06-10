TARGET = /home/box/shell
SOURCE = /home/box/main.cpp

all: $(TARGET)

shell: $(SOURCE)
	g++ -std=c++11 -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) result.out
