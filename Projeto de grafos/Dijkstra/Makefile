CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++11

SRC = dijkstra.cpp
TARGET = dijkstra.bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
