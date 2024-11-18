# Makefile for C++ version

# Compiler and flags
CXX = g++
CXXFLAGS = -W -Wall -std=c++11 -lrt

# Target executable
TARGET = test_dht_read

# Source files and dependencies
SOURCES = test_dht_read.cpp pi_dht_read.cpp bcm2708.cpp realtime.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Build the target executable
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(CXXFLAGS) $^

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS)
