CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++11

SRC = kosaraju.cpp
TARGET = kosaraju.bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
