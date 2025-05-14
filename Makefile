# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
INCLUDES = -Iinclude
SRCS = main.cpp src/decode.cpp src/getfile.cpp src/handler.cpp src/request_parser.cpp src/response.cpp src/mime.cpp src/logger.cpp src/router.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = build/server

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(SRCS)

clean:
	rm -f $(TARGET) $(OBJS)
