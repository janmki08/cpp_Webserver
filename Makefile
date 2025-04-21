# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall
TARGET = server
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
